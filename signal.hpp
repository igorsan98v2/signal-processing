#include <math.h>
#ifndef SIGNAL
#define SIGNAL
#define MATH_PI 3.14
#define pi 4.0 * atan(1.0)
#define WINDOW_TYPE_RECT 0
#define WINDOW_TYPE_TRIANGLE 1
#define WINDOW_TYPE_HAMM 2
#define WINDOW_TYPE_HANN 3
#define WINDOW_TYPE_BLACKMAN_HARISS 4
namespace signal {

	int conv(double *x, int xlen, double *c, int clen, double *y);
	int conv2d(double **x, int xRows, int xColumns, double **c, int cRows, int cColumns, double **y);
	void freeMatrixByRows(double ** matrix, int rows);
	/*
		@param amplitude - амплітудa
		@param fIn - частоту вхідного сигнал
		@param fSample - частоту вибірки
		@param phase  - фазу 
		@param samplesNum - кількість необхідних вибірок
		@param output - покажчик на послідовність виводу, nonNull
	*/
	int create_sinewave( double amplitude, double fIn, double fSamples, double phase, double sampleDuration, double *output);
	int add_waves(double* src1, double* src2, int srcLength, double * output);
	
	int dft(double* x, int N, double* X_Re, double* X_Im, double* X_magnitude, double* X_phase);
	double* zeropad(double* x, int xlen, int newlen);
	int windowing(double* x, int xlen, int window_type, double* y);

	typedef struct { double x; double y; } complex;
	void fft(int samplesNum, complex* X);
	double cmag(complex z);
	double cphase(complex z);
}
#endif