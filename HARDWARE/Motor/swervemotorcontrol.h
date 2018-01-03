#ifndef __SWERVEMOTORCONTROL_H__
#define __SWERVEMOTORCONTROL_H__

#include "os.h"
#include "sys.h"

extern float gSpeed_Advance;    /* 整体线速度V 单位cm/s */
extern float gSpeed_Angular;    /* 角速度W     单位°/s  */


void Motor_PulsePositionSet(uint8_t addr, float PositionSet);

#endif
