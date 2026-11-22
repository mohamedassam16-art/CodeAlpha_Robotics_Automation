# Task 3: Automation System Design — Automated Conveyor Belt Sorting System

**Intern:** Mohamed Essam Refaat  
**Intern ID:** CA/DF1/440462  
**Domain:** Robotics & Automation  

## Overview
An industrial automation system design for a multi-sensor conveyor belt sorting line controlled by an **ATmega32** microcontroller. The system detects incoming packages, inspects product dimensions and material properties in real time, and routes items into designated acceptance or rejection bins via a PWM-actuated diverter gate.

## Hardware & Control Highlights
* **Microcontroller:** ATmega32 @ 16 MHz (AVR RISC architecture).
* **Sensors:** Infrared (IR) entry detection sensor, HC-SR04 ultrasonic height/clearance sensor, inductive/exit confirmation sensor, and hardware Emergency Stop (E-Stop).
* **Actuators & HMI:** 12V DC conveyor motor driven by L298N (PWM speed control), servo-actuated diverter gate (Timer1 Fast PWM), 16x2 alphanumeric LCD batch counter, and status indicator tower.

## Repository Contents
* `CodeAlpha_Task3_Automation_Report.pdf` — 3-page technical explanation report detailing hardware architecture, operational logic, and state truth tables.
* `CodeAlpha_Task3_Design_Diagram.pdf` & `CodeAlpha_Task3_Conveyor_System_Design.png` — High-resolution circuit block diagram and control logic flowchart.
* `conveyor_sorting_controller.c` — Bare-metal Embedded C firmware for the ATmega32 controller.
