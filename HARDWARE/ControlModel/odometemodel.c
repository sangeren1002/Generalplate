#include "odometemodel.h"

int32_t LeftWheelIncrement(int32_t New_Position, int32_t Old_Position, float Angle);
int32_t RightWheeIncrement(int32_t New_Position, int32_t Old_Position, float Angle);

int32_t LeftWheelIncrement(int32_t New_Position, int32_t Old_Position, float Angle)
{
	int32_t leftwheel_increment = 0;
	
	Angle = Angle*PI/180.0f; 
	
	leftwheel_increment = (int32_t)(((float)(New_Position - Old_Position))/(LINENUM * FMC * REDUCTION_RATIO)*2.0f*PI*RADIUS*1000.0f*cos(Angle));
	
	return leftwheel_increment;
}

int32_t RightWheelIncrement(int32_t New_Position, int32_t Old_Position, float Angle)
{
	int32_t rightwheel_increment = 0;
	
	Angle = Angle*PI/180.0f; 
	
	rightwheel_increment = (int32_t)(((float)(New_Position - Old_Position))/(LINENUM * FMC * REDUCTION_RATIO)*2.0f*PI*RADIUS*1000.0f*cos(Angle));
	
	return rightwheel_increment;
}


