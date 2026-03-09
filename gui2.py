import tkinter as tk
from tkinter import ttk
import serial
import threading
import tkintermapview

# =========================
# KONFIGURASI SERIAL
# =========================

PORT = "COM6"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

# =========================
# WINDOW GUI
# =========================

root = tk.Tk()
root.title("ESP32 GPS Monitoring System")
root.geometry("1000x600")

# =========================
# PANEL DATA
# =========================

left_frame = ttk.Frame(root)
left_frame.pack(side="left", fill="y", padx=10, pady=10)

title = ttk.Label(left_frame, text="GPS Information", font=("Arial",16))
title.pack(pady=10)

lat_label = ttk.Label(left_frame, text="Latitude : -", font=("Arial",12))
lat_label.pack(pady=5)

lon_label = ttk.Label(left_frame, text="Longitude : -", font=("Arial",12))
lon_label.pack(pady=5)

status_label = ttk.Label(left_frame, text="Status : Connected", foreground="green")
status_label.pack(pady=10)

# =========================
# EXIT BUTTON
# =========================

def exit_program():
    ser.close()
    root.destroy()

exit_button = ttk.Button(left_frame, text="EXIT", command=exit_program)
exit_button.pack(pady=20)

# =========================
# MAP FRAME
# =========================

map_frame = ttk.Frame(root)
map_frame.pack(side="right", fill="both", expand=True, padx=10, pady=10)

map_widget = tkintermapview.TkinterMapView(map_frame, width=700, height=500)
map_widget.pack(fill="both", expand=True)

map_widget.set_zoom(15)

marker = None

# =========================
# SERIAL LOG
# =========================

log_box = tk.Text(root, height=8)
log_box.pack(fill="x", padx=10, pady=5)

# =========================
# THREAD SERIAL
# =========================

def read_serial():

    global marker

    while True:

        try:

            line = ser.readline().decode().strip()

            if line:

                log_box.insert(tk.END, line + "\n")
                log_box.see(tk.END)

                parts = line.split(",")

                data = {}

                for p in parts:
                    key, value = p.split(":")
                    data[key] = value

                lat = float(data["LAT"])
                lon = float(data["LON"])

                lat_label.config(text=f"Latitude : {lat}")
                lon_label.config(text=f"Longitude : {lon}")

                if marker is None:
                    marker = map_widget.set_marker(lat, lon, text="ESP32 GPS")
                else:
                    marker.set_position(lat, lon)

                map_widget.set_position(lat, lon)

        except:
            pass

# =========================
# START THREAD
# =========================

thread = threading.Thread(target=read_serial)
thread.daemon = True
thread.start()

# =========================
# RUN PROGRAM
# =========================

root.mainloop()
