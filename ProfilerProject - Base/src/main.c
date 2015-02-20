/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "math.h"

#include "config.h"
#include "gpio_defs.h"
#include "LCD_4bit.h"
#include "LEDs.h"
#include "timers.h"
#include "i2c.h"
#include "mma8451.h"

#include "delay.h"
#include "profile.h"
#include "trigger.h"
#include "heading_calc.h"
#include "sine_table.h"


uint32_t t,tgr;
uint8_t r;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	uint32_t i;
	uint16_t res=0;
	
	
#ifdef USE_LCD
	char buffer[9];
#endif
	
	Init_RGB_LEDs();

	// Initializing, so turn on yellow
	Control_RGB_LEDs(1, 1, 0);

#ifdef USE_LCD
	Init_LCD();
	Clear_LCD();
	Set_Cursor(0,0);
	Print_LCD(" Hello  ");
	Set_Cursor(0,1);
	Print_LCD(" World! ");
#endif
	

  Init_SineTable();
	i2c_init();		/* init i2c	*/
	init_trigger();
	res = init_mma();													/* init mma peripheral */
	if (res == 0) {
		// Signal error condition
		Control_RGB_LEDs(1, 0, 0);
		while (1)
			;
	}
	
	Delay(1000);

	Init_Profiling();
	__enable_irq();

	// Starting, so turn on magenta
	Control_RGB_LEDs(1,0,1);
	Enable_Profiling();
	
	for (i=0; i<NUM_TESTS; i++) {
		read_full_xyz();
  
		convert_xyz_to_roll_pitch();
		heading_calc(roll_r,pitch_r);

#ifdef USE_LCD
		sprintf(buffer, "R: %5.1f", roll);
		Set_Cursor(0,0);
		Print_LCD(buffer);

		sprintf(buffer, "P: %5.1f", pitch);
		Set_Cursor(0,1);
		Print_LCD(buffer);
#endif
	}
	Disable_Profiling();
	
	// Done, turn on blue LED
	Control_RGB_LEDs(0, 0, 1);
	while (1)
	{
 t = PTB->PDIR;
 tgr = ((t & trigger_mask));	
 if(!(tgr & ptb_pin_9))
 {
 Control_RGB_LEDs(0,1,0);	 
	}
 else if(!(tgr & ptb_pin_8))
 {
  Control_RGB_LEDs(1,0,0); 
}
}
}
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
