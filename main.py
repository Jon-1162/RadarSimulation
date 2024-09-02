import ctypes
import numpy as np
import os
from scipy import signal
import matplotlib.pyplot as plt

# Import signal generator ------------------------------------------------------------------------------------------------------------
lib = ctypes.CDLL('./radarSigGen.dll')
lib.generate_radar_signal.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.generate_radar_signal.restype = ctypes.POINTER(ctypes.c_double)

lib.free_radar_signal.argtypes = [ctypes.POINTER(ctypes.c_double)]
lib.free_radar_signal.restype = None

def generate_radar_signal(signal_length, amplitude, frequency, phase, num_targets, snr_db):
    result = lib.generate_radar_signal(signal_length, amplitude, frequency, phase, num_targets, snr_db)
    np_array = np.ctypeslib.as_array(result, shape=(signal_length,))
    np_array = np.array(np_array)
    lib.free_radar_signal(result)
    return np_array

# FFT and filtering functions --------------------------------------------------------------------------------------------------------
def fft(signal):
    return np.fft.fft(signal)

def apply_bandpass_filter(input_signal, lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    
    if low <= 0 or high >= 1:
        raise ValueError("Lowcut and highcut frequencies must be between 0 and the Nyquist frequency.")
    
    b, a = signal.butter(order, [low, high], btype='band')
    filtered_signal = signal.lfilter(b, a, input_signal)
    return filtered_signal

def apply_window(input_signal, window_type='hamming'):
    window = signal.get_window(window_type, len(input_signal))
    return input_signal * window

# Object Detection -------------------------------------------------------------------------------------------------------------------
def peak_detection(post_fft):
    # Adjusted to have lower minimum height for peak detection
    return signal.find_peaks(post_fft, height=0.1 * np.max(post_fft), distance=10)

def thresh_detect(peaks, threshVal):
    aboveThresh = [(i, peaks[i]) for i in range(len(peaks)) if peaks[i] > threshVal]
    return aboveThresh

# Visualization -----------------------------------------------------------------------------------------------------------------------
def signal_plotting(time_series):
    plt.figure(figsize=(10, 6))
    plt.plot(np.arange(len(time_series)), time_series)
    plt.title('Raw Radar Signal')
    plt.xlabel('Sample Number')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.show()

def fft_plot(post_fft_sig):
    freq = np.fft.fftfreq(len(post_fft_sig))
    plt.figure(figsize=(10, 6))
    plt.plot(freq, np.abs(post_fft_sig))
    plt.title('FFT of Radar Signal')
    plt.xlabel('Frequency')
    plt.ylabel('Magnitude')
    plt.grid(True)
    plt.show()

def obj_detection_plot(original_sig, obj_locations):
    plt.figure(figsize=(10, 6))
    plt.plot(np.arange(len(original_sig)), original_sig, label='Original Signal')
    indices, values = zip(*obj_locations) if obj_locations else ([], [])
    plt.scatter(indices, values, color='red', label='Detected Objects')
    plt.title('Radar Signal with Detected Objects')
    plt.xlabel('Sample Number')
    plt.ylabel('Amplitude')
    plt.legend()
    plt.grid(True)
    plt.show()

# Main Simulation Loop ----------------------------------------------------------------------------------------------------------------
def run_simulation(num_iterations=1):  # Reduced iterations for demonstration
    for i in range(num_iterations):
        time_series_signal = generate_radar_signal(1000, 1.0, 50.0, 0.0, 30, 10.0)
        
        windowed_sig = apply_window(time_series_signal)
        # signal_plotting(time_series_signal)
        # signal_plotting(windowed_sig)
        
        fs = 1e6  # 1 MHz
        lowcut = .5  # 1 kHz
        highcut = 10000  # 10 kHz
        postFilter_sig = apply_bandpass_filter(windowed_sig, lowcut, highcut, fs)
        post_fft_sig = fft(postFilter_sig)
        
        peaks, _ = peak_detection(np.abs(post_fft_sig))
        threshold_value = np.mean(np.abs(post_fft_sig)) + 2 * np.std(np.abs(post_fft_sig))

        detected_objects = thresh_detect(peaks, threshold_value)
        

        obj_detection_plot(time_series_signal, detected_objects)

run_simulation()
