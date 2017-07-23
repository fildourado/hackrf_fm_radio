import numpy as np
import sys
import matplotlib.pyplot as plt
import math
from struct import pack, unpack
import cmath
from scipy.fftpack import fft, fftfreq, fftshift
from scipy import signal
import wave
import struct

class SoundFile:
   def  __init__(self, x):
       self.file = wave.open('test.wav', 'wb')
       self.signal = x
       self.sr = 44100

   def write(self):
       self.file.setparams((1, 2, self.sr, len(self.signal), 'NONE', 'noncompressed'))
       self.file.writeframes(self.signal)
       self.file.close()


def import_data( filepath, N, N_buffers):

    x = np.zeros(N) + 1j*np.zeros(N)
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

                #print buffer_cnt
                signal_buffer.append( x.copy() )
                buffer_cnt += 1
                i = 0

    return signal_buffer, buffer_cnt

def main():

    # Sampling Variables
    fs = 4e6  # samples/se
    fm_fc = 107900000.0
    f_offset = -250000.0 - 253260
    f_offset = 1088000 - 1000
    f_offset = -500000
    audio_freq = 44100.0
    plot = True

    f_bw = 200000
    n_taps = 64


    print f_offset


    fc = fm_fc + f_offset
    N = 2 ** 10  # FFT window chunk
    # LPF Variables

    # Get the data
    path = "/home/fdourado/github_projects/hackrf_fm_radio/"
    filename = "test.txt"
    filepath = path+filename
    N_buffers = 30000

    print "Importing data"
    signal_buffer,N_buffers = import_data(filepath, N, N_buffers)

    x1 = np.reshape(signal_buffer, (1, N*N_buffers))
    x1 = x1[0]

    # To mix the data down, generate a digital complex exponential
    # (with the same length as x1) with phase -F_offset/Fs
    print "Mixing down to baseband"
    fc1 = np.exp(-1.0j * 2.0 * np.pi * f_offset / fs * np.arange(len(x1)))

    # Now, just multiply x1 and the digital complex expontential
    x2 = x1 * fc1

    if(plot):
        plt.figure(1)
        print "plot x1"
        plt.specgram(x1, NFFT=N, Fs=fs)
        plt.title("x1")
        plt.xlabel("Time (s)")
        plt.ylabel("Frequency (Hz)")
        plt.ylim(-fs / 2, fs / 2)
        plt.xlim(0, len(x2) / fs)
        plt.ticklabel_format(style='plain', axis='y')

        plt.figure(2)
        print "plot x2"
        plt.specgram(x2, NFFT=N, Fs=fs)
        plt.title("x2")
        plt.xlabel("Time (s)")
        plt.ylabel("Frequency (Hz)")
        plt.ylim(-fs / 2, fs / 2)
        plt.xlim(0, len(x2) / fs)
        plt.ticklabel_format(style='plain', axis='y')
        #plt.show()

    # An FM broadcast signal has  a bandwidth of 200 kHz
    print "filter and decimate"
    # Use Remez algorithm to design filter coefficients
#    lpf = signal.remez(n_taps, [0, f_bw, f_bw + (fs / 2 - f_bw) / 4, fs / 2], [1, 0], Hz=fs)
 #   x3 = signal.lfilter(lpf, 1.0, x2)

#    dec_rate = int(fs / f_bw)
 #   x4 = x3[0::dec_rate]
    # Calculate the new sampling rate
#    Fs_y = fs / dec_rate

    # An FM broadcast signal has  a bandwidth of 200 kHz
    dec_rate = int(fs / f_bw)
    x4 = signal.decimate(x2, dec_rate, zero_phase=True)
    # Calculate the new sampling rate
    Fs_y = fs / dec_rate


    # Clean up signal a bit - remove lower magnitude IQ samples
    #x4 = x4 / 50.0
    mag_x4 = np.abs(x4)
    avg_sig = np.average(mag_x4)
    if (plot):
        plt.figure(3)
        print "plot x4"
        plt.plot(np.array(range(len(mag_x4))), mag_x4, 'r')
        plt.plot(np.array(range(len(mag_x4))), np.ones(len(mag_x4))*avg_sig, 'b--')
        plt.title("x4")

    if(plot):
        plt.figure(4)
        print "plot x4"
        plt.specgram(x4, NFFT=N, Fs=Fs_y)
        plt.title("x4")
        plt.ylim(-Fs_y/2, Fs_y/2)
        plt.xlim(0,len(x4)/Fs_y)
        plt.ticklabel_format(style='plain', axis='y' )
        #plt.show()

    if(plot):
        # Plot the constellation of x4.  What does it look like?
        print "plot constellation"
        plt.figure(5)
        plt.scatter(1000*np.real(x4[0:50000]), 1000*np.imag(x4[0:50000]), color="red", alpha=0.05)
        #plt.plot(np.array(range(len(x4))), 10*np.log10(np.abs(x4)))
        plt.title("x4")
        plt.xlabel("real")
        #plt.xlim(-1.1, 1.1)
        plt.ylabel("imag")
        #plt.ylim(-1.1, 1.1)

    ### Polar discriminator
    y5 = x4[1:] * np.conj(x4[:-1])
    x5 = np.angle(y5)

    if(plot):
        plt.figure(6)
        plt.psd(x5, NFFT=N, Fs=Fs_y, color="blue")
        plt.title("x5")
        plt.axvspan(0,             15000,         color="red", alpha=0.2)
        plt.axvspan(19000-500,     19000+500,     color="green", alpha=0.4)
        plt.axvspan(19000*2-15000, 19000*2+15000, color="orange", alpha=0.2)
        plt.axvspan(19000*3-1500,  19000*3+1500,  color="blue", alpha=0.2)
        plt.ticklabel_format(style='plain', axis='y' )

    # The de-emphasis filter
    # Given a signal 'x5' (in a numpy array) with sampling rate Fs_y
    d = Fs_y * 75e-6   # Calculate the # of samples to hit the -3dB point
    x = np.exp(-1/d)   # Calculate the decay between each sample
    b = [1-x]          # Create the filter coefficients
    a = [1,-x]
    x6 = signal.lfilter(b,a,x5)

    # Find a decimation rate to achieve audio sampling rate between 44-48 kHz
    dec_audio = int(Fs_y/audio_freq)
    Fs_audio = Fs_y / dec_audio

    x7 = signal.decimate(x6, dec_audio, zero_phase=True)

    # Scale audio to adjust volume
    x7 *= 10000 / np.max(np.abs(x7))

    if (plot):
        plt.figure(7)
        print "plot x7"
        plt.specgram(x7, NFFT=N, Fs=Fs_audio)
        plt.title("x7")
        #plt.ylim(-Fs_audio / 2, Fs_audio / 2)
        plt.xlim(0, len(x7) / Fs_audio)
        plt.ticklabel_format(style='plain', axis='y')

        plt.show()

    x7 = x7.astype("int16")
    y = np.linspace(0.0, 1.0, len(x7))
    y_new = np.linspace(0.0, 1.0, 1.75*len(x7))
    x7 = np.interp(y_new, y, x7)


    values = []
    ssignal = ''
    for i in range(len(x7)):
        ssignal += wave.struct.pack('h', x7[i])  # transform to binary

    f = SoundFile(ssignal)
    f.write()


    # Save to file as 16-bit signed single-channel audio samples
    #("int16").tofile("wbfm-mono.raw")


if __name__ == "__main__":
    main()