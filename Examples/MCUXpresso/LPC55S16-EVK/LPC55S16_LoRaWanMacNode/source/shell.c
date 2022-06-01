/*
 * Copyright (c) 2019, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_SHELL
#include "McuShell.h"
#include "McuRTOS.h"
#include "McuRTT.h"
#include "McuArmTools.h"
#include "McuLog.h"
#include "McuTimeDate.h"
#include "McuShellUart.h"
#include "McuTimeDate.h"
#include "LoRaWAN.h"
#if PL_CONFIG_USE_MININI
  #include "McuMinINI.h"
  #include "minGlue-Flash.h"
  #include "McuFlash.h"
#endif
#include "application.h"

static const McuShell_ParseCommandCallback CmdParserTable[] =
{
  McuShell_ParseCommand,
#if McuLib_CONFIG_SDK_USE_FREERTOS
  McuRTOS_ParseCommand,
#endif
#if McuArmTools_CONFIG_PARSE_COMMAND_ENABLED
  McuArmTools_ParseCommand,
#endif
#if McuLog_CONFIG_IS_ENABLED && McuLog_CONFIG_PARSE_COMMAND_ENABLED
  McuLog_ParseCommand,
#endif
  McuTimeDate_ParseCommand,
#if PL_CONFIG_USE_MININI
  McuMinINI_ParseCommand,
  ini_ParseCommand,
  McuFlash_ParseCommand,
#endif
  LORAWAN_ParseCommand,
  APP_ParseCommand,
  NULL /* Sentinel */
};

typedef struct {
  McuShell_ConstStdIOType *stdio;
  unsigned char *buf;
  size_t bufSize;
} SHELL_IODesc;

static const SHELL_IODesc ios[] =
{
#if PL_CONFIG_USE_RTT
  {&McuRTT_stdio,  McuRTT_DefaultShellBuffer,  sizeof(McuRTT_DefaultShellBuffer)},
#endif
#if PL_CONFIG_USE_SHELL_UART
  {&McuShellUart_stdio,  McuShellUart_DefaultShellBuffer,  sizeof(McuShellUart_DefaultShellBuffer)},
#endif
#if PL_CONFIG_USE_USB_CDC
  {&USB_CdcStdio,  USB_CdcDefaultShellBuffer,  sizeof(USB_CdcDefaultShellBuffer)},
#endif
#if PL_CONFIG_USE_SWO
  {&McuSWO_stdio,  McuSWO_DefaultShellBuffer,  sizeof(McuSWO_DefaultShellBuffer)},
#endif
};

void SHELL_SendChar(unsigned char ch) {
  for(int i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    McuShell_SendCh(ch, ios[i].stdio->stdOut);
  }
}

void SHELL_SendString(unsigned char *str) {
  for(int i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    McuShell_SendStr(str, ios[i].stdio->stdOut);
  }
}

#if McuLib_CONFIG_SDK_USE_FREERTOS
static void ShellTask(void *pv) {
  int i;

  McuLog_trace("Shell task started");
  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    ios[i].buf[0] = '\0';
  }
  for(;;) {
    /* process all I/Os */
    for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
      (void)McuShell_ReadAndParseWithCommandTable(ios[i].buf, ios[i].bufSize, ios[i].stdio, CmdParserTable);
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
#endif

void SHELL_Process(void) {
  /* process all I/Os */
  for(int i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    (void)McuShell_ReadAndParseWithCommandTable(ios[i].buf, ios[i].bufSize, ios[i].stdio, CmdParserTable);
  }
}

void SHELL_Init(void) {
#if McuLib_CONFIG_SDK_USE_FREERTOS
  if (xTaskCreate(
      ShellTask,  /* pointer to the task */
      "Shell", /* task name for kernel awareness debugging */
      3000/sizeof(StackType_t), /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY+2,  /* initial priority */
      (TaskHandle_t*)NULL /* optional task handle to create */
    ) != pdPASS)
  {
     for(;;){} /* error! probably out of memory */
  }
#endif
  McuShell_SetStdio(ios[0].stdio);
#if McuLog_CONFIG_IS_ENABLED
  for(int i=0; i<sizeof(ios)/sizeof(ios[0]) && i<McuLog_CONFIG_NOF_CONSOLE_LOGGER; i++) {
    McuLog_set_console(ios[i].stdio, i);
  }
#endif /* McuLog_CONFIG_IS_ENABLED */
}

void SHELL_Deinit(void) {}

#endif /* PL_CONFIG_USE_SHELL */
