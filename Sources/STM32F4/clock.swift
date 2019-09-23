import CSTM32F4

final class Clock {
    static func setup() throws {
        m__PWR_CLK_ENABLE()
        m__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1)

        var oscInit = RCC_OscInitTypeDef()
        oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE
        oscInit.HSEState = UInt32(RCC_CR_HSEBYP | RCC_CR_HSEON)
        oscInit.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT
        oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE
        oscInit.PLL.PLLState = UInt32(RCC_PLL_ON)
        oscInit.PLL.PLLM = 8
        oscInit.PLL.PLLN = 336
        oscInit.PLL.PLLP = RCC_PLLP_DIV2
        oscInit.PLL.PLLQ = 7
        try HAL_RCC_OscConfig(&oscInit).throwOnFailure()

        try HAL_PWREx_EnableOverDrive().throwOnFailure()

        var clkInit = RCC_ClkInitTypeDef()
        clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
        clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK
        clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1
        clkInit.APB1CLKDivider = RCC_HCLK_DIV4
        clkInit.APB2CLKDivider = RCC_HCLK_DIV2
        try HAL_RCC_ClockConfig(&clkInit, FLASH_LATENCY_5).throwOnFailure()

        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000)
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK)
    }
}
