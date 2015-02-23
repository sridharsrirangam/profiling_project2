#include <MKL25Z4.H>
#include "i2c.h"

//init i2c0
int cnt;
int i2c_check;
void i2c_init(void)
{
	//clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);
	
	//set pins to I2C function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);
		
	//set to 100k baud
	//baud = bus freq/(scl_div+mul)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64
 	I2C0->F = (I2C_F_ICR(0x00) | I2C_F_MULT(0));
	
	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK );
}


//send start sequence
void i2c_start()
{
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
}

//send device and register addresses
void i2c_read_setup(uint8_t dev, uint8_t address)
	{ 
	start:
	I2C0->D = dev;			  /*send dev address	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;
	I2C0->D =  address;		/*send read address	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;	
	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;							 
	
	I2C_REC;						   /*set to recieve mode */

}

//read a byte and ack/nack as appropriate
uint16_t i2c_repeated_read(uint8_t isLastRead)
{
	uint16_t data;
	
	if(isLastRead)	{
		NACK;								/*set NACK after read	*/
	} else	{
		ACK;								/*ACK after read	*/
	}
	
 	data = I2C0->D;				/*dummy read	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) return 0xFFFF;						/*wait for completion */

	if(isLastRead)	{
		I2C_M_STOP;					/*send stop	*/
	}
	data = I2C0->D;				/*read data	*/

	return  data;					
}



//////////funcs for reading and writing a single byte
//using 7bit addressing reads a byte from dev:address
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;
	start:
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;							/*wait for completion */
	
	I2C0->D =  address;		/*send read address	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;							/*wait for completion */
		
	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;							 /*wait for completion */
	
	I2C_REC;						   /*set to recieve mode */
	NACK;									 /*set NACK after read	*/
	
	data = I2C0->D;					/*dummy read	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;								/*wait for completion */
	
	I2C_M_STOP;							/*send stop	*/
	data = I2C0->D;					/*read data	*/

	return data;
}



//using 7bit addressing writes a byte data to dev:address
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{
	start:
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;						  /*wait for ack */
	
	I2C0->D =  address;		/*send write address	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;
		
	I2C0->D = data;				/*send data	*/
	i2c_check=i2c_wait();		/*wait for completion */
	if(i2c_check) goto start;
	I2C_M_STOP;
	
}
void i2c_RESET(void)
{
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;  
            I2C0->C1 |= I2C_C1_TX_MASK;    /* Set transmit (TX) mode */  
            I2C0->C1 |= I2C_C1_MST_MASK;    /* START signal generated */  
            I2C0->C1 |= I2C_C1_IICEN_MASK;  
  
  
            /* send write command to release SDA line*/  
            I2C0->C1 |= I2C_C1_MST_MASK;    /* set MASTER mode */  
            I2C0->C1 |= I2C_C1_TX_MASK;    /* Set transmit (TX) mode */  
            I2C0->D = 0xFF;  
            while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {}  /* wait interrupt      */  
            I2C0->S |= I2C_S_IICIF_MASK;                    /* clear interrupt bit  */  
  
  
            /* Clear arbitration error flag*/  
            I2C0->S |= I2C_S_ARBL_MASK;  
  
  
            /* Send start */  
            I2C0->C1 &= ~I2C_C1_IICEN_MASK;  
            I2C0->C1 |= I2C_C1_TX_MASK;    /* Set transmit (TX) mode */  
            I2C0->C1 |= I2C_C1_MST_MASK;    /* START signal generated */  
  
            I2C0->C1 |= I2C_C1_IICEN_MASK;  
}
int i2c_wait(void)
{cnt=0;
while((I2C0->S & I2C_S_IICIF_MASK)==0) 
	{ cnt++;
		if(cnt>500)
		{ 
			i2c_RESET();
			return 1;
		}
		
		
} 
	I2C0->S |= I2C_S_IICIF_MASK;
 return 0;
}
