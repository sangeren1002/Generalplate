#include "tcp_server_demo.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
#include "led.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//NETCONN API±à³Ì·½Ê½µÄTCP·þÎñÆ÷²âÊÔ´úÂë	   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2014/8/15
//°æ±¾£ºV1.0
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//ÐÞ¸ÄÐÅÏ¢
//ÎÞ
////////////////////////////////////////////////////////////////////////////////// 	   
 

 typedef struct{
		u8 Head[2];									//Ö¡Í·1		0x88 0x66	
		u8 TotalControlEnable;		//×Ü¿ØÊ¹ÄÜ  0/Enable  1/Disable
		u8 LeftWheelIncrement[4];				//×óÂÖÔöÁ¿£¬µ¥Î»mm Á¿³ÌÎª0-4228.250625KM
		u8 RightWheelIncrement[4];				//ÓÒÂÖÔöÁ¿£¬µ¥Î»mm  Á¿³ÌÎª0-4228.250625KM
		u8 CCWAngleIncrement[2];		//ÄæÊ±Õë½Ç¶ÈÔöÁ¿(0.1¡ã)
		u8 EncoderErrorCode;			//±àÂëÆ÷¹ÊÕÏÂë
		u8 SpeedErrorCode;				//µç»ú×ªÊý¹ÊÕÏÂë
		u8 AngleErrorCode;				//µç»ú½Ç¶È¹ÊÕÏÂë
		u8 DriverErrorCode[2];				//Çý¶¯Æ÷¹ÊÕÏÂë
		u8 IMUErrorCode;					//IMU¹ÊÕÏÂë
		u8 CurrentBatteryLevel;		//µ±Ç°µçÁ¿°Ù·Ö±È
		u8 BatteryErrorCode;			//µç³Ø¹ÊÕÏÂë
		u8 ChargingPileAngle;			//³äµç×®Æ«½Ç
		u8 ChargingCircuitCurrent;//³äµç»ØÂ·µçÁ÷±êÖ¾
		u8 LocationErrorCode;			//¶¨Î»¹ÊÕÏÂë
		u8 UltrasonicDistanceNo1[2];//1ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicDistanceNo2[2];//2ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicDistanceNo3[2];//3ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicDistanceNo4[2];//4ºÅ³¬Éù²¨¾àÀë
	 	u8 UltrasonicDistanceNo5[2];//5ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicDistanceNo6[2];//6ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicDistanceNo7[2];//7ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicDistanceNo8[2];//8ºÅ³¬Éù²¨¾àÀë
		u8 UltrasonicErrorCode;		//³¬Éù²¨¹ÊÕÏÂë
		u8 InfraredRanging[4];			//ºìÍâ²â¾à   1/³µÂÖÐü¿Õ 0/³µÂÖÎ´Ðü¿Õ InfraredRanging[0] ºìÍâ1 InfraredRanging[1] ºìÍâ2 InfraredRanging[2] ºìÍâ3 InfraredRanging[3]ºìÍâ4  ²â¾à·¶Î§0-255mm
		u8 InfraredErrorCode;			//ºìÍâ²â¾à´íÎóÂë
		u8 CrashSensorStatus[2];		//Åö×²´«¸ÐÆ÷×´Ì¬ CrashSensorStatus[0] ´«¸ÐÆ÷1×´Ì¬ CrashSensorStatus[1]´«¸ÐÆ÷2×´Ì¬
		u8 CommunicationErrorCode;//Åö×²´«¸ÐÆ÷´íÎóÂë
		u8 Reserved1;						//±£ÁôÎ»
		u8 Reserved2;						//±£ÁôÎ»
		u8 Reserved3;						//±£ÁôÎ»
		u8 Reserved4;						//±£ÁôÎ»
		u8 Timestamp;							//Ê±¼ä´Á
		u8 CRCCheck[2];							//CRCÐ£Ñé
 }TCP_PACK;
u8 TCP_PACK_BUFSIZE=56;
TCP_PACK Tcp_Server_Recvbuf;
TCP_PACK Tcp_Server_Transbuf;
u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP¿Í»§¶Ë½ÓÊÕÊý¾Ý»º³åÇø
u8 *tcp_server_sendbuf="Explorer STM32F407 NETCONN TCP Server send data\r\n";	
u8 tcp_server_flag;								//TCP·þÎñÆ÷Êý¾Ý·¢ËÍ±êÖ¾Î»

//TCP¿Í»§¶ËÈÎÎñ
#define TCPSERVER_PRIO		6
//ÈÎÎñ¶ÑÕ»´óÐ¡
#define TCPSERVER_STK_SIZE	300
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB TCPSERVERTaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];
  

//tcp·þÎñÆ÷ÈÎÎñ
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

	conn = netconn_new(NETCONN_TCP);  //´´½¨Ò»¸öTCPÁ´½Ó
	netconn_bind(conn,IP_ADDR_ANY,TCP_SERVER_PORT);  //°ó¶¨¶Ë¿Ú 8ºÅ¶Ë¿Ú
	netconn_listen(conn);  		//½øÈë¼àÌýÄ£Ê½
	conn->recv_timeout = 10;  	//½ûÖ¹×èÈûÏß³Ì µÈ´ý10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //½ÓÊÕÁ¬½ÓÇëÇó
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //´¦ÀíÐÂÁ¬½ÓµÄÊý¾Ý
		{ 
			struct netbuf *recvbuf;

			netconn_getaddr(newconn,&ipaddr,&port,0); //»ñÈ¡Ô¶¶ËIPµØÖ·ºÍ¶Ë¿ÚºÅ
			
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
			printf("Ö÷»ú%d.%d.%d.%dÁ¬½ÓÉÏ·þÎñÆ÷,Ö÷»ú¶Ë¿ÚºÅÎª:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
			Tcp_Server_Transbuf.Head[0] =0x66;
			Tcp_Server_Transbuf.Head[1] =0x88;
			Tcp_Server_Transbuf.ChargingPileAngle =0xcf;
			while(1)
			{
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //ÓÐÊý¾ÝÒª·¢ËÍ
				{
//					err = netconn_write(newconn ,tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf),NETCONN_COPY); //·¢ËÍtcp_server_sendbufÖÐµÄÊý¾Ý
					
					err = netconn_write(newconn ,&Tcp_Server_Transbuf,TCP_PACK_BUFSIZE,NETCONN_COPY); //·¢ËÍtcp_server_sendbufÖÐµÄÊý¾Ý
					if(err != ERR_OK)
					{
						printf("·¢ËÍÊ§°Ü\r\n");
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//½ÓÊÕµ½Êý¾Ý
				{	
					OS_CRITICAL_ENTER();	//¹ØÖÐ¶Ï					
					memset(&Tcp_Server_Recvbuf,0,TCP_PACK_BUFSIZE);  //Êý¾Ý½ÓÊÕ»º³åÇøÇåÁã
//					memset(tcp_server_recvbuf,0,TCP_PACK_BUFSIZE);  //Êý¾Ý½ÓÊÕ»º³åÇøÇåÁã
					for(q=recvbuf->p;q!=NULL;q=q->next)  //±éÀúÍêÕû¸öpbufÁ´±í
					{
						//ÅÐ¶ÏÒª¿½±´µ½TCP_SERVER_RX_BUFSIZEÖÐµÄÊý¾ÝÊÇ·ñ´óÓÚTCP_SERVER_RX_BUFSIZEµÄÊ£Óà¿Õ¼ä£¬Èç¹û´óÓÚ
						//µÄ»°¾ÍÖ»¿½±´TCP_SERVER_RX_BUFSIZEÖÐÊ£Óà³¤¶ÈµÄÊý¾Ý£¬·ñÔòµÄ»°¾Í¿½±´ËùÓÐµÄÊý¾Ý
						if(q->len > (TCP_PACK_BUFSIZE-data_len)) memcpy(&Tcp_Server_Recvbuf+data_len,q->payload,(TCP_PACK_BUFSIZE-data_len));//¿½±´Êý¾Ý
						else memcpy(&Tcp_Server_Recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_PACK_BUFSIZE) break; //³¬³öTCP¿Í»§¶Ë½ÓÊÕÊý×é,Ìø³ö	
					}
					OS_CRITICAL_EXIT();	//¿ªÖÐ¶Ï
					data_len=0;  //¸´ÖÆÍê³Éºódata_lenÒªÇåÁã¡
					if((Tcp_Server_Recvbuf.Head[0] == 0x88)&&(Tcp_Server_Recvbuf.Head[1] == 0x66))
					{					
						printf("Tcp_Server_Recvbuf.Head check ok head1==0x88 head2==0x66\r\n");  //Êý¾ÝÓÐÎó
					}
					LCD_ShowString(30,270,210,200,16,(u8*)&Tcp_Server_Recvbuf);
					printf("rece:%s\r\n",(char*)&Tcp_Server_Recvbuf);  //Í¨¹ý´®¿Ú·¢ËÍ½ÓÊÕµ½µÄÊý¾Ý
					memset(&Tcp_Server_Recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //Êý¾Ý½ÓÊÕ»º³åÇøÇåÁã
					netbuf_delete(recvbuf);
				}else if(recv_err == ERR_CLSD)  //¹Ø±ÕÁ¬½Ó
				{
					netconn_close(newconn);
					netconn_delete(newconn);
					printf("Ö÷»ú:%d.%d.%d.%d¶Ï¿ªÓë·þÎñÆ÷µÄÁ¬½Ó\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
					break;
				}
				
			}
		}
	}
}


//´´½¨TCP·þÎñÆ÷Ïß³Ì
//·µ»ØÖµ:0 TCP·þÎñÆ÷´´½¨³É¹¦
//		ÆäËû TCP·þÎñÆ÷´´½¨Ê§°Ü
u8 tcp_server_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();	//½øÈëÁÙ½çÇø
	//´´½¨TCP·þÎñÆ÷Ïß³Ì
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
	OS_CRITICAL_EXIT();		//ÍË³öÁÙ½çÇø
	return 0;
}







