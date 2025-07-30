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
 * for each color channel.
 */
void configGPIO(void);

/**
 * @brief Generates a blocking delay using nested loops.
 */
void delay();

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
    while (1) {
    for (int i = 0; i < 16; i++) {
        LPC_GPIO0->FIOCLR = 0xFF;
        LPC_GPIO0->FIOSET = number[i];
        delay();
    }
    }
}
void configGPIO(void) {
    LPC_PINCON->PINSEL0 &= ~(0xFFFF);  // P0.0 to P0.7 as GPIO        
    LPC_PINCON->PINMODE0 &= ~(0x0000FFFF);  // clear 16 pin of pin mode
    LPC_PINCON->PINMODE0 |=  (0x0000AAAA);  // desactivate pull-up/pull-down resistors with option -01-
                                            // 0X AAAA = 1010 1010 1010 
    LPC_GPIO0 ->FIODIR |= 0xFF;       // Set P0.0 a P0.7 as output
}
void delay() {
    for(uint32_t i=0; i<DELAY; i++)
        for(uint32_t j=0; j<DELAY; j++);
}