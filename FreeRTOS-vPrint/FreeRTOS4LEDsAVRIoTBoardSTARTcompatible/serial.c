/*
 * serial.c
 *
 * Created: 23-Oct-19 8:10:28 PM
 *  Author: I17986
 */ 
#include "atmel_start_pins.h"

#include "FreeRTOS.h"
#include "queue.h"

char command[32] = "abcd";
void *buffPtr = command;

void serialHandler()
{
	uint8_t data;
	static uint8_t index = 0;
	extern QueueHandle_t xCommandQueue;
	
	/* Read the received data */
	data = USART2.RXDATAL;
	command[index++] = data;

	if(data == '\n')
	{
		command[index-2] = '\0';// avoid /r/n
		xQueueSendFromISR(xCommandQueue,(void *)&buffPtr,0);
		GREEN_LED_toggle_level();
		index = 0;
	}	
}