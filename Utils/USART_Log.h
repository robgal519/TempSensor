#ifndef USART_LOG_GUARD
#define USART_LOG_GUARD

#include "Log.h"

#ifndef LOGGER_CONNECTION
#define LOGGER_CONNECTION Driver_USART1
#endif

#ifndef LOGGER_BAUDRATE
#define LOGGER_BAUDRATE 921600
#endif

#endif