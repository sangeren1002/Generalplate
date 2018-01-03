#ifndef __ODOMETEMODEL_H__
#define __ODOMETEMODEL_H__

//#include "stm32f4xx_hal.h"
#include "math.h"
#include "sys.h"

#define LENTH           0.73f       /* 轴距 单位m */
#define WIDTH           0.42f       /* 轮距 单位m */
#define RADIUS          0.15f       /* 车轮半径 单位m */
#define REDUCTION_RATIO 36.0f       /* 减速器减速比 */
#define LINENUM         500.0f      /* 直行电机线数 */
#define FMC             4.0f        /* 电机控制器倍频系数 */
#define PI              3.1415926f  /* π的定义 */

extern int32_t LeftWheelIncrement(int32_t New_Position, int32_t Old_Position, float Angle);
extern int32_t RightWheelIncrement(int32_t New_Position, int32_t Old_Position, float Angle);

#endif

