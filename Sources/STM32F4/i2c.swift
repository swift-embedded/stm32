import CSTM32F4
import Hardware

extension STM32F4 {
    public var i2c: I2C {
        return getOrCreateResource(identifier: "i2c_1",
                                   create: I2C(address: I2C1_BASE,
                                               enableClock: m__HAL_RCC_I2C1_CLK_ENABLE))
    }

    public var i2c_2: I2C {
        return getOrCreateResource(identifier: "i2c_2",
                                   create: I2C(address: I2C2_BASE,
                                               enableClock: m__HAL_RCC_I2C2_CLK_ENABLE))
    }
}

public final class I2C {
    @usableFromInline
    var handle: UnsafeMutablePointer<I2C_HandleTypeDef>

    let enableClock: () -> Void

    var timeoutMs = 1000

    fileprivate init(address: UInt32, enableClock: @escaping () -> Void) {
        handle = UnsafeMutablePointer<I2C_HandleTypeDef>.allocate(capacity: 1)
        handle.initialize(repeating: I2C_HandleTypeDef(), count: 0)
        handle.pointee.Instance = UnsafeMutablePointer<I2C_TypeDef>(bitPattern: UInt(address))!
        self.enableClock = enableClock
    }

    deinit {
        self.handle.deallocate()
    }

    public func configure(speed: Int, clockPin: GPIO.Pin, dataPin: GPIO.Pin) throws {
        // connect pins
        let pinConfig = GPIO_InitTypeDef(Pin: 0, Mode: UInt32(GPIO_MODE_AF_OD),
                                         Pull: GPIO_PULLUP, Speed: GPIO_SPEED_FREQ_HIGH,
                                         Alternate: UInt32(GPIO_AF4_I2C1))
        clockPin.configure(.manual(hal: pinConfig))
        dataPin.configure(.manual(hal: pinConfig))
        enableClock()
        // init i2c component
        var initInfo = I2C_InitTypeDef()
        initInfo.ClockSpeed = UInt32(speed)
        initInfo.DutyCycle = I2C_DUTYCYCLE_2
        initInfo.AddressingMode = I2C_ADDRESSINGMODE_7BIT
        initInfo.DualAddressMode = I2C_DUALADDRESS_DISABLE
        initInfo.GeneralCallMode = I2C_GENERALCALL_DISABLE
        initInfo.NoStretchMode = I2C_NOSTRETCH_DISABLE
        handle.pointee.Init = initInfo
        try HAL_I2C_Init(handle).throwOnFailure()
    }
}

extension I2C: Hardware.I2C {
    public func read(address: Int, length: Int, stop: Bool) throws -> [UInt8] {
        precondition(stop, "I2C.write(..., stop=false) not supported")
        let buffer = UnsafeMutableBufferPointer<UInt8>.allocate(capacity: length)
        defer { buffer.deallocate() }
        let result = HAL_I2C_Master_Receive(handle, UInt16(address) << 1,
                                            buffer.baseAddress,
                                            UInt16(length), UInt32(timeoutMs))
        try result.throwOnFailure()
        return Array(buffer)
    }

    public func write(address: Int, data: [UInt8], stop: Bool) throws {
        precondition(stop, "I2C.write(..., stop=false) not supported")
        var data = data
        let result = data.withUnsafeMutableBufferPointer { buffer in
            HAL_I2C_Master_Transmit(self.handle, UInt16(address) << 1,
                                    buffer.baseAddress,
                                    UInt16(buffer.count), UInt32(self.timeoutMs))
        }
        try result.throwOnFailure()
    }
}

extension I2C: Hardware.I2CMemory {
    public func read(address: Int, register: UInt8, length: Int) throws -> [UInt8] {
        let buffer = UnsafeMutableBufferPointer<UInt8>.allocate(capacity: length)
        defer { buffer.deallocate() }
        let result = HAL_I2C_Mem_Read(handle, UInt16(address) << 1, UInt16(register),
                                      UInt16(I2C_MEMADD_SIZE_8BIT), buffer.baseAddress,
                                      UInt16(length), UInt32(timeoutMs))
        try result.throwOnFailure()
        return Array(buffer)
    }

    public func write(address: Int, register: UInt8, data: [UInt8]) throws {
        var data = data
        let result = data.withUnsafeMutableBufferPointer { buffer in
            HAL_I2C_Mem_Write(self.handle, UInt16(address) << 1, UInt16(register),
                              UInt16(I2C_MEMADD_SIZE_8BIT), buffer.baseAddress,
                              UInt16(buffer.count), UInt32(self.timeoutMs))
        }
        try result.throwOnFailure()
    }
}
