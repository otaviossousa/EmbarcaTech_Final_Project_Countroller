# Project EmbarcaTech - Raspberry Pi Pico W Interactive System

## Project Context
This project represents the final assessment of the EmbarcaTech program's Unit 7. The program covered fundamental concepts of Embedded Systems, C programming for embedded systems, microcontroller architecture and programming, and Internet of Things (IoT). This interactive system demonstrates the practical application of these concepts in a functional embedded system.

The project showcases:
- Embedded Systems fundamentals through hardware integration
- C programming implementation
- Microcontroller programming using Raspberry Pi Pico W
- Interactive IoT-ready system architecture


## Overview
This project implements an interactive system using a Raspberry Pi Pico microcontroller, featuring LED matrix control, joystick input, button interactions, OLED display output, and buzzer feedback.

## Hardware Components
- Raspberry Pi Pico W
- 5x5 NeoPixel LED Matrix (25 LEDs)
- SSD1306 OLED Display (128x64)
- 2 Buzzers
- 2 Push Buttons (A and B)
- Analog Joystick with button
- 3 Status LEDs (Red, Green, Blue)

## Pin Configuration

| Components       | Raspberry Pi Pico w Pins  |
|------------------|---------------------------|
| LED NeoPixel     | GPIO 7                    |
| Display OLED SDA | GPIO 14                   |
| Display OLED SCL | GPIO 15                   |
| Joystick (Eixo X)| GPIO 26 (ADC0)            |
| Joystick (Eixo Y)| GPIO 27 (ADC1)            |
| Botão A          | GPIO 5                    |
| Botão B          | GPIO 6                    |
| Botão Joystick   | GPIO 22                   |
| Buzzer 1         | GPIO 21                   |
| Buzzer 2         | GPIO 10                   |
| LED Azul         | GPIO 12                   |
| LED Vermelho     | GPIO 13                   |
| LED Verde        | GPIO 11                   |

## Features
1. Interactive LED Matrix Control
   - Joystick-controlled LED position
   - Pattern display (IF, PI, Heart)

2. Button System
   - Counter tracking for buttons A and B
   - LED and Buzzers feedback for button presses

3. Display Output
   - OLED status display
   - Button press counter display
   - Joystick position feedback

4. Sound Feedback
   - Dual buzzer system
   - PWM-controlled sound generation

## Functions
- LED Matrix Control: `npInit()`, `npSetLED()`, `drawPattern()`
- Display: `initDisplay()`, `displayMessage()`
- Button Handling: `processButtonAction()`, `joystickButtonAction()`
- Sound: `pwm_init_buzzer()`, `beep()`

## Setup and Usage
1. Initialize system components
2. Display startup patterns
3. Enter main loop:
   - Read joystick position
   - Update LED matrix
   - Process button inputs
   - Update display
   - Provide audio feedback

## Dependencies
- Raspberry Pi Pico SDK
- SSD1306 OLED Library
- WS2812B (NeoPixel) PIO Implementation
