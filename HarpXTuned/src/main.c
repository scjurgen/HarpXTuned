/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

#define MAXBUFFER 2048
#define CHANNELS 4
#define SAMPLERATE 48000
#define OVERSAMPLING 4

int16_t buf[CHANNELS][MAXBUFFER];
volatile Bool railFull = false;

volatile curChannel=0;
volatile writePos=0;
volatile overSamplePos=0;
 

// HFSampleLoop speed is SAMPLERATE*OVERSAMPLING*CHANNELS
// i.e. $> 48000*8*4=1536000 = 1.536Mhz
void HFSampleLoop() // this will sample the stuff, when the whole mux has been done it will set a flag so the data can be evaluated
{
	int16_t result=0;
	
	// rad adc
	// result=
	if (overSamplePos==0)
		buf[curChannel][writePos]=result;
	else
		buf[curChannel][writePos]+=result;
	curChannel++;
	
	// data read, initiate new read on adc with next channel
	// restart
	if (curChannel==CHANNELS)
	{
		curChannel=0;
		overSamplePos++;
		if (overSamplePos==OVERSAMPLING)
		{
			overSamplePos=0;
			railFull = true;
			writePos++;
			if (writePos>=MAXBUFFER)
				writePos=0;
		}
	}		
}


int main (void)
{
	board_init();

	uint32_t clk = F_CPU/SAMPLERATE/OVERSAMPLING/CHANNELS;
	tc_set_resolution(&TCC1,clk);
	tc_set_overflow_interrupt_callback(&TCC1, HFSampleLoop);
	while(1)
	{
		// check for data to show
	}
}
