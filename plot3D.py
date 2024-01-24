import numpy as np
from sklearn.decomposition import PCA
import sys
import json
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

# Function to initialize the 3D plot
def init():
  scatter.set_data_3d([], [],[])
  return scatter,


# Function to update the 3D plot for each iteration
def update(frame):
    data = history[frame]
    x, y, z = zip(*data)
    scatter.set_data(x, y)
    scatter.set_3d_properties(z)
    return scatter,
# Load the JSON data from the file
try:
    with open("data.json", "r") as file:
        data_str = file.read()
        input_data = json.loads(data_str)
except FileNotFoundError:
    print("JSON data file not found.")
    sys.exit(1)
except json.JSONDecodeError as e:
    print("Error decoding JSON data:", e)
    sys.exit(1)


# Initialize the 'ans' list
history = []

# Assuming 'history' is a list of iterations, where each iteration is a list of particles
for iteration in input_data:
    temp = []
    for particle in iteration:
        temp1 = particle[:-1]  # Extract all but the last element
        temp.append(temp1)

    temp = np.array(temp)  # Convert to a NumPy array for PCA

    # Initialize and fit PCA
    pca = PCA(n_components=2)
    result = pca.fit_transform(temp)

    combined_result = []  # Create an empty list to store the combined result

    for i in range(len(iteration)):
        combined_result.append(list(result[i]) + [iteration[i][-1]])

    history.append(combined_result)

# Create the 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
scatter, = ax.plot([], [], [], marker='o', linestyle='None')

# Calculate the axis limits based on the entire data range
x_min = min(x for frame in history for x, y, z in frame)
x_max = max(x for frame in history for x, y, z in frame)
y_min = min(y for frame in history for x, y, z in frame)
y_max = max(y for frame in history for x, y, z in frame)
z_min = min(z for frame in history for x, y, z in frame)
z_max = max(z for frame in history for x, y, z in frame)

ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)
ax.set_zlim(z_min, z_max)
# Set aspect ratio (optional)
ax.set_box_aspect([1, 1, -1])
# Create the animation
interval = 200
ani = FuncAnimation(fig, update, frames=len(history), init_func=init, blit=False,interval=interval)

plt.show()