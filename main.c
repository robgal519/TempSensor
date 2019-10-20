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

typedef uint8_t (*TwoDimArray)[OLED_COLUMNS];

int main()
{
	i2c_init();
	
	OLED *oled = oled_create();
	oled_send_framebuffer(oled);
	
	Framebuffer fb = oled_access_framebuffer(oled);
	
	TwoDimArray fb_as_array = (TwoDimArray)(fb.framebuffer);
	
	for(int i=2; i<8-2; i++){
		for(int j=30; j<128-30; j++){
			fb_as_array[i][j] = 0xff;
		}			
	}
	
	oled_send_framebuffer(oled);
	
	while(1);
}
