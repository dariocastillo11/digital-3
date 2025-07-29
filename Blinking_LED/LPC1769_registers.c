/**
 * @file LPC1769_reg.c
 * @brief Controls the RGB LED sequence on the LPC1769 board using GPIO registers.
 *
 * This program configures the GPIO pins connected to the onboard RGB LED of the LPC1769
 * and cycles through a sequence of colors: red, green, blue, cyan, magenta, yellow, and white.
 * The LEDs are active-low.
 */
#include "LPC17xx.h"

/**
 * @def REDLED
 * @brief Bit mask for the red LED (P0.22).
 */
#define REDLED      (1<<22)

/**
 * @def GREENLED
 * @brief Bit mask for the green LED (P3.25).
 */
#define GREENLED    (1<<25)

/**
 * @def BLUELED
 * @brief Bit mask for the blue LED (P3.26).
 */
#define BLUELED     (1<<26)

/**
 * @def DELAY
 * @brief Delay constant for LED timing.
 */
#define DELAY       2500

/**
 * @brief Configures the GPIO pins for the RGB LEDs as outputs.
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
 * @brief Main function. Initializes GPIO and cycles through RGB LED color sequence.
 *
 * The sequence includes primary colors (red, green, blue), secondary colors (cyan, magenta, yellow),
 * and white (all LEDs on), repeating indefinitely.
 * @return int Always returns 0 (never reached).
 */
int main(void) {
    configGPIO();

    LPC_GPIO0->FIOSET |= REDLED;    // Turn off red LED
    LPC_GPIO3->FIOSET |= GREENLED;  // Turn off green LED

    while(1) {
        // Blue
        LPC_GPIO0->FIOSET |= REDLED;
        LPC_GPIO3->FIOSET |= GREENLED;
        delay();

        // Red
        LPC_GPIO0->FIOCLR |= REDLED;
        LPC_GPIO3->FIOSET |= BLUELED;
        delay();

        // Green
        LPC_GPIO0->FIOSET |= REDLED;
        LPC_GPIO3->FIOCLR |= GREENLED;
        delay();

        // Cyan (Green + Blue)
        LPC_GPIO3->FIOCLR |= BLUELED;
        delay();

        // Magenta (Red + Blue)
        LPC_GPIO0->FIOCLR |= REDLED;
        LPC_GPIO3->FIOSET |= GREENLED;
        delay();

        // Yellow (Red + Green)
        LPC_GPIO3->FIOCLR |= GREENLED;
        LPC_GPIO3->FIOSET |= BLUELED;
        delay();

        // White (Red + Green + Blue)
        LPC_GPIO3->FIOCLR |= BLUELED;
        delay();
    }
    return 0 ;
}

void configGPIO(void) {
    LPC_PINCON->PINSEL1 &= ~(0x3<<12);      // P0.22 as GPIO
    LPC_PINCON->PINSEL7 &= ~(0xf<<18);      // P3.25 and P3.26 as GPIO

    LPC_GPIO0->FIODIR |= REDLED;            // Set P0.22 as output
    LPC_GPIO3->FIODIR |= BLUELED;           // Set P3.25 as output
    LPC_GPIO3->FIODIR |= GREENLED;          // Set P3.26 as output
}

void delay() {
    for(uint32_t i=0; i<DELAY; i++)
        for(uint32_t j=0; j<DELAY; j++);
}
