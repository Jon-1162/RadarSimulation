import ctypes
import numpy as np
import os

# Load the shared library
if os.name == 'posix':
    lib = ctypes.CDLL('./ftt.so')
elif os.name == 'nt':
    lib = ctypes.CDLL('./ftt.dll')
else:
    raise OSError("Unsupported operating system")

# Load the shared library
# lib = ctypes.windll.LoadLibrary(r'C:\Users\jco10\Documents\PersonalProjects\RadarSimulation\RadarSimulation\fft.dll')
# lib = ctypes.CDLL('fft.dll')
lib.fft.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]

lib.fft.restype = None

# Define the input signal
N = 1024
t = np.linspace(0, 1, N)
signal = np.sin(2 * np.pi * 50 * t)  # Example signal

# Prepare the complex array for FFT
complex_array = (ctypes.c_double * (2 * N))()
for i in range(N):
    complex_array[2 * i] = signal[i]
    complex_array[2 * i + 1] = 0.0

# Call the FFT function from the C library
lib.fft(complex_array, N)

# Extract the FFT result
fft_result = np.array([complex_array[2 * i] + 1j * complex_array[2 * i + 1] for i in range(N)])

# Plot the result
import matplotlib.pyplot as plt

plt.plot(np.abs(fft_result))
plt.title("FFT Result using C")
plt.xlabel("Frequency Bin")
plt.ylabel("Magnitude")
plt.show()
