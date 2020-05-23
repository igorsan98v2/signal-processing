#include <stdio.h>
#include <stdlib.h>
#include "convolution.hpp"
#include "math.h"

typedef struct { int startX; int endX; int startY;int endY;} ZONE;
double **selectZoneForAction(ZONE zone, double **matrix, int rows, int columns);
double ** matrixSelection(int rows, int columns, double ** matrix, int matrixYlim, int matrixXlim, int xShift, int yShift);
double multipleMatrixSum(double **m1, double **m2, int rows, int columns);
void mirrorMatrix(double **matrix, int rows, int columns);
ZONE selectSignalZone(double **matrix, int rows, int columns);
void swapSelection(double **matrix, double **selectedZone, ZONE zone);
int signal::conv(double *x, int xlen, double *c, int clen, double *y){
    double *pcstart = c;// Remember start address of coeff// w: state register, wlen: state register length 
    int wlen = xlen;
    double *w = (double*)calloc(wlen, sizeof(double));
 
    for (int i = 0; i < xlen; i++){
        w[0] = *x;// Add newsample
        for (int j = 0; j < clen; j++){
            *y += *c * w[j];// Calculate convolution sumc++;}
        // Update state registers
            for (int k = 1; k < xlen; k++){
                w[wlen -k] = w[wlen -k -1];// Shift old samples
            }
            c = pcstart;// Reset coeff pointer
            x++;// Set px to point to new sample x(n) 
            y++;// Set py to point to new output
        }
      
    }
   
    return 0;

}

int signal::conv2d(double **x, int xRows, int xColumns , double **c, int cRows, int cColumns, double **y){
    int yColumns = xColumns + cColumns - 1;
    int yRows  = xRows + cRows - 1; 
    
   
   
    for(int i =0; i < yRows; i++) {
        double *yRow =  (double*)calloc(yColumns, sizeof(double));
        for(int j = 0; j < yColumns; j++) {        
            int xShifter = xColumns -1  - j;
            int yShifter = xRows -1   - i;
	 
            //select from x matrix
            double **xSelection = matrixSelection(xRows, xColumns, x, xRows, xColumns, xShifter , yShifter);
            //select fromx c matrix
            double **cSelection = matrixSelection(xRows, xColumns, c, cRows, cColumns, xShifter, yShifter);
        
            ZONE zoneC = selectSignalZone(cSelection, xRows, xColumns);
            double **pureZone = selectZoneForAction(zoneC, cSelection, xRows, xColumns);
            int pureRowsC = zoneC.endY - zoneC.startY +1;
            int pureColumnsC = zoneC.endX - zoneC.startX +1;
            mirrorMatrix(pureZone, pureRowsC, pureColumnsC);
            swapSelection(cSelection, pureZone, zoneC);
          
            ZONE zoneX = selectSignalZone(xSelection, xRows, xColumns);
            int pureRowsX = zoneX.endY - zoneX.startY +1;
            int pureColumnsX = zoneX.endX - zoneX.startX +1;
            double ** correctedC = matrixSelection(xRows,xColumns,cSelection,xRows, xColumns, pureColumnsX-pureColumnsC, 0);     
           
            freeMatrixByRows(cSelection, xRows);
            cSelection = correctedC;
           
            yRow[j] = multipleMatrixSum(xSelection, cSelection, xRows, xColumns);
           
            freeMatrixByRows(xSelection, xRows);
            freeMatrixByRows(cSelection, xRows);
            freeMatrixByRows(pureZone, pureRowsC);
        }

        y[i] = yRow;
    }
   
    return 0;
}

void signal::freeMatrixByRows(double ** matrix, int rows){
    for(int i=0;i<rows; i++){
        free(matrix[i]);
    }
    free(matrix);
}

double ** matrixSelection(int rows, int columns, double ** matrix, int matrixYlim, int matrixXlim, int xShift, int yShift){
    double **selection = (double**)calloc(rows, sizeof(double*));
    
    for(int i = -yShift;i<rows - yShift; i++){
        double *selectionRow =  (double*)calloc(columns, sizeof(double));
        for(int j = -xShift; j<columns - xShift; j++) {
	      if(i < 0 || (i>matrixYlim-yShift) || j < 0 
                || (j>matrixXlim-xShift) 
                || i>=rows || j >=columns) {
                selectionRow[j+xShift] = 0;
            } else {
                 selectionRow[j+xShift] = matrix[i][j];
            }
        }
	
        selection[i+yShift] = selectionRow;
    }
    
    return selection;
}

double multipleMatrixSum(double **m1, double **m2, int rows, int columns){
    double sum = 0;
    for (int i = 0,r = rows - 1; i < rows; i++, r--) {
        for (int j = 0, c = columns - 1; j < columns; j++, c--) {
            if(m1[i][j] != 0 && m2[i][j] != 0){
                sum += (m1[i][j] * m2[i][j]);
            }
        }
    }
  
    return sum;
}

ZONE selectSignalZone(double **matrix, int rows, int columns){
    int minX = columns;
    int minY = rows;
    int maxX =0;
    int maxY = 0;

    for(int i=0; i < rows; i++ ) {
        for( int j=0; j < columns; j++) {
           int condidate = matrix[i][j];
           if(matrix[i][j] != 0) {
               if(minX > j) {
                   minX = j;
               }
               if(minY > i) {
                   minY = i;
               }
               if(maxX < j){
                   maxX = j;
               }
               if(maxY < j){
                   maxY = i;
               }
           }
        }
    }
    if(maxY < minY) maxY = minY;
    if(maxX < minX) maxX = minX;
    ZONE z = {startX:minX, endX:maxX, startY:minY, endY:maxY};
    return z;
}

void swapSelection(double **matrix, double **selectedZone, ZONE zone){
    int selectRows = zone.endY - zone.startY +1;
    int selectColumns = zone.endX - zone.startX +1;
     for(int i = 0, y = zone.startY; i < selectRows; y++, i++){
        for(int j = 0, x = zone.startX; j < selectColumns; j++, x++){
            matrix[y][x] = selectedZone[i][j] ;
        } 
    }
}

double **selectZoneForAction(ZONE zone, double **matrix, int rows, int columns){
    int selectRows = zone.endY - zone.startY +1;
    int selectColumns = zone.endX - zone.startX +1;
    double **selection = (double**)calloc(selectRows, sizeof(double*));
    for(int i = 0, y = zone.startY; i < selectRows; y++, i++){
        selection[i] =  (double*)calloc(selectColumns, sizeof(double));
        for(int j = 0, x = zone.startX; j < selectColumns; j++, x++){
            selection[i][j] = matrix[y][x];
        } 
    }
    return selection;
}
void mirrorMatrix(double **matrix, int rows, int columns){
    int i =0;
    int j =0;
    int mirroredRows = (rows/2. > 0) ? round(rows/2.) : 1;
    for(i = 0; i < mirroredRows; i++) {
        double *temp = matrix[i];
        matrix[i] = matrix[rows - i - 1];
        for(j = 0; j< columns/2; j++){
            double tempVal = temp[j];
            temp[j] = temp[columns - j - 1];
            temp[columns - j - 1] = tempVal;
        }
        matrix[rows - i - 1] = temp;
    }  
    return;   
}