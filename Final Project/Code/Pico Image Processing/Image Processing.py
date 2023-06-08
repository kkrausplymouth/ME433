# get a line of raw bitmap and plot the components
import serial
import numpy as np

ser = serial.Serial('COM6',230400) # the name of your Pico port
print('Opening port: ')
print(ser.name)

ser.write(b'hi\r\n') # send a newline to request data
data_read = ser.read_until(b'\n',50) # read the echo

sampnum = 0
index = 0
raw = []
reds = []
greens = []
blues = []
bright = []

# Pico sends back index and raw pixel value
while sampnum < 60: # width of bitmap
    data_read = ser.read_until(b'\n',50) # read until newline
    data_text = str(data_read,'utf-8') # convert bytes to string
    print(data_text)
    data = list(map(int,data_text.split())) # convert string to values

    if(len(data)==2):
        index = data[0]
        raw.append(data[1])
        reds.append(((data[1]>>5)&0x3F)/0x3F*100) # red value is middle 6 bits
        greens.append((data[1]&0x1F)/0x1F*100) # green value is rightmost 5 bits
        blues.append(((data[1]>>11)&0x1F)/0x1F*100) # blue vale is leftmost 5 bits
        bright.append((data[1]&0x1F)+((data[1]>>5)&0x3F)+((data[1]>>11)&0x1F)) # sum of colors
        sampnum = sampnum + 1

# print the raw color as a 16bit binary to double check bitshifting
for i in range(len(reds)):
    print(f"{raw[i]:#018b}")

redf = [reds[0]]*len(reds)
bluef = [blues[0]]*len(blues)
greenf = [greens[0]]*len(greens)
brightf = [bright[0]]*len(bright)
A = .3
B = .7
#filter data
for i in range(1, len(reds)):
    redf[i] = A*redf[i-1] + B*reds[i]
    greenf[i] = A*greenf[i-1] + B*greens[i]
    bluef[i] = A*bluef[i-1] + B*blues[i]
    brightf[i] = A*bright[i-1] + B*bright[i]

linevector = [0]*len(reds)
for i in range(len(brightf)):
    if brightf[i] > (np.max(brightf)*.9):
        linevector[i] = 100


# plot the colors 
import matplotlib.pyplot as plt 
x = range(len(reds)) # time array
plt.plot(x,redf,'r*-',x,greenf,'g*-',x,bluef,'b*-', x, brightf, 'k*-', x, linevector, 'm*-')
plt.ylabel('color')
plt.xlabel('position')




red_com = 0
green_com = 0
blue_com = 0
bright_com = 0
line_com = 0

red_sum = np.sum(redf)
blue_sum = np.sum(bluef)
green_sum = np.sum(greenf)
bright_sum = np.sum(brightf)
line_sum = np.sum(linevector)



for i in range(len(reds)):
    red_com += redf[i]*i
    blue_com += bluef[i]*i
    green_com += greenf[i]*i
    bright_com += bright[i]*i
    line_com += linevector[i]*i


red_com /= red_sum
green_com /= green_sum
blue_com /= blue_sum
bright_com /= bright_sum
line_com /= line_sum


print(red_com, green_com, blue_com)

plt.plot(red_com, 50, 'rX')
plt.plot(green_com, 50,'gX')
plt.plot(blue_com, 50,'bX')
plt.plot(bright_com, 50, 'kX')
plt.plot(line_com, 50, 'mX')
plt.legend(["Red Vals", "Green Vals", "Blue Vals", "Brightness Vals", "Vector", "Red COM", "Green COM", "Blue COM", "Brightness COM", "Line COM"])
plt.show()

# be sure to close the port
ser.close()