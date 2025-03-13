# EmbarcaTech Final Project

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

## Acknowledgments and References

This project was influenced by several example projects from the [BitDogLab-C](https://github.com/BitDogLab/BitDogLab-C), specifically:

- display_oled
- Joystick_led
- neopixel_pio
- button-buzzer
- button_led_rgb

Special thanks to Professor Jivago for his educational content and tutorials available on his YouTube channel: [Jivago](https://www.youtube.com/@profjivago9719). His videos were instrumental in understanding the concepts and implementation of various components in this project.

I would like to express my gratitude to all professors involved in the EmbarcaTech program who dedicated their time to guide us through synchronous classes, providing valuable insights and clarifications that were essential for my learning journey.

The final implementation combines and builds upon these reference projects to create an integrated interactive system.

## Code Structure
The project contains special files in the `src` folder for symbol and pattern generation:

- `drawls.c`: File generated using https://www.piskelapp.com/ containing the definitions of symbols (IF, PI, Heart) displayed on the LED matrix
- `piskel_convert.c`: Responsible for converting and processing patterns into the appropriate LED matrix format
- `main.c`: Main file containing the main loop and system initialization
These files are fundamental for displaying the animations and symbols shown during system initialization.

## Building Project
After cloning the repository, you will need to create a `build` directory to compile the project. The following commands can be useful to set up and build the project:

```bash
cd EmbarcaTech_Final_Project
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build .
```

## Demo Video

Watch a 30-second demonstration of the system's key features in action.

[Demo Video](https://youtu.be/H-Bg4PmH9-0?si=BmzUuu4fzx-mDRKq)

## Main Code
```c	
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// ================= Definitions for GPIO =================
#define LED_BLUE 12
#define LED_RED  13
#define LED_GREEN 11
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22
#define BUZZER_PIN_1 21
#define BUZZER_PIN_2 10
#define BUZZER_FREQUENCY 10000

// ================= Definitions for OLED Display =================
const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;
uint8_t buf[SSD1306_BUF_LEN];
struct render_area frame_area = {
    .start_col = 0,
    .end_col = SSD1306_WIDTH - 1,
    .start_page = 0,
    .end_page = SSD1306_NUM_PAGES - 1
};

// ================= Definitions for NeoPixel =================
#define LED_COUNT 25
#define LED_PIN 7
#define MAX_BRIGHTNESS 255
uint8_t brightnessLevel = MAX_BRIGHTNESS;

struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;
npLED_t leds[LED_COUNT];

PIO np_pio;
uint sm;

// ================= Global Variables =================
uint32_t button_a_count = 0;
uint32_t button_b_count = 0;

// ================= Functions  =================
void pwm_init_buzzer(uint pin);
void beep(uint pin, uint duration_ms);
void initDisplay(void);
void displayMessage(const char *msg);
void displayJoystickInfo(int pos);
void npInit(uint pin);
void setBrightness(uint8_t brightness);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear(void);
void npWrite(void);
int getIndex(int x, int y);
void drawPattern(int pattern[5][5][3]);
void displayPatterns(void);
void npTurnOffAll(void);
void processButtonAction(uint button, uint buzzer_pin, uint led_pin, uint32_t *counter, const char *btnName);
void joystickButtonAction(uint button, uint buzzer_pin_1, uint buzzer_pin_2, uint led_pin, uint32_t *counter_a, uint32_t *counter_b, const char *btnName);

// ================= Main  =================
int main() {
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting system...\n");
  
    // Initialize OLED Display
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
  
    // Initialize NeoPixel Matrix
    npInit(LED_PIN);
    npClear();
    setBrightness(200);
  
    // Initialize LEDs
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, false);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, false);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, false);
  
    // Initialize buttons
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    
    // Initialize joystick / button
    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK, GPIO_IN);
    gpio_pull_up(BUTTON_JOYSTICK);
  
    // Initialize buzzers
    gpio_init(BUZZER_PIN_1);
    gpio_set_dir(BUZZER_PIN_1, GPIO_OUT);
    gpio_init(BUZZER_PIN_2);
    gpio_set_dir(BUZZER_PIN_2, GPIO_OUT);

    // Initialize PWM for buzzers
    pwm_init_buzzer(BUZZER_PIN_1);
    pwm_init_buzzer(BUZZER_PIN_2);
  
    // Initialize I2C for OLED Display
    initDisplay();
    displayMessage("   Starting");
    displayPatterns();
    npTurnOffAll();
  
    // Variables for button state tracking
    bool prev_button_a = false;
    bool prev_button_b = false;
    bool prev_button_joystick = false;
  
    while (true) {
      // Joystick reading
      adc_select_input(0);
      uint adc_y_raw = adc_read();
      adc_select_input(1);
      uint adc_x_raw = adc_read();
      const uint adc_max = (1 << 12) - 1;
      uint col = adc_x_raw * 5 / (adc_max + 1);
      uint row = 4 - (adc_y_raw * 5 / (adc_max + 1));
      npClear();
      int pos = getIndex(col, row);
      displayJoystickInfo(pos);
      npSetLED(pos, 255, 0, 0);
      npWrite();
  
      // Buttons reading
      bool current_button_a = !gpio_get(BUTTON_A);
      bool current_button_b = !gpio_get(BUTTON_B);
  
      if (current_button_a && !prev_button_a) {
          processButtonAction(BUTTON_A, BUZZER_PIN_1, LED_BLUE, &button_a_count, "A");
          sleep_ms(1000);
      }
      if (current_button_b && !prev_button_b) {
          processButtonAction(BUTTON_B, BUZZER_PIN_2, LED_RED, &button_b_count, "B");
          sleep_ms(1000);
      }
  
      // Joystick button reading
      bool current_button_joystick = !gpio_get(BUTTON_JOYSTICK);
      if (current_button_joystick && !prev_button_joystick) {
          joystickButtonAction(BUTTON_JOYSTICK, BUZZER_PIN_1, BUZZER_PIN_2, LED_GREEN, &button_a_count, &button_b_count, "");
          sleep_ms(1000);
      }
      
      prev_button_a = current_button_a;
      prev_button_b = current_button_b;
      prev_button_joystick = current_button_joystick;
    }
  
    return 0;
  }

// ================= Buzzer Functions =================
void pwm_init_buzzer(uint pin) {
    printf("Initializing PWM for buzzer on pin %d\n", pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void beep(uint pin, uint duration_ms) {
    printf("Executing beep on pin %d for %d ms\n", pin, duration_ms);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 2048);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
    sleep_ms(100);
}

// ================= OLED Display Functions  =================
void initDisplay() {
    printf("Initializing OLED display via I2C\n");
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    SSD1306_init();
}

void displayMessage(const char *msg) {
    calc_render_area_buflen(&frame_area);
    memset(buf, 0, SSD1306_BUF_LEN);
    WriteString(buf, 5, 0, (char *)msg);
    render(buf, &frame_area);
    // printf("Display OLED: %s\n", msg);
}

void displayJoystickInfo(int pos) {
    char msg[32];
    snprintf(msg, sizeof(msg),"led position %d",pos);
    displayMessage(msg);
}

// ================= NeoPixel Matrix Functions =================
void npInit(uint pin) {
    printf("Initializing NeoPixel on PIO with pin %d\n", pin);
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

void setBrightness(uint8_t brightness) {
    if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    }
    brightnessLevel = brightness;
    printf("Brightness set to %d\n", brightnessLevel);
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (r * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].G = (g * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].B = (b * brightnessLevel) / MAX_BRIGHTNESS;
}

void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x);
    } else {
        return 24 - (y * 5 + (4 - x));
    }
}

void drawPattern(int pattern[5][5][3]) {
    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            int posicao = getIndex(linha, coluna);
            npSetLED(posicao, pattern[coluna][linha][0], pattern[coluna][linha][1], pattern[coluna][linha][2]);
        }
    }
    npWrite();
    sleep_ms(2000);
    npClear();
}

void displayPatterns() {
    printf("Displaying patterns on LED matrix\n");
    int IF[5][5][3] = {
        {{255, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}}
    };

    int PI[5][5][3] = {
        {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}, {255, 0, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}}
    };

    int Heart[5][5][3] = {
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
        {{255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}},
        {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}
    };

    drawPattern(IF);
    drawPattern(PI);
    drawPattern(Heart);
}

void npTurnOffAll() {
    printf("Turning off all LEDs\n");
    for (uint i = 0; i < LED_COUNT; ++i) {
        npSetLED(i, 0, 0, 0); 
    }
    npWrite();
}

// ================= Buttons Functions =================
void processButtonAction(uint button, uint buzzer_pin, uint led_pin, uint32_t *counter, const char *btnName) {
    (*counter)++;
    printf("Button %s pressed. Count = %u\n", btnName, *counter);
    gpio_put(led_pin, true);
    char msg[32];
    snprintf(msg, sizeof(msg), "   Button %s: %u", btnName, *counter);
    displayMessage(msg);
    beep(buzzer_pin, 1000);
    gpio_put(led_pin, false);
}

void joystickButtonAction(uint button, uint buzzer_pin_1, uint buzzer_pin_2, uint led_pin, 
    uint32_t *counter_a, uint32_t *counter_b, const char *btnName) {
    printf("Joystick pressed. Count A = %u, B = %u\n", *counter_a, *counter_b);
    gpio_put(led_pin, true);
    char msg[32];
    if (btnName[0] != '\0') {
        snprintf(msg, sizeof(msg), "%s: A %u, B %u", btnName, *counter_a, *counter_b);
    } else {
        snprintf(msg, sizeof(msg), "   A: %u, B: %u", *counter_a, *counter_b);
    }
    displayMessage(msg);
    beep(buzzer_pin_1, 1000);
    beep(buzzer_pin_2, 1000);
    gpio_put(led_pin, false);
}
```
