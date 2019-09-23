/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    VirtualCOM.c
 * Purpose: USB Device Virtual COM Port demonstration
 *----------------------------------------------------------------------------*/

#include "main.h"
#include "rl_usb.h"                     /* RL-USB function prototypes         */
#include "RTE_Components.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/*------------------------------------------------------------------------------
 *        Application
 *----------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

#ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize(0, 1);
  EventRecorderEnable (EventRecordError, 0xA0U, 0xA6U);  /* USBD Error Events */
  EventRecorderEnable (EventRecordAll  , 0xA0U, 0xA0U);  /* Core Events */
  EventRecorderEnable (EventRecordAll  , 0xA4U, 0xA4U);  /* CDC Events */
#endif

  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (0U, 0U*24U, "    USB Device      ");
  GLCD_DrawString         (0U, 1U*24U, "   CDC ACM Class    ");
  GLCD_DrawString         (0U, 2U*24U, " VirtualCOM Example ");
  GLCD_DrawString         (0U, 4U*24U, " USBFS <-> UART1    ");
  GLCD_DrawString         (0U, 5U*24U, " USBHS <-> UART3    ");
  GLCD_DrawString         (0U, 8U*24U, "  Keil Tools by ARM ");
  GLCD_DrawString         (0U, 9U*24U, "    www.keil.com    ");

  USBD_Initialize         (0U);         // USB Device 0 Initialization
  USBD_Connect            (0U);         // USB Device 0 Connect

  USBD_Initialize         (1U);         // USB Device 1 Initialization
  USBD_Connect            (1U);         // USB Device 1 Connect

  for (;;) {                            // Loop forever
    osDelay(1000U);
  }
}
