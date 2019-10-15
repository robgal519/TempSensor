/**
 * @file BMP280.h
 * @author Robert Gałat (robgal519@gmail.com)
 * @brief Interface for MBP280 temperature and preassure sensor
 * @version 0.1
 * @date 2019-10-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef BMP280DeviceDriver
#define BMP280DeviceDriver

#include "Driver_I2C.h" // ::CMSIS Driver:I2C
#include "stdint.h"

#define BMP250_DefaultAddress 0b1110110

/**
 * @brief oversampling register iws used to set precision of the measurement
 * maximum resolution is 20 bit, and minimum is 16 bit
 * 
 * Table 4 of the BMP280 specification describes how to set values of the oversampling
 */
typedef enum BMP280_osrs
{
    x0 = 0b000, // Skipped (output set to 0x80000)
    x1 = 0b001, // 16 bit / 0.0050 °C / 2.62 Pa
    x2 = 0b010, // 17 bit / 0.0025 °C / 1.31 Pa
    x4 = 0b011, // 18 bit / 0.0012 °C / 0.66 Pa
    x8 = 0b100, // 19 bit / 0.0006 °C / 0.33 Pa
    x16 = 0b101 // 20 bit / 0.0003 °C / 0.16 Pa
} BMP280_osrs;

/**
 * @brief set the mode of operation.
 * 
 */
typedef enum BMP280_mode
{
    Sleep = 0b00,  // default state after power on
    Forced = 0b10, // wake device for single measurement than fet to Sleep mode
    Normal = 0b11  // make measurements in cycle
} BMP280_mode;

/**
 * @brief set precision of measurement and mode of operation
 */
typedef struct BMP280_Config
{
    uint8_t osrs_t : 3;
    uint8_t osrs_p : 3;
    uint8_t mode : 2;
} BMP280_Config;

/**
 * @brief Initialize The BMP280 device
 * The Driver handle initialization and deinitialization of raw connection. 
 * 
 * @param connection Raw connection to I2C
 * 
*/
void BMP280_initConnection(ARM_DRIVER_I2C *connection);

/**
 * @brief set new configuration ( wake from Sleep mode)
 */
void BMP280_updateConfiguration(BMP280_Config configuration);

/**
 * @brief send command to read last measurement
 */
void BMP280_readMeasurements(void);

/**
 * @brief get value of last measurement
 * 
 * The function BMP280_readMeasurements should be called first to read data for intertpretation
 */
float BMP280_ExtractTemperature(void);

/**
 * @brief get value of last measurement
 * 
 * The function BMP280_readMeasurements should be called first to read data for intertpretation
 */
float BMP280_ExtractPressure(void);

/**
 * @brief Set SPI address of the device, the default address is set in BMP280_initConnection function 
 */
void BMP280_setAddres(uint8_t address);
#endif
