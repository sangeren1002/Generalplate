#include "encodertask.h"

Encoder sencoder;

void EncoderTask(void)
{
	sencoder.encoder_node1_angle = AskAngle(ENCODER_NODE1);
	sencoder.encoder_node2_angle = AskAngle(ENCODER_NODE2);
	sencoder.encoder_node3_angle = AskAngle(ENCODER_NODE3);
	sencoder.encoder_node4_angle = AskAngle(ENCODER_NODE4);
}

