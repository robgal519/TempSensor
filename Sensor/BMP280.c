/**
 * @file BMP280.c
 * @author Robert Gałat (robgal519@gmail.com)
 * @brief Implementation of BMP280 sensor interface
 * @version 0.1
 * @date 2019-10-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "BMP280.h"

#ifndef LOG
#define LOG(message)
#endif

static ARM_DRIVER_I2C *connection = NULL;

static uint32_t I2C_Event;

static int32_t t_fine = 0;
static uint16_t dig_T1 = 0;
static int16_t dig_T2 = 0;
static int16_t dig_T3 = 0;

static uint16_t dig_P1 = 0;
static int16_t dig_P2 = 0;
static int16_t dig_P3 = 0;
static int16_t dig_P4 = 0;
static int16_t dig_P5 = 0;
static int16_t dig_P6 = 0;
static int16_t dig_P7 = 0;
static int16_t dig_P8 = 0;
static int16_t dig_P9 = 0;

static uint8_t addr = 0;

static uint8_t measurements[6];

static void I2C_SignalEvent(uint32_t event)
{
    /* Save received events */
    I2C_Event |= event;
}

static float lastReadedTemp;
static float lastReadedPressure;

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
static uint32_t bmp280_compensate_P_int64(int32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (uint32_t)p;
}

// Function to calibrate measurement coppied from documentation
static int32_t bmp280_compensate_T_double(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
            ((int32_t)dig_T3)) >>
           14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

static int32_t readTemp()
{
    uint8_t precision = 4;
    uint16_t BaseValue = (measurements[3] << 8) + measurements[4];
    uint8_t ExtraValue = measurements[5] >> (8 - precision);

    int32_t Value = (BaseValue << precision) + ExtraValue;
    return bmp280_compensate_T_double(Value);
}


static int32_t readPressure()
{
    uint8_t precision = 4;
    uint16_t BaseValue = (measurements[0] << 8) + measurements[1];
    uint8_t ExtraValue = measurements[2] >> (8 - precision);

    int32_t Value = (BaseValue << precision) + ExtraValue;
    return bmp280_compensate_P_int64(Value);
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

static void readSingleRegister(uint8_t cmd, uint8_t *value, uint8_t Bytes)
{
    if (!connection)
    {
        LOG("Connection not provided");
        return;
    }
    I2C_Event = 0U;
    connection->MasterTransmit(addr, &cmd, 1, true);
    if (!WaitForTransfer())
        return;
    I2C_Event = 0U;

    connection->MasterReceive(addr, value, Bytes, false);
    if (!WaitForTransfer())
        return;
    I2C_Event = 0U;
}

static void readCalibrationParameters(void)
{
    if (!connection)
    {
        LOG("Connection not provided");
        return;
    }

    readSingleRegister(0x88, (uint8_t *)&dig_T1, sizeof(dig_T1));
    readSingleRegister(0x8A, (uint8_t *)&dig_T2, sizeof(dig_T2));
    readSingleRegister(0x8c, (uint8_t *)&dig_T3, sizeof(dig_T3));

    readSingleRegister(0x8E, (uint8_t *)&dig_P1, sizeof(dig_P1));
    readSingleRegister(0x90, (uint8_t *)&dig_P2, sizeof(dig_P2));
    readSingleRegister(0x92, (uint8_t *)&dig_P3, sizeof(dig_P3));
    readSingleRegister(0x94, (uint8_t *)&dig_P4, sizeof(dig_P4));
    readSingleRegister(0x96, (uint8_t *)&dig_P5, sizeof(dig_P5));
    readSingleRegister(0x98, (uint8_t *)&dig_P6, sizeof(dig_P6));
    readSingleRegister(0x9A, (uint8_t *)&dig_P7, sizeof(dig_P7));
    readSingleRegister(0x9C, (uint8_t *)&dig_P8, sizeof(dig_P8));
    readSingleRegister(0x9E, (uint8_t *)&dig_P9, sizeof(dig_P9));
}

void BMP280_initConnection(ARM_DRIVER_I2C *DataConnection)
{
    connection = DataConnection;
    connection->Initialize(I2C_SignalEvent);
    connection->PowerControl(ARM_POWER_FULL);
    connection->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    connection->Control(ARM_I2C_BUS_CLEAR, 0);

    BMP280_setAddres(BMP250_DefaultAddress);
    readCalibrationParameters();
}

void BMP280_updateConfiguration(BMP280_Config configuration)
{
    if (!connection)
    {
        LOG("Connection not provided");
        return;
    }

    uint8_t configCmd[2] = {0xF4, *((uint8_t *)&configuration)};
    I2C_Event = 0U;
    connection->MasterTransmit(addr, configCmd, 2, false);
    if (!WaitForTransfer())
        return;
    I2C_Event = 0U;
}

void BMP280_readMeasurements(void)
{
    if (!connection)
    {
        LOG("Connection not provided");
        return;
    }
    I2C_Event = 0U;
    uint8_t reg = 0xF7;
    connection->MasterTransmit(addr, &reg, 1, false);
    if (!WaitForTransfer())
        return;
    I2C_Event = 0U;
    connection->MasterReceive(addr, measurements, 6, false);
    if (!WaitForTransfer())
        return;
    I2C_Event = 0U;

    // Take care of the order temp first than pressure
    lastReadedTemp = readTemp()/100.;
    lastReadedPressure = readPressure()/255.;
}

float BMP280_ExtractTemperature(void)
{
    return lastReadedTemp;
}

float BMP280_ExtractPressure(void)
{
    return lastReadedPressure;
}

void BMP280_setAddres(uint8_t address)
{
    addr = address;
}
