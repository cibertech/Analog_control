import tkinter
import serial
import msvcrt

from tkinter import *


class Application(tkinter.Frame):
    """ GUI """

    def __init__(self, master):
        """ Initialize the Frame"""
        tkinter.Frame.__init__(self, master)
        self.grid()
        self.create_widgets()
        self.updater()

    def create_widgets(self):
        self.button1 = tkinter.Button(m, text='Exit', width=20, command=destroy)
        self.button1.place(x=400, y=0)
        self.button2 = tkinter.Button(m, text='1 +', width=20, command=OnePlus)
        self.button2.place(x=10, y=0)
        self.button3 = tkinter.Button(m, text='1 -', width=20, command=OneMinus)
        self.button3.place(x=150, y=0)
        self.button4 = tkinter.Button(m, text='2 +', width=20, command=TwoPlus)
        self.button4.place(x=10, y=20)
        self.button5 = tkinter.Button(m, text='2 -', width=20, command=TwoMinus)
        self.button5.place(x=150, y=20)
        #self.text1 = Text(m, width=40, height=2)
        #self.text1.place(x=400, y=50)

    def run(self):
        loop_active = True
        #while loop_active:
        tdata = ser.read(ser.inWaiting())
        # time.sleep(1)
        # data_left = ser.inWaiting()
        #print(tdata)
        if len(tdata) > 4:
            #self.text1.delete(1.0, END)
            #self.text1.insert('1.0', tdata)
            process_data(tdata)
       # else:
        #    input_data.join(map(chr, tdata))# += tdata

    def updater(self):
        self.run()
        self.after(20, self.updater)


m = tkinter.Tk()

port = "COM9"
baud = 115200

ser = serial.Serial(port, baud, timeout=1)
input_data = ""


def OnePlus():
    ser.write(b'1,+' + bytes([10]))#bytes([13, 10]))


def OneMinus():
    ser.write(b'1,-' + bytes([10]))


def TwoPlus():
    ser.write(b'2,+' + bytes([10]))


def TwoMinus():
    ser.write(b'2,-' + bytes([10]))


def process_data(input_data):
    #global Application.w1
    global w1
    global w2
    global LED1
    global LED2
    global LED3
    global LED4
    x = input_data.decode().split(",")
    #check if format is correct
    if x[0] == 'S':
        value1 = x[1].isdigit()
        if value1:
            w1.set(x[1])
        value1 = x[2].isdigit()
        if value1:
            w2.set(x[2])
        tdata = 'V,' + x[1] + ',' + x[2] + '\r\n'
        text1.delete(1.0, END)
        text1.insert('1.0', tdata)
        ser.write(tdata.encode())
    elif x[0] == 'B':
        value1 = x[1].isdigit()
        if value1:
            if x[1] == "1":
                LED1.place(x = 30,y = 200)
            else:
                LED1.place_forget()
        value1 = x[2].isdigit()
        if value1:
            if x[2] == "1":
                LED2.place(x=30, y=250)
            else:
                LED2.place_forget()
        value1 = x[3].isdigit()
        if value1:
            if x[3] == "1":
                LED3.place(x=30, y=300)
            else:
                LED3.place_forget()
        value1 = x[4].isdigit()
        if value1:
            if x[4] == "1":
                LED4.place(x=30, y=350)
            else:
                LED4.place_forget()
    input_data = ""


def destroy():
    global ser
    global m
    ser.close()
    m.destroy()


m.title("Analog control test ")
m.minsize(width=100, height=100)
m.geometry('800x600+0+0')

w1 = Scale(m, from_=100, to=0)
w1.place(x = 10,y = 50)
w2 = Scale(m, from_=100, to=0)  # , orient=HORIZONTAL)
w2.place(x = 50,y = 50)  # .pack()
text1 = Text(m, width=40, height=2)
text1.place(x=400, y=50)
LED1 = Canvas(m, width=50, height=50)
LED1.place(x = 30,y = 200)
LED1.create_oval(10, 10, 40, 40, fill='blue')  # outline="#f11",fill="#1f1", width=2)
LED2 = Canvas(m, width=50, height=50)
LED2.place(x = 30,y = 250)
LED2.create_oval(10, 10, 40, 40, fill='blue')
LED3 = Canvas(m, width=50, height=50)
LED3.place(x = 30,y = 300)
LED3.create_oval(10, 10, 40, 40, fill='blue')
LED4 = Canvas(m, width=50, height=50)
LED4.place(x = 30,y = 350)
LED4.create_oval(10, 10, 40, 40, fill='blue')

# open the serial port
if ser.isOpen():
    print(ser.name + ' is open...')

APP = Application(m)

m.mainloop()
