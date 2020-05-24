#include "signal.hpp";
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int signal::create_sinewave(double amplitude, double fIn, double fSamples, double phase, double samplesNum, double* output) {
	double tStep = 1 / fSamples;
	if (tStep <= 0) return -1;
	if (samplesNum <= 0) return -1;
	samplesNum = roundl (samplesNum/ tStep);
	for (int t = 0; t < samplesNum; t++) {
		output[t] = amplitude * sin(2 * MATH_PI * fIn * t + (phase*180.0/MATH_PI));
	}
	return 0;
}
int signal::add_waves(double* src1, double* src2, int srcLength, double* output) {
	if (srcLength <= 0) return -1;
	for (int t = 0; t < srcLength; t++) {
		output[t] = src1[t] + src2[t];
	}
	return 0;
	
}