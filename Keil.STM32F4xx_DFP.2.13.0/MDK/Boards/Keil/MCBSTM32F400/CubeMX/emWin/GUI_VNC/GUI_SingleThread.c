
#include "cmsis_os2.h"
#include "GUI.h"
#include "Dialog.h"

extern WM_HWIN CreateMyDialog(void);
extern int  GUI_VNC_X_StartServer(int, int);

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/*----------------------------------------------------------------------------
 *      GUIThread: GUI Thread for Single-Task Execution Model
 *---------------------------------------------------------------------------*/
#define GUI_THREAD_STK_SZ    (2048U)

static void         GUIThread (void *argument);         /* thread function */
static osThreadId_t GUIThread_tid;                      /* thread id */
static uint64_t     GUIThread_stk[GUI_THREAD_STK_SZ/8]; /* thread stack */

static const osThreadAttr_t GUIThread_attr = {
  .stack_mem  = &GUIThread_stk[0],
  .stack_size = sizeof(GUIThread_stk),
  .priority   = osPriorityIdle 
};

int Init_GUIThread (void) {

  GUIThread_tid = osThreadNew(GUIThread, NULL, &GUIThread_attr);
  if (GUIThread_tid == NULL) {
    return(-1);
  }

  return(0);
}

__NO_RETURN static void GUIThread (void *argument) {
  (void)argument;

  GUI_Init();           /* Initialize the Graphics Component */

  GUI_VNC_X_StartServer(0,0);
  CreateMyDialog();

  while (1) {
    
    /* All GUI related activities might only be called from here */

#ifdef RTE_Graphics_Touchscreen   /* Graphics Input Device Touchscreen enabled */
    GUI_TOUCH_Exec();             /* Execute Touchscreen support */
#endif
    GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();             /* Nothing left to do for the moment ... Idle processing */
    osDelay(100);
  }
}
