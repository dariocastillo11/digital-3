#include "LPC17xx.h"
#define  SEGMENTOS 0x7F
#define DISPLAY_1 8
#define DISPLAY_2 9
#define DISPLAY_3 10
#define CONTROL_DISPLAY ((1<<DISPLAY_1) | (1<<DISPLAY_2) | (1<<DISPLAY_3))
void configGPIO(void);
void actualizarDigitos(void);
void mostrarDisplayActual(void);
// Segmentos para números 0 a 9
const uint8_t number[10] = {
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111  // 9
};

// conteo de digitos
volatile int centena_real = 0;
volatile int decena_real  = 0;
volatile int unidad_real  = 0;
// control de miltiplexado
volatile int displayIndex = 0;
volatile int counter = 0;

int main(void) {
    SysTick_Config(1000000);
    configGPIO();
    while(1) {
    }
    return 0;
}

void configGPIO(void) {
    // Configurar P0.0 a P0.10 como GPIO
    LPC_PINCON->PINSEL0 &= ~(SEGMENTOS);
    LPC_PINCON->PINSEL0 &= ~((3 << (DISPLAY_1 * 2)) | (3 << (DISPLAY_2 * 2)) | (3 << (DISPLAY_3 * 2)));

    LPC_PINCON->PINMODE0 &= ~(SEGMENTOS);
    LPC_PINCON->PINMODE0 &= ~(CONTROL_DISPLAY);

    LPC_PINCON->PINMODE0 |= 0xAAAAAA; // Sin pull-up/pull-down

    LPC_GPIO0->FIODIR |= SEGMENTOS; // P0.0 a P0.10 como salida
    LPC_GPIO0->FIODIR |= (1<<DISPLAY_1) | (1<<DISPLAY_2) | (1<<DISPLAY_3);
}
void SysTick_Handler(void) {
    mostrarDisplayActual();
    // Actualizar cada 1 segundo (20ms * 50)
    counter++;
    if(counter >= 9) {
        counter = 0;
        actualizarDigitos();
    }
}

void mostrarDisplayActual(void) {
    // Apagar todos los displays
    LPC_GPIO0->FIOCLR = CONTROL_DISPLAY;
    // Apagar todos los segmentos (P0.0 a P0.6)
    LPC_GPIO0->FIOCLR = SEGMENTOS;
    // Mostrar el dígito correspondiente
    switch(displayIndex) {
        case 0: // centenas
            LPC_GPIO0->FIOSET = number[centena_real];
            LPC_GPIO0->FIOSET = (1 << DISPLAY_1); // activar display centenas
            break;
        case 1: // decenas
            LPC_GPIO0->FIOSET = number[decena_real];
            LPC_GPIO0->FIOSET = (1 << DISPLAY_2); // activar display decenas
            break;
        case 2: // unidades
            LPC_GPIO0->FIOSET = number[unidad_real];
            LPC_GPIO0->FIOSET = (1 << DISPLAY_3); // activar display unidades
            break;
    }
    // Siguiente display
    displayIndex = (displayIndex + 1) % 3;
}

void actualizarDigitos(void) {
    unidad_real = (unidad_real + 1) % 10;
    if (unidad_real == 0) {
        decena_real = (decena_real + 1) % 10;
        if (decena_real == 0) {
            centena_real = (centena_real + 1) % 10;
        }
    }
}
