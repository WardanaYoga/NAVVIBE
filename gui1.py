import tkinter as tk
from tkinter import ttk
import serial
import threading
import tkintermapview

# ======================

# KONFIGURASI SERIAL

# ======================

SERIAL_PORT = "COM7"   # ganti sesuai port bluetooth
BAUDRATE = 115200

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

# ======================

# WINDOW

# ======================

root = tk.Tk()
root.title("ESP32 Assistive Navigation System")
root.geometry("1200x700")
root.configure(bg="#1e1e1e")

# ======================

# FRAME DASHBOARD

# ======================

left_frame = tk.Frame(root, bg="#1e1e1e", width=350)
left_frame.pack(side="left", fill="y")

right_frame = tk.Frame(root)
right_frame.pack(side="right", fill="both", expand=True)

# ======================

# TITLE

# ======================

title = tk.Label(
left_frame,
text="SMART NAVIGATION",
font=("Arial", 20, "bold"),
fg="white",
bg="#1e1e1e"
)
title.pack(pady=20)

# ======================

# STATUS

# ======================

status_label = tk.Label(
left_frame,
text="STATUS: CONNECTED",
font=("Arial", 12),
fg="lightgreen",
bg="#1e1e1e"
)
status_label.pack(pady=10)

# ======================

# DISTANCE DISPLAY

# ======================

distance_title = tk.Label(
left_frame,
text="ULTRASONIC DISTANCE",
font=("Arial", 12),
fg="white",
bg="#1e1e1e"
)
distance_title.pack(pady=5)

distance_label = tk.Label(
left_frame,
text="0 cm",
font=("Arial", 32, "bold"),
fg="cyan",
bg="#1e1e1e"
)
distance_label.pack(pady=10)

# ======================

# OBSTACLE STATUS

# ======================

obstacle_label = tk.Label(
left_frame,
text="SAFE",
font=("Arial", 18, "bold"),
fg="green",
bg="#1e1e1e"
)
obstacle_label.pack(pady=20)

# ======================

# GPS INFO

# ======================

lat_label = tk.Label(
left_frame,
text="Latitude: -",
font=("Arial", 12),
fg="white",
bg="#1e1e1e"
)
lat_label.pack(pady=5)

lon_label = tk.Label(
left_frame,
text="Longitude: -",
font=("Arial", 12),
fg="white",
bg="#1e1e1e"
)
lon_label.pack(pady=5)

# ======================

# MAP

# ======================

map_widget = tkintermapview.TkinterMapView(
right_frame,
width=800,
height=700,
corner_radius=0
)

map_widget.pack(fill="both", expand=True)

map_widget.set_zoom(17)

marker = None

# ======================

# DATA PARSER

# ======================

def parse_data(line):

```
try:

    parts = line.split(",")

    lat = float(parts[0].split(":")[1])
    lon = float(parts[1].split(":")[1])
    dist = float(parts[2].split(":")[1])

    return lat, lon, dist

except:
    return None
```

# ======================

# SERIAL THREAD

# ======================

def read_serial():

```
global marker

while True:

    if ser.in_waiting:

        line = ser.readline().decode().strip()

        data = parse_data(line)

        if data:

            lat, lon, dist = data

            root.after(0, update_gui, lat, lon, dist)
```

# ======================

# UPDATE GUI

# ======================

def update_gui(lat, lon, dist):

```
global marker

lat_label.config(text=f"Latitude: {lat}")
lon_label.config(text=f"Longitude: {lon}")

distance_label.config(text=f"{dist:.1f} cm")

if dist < 100:

    obstacle_label.config(text="OBSTACLE!", fg="red")

else:

    obstacle_label.config(text="SAFE", fg="green")

map_widget.set_position(lat, lon)

if marker:
    marker.delete()

marker = map_widget.set_marker(lat, lon, text="Device")
```

# ======================

# THREAD START

# ======================

thread = threading.Thread(target=read_serial)
thread.daemon = True
thread.start()

root.mainloop()
