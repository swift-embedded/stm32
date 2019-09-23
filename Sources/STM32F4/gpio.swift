import CSTM32F4
import Hardware

extension STM32F4 {
    public var gpio: GPIO {
        return getOrCreateResource(identifier: "gpio", create: GPIO())
    }
}

public final class GPIO {
    public typealias InterruptHandler = () -> Void

    public func pin(peripheral: Peripheral, number: Int, mode: Pin.Mode? = nil) -> Pin {
        return peripheral.pin(number: number, mode: mode)
    }

    public final class Pin {
        public enum Pull {
            case up
            case down
            case no
        }

        public struct Edge: OptionSet {
            public let rawValue: UInt32

            public init(rawValue: UInt32) {
                self.rawValue = rawValue
            }

            public static let rising = Edge(rawValue: GPIO_MODE_IT_RISING)
            public static let falling = Edge(rawValue: GPIO_MODE_IT_FALLING)
        }

        public enum Mode {
            case input(pull: Pull)
            case output
            case interrupt(edge: Edge, pull: Pull, handler: InterruptHandler)
            case manual(hal: GPIO_InitTypeDef)
        }

        public let peripheral: Peripheral

        public let number: Int

        public private(set) var mode: Mode?

        @inlinable
        var numberHal: UInt32 { return 1 << number }

        fileprivate init(peripheral: GPIO.Peripheral, number: Int, mode: Mode? = nil) {
            self.peripheral = peripheral
            self.number = number
            if let mode = mode {
                configure(mode)
            }
        }

        public func configure(_ mode: Mode) {
            // start the peripheral's clock
            peripheral.enableClock()
            // prepare the init struct
            var config: GPIO_InitTypeDef
            switch mode {
            case let .input(pull):
                config = GPIO_InitTypeDef(
                    Pin: numberHal, Mode: UInt32(GPIO_MODE_INPUT),
                    Pull: pull.hal, Speed: GPIO_SPEED_FREQ_LOW,
                    Alternate: 0
                )
            case .output:
                config = GPIO_InitTypeDef(
                    Pin: numberHal, Mode: UInt32(GPIO_MODE_OUTPUT_PP),
                    Pull: GPIO_NOPULL, Speed: GPIO_SPEED_FREQ_VERY_HIGH,
                    Alternate: 0
                )
            case let .interrupt(edge, pull, handler):
                config = GPIO_InitTypeDef(
                    Pin: numberHal, Mode: edge.rawValue,
                    Pull: pull.hal, Speed: GPIO_SPEED_FREQ_VERY_HIGH,
                    Alternate: 0
                )
                interruptHandlers[self.number] = handler
                HAL_NVIC_SetPriority(interruptNumber, 3, 0)
                HAL_NVIC_EnableIRQ(interruptNumber)
            case let .manual(hal):
                config = hal
                config.Pin = numberHal
            }
            // init the pin
            HAL_GPIO_Init(peripheral.ptr, &config)
            self.mode = mode
        }

        public func unconfigure() {
            HAL_GPIO_DeInit(peripheral.ptr, numberHal)
            mode = nil
        }

        private var interruptNumber: IRQn_Type {
            switch number {
            case 0: return EXTI0_IRQn
            case 1: return EXTI1_IRQn
            case 2: return EXTI2_IRQn
            case 3: return EXTI3_IRQn
            case 4: return EXTI4_IRQn
            case 5 ... 9: return EXTI9_5_IRQn
            case 10 ... 15: return EXTI15_10_IRQn
            default: fatalError("invalid pin number")
            }
        }
    }

    public final class Peripheral {
        @usableFromInline
        let ptr: UnsafeMutablePointer<GPIO_TypeDef>

        private var pins: [Pin?] = [Pin?](repeating: nil, count: 16)

        fileprivate let enableClock: () -> Void

        fileprivate init(address: UInt32, enableClock: @escaping @autoclosure () -> Void) {
            ptr = UnsafeMutablePointer<GPIO_TypeDef>(bitPattern: UInt(address))!
            self.enableClock = enableClock
        }

        public func pin(number: Int, mode: Pin.Mode? = nil) -> Pin {
            precondition(number >= 0 && number <= 16, "invalid pin number")
            if let pin = self.pins[number] {
                return pin
            } else {
                let pin = Pin(peripheral: self, number: number, mode: mode)
                pins[number] = pin
                return pin
            }
        }

        public static let A = Peripheral(address: GPIOA_BASE,
                                         enableClock: m__HAL_RCC_GPIOA_CLK_ENABLE())
        public static let B = Peripheral(address: GPIOB_BASE,
                                         enableClock: m__HAL_RCC_GPIOB_CLK_ENABLE())
        public static let C = Peripheral(address: GPIOC_BASE,
                                         enableClock: m__HAL_RCC_GPIOC_CLK_ENABLE())
        public static let D = Peripheral(address: GPIOD_BASE,
                                         enableClock: m__HAL_RCC_GPIOD_CLK_ENABLE())
        public static let E = Peripheral(address: GPIOE_BASE,
                                         enableClock: m__HAL_RCC_GPIOE_CLK_ENABLE())
        public static let F = Peripheral(address: GPIOF_BASE,
                                         enableClock: m__HAL_RCC_GPIOF_CLK_ENABLE())
        public static let G = Peripheral(address: GPIOG_BASE,
                                         enableClock: m__HAL_RCC_GPIOG_CLK_ENABLE())
        public static let H = Peripheral(address: GPIOH_BASE,
                                         enableClock: m__HAL_RCC_GPIOH_CLK_ENABLE())
        public static let I = Peripheral(address: GPIOI_BASE,
                                         enableClock: m__HAL_RCC_GPIOI_CLK_ENABLE())
        public static let J = Peripheral(address: GPIOJ_BASE,
                                         enableClock: m__HAL_RCC_GPIOJ_CLK_ENABLE())
        public static let K = Peripheral(address: GPIOK_BASE,
                                         enableClock: m__HAL_RCC_GPIOK_CLK_ENABLE())
    }
}

extension GPIO.Pin: DigitalIn {
    @inlinable
    public func get() -> PinState {
        return PinState(hal: HAL_GPIO_ReadPin(peripheral.ptr, UInt16(numberHal)))
    }
}

extension GPIO.Pin: DigitalOut {
    @inlinable
    public func set(_ value: PinState) {
        HAL_GPIO_WritePin(peripheral.ptr, UInt16(numberHal), value.hal)
    }
}

extension GPIO.Pin: ToggleableDigitalOut {
    @inlinable
    public func toggle() {
        HAL_GPIO_TogglePin(peripheral.ptr, UInt16(numberHal))
    }
}

private var interruptHandlers = [GPIO.InterruptHandler?](repeating: nil, count: 16)

private func pinNumberHalToPinNumber(_ numberHal: UInt16) -> Int {
    var numberHal: UInt16 = numberHal
    var number: Int = 0
    while numberHal != 1 {
        numberHal = numberHal >> 1
        number += 1
    }
    return number
}

@_silgen_name("HAL_GPIO_EXTI_Callback")
internal func HAL_GPIO_EXTI_Callback(pinNumberHal: UInt16) {
    let pinNumber = pinNumberHalToPinNumber(pinNumberHal)
    interruptHandlers[pinNumber]?()
}

extension PinState {
    @inlinable
    init(hal: GPIO_PinState) {
        if hal.rawValue > 0 {
            self = .high
        } else {
            self = .low
        }
    }

    @inlinable
    var hal: GPIO_PinState {
        return GPIO_PinState(self == .high ? 1 : 0)
    }
}

extension GPIO.Pin.Pull {
    fileprivate var hal: UInt32 {
        switch self {
        case .up: return GPIO_PULLUP
        case .down: return GPIO_PULLDOWN
        case .no: return GPIO_NOPULL
        }
    }
}
