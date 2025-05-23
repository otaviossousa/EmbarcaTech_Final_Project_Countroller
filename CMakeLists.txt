# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializa o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(PROJECT_EMBARCATECH C CXX ASM)

pico_sdk_init()

# Adiciona o executável
add_executable(PROJECT_EMBARCATECH 
    src/main.c
    src/init.c
    src/joystick.c
    src/buttons.c
    src/buzzer.c
    src/neopixel.c
    src/display.c
)

# Adiciona bibliotecas
add_library(ssd1306 STATIC lib/ssd1306/ssd1306_i2c.c)
target_include_directories(ssd1306 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include/ssd1306
    ${PICO_SDK_PATH}/include
)
target_link_libraries(ssd1306 pico_stdlib hardware_i2c)

# Define o nome e a versão do programa
pico_set_program_name(PROJECT_EMBARCATECH "PROJECT_EMBARCATECH")
pico_set_program_version(PROJECT_EMBARCATECH "0.1")

# Gera cabeçalho para PIO
pico_generate_pio_header(PROJECT_EMBARCATECH ${CMAKE_CURRENT_LIST_DIR}/pio/ws2818b.pio)

# Habilita saída UART/USB
pico_enable_stdio_uart(PROJECT_EMBARCATECH 0)
pico_enable_stdio_usb(PROJECT_EMBARCATECH 1)

# Adiciona bibliotecas padrão
target_link_libraries(PROJECT_EMBARCATECH
    hardware_i2c
    pico_stdlib
    hardware_pio
    hardware_pwm
    hardware_clocks
    hardware_adc
    ssd1306
)

# Inclui diretórios necessários
target_include_directories(PROJECT_EMBARCATECH PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include/ssd1306
    ${PICO_SDK_PATH}/include
)

# Adiciona saídas extras (UF2, binário, etc.)
pico_add_extra_outputs(PROJECT_EMBARCATECH)