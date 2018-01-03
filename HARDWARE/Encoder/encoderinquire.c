/**********************************************************************
File name:   encoderinquire.c
Author:      WXK
Data:        2017.10.20
Description: 绝对值编码器逻辑功能层函数，主要描述用哪些硬件接口实现功能
History:
***********************************************************************/

#include "encoderinquire.h"

uint8_t askangle[5] = {0x55, 0xFF, 0x01, 0x00, 0x5A};   /* 绝对值编码器查询当前角度命令串口发送缓存区 */
uint8_t answerangle[6] = {0};    /* 绝对值编码器角度值接收缓存区 */

float Swerve_Node1_Angle = 0;
float Swerve_Node2_Angle = 0;
float Swerve_Node3_Angle = 0;
float Swerve_Node4_Angle = 0;

/****************************************
Function:    AskAngle
Description: 查询绝对值编码器当前角度值
Input:       addr-所要查询的绝对值编码器的地址
Return:      所查询到的角度值（°）
History:
****************************************/
float AskAngle(uint8_t addr)
{
	uint16_t tmp = 0;
	
	askangle[1] = addr;
	
	HAL_UART_Receive_IT(&huart4, answerangle, 6);
	HAL_UART_Transmit(&huart4, askangle, 5, 0xffff);

	HAL_Delay(10);

	tmp = answerangle[3];
	tmp = tmp<<8;
	tmp = tmp|answerangle[2];
	
	tmp = tmp/65536.0f*360.0f;
	
	return tmp;
}

void InquireSwerveMotorAngle(void)
{
	Swerve_Node1_Angle = AskAngle(ENCODERNODE1);
	Swerve_Node2_Angle = AskAngle(ENCODERNODE2);
	Swerve_Node3_Angle = AskAngle(ENCODERNODE3);
	Swerve_Node4_Angle = AskAngle(ENCODERNODE4);
}


