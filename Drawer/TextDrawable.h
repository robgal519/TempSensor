#ifndef TEXT_DRAWABLE_H
#define TEXT_DRAWABLE_H

#include <stdint.h>
#include "../Fonts/fontCommon.h"

typedef struct TextDrawable {
	const char *text;
	FontInfo font_info;
	uint8_t offset_x;
	uint8_t offset_y;
} TextDrawable;

#endif
