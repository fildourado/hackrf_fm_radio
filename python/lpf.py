import numpy as np
from scipy import signal
import sys
import matplotlib.pyplot as plt

def main():

    order = 6
    fs = 10e6
    nyq = fs/2
    cutoff = 200e3
    normal_cutoff = cutoff/nyq

    b,a = signal.butter(order, normal_cutoff, btype='low', analog=False)

    w, h = signal.freqz(b, a, worN=8000)

    plt.figure(1)
    plt.plot(0.5 * fs * w / np.pi, np.abs(h), 'b')
    plt.plot(cutoff, 0.5 * np.sqrt(2), 'ko')
    plt.axvline(cutoff, color='k')
    plt.xlim(0, 0.5 * fs)
    plt.title("Lowpass Filter Frequency Response")
    plt.xlabel('Frequency [Hz]')
    plt.grid()
    plt.show()

if __name__ == "__main__":
    main()