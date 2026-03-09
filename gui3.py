import tkinter as tk
from tkinter import ttk
import serial
import threading
import tkintermapview

# =============================
# SERIAL CONFIG
# =============================

PORT = "COM6"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

# =============================
# GUI WINDOW
# =============================

root = tk.Tk()
root.title("ESP32 Navigation Monitoring System")
root.geometry("1200x700")

# =============================
# LEFT PANEL
# =============================

left_frame = ttk.Frame(root)
left_frame.pack(side="left", fill="y", padx=15, pady=15)

title = ttk.Label(left_frame, text="NAVIGATION DATA", font=("Arial",16))
title.pack(pady=10)

lat_label = ttk.Label(left_frame, text="Latitude : -", font=("Arial",12))
lat_label.pack(pady=5)

lon_label = ttk.Label(left_frame, text="Longitude : -", font=("Arial",12))
lon_label.pack(pady=5)

dist_label = ttk.Label(left_frame, text="Distance : - cm", font=("Arial",12))
dist_label.pack(pady=5)

status_label = ttk.Label(left_frame, text="Status : Connected", foreground="green")
status_label.pack(pady=10)

# =============================
# DISTANCE BAR
# =============================

dist_text = ttk.Label(left_frame, text="Obstacle Distance")
dist_text.pack(pady=5)

dist_bar = ttk.Progressbar(left_frame, length=200)
dist_bar.pack(pady=5)

# =============================
# EXIT BUTTON
# =============================

def exit_program():
    ser.close()
    root.destroy()

exit_button = ttk.Button(left_frame, text="EXIT", command=exit_program)
exit_button.pack(pady=20)

# =============================
# MAP AREA
# =============================

map_frame = ttk.Frame(root)
map_frame.pack(side="right", fill="both", expand=True, padx=10, pady=10)

map_widget = tkintermapview.TkinterMapView(map_frame, width=800, height=500)
map_widget.pack(fill="both", expand=True)

map_widget.set_zoom(16)

marker = None
path = []

# =============================
# SERIAL LOG
# =============================

log_box = tk.Text(root, height=8)
log_box.pack(fill="x", padx=10, pady=5)

# =============================
# UPDATE GUI FUNCTION
# =============================

def update_gui(lat, lon, dist, raw):

    global marker
    global path

    lat_label.config(text=f"Latitude : {lat}")
    lon_label.config(text=f"Longitude : {lon}")
    dist_label.config(text=f"Distance : {dist} cm")

    dist_bar["value"] = min(dist,100)

    path.append((lat,lon))

    if marker is None:
        marker = map_widget.set_marker(lat, lon, text="ESP32")
    else:
        marker.set_position(lat, lon)

    if len(path) > 1:
        map_widget.set_path(path)

    map_widget.set_position(lat, lon)

    log_box.insert(tk.END, raw + "\n")
    log_box.see(tk.END)

# =============================
# SERIAL THREAD
# =============================

def read_serial():

    while True:

        try:

            line = ser.readline().decode().strip()

            if line:

                parts = line.split(",")

                data = {}

                for p in parts:
                    key,value = p.split(":")
                    data[key] = value

                lat = float(data["LAT"])
                lon = float(data["LON"])
                dist = float(data["DIST"])

                root.after(0, update_gui, lat, lon, dist, line)

        except:
            pass

# =============================
# START THREAD
# =============================

thread = threading.Thread(target=read_serial)
thread.daemon = True
thread.start()

# =============================
# RUN GUI
# =============================

root.mainloop()
