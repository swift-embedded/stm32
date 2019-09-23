import CSTM32F4

public enum STM32F4Error: Error {
    case unknownError
    case busy
    case timeout
}

public class STM32F4 {
    internal var resources: [String: Any] = [:]

    internal func getOrCreateResource<T>(identifier: String, create: @autoclosure () -> T) -> T {
        if let resource = self.resources[identifier] {
            return resource as! T
        } else {
            let resource = create()
            resources[identifier] = resource
            return resource
        }
    }

    public init() throws {
        try Clock.setup()
        try HAL_Init().throwOnFailure()
    }

    public func delay(ms: Int) {
        HAL_Delay(ms > 0 ? UInt32(ms) : 0)
    }

    public var systemClockFrequency: Int {
        return Int(HAL_RCC_GetSysClockFreq())
    }

    public var tick: UInt32 {
        return HAL_GetTick()
    }

    deinit {
        try! HAL_DeInit().throwOnFailure()
    }

    public static var version: (Int, Int, Int) {
        let ver = HAL_GetHalVersion()
        let major = Int((ver >> 24) & 0x0F)
        let minor = Int((ver >> 16) & 0x0F)
        let build = Int((ver >> 8) & 0x0F)
        return (major, minor, build)
    }
}

private var criticalSectionCounter = 0

public func criticalSection<RetVal>(_ f: () throws -> RetVal) rethrows -> RetVal {
    __disable_irq()
    criticalSectionCounter += 1
    defer {
        criticalSectionCounter -= 1
        if criticalSectionCounter == 0 {
            __enable_irq()
        }
    }
    return try f()
}

@_silgen_name("_sleep_ms")
func _sleep_ms(ms: UInt32) {
    HAL_Delay(ms)
}

extension HAL_StatusTypeDef {
    func throwOnFailure() throws {
        switch self {
        case HAL_OK:
            return
        case HAL_ERROR:
            throw STM32F4Error.unknownError
        case HAL_BUSY:
            throw STM32F4Error.busy
        case HAL_TIMEOUT:
            throw STM32F4Error.timeout
        default:
            fatalError("unknown hal status")
        }
    }
}
