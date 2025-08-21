
#ifdef __USE_CMSIS
#include <lpc17xx.h>
#endif

#include <cr_section_macros.h>
//#include "lpc17xx_systick.h"
#include <lpc17xx_systick.h>
#define STCTRL      (*( ( volatile unsigned long *) 0xE000E010 ))
#define STRELOAD    (*( ( volatile unsigned long *) 0xE000E014 ))
#define STCURR      (*( ( volatile unsigned long *) 0xE000E018 ))

#define SBIT_ENABLE    0
#define SBIT_TICKINT   1
#define SBIT_CLKSOURCE 2

#define  SEGMENTOS 0x3FFF;
#define CONTROL_DISPLAY 0x7>>10; // 10 9 8 CONTROL DE DISPLAY
#define DISPLAY_1 8;
#define DISPLAY_2 9;
#define DISPLAY_3 10;

//#define RELOAD_VALUE 1999999;//20ms
#define RELOAD_VALUE 9999999;//100ms

void configGPIO(void);
void actualizarDigitos(void);
void mostrarDisplayActual(void);

// Segmentos para números 0..9 (GFEDCBA)
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

// Dígitos reales
volatile int centena_real = 0;
volatile int decena_real  = 0;
volatile int unidad_real  = 0;

// Multiplexado
volatile int displayIndex = 1;
volatile int counter = 0;

int main(void) {
    SystemInit();
    STRELOAD = RELOAD_VALUE;
    STCTRL = (1<<SBIT_ENABLE) | (1<<SBIT_TICKINT) | (1<<SBIT_CLKSOURCE);
    configGPIO();
    while(1) {
    }
    return 0;
}

void configGPIO(void) {
    // Configurar P0.0 a P0.10 como GPIO
    LPC_PINCON->PINSEL0 &= ~(SEGMENTOS);
    LPC_PINCON->PINSEL0 = ~(1<<DISPLAY_1) & ~(1<<DISPLAY_2) & ~(1<<DISPLAY_3);

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
    LPC_GPIO0->FIOSET = CONTROL_DISPLAY;
    // Apagar todos los segmentos (P0.0 a P0.6)
    LPC_GPIO0->FIOCLR = SEGMENTOS;
    // Mostrar el dígito correspondiente
    switch(displayIndex) {
        case 0: // centenas
            LPC_GPIO0->FIOSET = number[centena_real];
            LPC_GPIO0->FIOCLR = (DISPLAY_1); // activar display centenas
            break;
        case 1: // decenas
            LPC_GPIO0->FIOSET = number[decena_real];
            LPC_GPIO0->FIOCLR = (DISPLAY_2); // activar display decenas
            break;
        case 2: // unidades
            LPC_GPIO0->FIOSET = number[unidad_real];
            LPC_GPIO0->FIOCLR = (DISPLAY_3); // activar display unidades
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
