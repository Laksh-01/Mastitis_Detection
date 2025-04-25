# 🐄 Mastitis Detection Using IoT and Edge ML

## 📌 Project Overview

Mastitis is a prevalent disease among dairy cattle that causes inflammation of the udder, leading to reduced milk yield and financial loss. Our project introduces a **smart IoT-based solution** to detect mastitis in its early stages using **real-time sensor data** and a **machine learning model** deployed on an **ESP32 microcontroller**.

---

## 🛠️ System Architecture

### 🔧 Sensor Selection

- **DHT22** – Measures temperature and humidity to detect inflammation or infection conditions.
- **Flex Sensor** – Detects abnormal udder swelling or changes in pressure.
- **Bend Sensor** – Monitors physical deformation, signaling unusual behavior or posture.

All sensors are connected to the **ESP32**, chosen for its Wi-Fi capabilities and support for running Edge ML models.

### 🤖 Edge Machine Learning

- Trained a lightweight model using **Edge Impulse** on real sensor data.
- Exported as **C++ code** and integrated into an **`.ino` Arduino sketch** on ESP32.
- Real-time inference is performed **on-device**, reducing latency and dependency on the cloud.

### 💻 Simulation Environment

- Complete setup was **simulated on Wokwi**, enabling accurate testing of both circuitry and embedded logic before physical deployment.

### 🔗 Data Flow & Communication

Sensor readings and inference results are transmitted using the **MQTT protocol** to:

- 🌐 A **custom-built website** for real-time health status and alerts.
- 📊 **ThingSpeak** for IoT-based live data plotting and monitoring.
- 📈 **Power BI** for advanced data analytics, historical tracking, and trend visualization.

### 📺 Visualization & Decision Support

- **Power BI Dashboards** for insights into long-term cattle health data.
- **ThingSpeak Charts** for real-time visualization of temperature, pressure, and ML predictions.
- **Website Dashboard** alerts farmers immediately upon detecting abnormal signs.

---

## ⚙️ Components Used

| Component        | Purpose                                      |
|------------------|----------------------------------------------|
| **ESP32**        | Core microcontroller with Wi-Fi & ML support |
| **DHT22 Sensor** | Temperature & humidity monitoring            |
| **Flex Sensor**  | Detects udder expansion or swelling          |
| **Bend Sensor**  | Monitors posture changes or deformation      |
| **MQTT Protocol**| Sends real-time data to cloud and dashboards |
| **Wokwi**        | Circuit simulation & virtual prototyping     |
| **Edge Impulse** | Model training and edge deployment           |
| **ThingSpeak**   | Real-time IoT dashboard and graphing         |
| **Power BI**     | Historical data visualization & analytics    |

---

## 🌐 Live Demo

🚀 **Deployed Website:** [Mastitis Detection Dashboard](https://dainty-sopapillas-5863ac.netlify.app/)

Check out the real-time cattle health monitoring dashboard where sensor data and ML predictions are visualized and alerts are triggered instantly.

---

## 💡 Key Highlights

- ✅ **End-to-end real-time mastitis monitoring system**
- 📶 **Low-power edge ML** deployed on ESP32
- 📊 **Multiple visualization platforms** integrated (Website, Power BI, ThingSpeak)
- 🌍 **Scalable, affordable**, and practical for small and large dairy farms

---

## 📄 Reference Paper

🔗 [Detection of Mastitis Disease in Dairy Cows Using IoT and Machine Learning](https://www.mdpi.com/2624-7402/3/3/37) – MDPI Electronics, 2021

---

## 📬 Contact / Contributors

**Team Name:** [Your Team Name]  
**Developed By:** [Your Names / College Info]  
**Mentor:** [Guide Name if applicable]  
**College:** [Your Institution Name]  
