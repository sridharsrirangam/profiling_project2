#include <MKL25Z4.H>
#include <math.h>
#include "sine_table.h"


//#define MAX_DAC_CODE (4095)
float SineTable[NUM_STEPS];
float CosTable[NUM_STEPS];

void Init_SineTable(void) {
	unsigned n;
	float i;
	
	for (n=0; n<NUM_STEPS; n++)
	{ 
		i= (float)(n*(3.142f/NUM_STEPS));
		SineTable[n] =  (sinf(i));
	}
for (n=0; n<NUM_STEPS; n++)
	{ 
		i= (float)(n*(float)(3.142f/NUM_STEPS));
		CosTable[n] = (float) (cosf(i));
	}
}
