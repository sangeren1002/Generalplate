#ifndef __STRAIGHTMOTORINQUIRE_H__
#define __STRAIGHTMOTORINQUIRE_H__

#include "usart.h"
#include "check.h"
#include "odometemodel.h"
#include "controlmodel.h"

/**************************************
           电机排列顺序
        2号 * * * * * 1号
				 *             *
				 *             *
				 *             *
				 *             *
				 *             *
				 *             *
				 *             *
				3号 * * * * * 4号
**************************************/

#define    NODE1    0x0001  /* 1号直行电机 */
#define		 NODE2	  0x0002  /* 2号直行电机 */
#define    NODE3    0x0003  /* 3号直行电机 */
#define    NODE4		0x0004  /* 4号直行电机 */

void InquireStraightMotorPosition(uint16_t node); /* 查询直行电机实际位置 */
void InquireStraightMotorCurrent(uint16_t node);  /* 查询直行电机实际电流 */
void UpdataLeftWheelIncrement(void);              /* 更新左虚拟轮增量 */
void UpdataRightWheelIncrement(void);             /* 更新右虚拟轮增量 */
void UpdataIncrement(void);

extern uint8_t motor_velocity_inquire[10];
extern uint8_t motor_velocity_receive[14];

extern int32_t LeftWheel_Increment;
extern int32_t RightWheel_Increment; 
extern int32_t LeftWheel_Total;
extern int32_t RightWheel_Total;

extern uint16_t Motor_Node1_AverageCurrent;
extern uint16_t Motor_Node2_AverageCurrent;
extern uint16_t Motor_Node3_AverageCurrent;
extern uint16_t Motor_Node4_AverageCurrent;



#endif
