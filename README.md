# Linux-Based Virtual IoT Sensor Using a Loadable Kernel Module

## 1. Project Overview

This project implements a Linux-based virtual IoT sensor using a **Loadable Kernel Module (LKM)** and a **character device**. Instead of using a physical temperature or humidity sensor, the kernel module generates virtual sensor readings and exposes them through:

```text
/dev/virtual_sensor
```

A user-space C application reads the sensor data from the character device and publishes it using **MQTT**. **Mosquitto** is used as the local MQTT broker, while **Node-RED** receives, processes, and visualizes the data on a dashboard.

The project demonstrates the complete path from a Linux kernel module to an IoT visualization platform.

## 2. Objectives

- Implement a Linux Loadable Kernel Module.
- Create and register a character device.
- Provide a virtual temperature and humidity sensor interface.
- Demonstrate communication between kernel space and user space.
- Develop a C-based user-space application.
- Publish sensor data using MQTT.
- Use Mosquitto as a local MQTT broker.
- Process and visualize sensor data using Node-RED.
- Demonstrate a complete embedded Linux and IoT data pipeline.

## 3. System Architecture

```text
+----------------------+
|   Virtual Sensor     |
| Temperature/Humidity |
+----------+-----------+
           |
           v
+----------------------+
| Linux Kernel Module  |
|   Character Device   |
+----------+-----------+
           |
           v
+----------------------+
| /dev/virtual_sensor  |
+----------+-----------+
           |
           v
+----------------------+
| User-Space C App     |
|  Reads Sensor Data   |
+----------+-----------+
           |
           | MQTT
           v
+----------------------+
| Mosquitto Broker     |
|    localhost:1883    |
+----------+-----------+
           |
           v
+----------------------+
|      Node-RED        |
| Parse + Process Data |
+----------+-----------+
           |
           v
+----------------------+
|   Node-RED Dashboard |
| Temperature / Humidity|
+----------------------+
```

## 4. Technologies Used

| Component | Technology |
|---|---|
| Operating System | Ubuntu 24.04.2 LTS |
| Kernel | Linux 6.14.0-32-generic |
| Kernel Module | C |
| Character Device | Linux Device Driver Interface |
| User Application | C |
| MQTT Broker | Mosquitto |
| IoT Messaging | MQTT |
| Visualization | Node-RED Dashboard |
| Development Environment | VMware Workstation Pro |

## 5. Hardware and Software Requirements

### Hardware

No physical sensor or microcontroller is required.

The project was implemented inside an Ubuntu virtual machine running on a Windows host system.

### Software

Required software includes:

- Ubuntu Linux
- Linux kernel headers
- GCC
- Make
- Mosquitto
- Mosquitto clients
- `libmosquitto-dev`
- Node.js
- Node-RED
- Node-RED Dashboard 2.0

## 6. Project Structure

```text
Virtual-Sensor-Module/
│
├── dashboard/
│   └── flows.json
│
├── docs/
│   ├── screenshots/
│   │   ├── 01_kernel_module_build.png
│   │   ├── 02_module_and_device.png
│   │   ├── 03_virtual_sensor_output.png
│   │   ├── 04_user_app_mqtt.png
│   │   ├── 05_nodered_mqtt.png
│   │   └── 06_dashboard.png
│   │
│   └── testing/
│       └── testing.md
│
├── kernel_module/
│   ├── Makefile
│   └── virtual_sensor.c
│
├── user_app/
│   ├── Makefile
│   └── sensor_app.c
│
├── .gitignore
└── README.md
```

## 7. Kernel Module

The kernel module implements the virtual sensor and registers a character device.

The module performs the following operations:

1. Initializes the kernel module.
2. Allocates a character device number.
3. Registers the character device.
4. Creates the device class.
5. Creates `/dev/virtual_sensor`.
6. Generates virtual temperature and humidity values.
7. Provides the generated data through the device `read()` operation.
8. Logs device operations using the Linux kernel logging system.
9. Cleans up the device and module when unloaded.

The device can be accessed from user space through:

```text
/dev/virtual_sensor
```

## 8. Virtual Sensor Data

The sensor is virtual, so no physical temperature or humidity sensor is required.

The kernel module generates values representing:

- Temperature in degrees Celsius
- Relative humidity in percentage

Example:

```text
Temperature: 29 C
Humidity: 46 %
```

The values are returned when the character device is read.

## 9. User-Space Application

The user-space application is written in C.

Its main functions are:

1. Open `/dev/virtual_sensor`.
2. Read the generated sensor data.
3. Display the sensor values.
4. Connect to the local MQTT broker.
5. Publish the readings to the MQTT topic.
6. Repeat the process at a fixed interval.

The MQTT topic used is:

```text
virtual_sensor/data
```

## 10. MQTT Communication

The project uses MQTT's publish/subscribe communication model.

The user-space application acts as the MQTT publisher.

Mosquitto acts as the MQTT broker.

Node-RED acts as the subscriber and processing/visualization application.

```text
User-Space C Application
          |
          | Publish
          v
     Mosquitto Broker
          |
          | Subscribe
          v
       Node-RED
```

The MQTT broker runs locally on:

```text
localhost:1883
```

The project uses MQTT QoS 0 for the local prototype.

## 11. Node-RED Dashboard

Node-RED receives messages from:

```text
virtual_sensor/data
```

The incoming sensor message is processed by a Function node.

The Function node extracts:

- Temperature
- Humidity

These values are then sent to separate dashboard gauges.

The final dashboard displays:

- Temperature in °C
- Humidity in %

The Node-RED dashboard is available at:

```text
http://localhost:1880/dashboard
```

## 12. Installation

### Install Build Tools and Kernel Headers

Update the package list:

```bash
sudo apt update
```

Install the required build tools and kernel headers:

```bash
sudo apt install build-essential linux-headers-$(uname -r)
```

### Install Mosquitto

```bash
sudo apt install mosquitto mosquitto-clients
```

Start the broker if required:

```bash
sudo systemctl start mosquitto
```

### Install MQTT Development Library

```bash
sudo apt install libmosquitto-dev
```

### Install Node-RED

Node-RED can be installed using the official Linux installation method.

After installation, start Node-RED with:

```bash
node-red
```

The Node-RED editor is available at:

```text
http://localhost:1880
```

## 13. Building the Kernel Module

Navigate to the kernel module directory:

```bash
cd ~/virtual_sensor/kernel_module
```

Build the module:

```bash
make
```

A successful build generates:

```text
virtual_sensor.ko
```

To remove generated build files:

```bash
make clean
```

## 14. Loading the Kernel Module

Load the module using:

```bash
sudo insmod virtual_sensor.ko
```

Verify that it is loaded:

```bash
lsmod | grep virtual_sensor
```

Check that the character device exists:

```bash
ls -l /dev/virtual_sensor
```

Kernel messages can be viewed using:

```bash
sudo dmesg | tail
```

## 15. Building the User-Space Application

Navigate to the user application directory:

```bash
cd ~/virtual_sensor/user_app
```

Build the application:

```bash
make
```

This generates the executable:

```text
sensor_app
```

To remove the executable:

```bash
make clean
```

## 16. Running the Complete System

### Step 1: Load the Kernel Module

```bash
cd ~/virtual_sensor/kernel_module
sudo insmod virtual_sensor.ko
```

### Step 2: Verify the Character Device

```bash
ls -l /dev/virtual_sensor
```

### Step 3: Start Mosquitto

```bash
sudo systemctl start mosquitto
```

### Step 4: Start Node-RED

```bash
node-red
```

Open:

```text
http://localhost:1880
```

Deploy the saved flow or import:

```text
dashboard/flows.json
```

### Step 5: Run the User Application

Open another terminal:

```bash
cd ~/virtual_sensor/user_app
sudo ./sensor_app
```

The application connects to the MQTT broker and starts publishing sensor readings.

## 17. Testing

The project was tested in stages:

1. Kernel module compilation.
2. Kernel module loading.
3. Character device creation.
4. Reading virtual sensor data.
5. User-space application communication.
6. MQTT broker connection.
7. MQTT publishing.
8. Node-RED MQTT reception.
9. Sensor data parsing.
10. Dashboard visualization.

Detailed test results are available in:

```text
docs/testing/testing.md
```

## 18. Sample Output

### Character Device

```text
Temperature: 29 C
```

### User-Space Application

```text
Connected to MQTT broker

Sensor Data:
Temperature: 27 C
Humidity: 60 %

Published to virtual_sensor/data
```

### Node-RED

Node-RED receives messages such as:

```text
Temperature: 26 C
Humidity: 41 %
```

### Dashboard

The dashboard displays the received temperature and humidity values using separate gauges.

## 19. Advantages

- Does not require physical sensor hardware.
- Demonstrates Linux kernel module development.
- Demonstrates character-device communication.
- Shows kernel-space to user-space data transfer.
- Uses standard MQTT communication.
- Provides a local IoT prototype using Node-RED.
- Can be tested entirely inside a virtual machine.

## 20. Limitations

- Sensor readings are simulated rather than obtained from physical hardware.
- The MQTT broker is hosted locally.
- The project is intended as a prototype and learning implementation.
- The character device provides a simple sensor interface rather than a production-grade hardware driver.
- No cloud platform is used.

## 21. Future Scope

The project can be extended in the future by:

- Connecting a real temperature and humidity sensor.
- Adding configurable sampling intervals.
- Adding threshold-based alerts.
- Logging sensor readings.
- Connecting the MQTT system to a cloud IoT platform.
- Adding additional virtual or physical sensors.
- Improving device access permissions and configuration.

## 22. Learning Outcomes

This project provides practical experience with:

- Linux kernel modules
- Character devices
- Linux device interfaces
- Kernel-space and user-space communication
- C programming
- MQTT publish/subscribe communication
- Mosquitto MQTT broker
- Node-RED
- IoT data visualization
- Embedded Linux concepts

## 23. Conclusion

The project successfully implements a Linux-based virtual IoT sensor using a loadable kernel module and character device.

The generated sensor data is transferred from the Linux kernel to a user-space C application, published through MQTT, received by Node-RED, and displayed on a dashboard.

The complete pipeline demonstrates how embedded Linux concepts and IoT communication technologies can be combined to create a functional software-based IoT prototype without requiring physical sensor hardware.
