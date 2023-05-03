import csv
from matplotlib import pyplot as plt
import numpy as np
from python_filtering_functions import plot, MAF, IIR, FIR
from h import h_sigD_HP

file = "sigD.csv"

### READ IN DATA

t = [] # column 0
data = [] # column 1

with open(file) as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data.append(float(row[1])) # second columnaxs[0, 0]
        #data2.append(float(row[2])) # third column

sample_rate = 1/(t[1] - t[0])
print(sample_rate)

### Moving Average Filter

MAL = 100
data_MAF = MAF(data, MAL)

title = "MAF Filter Analysis (" + str(MAL) + " Elements)"
plot(data, data_MAF, t, sample_rate, title)


### IIR Filter

#Parameters (A+B=1)
A = .8
B = .2
data_IIR = IIR(data, A, B)

title = "IIR Filter Analysis (A = " + str(A) + ", B = " + str(B) + ")"
plot(data, data_IIR, t, sample_rate, title)

###  FIR

data_FIR = FIR(data, h_sigD_HP)

plot(data, data_FIR, t, sample_rate, "FIR Filter Analysis")

