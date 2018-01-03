/**********************************************************************
File name:   straightmotorinquire.c
Author:      WXK
Data:        2017.10.26
Description: 直行电机信息查询逻辑功能层函数，主要描述用哪些硬件接口实现功能
History:
***********************************************************************/
#include "straightmotorinquire.h"

float   Swerve_Left_Angle          =   0.0f;      /* 左侧转向电机转向角 */
float   Swerve_Right_Angle         =   0.0f;      /* 右侧转向电机转向角 */
int32_t LeftWheel_Increment        =   0;         /* 左虚拟轮增量 */
int32_t RightWheel_Increment       =   0;         /* 右虚拟轮增量 */
int32_t LeftWheel_Total            =   0;         /* 左虚拟轮里程计累计量 */
int32_t RightWheel_Total           =   0;         /* 右虚拟轮里程计累计量 */

uint16_t Motor_Node1_AverageCurrent =  0;         /* 直行电机node1实时平均电流 */
uint16_t Motor_Node2_AverageCurrent =  0;         /* 直行电机node2实时平均电流 */
uint16_t Motor_Node3_AverageCurrent =  0;         /* 直行电机node3实时平均电流 */
uint16_t Motor_Node4_AverageCurrent =  0;         /* 直行电机node4实时平均电流 */

int32_t New_Motor_Node1_Position   =   0;         /* 直行电机node1实际位置记录 */ 
int32_t New_Motor_Node2_Position   =   0;         /* 直行电机node2实际位置记录 */
int32_t New_Motor_Node3_Position   =   0;         /* 直行电机node3实际位置记录 */
int32_t New_Motor_Node4_Position   =   0;         /* 直行电机node4实际位置记录 */
int32_t Old_Motor_Node1_Position   =   0;         /* 直行电机node1相较于实际位置的上一次记录 */ 
int32_t Old_Motor_Node2_Position   =   0;         /* 直行电机node2相较于实际位置的上一次记录 */ 
int32_t Old_Motor_Node3_Position   =   0;         /* 直行电机node3相较于实际位置的上一次记录 */ 
int32_t Old_Motor_Node4_Position   =   0;         /* 直行电机node4相较于实际位置的上一次记录 */

uint8_t motor_readposition[10]     =  {0};        /* 直行电机查询当前实际位置 */
uint8_t motor_readcurrent[10]      =  {0};        /* 直行电机查询当前实际电流 */
uint8_t motor_node1_current[14]    =  {0};        /* 直行电机node1当前电流串口接收缓存区*/
uint8_t motor_node2_current[14]    =  {0};        /* 直行电机node1当前电流串口接收缓存区*/
uint8_t motor_node3_current[14]    =  {0};        /* 直行电机node1当前电流串口接收缓存区*/
uint8_t motor_node4_current[14]    =  {0};        /* 直行电机node1当前电流串口接收缓存区*/
uint8_t motor_node1_position[14]   =  {0};        /* 直行电机node1实际位置串口接收缓存区*/
uint8_t motor_node2_position[14]   =  {0};	      /* 直行电机node2实际位置串口接收缓存区*/
uint8_t motor_node3_position[14]   =  {0};	      /* 直行电机node3实际位置串口接收缓存区*/
uint8_t motor_node4_position[14]   =  {0};	      /* 直行电机node4实际位置串口接收缓存区*/

SwerveMotor swermotor;
/*************************
Function:    InquireStraightMotorCurrent
Description: 查询电机实时平均电流 
Input:       node-所需要查询的电机地址
History:
*************************/
void InquireStraightMotorCurrent(uint16_t node)
{
	uint16_t motor_datarray[5] = {0};
	
	uint8_t node_tmp = node>>8;
	node = node<<8;
	node += node_tmp;
	
	motor_datarray[0] = 0x1001;
	motor_datarray[1] = 0x2027;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x0000;
	motor_datarray[4] =	0x4f4f;
	motor_datarray[3] = CalcFieldCRC((uint16_t*)motor_datarray,0x04);
	
	motor_readcurrent[0] = motor_datarray[0]>>8;
	motor_readcurrent[1] = motor_datarray[0];
	motor_readcurrent[2] = motor_datarray[1];
	motor_readcurrent[3] = motor_datarray[1]>>8;
	motor_readcurrent[4] = motor_datarray[2];
	motor_readcurrent[5] = motor_datarray[2]>>8;
	motor_readcurrent[6] = motor_datarray[3];
	motor_readcurrent[7] = motor_datarray[3]>>8;
	motor_readcurrent[8] = motor_datarray[4];
	motor_readcurrent[9] = motor_datarray[4]>>8;
	
//	switch(node)
//	{
//		case(0x0100):HAL_UART_Receive_IT(&huart1, motor_node1_current, 14);
//		             break;
//		case(0x0200):HAL_UART_Receive_IT(&huart1, motor_node2_current, 14);
//		             break;
//		case(0x0300):HAL_UART_Receive_IT(&huart1, motor_node3_current, 14);
//		             break;
//		case(0x0400):HAL_UART_Receive_IT(&huart1, motor_node4_current, 14);
//		             break;
//		default:     break;
//	}
//	
//	HAL_UART_Transmit(&huart1, motor_readcurrent, 10, 0xffff);
//	
//	HAL_Delay(10);
	
	switch(node)
	{
		case(0x0100):Motor_Node1_AverageCurrent = 0;
			           Motor_Node1_AverageCurrent = motor_node1_current[9];
		             Motor_Node1_AverageCurrent = Motor_Node1_AverageCurrent<<8;
		             Motor_Node1_AverageCurrent += motor_node1_current[8];
								 if(Motor_Node1_AverageCurrent > 60000)
								 {
									 Motor_Node1_AverageCurrent = 65535 - Motor_Node1_AverageCurrent;
								 }
		             break;
		case(0x0200):Motor_Node2_AverageCurrent = 0;
								 Motor_Node2_AverageCurrent = motor_node2_current[9];
		             Motor_Node2_AverageCurrent = Motor_Node2_AverageCurrent<<8;
		             Motor_Node2_AverageCurrent += motor_node2_current[8];
								 if(Motor_Node2_AverageCurrent > 60000)
								 {
									 Motor_Node2_AverageCurrent = 65535 - Motor_Node2_AverageCurrent;
								 }
		             break;
		case(0x0300):Motor_Node3_AverageCurrent = 0;
			           Motor_Node3_AverageCurrent = motor_node3_current[9];
		             Motor_Node3_AverageCurrent = Motor_Node3_AverageCurrent<<8;
		             Motor_Node3_AverageCurrent += motor_node3_current[8];
								 if(Motor_Node3_AverageCurrent > 60000)
								 {
									 Motor_Node3_AverageCurrent = 65535 - Motor_Node3_AverageCurrent;
								 }
		             break;
		case(0x0400):Motor_Node4_AverageCurrent = 0;
			           Motor_Node4_AverageCurrent = motor_node4_current[9];
		             Motor_Node4_AverageCurrent = Motor_Node4_AverageCurrent<<8;
		             Motor_Node4_AverageCurrent += motor_node4_current[8];
								 if(Motor_Node4_AverageCurrent > 60000)
								 {
									 Motor_Node4_AverageCurrent = 65535 - Motor_Node4_AverageCurrent;
								 }
		             break;
		default:     break;
	}
	
}

/*************************
Function:    InquireStraightMotorPosition
Description: 查询电机当前位置，用于计算里程计增量
Input:       node-所需要查询的电机地址
History:
*************************/
void InquireStraightMotorPosition(uint16_t node)
{
	uint16_t motor_datarray[5] = {0};
	
	uint8_t node_tmp = node>>8;
	node = node<<8;
	node += node_tmp;
	
	motor_datarray[0] = 0x1001;
	motor_datarray[1] = 0x6064;
	motor_datarray[2] = node;
	motor_datarray[3] = 0x0000;
	motor_datarray[4] =	0x4f4f;
	motor_datarray[3] = CalcFieldCRC((uint16_t*)motor_datarray,0x04);
	
	motor_readposition[0] = motor_datarray[0]>>8;
	motor_readposition[1] = motor_datarray[0];
	motor_readposition[2] = motor_datarray[1];
	motor_readposition[3] = motor_datarray[1]>>8;
	motor_readposition[4] = motor_datarray[2];
	motor_readposition[5] = motor_datarray[2]>>8;
	motor_readposition[6] = motor_datarray[3];
	motor_readposition[7] = motor_datarray[3]>>8;
	motor_readposition[8] = motor_datarray[4];
	motor_readposition[9] = motor_datarray[4]>>8;
	
//	switch(node)
//	{
//		case(0x0100):HAL_UART_Receive_IT(&huart2, motor_node1_position, 14);
//		             break;
//		case(0x0200):HAL_UART_Receive_IT(&huart2, motor_node2_position, 14);
//		             break;
//		case(0x0300):HAL_UART_Receive_IT(&huart2, motor_node3_position, 14);
//		             break;
//		case(0x0400):HAL_UART_Receive_IT(&huart2, motor_node4_position, 14);
//		             break;
//		default:     break;
//	}
//	
//	HAL_UART_Transmit(&huart2, motor_readposition, 10, 0xffff);
//	HAL_Delay(10);
	
	switch(node)
	{
		case(0x0100):
			if(motor_node1_position[0] == 0x4f && motor_node1_position[1] == 0x4f 
				 && motor_node1_position[2] == 0x00 && motor_node1_position[3] == 0x03)
			{
				if(motor_node1_position[8] == 0xfe && motor_node1_position[9] == 0xff 
					 && motor_node1_position[10] == 0xff && motor_node1_position[11] == 0xff)
				{
					New_Motor_Node1_Position = 0;
				}
				else
				{
					New_Motor_Node1_Position = (int32_t)((uint32_t)motor_node1_position[8]
																								+ ((((uint32_t)motor_node1_position[9]) 
					                                      + ((((uint32_t)motor_node1_position[10])
                                      					+ ((uint32_t)(motor_node1_position[11])<<8))<<8))<<8));
				}
			}
			break;
		case(0x0200):
			if(motor_node2_position[0] == 0x4f && motor_node2_position[1] == 0x4f 
				 && motor_node2_position[2] == 0x00 && motor_node2_position[3] == 0x03)
			{
				if(motor_node2_position[8] == 0xfe && motor_node2_position[9] == 0xff 
					 && motor_node2_position[10] == 0xff && motor_node2_position[11] == 0xff)
				{
					New_Motor_Node2_Position = 0;
				}
				else
				{
					New_Motor_Node2_Position = (int32_t)((uint32_t)motor_node2_position[8]
																								+ ((((uint32_t)motor_node2_position[9]) 
					                                      + ((((uint32_t)motor_node2_position[10])
                                      					+ ((uint32_t)(motor_node2_position[11])<<8))<<8))<<8));
				}
			}
			break;
		case(0x0300):
			if(motor_node3_position[0] == 0x4f && motor_node3_position[1] == 0x4f 
				 && motor_node3_position[2] == 0x00 && motor_node3_position[3] == 0x03)
			{
				if(motor_node3_position[8] == 0xfe && motor_node3_position[9] == 0xff 
					 && motor_node3_position[10] == 0xff && motor_node3_position[11] == 0xff)
				{
					New_Motor_Node3_Position = 0;
				}
				else
				{
					New_Motor_Node3_Position = (int32_t)((uint32_t)motor_node3_position[8]
																								+ ((((uint32_t)motor_node3_position[9]) 
					                                      + ((((uint32_t)motor_node3_position[10])
                                      					+ ((uint32_t)(motor_node3_position[11])<<8))<<8))<<8));
				}
			}
			break;
		case(0x0400):
			if(motor_node4_position[0] == 0x4f && motor_node4_position[1] == 0x4f 
				 && motor_node4_position[2] == 0x00 && motor_node4_position[3] == 0x03)
			{
				if(motor_node4_position[8] == 0xfe && motor_node4_position[9] == 0xff 
					 && motor_node4_position[10] == 0xff && motor_node4_position[11] == 0xff)
				{
					New_Motor_Node4_Position = 0;
				}
				else
				{
					New_Motor_Node4_Position = (int32_t)((uint32_t)motor_node4_position[8]
																								+ ((((uint32_t)motor_node4_position[9]) 
					                                      + ((((uint32_t)motor_node4_position[10])
                                      					+ ((uint32_t)(motor_node4_position[11])<<8))<<8))<<8));
				}
			}
			break;
		default:break;
	}
}

/*************************
Function:    InquireStraightMotorPosition
Description: 计算左虚拟轮里程计增量，将当前电机位置保存入历史数据中
History:
*************************/
void UpdataLeftWheelIncrement(void)
{
	Swerve_Left_Angle = swermotor.motor_node1_angle;
	InquireStraightMotorPosition(NODE1);
	LeftWheel_Increment = LeftWheelIncrement(New_Motor_Node1_Position, Old_Motor_Node1_Position, Swerve_Left_Angle);
	Old_Motor_Node1_Position = New_Motor_Node1_Position;
	
	LeftWheel_Total += LeftWheel_Increment;
}

/*************************
Function:    InquireStraightMotorPosition
Description: 计算右虚拟轮里程计增量，将当前电机位置保存入历史数据中
History:
*************************/
void UpdataRightWheelIncrement(void)
{
	Swerve_Right_Angle = swermotor.motor_node4_angle;
	InquireStraightMotorPosition(NODE4);
	RightWheel_Increment = RightWheelIncrement(New_Motor_Node4_Position, Old_Motor_Node4_Position, Swerve_Right_Angle);
	Old_Motor_Node4_Position = New_Motor_Node4_Position;
	
	RightWheel_Total += RightWheel_Increment;
}

void UpdataIncrement(void)
{
	UpdataLeftWheelIncrement();
	UpdataRightWheelIncrement();
}

