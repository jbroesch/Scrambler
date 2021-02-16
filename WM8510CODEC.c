
#include <xc.h>

#include "WM8510CODEC.h"

// Not used in this code. It is just here in case you need a 
// DCI Interrupt.
void __attribute__((__interrupt__,no_auto_psv)) _DCIInterrupt(void)
{
	_DCIIF = 0;
}

void WM8510Init()
{
	//Disable these modules before configuring them
	DCICON1bits.DCIEN = 0;
	I2C1CONbits.I2CEN = 0;

	// I2C is running during idle.
	I2C1CONbits.I2CSIDL = 0;
	// Inteligent Peripheral Interface Management is disabled						
	I2C1CONbits.IPMIEN = 0;
	//Use a 7 bit slave address.	
	I2C1CONbits.A10M	= 0;
	// No slew control		
	I2C1CONbits.DISSLW	= 1;
 	// No SMBus Enabled		
	I2C1CONbits.SMEN = 0;
	// Set the I2C baud rate to 100KHz.
	I2C1BRG = WM8510_I2CBAUD;
	WM8510_I2C_TRISSCL = 1;
	WM8510_I2C_TRISSDA = 1;
	
	DCICON1 = WM8510_DCICON1VAL;
	DCICON2 = WM8510_DCICON2VAL;
	TSCON = WM8510_TSCONVAL;
	RSCON = WM8510_RSCONVAL;	
}
	
void WM8510Start()
{
	TXBUF0	= 0; // Dummy write to start the DMA.
	I2C1CONbits.I2CEN = 1;	// Enable the I2C module	
	DCICON1bits.DCIEN = 1;	// Enable the DCI module
	_DCIIF = 0; // Clear the interrupt flag.
	// Un-comment if you need the DCI Interrupt.
//	_DCIIE = 1;	// Enable the interrupt.
//	_DCIIP = 7; // Set the DCI interrupt to the highest priority.
}
	
void WM8510Stop()
{
	DCICON1bits.DCIEN = 0;	// Stop the DCI module.
	_DCIIE = 0;				// Stop Interrupts
}

int WM8510IOCtl(int command, void * value)
{
	/* Use the I2C module to send the control data to the codec
	 * Send the device address first, the upper control word next
	 * which consists of register address and bit B8 of control data
	 * and finally send the control data bits B7-B0	*/
	
	CommandValueWord commandValue;
	
	commandValue.wordValue = *((int *)value);
	commandValue.bytes[1] &= 0x1;
	command = command << 1;
	command = command | commandValue.bytes[1];
	
	I2C1CONbits.SEN = 1;			// Send the start condition
	while(I2C1CONbits.SEN == 1);	// Wait for the condition to complete
	
	I2C1TRN = WM8510_I2C_ADDR;	// Address of the WM8510
	while(I2C1STATbits.TRSTAT);		// Wait till this has been tranmitted
	if (I2C1STATbits.ACKSTAT)		// If this value is 1 then a NACK was
	{								// was recieved	
		I2C1CONbits.PEN = 1;
		while(I2C1CONbits.PEN);		// Send the stop condition if a NACK
		return(-1);					// was received	
	}

	I2C1TRN = command;				// The address of the register.
	while(I2C1STATbits.TRSTAT);		// Wait till this has been tranmitted	
	if (I2C1STATbits.ACKSTAT)		// If this value is 1 then a NACK was
	{								// was recieved.
		I2C1CONbits.PEN = 1;
		while(I2C1CONbits.PEN);		// Send the stop condition if a NACK
		return(-1);					// was received				
	}
	
	I2C1TRN = commandValue.bytes[0];	// The value of the register
	while(I2C1STATbits.TRSTAT);			// Wait till this has been tranmitted
	if (I2C1STATbits.ACKSTAT)			// If this value is 1 then a NACK was
	{									// was recieved.
		I2C1CONbits.PEN = 1;
		while(I2C1CONbits.PEN);			// Send the stop condition if a NACK
		return(-1);						// was received.
	}
	
	I2C1CONbits.PEN = 1;
	while(I2C1CONbits.PEN);				// Send the stop condition.
	return(1);
}

void WM8510SampleRate8KConfig(void)
{
	int commandValue,result;
	commandValue = 1;		/* Any value can be written to reset the codec	*/
	result = WM8510IOCtl(WM8510_SOFTWARE_RESET, 	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b001101111;		
	result = WM8510IOCtl(WM8510_POWER_MGMT1, (void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000010101;
	result = WM8510IOCtl(WM8510_POWER_MGMT2,	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b010001001;
	result = WM8510IOCtl(WM8510_POWER_MGMT3, (void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000011000;
	result = WM8510IOCtl(WM8510_AUDIO_INTERFACE,	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b110101001;
	result = WM8510IOCtl(WM8510_CLOCKGEN_CTRL,	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000001010;
	result = WM8510IOCtl(WM8510_ADDITIONAL_CTRL,	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000000100;
	result = WM8510IOCtl(WM8510_GPIO_STUFF, 	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b100001000;
	result = WM8510IOCtl(WM8510_ADC_CONTROL, (void *) &commandValue);
	if (result == -1) while(1);	
	commandValue = 0b001000000;
	result = WM8510IOCtl(WM8510_NOTCH_FILTER1, (void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000000101;
	result = WM8510IOCtl(WM8510_NOTCH_FILTER2, 		(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000111111;
	result = WM8510IOCtl(WM8510_NOTCH_FILTER3, 		(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b001111101;
	result = WM8510IOCtl(WM8510_NOTCH_FILTER4, (void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b100000000;
	result = WM8510IOCtl(WM8510_NOTCH_FILTER4, (void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000011000;
	result = WM8510IOCtl(WM8510_PLL_N, (void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000001100;
	result = WM8510IOCtl(WM8510_PLL_K1, 	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b010010011;
	result = WM8510IOCtl(WM8510_PLL_K2, 	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b011101001;
	result = WM8510IOCtl(WM8510_PLL_K3, 	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000000100;
	result = WM8510IOCtl(WM8510_INPUT_CTRL, 	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000000000;
	result = WM8510IOCtl(WM8510_ADC_BOOST_CTRL,	(void *) &commandValue);
	if (result == -1) while(1);
	commandValue = 0b000000001;
	result = WM8510IOCtl(WM8510_MONO_MIXER_CTRL, (void *) &commandValue);
	if (result == -1) while(1);

// Increased input gain. JDB.
	commandValue = 0b000000000;
	result = WM8510IOCtl(WM8510_ALC_CONTROL1,(void *) &commandValue);
	if (result == -1) while(1);

//	Un-comment the following three lines for microphone operation. 
//	commandValue = 0b000111111;
//	result = WM8510IOCtl(codecHandle,WM8510_INP_PGA_CTRL,(void *) &commandValue);
//	if (result == -1) while(1);

	commandValue = 0b000000000;
	result = WM8510IOCtl(WM8510_ADC_BOOST_CTRL,(void *) &commandValue);
	if (result == -1) while(1);

	commandValue = 0b000000001;
	result = WM8510IOCtl(WM8510_MONO_MIXER_CTRL,(void *) &commandValue);
	if (result == -1) while(1);

}


