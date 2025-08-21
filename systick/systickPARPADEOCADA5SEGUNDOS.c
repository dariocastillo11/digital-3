#include "lpc17xx.h"
#define STCTRL      (*( ( volatile unsigned long *) 0xE000E010 )) // System Timer Control and status register
#define STRELOAD    (*( ( volatile unsigned long *) 0xE000E014 )) // System Timer Reload value register
#define STCURR      (*( ( volatile unsigned long *) 0xE000E018 )) // System Timer Current value register
#define SBIT_ENABLE 0 //System Tick counter enable. When 1, the counter is enabled. When 0,
					  //the counter is disabled.
#define SBIT_TICKINT 1 //System Tick interrupt enable. When 1, the System Tick interrupt is
					   //enabled. When 0, the System Tick interrupt is disabled. When enabled,
					   //the interrupt is generated when the System Tick counter counts down to 0.
#define SBIT_CLKSOURCE 2 //System Tick clock source selection. When 1, the CPU clock is selected.
				         //When 0, the external clock pin (STCLK) is selected
#define RELOAD_VALUE 999999;//167ms
/*!
 * 100MHz * 100ms = 10.000.000 - 1
 */
#define OUT_LED 22 //P0.22

int delay1=50;
int main(void) {
    //system init: inicializa el sistema
    //su funcion es configurar el oscilador y la pll
    //para que el micro funcione a la frecuencia deseada
    //en este caso 100MHz
    //ademas de configurar el vector de interrupciones
    //hace otras configuraciones necesarias para el correcto
    //funcionamiento del micro como desactivar el watchdog
    //y configurar el flash, entre otras cosas como inicializar
    //los perifericos que se vayan a usar  , y configurar
    //los pines para que funcionen como se desea
	SystemInit();
	STRELOAD = RELOAD_VALUE;
	STCTRL = (1<<SBIT_ENABLE) | (1<<SBIT_TICKINT) | (1<<SBIT_CLKSOURCE);
	LPC_GPIO0->FIODIR  |= (1<<OUT_LED);
	while(1) {
	}
	return 0 ;
}

void SysTick_Handler(void){
    //un xor en el pin del led . lo que hace es cambiar su estado como si fuera un toggle
    //un toggle es una operacion logica que cambia el estado de un bit
    //si el bit es 0 lo cambia a 1
    //si el bit es 1 lo cambia a 0
	delay1 -=1;
	if(!delay1){
		LPC_GPIO0->FIOPIN ^= (1<<OUT_LED);
		delay1=50;
	}
// LIMPIAR FLAGS
}
