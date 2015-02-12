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
	


	i2c_init();																/* init i2c	*/
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
		;
}

// *******************************ARM University Program Copyright � ARM Ltd 2013*************************************   
