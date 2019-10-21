#ifndef FONT_COMMON
#define FONT_COMMON

#include <stdint.h>

typedef struct FontCharInfo{
    uint8_t width;
    uint32_t offset;
} FontCharInfo;

typedef struct FontInfo {
    uint8_t *bitmaps;
    FontCharInfo *descriptors;
} FontInfo;

#define CharToLookup(x) ((uint8_t)(x-32))

#endif

