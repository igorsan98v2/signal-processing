#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "signal.hpp"
using namespace std;
using namespace signal;
void convSample();
void conv();
void conv2();
void signal_generation();
void dftSample();
void fftSample();

int main() 
{ 
   
    // convSample();
    //   signal_generation();
    //dftSample();
    fftSample();

    getchar();
    return 0; 
}
void signal_generation() {
    double samplCount = 120;//seconds
    double fSampl = 4; 
    int samplSize = roundl(samplCount / (1 / fSampl));

    double* o1 = (double*) calloc(samplSize, sizeof(double));
  
    double f1 = 60;// Hz
    double phase1 = 40;// degress
    double ampl1 = 4;//meters
    create_sinewave(ampl1, f1, fSampl, phase1, samplCount, o1);

    double* o2 = (double*)calloc(samplSize, sizeof(double));
    double f2 = 120;// Hz
    double phase2 = 80;// angles
    double ampl2 = 4;//meters
    create_sinewave(ampl2, f2, fSampl, phase2, samplCount, o2);
    cout << "\nouput #1:" << endl;
    for (int i = 0; i < samplSize; i++) {
        cout << o1[i] << endl;
    }

    cout << "\nouput #2:" << endl;
    for (int i = 0; i < samplSize; i++) {
        cout << o2[i] << endl;
    }
    double *sumO = (double*)calloc(samplSize, sizeof(double));
    add_waves(o1, o2, samplSize, sumO);

    cout << "\nouput #3:" << endl;
    for (int i = 0; i < samplSize; i++) {
        cout << sumO[i] << endl;
    }
}


void conv() {
    std::cout << "conv:" << std::endl;
    int i, xlen, hlen, ylen;
    double x[4] = { 1, 2, 3, 4 };
    double h[3] = { 1, 2, 3 };
    double y[6] = { 0 };
    double w[6] = { 0 };
    xlen = sizeof(x) / sizeof(double);
    hlen = sizeof(h) / sizeof(double);
    ylen = xlen + hlen - 1;
    signal::conv(x, xlen, h, hlen, y);
    for (i = 0; i < ylen; i++) {
        printf("%3.2f%s ", y[i], i + 1 == ylen ? ";" : ", ");
    }
    printf("\n");
}

void  conv2d() {

    double x1[3] = { 1,2,3 };
    double x2[3] = { 4,5,6 };
    double x3[3] = { 7,8,9 };
    double* x[3] = {
        x1,
        x2,
        x3,
    };
    double c1[2] = { 1,2 };
    double c2[2] = { 3,4 };
    double c3[2] = { 5,6 };

    double* c[3] = {
        c1,
        c2,
        c3,
    };


    int xColumns = sizeof(x1) / sizeof(double);
    int xRows = sizeof(x) / sizeof(double*);
    int cColumns = sizeof(c1) / sizeof(double);
    int cRows = sizeof(c) / sizeof(double*);
    int yColumns = xColumns + cColumns - 1;
    int yRows = xRows + cRows - 1;
    double** y = (double**)calloc(yRows, sizeof(double*));

    std::cout << "conv2d:" << std::endl;
    signal::conv2d((double**)x, xRows, xColumns, c, cRows, cColumns, y);

    for (int i = 0; i < yRows; i++) {
        for (int j = 0; j < yColumns; j++) {
            printf("%3.2f\t", y[i][j]);
        }
        printf("\n");
    }

}

void convSample(){
    conv();
    conv2d();
}

void dftSample() {
    for (int bit = 8; bit < 33; bit += 8) {

        double fSampl = 4;
        double samplCount = bit / fSampl;//seconds
        int i = 0;
        int samplSize = roundl(samplCount / (1 / fSampl));
        double f1 = 60;// Hz
        double phase1 = 40;// degress
        double ampl1 = 4;//meters
        double* wave1 = (double*)calloc(samplSize, sizeof(double));
        create_sinewave(ampl1, f1, fSampl, phase1, samplCount, wave1);
        double* X_Re = (double*)calloc(samplSize, sizeof(double));
        double* X_Im = (double*)calloc(samplSize, sizeof(double));

        double* X_magnitude = (double*)calloc(samplSize, sizeof(double));
        double* X_phase = (double*)calloc(samplSize, sizeof(double));

        dft(wave1, samplSize, X_Re, X_Im, X_magnitude, X_phase);
        
        printf("DFT of first sequence (discrete frequency-domain) for %d bit`s:\n", bit);
        printf("index| |X_magnitude  || ||X_phase          |\n");
        for (i = 0; i < samplSize; i++) {
            printf("%5d\t%8.8lf\t%8.8lf\n", i, X_magnitude[i], X_phase[i]);
        }

        double* windowed = (double*)calloc(samplSize, sizeof(double));
        windowing(wave1, samplSize, 2, windowed);

        printf("Windowed sequence (discrete time-domain) for %d bit`s:\n", bit);
        for (i = 0; i < samplSize; i++) {
            printf("windowed[%d] = %lf\n", i, windowed[i]);
        }


       

        double* newwave = zeropad(windowed, samplSize, samplSize * 2);
        printf("Zero padded sequence (discrete time-domain) for %d bit`s:\n", bit);
        for (i = 0; i < samplSize * 2; i++) {
            printf("newwave[%d] = %lf\n", i, newwave[i]);
        }
        
        free(X_Re);
        free(X_Im);
        free(X_phase);
        free(X_magnitude);

        X_Re = (double*)calloc(samplSize *2, sizeof(double));
        X_Im = (double*)calloc(samplSize *2, sizeof(double));

        X_magnitude = (double*)calloc(samplSize *2, sizeof(double));
        X_phase = (double*)calloc(samplSize *2, sizeof(double));

        dft(newwave, samplSize * 2, X_Re, X_Im, X_magnitude, X_phase);
        printf("DFT of zero padded, windowed sequence (discrete frequency-domain) for %d bit`s:\n", bit);
        printf("index| |X_magnitude  || ||X_phase          |\n");
        for (i = 0; i < samplSize * 2; i++) {
            printf("%5d\t%8.8lf\t%8.8lf\n", i, X_magnitude[i], X_phase[i]);
        }

        
     


        double* wave2 = (double*)calloc(samplSize, sizeof(double));
        double f2 = 120;// Hz
        double phase2 = 80;// angles
        double ampl2 = 4;//meters
        create_sinewave(ampl2, f2, fSampl, phase2, samplCount, wave2);

        dft(wave2, samplSize, X_Re, X_Im, X_magnitude, X_phase); 
        printf("DFT of second sequence (discrete frequency-domain) for %d bit`s:\n", bit);

        printf("index| |X_magnitude  || ||X_phase          |\n");
        for (i = 0; i < samplSize; i++) {
            printf("%5d\t%8.8lf\t%8.8lf\n", i, X_magnitude[i], X_phase[i]);
        }
      
      
        double* sum = (double*)calloc(samplSize, sizeof(double));
        add_waves(wave1, wave2, samplSize, sum);

        dft(sum, samplSize, X_Re, X_Im, X_magnitude, X_phase);
        printf("DFT of sum of two sequences (discrete frequency-domain) for %d bit`s:\n", i);
        printf("index| |X_magnitude  || ||X_phase          |\n");
        for (i = 0; i < samplSize; i++) {
            printf("%5d\t%8.8lf\t%8.8lf\n", i, X_magnitude[i], X_phase[i]);
        }

      
        
        free(windowed);
        free(newwave);
        free(wave1);
        free(wave2);
        free(sum);
        free(X_Re);
        free(X_Im);
        free(X_phase);
        free(X_magnitude);
      
    }
}

void fftSample() {
    int bit;
    bit = 8;
    double fSampl = 4;
    double samplCount = bit / fSampl;//seconds
    int i = 0;
    int N = roundl(samplCount / (1 / fSampl));
    double f1 = 60;// Hz
    double phase1 = 40;// degress
    double ampl1 = 4;//meters

    double* wave = (double*)calloc(N, sizeof(double));
    complex* x = (complex*)calloc(N, sizeof(complex));
    
    create_sinewave(ampl1, f1, fSampl, phase1, samplCount, wave);
    for (i = 0; i < N; i++){
        x[i].x = wave[i];

    }

    double* X_Re = (double*)calloc(N, sizeof(double));
    double* X_Im = (double*)calloc(N, sizeof(double));

    double* X_magnitude = (double*)calloc(N, sizeof(double));
    double* X_phase = (double*)calloc(N, sizeof(double));

    dft(wave, N, X_Re, X_Im, X_magnitude, X_phase);

    fft(N, x);


    printf("DFT of first sequence (discrete frequency-domain) for %d bit`s:\n", bit);
    printf("index| |X_magnitude  || ||X_phase          |\n");
    for (i = 0; i < N; i++) {
        printf("%5d\t%8.8lf\t%8.8lf\n", i, X_magnitude[i], X_phase[i]);
    }

    printf("FFT of sequence for %d bit`s:\n", bit);
    printf("index| |X_magnitude  || ||X_phase          |\n");
    for (i = 0; i < N; i++) {
        printf("%5d\t%8.8lf\t%8.8lf\n", i, cmag(x[i]), cphase(x[i]));
    }
    free(X_Re);
    free(X_Im);
    free(X_phase);
    free(X_magnitude);
    free(wave);
    free(x);
   
}
