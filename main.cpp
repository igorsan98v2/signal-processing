#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "convolution.hpp"

void convSample();
int main() 
{ 
    std::cout << "conv:" <<std::endl; 
    convSample();
    double x1[3] = {1,2,3};
    double x2[3] = {4,5,6};
    double x3[3] = {7,8,9};
    double  *x[3] = { 
        x1,
        x2,
         x3,
     };
    double c1[2] = {1,2};
    double c2[2] = {3,4};
    double c3[2] = {5,6};
 
    double *c[3] = { 
        c1,
        c2,
        c3,
    };
        
   
    int xColumns = sizeof(x1) / sizeof(double);
    int xRows = sizeof(x) / sizeof(double*);
    int cColumns = sizeof(c1) / sizeof(double);
    int cRows = sizeof(c) / sizeof(double*);
    int yColumns = xColumns +  cColumns- 1;
    int yRows  = xRows + cRows - 1; 
    double **y =  (double**)calloc(yRows, sizeof(double*));
    
    std::cout << "conv2d:" <<std::endl; 	
    signal::conv2d((double**)x, xRows, xColumns, c,cRows, cColumns, y);
  
    for (int i = 0; i < yRows; i++) {
        for (int j = 0; j < yColumns; j++) {
            printf("%3.2f\t", y[i][j]);
        }
        printf("\n");
    }
    
    getchar();
    return 0; 
}

void convSample(){
    int i, xlen, hlen, ylen;
    double x[4] = { 1, 2, 3, 4 };
    double h[3] = { 1, 2, 3 };
    double y[6] = { 0 };
    double w[6] = { 0 };
    xlen = sizeof(x) / sizeof(double);
    hlen = sizeof(h) / sizeof(double);
    ylen = xlen + hlen -1;
    signal::conv(x, xlen, h, hlen, y);
    for (i = 0; i < ylen; i++){
	 printf("%3.2f%s ",y[i], i+1 == ylen ? ";": ", ");
    }
    printf("\n");
}
