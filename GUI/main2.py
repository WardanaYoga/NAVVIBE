import serial
import tkinter as tk
import webbrowser
from collections import deque
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# ===============================
# SERIAL CONNECTION
# ===============================

try:
    ser = serial.Serial("COM3",9600,timeout=1)
    connected = True
except:
    connected = False

# ===============================
# GUI WINDOW
# ===============================

root = tk.Tk()
root.title("NavVibe - Monitoring Alat Tunanetra")
root.attributes('-fullscreen', True)
root.configure(bg="#0f172a")

# ===============================
# VARIABLES
# ===============================

distance_var = tk.StringVar(value="-- cm")
lat_var = tk.StringVar(value="--")
lon_var = tk.StringVar(value="--")

# ===============================
# HEADER
# ===============================

header = tk.Label(
    root,
    text="NAVVIBE MONITORING SYSTEM",
    font=("Arial",28,"bold"),
    bg="#0f172a",
    fg="white"
)
header.pack(pady=20)

# ===============================
# MAIN FRAME
# ===============================

main_frame = tk.Frame(root,bg="#0f172a")
main_frame.pack(expand=True,fill="both")

# ===============================
# LEFT PANEL (DATA)
# ===============================

left = tk.Frame(main_frame,bg="#1e293b",width=400)
left.pack(side="left",fill="y",padx=20,pady=20)

tk.Label(left,text="Jarak Objek",font=("Arial",18),
         bg="#1e293b",fg="white").pack(pady=10)

distance_label = tk.Label(
    left,
    textvariable=distance_var,
    font=("Arial",40,"bold"),
    bg="#1e293b",
    fg="green"
)

distance_label.pack(pady=20)

tk.Label(left,text="Latitude",font=("Arial",14),
         bg="#1e293b",fg="white").pack()

tk.Label(left,textvariable=lat_var,font=("Arial",12),
         bg="#1e293b",fg="white").pack(pady=5)

tk.Label(left,text="Longitude",font=("Arial",14),
         bg="#1e293b",fg="white").pack()

tk.Label(left,textvariable=lon_var,font=("Arial",12),
         bg="#1e293b",fg="white").pack(pady=5)

# ===============================
# GOOGLE MAPS BUTTON
# ===============================

def open_maps():

    lat = lat_var.get()
    lon = lon_var.get()

    if lat != "--":
        url = f"https://www.google.com/maps?q={lat},{lon}"
        webbrowser.open(url)

maps_btn = tk.Button(
    left,
    text="Buka Lokasi di Google Maps",
    font=("Arial",12),
    bg="#2563eb",
    fg="white",
    command=open_maps
)

maps_btn.pack(pady=20)

# ===============================
# STATUS
# ===============================

status = tk.Label(
    left,
    text="Connected" if connected else "Disconnected",
    fg="green" if connected else "red",
    bg="#1e293b",
    font=("Arial",12)
)

status.pack(pady=10)

# ===============================
# RIGHT PANEL (GRAPH)
# ===============================

right = tk.Frame(main_frame,bg="#0f172a")
right.pack(side="right",expand=True,fill="both",padx=20,pady=20)

distance_data = deque([0]*50,maxlen=50)

fig = plt.Figure(figsize=(6,4),dpi=100)
ax = fig.add_subplot(111)

line, = ax.plot(distance_data)

ax.set_title("Grafik Jarak Realtime")
ax.set_ylabel("cm")
ax.set_xlabel("time")

canvas = FigureCanvasTkAgg(fig,master=right)
canvas.get_tk_widget().pack(fill="both",expand=True)

# ===============================
# READ SERIAL DATA
# ===============================

def read_data():

    if connected and ser.in_waiting:

        try:

            data = ser.readline().decode(errors='ignore').strip()

            if "DIST:" in data:

                parts = data.split(",")

                dist = float(parts[0].split(":")[1])
                lat = parts[1].split(":")[1]
                lon = parts[2].split(":")[1]

                distance_var.set(str(dist) + " cm")
                lat_var.set(lat)
                lon_var.set(lon)

                # indikator warna
                if dist > 150:
                    distance_label.config(fg="green")
                elif dist > 80:
                    distance_label.config(fg="yellow")
                else:
                    distance_label.config(fg="red")

                # update grafik
                distance_data.append(dist)

                line.set_ydata(distance_data)
                line.set_xdata(range(len(distance_data)))

                ax.relim()
                ax.autoscale_view()

                canvas.draw()

        except:
            pass

    root.after(200,read_data)

# ===============================
# EXIT BUTTON
# ===============================

exit_btn = tk.Button(
    root,
    text="EXIT",
    font=("Arial",12),
    bg="red",
    fg="white",
    command=root.destroy
)

exit_btn.place(x=20,y=20)

read_data()

root.mainloop()
