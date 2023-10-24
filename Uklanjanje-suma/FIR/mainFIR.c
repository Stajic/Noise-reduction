//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *                                                                          
// * Description:  Main function.
// *                                                                          
// * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
// * Copyright (C) 2011 Spectrum Digital, Incorporated
// *                                                                          
// *                                                                          
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "WAVheader.h"
#include "fir.h"
#include "high_pass_34th_order.h"
#include "high_pass_76th_order.h"
#include "high_pass_128th_order.h"

static WAV_HEADER outputWAVhdr;
static WAV_HEADER inputWAVhdr;

#define SAMPLE_RATE 48000L
#define GAIN 0

#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];

/* TO DO: Define history buffers and Rd/Wr pointers*/
/* Your code here */

Int16 history[HP3_LENGTH];
Uint16 state;

/*
 *
 *  main( )
 *
 */
void main( void )
{   
	int i, j;
	Int32 number_of_blocks;
	/* Initialize BSL */
	EZDSP5535_init( );


	/* Initialise hardware interface and I2C for code */
	aic3204_hardware_init();

	aic3204_set_input_filename("../15.wav");
	aic3204_set_output_filename("../out_signal1.wav");

	/* Initialise the AIC3204 codec */
	aic3204_init();

	aic3204_dma_init();

	aic3204_read_wav_header(&inputWAVhdr);

	// Set up output WAV header
	outputWAVhdr = inputWAVhdr;

	number_of_blocks = inputWAVhdr.data.SubChunk2Size/
				(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample*AUDIO_IO_SIZE/8);

	aic3204_write_wav_header(&outputWAVhdr);

    /* TO DO: Initialize history buffers to 0 */
    /* Your code here */

	for (i = 0; i < HP3_LENGTH; i++)
	{
		history[i] = 0;
	}

	OutputBufferL[0] = fir_circular(32767, highpass_3500Hz_34th_order, history, HP3_LENGTH, &state);

	for (i = 0; i < HP3_LENGTH; i++)
	{
		OutputBufferL[i] = fir_circular(0, highpass_3500Hz_34th_order, history, HP3_LENGTH, &state);
	}

	for (i = 0; i < HP3_LENGTH; i++)
	{
		history[i] = 0;
	}

	OutputBufferR[0] = fir_circular(32767, highpass_3500Hz_34th_order, history, HP3_LENGTH, &state);

	for (i = 0; i < HP3_LENGTH; i++)
	{
		OutputBufferR[i] = fir_circular(0, highpass_3500Hz_34th_order, history, HP3_LENGTH, &state);
	}


	for(i = 0; i < number_of_blocks; ++i)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		/* TO DO: Invoke filtering routine */
		/* Your code here */

		for(j = 0; j < AUDIO_IO_SIZE; j++)
		{
			OutputBufferL[j] = fir_circular(InputBufferL[j], highpass_3500Hz_34th_order, history, HP3_LENGTH, &state);
			OutputBufferR[j] = fir_circular(InputBufferR[j], highpass_3500Hz_34th_order, history, HP3_LENGTH, &state);
		}
		aic3204_write_block(OutputBufferL, OutputBufferR);
	}

	/* Disable I2S and put codec into reset */ 
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

