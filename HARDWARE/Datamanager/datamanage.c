#include "datamanage.h"


void DataManage(void)
{
	int32_t speedtemp = 0;
	int32_t angletemp = 0;

	speedtemp |= ((uint32_t)tcp_server_recvbuf[7])<<24;
	speedtemp |= ((uint32_t)tcp_server_recvbuf[8])<<16;
	speedtemp |= ((uint32_t)tcp_server_recvbuf[9])<<8;
	speedtemp |=  tcp_server_recvbuf[10];	
	gSpeed_Advance = speedtemp;
	//Speed_Advance = 10.0;
	
	angletemp |= ((uint32_t)tcp_server_recvbuf[11])<<24;
	angletemp |= ((uint32_t)tcp_server_recvbuf[12])<<16;
	angletemp |= ((uint32_t)tcp_server_recvbuf[13])<<8;
	angletemp |= tcp_server_recvbuf[14];
	gSpeed_Angular = angletemp;
	//Speed_Angular = 100.0;

	tcp_server_sendbuf[0] = 0x88;
	tcp_server_sendbuf[1] = 0x66;
	
	tcp_server_sendbuf[2] = LeftWheel_Increment>>24;
	tcp_server_sendbuf[3] = LeftWheel_Increment>>16;
	tcp_server_sendbuf[4] = LeftWheel_Increment>>8;
	tcp_server_sendbuf[5] = LeftWheel_Increment;
	
	tcp_server_sendbuf[6] = RightWheel_Increment>>24;
	tcp_server_sendbuf[7] = RightWheel_Increment>>16;
	tcp_server_sendbuf[8] = RightWheel_Increment>>8;
	tcp_server_sendbuf[9] = RightWheel_Increment;
	
	/*tcp_server_sendbuf[20] = UPA0_distance>>8;
	tcp_server_sendbuf[21] = UPA0_distance;
	tcp_server_sendbuf[22] = UPA1_distance>>8;
	tcp_server_sendbuf[23] = UPA1_distance;
	tcp_server_sendbuf[24] = UPA2_distance>>8;
	tcp_server_sendbuf[25] = UPA2_distance;
	tcp_server_sendbuf[26] = UPA3_distance>>8;
	tcp_server_sendbuf[27] = UPA3_distance;
	tcp_server_sendbuf[28] = UPB0_distance>>8;
	tcp_server_sendbuf[29] = UPB0_distance;
	tcp_server_sendbuf[30] = UPB1_distance>>8;
	tcp_server_sendbuf[31] = UPB1_distance;
	tcp_server_sendbuf[32] = UPB2_distance>>8;
	tcp_server_sendbuf[33] = UPB2_distance;
	tcp_server_sendbuf[34] = UPB3_distance>>8;
	tcp_server_sendbuf[35] = UPB3_distance;*/
	
	/*tcp_server_sendbuf[37] = Fall_Flag;
	
	SensorArray[4] = UPA0_distance>>8;
	SensorArray[5] = UPA0_distance;
	SensorArray[8] = UPA1_distance>>8;
	SensorArray[9] = UPA1_distance;
	SensorArray[12] = UPA2_distance>>8;
	SensorArray[13] = UPA2_distance;
	SensorArray[16] = UPA3_distance>>8;
	SensorArray[17] = UPA3_distance;
	SensorArray[20] = UPB0_distance>>8;
	SensorArray[21] = UPB0_distance;
	SensorArray[24] = UPB1_distance>>8;
	SensorArray[25] = UPB1_distance;
	SensorArray[28] = UPB2_distance>>8;
	SensorArray[29] = UPB2_distance;
	SensorArray[32] = UPB3_distance>>8;
	SensorArray[33] = UPB3_distance;
	
	MotorArray[4] = LeftWheel_Total>>24;
	MotorArray[5] = LeftWheel_Total>>16;
	MotorArray[6] = LeftWheel_Total>>8;
	MotorArray[7] = LeftWheel_Total;
	MotorArray[8] = RightWheel_Total>>24;
	MotorArray[9] = RightWheel_Total>>16;
	MotorArray[10] = RightWheel_Total>>8;
	MotorArray[11] = RightWheel_Total;*/
}

