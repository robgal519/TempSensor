#include "I2C.h"
#include "Driver_I2C.h"

extern ARM_DRIVER_I2C Driver_I2C1;

static uint32_t i2c_event;

static void i2c_event_callback(uint32_t event);

static int8_t i2c_wait_for_transfer(void);


static void i2c_event_callback(uint32_t event) {
  /* Save received events */
  i2c_event |= event;
}

static int8_t i2c_wait_for_transfer(void) {
  while ((i2c_event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U)
    ;
  /* Check if all data transferred */
  if ((i2c_event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) {
    LOG("Transfer failed");
    i2c_event = 0;
    return -1;
  }
  i2c_event = 0;
  return 0;
}

void i2c_init(void) {
  Driver_I2C1.Initialize(i2c_event_callback);
	Driver_I2C1.PowerControl(ARM_POWER_FULL);
	Driver_I2C1.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
	Driver_I2C1.Control(ARM_I2C_BUS_CLEAR, 0);
}

int32_t i2c_master_transmit(uint32_t addr,
                            const uint8_t *data,
                            uint32_t num,
                            bool xfer_pending) {
	int32_t statusCode = Driver_I2C1.MasterTransmit(
		addr,
		data,
		num, 
		false);

  if(!statusCode) {
		i2c_wait_for_transfer();
	}
	
	return statusCode;
}
