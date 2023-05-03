import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec

def FFT(samples, timevector, sample_rate):
    y = samples # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/sample_rate
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]
    return frq, Y

def plot(data, data_cleaned, timevector, sample_rate, title = "Data FIltering Analysis"):

    ### Initialize Plot
    fig = plt.figure(figsize=(10, 5))
    gs = GridSpec(nrows=3, ncols=2)

    ### FFT
    frq, Y = FFT(data, timevector, sample_rate)

    ax0 = fig.add_subplot(gs[1, 0])
    ax0.plot(timevector,data,'k')
    ax0.set_xlabel('Time')
    ax0.set_ylabel('Amplitude')
    ax0.set_title("Unfiltered data", pad=10)

    ax1 = fig.add_subplot(gs[2, 0])
    ax1.loglog(frq,abs(Y),'k') # plotting the fft
    ax1.set_xlabel('Freq (Hz)')
    ax1.set_ylabel('|Y(freq)|')
    ax1.set_title("FFT of unfiltered data", pad=10)

    ax2 = fig.add_subplot(gs[0, :])
    ax2.plot(timevector, data, 'k')
    ax2.plot(timevector, data_cleaned, 'r')
    ax2.legend(["Unfiltered", "Filtered"])
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Amplitude')
    ax2.set_title("IIR Filtered Data", pad=10)

    frq, Y = FFT(data_cleaned, timevector, sample_rate)

    ax3 = fig.add_subplot(gs[1, 1])
    ax3.plot(timevector,data_cleaned,'r')
    ax3.set_xlabel('Time')
    ax3.set_ylabel('Amplitude')
    ax3.set_title("Filtered data", pad=10)

    ax4 = fig.add_subplot(gs[2, 1])
    ax4.loglog(frq,abs(Y),'r') # plotting the fft
    ax4.set_xlabel('Freq (Hz)')
    ax4.set_ylabel('|Y(freq)|')
    ax4.set_title("FFT of Filtered data", pad=10)

    plt.subplots_adjust(left=0.1,
                        bottom=0.1,
                        right=0.9,
                        top=0.9,
                        wspace=0.4,
                        hspace=0.4)
    fig.suptitle(title, fontsize=30)
    plt.show()

def MAF(data, MAF_Length):
    MAL = MAF_Length
    data_MAF = [0]*MAL
    for i in range(MAL, len(data)):
        sum = 0
        for j in range(MAL):
            sum += data[i-j]
        data_MAF.append(sum/MAL)

    for i in range(MAL):
        data_MAF[i] = data_MAF[MAL]

    return data_MAF


def IIR(data, A, B):

    data_IIR = [0]
    for i in range(1, len(data)):
        data_IIR.append(A*data_IIR[i-1] + B*data[i])

    data_IIR[0] = data_IIR[1]

    return data_IIR


def FIR(data, h):
    LEN = len(h)

    data_FIR = [0]*LEN
    for i in range(LEN, len(data)):
        sum = 0
        for j in range(LEN):
            sum += data[i-j]*h[-1 - j]
        data_FIR.append(sum)

    for i in range(LEN):
        data_FIR[i] = data_FIR[LEN]

    return data_FIR
