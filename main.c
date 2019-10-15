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

static bool WaitForTransfer()
{
    while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U)
        ;
    /* Check if all data transferred */
    if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U)
    {
        LOG("Transfer failed");
        return false;
    }
    return true;
}


void oled_write(bool Control_data, uint8_t Byte){

	uint8_t Bytes[2] = {Control_data << 6, Byte};
	uint8_t addr = 0b0111100;
	I2C_Event = 0U;
    Driver_I2C2.MasterTransmit(addr, Bytes, 2, true);
    if (!WaitForTransfer())
        return;

}

// These defaults are for a 128x64 configuration, adapt as needed
// seg remap, com scan, mux ratio, addr mode are suggested things to poke at
void oled_init() {
	oled_write(0, 0xAE); // display off
	
	oled_write(0, 0xD5); // clock
	oled_write(0, 0x81); // upper nibble is rate, lower nibble is divisor
	
	oled_write(0, 0xA8); // mux ratio
	oled_write(0, 0x3F); // rtfm
	
	oled_write(0, 0xD3); // display offset
	oled_write(0, 0x00); // rtfm
	oled_write(0, 0x00);
	
	oled_write(0, 0x8D); // charge pump
	oled_write(0, 0x14); // enable
	
	oled_write(0, 0x20); // memory addr mode
	oled_write(0, 0x00); // horizontal
	
	oled_write(0, 0xA1); // segment remap
	
	oled_write(0, 0xA5); // display on
	
	oled_write(0, 0xC8); // com scan direction
	oled_write(0, 0xDA); // com hardware cfg
	oled_write(0, 0x12); // alt com cfg
	
	oled_write(0, 0x81); // contrast aka current
	oled_write(0, 0x7F); // 128 is midpoint
	
	oled_write(0, 0xD9); // precharge
	oled_write(0, 0x11); // rtfm
	
	oled_write(0, 0xDB); // vcomh deselect level
	oled_write(0, 0x20); // rtfm
	
	oled_write(0, 0xA6); // non-inverted
	
	oled_write(0, 0xA4); // display scan on
	oled_write(0, 0xAF); // drivers on
}

int main()
{

	BMP280_initConnection(&Driver_I2C1);
	BMP280_Config cfg = {.osrs_t = x2, .osrs_p = x16, .mode = Forced};
	BMP280_updateConfiguration(cfg);

	BMP280_readMeasurements();

	volatile float temperature = BMP280_ExtractTemperature();
	volatile float preassure = BMP280_ExtractPressure();
	

    Driver_I2C2.Initialize(I2C_SignalEvent);
    Driver_I2C2.PowerControl(ARM_POWER_FULL);
    Driver_I2C2.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    Driver_I2C2.Control(ARM_I2C_BUS_CLEAR, 0);
	

oled_init();
	
	
	return 0;
}
