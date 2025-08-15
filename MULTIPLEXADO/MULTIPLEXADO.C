/**
* @file LPC1769_reg.c
* @brief BCD up-counter on the LPC1769 board using GPIO registers.
* This program configures the 8 GPIO pins to drive a 7-segment display.
* The main function is to count from 0 to 15 on the display and restart the count.
* A common-cathode display with positive logic is used.
*/
#include "LPC17xx.h"

/**
    * @def DELAY
    * @brief Delay constant for the blocking delay function.
 */
#define DELAY_MULTIPLEX 100
#define DELAY_CONTADOR  1000
int centena_valor;
int decena_valor;
int unidad_valor;
int valor_actual = 0;
/**
 * @brief Configures the GPIO pins for the display as outputs.
 *
 * Sets the pin function to GPIO and configures the direction as output
 */
void configGPIO(void);
void actualizarDigitos(int value);
/**
 * @brief Generates a blocking delay using nested loops.
 */
void delayContador(void);
void delayMultiplex(void);
void updateDisplays( void);
/**
* @brief Function to increment the value on display and reset if the count reaches the last value of the constant number
*/
void changeValue(void);
/**
 * @def number
 * @brief defines the BCD values for numbers 0-9 and letters A-F to be displayed on the 7-segment display.
 */
const uint32_t number[10] = {
    0x3F, // Number 0 in BCD
    0x06, // Number 1 in BCD
    0x5B, // Number 2 in BCD
    0x4F, // Number 3 in BCD
    0x66, // Number 4 in BCD
    0x6D, // Number 5 in BCD
    0x7D, // Number 6 in BCD
    0x07, // Number 7 in BCD
    0x7F, // Number 8 in BCD
    0x6F, // Number 9 in BCD
};

/**
* @brief Main function. Initializes GPIO and creates a loop to iterate through the constant
* of 16 hexadecimal values that are displayed on the pins connected to the 7-segment display.
* @return int Always returns 0 (never reached).
*/
int main(void) {
    configGPIO();
    int value = 0;
    while (1) {
    	actualizarDigitos(valor_actual);
        for (int i = 0; i < 100; i++) {
            updateDisplays();
        }
        valor_actual = (valor_actual + 1) % 1000;
        delayContador();

    }
}
void actualizarDigitos(int value) {
    int centenas = (value / 100) % 10;
    int decenas  = (value / 10) % 10;
    int unidades = value % 10;

    centena_valor = number[centenas];
    decena_valor  = number[decenas];
    unidad_valor  = number[unidades];
}
void changeValue (void){
    static int i=0;
    //LIMPIO PINES SEGMENTOS
    LPC_GPIO0->FIOCLR = 0xFF;
    LPC_GPIO0->FIOSET = number[i];

    delayContador();
    i = (i + 1) % 10;
}
//ACTUALIZAR DISPLAY
void updateDisplays() {
    static int displayIndex = 0;

    LPC_GPIO0->FIOSET = (1 << 9) | (1 << 10) | (1 << 11);  // Apagar displays
    LPC_GPIO0->FIOCLR = 0xFF;  // Apagar segmentos

    switch (displayIndex) {
        case 0:
            LPC_GPIO0->FIOCLR = (1 << 9);
            LPC_GPIO0->FIOSET = centena_valor;
            break;
        case 1:
            LPC_GPIO0->FIOCLR = (1 << 10);
            LPC_GPIO0->FIOSET = decena_valor;
            break;
        case 2:
            LPC_GPIO0->FIOCLR = (1 << 11);
            LPC_GPIO0->FIOSET = unidad_valor;
            break;
    }

    displayIndex = (displayIndex + 1) % 3;
    delayMultiplex();
}

void configGPIO(void) {

    LPC_PINCON->PINSEL0 &= ~(0x3FFFFF);  // P0.0 a P0.21 como GPIO
    LPC_PINCON->PINMODE0 &= ~(0x3FFFFF); // clear 16 pin of pin mode
    LPC_PINCON->PINMODE0 |=  (0xAAAAAA); // Desactivar resistencias pull-up/pull-down
                                           // Configurar con A 0101 (01)

    // Configurar P0.0 a P0.10 como salida
    LPC_GPIO0->FIODIR |= 0x7FF;  // 0x7FF = 0000 0111 1111 1111 (bits 0 a 10)

}
void delayMultiplex() {
    for (uint32_t i = 0; i < DELAY_MULTIPLEX; i++)
        for (uint32_t j = 0; j < DELAY_MULTIPLEX; j++);
}

void delayContador() {
    for (uint32_t i = 0; i < DELAY_CONTADOR; i++)
        for (uint32_t j = 0; j < DELAY_CONTADOR; j++);
}


