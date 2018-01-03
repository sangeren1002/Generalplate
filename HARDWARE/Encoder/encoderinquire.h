/********************************************
File name:   encoderinquire.h
Author:      WXK
Date:        2017.10.20
Description: 为逻辑功能层中的查询绝对值编码器角度值功能提供必要的数值、函数接口
History：
********************************************/

#ifndef __ENCODERINQUIRE_H__
#define __ENCODERINQUIRE_H__

#include "stm32f4xx_hal.h"
#include "usart.h"

#define    ENCODERNODE1    0x01
#define		 ENCODERNODE2	  0x02
#define    ENCODERNODE3    0x03
#define    ENCODERNODE4		0x04

float AskAngle(uint8_t addr);
void InquireSwerveMotorAngle(void);

extern float Swerve_Node1_Angle;
extern float Swerve_Node2_Angle;
extern float Swerve_Node3_Angle;
extern float Swerve_Node4_Angle;
#endif

