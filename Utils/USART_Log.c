#include "USART_Log.h"
#include <stdio.h>
#include <stdarg.h>
#include <Driver_USART.h>

static ARM_DRIVER_USART* Logger_connection = NULL;
static uint32_t UART_event;

extern ARM_DRIVER_USART LOGGER_CONNECTION;

static void callback(uint32_t event){
UART_event|=event;
}

static char sendBuffer[255];

static void UART_logger_init(ARM_DRIVER_USART *connection){
	if(connection){
	Logger_connection = connection;
	Logger_connection->Initialize(&callback);
	Logger_connection->PowerControl(ARM_POWER_FULL);
	Logger_connection->Control(
        ARM_USART_MODE_ASYNCHRONOUS | 
        ARM_USART_DATA_BITS_8 | 
        ARM_USART_PARITY_NONE | 
        ARM_USART_STOP_BITS_1 , LOGGER_BAUDRATE
    );
	Logger_connection->Control (ARM_USART_CONTROL_TX, 1);
	Logger_connection->Control (ARM_USART_CONTROL_RX, 1);
	}
}

void logger_send_message(const char* format, ...){
if(!Logger_connection){
		UART_logger_init(&LOGGER_CONNECTION);
	}

	va_list args;
	va_start (args, format);
  vsnprintf(sendBuffer,255,format,args);
	va_end(args);
	
	Logger_connection->Send(sendBuffer, sizeof(sendBuffer)/sizeof(sendBuffer[0]));
	while(UART_event!= ARM_USART_EVENT_SEND_COMPLETE);
	UART_event = 0;

}
