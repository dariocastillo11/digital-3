#include "lpc17xx.h"
//#define STCTRL      (*( ( volatile unsigned long *) 0xE000E010 )) // System Timer Control and status register
//#define STRELOAD    (*( ( volatile unsigned long *) 0xE000E014 )) // System Timer Reload value register
//#define STCURR      (*( ( volatile unsigned long *) 0xE000E018 )) // System Timer Current value register
//#define SBIT_ENABLE 0 //System Tick counter enable. When 1, the counter is enabled. When 0,
					  //the counter is disabled.
//#define SBIT_TICKINT 1 //System Tick interrupt enable. When 1, the System Tick interrupt is
					   //enabled. When 0, the System Tick interrupt is disabled. When enabled,
					   //the interrupt is generated when the System Tick counter counts down to 0.
//#define SBIT_CLKSOURCE 2 //System Tick clock source selection. When 1, the CPU clock is selected.
				         //When 0, the external clock pin (STCLK) is selected
//#define RELOAD_VALUE 99999;//1ms
/*!
 * 100MHz * 100ms = 10.000.000 - 1
 */
#define OUT_LED 22 //P0.22
#define entrada 11
void configurar_pines(void);
int leer_pin_P0_11(void);
int contador_periodo=20;
int contador_ciclo_trabajo=1;
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
	//SystemInit();
	configurar_pines();
	//STRELOAD = RELOAD_VALUE;
	//STCTRL = (1<<SBIT_ENABLE) | (1<<SBIT_TICKINT) | (1<<SBIT_CLKSOURCE);
    //ver de como usar systickconfig
    SysTick_Config(100000); // Configura SysTick para 1ms
	LPC_GPIO0->FIODIR  |= (1<<OUT_LED);
	while(1) {
	}
	return 0 ;
}

void SysTick_Handler(void){
    // debe entrar cada 1ms . periodo 20ms
    //prender led 1 ms apagarlo durante el resto del periodo
	
    if(contador_periodo<=contador_ciclo_trabajo){
        LPC_GPIO0->FIOSET |= (1<<OUT_LED);
    }
    else if (contador_periodo==0){
        contador_periodo=20;
    }
    else{
        LPC_GPIO0->FIOCLR |= (1<<OUT_LED);  
    }
	contador_periodo -=1;
    (void) SysTick->CTRL; // Leer el registro de control para limpiar el flag de interrupción
// LIMPIAR FLAGS
}
//p011 como entrada
void configurar_pines(){
	LPC_PINCON->PINSEL0 &= ~(0x3 << 22); // P0.11 como GPIO
	//PONGO RESISTENCIA DE PULLDOWN EN P0.11
	LPC_PINCON->PINMODE0 &= ~(0x3 << 22); // Limpia modo
	LPC_PINCON->PINMODE0 |=  (0x2 << 22); // Pull-down en P0.11
	//PONGO P0.11 COMO ENTRADA
	LPC_GPIO0->FIODIR &= ~(1 << entrada); // P0.11 como entrada
	//PONGO P0.22 COMO SALIDA
	LPC_GPIO0->FIODIR |=  (1 << OUT_LED); // P0
	//INICIALIZO EL LED APAGADO
	LPC_GPIO0->FIOCLR = (1 << OUT_LED); // P0.

}
int leer_pin_P0_11(){
    if(LPC_GPIO0->FIOPIN & (1 << entrada)) {
       return 1;
    } else {
        return 4;

    }
}
