# 🛠️ RESUMEN DRIVERS LPC17xx

## 📌 PINSEL - Configuración de Pines

### Funciones Principales
| Función | Descripción |
|---------|-------------|
| `PINSEL_ConfigPin(pinCfg)` | Configura un pin según estructura pinCfg (función, modo, open-drain) |
| `PINSEL_ConfigMultiplePins(pinCfg, pins)` | Configura múltiples pines con misma config usando máscara de bits |
| `PINSEL_ConfigTraceFunc(newState)` | Habilita/deshabilita función de trace en pines especiales |
| `PINSEL_SetI2CPins(driveMode, filterSlewRate)` | Configura pines I2C con modo drive y control filtro/slew rate |

### Parámetros
- **pinCfg**: `PINSEL_CFG_Type*` (puerto, pin, función, modo, open-drain)
- **pins**: Máscara 32-bit (bit=1 configura pin correspondiente)
- **newState**: `ENABLE` o `DISABLE`
- **driveMode**: `PINSEL_I2C_NORMAL` o `PINSEL_I2C_FAST`

### Estructura PINSEL_CFG_Type
```c
typedef struct {
    uint8_t portNum;    // PINSEL_PORT_0..4
    uint8_t pinNum;     // PINSEL_PIN_0..31
    uint8_t funcNum;    // PINSEL_FUNC_0..3 (0=GPIO)
    uint8_t pinMode;    // PINSEL_PULLUP/TRISTATE/PULLDOWN
    uint8_t openDrain;  // PINSEL_OD_NORMAL/OPENDRAIN
} PINSEL_CFG_Type;
```

---

## 🔌 GPIO - Control de Pines Digitales

### Funciones Básicas
| Función | Descripción |
|---------|-------------|
| `GPIO_SetDir(port, pins, dir)` | Configura dirección entrada/salida |
| `GPIO_SetPins(port, pins)` | Pone en HIGH los pines especificados |
| `GPIO_ClearPins(port, pins)` | Pone en LOW los pines especificados |
| `GPIO_WriteValue(port, value)` | Escribe valor completo al puerto |
| `GPIO_ReadValue(port)` | Lee estado de todos los pines |
| `GPIO_TogglePins(port, pins)` | Invierte estado de pines |

### Funciones de Interrupción
| Función | Descripción |
|---------|-------------|
| `GPIO_IntCmd(port, pins, edge)` | Habilita interrupciones en pines |
| `GPIO_GetPortIntStatus(port)` | Estado interrupción del puerto |
| `GPIO_ClearInt(port, pins)` | Borra flags de interrupción |

### Ejemplo de Uso
```c
GPIO_SetDir(GPIO_PORT_0, 0x00400000, GPIO_OUTPUT);  // P0.22 salida
GPIO_SetPins(GPIO_PORT_0, 0x00400000);              // P0.22 = HIGH
GPIO_ClearPins(GPIO_PORT_0, 0x00400000);            // P0.22 = LOW
```

---

## ⏰ SYSTICK - Timer del Sistema

### Funciones
| Función | Descripción |
|---------|-------------|
| `SYSTICK_InternalInit(time)` | Inicializa con clock CPU y tiempo en ms |
| `SYSTICK_Cmd(newState)` | Habilita/deshabilita contador |
| `SYSTICK_IntCmd(newState)` | Habilita/deshabilita interrupción |
| `SYSTICK_HasFired()` | Verifica si timer disparó |
| `SYSTICK_SetReloadValue(ticks)` | Establece valor de recarga |

### Ejemplo de Uso
```c
SYSTICK_InternalInit(1);    // SysTick cada 1ms
SYSTICK_IntCmd(ENABLE);     // Habilitar interrupción
SYSTICK_Cmd(ENABLE);        // Iniciar SysTick
```

---

## ⏱️ TIMER - Timers Generales

### Funciones Principales
| Función | Descripción |
|---------|-------------|
| `TIM_Init(TIMx, mode, config)` | Inicializa timer con configuración |
| `TIM_ConfigMatch(TIMx, matchConfig)` | Configura canal match |
| `TIM_Cmd(TIMx, newState)` | Habilita/deshabilita timer |
| `TIM_GetIntStatus(TIMx, flag)` | Estado de interrupción |
| `TIM_ClearIntPending(TIMx, flag)` | Borra flag de interrupción |

### Ejemplo de Uso
```c
TIM_TIMERCFG_Type cfg = {TIM_USVAL, 1};              // 1μs por tick
TIM_MATCHCFG_Type match = {0, ENABLE, ENABLE,        // Match0
                          DISABLE, TIM_NOTHING, 1000}; // 1ms

TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &cfg);
TIM_ConfigMatch(LPC_TIM0, &match);
TIM_Cmd(LPC_TIM0, ENABLE);
```

---

## 📊 ADC - Conversor Analógico-Digital

### Funciones
| Función | Descripción |
|---------|-------------|
| `ADC_Init(rate)` | Inicializa ADC con frecuencia |
| `ADC_ChannelCmd(channel, state)` | Habilita canal específico |
| `ADC_IntConfig(channel, state)` | Configura interrupciones |
| `ADC_StartCmd(mode)` | Inicia conversión |
| `ADC_ChannelGetData(channel)` | Lee resultado de canal |

### Ejemplo de Uso
```c
ADC_Init(200000);                           // 200kHz
ADC_ChannelCmd(ADC_CHANNEL_2, ENABLE);      // Canal 2
ADC_IntConfig(ADC_ADINTEN2, ENABLE);        // Interrupción
ADC_StartCmd(ADC_START_NOW);                // Iniciar
valor = ADC_ChannelGetData(ADC_CHANNEL_2);  // Leer
```

---

## 📈 DAC - Conversor Digital-Analógico

### Funciones
| Función | Descripción |
|---------|-------------|
| `DAC_Init()` | Inicializa DAC en P0.26 |
| `DAC_UpdateValue(value)` | Actualiza valor (0-1023) |
| `DAC_SetBias(bias)` | Configura corriente/velocidad |

### Cálculo de Voltaje
```
Voltaje = (value / 1024) × 3.3V
```

### Ejemplo de Uso
```c
DAC_Init();                               // Inicializar
DAC_SetBias(DAC_MAX_CURRENT_700uA);      // Máxima velocidad
DAC_UpdateValue(512);                     // 1.65V (50%)
```

---

## 🚀 GPDMA - Controlador DMA

### Funciones
| Función | Descripción |
|---------|-------------|
| `GPDMA_Init()` | Inicializa controlador DMA |
| `GPDMA_Setup(config)` | Configura canal DMA |
| `GPDMA_ChannelCmd(ch, state)` | Habilita/deshabilita canal |
| `GPDMA_IntGetStatus(type, ch)` | Estado de interrupción |

### Tipos de Transferencia
- **M2M**: Memoria → Memoria
- **M2P**: Memoria → Periférico
- **P2M**: Periférico → Memoria
- **P2P**: Periférico → Periférico

### Ejemplo de Uso
```c
GPDMA_Channel_CFG_Type dmaConfig = {
    0,                           // Canal 0
    1024,                        // 1024 elementos
    GPDMA_WIDTH_WORD,           // 32-bit
    (uint32_t)src_buffer,       // Origen
    (uint32_t)dst_buffer,       // Destino
    GPDMA_TRANSFERTYPE_M2M,     // Memoria a memoria
    0, 0, 0                     // Sin periféricos
};

GPDMA_Init();
GPDMA_Setup(&dmaConfig);
GPDMA_ChannelCmd(0, ENABLE);
```

---

## 💡 Consejos de Uso

### ⚡ Rendimiento
- Usar **FIO** en lugar de **GPIO** para operaciones rápidas
- **DMA** para transferencias grandes de datos
- **SysTick** para delays precisos

### 🔧 Configuración Típica
1. **PINSEL**: Configurar función de pines
2. **GPIO**: Configurar dirección entrada/salida
3. **Periférico**: Inicializar ADC/DAC/TIMER
4. **Interrupciones**: Configurar NVIC si es necesario

### 📊 Valores Comunes
- **ADC**: 12-bit (0-4095), máx 200kHz
- **DAC**: 10-bit (0-1023), salida 0-3.3V
- **Timer**: Hasta 4 timers (TIM0-TIM3)
- **GPIO**: 5 puertos (P0-P4), 32 pines cada uno