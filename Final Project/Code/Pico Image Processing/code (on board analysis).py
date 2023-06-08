# requires adafruit_ov7670.mpy and adafruit_st7735r.mpy in the lib folder
import time
from displayio import (
    Bitmap,
    Group,
    TileGrid,
    FourWire,
    release_displays,
    ColorConverter,
    Colorspace,
    Palette,
)
from adafruit_st7735r import ST7735R
import board
import busio
import digitalio
from adafruit_ov7670 import (
    OV7670,
    OV7670_SIZE_DIV1,
    OV7670_SIZE_DIV8,
    OV7670_SIZE_DIV16,
)
from ulab import numpy as np

def findline(bitmap, row, A=3, B=.7, D=.8):
    for i in range(0,60):
        reds[i] = ((bitmap[row,i]>>5)&0x3F)/0x3F*100
        greens[i] = ((bitmap[row,i])&0x1F)/0x1F*100
        blues[i] = (bitmap[row,i]>>11)/0x1F*100
        bright[i] = reds[i]+greens[i]+blues[i]

    brightf = [bright[0]]*len(bright)
    A = .3
    B = .7
    #filter data
    for i in range(1, len(reds)):
        brightf[i] = A*bright[i-1] + B*bright[i]


    linevector = [0]*len(reds)
    for i in range(len(brightf)):
        if brightf[i] > (np.max(brightf)*.8):
            linevector[i] = 100


    line_com = 0

    for i in range(len(reds)):
        line_com += linevector[i]*i


    line_com /= np.sum(linevector)

    return line_com


release_displays()
spi = busio.SPI(clock=board.GP2, MOSI=board.GP3)
display_bus = FourWire(spi, command=board.GP0, chip_select=board.GP1, reset=None)
display = ST7735R(display_bus, width=160, height=128, rotation=90)

# Ensure the camera is shut down, so that it releases the SDA/SCL lines,
# then create the configuration I2C bus

with digitalio.DigitalInOut(board.GP10) as reset:
    reset.switch_to_output(False)
    time.sleep(0.001)
    bus = busio.I2C(board.GP9, board.GP8) #GP9 is SCL, GP8 is SDA

# Set up the camera (you must customize this for your board!)
cam = OV7670(
    bus,
    data_pins=[
        board.GP12,
        board.GP13,
        board.GP14,
        board.GP15,
        board.GP16,
        board.GP17,
        board.GP18,
        board.GP19,
    ],  # [16]     [org] etc
    clock=board.GP11,  # [15]     [blk]
    vsync=board.GP7,  # [10]     [brn]
    href=board.GP21,  # [27/o14] [red]
    mclk=board.GP20,  # [16/o15]
    shutdown=None,
    reset=board.GP10,
)  # [14]

width = display.width
height = display.height

bitmap = None
# Select the biggest size for which we can allocate a bitmap successfully, and
# which is not bigger than the display
for size in range(OV7670_SIZE_DIV1, OV7670_SIZE_DIV16 + 1):
    #cam.size = size # for 4Hz
    #cam.size = OV7670_SIZE_DIV16 # for 30x40, 9Hz
    cam.size = OV7670_SIZE_DIV8 # for 60x80, 9Hz
    if cam.width > width:
        continue
    if cam.height > height:
        continue
    try:
        bitmap = Bitmap(cam.width, cam.height, 65536)
        break
    except MemoryError:
        continue

print(width, height, cam.width, cam.height)
if bitmap is None:
    raise SystemExit("Could not allocate a bitmap")
time.sleep(4)
g = Group(scale=1, x=(width - cam.width) // 2, y=(height - cam.height) // 2)
tg = TileGrid(
    bitmap, pixel_shader=ColorConverter(input_colorspace=Colorspace.BGR565_SWAPPED)
)
g.append(tg)
display.show(g)

t0 = time.monotonic_ns()
display.auto_refresh = False

#UART SETUP
uart = busio.UART(board.GP4, board.GP5, baudrate=9600) #tx pin, rx pin

# in while True: after reading an image and finding the line


def send_info(line_pos):
    value_str = str(line_pos)+'\n'
    uart.write(value_str.encode())


# arrays to store the color data
reds = np.zeros(60,dtype=np.uint16)
greens = np.zeros(60,dtype=np.uint16)
blues = np.zeros(60,dtype=np.uint16)
bright = np.zeros(60)
while True:
    cam.capture(bitmap)

    A = .3
    B = .7
    D = .85
    numpts = 5
    lineloc = [0]*numpts



    for i in range(numpts):
        row = int((cam.width/(numpts + 1))*(i+1))

        rowline = findline(bitmap, row, A, B, D)
        print(row, int(rowline))

        bitmap[row,int(rowline)] = 0x3F<<5
        lineloc[i] = int(rowline)

    weights = [.5, .25, .125, 0.0625, .0625]
    avgL = 0
    for i in range(numpts):
        avgL += weights[i] * lineloc[i]

    send_info(int(avgL))
    print()
    print(avgL)
    print()

    bitmap.dirty() # updae the image on the screen
    display.refresh(minimum_frames_per_second=0)
    t1 = time.monotonic_ns()
    #print("fps", 1e9 / (t1 - t0))
    t0 = t1
