/* Simple FreeRTOS test program for the AT91SAM7S256 ARM MCU */

// $Id$

static const char revision[] = "$Id$";

#error This application is currently BROKEN.  I do not currently have FreeRTOS working on the AT91SAM7S256.

#ifndef FREERTOS
#error You must define FREERTOS to compile this FreeRTOS application
#endif

#include <assert.h>
#include <cpu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#define MESSAGE_PERIOD	4000

xSemaphoreHandle console_lock;

void TaskFunction(void *parameters)
{
  char *message = parameters;

  portTickType waketime = xTaskGetTickCount();

  for (;;)
  {
    vTaskDelayUntil(&waketime, (MESSAGE_PERIOD/2 + (lrand48() % (MESSAGE_PERIOD/2)))/portTICK_RATE_MS);

    xSemaphoreTake(console_lock, portMAX_DELAY);
    puts(message);
    xSemaphoreGive(console_lock);
  }
}

int main(void)
{
  xTaskHandle task1;
  xTaskHandle task2;

  cpu_init(DEFAULT_CPU_FREQ);

  serial_stdio(CONSOLE_PORT, 115200);

// Display version information

  puts("\033[H\033[2JAT91SAM7S256 FreeRTOS Test (" __DATE__ " " __TIME__ ")\n");
  puts(revision);
  printf("\nCPU Freq:%ld Hz  Compiler:%s  FreeRTOS:%s\n\n", CPUFREQ, __VERSION__,
    tskKERNEL_VERSION_NUMBER);

// Create mutex to arbitrate console output

  console_lock = xSemaphoreCreateMutex();
  if (console_lock == NULL)
  {
    puts("ERROR: xSemaphoreCreateMutex() for console_lock failed!");
    fflush(stdout);
    assert(0);
  }

// Create a couple of tasks

  if (xTaskCreate(TaskFunction, (signed char *) "task1", 256, "This is Task 1", 1, &task1) != pdPASS)
  {
    puts("ERROR: xTaskCreate() for task1 failed!");
    fflush(stdout);
    assert(0);
  }

  if (xTaskCreate(TaskFunction, (signed char *) "task2", 256, "This is Task 2", 1, &task2) != pdPASS)
  {
    puts("ERROR: xTaskCreate() for task2 failed!");
    fflush(stdout);
    assert(0);
  }

  vTaskStartScheduler();
  assert(0);
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
  printf("ERROR: vApplicationStackOverflowHook(): Task \"%s\" overflowed its stack\n", pcTaskName);
  fflush(stdout);
  assert(0);
}
