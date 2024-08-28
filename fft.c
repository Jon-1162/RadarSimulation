#include <math.h>
#include <complex.h>
#include <stdlib.h>

void fft(double complex *X, int N) {
    if (N <= 1) return;

    // Divide
    double complex *X_even = malloc(N/2 * sizeof(double complex));
    double complex *X_odd = malloc(N/2 * sizeof(double complex));
    for (int i = 0; i < N/2; i++) {
        X_even[i] = X[i*2];
        X_odd[i] = X[i*2+1];
    }

    fft(X_even, N/2);
    fft(X_odd, N/2);

    // Conquer
    for (int k = 0; k < N/2; k++) {
        double complex t = cexp(-2.0 * I * M_PI * k / N) * X_odd[k];
        X[k] = X_even[k] + t;
        X[k + N/2] = X_even[k] - t;
    }

    free(X_even);
    free(X_odd);
}
