#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>

// Function to perform bit reversal on array indices
unsigned int bit_reverse(unsigned int x, int log2n) {
    int n = 0;
    for (int i = 0; i < log2n; i++) {
        n <<= 1;
        n |= (x & 1);
        x >>= 1;
    }
    return n;
}

// Iterative FFT function
void fft(double complex *X, int N) {
    int log2n = log2(N);
    
    // Bit-reversed addressing permutation
    for (int i = 0; i < N; i++) {
        int j = bit_reverse(i, log2n);
        // if (j > i) {
        //     double complex temp = X[i];
        //     X[i] = X[j];
        //     X[j] = temp;
        // }
    }

    // FFT butterfly operations
    double pie = 3.14;
    for (int s = 1; s <= log2n; s++) {
        int m = 1 << s;
        double complex wm = cexp(-2.0 * I * pie / m);
        for (int k = 0; k < N; k += m) {
            double complex w = 1.0;
            for (int j = 0; j < m / 2; j++) {
                double complex t = w * X[k + j + m / 2];
                double complex u = X[k + j];
                X[k + j] = u + t;
                X[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
}
