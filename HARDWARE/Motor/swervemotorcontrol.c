#include "swervemotorcontrol.h"
#include "controlmodel.h"


SwerveMotor swmotor;                            /* 直行电机结构体类型全局变量 */

/*转向电机控制转向函数*/
void swerve(SwerveMotor* swmotor_angle)
{
	UpdataSteeringAngle(&swmotor, gSpeed_Advance, gSpeed_Angular);
	Motor_PulsePositionSet(0x01, swmotor.motor_node1_angle);

}

/*************************
Function:    Motor_PulsePositionSet
Description: 设置电机的脉冲指定位置 
Input:       huartx-串口x
             addr-电机编号（RTU地址）
             PositionSet-电机旋转角度（电机的指定位置）（单位°），顺时针为正，逆时针为负
History:
*************************/
void Motor_PulsePositionSet(uint8_t addr, float PositionSet)  
{
	int32_t Position;
	uint8_t PulsePositionSet[] = {0xff, 0x10, 0x00, 0x2E, 0x00, 0x02, 0x04, 0xff, 
	                              0xff, 0xff, 0xff, 0xff, 0xff};   /* 初始化电机旋转角度（指定位置）的命令 */
	
	PulsePositionSet[0] = addr;
																
	 Position = PositionSet/360*2500;
																
	PulsePositionSet[sizeof(PulsePositionSet)-3] = Position & 0x000000ff;
	PulsePositionSet[sizeof(PulsePositionSet)-4] = (Position & 0x0000ff00)>>8;
	PulsePositionSet[sizeof(PulsePositionSet)-5] = (Position & 0x00ff0000)>>16;
	PulsePositionSet[sizeof(PulsePositionSet)-6] = (Position & 0xff000000)>>24;
																
//	uint16_t crc = UART_CRC16_Work(PulsePositionSet, sizeof(PulsePositionSet)-2); 
//																
//	PulsePositionSet[sizeof(PulsePositionSet)-2] = crc & 0x00ff;
//	PulsePositionSet[sizeof(PulsePositionSet)-1] = (crc & 0xff00)>>8;
//																
//	HAL_UART_Receive_IT(&huart2, recv485, 8);
//	HAL_GPIO_WritePin(RS485_control_GPIO_Port, RS485_control_Pin, GPIO_PIN_SET);
//	HAL_UART_Transmit(huartx, PulsePositionSet, sizeof(PulsePositionSet), 0xffff); 
//	HAL_GPIO_WritePin(RS485_control_GPIO_Port, RS485_control_Pin, GPIO_PIN_RESET);
//																
//	HAL_Delay(10);
}
