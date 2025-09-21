# 🔥 Fire Detection System

## 📌 Overview
This project implements an **IoT-based Fire Detection System** that combines **sensor data** and **computer vision** to improve fire detection accuracy.  
It uses **gas and temperature sensors** as the first line of detection. If potential fire conditions are detected, a **camera is activated**, and a **Convolutional Neural Network (CNN)** is used to confirm the presence of fire.  
Upon confirmation, the system sends **real-time notifications** to the user through **Blynk Cloud**.

This approach reduces false alarms by integrating **IoT + Machine Learning + Computer Vision**.

---

## 🚀 Features
- **Multi-sensor input**: Gas + Temperature readings  
- **Camera activation** only when sensors detect anomalies (power-efficient)  
- **CNN model** for accurate fire image classification  
- **IoT integration** with **ESP32 / Raspberry Pi** for real-time processing  
- **Cloud connectivity** via **Blynk** for instant alerts to users  
- **Modular design** (sensors, ML model, and cloud platform can be updated independently)  

---

## 🛠️ Technologies Used
- **Hardware**: ESP32, Raspberry Pi, Temperature & Gas sensors, Camera Module  
- **Programming**: Python, Arduino IDE  
- **Machine Learning**: PyTorch (CNN for fire detection)  
- **Cloud & IoT**: Blynk Cloud, MQTT (optional)  
- **Notebooks**: Jupyter Notebook for training and testing models  

---

## 📂 Repository Structure
Fire-Detection-System/

│── notebooks/ # Model training & evaluation

│── esp32/ # ESP32 code for sensors & cloud integration

│── raspberrypi/ # Raspberry Pi scripts for camera + ML inference

│── models/ # Saved ML models

│── data/ # Sample dataset (or link to dataset)

│── requirements.txt # Python dependencies

│── README.md # Project documentation

---


## ⚙️ Getting Started

### Prerequisites
- Python 3.x  
- Jupyter Notebook  
- ESP32 and Raspberry Pi setup  
- Required Python libraries (listed in `requirements.txt`)  

### Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/BasantSaad/Fire-Detection-System.git
   cd Fire-Detection-System
2. Install dependencies:
  pip install -r requirements.txt
3. Open the Jupyter Notebook to train/test the fire detection CNN.
4. Upload ESP32/RPi scripts to your board and configure your Wi-Fi + Blynk credentials.
---
## 📊 Demo Workflow

1. Sensors detect abnormal gas/temperature levels
2. Camera module activates
3. CNN model classifies whether fire is present
4. If confirmed → Notification sent via Blynk Cloud
