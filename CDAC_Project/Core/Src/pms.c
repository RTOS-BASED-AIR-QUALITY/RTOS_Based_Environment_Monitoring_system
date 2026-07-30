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


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{

		switch(pmsState)
		{
			case HEADER_1:
				if(rxByte == PMS5003_HEADER1)
					pmsState = HEADER_2;
				break;

			case HEADER_2:
				if(rxByte == PMS5003_HEADER2)
				{
					indx =0;
					pmsState = RECEIVE_FRAME;
				}
				else
					pmsState = HEADER_1;
				break;

			case RECEIVE_FRAME:

				rxBuffer[indx] = rxByte;
				indx ++;

				if(indx >= 30)
				{
					PMS5003_ProcessFrame();

					indx = 0;
					pmsState = HEADER_1;
				}
				break;
		}

		HAL_UART_Receive_IT(&huart1, &rxByte, 1);
	}
}









