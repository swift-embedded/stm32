#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_i2c.h"

#define EXPORT_MACRO_ARG0(rettype, name) \
    static inline rettype m##name(void) { name(); }

#define EXPORT_MACRO_ARG1(rettype, name, arg1type) \
    static inline rettype m##name(arg1type arg1) { name(arg1); }

#define EXPORT_MACRO_CONST(type, name) \
    static const type m_##name = name;

EXPORT_MACRO_CONST(uint32_t, SPI_MODE_MASTER)

EXPORT_MACRO_ARG0(void, __PWR_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOA_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOB_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOC_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOD_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOE_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOF_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOG_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOH_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOI_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOJ_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_GPIOK_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_I2C1_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_I2C2_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_SPI1_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_SPI2_CLK_ENABLE)
EXPORT_MACRO_ARG0(void, __HAL_RCC_SPI3_CLK_ENABLE)

EXPORT_MACRO_ARG1(void, __HAL_PWR_VOLTAGESCALING_CONFIG, uint32_t)

void I2C1_ClearBusyFlagErratum(I2C_HandleTypeDef *instance); // i2c.c

static inline HAL_StatusTypeDef _HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    return HAL_SPI_Transmit(hspi, (uint8_t*)pData, Size, Timeout);
}

#pragma clang diagnostic pop
