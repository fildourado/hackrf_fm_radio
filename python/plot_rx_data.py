import numpy as np
import sys
import matplotlib.pyplot as plt
import math
from struct import pack, unpack
import cmath
from scipy.fftpack import fft, fftfreq, fftshift


def main():

    path = "/home/fdourado/github_projects/hackrf_fm_radio/"
    filename = "test.txt"

    filepath = path+filename

    fs = 10e6 # samples/sec
    x = []

    N = 2**22 # number of samples to collect
    #N = 2**21 # number of samples to collect
    i = 0
    with open(filepath) as f:
        while True:
            c = f.read(1)
            if not c:
                print "End of file"
                break

            x.append(1.0*ord(c))

            #unsigned = ord(c)
            #signed = unsigned - 256 if unsigned > 127 else unsigned
            #x.append(1.0*signed)

            if i > N-2:
                break

            i+=1

    N = N/2
    x = np.array(x)

    I = x[0::2]
    Q = x[1::2]
    signal = I + 1j * Q


    t = np.array(range(N)) * (1.0/fs)
    mag = abs(signal)
    phase = np.angle(signal, deg=True)

    # Two subplots, the axes array is 1-d
    f, axarr = plt.subplots(2, sharex=True)
    axarr[0].plot(t, 10*np.log10(mag))
    axarr[0].set_title('Sharing X axis')
    axarr[1].plot(t, phase)


    FFT_bins = N
    F_res = fs / FFT_bins

    print "# FFT Bins %f" % (FFT_bins)
    print "Freq resolution %f Hz/bin" % (F_res)



    yf = fft(signal)
    xf = fftfreq(N, 1/fs)
    xf = fftshift(xf)
    yplot = fftshift(yf)

    plt.figure(2)
    #plt.plot(xf,yplot)
    plt.plot( xf, 10*np.log10( (1.0 / N) * np.abs(yplot) ) )
    #plt.xlim([-1,100])
    plt.grid()
    plt.show()

    sys.exit()


if __name__ == "__main__":
    main()