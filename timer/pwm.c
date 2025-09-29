/*
===============================================================================
 Name        : PWM_ADC_Control.c
 Author      : 
 Version     : 2.0
 Description : PWM controlado por ADC (potenciómetro) usando drivers CMSIS
===============================================================================
*/

#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"      // ← Nuevo driver ADC

// Definiciones
#define OUTPUT          (uint8_t) 1
#define INPUT           (uint8_t) 0

#define PWM_PIN         ((uint32_t)(1<<22))  // P0.22 - LED rojo
#define PWM_PORT        (uint8_t) 0

// Configuración PWM
#define PWM_PERIOD_US   1000     // 1ms = 1kHz (mejor para dimming LED)
#define PWM_DUTY_CYCLE  25       // 25% duty cycle inicial

// Configuración ADC
#define ADC_CHANNEL     0        // ADC0 (P0.23)
#define ADC_FREQUENCY   200000   // 200kHz
#define ADC_SAMPLES     10       // Promedio de muestras

// Variables globales
volatile uint32_t pwm_period = PWM_PERIOD_US;
volatile uint32_t pwm_on_time;
volatile uint32_t pwm_off_time;
volatile uint8_t pwm_state = 0;  // 0 = LOW, 1 = HIGH
volatile uint16_t adc_value = 0;
volatile uint8_t adc_ready = 0;

// Prototipos de funciones
void config_GPIO(void);
void config_timer_pwm(void);
void config_ADC(void);
void set_duty_cycle(uint8_t duty_percent);
void pwm_output_high(void);
void pwm_output_low(void);
void update_pwm_from_adc(void);
uint16_t read_adc(void);
uint8_t adc_to_duty_cycle(uint16_t adc_val);

// Handler de interrupción del Timer
void TIMER0_IRQHandler(void) {
    // Match 0 - Inicio del período PWM
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        
        if (pwm_on_time > 0) {
            pwm_output_high();  // PIN HIGH
            pwm_state = 1;
        } else {
            pwm_output_low();   // PIN LOW (duty = 0%)
            pwm_state = 0;
        }
    }
    
    // Match 1 - Fin del tiempo ON
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT)) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
        
        if (pwm_state == 1) {
            pwm_output_low();   // PIN LOW
            pwm_state = 0;
        }
    }
}

// Handler de interrupción del ADC
void ADC_IRQHandler(void) {
    if (ADC_ChannelGetStatus(LPC_ADC, ADC_CHANNEL, ADC_DATA_DONE)) {
        adc_value = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL);
        adc_ready = 1;
    }
}

int main(void) {
    // Inicialización
    config_GPIO();
    config_ADC();
    set_duty_cycle(PWM_DUTY_CYCLE);  // Configurar duty cycle inicial
    config_timer_pwm();
    
    uint16_t adc_sample_counter = 0;
    
    while (1) {
        // Leer ADC cada ciertos ciclos
        if (adc_sample_counter++ > 1000) {  // Cada ~1000 ciclos PWM
            adc_sample_counter = 0;
            update_pwm_from_adc();
        }
        
        // Pequeño delay para estabilidad
        for (volatile uint32_t i = 0; i < 1000; i++);
    }
    
    return 0;
}

void config_GPIO(void) {
    PINSEL_CFG_Type pin_config;
    
    // Configurar P0.22 como GPIO (PWM)
    pin_config.portNum = PINSEL_PORT_0;
    pin_config.pinNum = PINSEL_PIN_22;
    pin_config.funcNum = PINSEL_FUNC_0;     // GPIO
    pin_config.pinMode = PINSEL_PULLUP;
    pin_config.openDrain = PINSEL_OD_NORMAL;
    
    PINSEL_ConfigPin(&pin_config);
    
    // Configurar P0.23 como ADC0 (Potenciómetro)
    pin_config.portNum = PINSEL_PORT_0;
    pin_config.pinNum = PINSEL_PIN_23;
    pin_config.funcNum = PINSEL_FUNC_1;     // ADC0
    pin_config.pinMode = PINSEL_TRISTATE;   // Alta impedancia para ADC
    pin_config.openDrain = PINSEL_OD_NORMAL;
    
    PINSEL_ConfigPin(&pin_config);
    
    // Configurar P0.22 como salida
    GPIO_SetDir(PWM_PORT, PWM_PIN, OUTPUT);
    
    // Inicializar en LOW
    pwm_output_low();
}

void config_ADC(void) {
    // Configuración ADC
    ADC_Init(LPC_ADC, ADC_FREQUENCY);
    
    // Habilitar canal 0
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL, ENABLE);
    
    // Configurar interrupción
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE);
    NVIC_EnableIRQ(ADC_IRQn);
    
    // Iniciar primera conversión
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);
}

void config_timer_pwm(void) {
    TIM_TIMERCFG_Type timer_config;
    TIM_MATCHCFG_Type match0_config, match1_config;
    
    // Configuración del timer
    timer_config.prescaleOption = TIM_USVAL;    // Microsegundos
    timer_config.prescaleValue = 1;             // 1 μs por tick
    
    // MATCH 0 - Inicio del período PWM (reset del timer)
    match0_config.matchChannel = 0;
    match0_config.intOnMatch = ENABLE;
    match0_config.resetOnMatch = ENABLE;        // Reset timer al inicio del período
    match0_config.stopOnMatch = DISABLE;
    match0_config.extMatchOutputType = TIM_NOTHING;
    match0_config.matchValue = pwm_period;      // Período completo
    
    // MATCH 1 - Fin del tiempo ON
    match1_config.matchChannel = 1;
    match1_config.intOnMatch = ENABLE;
    match1_config.resetOnMatch = DISABLE;       // NO reset
    match1_config.stopOnMatch = DISABLE;
    match1_config.extMatchOutputType = TIM_NOTHING;
    match1_config.matchValue = pwm_on_time;     // Tiempo ON
    
    // Inicializar timer y configurar matches
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer_config);
    TIM_ConfigMatch(LPC_TIM0, &match0_config);
    TIM_ConfigMatch(LPC_TIM0, &match1_config);
    
    // Habilitar timer e interrupciones
    TIM_Cmd(LPC_TIM0, ENABLE);
    NVIC_EnableIRQ(TIMER0_IRQn);
}

void set_duty_cycle(uint8_t duty_percent) {
    // Limitar duty cycle entre 0% y 100%
    if (duty_percent > 100) duty_percent = 100;
    
    // Calcular tiempos
    pwm_on_time = (pwm_period * duty_percent) / 100;
    pwm_off_time = pwm_period - pwm_on_time;
    
    // Deshabilitar timer temporalmente
    TIM_Cmd(LPC_TIM0, DISABLE);
    
    // Reconfigurar Match 1 con nuevo tiempo ON
    TIM_MATCHCFG_Type match1_config;
    match1_config.matchChannel = 1;
    match1_config.intOnMatch = ENABLE;
    match1_config.resetOnMatch = DISABLE;
    match1_config.stopOnMatch = DISABLE;
    match1_config.extMatchOutputType = TIM_NOTHING;
    match1_config.matchValue = pwm_on_time;
    
    TIM_ConfigMatch(LPC_TIM0, &match1_config);
    
    // Rehabilitar timer
    TIM_Cmd(LPC_TIM0, ENABLE);
}

void update_pwm_from_adc(void) {
    // Iniciar nueva conversión ADC
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);
    
    // Esperar a que esté lista la conversión
    while (!adc_ready);
    adc_ready = 0;
    
    // Convertir valor ADC a duty cycle
    uint8_t new_duty = adc_to_duty_cycle(adc_value);
    set_duty_cycle(new_duty);
}

uint16_t read_adc(void) {
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);
    while (!adc_ready);
    adc_ready = 0;
    return adc_value;
}

uint8_t adc_to_duty_cycle(uint16_t adc_val) {
    // ADC de 12 bits: 0-4095
    // Mapear a duty cycle 0-100%
    uint32_t duty = (adc_val * 100) / 4095;
    
    // Limitar entre 0 y 100
    if (duty > 100) duty = 100;
    
    return (uint8_t)duty;
}

void pwm_output_high(void) {
    GPIO_ClearPins(PWM_PORT, PWM_PIN);  // LED encendido (active low)
}

void pwm_output_low(void) {
    GPIO_SetPins(PWM_PORT, PWM_PIN);    // LED apagado
}