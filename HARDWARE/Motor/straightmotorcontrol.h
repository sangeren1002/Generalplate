#ifndef __STRAIGHTMOTORCONTROL_H__
#define __STRAIGHTMOTORCONTROL_H__

//#include "stm32f4xx_hal.h"
#include "usart.h"
#include "check.h"
#include "controlmodel.h"

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

extern void StartMotor(void);
extern void MotorInit(void);
extern void StopMotor(void); 
extern void StraightMotorInit(uint16_t node);
extern void ReadStraightMotorPosition(uint16_t node);
extern void StraightMotorStop(uint16_t node);

extern uint8_t motor_disaeld[14];
extern uint8_t motor_enaeld[14];
extern uint8_t motor_velocity[14];
extern uint8_t motor_start[14];
extern uint8_t motor_stop[14];
extern uint8_t recv[10];
extern float gSpeed_Advance;
extern float gSpeed_Angular;
extern StraightMotor smotor;

#endif

