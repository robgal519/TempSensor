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
#include "Sensor/BMP280.h"
#include "Driver_I2C.h" // ::CMSIS Driver:I2C


#ifndef LOG
#define LOG(message)
#endif
extern ARM_DRIVER_I2C Driver_I2C1;
extern ARM_DRIVER_I2C Driver_I2C2;

static uint32_t I2C_Event;

void I2C_SignalEvent(uint32_t event){
	    /* Save received events */
    I2C_Event |= event;
}

static bool wait_for_i2c_to_finish_transfer()
{
    while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);
    /* Check if all data transferred */
    if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U)
    {
        LOG("Transfer failed");
        return false;
    }
    return true;
}

#define OLED_DEVICE_ADDRESS 0b0111100

void oled_send_command(uint8_t command){
	uint8_t bytes[2] = {
		0x00, // control byte - 0 << 6 for command
		command // data byte - command opcode and/or data
	};
	I2C_Event = 0U;
	Driver_I2C2.MasterTransmit(
		OLED_DEVICE_ADDRESS,
		bytes,
		sizeof(bytes)/sizeof(*bytes), 
		false);
	
	wait_for_i2c_to_finish_transfer();
}

#define COLUMNS 128
#define PAGES 8 // 1 byte contains column of 8 pixels => 64 pixels / 8

void oled_send_data(uint8_t framebuffer[COLUMNS * PAGES]) {
	oled_send_command(0x21); // Setup column range
	oled_send_command(0); // Start
	oled_send_command(127); // End

	oled_send_command(0x22); // Setup page range
	oled_send_command(0); // Start
	oled_send_command(7); // End

	uint8_t bytes[2*COLUMNS*PAGES];
	for(int i=0; i<COLUMNS*PAGES; i++){
		bytes[2*i] = 0x40;
		bytes[2*i + 1] = framebuffer[i];
	}
	
	I2C_Event = 0U;
	Driver_I2C2.MasterTransmit(
		OLED_DEVICE_ADDRESS,
		bytes,
		sizeof(bytes)/sizeof(*bytes), 
		false);
		
	wait_for_i2c_to_finish_transfer();
}

// These defaults are for a 128x64 configuration, adapt as needed
// seg remap, com scan, mux ratio, addr mode are suggested things to poke at
void oled_init() {
	oled_send_command(0xAE); // display off
	
	oled_send_command(0xD5); // clock
	oled_send_command(0xF1); // upper nibble is rate, lower nibble is divisor
	
	oled_send_command(0xA8); // mux ratio
	oled_send_command(0x3F); // rtfm
	
	oled_send_command(0xD3); // display offset
	oled_send_command(0x00); // rtfm
	oled_send_command(0x00);
	
	oled_send_command(0x8D); // charge pump
	oled_send_command(0x14); // enable
	
	oled_send_command(0x20); // memory addr mode
	oled_send_command(0x00); // horizontal
	
	oled_send_command(0xA1); // segment remap
	
	oled_send_command(0xA5); // display on
	
	oled_send_command(0xC8); // com scan direction
	oled_send_command(0xDA); // com hardware cfg
	oled_send_command(0x12); // alt com cfg
	
	oled_send_command(0x81); // contrast aka current
	oled_send_command(0x7F); // 128 is midpoint
	
	oled_send_command(0xD9); // precharge
	oled_send_command(0x11); // rtfm
	
	oled_send_command(0xDB); // vcomh deselect level
	oled_send_command(0x20); // rtfm
	
	oled_send_command(0xA6); // non-inverted
	
	oled_send_command(0xA4); // display scan on
	oled_send_command(0xAF); // drivers on
}

int main()
{
	Driver_I2C2.Initialize(I2C_SignalEvent);
	Driver_I2C2.PowerControl(ARM_POWER_FULL);
	Driver_I2C2.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
	Driver_I2C2.Control(ARM_I2C_BUS_CLEAR, 0);
	
	oled_init();
	
	uint8_t framebuffer[COLUMNS*PAGES] = {0};
	
	oled_send_data(framebuffer);
	
	return 0;
}
