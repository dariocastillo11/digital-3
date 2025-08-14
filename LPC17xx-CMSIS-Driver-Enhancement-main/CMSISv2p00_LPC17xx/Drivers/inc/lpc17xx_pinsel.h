/***********************************************************************/ /**
 * @file        lpc17xx_pinsel.h
 * @brief       Contains all macro definitions and function prototypes
 *              support for Pin connect block firmware library on LPC17xx
 * @version     2.0
 * @date        21. May. 2010
 * @author      NXP MCU SW Application Team
 **************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **************************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup PINSEL PINSEL
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_PINSEL_H_
#define LPC17XX_PINSEL_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup PINSEL_Public_Macros PINSEL Public Macros
 * @{
 */

/*********************************************************************/ /**
 *!< Macros define for port Selection.
 ***********************************************************************/
#define PINSEL_PORT_0               ((0))   /**< Port 0.*/
#define PINSEL_PORT_1               ((1))   /**< Port 1.*/
#define PINSEL_PORT_2               ((2))   /**< Port 2.*/
#define PINSEL_PORT_3               ((3))   /**< Port 3.*/
#define PINSEL_PORT_4               ((4))   /**< Port 4.*/

/***********************************************************************
 * Macros define for pin function selection.
 **********************************************************************/
#define PINSEL_FUNC_0               ((0))   /**< Default function.*/
#define PINSEL_FUNC_1               ((1))   /**< First alternate function.*/
#define PINSEL_FUNC_2               ((2))   /**< Second alternate function.*/
#define PINSEL_FUNC_3               ((3))   /**< Third or reserved alternate function.*/

/***********************************************************************
 * Macros define for pin selection.
 **********************************************************************/
#define PINSEL_PIN_0                ((0))   /**< Pin 0.*/
#define PINSEL_PIN_1                ((1))   /**< Pin 1.*/
#define PINSEL_PIN_2                ((2))   /**< Pin 2.*/
#define PINSEL_PIN_3                ((3))   /**< Pin 3.*/
#define PINSEL_PIN_4                ((4))   /**< Pin 4.*/
#define PINSEL_PIN_5                ((5))   /**< Pin 5.*/
#define PINSEL_PIN_6                ((6))   /**< Pin 6.*/
#define PINSEL_PIN_7                ((7))   /**< Pin 7.*/
#define PINSEL_PIN_8                ((8))   /**< Pin 8.*/
#define PINSEL_PIN_9                ((9))   /**< Pin 9.*/
#define PINSEL_PIN_10               ((10))  /**< Pin 10.*/
#define PINSEL_PIN_11               ((11))  /**< Pin 11.*/
#define PINSEL_PIN_12               ((12))  /**< Pin 12.*/
#define PINSEL_PIN_13               ((13))  /**< Pin 13.*/
#define PINSEL_PIN_14               ((14))  /**< Pin 14.*/
#define PINSEL_PIN_15               ((15))  /**< Pin 15.*/
#define PINSEL_PIN_16               ((16))  /**< Pin 16.*/
#define PINSEL_PIN_17               ((17))  /**< Pin 17.*/
#define PINSEL_PIN_18               ((18))  /**< Pin 18.*/
#define PINSEL_PIN_19               ((19))  /**< Pin 19.*/
#define PINSEL_PIN_20               ((20))  /**< Pin 20.*/
#define PINSEL_PIN_21               ((21))  /**< Pin 21.*/
#define PINSEL_PIN_22               ((22))  /**< Pin 22.*/
#define PINSEL_PIN_23               ((23))  /**< Pin 23.*/
#define PINSEL_PIN_26               ((26))  /**< Pin 26.*/
#define PINSEL_PIN_27               ((27))  /**< Pin 27.*/
#define PINSEL_PIN_28               ((28))  /**< Pin 28.*/
#define PINSEL_PIN_29               ((29))  /**< Pin 29.*/
#define PINSEL_PIN_24               ((24))  /**< Pin 24.*/
#define PINSEL_PIN_30               ((30))  /**< Pin 30.*/
#define PINSEL_PIN_25               ((25))  /**< Pin 25.*/
#define PINSEL_PIN_31               ((31))  /**< Pin 31.*/

/***********************************************************************
 * Macros define for Pin mode.
 **********************************************************************/
#define PINSEL_PINMODE_PULLUP       ((0))   /**< Internal pull-up resistor.*/
#define PINSEL_PINMODE_REPEATER     ((1))   /**< Repeater mode.*/
#define PINSEL_PINMODE_TRISTATE     ((2))   /**< Tri-state.*/
#define PINSEL_PINMODE_PULLDOWN     ((3))   /**< Internal pull-down resistor.*/

/***********************************************************************
 * Macros define for Pin mode (normal/open drain).
 **********************************************************************/
#define PINSEL_OD_MODE_NORMAL       ((0))   /**< Pin is not in open drain mode.*/
#define PINSEL_OD_MODE_OPENDRAIN    ((1))   /**< Pin is in the open drain mode.*/

/***********************************************************************
 * Macros define for I2C mode.
 ***********************************************************************/
#define PINSEL_I2C_NORMAL           ((0))   /**< The standard drive mode.*/
#define PINSEL_I2C_FAST             ((1))   /**< Fast Mode Plus drive mode.*/

/***********************************************************************
 * Macros define for masks.
 ***********************************************************************/
#define PINSEL_FUNC_MASK            ((0x03UL))  /**< Function selection mask.*/
#define PINSEL_PIN_MASK             ((0x01UL))  /** Pin selection mask.*/

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup PINSEL_Private_Macros PINSEL Private Macros
 * @{
 */

/* Pin selection define */
/* I2C Pin Configuration register bit description */
#define PINSEL_I2CPADCFG_SDADRV0 _BIT(0) /**< Drive mode control for the SDA0 pin, P0.27.*/
#define PINSEL_I2CPADCFG_SDAI2C0 _BIT(1) /**< I2C mode control for the SDA0 pin, P0.27.*/
#define PINSEL_I2CPADCFG_SCLDRV0 _BIT(2) /**< Drive mode control for the SCL0 pin, P0.28.*/
#define PINSEL_I2CPADCFG_SCLI2C0 _BIT(3) /**< I2C mode control for the SCL0 pin, P0.28.*/

/**
 * @}
 */

/* Public Types --------------------------------------------------------------- */
/** @defgroup PINSEL_Public_Types PINSEL Public Types
 * @{
 */

/** @brief Pin configuration structure.*/
typedef struct {
    uint8_t portNum;    /**< Should be PINSEL_PORT_x, where x should be
                             in range from 0 to 4.*/
    uint8_t pinNum;     /**< Should be PINSEL_PIN_x, where x should be
                             in range from 0 to 31.*/
    uint8_t funcNum;    /**< Should be PINSEL_FUNC_x, where x should be
                             in range from 0 to 3.*/
    uint8_t pinMode;    /**< Should be:
                             - PINSEL_PINMODE_PULLUP : Internal pull-up resistor.
                             - PINSEL_PINMODE_REPEATER : Repeater mode.
                             - PINSEL_PINMODE_TRISTATE : Tri-state.
                             - PINSEL_PINMODE_PULLDOWN : Internal pull-down resistor.*/
    uint8_t openDrain;  /**< Should be:
                             - PINSEL_OD_MODE_NORMAL : Pin is in the normal (not open drain) mode.
                             - PINSEL_OD_MODE_OPENDRAIN : Pin is in the open drain mode.*/
} PINSEL_CFG_Type;

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup PINSEL_Public_Functions PINSEL Public Functions
 * @{
 */

/*********************************************************************/ /**
 * @brief       Configures the pin according to the parameters in PinCfg.
 *
 * @param[in]   PinCfg  Pointer to a `PINSEL_CFG_Type` structure that contains
 *                      the configuration information for the specified pin.
 **********************************************************************/
void PINSEL_ConfigPin(const PINSEL_CFG_Type* PinCfg);

/*********************************************************************/ /**
 * @brief       Configures multiple pins according to the parameters in
 *              PinCfg and the bitValue mask.
 *
 * @param[in]   PinCfg    Pointer to a `PINSEL_CFG_Type` structure containing
 *                        the base configuration for the pins.
 * @param[in]   numPins   32-bit value where each bit set to 1 indicates that
 *                        the corresponding pin (0-31) will be configured.
 *
 * @note        For each bit set in bitValue, the corresponding pin is configured
 *              using the parameters from PinCfg, except that the pinNum field in
 *              the original PinCfg is ignored and set automatically for each pin.
 **********************************************************************/
void PINSEL_ConfigMultiplePins(const PINSEL_CFG_Type* PinCfg, uint32_t numPins);

/*********************************************************************/ /**
 * @brief       Configures the trace function.
 *
 * @param[in]   NewState Must be:
 *                       - ENABLE : Enable Trace Function.
 *                       - DISABLE : Disable Trace Function.
 **********************************************************************/
void PINSEL_ConfigTraceFunc(FunctionalState NewState);

/*********************************************************************/ /**
 * @brief       Configures the I2C pins according to the specified parameters.
 *
 * @param[in]   driveMode Should be one of the following:
 *                        - PINSEL_I2C_NORMAL : Standard drive mode.
 *                        - PINSEL_I2C_FAST   : Fast Mode Plus drive mode.
 *
 * @param[in]   filterSlewRate Should be:
 *                             - ENABLE  : Enables filter and slew rate control.
 *                             - DISABLE : Disables filter and slew rate control.
 *
 * @note        If `filterSlewRate` is DISABLE, the `driveMode` parameter
 *              is ignored and both pins are configured as standard drive mode
 *              (`PINSEL_I2C_NORMAL`) with filter and slew rate control disabled.
 **********************************************************************/
void PINSEL_SetI2CPins(uint8_t driveMode, FunctionalState filterSlewRate);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_PINSEL_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
