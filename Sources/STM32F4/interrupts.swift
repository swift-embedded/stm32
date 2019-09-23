import CSTM32F4

public typealias SysTickHandler = () -> Void

private var systickHandler: SysTickHandler?

public func registerSysTickHandler(_ handler: @escaping SysTickHandler) {
    systickHandler = handler
}

@_silgen_name("SysTick_Handler")
func SysTick_Handler() {
    HAL_IncTick()
    HAL_SYSTICK_IRQHandler()
}

@_silgen_name("HAL_SYSTICK_Callback")
func SysTick_Handler_User() {
    if let handler = systickHandler {
        handler()
    }
}

@_silgen_name("EXTI0_IRQHandler")
func EXTI0_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(1 << 0)
}

@_silgen_name("EXTI1_IRQHandler")
internal func EXTI1_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(1 << 1)
}

@_silgen_name("EXTI2_IRQHandler")
internal func EXTI2_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(1 << 2)
}

@_silgen_name("EXTI3_IRQHandler")
internal func EXTI3_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(1 << 3)
}

@_silgen_name("EXTI4_IRQHandler")
internal func EXTI4_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(1 << 4)
}

@_silgen_name("EXTI9_5_IRQHandler")
internal func EXTI9_5_IRQHandler() {
    for number in 5 ... 9 {
        HAL_GPIO_EXTI_IRQHandler(1 << number)
    }
}

@_silgen_name("EXTI15_10_IRQHandler")
internal func EXTI15_10_IRQHandler() {
    for number in 10 ... 15 {
        HAL_GPIO_EXTI_IRQHandler(1 << number)
    }
}
