import serial

ser = serial.Serial("COM7",115200,timeout=1)

print("Connected...")

while True:

    data = ser.readline().decode().strip()

    if data:
        print(data)
