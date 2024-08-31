#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

// A Single Radar Pulse
void generate_pulse(double *signal, int signal_length, double amplitude, double frequency, double phase) {
    for (int i = 0; i < signal_length; i++) {
        signal[i] = amplitude * sin(2 * PI * frequency * i / signal_length + phase);
    }
}

// Target Reflections
void add_target_reflections(double *signal, int signal_length, int num_targets) {
    srand(time(NULL));
    for (int i = 0; i < num_targets; i++) {
        int delay = rand() % signal_length;
        double amplitude = 0.1 + (rand() % 10) / 10.0; // Random amplitude between 0.1 and 1.0
        for (int j = delay; j < signal_length; j++) {
            signal[j] += amplitude * signal[j - delay];
        }
    }
}

double generate_gaussian_noise(double mean, double stddev) {
    // Box-Muller transform from https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
    double twopi =  PI * 2;

    double u, v, w = 0.0;
    do {
        u = (rand() / (RAND_MAX + 1.0)) * 2.0 - 1.0;
        v = (rand() / (RAND_MAX + 1.0)) * 2.0 - 1.0;
        w = u * u + v * v;
    } while (w >= 1.0 || w == 0.0);

    w = sqrt(-2.0 * log(w) / w);
    
    
}

double calculate_signal_power(double *signal, int signal_length) {
    // Calculate average power of the signal
    // ...
}

void add_noise(double *signal, int signal_length, double snr_db) {
    double signal_power = calculate_signal_power(signal, signal_length);

    // Calc SNR
    double noise_power = signal_power / pow(10, snr_db / 10.0);

    // add noise
    for (int i = 0; i < signal_length; i++) {
        double noise = generate_gaussian_noise(0, sqrt(noise_power));
        signal[i] += noise;
    }
}



int main() {
    int signal_length = 1000;
    double *signal = (double *)malloc(signal_length * sizeof(double));
    
    // Generate initial pulse
    generate_pulse(signal, signal_length, 1.0, 10.0, 0.0);
    
    // Add target reflections
    add_target_reflections(signal, signal_length, 3);
    
    // Print the first 20 samples of the signal
    printf("First 20 samples of the generated signal:\n");
    for (int i = 0; i < 20; i++) {
        printf("%f ", signal[i]);
    }
    printf("\n");
    
    free(signal);
    return 0;
}