import CSTM32F4
import Hardware

extension STM32F4 {
    public var uart3: UART {
        getOrCreateResource(identifier: "uart_3",
                            create: UART(address: USART3_BASE,
                                         enableClock: m__HAL_RCC_USART3_CLK_ENABLE,
                                         rxPin: gpio.pin(peripheral: .D, number: 9),
                                         txPin: gpio.pin(peripheral: .D, number: 8)))
    }
}

public final class UART {
    var handle: UnsafeMutablePointer<UART_HandleTypeDef>
    let enableClock: () -> Void
    let rxPin: GPIO.Pin
    let txPin: GPIO.Pin

    fileprivate init(address: UInt32,
                     enableClock: @escaping () -> Void,
                     rxPin: GPIO.Pin,
                     txPin: GPIO.Pin) {
        handle = UnsafeMutablePointer<UART_HandleTypeDef>.allocate(capacity: 1)
        handle.pointee = UART_HandleTypeDef()
        handle.pointee.Instance = UnsafeMutablePointer<USART_TypeDef>(bitPattern: UInt(address))
        self.enableClock = enableClock
        self.txPin = txPin
        self.rxPin = rxPin
    }

    deinit {
        self.handle.deallocate()
    }

    public enum Parity {
        case none
        case even
        case odd
    }

    public struct Configuration {
        var baudrate: Int
        var parity: Parity

        public init(baudrate: Int = 9600, parity: Parity = .none) {
            self.baudrate = baudrate
            self.parity = parity
        }
    }

    public func configure(_ config: Configuration) throws {
        // enable peripheral's clock
        enableClock()
        // configure gpio
        let pinConfig = GPIO_InitTypeDef(Pin: 0,
                                         Mode: UInt32(GPIO_MODE_AF_PP),
                                         Pull: GPIO_PULLUP,
                                         Speed: GPIO_SPEED_FREQ_VERY_HIGH,
                                         Alternate: UInt32(GPIO_AF7_USART3))
        rxPin.configure(.manual(hal: pinConfig))
        txPin.configure(.manual(hal: pinConfig))
        // prepare init struct
        handle.pointee.Init = config.toHAL()
        // init the peripheral
        try HAL_UART_Init(handle).throwOnFailure()

        HAL_NVIC_SetPriority(USART3_IRQn, 3, 0)
        HAL_NVIC_EnableIRQ(USART3_IRQn)
    }
}

extension UART: Hardware.UART {
    public func write(_ buffer: UnsafeBufferPointer<UInt8>, timeout _: TimeInterval) throws {
        try _HAL_UART_Transmit(handle, buffer.baseAddress, UInt16(buffer.count),
                               UInt32(5000)).throwOnFailure()
    }

    public func read(into buffer: UnsafeMutableBufferPointer<UInt8>, timeout: TimeInterval) throws -> Int {
        if timeout == 0 {
            do {
                try HAL_UART_Receive(handle, buffer.baseAddress, 1, 0)
                    .throwOnFailure()
                return 1
            } catch STM32F4Error.timeout {
                return 0
            }
        } else {
            try HAL_UART_Receive(handle, buffer.baseAddress, UInt16(buffer.count), UInt32(timeout))
                .throwOnFailure()
            return buffer.count
        }
    }
}

extension UART.Parity {
    func toHAL() -> UInt32 {
        switch self {
        case .none: return UART_PARITY_NONE
        case .odd: return USART_CR1_PCE | USART_CR1_PS
        case .even: return UART_PARITY_EVEN
        }
    }
}

extension UART.Configuration {
    func toHAL() -> UART_InitTypeDef {
        UART_InitTypeDef(BaudRate: UInt32(baudrate),
                         WordLength: UART_WORDLENGTH_8B,
                         StopBits: UART_STOPBITS_1,
                         Parity: parity.toHAL(),
                         Mode: UART_MODE_RX | UART_MODE_TX,
                         HwFlowCtl: UART_HWCONTROL_NONE,
                         OverSampling: UART_OVERSAMPLING_16)
    }
}
