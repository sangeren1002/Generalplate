/**********************************************************************
File name:   controlmodel.c
Author:      WXK
Data:        2017.10.26
Description: 运动控制模型，只与数学计算有关，作为底层算法支持
History:
***********************************************************************/
#include "controlmodel.h"

float gSpeed_Advance = 0.0;    /* 整体线速度V 单位cm/s */
float gSpeed_Angular = 0.0;    /* 角速度W     单位°/s  */


float i = 0;
float j = 0;

void UpdataSteeringAngle(SwerveMotor *motor, float Speed_Advance, float Speed_Angular)
{
	Speed_Advance = Speed_Advance/100.0f;
	Speed_Angular = Speed_Angular*PI/180.0f;
	
	if(Speed_Angular == 0)
	{
		motor->motor_node1_angle = 0;
		motor->motor_node4_angle = 0;
		motor->motor_node2_angle = 0;
		motor->motor_node3_angle = 0;
	}
	else if(Speed_Advance == 0 && Speed_Angular > 0)
	{
		motor->motor_node1_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)-(WIDTH/2))) * 180.0f / PI;
		motor->motor_node4_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)+(WIDTH/2))) * 180.0f / PI;
		motor->motor_node2_angle = -(motor->motor_node1_angle);
		motor->motor_node3_angle = -(motor->motor_node4_angle);
	}
	else if(Speed_Advance == 0 && Speed_Angular < 0)
	{
		Speed_Angular = -(Speed_Angular);
		
		motor->motor_node1_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)-(WIDTH/2))) * 180.0f / PI;
		motor->motor_node4_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)+(WIDTH/2))) * 180.0f / PI;
		motor->motor_node2_angle = -(motor->motor_node1_angle);
		motor->motor_node3_angle = -(motor->motor_node4_angle);
	}
	
	//Speed_Advance = Speed_Advance/100.0f;
	//Speed_Angular = Speed_Angular*PI/180.0f;
	
	//if((Speed_Advance > 0 && Speed_Angular > 0) || (Speed_Advance < 0 && Speed_Angular < 0))
	//{
	else if(Speed_Advance > 0 && Speed_Angular > 0)
  {	 
		motor->motor_node1_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)-(WIDTH/2))) * 180.0f / PI;
		motor->motor_node4_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)+(WIDTH/2))) * 180.0f / PI;
		motor->motor_node2_angle = -(motor->motor_node1_angle);
		motor->motor_node3_angle = -(motor->motor_node4_angle);
  }
	else if(Speed_Advance < 0 && Speed_Angular > 0)
	{
		Speed_Advance = -(Speed_Advance);
		
		motor->motor_node1_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)+(WIDTH/2))) * 180.0f / PI;
		motor->motor_node1_angle = -(motor->motor_node1_angle);
		motor->motor_node4_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)-(WIDTH/2))) * 180.0f / PI;
		motor->motor_node4_angle = -(motor->motor_node4_angle);
		motor->motor_node2_angle = -(motor->motor_node1_angle);
		motor->motor_node3_angle = -(motor->motor_node4_angle);
		
	}
	else if(Speed_Advance > 0 && Speed_Angular < 0)
	{
		Speed_Angular = -(Speed_Angular);
		
		motor->motor_node1_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)+(WIDTH/2))) * 180.0f / PI;
		motor->motor_node1_angle = -(motor->motor_node1_angle);
		motor->motor_node4_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)-(WIDTH/2))) * 180.0f / PI;
		motor->motor_node4_angle = -(motor->motor_node4_angle);
		motor->motor_node2_angle = -(motor->motor_node1_angle);
		motor->motor_node3_angle = -(motor->motor_node4_angle);
	}
	else
	{
		Speed_Advance = -(Speed_Advance);
		Speed_Angular = -(Speed_Angular);
		
		motor->motor_node1_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)-(WIDTH/2))) * 180.0f / PI;
		motor->motor_node4_angle = atan((LENTH/2.0f)/((Speed_Advance/Speed_Angular)+(WIDTH/2))) * 180.0f / PI;
		motor->motor_node2_angle = -(motor->motor_node1_angle);
		motor->motor_node3_angle = -(motor->motor_node4_angle);
	}
	//}

}

void UpdataMotorRevSpeed(StraightMotor *motor, float Speed_Advance, float Speed_Angular)
{
	Speed_Advance = (Speed_Advance/100.0f);
	Speed_Angular = (Speed_Angular*PI/180.0f);
	
	if(Speed_Angular == 0 && Speed_Advance > 0)
	{
		motor->motor_node1_rev = Speed_Advance / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f;
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = -motor->motor_node1_rev;
		motor->motor_node4_rev = motor->motor_node3_rev;
	}
	else if(Speed_Angular == 0 && Speed_Advance < 0)
	{
		Speed_Advance = -(Speed_Advance);
		
		motor->motor_node1_rev = Speed_Advance / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f;
		motor->motor_node1_rev = -(motor->motor_node1_rev);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = -motor->motor_node1_rev;
		motor->motor_node4_rev = motor->motor_node3_rev;
	}
	else if(Speed_Advance == 0 && Speed_Angular > 0)
	{
		motor->motor_node1_rev = (sqrt(pow((Speed_Advance/Speed_Angular - WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);  /* 转数/min */
		motor->motor_node4_rev = (sqrt(pow((Speed_Advance/Speed_Angular + WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);
		motor->motor_node1_rev = -(motor->motor_node1_rev);
		motor->motor_node4_rev = -(motor->motor_node4_rev);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = motor->motor_node4_rev;
	}
	else if(Speed_Advance == 0 && Speed_Angular < 0)
	{
		Speed_Angular = -(Speed_Angular);
		
		motor->motor_node1_rev = (sqrt(pow((Speed_Advance/Speed_Angular + WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);  /* 转数/min */
		motor->motor_node4_rev = (sqrt(pow((Speed_Advance/Speed_Angular - WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = motor->motor_node4_rev;
	}
	//Speed_Advance = (Speed_Advance/100.0f);
	//Speed_Angular = (Speed_Angular*PI/180.0f);
	
	//if(Speed_Angular != 0)
	//{
	else if(Speed_Advance > 0 && Speed_Angular > 0)
	{
		motor->motor_node1_rev = (sqrt(pow((Speed_Advance/Speed_Angular - WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);  /* 转数/min */
		motor->motor_node4_rev = (sqrt(pow((Speed_Advance/Speed_Angular + WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);
		motor->motor_node4_rev = -(motor->motor_node4_rev);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = motor->motor_node4_rev;
	}
	else if(Speed_Advance < 0 && Speed_Angular > 0)
	{
		Speed_Advance = -(Speed_Advance);
		
		motor->motor_node1_rev = (sqrt(pow((Speed_Advance/Speed_Angular + WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);  /* 转数/min */
		motor->motor_node4_rev = (sqrt(pow((Speed_Advance/Speed_Angular - WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);
		motor->motor_node1_rev = -(motor->motor_node1_rev);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = motor->motor_node4_rev;
	}
	else if(Speed_Advance > 0 && Speed_Angular < 0)
	{
		Speed_Angular = -(Speed_Angular);
		
		motor->motor_node1_rev = (sqrt(pow((Speed_Advance/Speed_Angular + WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);  /* 转数/min */
		motor->motor_node4_rev = (sqrt(pow((Speed_Advance/Speed_Angular - WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);
		motor->motor_node4_rev = -(motor->motor_node4_rev);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = motor->motor_node4_rev;
	}
  else
	{
		Speed_Advance = -(Speed_Advance);
		Speed_Angular = -(Speed_Angular);
		
		motor->motor_node1_rev = (sqrt(pow((Speed_Advance/Speed_Angular - WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);  /* 转数/min */
		motor->motor_node4_rev = (sqrt(pow((Speed_Advance/Speed_Angular + WIDTH/2),2) + pow((LENTH/2),2)) * Speed_Angular / RADIUS / (2*PI) * REDUCTION_RATIO * 60.0f);
		motor->motor_node1_rev = -(motor->motor_node1_rev);
		motor->motor_node2_rev = motor->motor_node1_rev;
		motor->motor_node3_rev = motor->motor_node4_rev;
		
	}
		
		//}
}

