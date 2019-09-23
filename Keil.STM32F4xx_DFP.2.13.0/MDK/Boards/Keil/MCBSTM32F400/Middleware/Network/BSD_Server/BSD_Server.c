/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    BSD_Server.c
 * Purpose: LED Server example using BSD sockets
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "RTE_Components.h" 

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

//   <o>Socket Address Family <0=>IPv4 <1=>IPv6
//   <i> Define either IPv4 or IPv6 connections are accepted
//   <i> Default: 1
#define SOCK_AF        1

//   <o>Socket Local Port <1-65535>
//   <i> Do not set number of port too small,
//   <i> maybe it is already used.
//   <i> Default: 1001
#define SOCK_PORT      1001

//------------- <<< end of configuration section >>> -----------------------

#define BLINKLED 0x01                   /* Command for blink the leds         */

extern GLCD_FONT GLCD_Font_16x24;

static uint32_t x =  0;

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

static void Server (void *arg);

/* Initialize display */
static void init_display () {
  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (x*16, 1*24, "       MDK-MW       ");
  GLCD_DrawString         (x*16, 2*24, " BSD Server example ");
}

/* Print IP addresses */
static void print_address (void) {
  static uint8_t ip_addr[NET_ADDR_IP6_LEN];
  static char    ip_ascii[40];
  static char    buf[24];

#if (SOCK_AF == 0)
  /* Print IPv4 network address */
  netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr));
  netIP_ntoa (NET_ADDR_IP4, ip_addr, ip_ascii, sizeof(ip_ascii));

  sprintf (buf, "IP4:%-15s", ip_ascii);
  GLCD_DrawString (x*16, 5*24, buf);
#else
  /* Print IPv6 network address */
  netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP6_LinkLocalAddress, ip_addr, sizeof(ip_addr));
  netIP_ntoa(NET_ADDR_IP6, ip_addr, ip_ascii, sizeof(ip_ascii));

  sprintf (buf, "IP6:%.16s", ip_ascii);
  GLCD_DrawString ( x    *16, 5*24, buf);
  sprintf (buf, "%s", ip_ascii+16);
  GLCD_DrawString ((x+10)*16, 6*24, buf);
#endif
}

/*----------------------------------------------------------------------------
  Thread 'Server': BSD Server socket process (2 instances)
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Server (void *arg) {
  SOCKADDR_IN6 addr;
  int sock, sd, res;
  int type = (int)arg;
  char dbuf[4];

  while (1) {
    /* Setup network parameters */
    memset (&addr, 0, sizeof(addr));

#if (SOCK_AF == 0)
    /* Accept only IPv4 connections */
    addr.sin6_family = AF_INET;
#else
    /* Accept only IPv6 connections */
    addr.sin6_family = AF_INET6;
#endif
    addr.sin6_port   = htons(SOCK_PORT);

    /* Create socket */
    sock = socket (addr.sin6_family, type, 0);

    /* Bind address and port to socket */
    bind (sock, (SOCKADDR *)&addr, sizeof(addr));

    if (type == SOCK_STREAM) {
      listen (sock, 1);
      sd = accept (sock, NULL, NULL);
      closesocket (sock);
      sock = sd;
    }

    while (1) {
      res = recv (sock, dbuf, sizeof (dbuf), 0);
      if (res <= 0) {
        break;
      }
      if (dbuf[0] == BLINKLED) {
        LED_SetOut (dbuf[1]);
      }
    }
    closesocket (sock);
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
  EventRecorderEnable (EventRecordAll,   0xD2U, 0xD2U);    /* BSD Events     */
#endif

  LED_Initialize ();
  init_display ();

  netInitialize ();
  print_address ();

  /* Create BSD socket processor threads */
  osThreadNew (Server, (void *)SOCK_STREAM, NULL);
  osThreadNew (Server, (void *)SOCK_DGRAM,  NULL);

  while(1) {
    osThreadFlagsWait (0, osFlagsWaitAny, osWaitForever);
  }
}
