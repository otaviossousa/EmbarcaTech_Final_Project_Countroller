// Converte um código hexadecimal gerado no site: https://www.piskelapp.com/
// Para decimal em no padrão RGB.
// O código gerado será utilizado para exibir imagens na matriz de leds neopixel.
#include <stdio.h>

#define MATRIX_ROWS 5
#define MATRIX_COLS 5
#define MATRIX_DEPTH 3

// Função para converter valores ARGB (0xAARRGGBB) para RGB
void convertToRGB(int argb, int rgb[3]) {
    rgb[0] = argb & 0xFF;          // Blue
    rgb[2] = (argb >> 16) & 0xFF;  // Red
    rgb[1] = (argb >> 8) & 0xFF;   // Green
}

int main() {
    // Matriz de entrada com os valores ARGB
    int argb_values[MATRIX_ROWS * MATRIX_COLS] = // PI
    {
        0x00000000, 0xff0000ff, 0x00000000, 0xff0000ff, 0x00000000, 
        0xff0000ff, 0x00000000, 0xff0000ff, 0x00000000, 0xff0000ff, 
        0xff0000ff, 0x00000000, 0x00000000, 0x00000000, 0xff0000ff, 
        0x00000000, 0xff0000ff, 0x00000000, 0xff0000ff, 0x00000000, 
        0x00000000, 0x00000000, 0xff0000ff, 0x00000000, 0x00000000
    };

    // Matriz 5x5x3 para armazenar os valores RGB
    int rgb_matrix[MATRIX_ROWS][MATRIX_COLS][MATRIX_DEPTH];

    // Preencher a matriz RGB com a conversão dos valores ARGB
    for (int i = 0; i < MATRIX_ROWS * MATRIX_COLS; i++) {
        int rgb[3];
        convertToRGB(argb_values[i], rgb);
        
        int row = i / MATRIX_COLS;    // Cálculo da linha
        int col = i % MATRIX_COLS;    // Cálculo da coluna
        
        // Armazenar os valores RGB na matriz 5x5x3
        rgb_matrix[row][col][0] = rgb[0];  // Red
        rgb_matrix[row][col][1] = rgb[1];  // Green
        rgb_matrix[row][col][2] = rgb[2];  // Blue
    }

    // Exibir a matriz RGB resultante
    printf("{\n");
    for (int i = 0; i < MATRIX_ROWS; i++) {
        printf("    {");
        for (int j = 0; j < MATRIX_COLS; j++) {
            printf("{%d, %d, %d}", rgb_matrix[i][j][0], rgb_matrix[i][j][1], rgb_matrix[i][j][2]);
            if (j < MATRIX_COLS - 1) {
                printf(", ");
            }
        }
        printf("}");
        if (i < MATRIX_ROWS - 1) {
            printf(",");
        }
        printf("\n");
    }
    printf("}\n");

    return 0;
}