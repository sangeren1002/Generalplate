/**********************************************************************
File name:   motorcontrol.c
Author:      WXK
Data:        2017.10.26
Description: 直行电机控制逻辑功能层函数，主要描述用哪些硬件接口实现功能
History:
***********************************************************************/

#include "straightmotorcontrol.h"

void MotorInit(void);	                           /* 直行电机+转向电机初始化 */
void StartMotor(void);                           /* 直行电机+转向电机开始运动 */
void StopMotor(void);                            /* 直行电机停止运动 */
void StraightMotorInit(uint16_t node);           /* 直行电机初始化 */
void StraightMotorSetSpeed(uint16_t node);       /* 直行电机设置速度并使能 */
void StraightMotorStop(uint16_t node);           /* 直行电机停止运动 */

StraightMotor smotor;                            /* 直行电机结构体类型全局变量 */

//float Speed_Advance                    =   0;    /* 整体线速度V 单位mm/s */
//float Speed_Angular                    =   0;    /* 角速度W     单位°/s  */

uint8_t motor_model[14]                =  {0};   /* 直行电机操作模式选择命令串口发送缓存区 */
uint8_t motor_outline[14]              =  {0};   /* 直行电机运动轮廓选择命令串口发送缓存区 */
uint8_t motor_disable[14]              =  {0};   /* 直行电机控制字关命令串口发送缓存区 */
uint8_t motor_enable[14]               =  {0};   /* 直行电机控制字开命令串口发送缓存区 */
uint8_t motor_velocity[14]             =  {0};   /* 直行电机设置速度命令串口发送缓存区 */
uint8_t motor_start[14]                =  {0};   /* 直行电机开始运动命令串口发送缓存区 */
uint8_t motor_clearerror[14]           =  {0};   /* 直行电机清错命令串口发送缓存区 */
uint8_t motor_stop[14]                 =  {0};   /* 直行电机停止运动命令串口发送缓存区 */
uint8_t recv[10] = {0};

/*************************
Function:    StartMotor
Description: 开始NODE1、NODE2、NODE3、NODE4直行电机运动
History:
*************************/
void StartMotor(void)
{
	UpdataMotorRevSpeed(&smotor, gSpeed_Advance, gSpeed_Angular);
	
	StraightMotorSetSpeed(NODE1);
	StraightMotorSetSpeed(NODE2);
	StraightMotorSetSpeed(NODE3);
	StraightMotorSetSpeed(NODE4);
}

void StopMotor(void)
{
	StraightMotorStop(NODE1);
	StraightMotorStop(NODE2);
	StraightMotorStop(NODE3);
	StraightMotorStop(NODE4);
}

/*************************
Function:    MotorInit
Description: 初始化NODE1、NODE2、NODE3、NODE4电机运动状态
History:
*************************/
void MotorInit(void)
{
	StraightMotorInit(NODE1);
	StraightMotorInit(NODE2);
	StraightMotorInit(NODE3);
	StraightMotorInit(NODE4);
}

/*************************
Function:    StraightMotorInit
Description: 直行电机初始化，使电机处于就绪随时可运动状态 
Input:       node-所需要设置的电机地址
History:
*************************/
void StraightMotorInit(uint16_t node)
{
	uint16_t motor_datarray[7] = {0};
	
	uint8_t node_tmp = node>>8;
	node = node<<8;
	node += node_tmp;
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6040;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x0080;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_clearerror[0]  = motor_datarray[0]>>8;
	motor_clearerror[1]  = motor_datarray[0];
	motor_clearerror[2]  = motor_datarray[1];
	motor_clearerror[3]  = motor_datarray[1]>>8;
	motor_clearerror[4]  = motor_datarray[2];
	motor_clearerror[5]  = motor_datarray[2]>>8;
	motor_clearerror[6]  = motor_datarray[3];
	motor_clearerror[7]  = motor_datarray[3]>>8;
	motor_clearerror[8]  = motor_datarray[4];
	motor_clearerror[9]  = motor_datarray[4]>>8;
	motor_clearerror[10] = motor_datarray[5];
	motor_clearerror[11] = motor_datarray[5]>>8;
	motor_clearerror[12] = motor_datarray[6];
	motor_clearerror[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);

//	HAL_UART_Transmit_DMA(&huart1, motor_clearerror, 14);
//	HAL_Delay(10);	
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6060;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x0003;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_model[0]  = motor_datarray[0]>>8;
	motor_model[1]  = motor_datarray[0];
	motor_model[2]  = motor_datarray[1];
	motor_model[3]  = motor_datarray[1]>>8;
	motor_model[4]  = motor_datarray[2];
	motor_model[5]  = motor_datarray[2]>>8;
	motor_model[6]  = motor_datarray[3];
	motor_model[7]  = motor_datarray[3]>>8;
	motor_model[8]  = motor_datarray[4];
	motor_model[9]  = motor_datarray[4]>>8;
	motor_model[10] = motor_datarray[5];
	motor_model[11] = motor_datarray[5]>>8;
	motor_model[12] = motor_datarray[6];
	motor_model[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);
//	HAL_UART_Transmit_DMA(&huart1, motor_model, 14);
//	HAL_Delay(10);	
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6086;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x0000;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_outline[0]  = motor_datarray[0]>>8;
	motor_outline[1]  = motor_datarray[0];
	motor_outline[2]  = motor_datarray[1];
	motor_outline[3]  = motor_datarray[1]>>8;
	motor_outline[4]  = motor_datarray[2];
	motor_outline[5]  = motor_datarray[2]>>8;
	motor_outline[6]  = motor_datarray[3];
	motor_outline[7]  = motor_datarray[3]>>8;
	motor_outline[8]  = motor_datarray[4];
	motor_outline[9]  = motor_datarray[4]>>8;
	motor_outline[10] = motor_datarray[5];
	motor_outline[11] = motor_datarray[5]>>8;
	motor_outline[12] = motor_datarray[6];
	motor_outline[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);
//	HAL_UART_Transmit_DMA(&huart1, motor_outline, 14);
//	HAL_Delay(10);	
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6040;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x0006;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_disable[0]  = motor_datarray[0]>>8;
	motor_disable[1]  = motor_datarray[0];
	motor_disable[2]  = motor_datarray[1];
	motor_disable[3]  = motor_datarray[1]>>8;
	motor_disable[4]  = motor_datarray[2];
	motor_disable[5]  = motor_datarray[2]>>8;
	motor_disable[6]  = motor_datarray[3];
	motor_disable[7]  = motor_datarray[3]>>8;
	motor_disable[8]  = motor_datarray[4];
	motor_disable[9]  = motor_datarray[4]>>8;
	motor_disable[10] = motor_datarray[5];
	motor_disable[11] = motor_datarray[5]>>8;
	motor_disable[12] = motor_datarray[6];
	motor_disable[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);
//	HAL_UART_Transmit_DMA(&huart1, motor_disable, 14);
//	HAL_Delay(10);
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6040;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x000f;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_enable[0]  = motor_datarray[0]>>8;
	motor_enable[1]  = motor_datarray[0];
	motor_enable[2]  = motor_datarray[1];
	motor_enable[3]  = motor_datarray[1]>>8;
	motor_enable[4]  = motor_datarray[2];
	motor_enable[5]  = motor_datarray[2]>>8;
	motor_enable[6]  = motor_datarray[3];
	motor_enable[7]  = motor_datarray[3]>>8;
	motor_enable[8]  = motor_datarray[4];
	motor_enable[9]  = motor_datarray[4]>>8;
	motor_enable[10] = motor_datarray[5];
	motor_enable[11] = motor_datarray[5]>>8;
	motor_enable[12] = motor_datarray[6];
	motor_enable[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);
//	HAL_UART_Transmit_DMA(&huart1, motor_enable, 14);
//	HAL_Delay(10);
}

/*************************
Function:    StraightMotorSetSpeed
Description: 设置直行电机转速，并使其运转
Input:       node-所需要设置的电机地址
History:
*************************/
void StraightMotorSetSpeed(uint16_t node)
{
	uint16_t motor_datarray[7] = {0};
	
	uint8_t node_tmp = node>>8;
	node = node<<8;
	node += node_tmp;
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x60ff;
	motor_datarray[2] = node;
	
	switch(node)
	{
		case(0x0100):motor_datarray[3] = smotor.motor_node1_rev; 
								 motor_datarray[4] = (smotor.motor_node1_rev>>16);
								 break;
		case(0x0200):motor_datarray[3] = smotor.motor_node2_rev; 
								 motor_datarray[4] = (smotor.motor_node2_rev>>16);
		             break;
		case(0x0300):motor_datarray[3] = smotor.motor_node3_rev; 
		             motor_datarray[4] = (smotor.motor_node3_rev>>16);
		             break;
		case(0x0400):motor_datarray[3] = smotor.motor_node4_rev; 
		             motor_datarray[4] = (smotor.motor_node4_rev>>16);
		             break;
		default:     break;
	}
  /*if(node == 0x0100 || node == 0x0200)
	{
		motor_datarray[3] = 0x0190;
		motor_datarray[4] = 0x0000;
	}
	else
	{
		motor_datarray[3] = 0xfe70;
		motor_datarray[4] = 0xffff;
	}*/
	
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_velocity[0]  = motor_datarray[0]>>8;
	motor_velocity[1]  = motor_datarray[0];
	motor_velocity[2]  = motor_datarray[1];
	motor_velocity[3]  = motor_datarray[1]>>8;
	motor_velocity[4]  = motor_datarray[2];
	motor_velocity[5]  = motor_datarray[2]>>8;
	motor_velocity[6]  = motor_datarray[3];
	motor_velocity[7]  = motor_datarray[3]>>8;
	motor_velocity[8]  = motor_datarray[4];
	motor_velocity[9]  = motor_datarray[4]>>8;
	motor_velocity[10] = motor_datarray[5];
	motor_velocity[11] = motor_datarray[5]>>8;
	motor_velocity[12] = motor_datarray[6];
	motor_velocity[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);
//	HAL_UART_Transmit(&huart1, motor_velocity, 14, 0xffff);
//	HAL_Delay(10);
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6040;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x000f;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_start[0]  = motor_datarray[0]>>8;
	motor_start[1]  = motor_datarray[0];
	motor_start[2]  = motor_datarray[1];
	motor_start[3]  = motor_datarray[1]>>8;
	motor_start[4]  = motor_datarray[2];
	motor_start[5]  = motor_datarray[2]>>8;
	motor_start[6]  = motor_datarray[3];
	motor_start[7]  = motor_datarray[3]>>8;
	motor_start[8]  = motor_datarray[4];
	motor_start[9]  = motor_datarray[4]>>8;
	motor_start[10] = motor_datarray[5];
	motor_start[11] = motor_datarray[5]>>8;
	motor_start[12] = motor_datarray[6];
	motor_start[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);
//	HAL_UART_Transmit(&huart1, motor_start, 14, 0xffff);
//	HAL_Delay(10);
}

void StraightMotorStop(uint16_t node)
{
	uint16_t motor_datarray[7] = {0};
	
	uint8_t node_tmp = node>>8;
	node = node<<8;
	node += node_tmp;
	
	motor_datarray[0] = 0x1103;
	motor_datarray[1] = 0x6040;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x010f;
	motor_datarray[4] =	0x0000;
	motor_datarray[5] = 0x0000;
	motor_datarray[6] = 0x4f4f;
	motor_datarray[5] = CalcFieldCRC((uint16_t*)motor_datarray,0x06);
	
	motor_stop[0]  = motor_datarray[0]>>8;
	motor_stop[1]  = motor_datarray[0];
	motor_stop[2]  = motor_datarray[1];
	motor_stop[3]  = motor_datarray[1]>>8;
	motor_stop[4]  = motor_datarray[2];
	motor_stop[5]  = motor_datarray[2]>>8;
	motor_stop[6]  = motor_datarray[3];
	motor_stop[7]  = motor_datarray[3]>>8;
	motor_stop[8]  = motor_datarray[4];
	motor_stop[9]  = motor_datarray[4]>>8;
	motor_stop[10] = motor_datarray[5];
	motor_stop[11] = motor_datarray[5]>>8;
	motor_stop[12] = motor_datarray[6];
	motor_stop[13] = motor_datarray[6]>>8;
	
//	HAL_UART_Receive_IT(&huart1, recv, 10);

//	HAL_UART_Transmit_DMA(&huart1, motor_stop, 14);
//	HAL_Delay(10);	
}

