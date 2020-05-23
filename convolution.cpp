#include <stdio.h>
#include <stdlib.h>
#include "convolution.hpp"
#include "math.h"
/*
@author Ihor Yutsyk, 2020, MAY
*/
typedef struct { int startX; int endX; int startY;int endY;} ZONE;
double **selectZoneForAction(ZONE zone, double **matrix, int rows, int columns);
double ** matrixSelection(int rows, int columns, double ** matrix, int matrixYlim, int matrixXlim, int xShift, int yShift);
double multipleMatrixSum(double **m1, double **m2, int rows, int columns);
void mirrorMatrix(double **matrix, int rows, int columns);
ZONE selectSignalZone(double **matrix, int rows, int columns);
ZONE selectMainZone(ZONE zoneX, ZONE zoneC, int xRows, int xColumns, int cRows, int cColumns);
void swapSelection(double **matrix, double **selectedZone, ZONE zone);
int signal::conv(double *x, int xlen, double *c, int clen, double *y){
  
    double** ypArr = (double**)calloc(xlen + clen - 1, sizeof(double*));
    conv2d(&x, 1, xlen, &c, 1, clen, ypArr);
    for (int i = 0; i < xlen + clen - 1; i++) {
        y[i] = ypArr[0][i];
    }
    freeMatrixByRows(ypArr, 1);
    return 0;

}

int signal::conv2d(double **x, int xRows, int xColumns , double **c, int cRows, int cColumns, double **y){
    int yColumns = xColumns + cColumns - 1;
    int yRows  = xRows + cRows - 1; 
    
    int selRows = xRows > cRows ? xRows: cRows;
    int selCols = xColumns > cColumns ? xColumns : cColumns;
   
    for(int i =0; i < yRows; i++) {
        double *yRow =  (double*)calloc(yColumns, sizeof(double));
        for(int j = 0; j < yColumns; j++) {        
            int xShifter = selCols -1  - j;
            int yShifter = selRows -1   - i;
	 
            //select from x matrix
            double **xSelection = matrixSelection(selRows, selCols, x, xRows, xColumns, xShifter , yShifter);
            //select fromx c matrix
            double **cSelection = matrixSelection(selRows, selCols, c, cRows, cColumns, xShifter, yShifter);

            ZONE zoneX = selectSignalZone(xSelection, selRows, selCols);
            ZONE zoneC = selectSignalZone(cSelection, selRows, selCols);
            ZONE zone = selectMainZone(zoneX, zoneC, xRows, xColumns, cRows, cColumns);
           
           
            int pureRows = zone.endY - zone.startY + 1;
            int pureColumns = zone.endX - zone.startX + 1;



            if (zone.startX == selCols || zone.startY == selRows) {
                continue; 
            }


            double** pureZone = selectZoneForAction(zone, cSelection, selRows, selCols);

            mirrorMatrix(pureZone, pureRows, pureColumns);
            swapSelection(cSelection, pureZone, zone);
          
    
           
            yRow[j] = multipleMatrixSum(xSelection, cSelection, selRows, selCols);
           
            freeMatrixByRows(xSelection, selRows);
            freeMatrixByRows(cSelection, selRows);
            freeMatrixByRows(pureZone, pureRows);
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
	      if(i < 0 
                || i>matrixYlim-yShift
                || j < 0 
                || j > matrixXlim-xShift 
                || i >=rows || j >=columns
                || i >= matrixYlim
                || j >=matrixXlim) {
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

ZONE selectMainZone(ZONE zoneX, ZONE zoneC, int xRows, int xColumns, int cRows, int cColumns) {
    ZONE zone = {};
    if (xRows > cRows) {
        zone.startY = zoneX.startY;
        zone.endY = zoneX.endY;
    }
    else {
        zone.startY = zoneC.startY;
        zone.endY = zoneC.endY;
    }
    if (xColumns > cColumns) {
        zone.startX = zoneX.startX;
        zone.endX = zoneX.endX;
    }
    else {
        zone.startX = zoneC.startX;
        zone.endX = zoneC.endX;
    }
    return zone;
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

               if (maxX < j) {
                   maxX = j;
               }

               if(minY > i) {
                   minY = i;
               }
             
               if(maxY < i){
                   maxY = i;
               }
           }
        }
    }
    if(maxY < minY) maxY = minY;
    if(maxX < minX) maxX = minX;
    ZONE z;
    z.startX = minX;
    z.endX = maxX;
    z.startY = minY;
    z.endY = maxY;
    //ZONE z = {startX:minX, endX:maxX, startY:minY, endY:maxY};
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
    int mirroredColumns = (columns/ 2. > 0) ? round(columns / 2.) : 0;
    for(i = 0; i < mirroredRows; i++) {
        double *temp = matrix[i];
        matrix[i] = matrix[rows - i - 1];
        matrix[rows - i - 1] = temp;
    }
    
    for (int i = 0; i < rows; i++) {
        for (j = 0; j < mirroredColumns  ; j++) {
            double tempVal = matrix[i][j];
            matrix[i][j] = matrix[i][columns - j - 1];
            matrix[i][columns - j - 1] = tempVal;
        }
    }
    return;   
}