# INFERNO Rover Code

Welcome to the official repository for Team INFERNO's Rover! This repository contains all the necessary code to control our rover's arm and driving movement using an ESP32, as well as integrate it with both ROS1 and ROS2 for joystick-based control.

## Repository Overview

### 1. **ESP32 Code**
- **Arm Control**: Code for controlling the robotic arm's movements.
- **Drive Control**: Code for handling the driving mechanism, including forward, backward, left, right, and speed control.

### 2. **ROS Integration**
- **Cross Compatibility**: This repository provides integration with both ROS1 and ROS2, enabling joystick movement for controlling the rover.
- **Joystick Integration**: A ROS node listens to the `/joy` topic, capturing joystick inputs and translating them into commands that are sent to the ESP32 for precise and intuitive control.

### 3. **Serial Communication**
This repository includes `ctrl.py`, which manages both the rover’s drive system and robotic arm via serial communication with the ESP32. Joystick inputs are captured through a ROS node, processed, and sent as serial commands to control the rover’s movements and arm functions in real-time.

### 4. **Pin Configuration**
Below are the pin numbers used for the ESP32 to control the rover's drive system and robotic arm:

- **Drive System(2 wheel drive):**
  - Left Motor: `{dir: 26, pwm: 25}`
  - Right Motor: `{dir: 33, pwm: 32}`

- **Drive System(6 wheel drive):**
  - **Left Front Motor**: `{dir: 19, pwm: 21}`
  - **Left Back Motor**: `{dir: 22, pwm: 23}`
  - **Left Middle Motor**: `{dir: 26, pwm: 25}`
  - **Right Front Motor**: `{dir: 13, pwm: 12}`
  - **Right Back Motor**: `{dir: 14, pwm: 27}`
  - **Right Middle Motor**: `{dir: 33, pwm: 32}`

- **Arm Control:**
  - Actuator 1: `{dir: 19, pwm: 21}`
  - Actuator 2: `{dir: 22, pwm: 23}`
  - Pitch: `{dir: 32, pwm: 33}`
  - Base: `{dir: 25, pwm: 26}`
  - Roll: `{dir: 5, pwm: 4}`
  - Gripper: `{dir: 2, pwm: 15}`


## Installation
This repository contains package for both ROS noetic and ROS2 humble.
### For ROS:
Place the `roverPilot` directory inside `src` folder of your workspace. Run the following commands and replace `catkin_ws` with workspace in which you want to install package.
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.sh
```

## Usage
### For ROS:
Use the following command to run nodes:
```bash
rosun roverPilot <node_name>
```
Possible values of <node_name>:
- `drive_node_test` : To test the working of drive.
- `drive_node` : To run the drive node.
- `arm_node_test` : To test the working of arm.
- `arm_node` : To run the arm node.
