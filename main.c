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
#include "Fonts/consolas11.h"
#include "Fonts/consolas18.h"
#include "Fonts/consolas24.h"

typedef uint8_t (*TwoDimArray)[OLED_COLUMNS];

int main()
{
	i2c_init();
	
	OLED *oled = oled_create();
	
	Framebuffer fb = oled_access_framebuffer(oled);
	
	TextDrawable text_drawable = {
		.text = "Wit@m",
		.font_info = consolas_24pt,
		.offset_x = 20,
		.offset_y = 10,
	};
	
	drawer_draw_text(fb, text_drawable);
	
	oled_send_framebuffer(oled);
	
	while(1);
}
