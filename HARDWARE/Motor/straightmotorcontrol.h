#ifndef __STRAIGHTMOTORCONTROL_H__
#define __STRAIGHTMOTORCONTROL_H__

#include "usart.h"
#include "check.h"
#include "controlmodel.h"
#include "delay.h"

/**************************************
           电机排列顺序
        1号 * * * * * 4号
				 *             *
				 *             *
				 *             *
				 *             *
				 *             *
				 *             *
				 *             *
				2号 * * * * * 3号
**************************************/

#define    NODE1    0x0001  /* 1号直行电机 */
#define		 NODE2	  0x0002  /* 2号直行电机 */
#define    NODE3    0x0003  /* 3号直行电机 */
#define    NODE4		0x0004  /* 4号直行电机 */

void StraightMotorInit(uint16_t node);
void StraightMotorSetSpeed(uint16_t node);       /* 直行电机设置速度并使能 */
void StraightMotorStop(uint16_t node);
void StraightMotorSetSpeed(uint16_t node);
#endif

