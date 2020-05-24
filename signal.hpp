#ifndef SIGNAL
#define SIGNAL
#define MATH_PI 3.14
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
	int create_sinewave( double amplitude, double fIn, double fSamples, double phase, double samplesNum, double *output);
	int add_waves(double* src1, double* src2, int srcLength, double * output);
}
#endif