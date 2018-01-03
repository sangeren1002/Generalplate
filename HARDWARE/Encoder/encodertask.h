#ifndef __ENCODERTASK_H__
#define __ENCODERTASK_H__

#include "encoderinquire.h"
#include "stm32f4xx_hal.h"

#define ENCODER_NODE1  0x01
#define ENCODER_NODE2  0x02
#define ENCODER_NODE3  0x03
#define ENCODER_NODE4  0x04

void EncoderTask(void);

typedef struct Encoder{        /* 绝对值编码器角度结构体 */
	int32_t encoder_node1_angle;
	int32_t encoder_node2_angle;
	int32_t encoder_node3_angle;
	int32_t encoder_node4_angle;
}Encoder;

#endif

