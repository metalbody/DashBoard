# Nano 33 BLE Sense Rev2 Telemetry Dashboard

A real-time telemetry and visualization platform for the Arduino Nano 33 BLE Sense Rev2 using Node.js, Socket.IO, and Chart.js.

This project streams live sensor data from the Arduino over serial communication to a Node.js gateway server, which broadcasts the data to multiple responsive web dashboards accessible from desktop or mobile devices.

---

# Features

## Real-Time Sensor Streaming

Streams live data from onboard sensors including:

* Accelerometer
* Gyroscope
* Magnetometer
* Temperature
* Humidity
* Pressure
* Microphone (approximate dB conversion)

---

## Live Interactive Dashboards

Dedicated visualization pages for:

* Main Dashboard
* Accelerometer Graphs
* Gyroscope Graphs
* Environmental Sensors
* Magnetometer Graphs
* Sound Monitoring

---

## Responsive Web Interface

* Mobile-friendly layout
* Responsive chart scaling
* Dedicated environment sensor layout
* Real-time WebSocket updates

---

## Distributed Architecture

The system separates:

* Sensor acquisition
* Data transport
* Visualization

Architecture:

```text
Arduino Nano 33 BLE Sense Rev2
        ↓
USB Serial Connection
        ↓
Node.js Gateway
        ↓
Socket.IO WebSocket Server
        ↓
Web Dashboard Clients
```

---

# Hardware Requirements

## Microcontroller

* Arduino Nano 33 BLE Sense Rev2

## Computer

* Windows/Linux/macOS system
* USB connection to Arduino

---

# Software Stack

## Backend

* Node.js
* Express.js
* Socket.IO
* serialport

## Frontend

* HTML5
* CSS3
* Chart.js
* Socket.IO Client

---

# Installation

## 1. Clone Repository

```bash
git clone https://github.com/metalbody/DashBoard.git
cd DashBoard
```

---

## 2. Install Dependencies

```bash
npm install
```

---

## 3. Upload Arduino Firmware

Upload the Arduino sketch to the Nano 33 BLE Sense Rev2 using Arduino IDE.

Required libraries:

* Arduino_BMI270_BMM150
* Arduino_HS300x
* Arduino_LPS22HB
* Arduino_APDS9960
* PDM

---

## 4. Configure Serial Port

Open:

```text
gateway.js
```

Update:

```javascript
path: 'COM5'
```

with your Arduino serial port.

Examples:

```text
COM3
COM6
/dev/ttyUSB0
```

---

## 5. Start Gateway Server

```bash
node gateway.js
```

---

## 6. Open Dashboard

Open browser:

```text
http://localhost:3000
```

---

# Project Structure

```text
project/
│
├── gateway.js
├── server.js
├── package.json
├── package-lock.json
│
├── public/
│   ├── index.html
│   ├── accel.html
│   ├── gyro.html
│   ├── env.html
│   ├── sound.html
│   ├── mag.html
│   └── style.css
│
└── sketch/
    └── telemetry.ino
```

---

# Dashboard Pages

## Main Dashboard

Displays:

* Live sensor values
* Connection status
* Navigation controls

---

## Accelerometer Dashboard

Displays:

* AX
* AY
* AZ

Real-time motion graphs.

---

## Gyroscope Dashboard

Displays:

* GX
* GY
* GZ

Filtered rotational motion visualization.

---

## Environment Dashboard

Displays:

* Temperature
* Humidity
* Pressure

Uses a responsive triangular layout.

---

## Magnetometer Dashboard

Displays:

* MX
* MY
* MZ

Real-time magnetic field visualization.

---

## Sound Dashboard

Displays:

* Approximate sound pressure level
* Real-time microphone activity

---

# Sensor Processing

## Accelerometer Filtering

Low-pass filtering reduces:

* sensor jitter
* high-frequency noise
* unstable readings

---

## Gyroscope Stabilization

Implements:

* low-pass filtering
* deadband thresholding

to reduce drift and near-zero noise.

---

## Microphone Processing

Microphone data is converted from raw PDM amplitude into approximate decibel values using RMS amplitude estimation.

Note:

The microphone values are approximate and are not calibrated SPL measurements.

---

# Cloud Deployment

This project can be deployed using:

* Render
* Railway
* ngrok
* VPS hosting

For cloud deployment:

```text
Arduino → Gateway → Cloud Server → Remote Clients
```

The gateway forwards serial sensor data to the cloud-hosted Socket.IO server.

---

# Known Limitations

* Microphone dB values are approximate
* No persistent database storage
* No authentication system
* No historical telemetry logging
* Arduino remains USB-connected to gateway system

---

# Future Improvements

Potential upgrades:

* MQTT integration
* Database storage
* Historical analytics
* FFT audio visualization
* 3D orientation rendering
* Sensor calibration
* Device authentication
* Multi-device support
* OTA firmware updates
* ESP32 WiFi migration

---

# Troubleshooting

## All Values Show Zero

Check:

* Arduino serial output
* COM port configuration
* gateway.js console logs
* Socket.IO connection
* browser console errors

---

## Charts Not Updating

Verify:

* Chart.js is loaded
* Socket.IO connection is active
* sensorData packets are received
* browser console has no JavaScript errors

---

## Git Push Authentication Errors

Use:

* GitHub Personal Access Token
* GitHub Desktop
* SSH authentication

instead of GitHub password authentication.

---

# License

This project is provided for educational and experimental purposes.

---

# Author

Ayush Kumar

Electrical Engineering Student

Telemetry, embedded systems, robotics, and IoT development.
