#include <xc.h>
#include "timer.h"
#include "DSCInit.h"

volatile unsigned int timer1Count = 0;

// Interrupt handler for timer 0.
void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
{

	// LED State.
	volatile static int flagLED = 0;
	// Change the LED status each second.
	if (timer1Count++ % 10 == 0)
	{
		if (flagLED == 1)
		{
			RED_LED = SASK_LED_OFF; // Turn off the LED.
			flagLED = 0;
		}
	
		else // Then the flag must be 0.
		{
			RED_LED = SASK_LED_ON; // Turn on the LED.	
			flagLED = 1;
		}

	}

	IFS0bits.T1IF = 0; // Clear the interrupt flag.
}

// Configure T1 as a periodic timer and load the duration period.
void timer1Init(unsigned int count)
{
	// Priority level is 1.
	IPC0bits.T1IP = 1;
	// Timer1 interrupt enabled.
	IEC0bits.T1IE = 1;
	// Interrupt period.
	PR1   = count;
	// Select the internal clock source.
	T1CONbits.TCS = 0;
	// Pre-scale by 256.
	T1CONbits.TCKPS = 3;
	// Start the timer.
 	T1CONbits.TON = 1;
}
