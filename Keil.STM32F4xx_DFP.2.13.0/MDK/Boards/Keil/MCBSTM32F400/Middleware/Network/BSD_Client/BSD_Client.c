/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    BSD_Client.c
 * Purpose: LED Client example using BSD sockets
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

//   <s.40>Remote IP Address
//   =======================
//   <i> IPv4 or IPv6 Address in text representation
//   <i>Default: "192.168.0.100" or "fe80::1c30:6cff:fea2:455e"
#define IP_ADDR        "fe80::1c30:6cff:fea2:455e"

//   <o>Remote Port <1-65535>
//   <i> Do not set number of port too small,
//   <i> maybe it is already used.
//   <i> Default: 1001
#define PORT_NUM       1001

//   <o>Communication Protocol <0=> Stream (TCP) <1=> Datagram (UDP)
//   <i> Select a protocol for sending data.
#define PROTOCOL       1

//   <o>LED Blinking speed <1-100>
//   <i> Blinking speed = SPEED * 100ms
//   <i> Default: 2
#define SPEED          2

//------------- <<< end of configuration section >>> -----------------------

#define BLINKLED 0x01                   /* Command for blink the leds         */
#if (PROTOCOL == 0)
 #define SOCKTYPE   SOCK_STREAM
#else
 #define SOCKTYPE   SOCK_DGRAM
#endif

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

extern GLCD_FONT GLCD_Font_16x24;

static uint32_t x =  0;

static void Client (void *arg);

/* Initialize display */
static void init_display () {
  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
  GLCD_DrawString         (x*16, 1*24, "       MDK-MW       ");
  GLCD_DrawString         (x*16, 2*24, " BSD Client example ");
}

/* Print IP addresses */
static void print_address (void) {
  static uint8_t ip_addr[NET_ADDR_IP6_LEN];
  static char    ip_ascii[40];
  static char    buf[24];

  /* Print IPv4 network address */
  netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr));
  netIP_ntoa (NET_ADDR_IP4, ip_addr, ip_ascii, sizeof(ip_ascii));

  sprintf (buf, "IP4:%-15s", ip_ascii);
  GLCD_DrawString (x*16, 4*24, buf);

  /* Print IPv6 network address */
  netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP6_LinkLocalAddress, ip_addr, sizeof(ip_addr));
  netIP_ntoa(NET_ADDR_IP6, ip_addr, ip_ascii, sizeof(ip_ascii));

  sprintf (buf, "IP6:%.16s", ip_ascii);
  GLCD_DrawString ( x    *16, 5*24, buf);
  sprintf (buf, "%s", ip_ascii+16);
  GLCD_DrawString ((x+10)*16, 6*24, buf);
}

/*----------------------------------------------------------------------------
  Thread 'Client': BSD Client socket process
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Client (void *arg) {
  SOCKADDR_IN6 addr;
  int sock, res;
  char dbuf[4];
  uint8_t p2val,lshf;

  (void)arg;

  while (1) {
    memset (&addr, 0, sizeof(addr));

    /* Setup network parameters */
    if (netIP_aton (IP_ADDR, NET_ADDR_IP4, (uint8_t *)&addr.sin6_addr)) {
      /* IP_ADDR specifies IPv4 address */
      addr.sin6_family = AF_INET;
    }
    else if (netIP_aton (IP_ADDR, NET_ADDR_IP6, (uint8_t *)&addr.sin6_addr)) {
      /* IP_ADDR specified IPv6 address */
      addr.sin6_family = AF_INET6;
    }
    addr.sin6_port = htons(PORT_NUM);

    /* Create socket */
    sock = socket (addr.sin6_family, SOCKTYPE, 0);

    /* Establish connection to remote host */
    connect (sock, (SOCKADDR *)&addr, sizeof (addr));

    lshf  = 1;
    p2val = 0x01;
    while (1) {
      // Shift the LEDs
      LED_SetOut (p2val);
      p2val = lshf ? (uint8_t)((uint32_t)p2val << 1) : (uint8_t)((uint32_t)p2val >> 1);
      if (p2val == 0x80) lshf = 0;
      if (p2val == 0x01) lshf = 1;
    
      // Send the data to LED Server.
      dbuf[0] = BLINKLED;
      dbuf[1] = p2val;
      res = send (sock, (char *)&dbuf, 2, 0);
      if (res < 0) {
        break;
      }
      osDelay (100 * SPEED);
    }
    closesocket (sock);
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
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
  init_display   ();
  
  netInitialize ();
  print_address ();

  // Create networking task
  osThreadNew (Client, NULL, NULL);
  while(1) {
    osThreadFlagsWait (0, osFlagsWaitAny, osWaitForever);
  }
}
