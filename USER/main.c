#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "usmart.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"
#include "rtc.h"
#include "temperature.h"
#include "beep.h"
#include "sram.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "includes.h"
#include "lwipopts.h"
#include "httpd.h"
#include "tcp_server_demo.h"
#include "rs485.h"	


//UCOSIII中以下优先级用户程序不能使用
//将这些优先级分配给UCOSIII的5个系统内部任务
//优先级0：中断服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2： 定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()


  
//KEY任务
//任务优先级
#define KEY_TASK_PRIO		8
//任务堆栈大小
#define KEY_STK_SIZE		128
//任务控制块
OS_TCB KeyTaskTCB;
//任务堆栈
CPU_STK	KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata); 
//LED任务
//任务优先级
#define LED_TASK_PRIO		9
//任务堆栈大小
#define LED_STK_SIZE		128
//任务控制块
OS_TCB LedTaskTCB;
//任务控堆栈
CPU_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);  

//在LCD上显示地址信息任务
//任务优先级
#define DISPLAY_TASK_PRIO	10
//任务堆栈大小
#define DISPLAY_STK_SIZE	128
//任务控制块
OS_TCB DisplayTaskTCB;
//任务控堆栈
CPU_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);

//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务控制块
OS_TCB StartTaskTCB;
//任务控堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 

extern void Adc_Temperate_Init(void); //声明Adc_Temperate_Init函数

//在LCD上显示地址信息
//mode:1 		显示DHCP获取到的地址
//	  其他 	显示静态地址
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,170,210,16,16,buf); 
		LCD_ShowString(30,190,210,16,16,"Port:8088!");																																					//打印端口号
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印动态IP地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);		//打印子网掩码地址
		LCD_ShowString(30,170,210,16,16,buf); 
		LCD_ShowString(30,190,210,16,16,"Port:8088!");  																																					//打印端口号 
	}	
}

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组配置
	uart_init(115200);    	//串口1初始化
	usmart_dev.init(84); 	//初始化USMART
	LED_Init();  			//LED初始化
	KEY_Init();  			//按键初始化
	LCD_Init();  			//LCD初始化
    BEEP_Init(); 			//蜂鸣器初始化
    Adc_Init();  			//ADC1_CH5初始化
	RS485_Init(115200);		//RS485初始化 115200波特率传输距离为500m
	Adc_Temperate_Init(); //初始化内部温度传感器
	FSMC_SRAM_Init();		//SRAM初始化
	
	mymem_init(SRAMIN);  	//初始化内部内存池
	mymem_init(SRAMEX);  	//初始化外部内存池
	mymem_init(SRAMCCM); 	//初始化内部内存池
	
	POINT_COLOR = RED; 		//红色字体
	LCD_ShowString(30,30,200,20,16,"Explorer STM32F4");
	LCD_ShowString(30,50,200,20,16,"WebServer Test");
	LCD_ShowString(30,70,200,20,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,20,16,"2015/8/24");
	POINT_COLOR = BLUE; 	//蓝色字体
	
	OSInit(&err); 					//UCOSIII初始化
	while(lwip_comm_init()) 		//lwip初始化
	{
		LCD_ShowString(30,110,200,20,16,"Lwip Init failed!"); 	//lwip初始化失败
		delay_ms(500);
		LCD_Fill(30,110,230,150,WHITE);
		delay_ms(500); 
	}
	LCD_ShowString(30,110,200,20,16,"Lwip Init Success!"); 		//lwip初始化成功
//	httpd_init();  												//http初始化
	while(tcp_server_init()) 									//tcp_server初始化(创建tcp_server线程)
	{
		LCD_ShowString(30,150,200,20,16,"TCP Server failed!!"); //tcp服务器创建失败
		delay_ms(500);
		LCD_Fill(30,150,230,170,WHITE);
		delay_ms(500);
	}
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
	(CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接受的最大消息数目，为0时禁止接受消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区
	OSStart(&err); 		//开启UCOS
}

//start任务
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务             
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断官博时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当时间片轮转的时候
	//使能时间片轮转调度功能，时间片长度为1个系统时钟节拍，即1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //创建DHCP任务
#endif

	OS_CRITICAL_ENTER();	//进入临界区
	//创建LED任务
	OSTaskCreate((OS_TCB 	* )&LedTaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK   * )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);								 
	//创建显示任务
	OSTaskCreate((OS_TCB 	* )&DisplayTaskTCB,		
				 (CPU_CHAR	* )"display task", 		
                 (OS_TASK_PTR )display_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )DISPLAY_TASK_PRIO,     	
                 (CPU_STK   * )&DISPLAY_TASK_STK[0],	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE/10,	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				
//创建KEY任务
	OSTaskCreate((OS_TCB 	* )&KeyTaskTCB,		
				 (CPU_CHAR	* )"key task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEY_TASK_PRIO,     	
                 (CPU_STK   * )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);											 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务
	OS_CRITICAL_EXIT();	//退出临界区
}

//显示地址任务
void display_task(void *pdata)
{
	OS_ERR err;
	while(1)
	{ 
#if LWIP_DHCP									//当开启DHCP的时候
		if(lwipdev.dhcpstatus != 0) 			//开启DHCP
		{
			show_address(lwipdev.dhcpstatus );	//显示动态地址信息
			OS_TaskSuspend((OS_TCB*)&DisplayTaskTCB,&err);		//挂起显示地址任务
		}
#else
		show_address(0); 						//显示静态地址信息
		OSTaskSuspend(&DisplayTaskTCB,&err); 			//挂起显示地址任务
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

//led任务
void led_task(void *pdata)
{
	OS_ERR err;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
 	}
}
//key任务
void key_task(void *pdata)
{
	u8 key; 
	u8 i;
	OS_ERR err;
	u16 c[7]={0x1103,0x6060,0x0001,0x0300,0x0000,0xc5f4,0x4f4f};
	while(1)
	{
		key = KEY_Scan(0);
		if(key==KEY0_PRES) //发送数据
		{		
			tcp_server_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
			for(i=0;i<7;i++)
				printf("%x",c[i]);
//			printf("%d",b);
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}










