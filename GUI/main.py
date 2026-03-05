import serial
import tkinter as tk

ser = serial.Serial('COM3',9600,timeout=1)

root = tk.Tk()
root.title("Monitoring Alat Tunanetra")
root.geometry("400x250")

distance_var = tk.StringVar()
lat_var = tk.StringVar()
lon_var = tk.StringVar()

tk.Label(root,text="Jarak Objek",font=("Arial",14)).pack()
tk.Label(root,textvariable=distance_var,font=("Arial",20)).pack()

tk.Label(root,text="Latitude",font=("Arial",12)).pack()
tk.Label(root,textvariable=lat_var).pack()

tk.Label(root,text="Longitude",font=("Arial",12)).pack()
tk.Label(root,textvariable=lon_var).pack()

status = tk.Label(root,text="Connected",fg="green")
status.pack(pady=10)


def read_data():

    if ser.in_waiting:

        data = ser.readline().decode('utf-8',errors='ignore').strip()

        try:

            parts = data.split(",")

            dist = parts[0].split(":")[1]
            lat = parts[1].split(":")[1]
            lon = parts[2].split(":")[1]

            distance_var.set(dist + " cm")
            lat_var.set(lat)
            lon_var.set(lon)

        except:
            pass

    root.after(200,read_data)

read_data()

root.mainloop()
