/*
 * BMP.c
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



HAL_StatusTypeDef BME280_ReadTempCalibration(I2C_HandleTypeDef *hi2c)     // calibreation as per bosch standard
{
	 uint8_t calib[6];

	    if(HAL_I2C_Mem_Read(hi2c,BME280_ADDR,BME280_CALIB_START,I2C_MEMADD_SIZE_8BIT,calib,6,HAL_MAX_DELAY) != HAL_OK)
	    {
	        return HAL_ERROR;
	    }


	 dig_T1 = (uint16_t)((calib[1] << 8) | calib[0]);
	 dig_T2 = (int16_t)((calib[3] << 8) | calib[2]);
	 dig_T3 = (int16_t)((calib[5] << 8) | calib[4]);

	 return HAL_OK;
}

// humidity calibration

HAL_StatusTypeDef BME280_ReadHumidityCalibration(I2C_HandleTypeDef *hi2c)
{
    uint8_t calib[7];

    HAL_I2C_Mem_Read(hi2c,BME280_ADDR,0xA1,I2C_MEMADD_SIZE_8BIT,&dig_H1,1,HAL_MAX_DELAY);   // address is not consigative

    HAL_I2C_Mem_Read(hi2c,BME280_ADDR,0xE1,I2C_MEMADD_SIZE_8BIT,calib,7,HAL_MAX_DELAY);     // consicutive addresses

    dig_H2 = (int16_t)((calib[1] << 8) | calib[0]);
    dig_H3 = calib[2];
    dig_H4 = (int16_t)((calib[3] << 4) | (calib[4] & 0x0F));
    dig_H5 = (int16_t)((calib[5] << 4) | (calib[4] >> 4));
    dig_H6 = (int8_t)calib[6];

    return HAL_OK;
}


float BME280_GetTemperature(I2C_HandleTypeDef *hi2c)           // calculations as per the datasheet
{
	int32_t ADC_T;
	int32_t var1;
	int32_t var2;
	int32_t T;

	ADC_T = BME280_ReadRawTemp(&hi2c1);       // raw value of temp

	 var1 = ((((ADC_T >> 3) - ((int32_t)dig_T1 << 1))) *((int32_t)dig_T2)) >> 11;           // calculations for calibration from datasheet

	 var2 = (((((ADC_T >> 4) - ((int32_t)dig_T1)) * ((ADC_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	    t_fine = var1 + var2;

	    T = (t_fine * 5 + 128) >> 8;

	    return (float)T / 100.0f;
}

// actual humudity calculated using BOASH formulae

float BME280_GetHumidity(I2C_HandleTypeDef *hi2c)
{
    int32_t adc_H;
    int32_t v_x1_u32r;

    adc_H = BME280_ReadRawHumidity(hi2c);      // raw value of humidity

    v_x1_u32r = t_fine - ((int32_t)76800);

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20)-(((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *(((((((v_x1_u32r *                                   // formulae from datasheet
                ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));

    v_x1_u32r = v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4);

    if(v_x1_u32r < 0)
    {
        v_x1_u32r = 0;
    }

    if(v_x1_u32r > 419430400)
    {
        v_x1_u32r = 419430400;
    }

    return (float)(v_x1_u32r >> 12) / 1024.0f;
}
