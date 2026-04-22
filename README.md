# 🔐 Multi-Level Security Based Access Control System (LPC2129)
<img width="347" height="169" alt="tittle" src="https://github.com/user-attachments/assets/32cefd52-a926-4012-8a8c-7c562fde10de" />

## 📌 Project Overview
This project is an embedded system based on the LPC2129 microcontroller that implements a secure access control mechanism using **password authentication + OTP verification**. It is designed for applications like smart lockers, restricted rooms, and security systems.

---

## ⚙️ Working Principle
The system works in two security levels:

### 1️⃣ Password Authentication
- User enters an 8-digit password using a 4x4 keypad.
- <img width="308" height="162" alt="password enter" src="https://github.com/user-attachments/assets/aa18f2c7-c0e3-424e-b82a-645384e66e4d" />
- The password is stored in EEPROM.
- If the password matches, the system moves to OTP verification.



### 2️⃣ OTP Verification
- A 4-digit OTP is generated using a timer value.
- OTP is sent to the user via GSM module.
- <img width="413" height="222" alt="virtualterminalotp" src="https://github.com/user-attachments/assets/cb1c4e9c-7408-48b8-95e3-85b14ddd3ac6" />

- User enters OTP using keypad.
- If OTP matches → access is granted.
<img width="307" height="156" alt="lcd_otp" src="https://github.com/user-attachments/assets/d32c25f6-f278-47ed-b3f6-a2a032c31a64" />

---

## 🔓 Access Control
- ✔ Correct password + correct OTP → Locker opens (DC motor ON)
- <img width="299" height="143" alt="lockeropen" src="https://github.com/user-attachments/assets/b88fda0a-5f18-4ca3-901c-ca3cda4bb6e8" />

- ❌ Wrong password → Access denied
- ❌ Wrong OTP (3 attempts allowed) → Access blocked + SMS alert sent
<img width="407" height="241" alt="invalid user" src="https://github.com/user-attachments/assets/3668420f-d7ae-42b5-9e60-2507d85f4fee" />

---

## 🧰 Hardware Used
- LPC21XX Microcontroller
- 4x4 Keypad
- LCD Display
- GSM Module
- EEPROM (I2C based)
- DC Motor
- LED Indicators

---

## 💻 Software / Drivers Used
- UART Communication (GSM + Debugging)
- I2C Protocol (EEPROM)
- LCD Interface (4-bit mode)
- Timer (OTP generation)
- GPIO (Keypad + Motor control)

---

## 🔄 System Flow
1. System starts and initializes all peripherals
2. User enters password
3. Password is checked from EEPROM
4. If correct → OTP is generated
5. OTP is sent via GSM
6. User enters OTP
7. If OTP matches → Access granted
8. Else → Retry or Access denied

---

## 📲 GSM Features
- Sends OTP to registered mobile number
- Sends alert message on unauthorized access

---

## 🚀 Features
- Dual security authentication
- Real-time OTP verification
- EEPROM-based password storage
- GSM-based alert system
- Motor-controlled locking system

---

## 📌 Applications
- Smart lockers
- Door security systems
- Bank vault security
- Office access control
---

## Schematic capture
<img width="1297" height="935" alt="schematic_view" src="https://github.com/user-attachments/assets/9cd07304-261e-4b44-a2cd-9e7d2f8bbae0" />
