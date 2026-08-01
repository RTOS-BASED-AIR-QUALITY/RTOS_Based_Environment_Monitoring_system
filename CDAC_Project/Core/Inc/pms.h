/*
 * pms.h
 *
 *      Author: 
 */

#ifndef INC_PMS_H_
#define INC_PMS_H_

#include "main.h"

#define PMS5003_FRAME_SIZE    32

#define PMS5003_HEADER1       0x42
#define PMS5003_HEADER2       0x4D

typedef struct
{
    uint16_t pm1_0;      // (Byte 10-11)
    uint16_t pm2_5;      // (Byte 12-13)
    uint16_t pm10;       // (Byte 14-15)

} PMS5003_Data;

extern PMS5003_Data rxData;

void PMS5003_Start(void);
uint16_t S5003_Checksum(void);
void PMS5003_ProcessFrame(void);




#endif /* INC_PMS_H_ */
