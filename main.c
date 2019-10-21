/**
 * @project TempSensor
 * @author Robert Gałat (robgal519@gmail.com)
 * @author Gabriel Górski (glaeqen@gmail.com)
 * @brief Measure temperature & pressure, then display on LCD screen
 * @version 0.1
 * @date 2019-10-14
 * 
 * @copyright Copyright (c) 2019
 */

#include "I2C/I2C.h"
#include "OLED/OLED.h"
#include "Drawer/Drawer.h"

typedef uint8_t (*TwoDimArray)[OLED_COLUMNS];

int main()
{
	i2c_init();
	
	OLED *oled = oled_create();
	
	Framebuffer fb = oled_access_framebuffer(oled);
	
	uint8_t data[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 
					0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 
					0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 
	 				};
	

	Drawable drawable = { .data = data, .offset_x = 0, .offset_y = 0, .width = 8, .height = 18 };
	drawer_draw_bitmap(fb, drawable);
	drawable.offset_y += 21;
	drawer_draw_bitmap(fb, drawable);
	drawable.offset_x += 50;
	drawer_draw_bitmap(fb, drawable);
	drawable.offset_x -= 20;
	drawer_draw_bitmap(fb, drawable);
	
	oled_send_framebuffer(oled);
	
	while(1);
}
