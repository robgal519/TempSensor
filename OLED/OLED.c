#include "OLED.h"
#include "../I2C/I2C.h"
#include "stdlib.h"
#include "string.h"

const uint8_t OLED_I2C_DEVICE_ADDRESS = 0b0111100;

static void oled_send_command(uint8_t command);

typedef struct OLED {
	uint8_t control_byte;
	uint8_t framebuffer[COLUMNS*PAGES];
} OLED;

OLED *oled_create(void) {
	oled_send_command(0xAE); // display off
	
	oled_send_command(0xD5); // clock
	oled_send_command(0xF1); // upper nibble is rate, lower nibble is divisor
	
	oled_send_command(0xA8); // mux ratio
	oled_send_command(0x3F); // rtfm
	
	oled_send_command(0xD3); // display offset
	oled_send_command(0x00); // rtfm
	
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
	oled_send_command(0xCF); // 128 is midpoint
	
	oled_send_command(0xD9); // precharge
	oled_send_command(0xF1); // rtfm
	
	oled_send_command(0xDB); // vcomh deselect level
	oled_send_command(0x40); // rtfm
	
	oled_send_command(0x2E); // deactivate scroll
	oled_send_command(0xA4); // display scan on
	oled_send_command(0xA6); // non-inverted
	oled_send_command(0xAF); // drivers on
	
	OLED *oled = calloc(1, sizeof(OLED));
	oled->control_byte = 0x40; // Data/Control bit set to data mode
	
	return oled;
}

void oled_destroy(OLED *oled) {
	free(oled);
}

void oled_display_on(OLED *oled) {
	oled_send_command(0xAF);
}

void oled_display_off(OLED *oled) {
	oled_send_command(0xAE);
}

uint8_t *oled_access_framebuffer(OLED *oled) {
	return oled->framebuffer;
}

void oled_lock_display(OLED *oled) {
	oled_send_command(0xA5);
}

void oled_unlock_display(OLED *oled) {
	oled_send_command(0xA4);
}

void oled_send_framebuffer(OLED *oled) {
	oled_send_command(0x21);
	oled_send_command(0);
	oled_send_command(127);

	oled_send_command(0x22);
	oled_send_command(0);
	oled_send_command(7);

	i2c_master_transmit(
	  OLED_I2C_DEVICE_ADDRESS,
	  (uint8_t *) oled,
    COLUMNS*PAGES+1,
	  false
	);
}

void oled_clear_framebuffer(OLED *oled) {
	memset(oled->framebuffer, 0, COLUMNS*PAGES);
}

static void oled_send_command(uint8_t command){
	uint8_t bytes[2] = {
		0x00,   // control byte - 0 << 6 for command
		command // data byte - command opcode and/or data
	};

	i2c_master_transmit(
		OLED_I2C_DEVICE_ADDRESS,
		bytes,
		sizeof(bytes)/sizeof(*bytes), 
		false);
}
