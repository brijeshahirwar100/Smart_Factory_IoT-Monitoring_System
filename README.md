# Smart Factory IoT Monitoring System

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Arduino%20Uno-00979D?logo=arduino&logoColor=white" />
  <img src="https://img.shields.io/badge/Simulation-PICSimLab-C0392B?logoColor=white" />
  <img src="https://img.shields.io/badge/Protocol-MQTT-FF6600?logoColor=white" />
  <img src="https://img.shields.io/badge/Cloud-ThingsBoard-1B8A56?logoColor=white" />
  <img src="https://img.shields.io/badge/Network-W5100%20Ethernet-6A0DAD?logoColor=white" />
  <img src="https://img.shields.io/badge/Firmware-C%2FC%2B%2B-00599C?logoColor=white" />
  <img src="https://img.shields.io/badge/License-MIT-yellow" />
</p>

<p align="center">
  A dual-node embedded IoT system for real-time industrial monitoring, automation, and remote control — simulated end-to-end using PICSimLab and ThingsBoard Cloud.
</p>

---

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Node Descriptions](#node-descriptions)
- [Hardware Components](#hardware-components)
- [Firmware Architecture](#firmware-architecture)
- [Dashboard & Visualization](#dashboard--visualization)
- [Setup & Installation](#setup--installation)
- [Simulation Results](#simulation-results)
- [Screenshots](#screenshots)
- [Project Structure](#project-structure)
- [Author](#author)

---

## Overview

The **Smart Factory IoT Monitoring System** simulates a real-world industrial IoT deployment across two functional zones — a **Production Line** and a **Warehouse** — using Arduino-based embedded nodes communicating over MQTT via Ethernet.

Key capabilities:
- **Real-time telemetry** published every 5 seconds to ThingsBoard Cloud
- **Automated control logic** (e.g., ventilation triggered by humidity threshold)
- **Remote actuation** via ThingsBoard RPC commands
- **Fault-tolerant networking** with MQTT auto-reconnect
- **Alarm generation** for out-of-range sensor conditions

The entire system — from sensor simulation to cloud dashboard — runs without physical hardware, using PICSimLab for embedded simulation and ThingsBoard for IoT visualization.

---

## System Architecture

```
┌─────────────────────────────────────────────┐
│              PICSimLab (Simulation)          │
│                                             │
│  ┌─────────────────┐  ┌──────────────────┐  │
│  │  Node 1         │  │  Node 2          │  │
│  │  Production Line│  │  Warehouse       │  │
│  │  (Arduino Uno)  │  │  (Arduino Uno)   │  │
│  └────────┬────────┘  └────────┬─────────┘  │
│           │  W5500 Ethernet    │            │
└───────────┼────────────────────┼────────────┘
            │                    │
            ▼                    ▼
     ┌──────────────────────────────┐
     │       MQTT Broker            │
     │   (ThingsBoard Cloud)        │
     └──────────────────────────────┘
                    │
                    ▼
     ┌──────────────────────────────┐
     │   ThingsBoard Dashboard      │
     │   Telemetry · Alarms · RPC   │
     └──────────────────────────────┘
```

---

## Node Descriptions

### Node 1 — Production Line

Monitors the factory floor with a focus on machine health and environmental safety.

| Sensor / Component | Purpose |
|--------------------|---------|
| DHT22 | Ambient temperature & humidity |
| LM35 | Machine surface temperature |
| Vibration sensor | Machine health monitoring |
| Push button | Manual machine ON/OFF |
| Relay | Remote-controlled actuator (RPC) |

**Automation Logic:**
- Temperature alarm triggered when surface temperature exceeds **35°C**
- Relay state controllable remotely via ThingsBoard RPC

---

### Node 2 — Warehouse

Monitors storage conditions with emphasis on environment, security, and access.

| Sensor / Component | Purpose |
|--------------------|---------|
| DHT22 | Ambient temperature & humidity |
| LDR | Ambient light level |
| PIR sensor | Motion / occupancy detection |
| Reed switch | Door open/close status |
| Relay (ventilation) | Auto-controlled fan/vent |

**Automation Logic:**
- Ventilation relay activates automatically when humidity ≥ **85%**
- Door events and motion events are logged and visualized on the dashboard

---

## Hardware Components

| Component | Model | Quantity |
|-----------|-------|----------|
| Microcontroller | Arduino Uno (simulated) | 2 |
| Temp & Humidity | DHT22 | 2 |
| Surface Temperature | LM35 | 1 |
| Light Sensor | LDR | 1 |
| Motion Detector | PIR Sensor | 1 |
| Door Sensor | Reed Switch | 1 |
| Ethernet Module | W5500 | 2 |
| Actuator | Relay Module | 2 |

---

## Firmware Architecture

The firmware follows a **modular embedded architecture**, separating concerns into dedicated manager modules per node.

```
firmware/
│
├── node1_production/
│   ├── node1.ino               ← Main entry point
│   ├── config.h                ← MQTT credentials, thresholds, pin map
│   ├── sensor_manager.h/.cpp   ← DHT22, LM35, vibration reads
│   ├── actuator_manager.h/.cpp ← Relay control logic
│   ├── network_manager.h/.cpp  ← Ethernet init, MQTT connect/reconnect
│   ├── telemetry_manager.h/.cpp← JSON payload construction & publish
│   └── rpc_handler.h/.cpp      ← ThingsBoard RPC callback handler
│
└── node2_warehouse/
    ├── node2.ino
    ├── config.h
    ├── sensor_manager.h/.cpp
    ├── actuator_manager.h/.cpp
    ├── network_manager.h/.cpp
    ├── telemetry_manager.h/.cpp
    ├── rpc_handler.h/.cpp
    └── debug.h/.cpp             ← Serial logging utility
```

### Module Responsibilities

| Module | Responsibility |
|--------|---------------|
| `sensor_manager` | Reads all sensors; handles read errors gracefully |
| `actuator_manager` | Controls relay states; enforces automation rules |
| `network_manager` | Initializes W5500; manages MQTT connect/reconnect loop |
| `telemetry_manager` | Serializes sensor data to JSON; publishes to ThingsBoard |
| `rpc_handler` | Parses incoming RPC commands; dispatches to actuators |
| `debug` | Serial logging for simulation diagnostics (Node 2) |

---

## Dashboard & Visualization

The ThingsBoard Cloud dashboard provides:

- **Time-series charts** — temperature and humidity trends per node
- **Gauge widgets** — real-time sensor readings
- **LED indicators** — relay state, door status, motion events
- **Alarm panel** — active alerts with timestamps
- **RPC control buttons** — toggle relays from the dashboard UI
- **Historical data** — logged telemetry for post-analysis

> Dashboard JSON export is available in `dashboard/` for direct import into your ThingsBoard instance.

---

## Setup & Installation

### Prerequisites

- [PICSimLab](https://sourceforge.net/projects/picsimlab/) installed
- [Arduino IDE](https://www.arduino.cc/en/software) (for firmware compilation)
- [ThingsBoard Cloud](https://thingsboard.io/) account (free tier is sufficient)

### 1. Clone the Repository

```bash
git clone https://github.com/brijeshahirwar100/Smart-Factory-IoT-Monitoring-System.git
cd Smart-Factory-IoT-Monitoring-System
```

### 2. Configure Credentials

Open `firmware/node1_production/config.h` and `firmware/node2_warehouse/config.h`:

```cpp
// ThingsBoard device access tokens
#define TB_ACCESS_TOKEN   "YOUR_DEVICE_TOKEN_HERE"

// MQTT broker
#define MQTT_SERVER       "mqtt.thingsboard.cloud"
#define MQTT_PORT         1883

// Static IP for W5500
byte ip[] = { 192, 168, 1, 177 };
```

### 3. Load Firmware in PICSimLab

1. Open PICSimLab
2. Load the `.hex` file for Node 1 (`node1_production/`)
3. Repeat for Node 2 (`node2_warehouse/`)
4. Start simulation — both nodes will connect to MQTT and begin publishing

### 4. Import the ThingsBoard Dashboard

1. Login to [ThingsBoard Cloud](https://thingsboard.cloud)
2. Go to **Dashboards → Import**
3. Upload `dashboard/smart_factory_dashboard.json`
4. Assign your device tokens to the widgets

---

## Simulation Results

| Test Case | Expected Behavior | Result |
|-----------|-------------------|--------|
| Normal operation | Telemetry published every 5s | ✅ Confirmed |
| Humidity ≥ 85% | Auto ventilation relay ON | ✅ Confirmed |
| Temperature > 35°C | Alarm triggered in ThingsBoard | ✅ Confirmed |
| MQTT disconnect | Auto-reconnect on next loop cycle | ✅ Confirmed |
| RPC command received | Relay state toggled correctly | ✅ Confirmed |
| Sensor read error | Error flag published; no crash | ✅ Confirmed |

---

## Screenshots

## 📸 Screenshots

<h3 align="center">PICSimLab Simulation</h3>
<p align="center">
  <img src="Smart Factory IoT Monitoring System/Media/Images/Simulation_PICSIMLAB.png" width="800"/>
</p>

<h3 align="center">Smart Factory Overview Dashboard</h3>
<p align="center">
  <img src="Smart Factory IoT Monitoring System/Media/Images/Smart Factory Dashboard.png" width="800"/>
</p>

<h3 align="center">Node 1 — Production Line Dashboard</h3>
<p align="center">
  <img src="Smart Factory IoT Monitoring System/Media/Images/Production Line Node1 Dashboard.png" width="800"/>
</p>

<h3 align="center">Node 2 — Warehouse Dashboard</h3>
<p align="center">
  <img src="Smart Factory IoT Monitoring System/Media/Images/Warehouse Monitor _Dashboard.png" width="800"/>
</p>

## Project Structure

```
Smart-Factory-IoT-Monitoring-System/
│
├── firmware/
│   ├── node1_production/       ← Node 1 source code
│   └── node2_warehouse/        ← Node 2 source code
│
├── simulation/                 ← PICSimLab workspace files
│
├── dashboard/
│   └── smart_factory_dashboard.json  ← ThingsBoard import file
│
├── docs/                       ← Project report, circuit diagrams
│
└── media/
    └── images/                 ← Screenshots and demo images
```

---

## Libraries Required

Install via Arduino Library Manager:

| Library | Purpose |
|---------|---------|
| `DHT sensor library` (Adafruit) | DHT22 temperature & humidity |
| `PubSubClient` (Nick O'Leary) | MQTT client |
| `Ethernet` (Arduino) | W5500 Ethernet driver |
| `ArduinoJson` | JSON serialization for telemetry |

---

## Author

**Brijesh Ahirwar**  
B.Tech, Electronics & Communication  
Indian Institute of Technology (BHU), Varanasi

---

## License

This project is licensed under the [MIT License](LICENSE).

---

*Developed as part of the Internship Project at Emertxe.*
