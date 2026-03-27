# f1tenth_auto_blocking

# Autonomous Blocking via Trajectory Switching

This repository contains the implementation of a robust autonomous racing framework for **defensive maneuvering** on the F1TENTH platform.  
Unlike standard single-path tracking, this system utilizes a **Multi-Lane Pure Pursuit** controller that dynamically switches between optimized global racelines (inner, middle, and outer) in real time to prevent opponents from overtaking.

---

## 🏎️ System Architecture

The framework is built using **ROS 2** and is divided into four functional layers:

- **Localization Layer:**  
  Performs vehicle state estimation using a particle filter within a pre-mapped environment.

- **Perception Layer:**  
  Employs a ZED2 RGB-D camera and a **YOLOv11** model to estimate the relative position and distance of trailing opponent vehicles.

- **Tactical Layer:**  
  A Python-based manager node that evaluates distance estimates and publishes the selected raceline index to the controller.

- **Control Layer:**  
  A high-performance C++ Pure Pursuit controller that generates lateral and longitudinal commands for the active raceline.

---

## 🛠️ Key Features

- **Multi-Trajectory Tracking:**  
  Maintains three concurrent racing lines with a parallelized state-tracking architecture to eliminate switching latency.

- **Dynamic Selection Logic:**  
  A Finite-State Machine (FSM) triggers defensive lane changes when an opponent is detected within a proximity threshold of `d < 2.0 m`.

- **Slew-Rate Limiting:**  
  Constrains the change in steering angle between control cycles to prevent particle filter divergence during aggressive transitions.

- **Optimized Trajectories:**  
  Three distinct racelines are synthesized to leave a lateral margin of roughly one vehicle width, effectively "sealing" the defended line against passing maneuvers.

---

## 📥 Installation

### Prerequisites

- **ROS 2** (Humble or Foxy recommended)  
- **NVIDIA Jetson Orin Nano**  
- **ZED2 SDK** and ROS 2 Wrapper  
- **YOLOv11** (Ultralytics)

### Build Instructions

```bash
# Clone the repository into your ROS 2 workspace
cd ~/f1tenth_ws/src
git clone <repository_url>

# Install dependencies
rosdep install --from-paths src --ignore-src -r -y

# Build the packages
colcon build

# Source the workspace
source install/setup.bash

## 🚀 Usage
ros2 launch f1tenth_stack bringup_launch.py
ros2 launch f1tenth_blocking localize_launch.py
ros2 launch f1tenth_blocking pure_pursuit_multi_launch.py
ros2 launch f1tenth_blocking car_distance_estimator.launch.py
ros2 launch f1tenth_blocking raceline_manager.launch.py