/*
 * BMP.c
 *
 *  Created on: 27-Jul-2026
 *      Author: sunbeam
 */

#include <bme280.h>

static uint16_t dig_T1;
static int16_t  dig_T2;
static int16_t  dig_T3;
static uint8_t  dig_H1;
static int16_t  dig_H2;
static uint8_t  dig_H3;
static int16_t  dig_H4;
static int16_t  dig_H5;
static int16_t  dig_H6;

static int32_t t_fine;
//static int32_t h_fine;

extern I2C_HandleTypeDef hi2c1;

uint8_t BME280_CheckChipID(I2C_HandleTypeDef *hi2c)   // check bmp chip id
{
	uint8_t id = 0;
	HAL_I2C_Mem_Read(&hi2c1,BME280_ADDR, BME280_REG_ID, I2C_MEMADD_SIZE_8BIT,&id,1,HAL_MAX_DELAY);

	return id;
}


HAL_StatusTypeDef BME280_StartTemp(I2C_HandleTypeDef *hi2c)        // temp oversampling and mode
{
	uint8_t ctrl_meas = 0x43;       // config value

	return HAL_I2C_Mem_Write(&hi2c1,BME280_ADDR,BME280_REG_CTRL,I2C_MEMADD_SIZE_8BIT, &ctrl_meas,1,HAL_MAX_DELAY);

}


HAL_StatusTypeDef BME280_StartHumidity(I2C_HandleTypeDef *hi2c)    // humidity oversampling
{
    uint8_t ctrl_hum = 0x01;     // humidity oversampling x1

    return HAL_I2C_Mem_Write(hi2c,BME280_ADDR,BME280_REG_CTRL_HUM,I2C_MEMADD_SIZE_8BIT,&ctrl_hum,1,HAL_MAX_DELAY);
}

uint32_t BME280_ReadRawTemp(I2C_HandleTypeDef *hi2c)
{
	  uint8_t temp_data[3];

	  HAL_I2C_Mem_Read(&hi2c1,BME280_ADDR ,BME280_REG_TEMP_MSB,I2C_MEMADD_SIZE_8BIT,temp_data,3,HAL_MAX_DELAY);

	  return ((uint32_t)temp_data[0] << 12) | ((uint32_t)temp_data[1] << 4) | ((uint32_t)temp_data[2] >> 4);
}

uint16_t BME280_ReadRawHumidity(I2C_HandleTypeDef *hi2c)
{
    uint8_t hum_data[2];

    HAL_I2C_Mem_Read(hi2c,BME280_ADDR,BME280_REG_HUM_MSB,I2C_MEMADD_SIZE_8BIT,hum_data,2,HAL_MAX_DELAY);

    return ((uint16_t)hum_data[0] << 8) | hum_data[1];
}

