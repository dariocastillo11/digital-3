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
#define DELAY 2500

/**
 * @brief Configures the GPIO pins for the display as outputs.
 *
 * Sets the pin function to GPIO and configures the direction as output
 */
void configGPIO(void);
/**
 * @brief Configures the interrupt settings for the GPIO pins.
 */
void configInterrupt(void);

/**
 * @brief Generates a blocking delay using nested loops.
 */
void delay();

/**
* @brief Function to increment the value on display and reset if the count reaches the last value of the constant number
*/
void changeValue();
/**
 * @def number
 * @brief defines the BCD values for numbers 0-9 and letters A-F to be displayed on the 7-segment display.
 */
const uint32_t number[16] = {
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
    0x77, // letter a in BCD
    0X7C, // letter b in BCD
    0X39, // letter c in BCD
    0X5E, // letter d in BCD
    0X7b, // letter E in BCD
    0X71  // letter F in BCD
};

/**
* @brief Main function. Initializes GPIO and creates a loop to iterate through the constant
* of 16 hexadecimal values that are displayed on the pins connected to the 7-segment display.
* @return int Always returns 0 (never reached).
*/
int main(void) {
    configGPIO();
    configInterrupt();
    while (1) {
        //changeValue();
    }
}

void configGPIO(void) {
    LPC_PINCON->PINSEL0 &= ~(0xFFFF);  // P0.0 to P0.7 as GPIO
    LPC_PINCON->PINMODE0 &= ~(0x0000FFFF);  // clear 16 pin of pin mode
    LPC_PINCON->PINMODE0 |=  (0x0000AAAA);  // desactivate pull-up/pull-down resistors with option -01-
                                            // 0X AAAA = 1010 1010 1010
    LPC_GPIO0 ->FIODIR |= 0xFF;       // Set P0.0 a P0.7 as output
}

void configInterrupt(void) {
    //  P2.13 como entrada
    LPC_GPIO2->FIODIR &= ~(1 << 10);
    // Configura el pin sin resistencias pull-up/pull-down
    LPC_PINCON->PINMODE4 &= ~(0x3 << 26); // Limpia bits 26-27 (P2.13)
    LPC_PINCON->PINMODE4 |=  (0x2 << 26); // Pull-down
    // Habilito interrupción por flanco descendente en P2.13
    LPC_GPIOINT->IO2IntEnF |= (1 << 13);
    // Limpio flags
    LPC_GPIOINT->IO2IntClr = (1 << 13);
    // Habilita la interrupción en el NVIC, EINT3 para interrupciones GPIO
    NVIC_EnableIRQ(EINT3_IRQn);
// logica interrupcion para GPIO2 (EINT3)
}
void EINT3_IRQHandler(void) {
	for (volatile int i = 0; i < 50000; i++);
	if (!(LPC_GPIO2->FIOPIN & (1 << 13))) { // Si sigue presionado
		changeValue();
	}
	LPC_GPIOINT->IO2IntClr = (1 << 13);

}
void delay() {
    for(uint32_t i=0; i<DELAY; i++)
        for(uint32_t j=0; j<DELAY; j++);
}

void changeValue() {
    static int i=0;
    LPC_GPIO0->FIOCLR = 0xFF;
    LPC_GPIO0->FIOSET = number[i];
    //delay();
    i = (i + 1) % 16;
}
