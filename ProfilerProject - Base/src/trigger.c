#include <MKL25Z4.H>
#include "trigger.h"

void init_trigger(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
  PORTB->PCR[trigger1] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[trigger1] |= PORT_PCR_MUX(1)| PORT_PCR_PS_MASK |PORT_PCR_PE_MASK ;     
	PORTB->PCR[trigger2] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[trigger2] |= PORT_PCR_MUX(1)| PORT_PCR_PS_MASK |PORT_PCR_PE_MASK ;     
	
}
