/*
===============================================================================
 Name        : Timer_and_GPIO.c
 Author      :
 Version     :
===============================================================================
*/

#include "LPC17xx.h"

/*!
 * Header files to project. Include library
 */
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

// TODO: insert other definitions and declarations here

#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3

void config_GPIO(void);
void config_timer(void);

void TIMER0_IRQHandler(void){
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {
		// Clear the interrupt flag
		TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
		static uint8_t i=0;
		if(i==0){
			GPIO_ClearPins(PORT_ZERO, PIN_22);
			printf("prender led\r\n");
			i=1;
		} else {
			GPIO_SetPins(PORT_ZERO,PIN_22);
			printf("apagar led\r\n");
			i=0;
		}
	

	}



	//TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

	/*if (GPIO_ReadValue(PORT_ZERO)&PIN_22) {
		//GPIO_ClearPins
		//GPIO_ClearValue
		GPIO_ClearPins(PORT_ZERO, PIN_22);
	} else {
		//GPIO_SetPins
		//GPIO_SetValue
		GPIO_SetPins(PORT_ZERO,PIN_22);
	}*/

	return;
}

int main(void) {

	config_GPIO();
	config_timer();
	//GPIO_SetPins
	//GPIO_SetValue
	GPIO_SetPins(PORT_ZERO,PIN_22);

	while(1) {}
    return 0 ;
}

void config_GPIO(){
	PINSEL_CFG_Type pin_configuration;
	PINSEL_CFG_Type config_match;
	//PortNum
	pin_configuration.portNum 	=	PINSEL_PORT_0;
	pin_configuration.pinNum	=	PINSEL_PIN_22;
	pin_configuration.pinMode	=	PINSEL_PULLUP;
	pin_configuration.funcNum	= 	PINSEL_FUNC_0;
	pin_configuration.openDrain	=	PINSEL_OD_NORMAL;

	//CONFIGURO PIN MATCH
	config_match.portNum 	=	PINSEL_PORT_1;
	config_match.pinNum		=	PINSEL_PIN_22;
	config_match.funcNum	= 	PINSEL_FUNC_3;// FUNCION MATCH
	config_match.openDrain	=	PINSEL_OD_NORMAL;
	
	PINSEL_ConfigPin(&pin_configuration);
	PINSEL_ConfigPin(&config_match);

	GPIO_SetDir( PORT_ZERO , PIN_22 , OUTPUT );
	return;
}

void config_timer(){
	TIM_TIMERCFG_Type	struct_config;
	TIM_MATCHCFG_Type	struct_match;

	struct_config.prescaleOption	=	TIM_USVAL;
	struct_config.prescaleValue		=	1;

	struct_match.matchChannel		=	0;
	struct_match.intOnMatch			=	ENABLE;
	struct_match.resetOnMatch		=	ENABLE;
	struct_match.stopOnMatch		=	DISABLE;
	struct_match.extMatchOutputType	=	TIM_NOTHING;
	struct_match.matchValue			=	1000000;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &struct_config);
	TIM_ConfigMatch(LPC_TIM0, &struct_match);

	TIM_Cmd(LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);

	return;
}
