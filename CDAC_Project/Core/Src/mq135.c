/*
 * ADC_read.c
 *
 *  Created on: 05-Jul-2026
 *      Author: Dhiraj
 */

#include <mq135.h>
extern ADC_HandleTypeDef hadc1;


float MQ135calculateRS(uint32_t adc_count)
{
	float Vout;

	Vout = ((float)adc_count * VREF) / ADC_MAX;

	return (RL * (VC - Vout)) / Vout;
}

MQ135_Data ADCRead()
{
	uint32_t ADC_sum = 0, i;
	MQ135_Data data;


	for(i = 0; i<5; i++)
	{
		HAL_ADC_Start(&hadc1);

		HAL_ADC_PollForConversion(&hadc1, 10);

		ADC_sum += HAL_ADC_GetValue(&hadc1);

		HAL_ADC_Stop(&hadc1);
	}

	data.ADC_val = (uint16_t)ADC_sum/5;

	float RS= MQ135calculateRS(data.ADC_val);

	data.sensorResistace_ratio = (RS / R0);

	return data;
}
