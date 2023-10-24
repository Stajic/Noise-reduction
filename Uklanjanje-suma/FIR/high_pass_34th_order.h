/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 35
 * Stable            : Yes
 * Linear Phase      : Yes (Type 1)
 * Arithmetic        : fixed
 * Numerator         : s16,15 -> [-1 1)
 * Input             : s16,15 -> [-1 1)
 * Type				 : HIGH PASS
 * Fs				 : 48 kHz
 * Fc				 : 3.5 kHz
 */

#include "tistdtypes.h"

#define HP1_LENGTH 35
Int16 highpass_3500Hz_34th_order[35] = {
						 -594,
				         -530,
				         -342,
				          -53,
				          291,
				          631,
				          892,
				         1007,
				          921,
				          602,
				           53,
				         -691,
				        -1566,
				        -2483,
				        -3339,
				        -4036,
				        -4491,
				        27523,
				        -4491,
				        -4036,
				        -3339,
				        -2483,
				        -1566,
				         -691,
				           53,
				          602,
				          921,
				         1007,
				          892,
				          631,
				          291,
				          -53,
				         -342,
				         -530,
				         -594
};
