#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>

int16_t acc_X=0, acc_Y=0, acc_Z=0;
//float acc_X=0, acc_Y=0, acc_Z=0;
float roll=0.0, pitch=0.0;
float roll_r=0.0,pitch_r=0.0;

//mma data ready
extern uint32_t DATA_READY;



//initializes mma8451 sensor
//i2c has to already be enabled
int init_mma()
{
	  //check for device
		if(i2c_read_byte(MMA_ADDR, REG_WHOAMI) == WHOAMI)	{
			
		  Delay(100);
		  //turn on data ready irq; defaults to int2 (PTA15)
		  i2c_write_byte(MMA_ADDR, REG_CTRL4, 0x01);
		  Delay(100);
		  //set active 14bit mode and 100Hz (0x19)
		  i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x03);
				
		  //enable the irq in the NVIC
		  //NVIC_EnableIRQ(PORTA_IRQn);
		  return 1;
		}
		
		//else error
		return 0;
	
}

void read_full_xyz()
{
	int i;
	uint8_t data[6];
	//int16_t temp[3];
	float temp[3];
	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);
	
	for (i=0;i<3;i++)
	{
		if(i==2)
			data[i] = i2c_repeated_read(1);
		else
			data[i] = i2c_repeated_read(0);
	}
	temp[0] = (data[0]<<8);
	temp[1] = (data[1]<<8);
	temp[2] = (data[2]<<8);
	
	/*
	for( i=0;i<6;i++)	{
		if(i==5)
			data[i] = i2c_repeated_read(1);
		else
			data[i] = i2c_repeated_read(0);
	}
	
	
	temp[0] = (int16_t)((data[0]<<8) | (data[1]));
	temp[1] = (int16_t)((data[2]<<8) | (data[3]));
	temp[2] = (int16_t)((data[4]<<8) | (data[5]));
*/
	
	
	acc_X = temp[0];
	acc_Y = temp[1];
	acc_Z = temp[2];
}


void read_xyz(void)
{
	// sign extend byte to 16 bits - need to cast to signed since function
	// returns uint8_t which is unsigned
	acc_X = (int8_t) i2c_read_byte(MMA_ADDR, REG_XHI);
	Delay(100);
	acc_Y = (int8_t) i2c_read_byte(MMA_ADDR, REG_YHI);
	Delay(100);
	acc_Z = (int8_t) i2c_read_byte(MMA_ADDR, REG_ZHI);

}

/*float sqqrt( float x)
{ 
	float z;
	z= powf(2,(log2f(x)/2));
}*/
#if 0
	float norm_atan2( float y, float x )
		{
		
static const uint32_t sign_mask = 0x80000000;
			static const float b = 0.596227f;
// Extract the sign bits
uint32_t ux_s = sign_mask & (uint32_t)x;
uint32_t uy_s = sign_mask & (uint32_t)y;
// Determine the quadrant offset 
			float q=(float)(( ~ux_s&uy_s)>>29|ux_s>>30);
// Calculate the arctangent in the first quadrant
			float bxy_a = fabs(b*x*y); 
			float num=bxy_a+y*y; 
			float atan_1q=num/(x*x+bxy_a+num);
// Translate it to the proper quadrant
uint32_t uatan_2q = (ux_s ^ uy_s )| (uint32_t)atan_1q;
			return (q + (float)uatan_2q);
}
#endif
void convert_xyz_to_roll_pitch(void) {
	float ax = acc_X*iCOUNTS_PER_G,
				ay = acc_Y*iCOUNTS_PER_G,
				az = acc_Z*iCOUNTS_PER_G;
#if 0	
	float y2 = ay*ay;
	float z2 = az*az;
	float yz = y2+z2;
#endif
 pitch_r= asinf(-ax);
pitch=	(float)pitch_r*overPi;
 //roll = asinf(ay/cosf(pitch));
 roll_r = atan2f(ay,az);
	roll=(float)roll_r*overPi;
	//pitch = norm_atan2(ax, sqrtf(yz))*overPi; //calculate using sin
	
}


//mma data ready irq
// void PORTA_IRQHandler()
// {
// 	NVIC_ClearPendingIRQ(PORTA_IRQn);
// 	DATA_READY = 1;	
// }
