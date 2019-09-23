/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device - Human Interface Device example
 *----------------------------------------------------------------------------*/

#include "main.h"
#include "rl_usb.h"                     /* RL-USB function prototypes         */

#include "Board_LED.h"
#include "Board_Buttons.h"
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
  uint8_t but, but_last;

  (void)arg;

#ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize(0, 1);
  EventRecorderEnable (EventRecordError, 0xA0U, 0xA6U);  /* USBD Error Events */
  EventRecorderEnable (EventRecordAll  , 0xA0U, 0xA0U);  /* Core Events */
  EventRecorderEnable (EventRecordAll  , 0xA5U, 0xA5U);  /* HID Events */
#endif

  LED_Initialize();                     /* LED Initialization                 */
  Buttons_Initialize();                 /* Buttons Initialization             */

  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (0U, 0U*24U, "    USB Device      ");
  GLCD_DrawString         (0U, 1U*24U, "    HID Class       ");
  GLCD_DrawString         (0U, 2U*24U, "    HID Example     ");
  GLCD_DrawString         (0U, 4U*24U, "USBFS: HID0         ");
  GLCD_DrawString         (0U, 5U*24U, "USBHS: HID1         ");
  GLCD_DrawString         (0U, 8U*24U, "  Keil Tools by ARM ");
  GLCD_DrawString         (0U, 9U*24U, "    www.keil.com    ");

  USBD_Initialize(0U);                  /* USB Device 0 Initialization        */
  USBD_Connect   (0U);                  /* USB Device 0 Connect               */

  USBD_Initialize(1U);                  /* USB Device 1 Initialization        */
  USBD_Connect   (1U);                  /* USB Device 1 Connect               */

  but = but_last = 0U;
  for (;;) {                            /* Loop forever                       */
    but = (uint8_t)(Buttons_GetState ());
    if ((but ^ but_last) & 0x1FU) {
      but_last = but & 0x1FU;
      if (USBD_Configured (0)) { USBD_HID_GetReportTrigger(0U, 0U, &but, 1U); }
      if (USBD_Configured (1)) { USBD_HID_GetReportTrigger(1U, 0U, &but, 1U); }
    }
    osDelay(100U);                      /* 100 ms delay for sampling buttons  */
  }
}
