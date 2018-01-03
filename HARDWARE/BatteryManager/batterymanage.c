/**********************************************************************
File name:   batterymanage.c
Author:      WXK
Data:        2017.10.20
Description: 电池管理逻辑功能层函数，主要描述用哪些硬件接口实现功能
History:
***********************************************************************/

#include "batterymanage.h"

uint8_t battery_ask[7]     = {0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77}; /* 电池查询当前状态 */
uint8_t battery_answer[34] = {0};                                        /* 电池状态应答缓存区 */

float BatteryPower;                /* 电池总电压（V）*/
float BatteryResidualCapacity;     /* 电池剩余容量（mAH）*/ 
float BatteryCurrent;              /* 电池电流（A）*/
float BatteryRSOC;                 /* 剩余容量百分比 */
float BatteryTemperature;          /* 电池温度（°C）*/
float BatteryTemperature_RiseRate; /* 电池温度上升速度（°C/min）*/

/*************************
Function:    BatteryInquire
Description: 查询电池状态 
Output:      BatteryPower-电池总电压（V）
             BatteryResidualCapacity-电池剩余容量（mAH）
             BatteryCurrent-电池电流（A）
             BatteryRSOC-剩余容量百分比
             BatteryTemperature-电池温度（°C）
History:
*************************/
void BatteryInquire(void)
{
	uint16_t temp;
	
	HAL_GPIO_WritePin(RS485_control_GPIO_Port, RS485_control_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart4, battery_ask, 7, 0xffff);
	HAL_GPIO_WritePin(RS485_control_GPIO_Port, RS485_control_Pin, GPIO_PIN_RESET);
	HAL_UART_Receive(&huart4, battery_answer, 34, 0xff);

	if(battery_answer[0] == 0xDD && battery_answer[2] == 0x00)
	{
		temp = battery_answer[4];
		BatteryPower   = (temp <<8 | battery_answer[5])*0.01f; /* 总电压（V）*/
		
		temp = battery_answer[6];
		BatteryCurrent = (temp <<8 | battery_answer[7])*0.01f; /* 电流（A）*/
		if(BatteryCurrent < 0)
		{
			BatteryCurrent = BatteryCurrent + 655.35f;
		}
		
		temp = battery_answer[8];
		BatteryResidualCapacity = ( temp<< 8 | battery_answer[9])*10.0f; /* 电池剩余容量（mAH）*/
	
		BatteryRSOC = battery_answer[23]; /* 剩余容量百分比 */
		
		temp = battery_answer[27];
		uint16_t T1 = temp << 8 | battery_answer[28];
		
		temp = battery_answer[29];
		uint16_t T2 = temp << 8 | battery_answer[30];
		
		if(T1 >= T2)  /* 取两个NTC热敏电阻温度高的作为电池温度 */
		{
			BatteryTemperature = (T1 - 2731)/10.0f;
		}
		else
		{
			BatteryTemperature = (T2 - 2731)/10.0f;
		}
	}
}

