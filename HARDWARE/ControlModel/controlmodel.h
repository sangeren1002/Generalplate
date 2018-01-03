#ifndef __CONTROLMODEL_H__
#define __CONTROLMODEL_H__

//#include "stm32f4xx_hal.h"
#include "math.h"
#include "sys.h"

#define LENTH           0.73f       /* 轴距 单位m */
#define WIDTH           0.42f       /* 轮距 单位m */
#define RADIUS          0.15f       /* 车轮半径 单位m */
#define REDUCTION_RATIO 36.0f       /* 减速器减速比 */
#define PI              3.1415926f  /* π的定义 */

typedef struct StraightMotor{       /* 直行电机转速结构体 */
	int32_t motor_node1_rev;
	int32_t motor_node2_rev;
	int32_t motor_node3_rev;
	int32_t motor_node4_rev;
}StraightMotor;

typedef struct SwerveMotor{         /* 转向电机转角结构体 */
	int32_t motor_node1_angle;
	int32_t motor_node2_angle;
	int32_t motor_node3_angle;
	int32_t motor_node4_angle;
}SwerveMotor;

void UpdataSteeringAngle(SwerveMotor   *motor, float Speed_Advance, float Speed_Angular);
void UpdataMotorRevSpeed(StraightMotor *motor, float Speed_Advance1, float Speed_Angular1);


#endif

