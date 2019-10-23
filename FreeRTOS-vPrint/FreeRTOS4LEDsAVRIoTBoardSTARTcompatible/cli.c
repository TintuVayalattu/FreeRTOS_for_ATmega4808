/*
 * cli.c
 *
 * Created: 23-Oct-19 7:34:45 PM
 *  Author: I17986
 */ 

#include <string.h>
#include "cli.h"

/* Variables */
QueueHandle_t xCommandQueue;

/* Function prototypes */
void vPrintString(void *pvParams);

void vCommandReceiverTask(void *pvParams)
{
	char *ptr=NULL;
	for (;;)
	{
		if( xQueueReceive(xCommandQueue,(void *)&ptr,100) == pdPASS )
		{
			if (!strcmp((char *)ptr,CMD_VERSION))
			{
				vPrintString((void *)"version = 1.0.0\r\n");
			}
			else if (!strcmp((char *)ptr,CMD_TIME))
			{
				vPrintString((void *)"18:30:00\r\n");
			}
			else if (!strcmp((char *)ptr,CMD_CLI_VERSION))
			{
				vPrintString((void *)"18:30:00\r\n");
			}		
			else if (!strcmp((char *)ptr,CMD_CPU_USAGE))
			{
				vPrintString((void *)"18:30:00\r\n");
			}
			else
			{
				vPrintString((void *)ptr);
			}
		}
	}
}