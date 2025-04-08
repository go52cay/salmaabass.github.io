import pandas as pd
import matplotlib.pyplot as plt
import os

filename = "output.csv"
dirname = "csv_exports"

pathname = os.getcwd()
if pathname.endswith("src"):
    pathname = os.path.dirname(pathname)
pathname = os.path.join(pathname, dirname)

data = pd.read_csv(os.path.join(pathname, filename))

plt.plot(data["TimeStamp"], data["Velocity"], label="Velocity")
plt.plot(data["TimeStamp"], data["DesiredVelocity"], label="Desired Velocity")
plt.plot(data["TimeStamp"], data["Acceleration"], label="Acceleration")
plt.plot(data["TimeStamp"], data["ControllerOutput"] / 1000, label="Controller Output")

plt.xlabel("Time [s]")
plt.ylabel("Velocities [m/s] / Acceleration [m/s^2] / Controller Output [kN]")
plt.title("Cruise Control Visualization")
plt.legend()
plt.grid(True)
plt.show()
