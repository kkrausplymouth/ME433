import math
import ulab.numpy as np
import time


def FFT(samples, timevector, sample_rate):
    y = samples # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/sample_rate
    frq = k/T # two sides frequency range
    Y, img = np.fft.fft(y) # fft computing and normalization

    return frq, Y

L = 1024

timevec = np.arange(L)
sineWave1 = np.zeros(L)
sineWave2 = np.zeros(L)
sineWave3 = np.zeros(L)
sumSines = np.zeros(L)

F1 = 1
F2 = 5
F3 = 10

for i in range(L):
    sineWave1[i] = math.sin(2*math.pi*F1*(i/1024))
    sineWave2[i] = math.sin(2*math.pi*F2*(i/1024))
    sineWave3[i] = math.sin(2*math.pi*F3*(i/1024))
    sumSines[i] = sineWave1[i] + sineWave2[2] + sineWave3[i]

frq, Y = FFT(sumSines, timevec, 1024)

print(Y[0])

for i in range(L):
    if i % 11 == 0:
        print("("+str(math.log(abs(Y[i])))+")")
        time.sleep(.005)

