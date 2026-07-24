/*
 * ADC_read.h
 *
 *      Author: sunbeam
 */

#ifndef INC_MQ135_H_
#define INC_MQ135_H_

#include "main.h"

typedef struct MQ135_Data
{
	 uint16_t ADC_val;
	 float sensorResistace_ratio;
} MQ135_Data;

float MQ135calculateRS(uint32_t adc);
MQ135_Data ADCRead();

#endif /* INC_MQ135_H_ */
