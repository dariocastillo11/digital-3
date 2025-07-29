/**
 * @file LPC1769_dri.c
 * @brief Controls the RGB LED sequence on the LPC1769 board using CMSIS drivers.
 *
 * This program configures the GPIO pins connected to the onboard RGB LED of the LPC1769
 * and cycles through a sequence of colors: red, green, blue, cyan, magenta, yellow, and white.
 * The LEDs are active-low. GPIO is managed using CMSIS peripheral drivers.
 */
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

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

    GPIO_SetValue(0, REDLED);               // Turn off red LED
    GPIO_SetValue(3, GREENLED);             // Turn off green LED

    while(1) {
        // Blue
        GPIO_SetValue(0, REDLED);
        GPIO_SetValue(3, GREENLED);
        delay();

        // Red
        GPIO_ClearValue(0, REDLED);
        GPIO_SetValue(3, BLUELED);
        delay();

        // Green
        GPIO_SetValue(0, REDLED);
        GPIO_ClearValue(3, GREENLED);
        delay();

        // Cyan (Green + Blue)
        GPIO_ClearValue(3, BLUELED);
        delay();

        // Magenta (Red + Blue)
        GPIO_ClearValue(0, REDLED);
        GPIO_SetValue(3, GREENLED);
        delay();

        // Yellow (Red + Green)
        GPIO_ClearValue(3, GREENLED);
        GPIO_SetValue(3, BLUELED);
        delay();

        // White (Red + Green + Blue)
        GPIO_ClearValue(3, BLUELED);
        delay();
    }
    return 0 ;
}

void configGPIO(void) {
    PINSEL_CFG_Type pinCfg;                 // Initialization of the PINSEL configuration structure.
    pinCfg.Portnum = PINSEL_PORT_0;
    pinCfg.Pinnum = PINSEL_PIN_22;
    pinCfg.Funcnum = PINSEL_FUNC_0;
    pinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
    pinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    PINSEL_ConfigPin(&pinCfg);              // Configure P0.22 as GPIO.

    pinCfg.Portnum = PINSEL_PORT_3;
    pinCfg.Pinnum = PINSEL_PIN_25;
    PINSEL_ConfigPin(&pinCfg);              // Configure P3.25 as GPIO.

    pinCfg.Pinnum = PINSEL_PIN_26;
    PINSEL_ConfigPin(&pinCfg);              // Configure P3.26 as GPIO.

    GPIO_SetDir(0, REDLED, 1);              // Set P0.22 as output.
    GPIO_SetDir(3, GREENLED | BLUELED, 1);  // Set P3.25 and P3.26 as output.
}

void delay() {
    for(uint32_t i=0; i<DELAY; i++)
        for(uint32_t j=0; j<DELAY; j++);
}
