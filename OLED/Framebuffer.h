#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

// Assumed vertical paging + page size = 8

typedef struct Framebuffer {
	uint8_t *framebuffer;
	uint8_t width;
	uint8_t height_pages;
} Framebuffer;

#endif
