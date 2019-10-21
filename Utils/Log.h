#ifndef LOG_H
#define LOG_H

#ifndef LOGGER_TARGET // default target
#define LOGGER_TARGET NONE
#endif

#define LOG(...) logger_send_message(__VA_ARGS__)

enum Log_supported_targets{
	USART,
	NONE
};

void logger_send_message(const char* format, ...);

#if LOGGER_TARGET == USART
#include "USART_Log.h" 
#else
void logger_send_message(const char* format, ...){}
#endif


#endif
