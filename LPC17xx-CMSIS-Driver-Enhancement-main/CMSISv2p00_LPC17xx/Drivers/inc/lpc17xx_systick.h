/***********************************************************************//**
 * @file		lpc17xx_systick.h
 * @brief		Contains all macro definitions and function prototypes
 * 				support for SYSTICK firmware library on LPC17xx
 * @version		2.0
 * @date		21. May. 2010
 * @author		NXP MCU SW Application Team
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
/** @defgroup SYSTICK SYSTICK
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_SYSTICK_H_
#define LPC17XX_SYSTICK_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private Macros ------------------------------------------------------------- */
/** @defgroup SYSTICK_Private_Macros SYSTICK Private Macros
 * @{
 */
/*********************************************************************//**
 * Macro defines for System Timer Control and status (STCTRL) register.
 **********************************************************************/
#define ST_CTRL_ENABLE		    ((uint32_t)(1<<0))
#define ST_CTRL_TICKINT		    ((uint32_t)(1<<1))
#define ST_CTRL_CLKSOURCE	    ((uint32_t)(1<<2))
#define ST_CTRL_COUNTFLAG	    ((uint32_t)(1<<16))

/*********************************************************************//**
 * Macro defines for System Timer Reload value (STRELOAD) register.
 **********************************************************************/
#define ST_RELOAD_RELOAD(n)		((uint32_t)(n & 0x00FFFFFF))

/*********************************************************************//**
 * Macro defines for System Timer Current value (STCURRENT) register.
 **********************************************************************/
#define ST_RELOAD_CURRENT(n)	((uint32_t)(n & 0x00FFFFFF))

/*********************************************************************//**
 * Macro defines for System Timer Calibration value (STCALIB) register.
 **********************************************************************/
#define ST_CALIB_TENMS(n)		((uint32_t)(n & 0x00FFFFFF))
#define ST_CALIB_SKEW			((uint32_t)(1<<30))
#define ST_CALIB_NOREF			((uint32_t)(1<<31))

#define CLKSOURCE_EXT			((uint32_t)(0))
#define CLKSOURCE_CPU			((uint32_t)(1))

#define ST_MAX_LOAD             ((uint32_t)(0x00FFFFFF))

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup SYSTICK_Public_Functions SYSTICK Public Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Initializes the System Tick timer using the internal CPU clock source.
 * @param[in]	time    Time interval in milliseconds.
 * @note		If the requested time exceeds the maximum possible interval for the SysTick timer,
 *              the LOAD register is set to its maximum value ST_MAX_LOAD, resulting in the
 *              longest possible timer interval.
 **********************************************************************/
void SYSTICK_InternalInit(uint32_t time);


/*********************************************************************//**
 * @brief       Initializes the System Tick timer using an external clock source.
 * @param[in]   extFreq External clock frequency in Hz.
 * @param[in]   time    Time interval in milliseconds.
 * @note        If the requested time exceeds the maximum possible interval for the SysTick timer
 *              with the given external frequency, the LOAD register is set to its maximum value
 *              ST_MAX_LOAD, resulting in the longest possible timer interval.
 **********************************************************************/
void SYSTICK_ExternalInit(uint32_t extFreq, uint32_t time);

/*********************************************************************//**
 * @brief       Enable or disable the System Tick counter.
 * @param[in]   NewState    System Tick counter status, should be:
 *                          - ENABLE
 *                          - DISABLE
 **********************************************************************/
void SYSTICK_Cmd(FunctionalState NewState);

/*********************************************************************//**
 * @brief       Enable or disable the System Tick interrupt.
 * @param[in]   NewState    System Tick interrupt status, should be:
 *                          - ENABLE
 *                          - DISABLE
 **********************************************************************/
void SYSTICK_IntCmd(FunctionalState NewState);

/*********************************************************************//**
 * @brief       Get the current value of the System Tick counter.
 * @return      Current value of the System Tick counter.
 **********************************************************************/
static __INLINE uint32_t SYSTICK_GetCurrentValue(void) {
    return (SysTick->VAL);
}

/*********************************************************************//**
 * @brief       Clear the System Tick counter flag.
 **********************************************************************/
static __INLINE void SYSTICK_ClearCounterFlag(void) {
    (void)SysTick->CTRL;
}

/*********************************************************************//**
 * @brief       Get the current reload value of the System Tick timer.
 * @return      Current reload value.
 **********************************************************************/
static __INLINE uint32_t SYSTICK_GetReloadValue(void) {
    return (SysTick->LOAD & ST_MAX_LOAD);
}

/*********************************************************************//**
 * @brief       Set a new reload value for the System Tick timer.
 * @param[in]   reloadTicks Reload value to set, in SysTick timer ticks.
 * @note        If reloadValue exceeds 24 bits, only the least significant 24 bits are used.
 **********************************************************************/
static __INLINE void SYSTICK_SetReloadValue(uint32_t reloadTicks) {
    SysTick->LOAD = (reloadTicks & ST_MAX_LOAD);
}

/*********************************************************************//**
 * @brief       Check if the System Tick timer is currently enabled.
 * @return      1 if enabled, 0 otherwise.
 **********************************************************************/
static __INLINE uint8_t SYSTICK_IsActive(void){
    return (SysTick->CTRL & ST_CTRL_ENABLE) ? 1 : 0;
}

/*********************************************************************//**
 * @brief       Check if the System Tick COUNTFLAG is set (timer has fired).
 * @return      1 if COUNTFLAG is set, 0 otherwise.
 **********************************************************************/
static __INLINE uint8_t SYSTICK_HasFired(void) {
    return (SysTick->CTRL & ST_CTRL_COUNTFLAG) ? 1 : 0;
}

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* LPC17XX_SYSTICK_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
