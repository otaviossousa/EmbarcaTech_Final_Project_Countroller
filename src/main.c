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

// ================= Definições de Pinagem =================
#define LED_BLUE 12
#define LED_RED  13
#define LED_GREEN 11
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22
#define BUZZER_PIN   21
#define BUZZER_PIN_2 10
#define BUZZER_FREQUENCY 10000

// ================= Definições para o Display OLED =================
const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;
uint8_t buf[SSD1306_BUF_LEN];
struct render_area frame_area = {
    .start_col = 0,
    .end_col = SSD1306_WIDTH - 1,
    .start_page = 0,
    .end_page = SSD1306_NUM_PAGES - 1
};

// ================= Definições para NeoPixel =================
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

// ================= Variáveis Globais =================
uint32_t button_a_count = 0;
uint32_t button_b_count = 0;

// ================= Funções  =================
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

// ================= Função Principal =================
int main() {
    stdio_init_all();
    sleep_ms(5000);
    printf("Iniciando o sistema...\n");
  
    // Inicializa ADC para o joystick
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
  
    // Inicializa NeoPixel
    npInit(LED_PIN);
    npClear();
    setBrightness(128);
  
    // Inicializa os LEDs da placa
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, false);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, false);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, false);
  
    // Configura os GPIOs dos botões A e B
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    
  
    // Configura o GPIO do botão do joystick (GPIO22)
    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK, GPIO_IN);
    gpio_pull_up(BUTTON_JOYSTICK);
  
    // Configura os GPIOs dos buzzers
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_init(BUZZER_PIN_2);
    gpio_set_dir(BUZZER_PIN_2, GPIO_OUT);
  
    // Inicializa o PWM para ambos os buzzers
    pwm_init_buzzer(BUZZER_PIN);
    pwm_init_buzzer(BUZZER_PIN_2);
  
    // Inicializa o display OLED
    initDisplay();
    displayMessage("   Iniciando");
    displayPatterns();
    npTurnOffAll();
  
    // Variáveis para controle de antirrebote
    bool prev_button_a = false;
    bool prev_button_b = false;
    bool prev_button_joystick = false;
  
    while (true) {
      // Leitura do joystick
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
  
      // Leitura dos botões
      bool current_button_a = !gpio_get(BUTTON_A);
      bool current_button_b = !gpio_get(BUTTON_B);
  
      if (current_button_a && !prev_button_a) {
          processButtonAction(BUTTON_A, BUZZER_PIN, LED_BLUE, &button_a_count, "A");
          sleep_ms(1000);
      }
      if (current_button_b && !prev_button_b) {
          processButtonAction(BUTTON_B, BUZZER_PIN_2, LED_RED, &button_b_count, "B");
          sleep_ms(1000);
      }
  
      // Leitura do botão do joystick (GPIO22)
      bool current_button_joystick = !gpio_get(BUTTON_JOYSTICK);
      if (current_button_joystick && !prev_button_joystick) {
          joystickButtonAction(BUTTON_JOYSTICK, BUZZER_PIN, BUZZER_PIN_2, LED_GREEN, &button_a_count, &button_b_count, "");
          sleep_ms(1000);
      }
      
      prev_button_a = current_button_a;
      prev_button_b = current_button_b;
      prev_button_joystick = current_button_joystick;
    }
  
    return 0;
  }

// ================= Funções para Buzzer =================
void pwm_init_buzzer(uint pin) {
    printf("Inicializando PWM para buzzer no pino %d\n", pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void beep(uint pin, uint duration_ms) {
    printf("Executando beep no pino %d por %d ms\n", pin, duration_ms);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 2048);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
    sleep_ms(100);
}

// ================= Funções para Display OLED =================
void initDisplay() {
    printf("Inicializando display OLED via I2C\n");
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

void   displayJoystickInfo(int pos) {
    char msg[32];
    snprintf(msg, sizeof(msg),"posicao led %d",pos);
    displayMessage(msg);
}

// ================= Funções para NeoPixel =================
void npInit(uint pin) {
    printf("Inicializando NeoPixel na PIO com pino %d\n", pin);
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
    printf("Brilho definido para %d\n", brightnessLevel);
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
    printf("Mostrando padrões na matriz de LEDs\n");
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
  printf("Desligando todos os LEDs\n");
  for (uint i = 0; i < LED_COUNT; ++i) {
      npSetLED(i, 0, 0, 0); 
  }
  npWrite();
}

// ================= Funções para Botões =================
void processButtonAction(uint button, uint buzzer_pin, uint led_pin, uint32_t *counter, const char *btnName) {
  (*counter)++;
  printf("Botão %s pressionado. Contagem = %u\n", btnName, *counter);
  gpio_put(led_pin, true);
  char msg[32];
  snprintf(msg, sizeof(msg), "   Botao %s: %u", btnName, *counter);
  displayMessage(msg);
  beep(buzzer_pin, 1000);
  gpio_put(led_pin, false);
}

void joystickButtonAction(uint button, uint buzzer_pin_1, uint buzzer_pin_2, uint led_pin, 
    uint32_t *counter_a, uint32_t *counter_b, const char *btnName) {
printf("Joystick pressionado. Contagem A = %u, B = %u\n", *counter_a, *counter_b);

gpio_put(led_pin, true);

// Monta a mensagem com os contadores. Se btnName não estiver vazio, inclui-o na mensagem.
char msg[32];
if (btnName[0] != '\0') {
    snprintf(msg, sizeof(msg), "%s: A %u, B %u", btnName, *counter_a, *counter_b);
} else {
    snprintf(msg, sizeof(msg), "A: %u, B: %u", *counter_a, *counter_b);
}
displayMessage(msg);
beep(buzzer_pin_1, 1000);
beep(buzzer_pin_2, 1000);
gpio_put(led_pin, false);
}