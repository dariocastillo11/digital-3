#include "LPC17xx.h"

#define DIR_PIN   (1 << 15)
#define STEP_PIN  (1 << 16)
#define EN_PIN    (1 << 17)

#define ROTATION_TIME_MS 10000  // Tiempo total de giro por dirección
#define INTERVALO_MS     2      // Intervalo fijo entre pasos (velocidad)

volatile uint32_t systick_ms = 0;

void configGPIO(void);
void configSysTick(void);
void waitMs(uint32_t ms);
void stepMotorConstante(uint32_t tiempo_ms, int direccion, uint32_t intervalo_ms);

void configGPIO(void) {
    // Configurar pines como GPIO
    LPC_PINCON->PINSEL0 &= ~( (3 << 30) | (3 << 0) | (3 << 2) );
    LPC_PINCON->PINSEL1 &= ~( (3 << 0) );

    LPC_GPIO0->FIODIR |= DIR_PIN | STEP_PIN | EN_PIN;
    LPC_GPIO0->FIOCLR = EN_PIN; // habilitar driver (activo bajo)
}

void configSysTick(void) {
    SysTick->LOAD = (SystemCoreClock / 1000) - 1; // 1 ms
    SysTick->VAL = 0;
    SysTick->CTRL = 7; // habilitado con interrupción
}

void SysTick_Handler(void) {
    systick_ms++;
}

void waitMs(uint32_t ms) {
    uint32_t start = systick_ms;
    while((systick_ms - start) < ms);
}

// Movimiento a velocidad constante
void stepMotorConstante(uint32_t tiempo_ms, int direccion, uint32_t intervalo_ms) {
    if(direccion)
        LPC_GPIO0->FIOSET = DIR_PIN;
    else
        LPC_GPIO0->FIOCLR = DIR_PIN;

    uint32_t start = systick_ms;

    while((systick_ms - start) < tiempo_ms) {
        LPC_GPIO0->FIOSET = STEP_PIN;
        waitMs(intervalo_ms);        // pulso alto
        LPC_GPIO0->FIOCLR = STEP_PIN;
        waitMs(intervalo_ms);        // pulso bajo
    }
}

int main(void) {
    configGPIO();
    configSysTick();

    while(1) {
        stepMotorConstante(ROTATION_TIME_MS, 1, INTERVALO_MS);  // giro en un sentido
       // waitMs(500);
        //stepMotorConstante(ROTATION_TIME_MS, 0, INTERVALO_MS);  // giro en el otro
        //waitMs(500);
    }
}
