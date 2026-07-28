#include "pms.h"

typedef enum
{
    HEADER_1,
    HEADER_2,
    RECEIVE_FRAME
} PMS_State_t;

PMS_State_t pmsState = HEADER_1;


extern UART_HandleTypeDef huart1;

static uint8_t rxByte;
uint8_t indx = 0;
uint8_t rxBuffer[30];

PMS5003_Data rxData;


void PMS5003_Start(void)
{
	HAL_UART_Receive_IT(&huart1, &rxByte, 1);

}







