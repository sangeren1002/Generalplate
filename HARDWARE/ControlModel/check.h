#ifndef __CHECK_H__
#define __CHECK_H__
#include "sys.h"
//#include "stm32f4xx_hal.h"

uint16_t CalcFieldCRC(uint16_t* pDataArray, uint16_t numberOfWords);
uint8_t BccCheckSum(uint8_t buf[], uint8_t lenth);
uint16_t UART_CRC16_Work(uint8_t *CRC_Buf,uint8_t CRC_Leni);

#endif

