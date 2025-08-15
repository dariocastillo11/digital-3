#include "LPC17xx.h"

void configurar_pines();
void leer_pin_P0_11();

int main(void){
    configurar_pines();
    while(1) {
        leer_pin_P0_11();
    }
    return 0;
}

void configurar_pines(){
    // P0.10 y P0.11 como GPIO
    LPC_PINCON->PINSEL0 &= ~(0xF << 20);     // Bits 20-23: P0.10 y P0.11 como GPIO

    // Pull-down en P0.11
    LPC_PINCON->PINMODE0 &= ~(0x3 << 22);    // Limpia modo
    LPC_PINCON->PINMODE0 |=  (0x2 << 22);    // Pull-down en P0.11

    // Dirección de pines
    LPC_GPIO0->FIODIR &= ~(1 << 11);         // P0.11 como entrada
    LPC_GPIO0->FIODIR |=  (1 << 10);         // P0.10 como salida

    // Inicializa LED apagado
    LPC_GPIO0->FIOCLR = (1 << 10);
}

void leer_pin_P0_11(){
    if(LPC_GPIO0->FIOPIN & (1 << 11)) {
        LPC_GPIO0->FIOSET = (1 << 10);       // Enciende LED
        for(volatile int i = 0; i < 3000000; i++);
        LPC_GPIO0->FIOCLR = (1 << 10);       // Apaga LED
        for(volatile int i = 0; i < 3000000; i++);
    } else {
        LPC_GPIO0->FIOSET = (1 << 10);       // Enciende LED
        for(volatile int i = 0; i < 1000000; i++);
        LPC_GPIO0->FIOCLR = (1 << 10);       // Apaga LED
        for(volatile int i = 0; i < 1000000; i++);

    }
}
