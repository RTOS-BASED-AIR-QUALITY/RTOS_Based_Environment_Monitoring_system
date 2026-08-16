# Air-Sense — RTOS-Based Environmental Monitoring System

Air-Sense is a real-time environmental monitoring system developed using an **STM32F407** microcontroller and **FreeRTOS**. It collects environmental and air-quality data from **BME280, MQ135, and PMS5003**, processes the readings using embedded firmware, and sends data through an **ESP32 Wi-Fi/MQTT layer** to a server and real-time web dashboard.

## Features

- Real-time environmental monitoring
- Temperature, humidity and pressure sensing using **BME280**
- Air-quality/gas sensing using **MQ135**
- Particulate monitoring using **PMS5003**
- FreeRTOS-based multitasking
- I2C, ADC and UART peripheral interfacing
- STM32-to-ESP32 UART communication
- ESP32 Wi-Fi connectivity
- MQTT-based data transmission
- Python/Flask server-side processing
- Real-time web dashboard
- Air-quality alarm/buzzer support
- Modular sensor-driver architecture

## System Architecture

```text
 BME280 ─────┐
             │ I2C
 MQ135 ──────┤ ADC
             │
 PMS5003 ────┤ UART
             ▼
      ┌─────────────────┐
      │    STM32F407    │
      │    FreeRTOS     │
      │                 │
      │ BME280 Task     │
      │ MQ135 Task      │
      │ PMS5003 Task    │
      │ Communication   │
      └────────┬────────┘
               │ UART
               ▼
      ┌─────────────────┐
      │      ESP32      │
      │ Wi-Fi + MQTT    │
      └────────┬────────┘
               │ Wi-Fi / MQTT
               ▼
      ┌─────────────────┐
      │     Server      │
      │ MQTT Subscriber │
      │ Python + Flask  │
      └────────┬────────┘
               ▼
      ┌─────────────────┐
      │ Web Dashboard   │
      │ Real-Time Data  │
      └─────────────────┘
```

## Hardware

### STM32F407

The STM32F407 is the main embedded controller. It handles:

- Sensor interfacing
- Sensor-data acquisition
- Data processing
- FreeRTOS scheduling
- UART communication
- System integration
- Communication with the ESP32

### BME280

BME280 is used for environmental measurements:

- Temperature
- Humidity
- Atmospheric pressure

The STM32 communicates with the BME280 using **I2C**.

### MQ135

MQ135 is used for air-quality/gas sensing.

The STM32 reads the analog output using its **ADC** and performs sensor calculations involving voltage, sensor resistance, and the Rs/R0 ratio.

> MQ135 is an air-quality/gas-sensing component and should not be treated as a laboratory-grade measurement instrument without appropriate calibration.

### PMS5003

PMS5003 is used for particulate-matter measurements, including:

- PM1.0
- PM2.5
- PM10

The STM32 communicates with PMS5003 through **UART**, including frame synchronization, parsing and checksum validation.

### ESP32

The ESP32 provides the IoT connectivity layer:

- Receives data from STM32
- Connects to Wi-Fi
- Publishes sensor data using MQTT

## Software Architecture

The STM32 firmware uses FreeRTOS to separate sensor acquisition into independent tasks.

```text
FreeRTOS Scheduler
        │
        ├── BME280 Task
        │      └── Temperature / Humidity / Pressure
        │
        ├── MQ135 Task
        │      └── ADC / Air-quality calculation
        │
        ├── PMS5003 Task
        │      └── PM1 / PM2.5 / PM10
        │
        └── Communication Task
               └── STM32 → ESP32
```

The project considers:

- Task priorities
- Task delays
- UART communication
- Sensor timing
- Task stack sizing
- Concurrent sensor operation
- Data sharing and integration

## Communication

| Component | Interface |
|---|---|
| BME280 ↔ STM32 | I2C |
| MQ135 ↔ STM32 | ADC |
| PMS5003 ↔ STM32 | UART |
| STM32 ↔ ESP32 | UART |
| ESP32 ↔ MQTT broker | Wi-Fi / MQTT |
| Server ↔ Dashboard | HTTP/Web communication |

### STM32 → ESP32

The STM32 sends processed sensor information to the ESP32 over UART using structured JSON-style data.

Example:

```json
{
  "Temperature": 25.4,
  "Humidity": 55.2,
  "MQ135": 2100,
  "Ratio": 2.3,
  "Alarm": 0
}
```

The exact fields depend on the firmware/integration version.

### ESP32 → MQTT

The ESP32 publishes received sensor information to an MQTT broker over Wi-Fi.

The server subscribes to the relevant MQTT topic and processes the incoming data.

## Server and Dashboard

The server-side component receives sensor data from MQTT and presents it through a web dashboard.

The dashboard is intended to display:

- Temperature
- Humidity
- Air-quality data
- PM2.5 / particulate data
- Alarm status

The server portion uses Python/Flask and MQTT subscriber functionality.

## Project Structure

```text
.
├── CDAC_Project/
│   ├── Core/
│   │   ├── Inc/
│   │   └── Src/
│   ├── Drivers/
│   └── ...
│
├── Project_Mqtt/
│   └── Project_Mqtt.ino
│
├── server/
│   ├── mqttsub.py
│   └── templates/
│       └── dashboard.html
│
└── README.md
```

The STM32 project was generated/configured using **STM32CubeMX / STM32CubeIDE**, with application and driver modifications implemented in the source/header files.

## Technologies

| Technology | Purpose |
|---|---|
| STM32F407 | Main embedded controller |
| FreeRTOS | Real-time multitasking |
| STM32CubeMX | MCU/peripheral configuration |
| STM32CubeIDE | Firmware development |
| C | STM32 firmware |
| BME280 | Environmental sensing |
| MQ135 | Air-quality/gas sensing |
| PMS5003 | Particulate sensing |
| ESP32 | Wi-Fi / IoT connectivity |
| MQTT | IoT messaging |
| Python | Server-side processing |
| Flask | Web server/dashboard |
| I2C | BME280 communication |
| ADC | MQ135 measurement |
| UART | PMS5003 and ESP32 communication |

## Data Flow

```text
BME280 ─────┐
MQ135 ──────┤
PMS5003 ────┤
            ▼
        STM32F407
            │
         FreeRTOS
            │
      Data Processing
            │
           UART
            ▼
          ESP32
            │
          Wi-Fi
            │
          MQTT
            │
            ▼
          Server
            │
            ▼
      Web Dashboard
```

## Development Challenges

### FreeRTOS Integration

Sensor code that works in a bare-metal application can behave differently after introducing the RTOS scheduler. Task priorities, delays, timing and stack sizes therefore had to be considered.

### Sensor Timing and Communication

The project required careful handling of sensor-specific timing and UART communication, particularly for particulate sensor data.

### MQ135 Interpretation

MQ135 provides an analog response requiring calibration and resistance calculations. Its output should not be interpreted as an exact pollutant concentration without suitable calibration.

### IoT Data Flow

The complete data path spans multiple systems:

```text
STM32 → UART → ESP32 → Wi-Fi → MQTT → Server → Dashboard
```

Maintaining reliable communication across these layers was an important integration challenge.

## Team Contributions

### Member 1 — Dhiraj Chimanshette

Primary responsibilities:

- BME280 driver
- Environmental sensing
- BME280 FreeRTOS integration
- BME280 testing
- Sensor-system integration

### Member 2 — Muhammad Karim Shaikh

Primary responsibilities:

- MQ135 driver
- ADC-based acquisition
- Air-quality calculations
- MQ135 FreeRTOS integration
- Alarm handling

### Member 3 — Rajashri Sanjay Patil

Primary responsibilities:

- PMS5003 driver
- UART reception
- Frame parsing
- Checksum validation
- PMS5003 FreeRTOS integration

### Member 4 — MD Akif Amaan

Primary responsibilities:

- ESP32 communication
- Wi-Fi connectivity
- MQTT integration
- Server-side MQTT subscriber
- Web dashboard
- End-to-end IoT integration

## Build and Run

### STM32

1. Open `CDAC_Project` in STM32CubeIDE.
2. Verify the STM32F407 peripheral configuration.
3. Build the firmware.
4. Flash it to the STM32F407 board.
5. Connect BME280, MQ135, PMS5003 and ESP32 as required.
6. Use a serial terminal to observe firmware output.

### ESP32

Open:

```text
Project_Mqtt/Project_Mqtt.ino
```

Configure the required:

- Wi-Fi credentials
- MQTT broker address
- MQTT port/topic

Then upload the firmware to the ESP32.

### Server

The server code is located in:

```text
server/
```

Install the required Python dependencies, configure the MQTT broker connection, and start the Flask/server application according to the deployment configuration.

## Project Goals

1. Build a practical RTOS-based embedded system.
2. Interface multiple sensors using different protocols.
3. Demonstrate FreeRTOS multitasking.
4. Process sensor data on STM32.
5. Transfer embedded data to an IoT connectivity layer.
6. Use MQTT for network communication.
7. Display environmental information through a web interface.
8. Demonstrate complete embedded-to-IoT integration.

## Future Improvements

- More accurate MQ135 calibration
- Additional air-quality sensors
- Sensor fault detection
- Historical data storage
- Dashboard graphs and analytics
- Cloud deployment
- MQTT TLS/security
- OTA firmware updates
- Advanced alerts and notifications
- Power optimization

## Disclaimer

This project is an educational/engineering prototype demonstrating embedded systems, RTOS, sensor interfacing and IoT communication.

The MQ135 portion should not be treated as a certified air-quality measurement instrument. Accurate pollutant measurement requires appropriate calibration and validation against suitable reference equipment.

## Authors

**Air-Sense — RTOS-Based Environmental Monitoring System**

Developed by:

- **Dhiraj Chimanshette**
- **Muhammad Karim Shaikh**
- **Rajashri Sanjay Patil**
- **MD Akif Amaan**
