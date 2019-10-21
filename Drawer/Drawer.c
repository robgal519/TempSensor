#include "Drawer.h"

#include <stdint.h>
#include "../Utils/Log.h"

void drawer_draw_bitmap(Framebuffer framebuffer, Drawable drawable){
	const uint8_t page_size = DRAWER_PAGE_SIZE;
	
	// Calculate mask for last row of pages to ignore /don't care/ bits
	const uint8_t mask_for_last_row = 0xff >> page_size - (drawable.height % page_size);
	const uint8_t drawable_height_pages = drawable.height % page_size ? drawable.height/page_size + 1: drawable.height/page_size;
	for(uint8_t i=0; i<drawable_height_pages; i++){	
		for(uint8_t j=0; j<drawable.width; j++){
			uint8_t framebuffer_x = drawable.offset_x + j;
			uint8_t framebuffer_y_page_top = (i*page_size + drawable.offset_y)/page_size;
			uint8_t framebuffer_y_page_bottom = (i*page_size + drawable.offset_y + page_size - 1)/page_size;
			
			if(framebuffer_y_page_bottom * framebuffer.width + framebuffer_x > framebuffer.width * framebuffer.height_pages) {
				continue;
			}
			
			uint8_t mask = drawable.height % page_size && i == drawable_height_pages - 1 ? mask_for_last_row : 0xff;
			
			*(framebuffer.framebuffer
			+ framebuffer_y_page_top * framebuffer.width
			+ framebuffer_x) 
			|= 
			((*(drawable.data + i * drawable.width + j) & mask) << drawable.offset_y % page_size);

			*(framebuffer.framebuffer
			+ framebuffer_y_page_bottom * framebuffer.width
			+ framebuffer_x) 
			|= 
			((*(drawable.data + i * drawable.width + j) & mask) >> page_size - (drawable.offset_y % page_size));
		}
	}
}
