#include "tcp_server_demo.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
#include "led.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h"
#include "lcd.h"

 


u8 TCP_SEND_PACK_BUFSIZE=60;
static TCP_SEND_PACK Tcp_Server_Recvbuf;
static TCP_SEND_PACK Tcp_Server_Transbuf;
TCP_SEND_PACK Tcp_Server_Recvconbuf;
TCP_SEND_PACK Tcp_Server_Transconbuf;
//u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP服务器数据接收缓存区
char *tcp_server_sendbuffer="Explorer STM32F407 NETCONN TCP Server received data\r\n";	
u8 tcp_server_flag;								//TCP服务器发送数据标志位

//TCP服务端任务
#define TCPSERVER_PRIO		6
//任务堆栈大小
#define TCPSERVER_STK_SIZE	300
//任务控制块
OS_TCB TCPSERVERTaskTCB;
//任务堆栈
CPU_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];
  

//tcp服务器任务
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
//	static u8 TCP_PACK_BUFSIZE = sizeof(Tcp_Server_Transbuf);
	LWIP_UNUSED_ARG(arg);

	conn = netconn_new(NETCONN_TCP);  //创建一个TCP连接
	netconn_bind(conn,IP_ADDR_ANY,TCP_SERVER_PORT);  //绑定端口 8080端口 
	netconn_listen(conn);  		//	进入监听模式
	conn->recv_timeout = 10;  	//禁止阻塞线程  等待10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //接收连接请求
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //处理新连接的数据
		{ 
			struct netbuf *recvbuf;

			netconn_getaddr(newconn,&ipaddr,&port,0); //获取远端IP地址和端口号
			
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
			printf("主机%d.%d.%d.%d连接上服务器,主机端口号为:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);

			while(1)
			{
			Tcp_Server_Transbuf.Head[0] =0x66;
			Tcp_Server_Transbuf.Head[1] =0x88;
			Tcp_Server_Transbuf.ChargingPileAngle =0xcf;
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
				{
//					err = netconn_write(newconn ,tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf),NETCONN_COPY); //发送tcp_server_sendbuf中的数据
					
					err = netconn_write(newconn ,&Tcp_Server_Transbuf,TCP_SEND_PACK_BUFSIZE,NETCONN_COPY); //发送Tcp_Server_Transbuf指向的数据
					if(err != ERR_OK)
					{
						printf("发送失败\r\n");
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//接收到数据
				{	
					OS_CRITICAL_ENTER();	//关中断				
					memset(&Tcp_Server_Recvbuf,0,TCP_SEND_PACK_BUFSIZE);  //数据接收缓存区清零
					for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
					{
						//判断要拷贝到Tcp_Server_Recvbuf的数据是否大于Tcp_Server_Recvbuf的剩余空间大小，如果大于
						//就只拷贝Tcp_Server_Recvbuf中剩余长度的数据，否则的话就拷贝所有的数据
						if(q->len > (TCP_SEND_PACK_BUFSIZE-data_len)) memcpy(&Tcp_Server_Recvbuf+data_len,q->payload,(TCP_SEND_PACK_BUFSIZE-data_len));//拷贝数据
						else memcpy(&Tcp_Server_Recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SEND_PACK_BUFSIZE) break; //超出TCP服务器接收数组大小，跳出
					}
					OS_CRITICAL_EXIT();	//开中断
					data_len=0;  //复制完后data_len要清零
					if((Tcp_Server_Recvbuf.Head[0] == 0x88)&&(Tcp_Server_Recvbuf.Head[1] == 0x66))
					{	
						Tcp_Server_Transbuf.Head[0] =0x66;
			Tcp_Server_Transbuf.Head[1] =0x88;
			Tcp_Server_Transbuf.ChargingPileAngle =0xcf;
//						memcpy(&Tcp_Server_Recvconbuf,Tcp_Server_Recvbuf,TCP_PACK_BUFSIZE);
						err = netconn_write(newconn ,&Tcp_Server_Transbuf,TCP_SEND_PACK_BUFSIZE,NETCONN_COPY); //回复Tcp_Server_Transbuf数据
						if(err != ERR_OK)
						{
							printf("发送失败\r\n");
						}
						printf("Tcp_Server_Recvbuf.Head check ok head1==0x88 head2==0x66\r\n");  //串口发送数据
					}
					err = netconn_write(newconn ,tcp_server_sendbuffer,strlen(tcp_server_sendbuffer),NETCONN_COPY);
					LCD_ShowString(30,270,210,200,16,(u8*)&Tcp_Server_Recvbuf);
					printf("rece:%s\r\n",(char*)&Tcp_Server_Recvbuf);  //通过串口发送TCP_server接收到的数据
					memset(&Tcp_Server_Recvbuf,0,TCP_SEND_PACK_BUFSIZE);  //数据接收缓存区清零
					netbuf_delete(recvbuf);
				}else if(recv_err == ERR_CLSD)  //关闭连接
				{
					netconn_close(newconn);
					netconn_delete(newconn);
					printf("主机:%d.%d.%d.%d断开与服务器的连接\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
					break;
				}
				
			}
		}
	}
}


//创建TCP服务器任务
//返回值：0 TCP服务器任务创建成功
//		其他 TCP服务器任务创建失败
u8 tcp_server_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建TCP服务器线程
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
	OS_CRITICAL_EXIT();		//退出临界区
	return 0;
}







