#include "signal.hpp"
#include <math.h>
using namespace signal;

void shuffle(int N, complex* X);

void cswap(signal::complex* a, signal::complex* b);
int bitrev(int n, int B);

void dftmerge(int N,signal::complex* XF);
complex cexp(signal::complex z);

complex cadd(complex a, complex b);
complex csub(complex a, complex b);

complex cmul(complex a, complex b);
complex cdiv(complex a, complex b);

complex rmul(double a, complex z);
complex rdiv(complex z, double a);

complex conjg(complex z);
double real(complex z);

double aimag(complex z);
double cphase(complex z);


signal::complex cmplx(double x, double y) {

	signal::complex z;
	z.x = x;

	z.y = y;
	return z;

}

void signal::fft(int N, complex* X) {
	shuffle(N, X);
	dftmerge(N, X);
}


void shuffle(int N, complex* X)	/* N must be a power of 2 */

{
	int n, r, B = 1;

	while ((N >> B) > 0)	/* B = number of bits */

	{
		B++;

	}

	B--;

	for (n = 0; n < N; n++){

		r = bitrev(n, B);	/* bit-reversed version of n */

		if (r < n) continue; /* swap only half of the n's */
		cswap(X + n, X + r); /* swap by addresses */
	}

}


void cswap(complex* a, complex* b){
	complex t;

	t = *a;
	*a = *b;
	*b = t;

}




int bitrev(int x, int B)

{

	int m, r; r = 0;
	for (m = B - 1; m >= 0; m--) {
		/* if 2^m term is present, then */
		if ((x >> m) == 1)		{
			r += (int)pow(2, (B - 1 - m)); /* add (B-1-m)^2 to r, and */

			x -= (int)pow(2, m);	/* subtract m^2 from x */
		}

	}
	return(r);

}

void dftmerge(int N, complex* XF)
{

	int k, i, p, q, M;
	complex A, B, V, W;


	M = 2;

	while (M <= N)	// two M/2-DFTs into one M-DFT
	{

		W = cexp(cmplx(0.0, -2 * pi / M));	// order-M twiddle factor
		V = cmplx(1.0, 0.0);	// successive powers of W

		for (k = 0; k < M / 2; k++)	// index for an M/2-DFT
		{

			for (i = 0; i < N; i += M) // i-th butterfly; increment by M
			{

				p = k + i;	// absolute indices for i-th butterfly
				q = p + M / 2; // p: lower half, q: upper half

				A = XF[p];	// A = A(m)
				B = cmul(XF[q], V);  // B = W_N^m B(m)

				XF[p] = cadd(A, B);	// butterfly operations:		X(m) = A(m) + W_N ^ m B(m)

					XF[q] = csub(A, B);	// butterfly operations:	X(m + N / 2) = A(m) - W_N ^ m B(m)

		
			}

			V = cmul(V, W);	// V = VW = W\sp{k+1}\)
		}

		M = 2 * M;	// next stage
	}

}

double signal::cmag(complex z)	// magnitude of complex number z
{

	return sqrt(z.x * z.x + z.y * z.y);
}

double signal::cphase(complex z)	// phase of complex number z

{
	return atan2(z.y, z.x) / pi * 180;

}

complex conjg(complex z)	/* complex conjugate of z=x+jy */
{

	return cmplx(z.x, -z.y);	/* returns z* = x-jy */
}

complex cadd(complex a, complex b)	/* complex addition */
{
	return cmplx(a.x + b.x, a.y + b.y);

}
complex csub(complex a, complex b)	/* complex subtraction */
{
	return cmplx(a.x - b.x, a.y - b.y);
}
complex cmul(complex a, complex b)	/* complex multiplication */
{
	return cmplx(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);

}

complex rmul(double a, complex z) /* multiplication by real */ {

	return cmplx(a * z.x, a * z.y);
}

complex cdiv(complex a, complex b)	/* complex division */

{
	double D = b.x * b.x + b.y * b.y;

	return cmplx((a.x * b.x + a.y * b.y) / D, (a.y * b.x - a.x * b.y) /

		D);
}

complex rdiv(complex z, double a) /* division by real */ {
	return cmplx(z.x / a, z.y / a);

}

double real(complex z)	/* real part Re(z) */
{

	return z.x;
}

double aimag(complex z)	/* imaginary part Im(z) */

{
	return z.y;

}

complex cexp(complex z)	/* complex exponential */
{

	double R = exp(z.x);

	return cmplx(R * cos(z.y), R * sin(z.y));
}

unsigned long next_power_of_two(unsigned long x)

{
	x--;

	x |= x >> 1;
	x |= x >> 2;

	x |= x >> 4;
	x |= x >> 8;

	x |= x >> 16;
	x++;

	return x;
}

int isPowerOfTwo(int x)

{
	int result = x && !(x & (x - 1));

	return result;
}


