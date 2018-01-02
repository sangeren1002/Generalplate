#include "tcp_server_demo.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
#include "led.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//NETCONN API��̷�ʽ��TCP���������Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 

 typedef struct{
		u8 Head[2];									//֡ͷ1		0x88 0x66	
		u8 TotalControlEnable;		//�ܿ�ʹ��  0/Enable  1/Disable
		u8 LeftWheelIncrement[4];				//������������λmm ����Ϊ0-4228.250625KM
		u8 RightWheelIncrement[4];				//������������λmm  ����Ϊ0-4228.250625KM
		u8 CCWAngleIncrement[2];		//��ʱ��Ƕ�����(0.1��)
		u8 EncoderErrorCode;			//������������
		u8 SpeedErrorCode;				//���ת��������
		u8 AngleErrorCode;				//����Ƕȹ�����
		u8 DriverErrorCode[2];				//������������
		u8 IMUErrorCode;					//IMU������
		u8 CurrentBatteryLevel;		//��ǰ�����ٷֱ�
		u8 BatteryErrorCode;			//��ع�����
		u8 ChargingPileAngle;			//���׮ƫ��
		u8 ChargingCircuitCurrent;//����·������־
		u8 LocationErrorCode;			//��λ������
		u8 UltrasonicDistanceNo1[2];//1�ų���������
		u8 UltrasonicDistanceNo2[2];//2�ų���������
		u8 UltrasonicDistanceNo3[2];//3�ų���������
		u8 UltrasonicDistanceNo4[2];//4�ų���������
	 	u8 UltrasonicDistanceNo5[2];//5�ų���������
		u8 UltrasonicDistanceNo6[2];//6�ų���������
		u8 UltrasonicDistanceNo7[2];//7�ų���������
		u8 UltrasonicDistanceNo8[2];//8�ų���������
		u8 UltrasonicErrorCode;		//������������
		u8 InfraredRanging[4];			//������   1/�������� 0/����δ���� InfraredRanging[0] ����1 InfraredRanging[1] ����2 InfraredRanging[2] ����3 InfraredRanging[3]����4  ��෶Χ0-255mm
		u8 InfraredErrorCode;			//�����������
		u8 CrashSensorStatus[2];		//��ײ������״̬ CrashSensorStatus[0] ������1״̬ CrashSensorStatus[1]������2״̬
		u8 CommunicationErrorCode;//��ײ������������
		u8 Reserved1;						//����λ
		u8 Reserved2;						//����λ
		u8 Reserved3;						//����λ
		u8 Reserved4;						//����λ
		u8 Timestamp;							//ʱ���
		u8 CRCCheck[2];							//CRCУ��
 }TCP_PACK;
u8 TCP_PACK_BUFSIZE=56;
TCP_PACK Tcp_Server_Recvbuf;
TCP_PACK Tcp_Server_Transbuf;
u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
u8 *tcp_server_sendbuf="Explorer STM32F407 NETCONN TCP Server send data\r\n";	
u8 tcp_server_flag;								//TCP���������ݷ��ͱ�־λ

//TCP�ͻ�������
#define TCPSERVER_PRIO		6
//�����ջ��С
#define TCPSERVER_STK_SIZE	300
//������ƿ�
OS_TCB TCPSERVERTaskTCB;
//�����ջ
CPU_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];
  

//tcp����������
static void tcp_server_thread(void *arg)
{
//	OS_ERR err_os;
	CPU_SR_ALLOC();
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	u8 remot_addr[4];
	struct netconn *conn, *newconn;
	static ip_addr_t ipaddr;
	static u16_t 			port;
	 
	LWIP_UNUSED_ARG(arg);

	conn = netconn_new(NETCONN_TCP);  //����һ��TCP����
	netconn_bind(conn,IP_ADDR_ANY,TCP_SERVER_PORT);  //�󶨶˿� 8�Ŷ˿�
	netconn_listen(conn);  		//�������ģʽ
	conn->recv_timeout = 10;  	//��ֹ�����߳� �ȴ�10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //������������
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //���������ӵ�����
		{ 
			struct netbuf *recvbuf;

			netconn_getaddr(newconn,&ipaddr,&port,0); //��ȡԶ��IP��ַ�Ͷ˿ں�
			
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
			printf("����%d.%d.%d.%d�����Ϸ�����,�����˿ں�Ϊ:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
			Tcp_Server_Transbuf.Head[0] =0x66;
			Tcp_Server_Transbuf.Head[1] =0x88;
			Tcp_Server_Transbuf.ChargingPileAngle =0xcf;
			while(1)
			{
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
				{
//					err = netconn_write(newconn ,tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf),NETCONN_COPY); //����tcp_server_sendbuf�е�����
					
					err = netconn_write(newconn ,&Tcp_Server_Transbuf,TCP_PACK_BUFSIZE,NETCONN_COPY); //����tcp_server_sendbuf�е�����
					if(err != ERR_OK)
					{
						printf("����ʧ��\r\n");
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//���յ�����
				{	
					OS_CRITICAL_ENTER();	//���ж�					
					memset(&Tcp_Server_Recvbuf,0,TCP_PACK_BUFSIZE);  //���ݽ��ջ���������
//					memset(tcp_server_recvbuf,0,TCP_PACK_BUFSIZE);  //���ݽ��ջ���������
					for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
						if(q->len > (TCP_PACK_BUFSIZE-data_len)) memcpy(&Tcp_Server_Recvbuf+data_len,q->payload,(TCP_PACK_BUFSIZE-data_len));//��������
						else memcpy(&Tcp_Server_Recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_PACK_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
					}
					OS_CRITICAL_EXIT();	//���ж�
					data_len=0;  //������ɺ�data_lenҪ����
					if((Tcp_Server_Recvbuf.Head[0] == 0x88)&&(Tcp_Server_Recvbuf.Head[1] == 0x66))
					{					
						printf("Tcp_Server_Recvbuf.Head check ok head1==0x88 head2==0x66\r\n");  //��������
					}
					LCD_ShowString(30,270,210,200,16,(u8*)&Tcp_Server_Recvbuf);
					printf("rece:%s\r\n",(char*)&Tcp_Server_Recvbuf);  //ͨ�����ڷ��ͽ��յ�������
					memset(&Tcp_Server_Recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
					netbuf_delete(recvbuf);
				}else if(recv_err == ERR_CLSD)  //�ر�����
				{
					netconn_close(newconn);
					netconn_delete(newconn);
					printf("����:%d.%d.%d.%d�Ͽ��������������\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
					break;
				}
				
			}
		}
	}
}


//����TCP�������߳�
//����ֵ:0 TCP�����������ɹ�
//		���� TCP����������ʧ��
u8 tcp_server_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����TCP�������߳�
	OSTaskCreate((OS_TCB 	* )&TCPSERVERTaskTCB,		
				 (CPU_CHAR	* )"tcp server task", 		
                 (OS_TASK_PTR )tcp_server_thread, 			
                 (void		* )0,					
                 (OS_PRIO	  )TCPSERVER_PRIO,     	
                 (CPU_STK   * )&TCPSERVER_TASK_STK[0],	
                 (CPU_STK_SIZE)TCPSERVER_STK_SIZE/10,	
                 (CPU_STK_SIZE)TCPSERVER_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//�˳��ٽ���
	return 0;
}







