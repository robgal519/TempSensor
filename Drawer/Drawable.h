#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <stdint.h>

// Assumed vertical paging + page size = 8

// If 'height' % 8 != 0, it is assumed that 
// another row of pages is provided and redundant
// bits are ignored .

typedef struct Drawable {
	uint8_t *data;
	uint8_t offset_x;
	uint8_t offset_y;
	uint8_t width;
	uint8_t height;
} Drawable;

#endif
