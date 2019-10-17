/**
 * @file main.c
 * @author Robert Gałat (robgal519@gmail.com)
 * @brief measure temperature and preassure and display it on lcd
 * @version 0.1
 * @date 2019-10-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "I2C/I2C.h"
#include "OLED/OLED.h"

int main()
{
	i2c_init();
	
	OLED *oled = oled_create();
	oled_send_framebuffer(oled);
	
	while(1);
}
