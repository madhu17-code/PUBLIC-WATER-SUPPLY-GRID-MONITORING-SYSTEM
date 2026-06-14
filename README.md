# PUBLIC WATER SUPPLY GRID MONITORING SYSTEM

## Overview

An ESP32-based embedded monitoring and control system designed to supervise critical parameters of a water distribution network in real time. The system acquires data from multiple sensors, detects abnormal conditions such as leakage and unauthorized access, controls actuators through relays, and provides remote monitoring through a web-based dashboard.

---

## Key Features

* Real-time water level monitoring
* Water flow status detection
* Leakage detection using moisture sensors
* Unauthorized access detection using IR sensors
* Relay-based pump and water-line control
* Buzzer-based alert generation
* Wireless monitoring using ESP32 Wi-Fi
* Web dashboard for remote supervision

---

## System Architecture

The system is centered around the ESP32 microcontroller, which interfaces with multiple sensing, control, and communication modules.

### Inputs

* Water Level Sensors
* Flow Sensor
* Soil Moisture Sensors
* IR Sensor

### Processing

* Sensor data acquisition
* Event detection logic
* Leakage monitoring
* Flow monitoring
* Security monitoring

### Outputs

* Relay-controlled water lines
* Water pump control
* Buzzer alerts
* Web dashboard updates

---

## Hardware Components

| Component             | Purpose                                    |
| --------------------- | ------------------------------------------ |
| ESP32                 | Central controller and Wi-Fi communication |
| Water Level Sensors   | Tank level monitoring                      |
| Flow Sensor           | Water flow detection                       |
| Soil Moisture Sensors | Leakage detection                          |
| IR Sensor             | Unauthorized access detection              |
| Relay Module          | Pump and line control                      |
| Buzzer                | Alert indication                           |
| Water Pump            | Water transfer mechanism                   |

---

## Embedded Software Functionality

The ESP32 firmware continuously reads sensor inputs and executes monitoring logic to:

* Detect water leakage conditions
* Monitor tank water levels
* Track water flow status
* Detect unauthorized access events
* Control relays based on system conditions
* Update monitoring data on the web interface

The system follows an event-driven approach where sensor inputs trigger control actions and status updates.

---

## Project Outcomes

* Reduced dependence on manual monitoring
* Enabled real-time anomaly detection
* Improved visibility of system status through remote monitoring
* Demonstrated integration of sensing, communication, and control modules within an embedded system

---

## Repository Structure

```text
PUBLIC-WATER-SUPPLY-GRID-MONITORING-SYSTEM
│
├── README.md
├── firmware
│   └── source_code.ino
└── project_documentation
```

---

## Future Enhancements

* GPS-based fault localization
* Cloud data logging
* Predictive maintenance using machine learning
* Mobile application integration




