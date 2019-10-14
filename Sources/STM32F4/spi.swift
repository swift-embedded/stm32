import CSTM32F4
import Hardware

extension STM32F4 {
    public var spi: SPI {
        getOrCreateResource(
            identifier: "spi_1",
            create: SPI(address: SPI1_BASE,
                        enableClock: m__HAL_RCC_SPI1_CLK_ENABLE,
                        clockFrequencyGetter: HAL_RCC_GetPCLK2Freq)
        )
    }

    public var spi2: SPI {
        getOrCreateResource(
            identifier: "spi_2",
            create: SPI(address: SPI2_BASE,
                        enableClock: m__HAL_RCC_SPI2_CLK_ENABLE,
                        clockFrequencyGetter: HAL_RCC_GetPCLK1Freq)
        )
    }

    public var spi3: SPI {
        getOrCreateResource(
            identifier: "spi_3",
            create: SPI(address: SPI3_BASE,
                        enableClock: m__HAL_RCC_SPI3_CLK_ENABLE,
                        clockFrequencyGetter: HAL_RCC_GetPCLK1Freq)
        )
    }
}

public final class SPI {
    var handle: UnsafeMutablePointer<SPI_HandleTypeDef>

    let enableClock: () -> Void
    let getClockFrequency: () -> UInt32

    public enum Frequency {
        case prescaler(Int)
        case baudrate(Int)
    }

    public enum Mode {
        case slave
        case master(frequency: Frequency)
    }

    public enum Direction {
        case twoLines
        case twoLinesReadOnly
        case oneLine
    }

    public enum DataSize {
        case eightBits
        case sixteenBits
    }

    public enum Polarity {
        case low
        case high
    }

    public enum Phase {
        case firstEdge
        case secondEdge
    }

    public enum FirstBit {
        case msb
        case lsb
    }

    public enum CRC {
        case disabled
        case polynomial(UInt16)
    }

    public enum SlaveSelect {
        case soft
        case hardInput(GPIO.Pin)
        case hardOuptut(GPIO.Pin)
    }

    public struct Configuration {
        var sck: GPIO.Pin
        var miso: GPIO.Pin
        var mosi: GPIO.Pin
        var mode: Mode = .master(frequency: .baudrate(328_125))
        var direction: Direction = .twoLines
        var dataSize: DataSize = .eightBits
        var polarity: Polarity = .high
        var phase: Phase = .firstEdge
        var firstBit: FirstBit = .msb
        var crc: CRC = .disabled
        var tiMode = false

        public init(sck: GPIO.Pin, miso: GPIO.Pin, mosi: GPIO.Pin) {
            self.sck = sck
            self.miso = miso
            self.mosi = mosi
        }
    }

    fileprivate init(address: UInt32,
                     enableClock: @escaping () -> Void,
                     clockFrequencyGetter: @escaping () -> UInt32) {
        handle = UnsafeMutablePointer<SPI_HandleTypeDef>.allocate(capacity: 1)
        handle.initialize(repeating: SPI_HandleTypeDef(), count: 1)
        handle.pointee.Instance = UnsafeMutablePointer<SPI_TypeDef>(
            bitPattern: UInt(address)
        )!
        self.enableClock = enableClock
        getClockFrequency = clockFrequencyGetter
    }

    deinit {
        self.handle.deallocate()
    }

    public func configure(_ config: Configuration) throws {
        // connect pins
        // TODO: GPIO_AF6_SPI3
        let pinConfig = GPIO_InitTypeDef(Pin: 0, Mode: UInt32(GPIO_MODE_AF_PP),
                                         Pull: GPIO_NOPULL, Speed: GPIO_SPEED_FREQ_HIGH,
                                         Alternate: UInt32(GPIO_AF6_SPI3))
        config.sck.configure(.manual(hal: pinConfig))
        config.mosi.configure(.manual(hal: pinConfig))
        config.miso.configure(.manual(hal: pinConfig))
        enableClock()

        handle.pointee.Init = config.toHAL(
            peripheralClockFrequency: Int(getClockFrequency())
        )

        try HAL_SPI_Init(handle).throwOnFailure()
    }
}

extension SPI: Hardware.SPI {
    public func send(_ buffer: UnsafeBufferPointer<UInt8>, timeout _: TimeInterval) throws {
        try _HAL_SPI_Transmit(handle, buffer.baseAddress, UInt16(buffer.count),
                              UInt32(5000)).throwOnFailure()
        while HAL_SPI_GetState(handle) != HAL_SPI_STATE_READY {}
    }
}

extension SPI.Configuration {
    func toHAL(peripheralClockFrequency: Int) -> SPI_InitTypeDef {
        var config = SPI_InitTypeDef()

        switch mode {
        case let .master(frequency):
            config.Mode = m_SPI_MODE_MASTER
            config.BaudRatePrescaler = selectPrescaler(frequency,
                                                       peripheralClockFrequency: peripheralClockFrequency)
        case .slave:
            config.Mode = SPI_MODE_SLAVE
        }

        switch direction {
        case .twoLines:
            config.Direction = SPI_DIRECTION_2LINES
        case .twoLinesReadOnly:
            config.Direction = SPI_DIRECTION_2LINES_RXONLY
        case .oneLine:
            config.Direction = SPI_DIRECTION_1LINE
        }

        switch dataSize {
        case .eightBits:
            config.DataSize = SPI_DATASIZE_8BIT
        case .sixteenBits:
            config.DataSize = SPI_DATASIZE_16BIT
        }

        switch polarity {
        case .low:
            config.CLKPolarity = SPI_POLARITY_LOW
        case .high:
            config.CLKPolarity = SPI_POLARITY_HIGH
        }

        switch phase {
        case .firstEdge:
            config.CLKPhase = SPI_PHASE_1EDGE
        case .secondEdge:
            config.CLKPhase = SPI_PHASE_2EDGE
        }

        switch firstBit {
        case .msb:
            config.FirstBit = SPI_FIRSTBIT_MSB
        case .lsb:
            config.FirstBit = SPI_FIRSTBIT_LSB
        }

        switch crc {
        case .disabled:
            config.CRCCalculation = SPI_CRCCALCULATION_DISABLE
        case let .polynomial(n):
            config.CRCCalculation = SPI_CRCCALCULATION_ENABLE
            config.CRCPolynomial = UInt32(n)
        }

        switch tiMode {
        case false:
            config.TIMode = SPI_TIMODE_DISABLE
        case true:
            config.TIMode = SPI_TIMODE_ENABLE
        }

        config.NSS = SPI_NSS_SOFT

        return config
    }

    func selectPrescaler(_ frequency: SPI.Frequency, peripheralClockFrequency: Int) -> UInt32 {
        let prescale: Int
        switch frequency {
        case let .baudrate(baudrate):
            prescale = (peripheralClockFrequency + baudrate - 2) / baudrate
        case let .prescaler(specified):
            prescale = specified
        }

        if prescale <= 2 { return SPI_BAUDRATEPRESCALER_2 }
        else if prescale <= 4 { return SPI_BAUDRATEPRESCALER_4 }
        else if prescale <= 8 { return SPI_BAUDRATEPRESCALER_8 }
        else if prescale <= 16 { return SPI_BAUDRATEPRESCALER_16 }
        else if prescale <= 32 { return SPI_BAUDRATEPRESCALER_32 }
        else if prescale <= 64 { return SPI_BAUDRATEPRESCALER_64 }
        else if prescale <= 128 { return SPI_BAUDRATEPRESCALER_128 }
        else { return SPI_BAUDRATEPRESCALER_256 }
    }
}
