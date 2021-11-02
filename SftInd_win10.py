## HONDA S660 (JW5) Shift indicator by qoosen
## CAN serial read for windows
## https://github.com/qoosen/ShiftIndicator
## File:SftInd_win.py

### CAN serial read for python3 
### Import library
from tkinter import *
import serial
### End of import library

### Define parameters
ser = serial.Serial("COM5", 115200)    ### Serial port depends on target system.
String_block = ''
c = Canvas(width = 1000, height = 600, background = '#00ffff') ### Data window size
c.pack()
WhSpdFL_kph = 0
WhSpdFR_kph = 0
WhSpdRL_kph = 0
WhSpdRR_kph = 0
EngSpd_rpm = 0
GearPos = 1
AccelPos_pct = 0
### End of define parameters

### Define functions
# ---------------------------------------------------
# Get information from CAN data
# ---------------------------------------------------
def GetDisplayItem():
    global WhSpdFL_kph
    global WhSpdFR_kph
    global WhSpdRL_kph
    global WhSpdRR_kph
    global GearPos
    global EngSpd_rpm
    global AccelPos_pct

    WhSpdFL_kph = str(String_block)[0:3]        ### Wheel speed Front Left [km/h]
    WhSpdFR_kph = str(String_block)[4:7]        ### Wheel speed Front Right [km/h]
    WhSpdRL_kph = str(String_block)[8:11]       ### Wheel speed Rear Left [km/h]
    WhSpdRR_kph = str(String_block)[12:15]      ### Wheel speed Rear Right [km/h]
    GearPos = str(String_block)[16:17]          ### Gear postion [Speed]
    EngSpd_rpm = str(String_block)[18:22]       ### Engine speed [rpm]
    AccelPos_pct  = str(String_block)[23:26]    ### Accelerator pedal opening [%]

# ---------------------------------------------------
# Display item indication
# ---------------------------------------------------
def IndicateDisplayItem():
    global WhSpdFL_kph
    global WhSpdFR_kph
    global WhSpdRL_kph
    global WhSpdRR_kph
    global EngSpd_rpm
    global AccelPos_pct
    ###Display CAN data
    c.create_rectangle(0, 0, 1000, 600, outline = '#00ffff', fill = '#00ffff')
    c.create_text(500, 50, text = "車輪速", font = ('', 50), fill = 'blue')  
    c.create_text(250, 120, text = "前左: " + str(WhSpdFL_kph) +" km/h", font = ('', 50), fill = 'blue')    
    c.create_text(750, 120, text = "前右: " + str(WhSpdFR_kph) +" km/h", font = ('', 50), fill = 'blue')
    c.create_text(250, 190, text = "後左: " + str(WhSpdRL_kph) +" km/h", font = ('', 50), fill = 'blue')
    c.create_text(750, 190, text = "後右: " + str(WhSpdRR_kph) +" km/h", font = ('', 50), fill = 'blue')
    c.create_text(250, 310, text = "APO: " + str(AccelPos_pct) +" %", font = ('', 50), fill = 'blue')   
    c.create_text(750, 310, text = "Ne: " + str(EngSpd_rpm) +" rpm", font = ('', 50), fill = 'blue')
    c.create_text(500, 470, text = "推定ギア位置: " + str(GearPos), font = ('', 80), fill = 'red')
    c.update()

### End of Define functions


# ---------------------------------------------------
# Main routine
# ---------------------------------------------------
try:
    while True:
        ###CAN data read 
        String_data = ser.read()
        if String_data == b'\n':
            GetDisplayItem()        ###Get information
            IndicateDisplayItem()   ###Display CAN data
            String_block = ''       ###Clear string block  
        else:
            String_block += str(String_data)[2:3]   ###Stack string block
except:
    pass

ser.close()

### End of Main routine
### End of File