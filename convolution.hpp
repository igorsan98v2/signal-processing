#ifndef SIGNAL_CONV
#define SIGNAL_CONV
namespace signal{
int conv(double *x, int xlen, double *c, int clen, double *y);
int conv2d(double **x, int xRows, int xColumns , double **c, int cRows, int cColumns, double **y);
void freeMatrixByRows(double ** matrix, int rows);
}
#endif