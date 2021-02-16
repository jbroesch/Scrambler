#ifndef _WM8510CODECD_H_
#define  _WM8510CODEC_H_

#include <xc.h>

#define WM8510_DCI_MASTER 				1
#define WM8510_CODEC_FRAME 				128
#define WM8510_DRV_BUFFER_SIZE			(WM8510_CODEC_FRAME * 4)
#define WM8510_DCI_INTERRUPT_PRIORITY	4
#define WM8510_I2CBAUD					272
#define WM8510_I2C_ADDR					0b00110100
#define WM8510_DCICON1VAL				 0x0700
#define WM8510_DCICON2VAL				0x000F
#define WM8510_TSCONVAL					0x1
#define WM8510_RSCONVAL					0x1

/* These value applicable only if DCI is master	*/
#define WM8510_SAMPLE_RATE				8000 	
#define WM8510_CODEC_DRV_FCY			40000000
#define WM8510_DCI_BRG_VALUE 			8

#define	WM8510_I2C_TRISSCL _TRISG2
#define WM8510_I2C_TRISSDA _TRISG3

typedef union uCommandValueWord
{
	int wordValue;
	unsigned char bytes[2];
}CommandValueWord;	

/* WM8510 Codec Register Defines	*/
#define WM8510_SOFTWARE_RESET	0
#define WM8510_POWER_MGMT1	1
#define WM8510_POWER_MGMT2	2
#define WM8510_POWER_MGMT3	3
#define WM8510_AUDIO_INTERFACE	4
#define WM8510_COMPANDING_CTRL	5
#define WM8510_CLOCKGEN_CTRL	6
#define WM8510_ADDITIONAL_CTRL	7
#define WM8510_GPIO_STUFF		8
#define WM8510_DAC_CTRL			10
#define WM8510_DAC_VOLUME		11
#define WM8510_ADC_CONTROL		14
#define WM8510_ADC_DIGITAL_VOL	15
#define WM8510_DAC_LIMITER1		24
#define WM8510_DAC_LIMITER2		25
#define WM8510_NOTCH_FILTER1	27
#define WM8510_NOTCH_FILTER2	28
#define WM8510_NOTCH_FILTER3	29
#define WM8510_NOTCH_FILTER4	30
#define WM8510_ALC_CONTROL1	32
#define WM8510_ALC_CONTROL2	33
#define WM8510_ALC_CONTROL3	34
#define WM8510_NOISE_GATE		35
#define WM8510_PLL_N				36
#define WM8510_PLL_K1			37
#define WM8510_PLL_K2			38
#define WM8510_PLL_K3			39
#define WM8510_ATTEN_CTRL		40
#define WM8510_INPUT_CTRL		44
#define WM8510_INP_PGA_CTRL		45
#define WM8510_ADC_BOOST_CTRL	47
#define WM8510_OUTPUT_CTRL		49
#define WM8510_SPK_MIXER_CTRL	50
#define WM8510_SPK_VOLUME_CTRL	54
#define WM8510_MONO_MIXER_CTRL	56

void 	WM8510Init		(void);
void 	WM8510Start		(void);
void 	WM8510Stop		(void);

int 	WM8510IOCtl		(int command, void *value);

/* These are helper functions. Use if you need to	*/

void WM8510SampleRate8KConfig();
void WM8510SampleRate16KConfig();

#endif
