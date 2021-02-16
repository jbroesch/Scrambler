/**********************************************************************
*
* © James D. Broesch, 2011, 2012 ... 2021
* 
* This program demonstrate the use of interrupts to free up 
* processor.
* A 750Hz sinusoidal signal is output.
* The tone stops when switch 1 is pressed.
* A timer interrupt provides background functions.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33FJ256GP506
* Compiler:        MPLAB® C30 v3.00 or higher
*
************************************************************************/
#include <xc.h>

#include <math.h>

#include "WM8510CODEC.h"
#include "timer.h"
#include "dscInit.h"
#include "dma.h"

_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

// Make varibles in other modules visibile in this one.
extern unsigned int timer1Count; // Not used in this module.
// Frame recieved interrupt flag.
extern unsigned int rxFlag; // Not used in this module.
extern unsigned int txFlag; // Not used in this module.

// Transmit and recieve buffers.
extern signed int txSignal[];
extern signed int rxSignal[];

// Working pointers. Use if you like.
extern signed int *pSignalRx; // Not used in this module.
extern signed int *pSignalTx;

int main(void)
{	
	short unsigned int n; // Working index.	

	// Intialize the board and the drivers
	DSCInit();

	// Initalize the CODEC.
	WM8510Init();
	// Start Audio input and output function
	WM8510Start();
		
	// Configure the CODEC for 8K operation	
	WM8510SampleRate8KConfig();

	// Use timer 1 as a real time clock.
	// 1 tick = 15,625 * 256 clock cycles = 100ms.
	timer1Init(15625);

	// Intialize and start the DMA channels.
	// Channel 0 moves data from the transmit buffers to the DCI.
	// Channel 1 moves data from the DCI to the recieve buffers.
	dmaInit();

	// Main processing loop.
	// Quit if switch 1 is pressed.

	while(SWITCH_S1)
		{
		 if (rxFlag)
			{
			if (SWITCH_S2)
				for (n = 0; n < FRAME_SIZE; n++)
					txSignal[n] = rxSignal[n];
			else
			{
			for (n = 0; n < FRAME_SIZE; n++)
				{
				if (n & 0x0001)
					txSignal[n] = rxSignal[n];
				else 
					txSignal[n] = -rxSignal[n] ;
				}	
			} 
			rxFlag = 0;
			}
		} 

	// Disable the DMA Channels.
	DMA0CONbits.CHEN = 0;
	DMA1CONbits.CHEN = 0;

	// Stop the CODEC interface.
	WM8510Stop();

return(0); // Go home.
}
