#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H
#include "sys.h"
#include "includes.h"
#include "straightmotorcontrol.h"
  
 typedef struct{
		u8 Head[2];									//帧头	0x88 0x66	
//		u8 TotalControlEnable;		//总控使能 0/Enable  1/Disable
		u8 LeftWheelIncrement[4];				//左轮增量  量程0-4228.250625KM
		u8 RightWheelIncrement[4];				//右轮增量  量程0-4228.250625KM
		u8 CCWAngleIncrement[2];		//逆时针角度增量(0.1°)
		u8 EncoderErrorCode;			//编码器故障
		u8 SpeedErrorCode;				//电机转速故障
		u8 AngleErrorCode;				//电机角度故障
		u8 DriverErrorCode[2];				//驱动器故障码
		u8 IMUErrorCode;					//IMU故障码
		u8 CurrentBatteryLevel;		//当前电量百分比
		u8 BatteryErrorCode;			//电池故障码
		u8 ChargingPileAngle;			//充电桩偏角
		u8 ChargingCircuitCurrent;//充电回路电流标志
		u8 LocationErrorCode;			//定位故障码
		u8 UltrasonicDistanceNo1[2];//1号超声波距离
		u8 UltrasonicDistanceNo2[2];//2号超声波距离
		u8 UltrasonicDistanceNo3[2];//3号超声波距离
		u8 UltrasonicDistanceNo4[2];//4号超声波距离
	 	u8 UltrasonicDistanceNo5[2];//5号超声波距离
		u8 UltrasonicDistanceNo6[2];//6号超声波距离
		u8 UltrasonicDistanceNo7[2];//7号超声波距离
		u8 UltrasonicDistanceNo8[2];//8号超声波距离
		u8 UltrasonicErrorCode;		//超声波故障码
		u8 InfraredRanging[4];			//红外测距  1/车轮悬空 0/车轮未悬空 InfraredRanging[0] 红外1 InfraredRanging[1] 红外2 InfraredRanging[2] 红外3 InfraredRanging[3]红外4  红外测距量程0-255mm
		u8 InfraredErrorCode;			//红外测距故障码
		u8 CrashSensorStatus[2];		//碰撞传感器状态 CrashSensorStatus[0] 传感器1状态 CrashSensorStatus[1]传感器2状态
		u8 CommunicationErrorCode;//碰撞 传感器错误码
		u8 Reserved1;						//保留位
		u8 Reserved2;						//保留位
		u8 Reserved3;						//保留位
		u8 Reserved4;						//保留位
		u8 Timestamp;							//时间戳
		u8 CRCCheck[2];							//CRC校验
 }TCP_SEND_PACK;


#define TCP_SERVER_RX_BUFSIZE	32		//定义tcp server所接受的单次最大数据个数
#define TCP_SERVER_PORT			8088	//定义tcp server端口号
#define LWIP_SEND_DATA			0X80	//定义有数据要发送

extern u8 TCP_SEND_PACK_BUFSIZE;
 extern u8 tcp_server_flag;			//TCP服务器数据发送标志位


extern TCP_SEND_PACK Tcp_Server_Recvconbuf;

u8 tcp_server_init(void);		//TCP服务器初始化(创建TCP服务器任务)
#endif

