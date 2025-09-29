#include "LPC17xx.h"
#define OUT_LED 22     // P0.22 como salida
#define entrada 11     // P0.11 como entrada
#define salida 10
void configurar_pines(void);
int leer_pin_P0_11(void);

int contador_periodo = 20;
//int contador_ciclo_trabajo = 1;

int main(void) {
    configurar_pines();
    SysTick_Config(100000); // Configura SysTick para 1ms si Fclk = 100MHz

    while (1) {
        // Podés usar esta lectura para modificar el ciclo de trabajo dinámicamente
        // Ejemplo: contador_ciclo_trabajo = leer_pin_P0_11();
    }

    return 0;
}

void SysTick_Handler(void) {
    // PWM por software: periodo de 20ms, ciclo de trabajo de 1ms
    if (contador_periodo > (20 - leer_pin_P0_11())) {
        LPC_GPIO0->FIOCLR = (1 << OUT_LED); // Encender LED
        LPC_GPIO0->FIOSET = (1 << salida);
    } else {
        LPC_GPIO0->FIOSET = (1 << OUT_LED); // Apagar LED
        LPC_GPIO0->FIOCLR = (1 << salida);
    }

    contador_periodo--;
    if (contador_periodo <= 0) {
        contador_periodo = 20;
    }
}

// Configuración de pines
void configurar_pines(void) {
    // P0.11 como GPIO
    LPC_PINCON->PINSEL0 &= ~(0x3 << 22); // Bits 22-23 controlan P0.11
    LPC_PINCON->PINSEL0 &= ~(0x3 << 20);//pin p0.10 salida

    // Pull-down en P0.11
    LPC_PINCON->PINMODE0 &= ~(0x3 << 22); // Limpia modo
    LPC_PINCON->PINMODE0 |=  (0x2 << 22); // Pull-down

    // P0.11 como entrada
    LPC_GPIO0->FIODIR &= ~(1 << entrada);
    // P0.10 como salida
    LPC_GPIO0->FIODIR |= (1 << salida);

    // P0.22 como salida
    LPC_GPIO0->FIODIR |= (1 << OUT_LED);

    // Inicializa LED apagado
    LPC_GPIO0->FIOCLR = (1 << OUT_LED);
    LPC_GPIO0->FIOCLR = (1 << salida);

}

// Lectura digital de P0.11
int leer_pin_P0_11(void) {
    return (LPC_GPIO0->FIOPIN & (1 << entrada)) ? 1 : 5;
}
