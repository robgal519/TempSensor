#ifndef I2C_H
#define I2C_H


#include "../Utils/Log.h"
#include <stdint.h>
#include <stdbool.h>

void i2c_init(void);

int32_t i2c_master_transmit(
		uint32_t addr,
		const uint8_t *data,
		uint32_t num,
		bool xfer_pending);
		
int32_t i2c_master_receive(
		uint32_t addr,
		uint8_t *data,
		uint32_t num,
		bool xfer_pending 
);

#endif
