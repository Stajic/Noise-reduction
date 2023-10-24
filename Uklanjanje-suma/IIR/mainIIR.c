//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *
// *
// * Description: Lab exercise 8
// * Course: ADOZ
// * Year: 2022
// *
// * Author: <TO DO - add student name and index>
// * Faculty of Technical Sciences, Novi Sad
// *
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "WAVheader.h"
#include "iir.h"
#include "IIR_coeffs.h"

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

Int16 p1[AUDIO_IO_SIZE];
Int16 p2[AUDIO_IO_SIZE];

Int16 x_history[3];
Int16 y_history[3];
Int16 x_history1[3];
Int16 y_history1[3];
Int16 x_history2[3];
Int16 y_history2[3];

/*
 *
 *  main( )
 *
 */
void main( void )
{   
	int i;
	Int32 j;
	Int32 number_of_blocks;

	/* Initialize BSL */
    EZDSP5535_init( );

	printf("\n ADOZ - Vezba 8 \n");
		
    /* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();
	
	aic3204_set_input_filename("../signal1.wav");
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
    for(i = 0; i < 3; i++)
    {
    		x_history[i] = 0;
    		y_history[i] = 0;
    		x_history1[i] = 0;
			y_history1[i] = 0;
			x_history2[i] = 0;
			y_history2[i] = 0;
    }

    OutputBufferL[0] = second_order_IIR(32767, IIR_2nd_order, x_history, y_history);

    for (i = 0; i < AUDIO_IO_SIZE; i++)
    {
    	OutputBufferL[i] = second_order_IIR(0, IIR_2nd_order, x_history, y_history);
    }


	for(i=0;i<number_of_blocks;i++)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		/* TO DO: Invoke filtering routine */
		/* Your code here */

		for(j = 0; j < AUDIO_IO_SIZE; j++)
		{
			p1[j] = second_order_IIR(InputBufferL[j], IIR_2nd_order, x_history, y_history);
			p2[j] = second_order_IIR(p1[j], IIR_2nd_order, x_history1, y_history1);
			OutputBufferL[j] = second_order_IIR(p2[j], IIR_2nd_order, x_history2, y_history2);
		}
		aic3204_write_block(OutputBufferL, OutputBufferR);
	}
    	
	/* Disable I2S and put codec into reset */ 
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

