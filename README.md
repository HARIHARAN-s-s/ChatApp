# MCU Communication & Porting Project

## 📦 Project Overview

This project demonstrates a progressive approach to embedded communication using C and Zephyr RTOS. Over two weeks, we implemented a foundational **UDP Chat Application** in Linux (Week 1) and extended communication to **BLE (Bluetooth Low Energy)** using a custom board definition (Week 2). The overall goal is to master protocol stack development, RTOS porting, data structures, and inter-node communication strategies for microcontrollers.

---

## ✅ Week 1: UDP Chat Application in C (Linux)

### 🎯 Objective

Build a **UDP Chat Application** in C that implements:

- OSI Layer mapping to real protocols
- State Machine for application-layer behavior
- Multi-threading using POSIX threads
- Data Structures like Circular Buffer, Queue, and Linked List
- Event-driven message processing and callbacks

This task reinforces embedded communication principles in a **Linux development environment**.

### 🧠 Key Topics Covered

- **OSI Model** mapping to UDP
- **State Machines** and Event Handling
- **POSIX Threads** for concurrent processing
- **Scheduler** vs Superloop comparison
- **C Data Structures**:
  - Queue using Linked List
  - Circular Buffer implementation
- **Pointer Concepts**:
  - Structure pointers
  - Function pointers and callbacks
- **Bitwise Operations**:
  - Byte packing/unpacking
  - LSB/MSB operations

---

## ✅ Week 2: BLE Communication Using BGM220P + Zephyr

### 🎯 Objective

Port Zephyr RTOS for the **Silicon Labs BGM220P** BLE module and establish BLE-based message exchange with the host. This includes:

- Creating a custom board support package (BSP)
- Writing device tree (`.dts`) and pin configuration files
- Fixing `Kconfig` dependencies and toolchain issues
- Building and flashing a Zephyr application

### 🧠 Key Topics Covered

- **Zephyr RTOS Porting**:
  - Custom `board.cmake`, `bgm220p.dts`, `bgm220p_defconfig`, `Kconfig.board`
- **Kconfig Dependency Resolution**:
  - Circular dependency fix for `SOC_SERIES_EFR32BG22`
  - Correcting undefined symbols (e.g., `ARM`, `SOC_FAMILY_SILABS_S2`)
- **Toolchain Troubleshooting**:
  - Proper setup of `ZEPHYR_SDK_INSTALL_DIR`
  - Fixing `target.cmake` toolchain lookup for GCC
- **Devicetree Management**:
  - Including correct `.dtsi` files
  - GPIO and LED configuration
- **Build Process Understanding**:
  - Full CMake and west build flow
  - Handling configuration-time vs build-time errors

---

## 📁 Project Structure

