#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159

// A Single Radar Pulse
void generate_pulse(double *signal, int signal_length, double amplitude, double frequency, double phase) {
    // signal -> list of doubles where the signal will be sotred over its indexes representing different times.
    // signal_length -> the count of signals over time.
    // amplitude -> in real systems this is the voltage
    // frequency -> in Hz
    // phase -> starting point of sin wave (0-2pi)
    for (int i = 0; i < signal_length; i++) {
        signal[i] = amplitude * sin(2 * PI * frequency * i / signal_length + phase);
    }
}

// Target Reflections
void add_target_reflections(double *signal, int signal_length, int num_targets) {
    srand(time(NULL));
    for (int i = 0; i < num_targets; i++) {
        int delay = rand() % signal_length;
        double amplitude = 1 + (rand() % 10) / 10.0;
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
    return mean + stddev * u * w; 
}

double calculate_signal_power(double *signal, int signal_length) {
    // Calculate average power of the signal
    // https://electronics.stackexchange.com/questions/154143/calculating-the-power-of-a-signal
    // https://en.wikipedia.org/wiki/Signal-to-noise_ratio
    double signal_power = 0.0;

    for (int i = 0; i < signal_length; i++) {
        double current_signal = signal[i];
        signal_power += current_signal * current_signal;
    }

    return signal_power / signal_length;
}

void add_noise(double *signal, int signal_length, double snr_db) {
    // snr_db is in decibels (dB)
    double signal_power = calculate_signal_power(signal, signal_length);

    // Calc SNR
    double noise_power = signal_power / pow(10, snr_db / 10.0);

    // add noise
    for (int i = 0; i < signal_length; i++) {
        double noise = generate_gaussian_noise(0, sqrt(noise_power));
        signal[i] += noise;
    }
}

double* generate_radar_signal(int signal_length, double amplitude, double frequency, double phase, int num_targets, double snr_db) {
    double* signal = (double*)malloc(signal_length * sizeof(double));
    
    generate_pulse(signal, signal_length, amplitude, frequency, phase);
    add_target_reflections(signal, signal_length, num_targets);
    add_noise(signal, signal_length, snr_db);
    
    return signal;
}

void free_radar_signal(double* signal) {
    free(signal);
}



// Sources used to research
// https://www.mathworks.com/discovery/how-do-radars-work.html#:~:text=Radar%20either%20uses%20its%20own,a%20change%20in%20the%20environment.
// https://www.reddit.com/r/ElectricalEngineering/comments/9jyuba/im_trying_to_understand_the_math_behind_radar/