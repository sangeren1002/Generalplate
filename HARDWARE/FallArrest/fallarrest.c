#include "fallarrest.h"

void FallCheck(void);  /* 跌落检测 */

uint8_t Fall_Flag = 0; /* 跌落检测状态 0:正常 1:危险 */

void FallCheck(void)
{
	uint8_t fall_1 = 0;
	uint8_t fall_2 = 0;
	uint8_t fall_3 = 0;
	uint8_t fall_4 = 0;
	
	fall_1 = HAL_GPIO_ReadPin(FALL_1_GPIO_Port, FALL_1_Pin);
	fall_2 = HAL_GPIO_ReadPin(FALL_2_GPIO_Port, FALL_2_Pin);
	fall_3 = HAL_GPIO_ReadPin(FALL_3_GPIO_Port, FALL_3_Pin);
	fall_4 = HAL_GPIO_ReadPin(FALL_4_GPIO_Port, FALL_4_Pin);
	
	if(fall_1 == 0 && fall_2 == 0 && fall_3 == 0 && fall_4 == 0)
	{
		Fall_Flag = 0x00;
	}
	else
	{
		Fall_Flag = 0x01;
	}
}

