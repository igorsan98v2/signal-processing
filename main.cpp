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
    double c1[3] = {1,2};
    double c2[3] = {3,4};
    double c3[3] = {5,6};
    double *c[3] = { 
        c1,
        c2,
        c3
    };
        
   
    int xColumns = 3; 
    int xRows =   3;
    int cColumns = 2;
    int cRows =   3;
    int yColumns = xColumns +  cColumns- 1;
    int yRows  = xRows + cRows - 1; 
    double **y =  (double**)calloc(yRows, sizeof(double*));
    std::cout << "conv2d:" <<std::endl; 	
    signal::conv2d((double**)x,xRows,xColumns,(double**)c,cRows,cColumns,y);
  
    for (int i = 0; i < yRows; i++){
        for (int j = 0; j < yColumns; j++){   
         printf("%3.2f\t",y[i][j]);
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
