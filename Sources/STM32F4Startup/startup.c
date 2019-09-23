/**************************************************************************//**
 * @file     startup_ARMCM4.s
 * @brief    CMSIS Core Device Startup File for
 *           ARMCM4 Device Series
 * @version  V5.00
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>


/*----------------------------------------------------------------------------
  Linker generated Symbols
 *----------------------------------------------------------------------------*/
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __copy_table_start__;
extern uint32_t __copy_table_end__;
extern uint32_t __zero_table_start__;
extern uint32_t __zero_table_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );


/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
#ifndef __START
extern void  _start(void) __attribute__((noreturn));    /* PreeMain (C library entry point) */
#else
extern int  __START(void) __attribute__((noreturn));    /* main entry point */
#endif

#ifndef __NO_SYSTEM_INIT
extern void SystemInit (void);            /* CMSIS System Initialization      */
#endif


/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Default_Handler(void);                          /* Default empty handler */
void Reset_Handler(void);                            /* Reset Handler */


/*----------------------------------------------------------------------------
  User Initial Stack & Heap
 *----------------------------------------------------------------------------*/
#ifndef __STACK_SIZE
  #define	__STACK_SIZE  0x00004000
#endif
static uint8_t stack[__STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

#ifndef __HEAP_SIZE
  #define	__HEAP_SIZE   0x00004000
#endif
#if __HEAP_SIZE > 0
static uint8_t heap[__HEAP_SIZE]   __attribute__ ((aligned(8), used, section(".heap")));
#endif


/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M4 Processor Exceptions */
void NMI_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler  (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));

/* ARMCM4 Specific Interrupts */
void WWDG_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream0_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream2_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream3_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream4_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream5_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream6_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void ADC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN1_TX_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void OTG_FS_WKUP_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_TIM12_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void FMC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SDIO_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM5_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void UART4_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void UART5_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream0_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void ETH_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void ETH_WKUP_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN2_TX_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN2_RX0_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN2_RX1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void CAN2_SCE_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void USART6_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_WKUP_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DCMI_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void HASH_RNG_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void UART7_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void UART8_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPI4_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPI5_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPI6_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SAI1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void LTDC_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void LTDC_ER_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void DMA2D_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
const pFunc __Vectors[] __attribute__ ((section(".vectors"))) = {
  /* Cortex-M4 Exceptions Handler */
  (pFunc)((uint32_t)&__StackTop),           /*      Initial Stack Pointer     */
  Reset_Handler,                            /*      Reset Handler             */
  NMI_Handler,                              /*      NMI Handler               */
  HardFault_Handler,                        /*      Hard Fault Handler        */
  MemManage_Handler,                        /*      MPU Fault Handler         */
  BusFault_Handler,                         /*      Bus Fault Handler         */
  UsageFault_Handler,                       /*      Usage Fault Handler       */
  0,                                        /*      Reserved                  */
  0,                                        /*      Reserved                  */
  0,                                        /*      Reserved                  */
  0,                                        /*      Reserved                  */
  SVC_Handler,                              /*      SVCall Handler            */
  DebugMon_Handler,                         /*      Debug Monitor Handler     */
  0,                                        /*      Reserved                  */
  PendSV_Handler,                           /*      PendSV Handler            */
  SysTick_Handler,                          /*      SysTick Handler           */

  /* External interrupts */
  WWDG_IRQHandler,                   /* Window WatchDog              */
  PVD_IRQHandler,                    /* PVD through EXTI Line detection */
  TAMP_STAMP_IRQHandler,             /* Tamper and TimeStamps through the EXTI line */
  RTC_WKUP_IRQHandler,               /* RTC Wakeup through the EXTI line */
  FLASH_IRQHandler,                  /* FLASH                        */
  RCC_IRQHandler,                    /* RCC                          */
  EXTI0_IRQHandler,                  /* EXTI Line0                   */
  EXTI1_IRQHandler,                  /* EXTI Line1                   */
  EXTI2_IRQHandler,                  /* EXTI Line2                   */
  EXTI3_IRQHandler,                  /* EXTI Line3                   */
  EXTI4_IRQHandler,                  /* EXTI Line4                   */
  DMA1_Stream0_IRQHandler,           /* DMA1 Stream 0                */
  DMA1_Stream1_IRQHandler,           /* DMA1 Stream 1                */
  DMA1_Stream2_IRQHandler,           /* DMA1 Stream 2                */
  DMA1_Stream3_IRQHandler,           /* DMA1 Stream 3                */
  DMA1_Stream4_IRQHandler,           /* DMA1 Stream 4                */
  DMA1_Stream5_IRQHandler,           /* DMA1 Stream 5                */
  DMA1_Stream6_IRQHandler,           /* DMA1 Stream 6                */
  ADC_IRQHandler,                    /* ADC1, ADC2 and ADC3s         */
  CAN1_TX_IRQHandler,                /* CAN1 TX                      */
  CAN1_RX0_IRQHandler,               /* CAN1 RX0                     */
  CAN1_RX1_IRQHandler,               /* CAN1 RX1                     */
  CAN1_SCE_IRQHandler,               /* CAN1 SCE                     */
  EXTI9_5_IRQHandler,                /* External Line[9:5]s          */
  TIM1_BRK_TIM9_IRQHandler,          /* TIM1 Break and TIM9          */
  TIM1_UP_TIM10_IRQHandler,          /* TIM1 Update and TIM10        */
  TIM1_TRG_COM_TIM11_IRQHandler,     /* TIM1 Trigger and Commutation and TIM11 */
  TIM1_CC_IRQHandler,                /* TIM1 Capture Compare         */
  TIM2_IRQHandler,                   /* TIM2                         */
  TIM3_IRQHandler,                   /* TIM3                         */
  TIM4_IRQHandler,                   /* TIM4                         */
  I2C1_EV_IRQHandler,                /* I2C1 Event                   */
  I2C1_ER_IRQHandler,                /* I2C1 Error                   */
  I2C2_EV_IRQHandler,                /* I2C2 Event                   */
  I2C2_ER_IRQHandler,                /* I2C2 Error                   */
  SPI1_IRQHandler,                   /* SPI1                         */
  SPI2_IRQHandler,                   /* SPI2                         */
  USART1_IRQHandler,                 /* USART1                       */
  USART2_IRQHandler,                 /* USART2                       */
  USART3_IRQHandler,                 /* USART3                       */
  EXTI15_10_IRQHandler,              /* External Line[15:10]s        */
  RTC_Alarm_IRQHandler,              /* RTC Alarm (A and B) through EXTI Line */
  OTG_FS_WKUP_IRQHandler,            /* USB OTG FS Wakeup through EXTI line */
  TIM8_BRK_TIM12_IRQHandler,         /* TIM8 Break and TIM12         */
  TIM8_UP_TIM13_IRQHandler,          /* TIM8 Update and TIM13        */
  TIM8_TRG_COM_TIM14_IRQHandler,     /* TIM8 Trigger and Commutation and TIM14 */
  TIM8_CC_IRQHandler,                /* TIM8 Capture Compare         */
  DMA1_Stream7_IRQHandler,           /* DMA1 Stream7                 */
  FMC_IRQHandler,                    /* FMC                          */
  SDIO_IRQHandler,                   /* SDIO                         */
  TIM5_IRQHandler,                   /* TIM5                         */
  SPI3_IRQHandler,                   /* SPI3                         */
  UART4_IRQHandler,                  /* UART4                        */
  UART5_IRQHandler,                  /* UART5                        */
  TIM6_DAC_IRQHandler,               /* TIM6 and DAC1&2 underrun errors */
  TIM7_IRQHandler,                   /* TIM7                         */
  DMA2_Stream0_IRQHandler,           /* DMA2 Stream 0                */
  DMA2_Stream1_IRQHandler,           /* DMA2 Stream 1                */
  DMA2_Stream2_IRQHandler,           /* DMA2 Stream 2                */
  DMA2_Stream3_IRQHandler,           /* DMA2 Stream 3                */
  DMA2_Stream4_IRQHandler,           /* DMA2 Stream 4                */
  ETH_IRQHandler,                    /* Ethernet                     */
  ETH_WKUP_IRQHandler,               /* Ethernet Wakeup through EXTI line */
  CAN2_TX_IRQHandler,                /* CAN2 TX                      */
  CAN2_RX0_IRQHandler,               /* CAN2 RX0                     */
  CAN2_RX1_IRQHandler,               /* CAN2 RX1                     */
  CAN2_SCE_IRQHandler,               /* CAN2 SCE                     */
  OTG_FS_IRQHandler,                 /* USB OTG FS                   */
  DMA2_Stream5_IRQHandler,           /* DMA2 Stream 5                */
  DMA2_Stream6_IRQHandler,           /* DMA2 Stream 6                */
  DMA2_Stream7_IRQHandler,           /* DMA2 Stream 7                */
  USART6_IRQHandler,                 /* USART6                       */
  I2C3_EV_IRQHandler,                /* I2C3 event                   */
  I2C3_ER_IRQHandler,                /* I2C3 error                   */
  OTG_HS_EP1_OUT_IRQHandler,         /* USB OTG HS End Point 1 Out   */
  OTG_HS_EP1_IN_IRQHandler,          /* USB OTG HS End Point 1 In    */
  OTG_HS_WKUP_IRQHandler,            /* USB OTG HS Wakeup through EXTI */
  OTG_HS_IRQHandler,                 /* USB OTG HS                   */
  DCMI_IRQHandler,                   /* DCMI                         */
  0,                                 /* Reserved                     */
  HASH_RNG_IRQHandler,               /* Hash and Rng                 */
  FPU_IRQHandler,                    /* FPU                          */
  UART7_IRQHandler,                  /* UART7                        */
  UART8_IRQHandler,                  /* UART8                        */
  SPI4_IRQHandler,                   /* SPI4                         */
  SPI5_IRQHandler,                   /* SPI5 						  */
  SPI6_IRQHandler,                   /* SPI6						  */
  SAI1_IRQHandler,                   /* SAI1						  */
  LTDC_IRQHandler,                   /* LTDC           		      */
  LTDC_ER_IRQHandler,                /* LTDC error          	      */
  DMA2D_IRQHandler                   /* DMA2D                        */
};


/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) {
  uint32_t *pSrc, *pDest;
  uint32_t *pTable __attribute__((unused));

/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
  pTable = &__copy_table_start__;

  for (; pTable < &__copy_table_end__; pTable = pTable + 3) {
		pSrc  = (uint32_t*)*(pTable + 0);
		pDest = (uint32_t*)*(pTable + 1);
		for (; pDest < (uint32_t*)(*(pTable + 1) + *(pTable + 2)) ; ) {
      *pDest++ = *pSrc++;
		}
	}
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
  pSrc  = &__etext;
  pDest = &__data_start__;

  for ( ; pDest < &__data_end__ ; ) {
    *pDest++ = *pSrc++;
  }
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
  pTable = &__zero_table_start__;

  for (; pTable < &__zero_table_end__; pTable = pTable + 2) {
		pDest = (uint32_t*)*(pTable + 0);
		for (; pDest < (uint32_t*)(*(pTable + 0) + *(pTable + 1)) ; ) {
      *pDest++ = 0;
		}
	}
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
  pDest = &__bss_start__;

  for ( ; pDest < &__bss_end__ ; ) {
    *pDest++ = 0UL;
  }
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	SystemInit();
#endif

#ifndef __START
#define __START _start
#endif
	__START();

}


/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void) {

	while(1);
}
