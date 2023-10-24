/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 IIR_all_pass_filters.h                                                 */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file for IIR all pass filters.                                  */
/*                                                                           */
/*   The order of the coefficients is B0, B1/2, B2, A0, A1/2, A2, where      */
/*   B0, B1/2 and B2 are the numerator coefficients, A0, A1/2 and A2 are     */
/*   the denominator coefficients.                                           */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Dejan Bokan                                                   */
/*****************************************************************************/
#ifndef IIR_COEFFS_H
#define IIR_COEFFS_H

Int16 IIR_2nd_order[6] = {32767, -10, 32767,
								   32767, -10, 20000};
/*
A0 = 1
A1 = -2cos(2*pi*3500/48000)
A2 = 1
B1 = -2cos(2*pi*3500/48000)
B2 = 1 - (100/Q)^2
 */
#endif

/*****************************************************************************/
/* End of IIR_coeffs.h                                            */
/*****************************************************************************/
