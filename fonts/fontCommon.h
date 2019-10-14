#ifndef FONT_COMMON
#define FONT_CONNON
#include <stdint.h>
typedef struct FONT_CHAR_INFO{
    uint8_t width;
    uint32_t offset;
}FONT_CHAR_INFO;

#define CharToLookup(x) ((uint8_t)(x-32))


#endif

