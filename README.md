# SmartHome System with Fire and Smoke Alarm, Automatic Lighting, Door Control, and LCD Display

## 📌 Overview
This project is a **Smart Home System** that integrates multiple sensors and actuators to enhance home safety and automation. It includes fire and smoke detection, automatic lighting, door control, and an LCD display for real-time monitoring. The system is connected to the **ERa IoT** platform for remote monitoring and control.

## 🎯 Features
- 🔥 **Fire and Smoke Alarm**: Detects fire and smoke using **MQ-2** gas sensors and **Flame Sensors**.
- 💡 **Automatic Lighting**: Controls lighting based on environmental conditions.
- 🚪 **Door Control**: Uses a **servo motor** to automate door movement.
- 📟 **LCD Display**: Shows real-time sensor data using an **I2C LCD screen**.
- 📲 **Remote Monitoring and Control**: Integrated with the **ERa IoT** app for remote access.

## 🛠 Hardware Components
- **ESP32** (Microcontroller)
- **DHT11** (Temperature & Humidity Sensor)
- **MQ-2** (Gas Sensor)
- **Flame Sensors**
- **Servo Motor** (for door control)
- **Relays** (for controlling electrical appliances)
- **LCD 16x2 I2C Display**
- **Optocoupler & MOSFETs** (for isolated switching)
- **Power Supply & Jumper Wires**

## 🏗 Software Requirements
- **Arduino IDE** (with ESP32 board support)
- **ERa IoT** Library
- **ESP32Servo Library**
- **DHT Sensor Library**
- **Wire Library** (for I2C communication)

## 🚀 Installation & Setup
1. **Clone the repository:**
   ```bash
   git clone https://github.com/ndhanh2711/Smart_Home_Ver1.1.git
   ```
2. **Install required libraries** in Arduino IDE:
   - Go to **Library Manager** and install:
     - `ERa IoT`
     - `DHT Sensor`
     - `ESP32Servo`
     - `Wire`
3. **Configure WiFi Credentials** in `config.h`:
   ```cpp
   const char ssid[] = "YOUR_WIFI_SSID";
   const char pass[] = "YOUR_WIFI_PASSWORD";
   ```
4. **Upload the code** to ESP32 using Arduino IDE.
5. **Connect to ERa IoT App** and monitor the system.

## 📊 System Workflow
1. **Fire/Smoke Detection:**
   - If smoke or fire is detected, the system triggers an alarm and sends a notification to the ERa IoT app.
2. **Automatic Lighting:**
   - Lights turn on/off based on sensor readings and user preferences.
3. **Door Automation:**
   - The servo motor moves the door when triggered via the app or a physical sensor.
4. **LCD Display:**
   - Shows temperature, humidity, and sensor status in real-time.

## 🔧 Troubleshooting
- **ESP32 not connecting to WiFi?** Check SSID and Password in `config.h`.
- **Sensor values not updating?** Ensure correct wiring and library installation.
- **App not receiving updates?** Verify ERa IoT setup and internet connection.

## 📌 Future Enhancements
- 🔑 **Face Recognition for Door Access**
- 📷 **Camera Integration for Surveillance**
- 🔋 **Battery Backup for Power Outages**
- 🌐 **Integration with Home Assistant**

## 🤝 Contributing
Feel free to **fork** this repo, **contribute** new features, and submit **pull requests**! 🚀

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📬 Contact
For queries and collaboration, reach out to:
📧 **nguyenhanh2711abc@gmail.com**
🌐 **GitHub:[https://github.com/ndhanh2711]**
