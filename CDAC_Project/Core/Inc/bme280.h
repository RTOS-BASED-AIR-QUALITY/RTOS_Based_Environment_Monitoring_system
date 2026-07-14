/*
 * BMP280.h
 *
 *  Created on: 27-Jul-2026
 *      Author: sunbeam
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define BME280_ADDR        (0x76 << 1)   // 7 bit slave address address

// 1 Byte registers						  register addresses
#define BME280_REG_ID       0xD0            // register address in which BMP chip id is present
#define BME280_REG_CTRL     0xF4           // measurement control using temp_oversampling(5,6,7), temp_oversampling(2,3,4), mode(0,2)
#define BME280_REG_CONFIG   0xF5           // used to set sensor behaviour and spi mode/i2c mode
#define BME280_REG_TEMP_MSB 0xFA

#define BME280_CALIB_START  0X88          // LSB address of calibreation register dig_T1

// humidity registres
#define BME280_REG_CTRL_HUM   0xF2        // oversampling register address for humidity
#define BME280_REG_HUM_MSB    0xFD        // humidity MSB register
#define BME280_REG_HUM_LSB    0xFE       //  humidity LSB register


typedef struct
{
    float Temp;
    float Humidity;

} BME280_Data;

// Read Chip ID //
uint8_t BME280_CheckChipID(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BME280_StartTemp(I2C_HandleTypeDef *hi2c); //  config register to read temp
uint32_t BME280_ReadRawTemp(I2C_HandleTypeDef *hi2c);        // read Raw temp.

HAL_StatusTypeDef BME280_ReadTempCalibration(I2C_HandleTypeDef *hi2c);  // read value after calibration
float BME280_GetTemperature(I2C_HandleTypeDef *hi2c);        // convert calibreted value in temp using formulae

// humidity functions
HAL_StatusTypeDef BME280_StartHumidity(I2C_HandleTypeDef *hi2c);
uint16_t BME280_ReadRawHumidity(I2C_HandleTypeDef *hi2c);
float BME280_GetHumidity(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BME280_ReadHumidityCalibration(I2C_HandleTypeDef *hi2c);



#endif /* INC_BME280_H_ */
