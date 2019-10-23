/*
 * cli.h
 *
 * Created: 23-Oct-19 7:36:27 PM
 *  Author: I17986
 */ 


#ifndef CLI_H_
#define CLI_H_

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* defines */
#define CMD_VERSION			"version"
#define CMD_TIME			"time"
#define CMD_CLI_VERSION		"cli-version"
#define CMD_SET_TIME		"set-time"
#define CMD_CPU_USAGE		"cpu-usage"
#define CMD_HEAP_USAGE		"heap-usage"

/* variables */
extern QueueHandle_t xCommandQueue;

/* Functions */
void vCommandReceiverTask(void *pvParams);

#endif /* CLI_H_ */