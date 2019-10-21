#include "Drawer.h"

#include <stdint.h>
#include <string.h>
#include "../Utils/Log.h"

void drawer_draw_bitmap(Framebuffer framebuffer, Drawable drawable){
	const uint8_t page_size = DRAWER_PAGE_SIZE;
	
	for(uint8_t i=0; i<drawable.height_pages; i++){	
		for(uint8_t j=0; j<drawable.width; j++){
			uint8_t framebuffer_x = drawable.offset_x + j;
			uint8_t framebuffer_y_page_top = (i*page_size + drawable.offset_y)/page_size;
			uint8_t framebuffer_y_page_bottom = (i*page_size + drawable.offset_y + page_size - 1)/page_size;
			
			if(framebuffer_y_page_bottom * framebuffer.width + framebuffer_x >= framebuffer.width * framebuffer.height_pages) {
				continue; // Out of range
			}
			
			if(framebuffer_x > framebuffer.width) {
				continue; // Overflow over screen
			}
			
			*(framebuffer.framebuffer
			+ framebuffer_y_page_top * framebuffer.width
			+ framebuffer_x) 
			|= 
			(*(drawable.data + i * drawable.width + j) << (drawable.offset_y % page_size));

			*(framebuffer.framebuffer
			+ framebuffer_y_page_bottom * framebuffer.width
			+ framebuffer_x) 
			|= 
			(*(drawable.data + i * drawable.width + j) >> (page_size - (drawable.offset_y % page_size)));
		}
	}
}

void drawer_draw_text(Framebuffer framebuffer, TextDrawable text_drawable) {
	uint8_t char_height_pages = (text_drawable.font_info.descriptors[1].offset - text_drawable.font_info.descriptors[0].offset)/text_drawable.font_info.descriptors[0].width;
	for(int i=0; i<strlen(text_drawable.text); i++){
		Drawable drawable = { 
			.data = text_drawable.font_info.bitmaps + text_drawable.font_info.descriptors[CharToLookup(text_drawable.text[i])].offset, 
			.offset_x = text_drawable.offset_x + i*text_drawable.font_info.descriptors[CharToLookup(text_drawable.text[i])].width,
			.offset_y = text_drawable.offset_y,
			.width = text_drawable.font_info.descriptors[CharToLookup(text_drawable.text[i])].width,
			.height_pages = char_height_pages
		};
		drawer_draw_bitmap(framebuffer, drawable);
	}
	
}
