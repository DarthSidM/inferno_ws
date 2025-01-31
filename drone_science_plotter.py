import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from pymavlink import mavutil

master = mavutil.mavlink_connection('/dev/ttyUSB0', baud=57600)
master.wait_heartbeat()
print(f"Connected to system {master.target_system}")


data_dict = {}


plt.style.use("dark_background")
fig = plt.figure(figsize=(15, 10))
fig.patch.set_facecolor("#000")  # Pure black background
plt.subplots_adjust(hspace=0.5)

# ✅ Create subplots for each sensor (4 rows x 2 columns)
axes = []
sensor_positions = [
    "mq131", "mq135", "mq7", "uv", "humidity", "temp", "pressure", "altitude"
]

for i in range(8):
    ax = fig.add_subplot(4, 2, i + 1)
    ax.set_title(sensor_positions[i].upper(), fontsize=12, color="lime", fontweight="bold")
    ax.set_xlabel("Time (frames)", color="cyan")
    ax.set_ylabel("Value", color="cyan")
    ax.set_facecolor("#002b36")  # Hacker dark green theme
    ax.grid(True, color="green", linestyle="--", alpha=0.4)
    axes.append(ax)

# ✅ Real-time update function
def update(frame):
    global data_dict

    # Receive MAVLink messages
    msg = master.recv_match(type="NAMED_VALUE_FLOAT", blocking=True)
    if msg:
        sensor_name = msg.name.strip()
        sensor_value = msg.value

        # ✅ Store values for plotting
        if sensor_name not in data_dict:
            data_dict[sensor_name] = []
        data_dict[sensor_name].append(sensor_value)

        # Keep only the last 50 data points
        if len(data_dict[sensor_name]) > 50:
            data_dict[sensor_name].pop(0)

    # ✅ Update plots
    for i, sensor in enumerate(sensor_positions):
        axes[i].clear()
        axes[i].set_title(sensor.upper(), fontsize=12, color="lime", fontweight="bold")
        axes[i].set_xlabel("Time (frames)", color="cyan")
        axes[i].set_ylabel("Value", color="cyan")
        axes[i].grid(True, color="green", linestyle="--", alpha=0.4)
        axes[i].set_facecolor("#002b36")  # Hacker dark green

        if sensor in data_dict:
            axes[i].plot(data_dict[sensor], linewidth=2, color="lime")

# ✅ Run animations
ani = animation.FuncAnimation(fig, update, interval=500)
plt.show()

