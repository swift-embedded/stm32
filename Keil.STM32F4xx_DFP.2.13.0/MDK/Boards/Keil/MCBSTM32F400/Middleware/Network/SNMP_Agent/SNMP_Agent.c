/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    SNMP_Agent.c
 * Purpose: SNMP Agent example
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"
#include "RTE_Components.h" 

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCBSTM32F400::Board Support:Graphic LCD

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
extern void register_mib_table (void);

char lcd_text[2][20+1] = { "LCD line 1",
                           "LCD line 2" };

/* Thread IDs */
osThreadId_t TID_Display;

/* Thread declarations */
static void Display (void *arg);

/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
    osThreadFlagsSet (TID_Display, 0x01);
  }
}

/*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Display (void *arg) {
  static uint8_t ip_addr[NET_ADDR_IP6_LEN];
  static char    ip_ascii[40];
  static char    buf[24];
  uint32_t x = 0;

  (void)arg;

  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (x, 1*24, "     MW Network     ");
  GLCD_DrawString         (x, 2*24, "     SNMP Agent     ");

  GLCD_DrawString (x*16, 4*24, "IP4:Waiting for DHCP");

  /* Print Link-local IPv6 address */
  netIF_GetOption (NET_IF_CLASS_ETH,
                   netIF_OptionIP6_LinkLocalAddress, ip_addr, sizeof(ip_addr));

  netIP_ntoa(NET_ADDR_IP6, ip_addr, ip_ascii, sizeof(ip_ascii));

  sprintf (buf, "IP6:%.16s", ip_ascii);
  GLCD_DrawString ( x   *16, 5*24, buf);
  sprintf (buf, "%s", ip_ascii+16);
  GLCD_DrawString ((x+10)*16, 6*24, buf);

  while(1) {
    /* Wait for signal from DHCP */
    osThreadFlagsWait (0x01, osFlagsWaitAny, osWaitForever);

    /* Retrieve and print IPv4 address */
    netIF_GetOption (NET_IF_CLASS_ETH,
                     netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr));

    netIP_ntoa (NET_ADDR_IP4, ip_addr, ip_ascii, sizeof(ip_ascii));

    sprintf (buf, "IP4:%-16s",ip_ascii);
    GLCD_DrawString (x*16, 4*24, buf);

    /* Display user text lines */
    sprintf (buf, "%-20s", lcd_text[0]);
    GLCD_DrawString (0, 7*24, buf);
    sprintf (buf, "%-20s", lcd_text[1]);
    GLCD_DrawString (0, 8*24, buf);
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'app_main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

#ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize(0U, 1U);
  EventRecorderEnable (EventRecordError, 0xC0U, 0xDCU);    /* Net Events     */
  EventRecorderEnable (EventRecordAll,   0xCAU, 0xCAU);    /* DHCP Events    */
  EventRecorderEnable (EventRecordAll,   0xD8U, 0xD8U);    /* SNMP Agent     */
#endif

  LED_Initialize ();
  Buttons_Initialize ();

  netInitialize ();
  register_mib_table ();

  TID_Display = osThreadNew (Display,  NULL, NULL);

  while(1) {
    osThreadFlagsWait (0, osFlagsWaitAny, osWaitForever);
  }
}
