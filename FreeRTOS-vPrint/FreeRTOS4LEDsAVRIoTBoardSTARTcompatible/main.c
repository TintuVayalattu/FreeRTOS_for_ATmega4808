/*
(C) 2018 Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and
any derivatives exclusively with Microchip products. It is your responsibility
to comply with third party license terms applicable to your use of third party
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED
BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY
TO MICROCHIP FOR THIS SOFTWARE.
*/

#include <avr/io.h>
#include <stdio.h>
#include <atomic.h>
#include <string.h>
#include "atmel_start.h"
#include "config/clock_config.h"
#include <util/delay.h>
#include "cli.h"
#include "serial.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Tasks */
void vSenderTask(void *pvParams);
void vSenderTask2(void *pvParams);
void vReceiverTask(void *pvParams);

/* Function prototypes */
void vPrintString(void *pvParams);

QueueHandle_t xPrintQueue;
char msg[] = "This is the Message\r\n";
char msg2[] = "This is the Message two\r\n";
char *ptr = msg;
extern char command[10];

int main(void)
{
	atmel_start_init();
	USART_0_set_ISR_cb( &serialHandler,RX_CB );
	
	xPrintQueue = xQueueCreate(5,2);
	xCommandQueue = xQueueCreate(5,2);
	/* Task Registration and creation */
	xTaskCreate(vSenderTask, "sender", configMINIMAL_STACK_SIZE, (void *)msg, 1, NULL);
	xTaskCreate(vSenderTask2, "sender2", configMINIMAL_STACK_SIZE, (void *)msg2, 1, NULL);
	xTaskCreate(vReceiverTask, "receiver", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(vCommandReceiverTask, "cmd receiver", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	
	vTaskStartScheduler();
	return 0;
}

void vSenderTask(void *pvParams)
{
	for (;;)
	{
		if (strlen((char*)pvParams)>0)
		{
			xQueueSend(xPrintQueue,(void *)&pvParams,0);
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vSenderTask2(void *pvParams)
{
	for (;;)
	{
		if (strlen((char*)pvParams)>0)
		{
			xQueueSend(xPrintQueue,(void *)&pvParams,0);
		}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void vReceiverTask(void *pvParams)
{
	char *ptr=NULL;
	for (;;)
	{
		if( xQueueReceive(xPrintQueue,(void *)&ptr,100) == pdPASS )
		{
			vPrintString((void *)ptr);
		}
	}
}

void vPrintString(void *pvParams)
{
	vTaskSuspendAll();
	printf((const char *)pvParams);
	xTaskResumeAll();
}
