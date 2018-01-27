/********************************************
File name:   encoder.h
Author:      WXK
Date:        2017.10.20
Description: ?????????????????????????????????
History:
********************************************/

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx_it.h"
#include "usart.h"
#include "sys.h"
#include "stdint.h"

#define    ENCODERNODE1    0x04
#define		 ENCODERNODE2	   0x02
#define    ENCODERNODE3    0x03
#define    ENCODERNODE4		 0x01

void InquireSwerveMotorAngle(void);
float AskAngle(uint8_t addr);
void TurnMotorToZero(void);


extern float Swerve_Node1_Angle;
extern float Swerve_Node2_Angle;
extern float Swerve_Node3_Angle;
extern float Swerve_Node4_Angle;
#endif

