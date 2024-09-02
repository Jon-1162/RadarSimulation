#ifdef __cplusplus
extern "C" {
#endif

double* generate_radar_signal(int signal_length, double amplitude, double frequency, double phase, int num_targets, double snr_db);
void free_radar_signal(double* signal);

#ifdef __cplusplus
}
#endif