/*----------------------------------------------------------------------------
 * Name:    GUI_VNC.c
 * Purpose: 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2018 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"
#include "Board_LED.h"                  // ::Board Support:LED
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

extern void Init_GUIThread(void);

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *argument) {

  (void)argument;

  LED_Initialize();
  netInitialize();

  Init_GUIThread();

  for (;;) {
    osDelay(1000);
  }
}
