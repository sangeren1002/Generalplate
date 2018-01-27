/**********************************************************************
File name:   encoderinquire.c
Author:      WXK
Data:        2017.10.20
Description: ?????????????,???????????????
History:
***********************************************************************/
#include "encoder.h"
#include "usart3.h"
#include "delay.h"
#include "rs485.h"

uint8_t askangle[5] = {0x55, 0xFF, 0x01, 0x00, 0x5A};   /* ????????????????????? */
uint8_t answerangle[6] = {0};    /* ?????????????? */

float Swerve_Node1_Angle = 0;
float Swerve_Node2_Angle = 0;
float Swerve_Node3_Angle = 0;
float Swerve_Node4_Angle = 0;
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];

/****************************************
Function:    AskAngle
Description: ?????????????
Input:       addr-??????????????
Return:      ????????(°)
History:
****************************************/
float AskAngle(uint8_t addr)
{
	uint16_t tmp = 0;
	u8 j;
	
	askangle[1] = addr;
	
//	HAL_UART_Receive_IT(&huart4, answerangle, 6);
//	HAL_UART_Transmit(&huart4, askangle, 5, 0xffff);

//	HAL_Delay(10);
//	u4_printf((char*)askangle);
	USART3_RX_STA=0;
	for(j=0;j<5;j++)//Ñ­»··¢ËÍÊý¾Ý
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);  //µÈ´ýÉÏ´Î´«ÊäÍê³É 
		USART_SendData(USART3,(uint8_t)askangle[j]); 	 //·¢ËÍÊý¾Ýµ½´®¿Ú3 
	}
	delay_ms(10);
	strncpy((char*)answerangle,(char *)USART3_RX_BUF,6);

//	memset((void*)USART3_RX_BUF,0,6);
	tmp = answerangle[3];
	tmp = tmp<<8;
	tmp = tmp|answerangle[2];
	
	tmp = tmp/65536.0f*360.0f;
	delay_ms(10);
	return tmp;
}

void TurnMotorToZero(void)
{
	unsigned int num;
	char angle_pul[12]={0};
	float tmp = 0;
	if(Swerve_Node1_Angle >= 180)
	{
		tmp = 360.0f - Swerve_Node1_Angle;
		tmp=tmp*10000/18;

		sprintf(angle_pul,"1FL%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
	do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
//		Motor_PulsePositionSet(&huart2, ENCODERNODE1, tmp);
	}
	else if(Swerve_Node1_Angle < 180)
	{
		tmp=Swerve_Node1_Angle*10000/18;
		sprintf(angle_pul,"1FL-%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}
	
	if(Swerve_Node2_Angle >= 180)
	{

		tmp = 360.0f - Swerve_Node2_Angle;
		tmp=tmp*10000/18;
		sprintf(angle_pul,"2FL%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}
	else if(Swerve_Node2_Angle < 180)
	{

		tmp=Swerve_Node2_Angle*10000/18;
		sprintf(angle_pul,"2FL-%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}

	if(Swerve_Node3_Angle >= 180)
	{
		tmp = 360.0f - Swerve_Node3_Angle;
		tmp=tmp*10000/18;
		sprintf(angle_pul,"3FL%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}
	else if(Swerve_Node3_Angle < 180)
	{
		tmp=Swerve_Node3_Angle*10000/18;
		sprintf(angle_pul,"3FL-%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}	
	
	if(Swerve_Node4_Angle >= 180)
	{
		tmp = 360.0f - Swerve_Node4_Angle;
		tmp=tmp*10000/18;
		sprintf(angle_pul,"4FL%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}
	else if(Swerve_Node4_Angle < 180)
	{
		tmp=Swerve_Node4_Angle*10000/18;
		sprintf(angle_pul,"4FL-%d\r\n",(int)tmp);
		num = strlen((const char*)angle_pul);
		do{
		RS485_Send_Data((u8*)angle_pul,num);
		delay_ms(20);
		}while(strncmp(&RS485_RX_BUF[1],"%",1)!=0);
	}	
}
void InquireSwerveMotorAngle(void)
{
	Swerve_Node1_Angle = AskAngle(ENCODERNODE1);
	Swerve_Node2_Angle = AskAngle(ENCODERNODE2);
	Swerve_Node3_Angle = AskAngle(ENCODERNODE3);
	Swerve_Node4_Angle = AskAngle(ENCODERNODE4);
	
	TurnMotorToZero();
}


