import numpy as np
import sys
import matplotlib.pyplot as plt
import math
from struct import pack, unpack
import cmath
from scipy.fftpack import fft, fftfreq, fftshift
from scipy import signal

def lpf_filter_coeff(order, fs, cutoff):

    nyq = fs/2
    normal_cutoff = cutoff/nyq

    b,a = signal.butter(order, normal_cutoff, btype='low', analog=False)

    return b,a

def main():

    path = "/home/fdourado/github_projects/hackrf_fm_radio/"
    filename = "test.txt"

    filepath = path+filename

    fs = 10e6 # samples/sec
    fc = 107950000.0 # center frequency


    #N = 2**22 # number of samples to collect
    #N = 2**21 # number of samples to collect
    N = 2**16 # FFT window chunk
    N_buffers = 1000

    x = np.zeros(N) + 1j*np.zeros(N)
    print x
    signal_buffer = []

    i = 0
    buffer_cnt = 0
    with open(filepath) as f:
        while True:

            I = f.read(1)
            Q = f.read(1)

            if not I:
                print "End of file"
                N_buffers = buffer_cnt
                break

            if not Q:
                print "End of file"
                N_buffers = buffer_cnt
                break


            if i < N:
                temp_samp = 1.0*ord(I) + 1j*1.0*ord(Q)
                x[i] = temp_samp
                i += 1
            else:

                if buffer_cnt == N_buffers:
                    break

                print buffer_cnt
                signal_buffer.append( x.copy() )
                buffer_cnt += 1
                i = 0
                #x.fill(0.0 + 1j*0.0)


    #plt.figure(1)
    #x = np.array(range(N))
    #plt.plot(x, abs(signal[0]), 'r', label='0')
    #plt.plot(x, abs(signal[1]), 'b', label='1')
    #plt.show()
    #ys.exit()

    order = 6
    cutoff = 200e3
    b,a = lpf_filter_coeff(order, fs, cutoff)

    FFT_bins = N
    F_res = fs / FFT_bins

    print "# FFT Bins %f" % (FFT_bins)
    print "Freq resolution %f Hz/bin" % (F_res)
    print "Max Frequency: %f MHz" % ( (fs/2.0)/1e6 )

    print len(signal_buffer)
    print N_buffers
    """ Perform N_buffers FFTs and hold the peak value from each """
    xf = fftfreq(N, 1.0 / fs)
    xf = fftshift(xf)

    xf = (xf + fc) / 1e6
    #xf = (xf + fc)

    peak_hold = np.zeros(N) - 1.0
    avg = np.zeros(N)
    #data = np.array([])

    for i in range(N_buffers):

        y = signal.lfilter(b, a, signal_buffer[i])

        yf = fft(y)
        yf = fftshift(yf)
        yf = np.abs(yf)
        temp = yf.copy()
        peak_hold = np.maximum(peak_hold, temp)
        avg = avg + (yf)
        #data = np.concatenate((data,y))

    avg = avg * (1.0/N_buffers)

    plt.figure(1)
    plt.plot(xf, 10 * np.log10( (1.0 / N) * peak_hold), 'b', label="Peak Hold" )
    plt.plot(xf, 10 * np.log10( (1.0 / N) * avg), 'r--', label="Average" )
    #plt.plot(xf, (1.0 / N) * peak_hold, 'b', label="Peak Hold" )
    #plt.plot(xf, (1.0 / N) * avg, 'r--', label="Average" )
    plt.xlim([107.9, 108.0])
    plt.legend()
    plt.grid()

    plt.figure(2)
    data = np.concatenate((signal_buffer[0],signal_buffer[100]))
    f, t, Sxx = signal.spectrogram(data, fs)

    f = (f + fc)/1e6
    plt.pcolormesh(t, f, Sxx)
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')
    plt.show()

    sys.exit()


if __name__ == "__main__":
    main()