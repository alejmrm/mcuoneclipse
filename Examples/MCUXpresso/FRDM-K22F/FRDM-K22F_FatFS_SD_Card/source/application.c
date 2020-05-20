/*
 * Copyright (c) 2020, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "McuWait.h"
#include "McuRTOS.h"
#include "leds.h"
#include "McuTimeout.h"
#include "McuTimeDate.h"

static TimerHandle_t timerTimeoutHndl, timerTimeHndl;

static void vTimerTimoutAddTick(TimerHandle_t pxTimer) {
  McuTimeout_AddTick();
}

static void vTimerTimeAddTick(TimerHandle_t pxTimer) {
  McuTimeDate_AddTick();
}

static void AppTask(void *pv) {
  for(;;) {
    McuLED_Toggle(LEDS_LedRed);
    vTaskDelay(pdMS_TO_TICKS(100));
    McuLED_Toggle(LEDS_LedGreen);
    vTaskDelay(pdMS_TO_TICKS(100));
    McuLED_Toggle(LEDS_LedBlue);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void APP_Run(void) {
  BaseType_t result;

  PL_Init();
  result =xTaskCreate(AppTask, "AppTask", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, NULL);
  if (result!=pdPASS) {
    /* error! */
  }
  timerTimeoutHndl = xTimerCreate(  /* timer to handle periodic things */
        "timeout", /* name */
        pdMS_TO_TICKS(McuTimeout_TICK_PERIOD_MS), /* period/time */
        pdTRUE, /* auto reload */
        (void*)0, /* timer ID */
        vTimerTimoutAddTick); /* callback */
  if (timerTimeoutHndl==NULL) {
    for(;;); /* failure! */
  }
  if (xTimerStart(timerTimeoutHndl, 0)!=pdPASS) { /* start the timer */
    for(;;); /* failure!?! */
  }

  timerTimeHndl = xTimerCreate(  /* timer to handle periodic things */
        "time", /* name */
        pdMS_TO_TICKS(McuTimeDate_TICK_TIME_MS), /* period/time */
        pdTRUE, /* auto reload */
        (void*)0, /* timer ID */
        vTimerTimeAddTick); /* callback */
  if (timerTimeHndl==NULL) {
    for(;;); /* failure! */
  }
  if (xTimerStart(timerTimeHndl, 0)!=pdPASS) { /* start the timer */
    for(;;); /* failure!?! */
  }

  vTaskStartScheduler();
  for(;;) {}
}