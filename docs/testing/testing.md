# Testing and Validation

The Virtual Sensor Module was tested in stages to verify the complete data flow from the Linux kernel module to the Node-RED dashboard.

## Test Cases

| Test Case | Expected Result | Status |
|---|---|---|
| Kernel module compilation | Kernel module compiles successfully and generates `virtual_sensor.ko` | PASS |
| Kernel module loading | `virtual_sensor` appears in the loaded kernel modules | PASS |
| Character device creation | `/dev/virtual_sensor` is created successfully | PASS |
| Sensor data generation | Virtual temperature data is returned through the character device | PASS |
| User-space communication | C application successfully reads data from `/dev/virtual_sensor` | PASS |
| MQTT connection | User-space application connects to the local Mosquitto broker | PASS |
| MQTT publishing | Sensor readings are published to `virtual_sensor/data` | PASS |
| Node-RED reception | Node-RED receives the MQTT sensor messages | PASS |
| Data parsing | Node-RED separates temperature and humidity values | PASS |
| Dashboard visualization | Temperature and humidity are displayed using dashboard gauges | PASS |

## Functional Verification

### 1. Kernel Module

The kernel module was compiled using the Linux kernel build system. The resulting `virtual_sensor.ko` module was successfully generated.

### 2. Character Device

After loading the module, the character device was available at:

```text
/dev/virtual_sensor
```

Reading from this device returned the generated virtual sensor data.

### 3. User-Space Application

The user-space C application successfully opened the character device and read temperature and humidity values from the kernel module.

### 4. MQTT Communication

The application connected to the local Mosquitto MQTT broker and published sensor readings to:

```text
virtual_sensor/data
```

Multiple sensor readings were successfully transmitted.

### 5. Node-RED

Node-RED successfully received the MQTT messages. The incoming sensor string was parsed into separate temperature and humidity values.

### 6. Dashboard

The processed values were displayed on the Node-RED Dashboard using separate gauges for:

- Temperature (°C)
- Humidity (%)

## Result

All major functional components of the system were successfully tested. The complete data path was verified:

```text
Virtual Sensor
      ↓
Linux Kernel Module
      ↓
/dev/virtual_sensor
      ↓
User-Space C Application
      ↓
MQTT
      ↓
Node-RED
      ↓
Dashboard
```

The testing confirms that the implemented virtual IoT sensor pipeline operates successfully in the Ubuntu virtual machine environment.
