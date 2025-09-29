#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_dac.h"     

#define SALIDA          (uint8_t) 1
#define ENTRADA           (uint8_t) 0

#define PIN_22          ((uint32_t)(1<<22))

#define PORT_CERO       (uint8_t) 0
#define PORT_UNO        (uint8_t) 1
#define PORT_DOS        (uint8_t) 2
#define PORT_TRES    (uint8_t) 3

// Configuración de la onda
#define NUMERO_MUESTRAS    128          // Número de muestras en la tabla
#define MAXIMO_VALOR_DAC   1023        // 10-bit DAC (0-1023)
#define FRECUENCIA_DE_MUESTREO_US  78         
/*
FORMULA PARA FRECUENCIA_DE_MUESTRO_US
 10ms ÷ 128 = 0.078125ms = 78.125μs
*/
// - SEÑAL TRIANGULAR
const uint16_t TABLA_DE_LA_SEÑAL_TRIANGULAR[NUMERO_MUESTRAS] = {
    0,    16,   32,   48,   64,   80,   96,   112,  128,  144,  160,  176,  192,  208,  224,  240,
    256,  272,  288,  304,  320,  336,  352,  368,  384,  400,  416,  432,  448,  464,  480,  496,
    512,  528,  544,  560,  576,  592,  608,  624,  640,  656,  672,  688,  704,  720,  736,  752,
    768,  784,  800,  816,  832,  848,  864,  880,  896,  912,  928,  944,  960,  976,  992,  1008,
    1023, 1008, 992,  976,  960,  944,  928,  912,  896,  880,  864,  848,  832,  816,  800,  784,
    768,  752,  736,  720,  704,  688,  672,  656,  640,  624,  608,  592,  576,  560,  544,  528,
    512,  496,  480,  464,  448,  432,  416,  400,  384,  368,  352,  336,  320,  304,  288,  272,
    256,  240,  224,  208,  192,  176,  160,  144,  128,  112,  96,   80,   64,   48,   32,   16
};
volatile uint8_t INDICE_TABLA_DE_ONDA = 0;  // Índice actual en la tabla

void config_GPIO(void);
void config_timer(void);
void config_DAC(void);

void TIMER0_IRQHandler(void){
    if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {
        // LIMPIO FLAG DE INTERRUPCION
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        
        // Escribir valor actual de la onda al DAC
        DAC_UpdateValue(TABLA_DE_LA_SEÑAL_TRIANGULAR[INDICE_TABLA_DE_ONDA]);
        
        // Avanzar al siguiente punto de la onda
        INDICE_TABLA_DE_ONDA++;
        if(INDICE_TABLA_DE_ONDA >= NUMERO_MUESTRAS) {
            INDICE_TABLA_DE_ONDA = 0;  // Reiniciar al principio de la onda
        }
        
        // PARPADEAR LED
        static uint16_t led_counter = 0;
        if(++led_counter >= 500) {  // Toggle cada 500 interrupciones
            led_counter = 0;
            if(GPIO_ReadValue(PORT_CERO) & PIN_22) {
                GPIO_ClearPins(PORT_CERO, PIN_22);
            } else {
                GPIO_SetPins(PORT_CERO, PIN_22);
            }
        }
    }
    return;
}

int main(void) {
    config_GPIO();
    config_DAC();   
    config_timer();
    
    // Inicializar LED
    GPIO_SetPins(PORT_CERO, PIN_22);

    while(1) {
    }
    
    return 0;
}

void config_GPIO(){
    PINSEL_CFG_Type pin_configuration;
  
    // Configurar P0.22 como GPIO (LED)
    pin_configuration.portNum   = PINSEL_PORT_0;
    pin_configuration.pinNum    = PINSEL_PIN_22;
    pin_configuration.pinMode   = PINSEL_PULLUP;
    pin_configuration.funcNum   = PINSEL_FUNC_0;  // GPIO
    pin_configuration.openDrain = PINSEL_OD_NORMAL;

    PINSEL_ConfigPin(&pin_configuration);
    
    //  Configurar P0.26 como AOUT (salida DAC)
    pin_configuration.portNum   = PINSEL_PORT_0;
    pin_configuration.pinNum    = PINSEL_PIN_26;  // Pin del DAC
    pin_configuration.funcNum   = PINSEL_FUNC_2;  // AOUT función
    pin_configuration.pinMode   = PINSEL_TRISTATE;
    pin_configuration.openDrain = PINSEL_OD_NORMAL;
    
    PINSEL_ConfigPin(&pin_configuration);

    GPIO_SetDir(PORT_CERO, PIN_22, SALIDA);
    return;
}

void config_DAC(void) {
    DAC_Init();
    // Configurar para máxima velocidad
    DAC_SetBias(DAC_MAX_CURRENT_700uA);
    
    // Valor inicial
    DAC_UpdateValue(0);
}

void config_timer(){
    TIM_TIMERCFG_Type configuracion_de_timer;
    TIM_MATCHCFG_Type configuracion_de_match;

    // Timer en microsegundos
    configuracion_de_timer.prescaleOption = TIM_USVAL;
    configuracion_de_timer.prescaleValue  = 1;

    // interrupé match  cada 100μs = 10kHz frecuencia de muestreo
    configuracion_de_match.matchChannel        = 0;
    configuracion_de_match.intOnMatch          = ENABLE;
    struct_match.resetOnMatch        = ENABLE;
    struct_match.stopOnMatch         = DISABLE;
    struct_match.extMatchOutputType  = TIM_NOTHING;
    struct_match.matchValue          = FRECUENCIA_DE_MUESTREO_US; 

    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &configuracion_de_timer);
    TIM_ConfigMatch(LPC_TIM0, &struct_match);
    TIM_Cmd(LPC_TIM0, ENABLE);
    
    NVIC_EnableIRQ(TIMER0_IRQn);

    return;
}