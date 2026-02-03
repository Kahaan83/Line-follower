
# PID Line Follower Robot

A high-performance Arduino-based line follower robot utilizing a 5-sensor array and a PD (Proportional-Derivative) control algorithm for smooth and precise path tracking.

## 🚀 Features

* **PD Control Logic:** Uses Proportional and Derivative gains ( and ) to minimize oscillation and handle sharp turns.
* **Weighted Sensor Positioning:** Calculates the exact deviation from the center line using a weighted average of 5 infrared sensors.
* **Dynamic Motor Mixing:** Adjusts individual motor speeds in real-time to maintain course.
* **Anti-Hum Logic:** Ensures motors receive enough PWM duty cycle to overcome static friction at low speeds.
* **Active Braking:** Supports reverse motor rotation for tighter, more aggressive corrections.

## 🛠 Hardware Requirements

* **Microcontroller:** Arduino Uno/Nano or compatible.
* **Sensors:** 5-Channel IR Sensor Array (Digital output).
* **Motor Driver:** L298N or L293D Dual H-Bridge.
* **Chassis:** 2WD Robot Chassis.

## 📌 Pin Mapping

### IR Sensors (Analog Pins used as Digital)

| Sensor | Pin | Position |
| --- | --- | --- |
| S1 | A4 | Far Left |
| S2 | A3 | Left |
| S3 | A2 | Center |
| S4 | A1 | Right |
| S5 | A0 | Far Right |

### Motor Driver

| Component | Pin | Function |
| --- | --- | --- |
| **Right Motor** | 2 | IN1 |
|  | 3 | Speed (PWM) |
|  | 4 | IN2 |
| **Left Motor** | 7 | IN1 |
|  | 8 | IN2 |
|  | 9 | Speed (PWM) |

## ⚙️ How it Works

### 1. Position Calculation

The code assigns a numerical weight to each sensor:

* Far Left: `-2000` | Center: `0` | Far Right: `+2000`
The `getPosition()` function calculates the weighted average. If no line is detected, it returns a special value (`9999`) to tell the robot to use its last known error.

### 2. PD Algorithm

The correction value is calculated as:

```cpp
correction = (Kp * P) + (Kd * D)

```

* **Proportional (P):** Current distance from the center.
* **Derivative (D):** The rate of change of the error (prevents overshooting).

### 3. Tuning

You can modify these variables in the code to suit your specific track and motor power:

* `LEFT_BASE` / `RIGHT_BASE`: The cruising speed of the robot.
* `Kp`: Increase for sharper response; decrease if the robot wobbles.
* `Kd`: Increase to dampen the wobbling effect.

## 💻 Installation

1. Clone the repository:
```bash
git clone https://github.com/Kahaan83/Line-follower.git

```


2. Open `Line-follower.ino` in the Arduino IDE.
3. Select your Board and Port.
4. Click **Upload**.

## 📝 License

This project is open-source. Feel free to use and modify it for your own robot builds!
