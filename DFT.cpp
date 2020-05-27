#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "signal.hpp"

int signal::dft(double* x, int N, double* X_Re, double* X_Im, double* X_magnitude, double* X_phase)

{
	int n, m;

	for (m = 0; m < N; m++)
	{
		X_Re[m] = 0;
		X_Im[m] = 0;
		for (n = 0; n < N; n++)
		{

			X_Re[m] += x[n] * cos(2 * pi * n * m / N);
			X_Im[m] -= x[n] * sin(2 * pi * n * m / N);
			
		}

		X_magnitude[m] = sqrt(X_Re[m] * X_Re[m] + X_Im[m] *
			X_Im[m]);
		X_phase[m] = atan2(X_Im[m], X_Re[m]) / pi * 180;
	}
	return 0;

}

double* signal::zeropad(double* x, int xlen, int newlen)
{

	int i;

	double* y = (double*)calloc(newlen, sizeof(double));
	memcpy(y, x, xlen * sizeof(double)); 
	for (i = 0; i < (newlen - xlen); i++) {
		y[xlen + i] = 0;
	}
	return y;

}

int signal::windowing(double* x, int xlen, int window_type, double* y) {
	int i;
	switch (window_type)
	{
	case WINDOW_TYPE_RECT:

		//	Rectangular window 
		for (i = 0; i < xlen; i++)
		{
		y[i] = x[i];
		}
		break;

	case WINDOW_TYPE_TRIANGLE:

		//	Triangular window 
		for (i = 0; i < xlen; i++)
		{
			y[i] = x[i] * (1 - abs((i - (xlen - 1) / 2) / (xlen / 2)));
		}
		break;

	case WINDOW_TYPE_HAMM:

		//	Hamming window 
		for (i = 0; i < xlen; i++)
		{
		y[i] = x[i] * (0.54 + 0.46 * cos(2 * pi * i / (xlen - 1)));
		}
	break;

	case WINDOW_TYPE_HANN:
		// Hanning window
		for (i = 0; i < xlen; i++)
		{

			y[i] = x[i] * (0.5 + 0.5 * cos(2 * pi * i / (xlen - 1)));
		}
		break;

	case WINDOW_TYPE_BLACKMAN_HARISS:

		//	Blackman-Harris window 
		for (i = 0; i < xlen; i++)
         {

			y[i] = x[i] 
				* (0.35875 - 0.48829 
				* cos(2 * pi * i / (xlen - 1)) 
				+ 0.14128 * cos(4 * pi * i / (xlen - 1)) 
				- 0.01168 * cos(6 * pi * i / (xlen - 1))
				);
	    }

	break;

	default:
		//	Rectangular window is default 
		for (i = 0; i < xlen; i++){
			y[i] = x[i];
		}
	break;

	}
	return 0;

}
