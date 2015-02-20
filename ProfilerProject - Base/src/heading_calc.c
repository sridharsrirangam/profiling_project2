#include "heading_calc.h"
#include <math.h>
#include "sine_table.h"

float value_scaled;



float Z,Xrc,Yrc,Zrc,Xh,Yh,Zh,azi,Zsqrt;
float s_r,c_r,c_p,s_p,s_r_lt,c_r_lt;
float H_2,X_2,Y_2;

float sine_calc(float value)
{ 
	float conv = 81.487f,b;
	arm_mult_f32(&value,&conv,&value_scaled,1);
	//value_scaled =(float) value*conv;
	if(value_scaled>=0)
 b = SineTable[(int)value_scaled];
	else
	b = -SineTable[(int)(-value_scaled)];	
		
return b;
}

float cos_calc(float value)
{ 
	float conv = 81.487f,b;
	
	value_scaled =(float) value*conv;

 b = CosTable[(int)value_scaled];
	
		
return b;
}
void heading_calc(float roll,float pitch)
{


c_r=cos_calc(roll); //cosine of roll
s_r = sine_calc(roll);//sine of roll
c_p = cos_calc(pitch);//cosine of pitch
s_p = sine_calc(pitch);//sine of pitch


/*
	s_r = sinf(roll);
c_p = cosf(pitch);
s_p = sinf(pitch);
c_r = cosf(roll);
	*/

	//Z calculation
	Z=-42.3f;

	/*

//roll compensation
Xrc = X;
Yrc = (Y*(float)c_r)+(Z*(float)s_r);
Zrc = (float)((-Y)*(float)s_r) + (Z*(float)c_r);

	//pitch compensation
Xh = (float)Xrc*c_p - Zrc*c_p;
Yh= Yrc;
Zh = (float)Xrc*s_p + Zrc*c_p;
*/
//alternate calculation
Xh=X*c_p+Z*s_p;
Yh= (X*s_r*s_p)+(Y*c_r)-(Z*s_r*c_p);
//azimuth calculation

azi = (float)(180+atan2f(-Yh,-Xh)*overPi);
}
