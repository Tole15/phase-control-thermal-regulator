# Phase-Synchronized Thermal Regulation System

## AC Phase-Control via Zero-Cross Detection & Discrete PI Control

This repository implements a closed-loop thermal regulation system using phase-angle control for AC resistive loads. The system synchronizes TRIAC firing with mains zero-cross events and regulates temperature through a discrete-time Proportional-Integral (PI) controller.

**Validated on Hardware:** 127V / 60Hz.

---

## 1. System Overview

The system monitors temperature via an **LM35 sensor** (signal-conditioned to a 0–5V range). The signal is processed by a discrete PI controller, which determines the necessary power delivery. Power is modulated using a TRIAC on a 75W resistive load.

* **Control Loop Frequency:** 2 Hz (Updates every 0.5s).
* **Synchronization Frequency:** 120 Hz (Synchronized to every AC half-cycle).

---

## 2. Hardware Architecture

### Control Unit

* **Microcontroller:** Arduino (ATmega328P).
* **External Interrupt (INT0):** Dedicated to high-speed Zero-Cross detection.
* **Timer1:** Utilized for high-precision firing delay (via `PWMUNO` or custom registers).

### Power Stage

* **Detection:** Opto-isolated zero-cross sensing for galvanic isolation.
* **Actuation:** MOC-series opto-triac driver + Power TRIAC.
* **Load:** 75W AC resistive load (Incandescent/Heating element).

### Sensor & Conditioning

* **Sensor:** LM35 Linear Temperature Sensor.
* **Conditioning:** Active filtering and amplification via Operational Amplifiers (Op-Amps) to utilize the full 10-bit ADC range (0–5V).

---

## 3. Control Strategy

### Timing & Sampling

* **Zero-Cross Events:** Detected every 8.33ms (120 Hz).
* **Control Update:** Every 60 zero-cross events.
* **Effective Sampling Time ($T_s$):** $0.5\text{ s}$.

### Discrete PI Controller

The controller uses the Tustin (Bilinear) or Backward Euler approximation. The recursive form implemented is:

$$u(k) = u(k-1) + q_0 e(k) + q_1 e(k-1)$$

Where the constants for a Trapezoidal integration are:

* $q_0 = K_p + \frac{K_i T_s}{2}$
* $q_1 = \frac{K_i T_s}{2} - K_p$

**Variables:**

* $e(k)$: Current error (Setpoint - Measured).
* $u(k)$: Control effort (Mapped to TRIAC conduction angle).

### Power Modulation

The control effort $u(k)$ is converted into a timer compare value (**CMP**). The TRIAC is triggered after a delay calculated as:
$$t_{delay} = t_{half\_cycle} - t_{conduction}$$
The output is saturated between $0$ and $ICR1$ to prevent timing overflows.

---

## 4. Engineering Specifications

| Parameter | Value |
| :--- | :--- |
| **Mains Voltage** | 127V AC |
| **Mains Frequency** | 60 Hz |
| **Half-Cycle Period** | 8.333 ms |
| **Resolution** | 16-bit (Timer1) |
| **Zero-Cross Interrupt** | Rising Edge (INT0) |

---

## 5. Repository Structure

```text
├── firmware/
│   └── arduino/
│       └── Control.ino        # PI Control + Zero-Cross ISR logic
├── docs/
│   ├── DIMMER.pdf      # Detailed experimental report
│   └── AcondicionamientoLM35.pdf       # System block diagram
└── hardware/
    └── cadcam/            # Gerber files / PCB Design
