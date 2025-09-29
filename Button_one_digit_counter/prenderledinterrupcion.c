#incllude "LPC17xx.h"
#define DELAY 2500
void configuracionPuertos();
void configuracionInterrupcion();
void delay();
#define LED 0  // PIN 0.5 ESTARA EL LED UNA SALIDA
#define entrada_puerto 0
#define entrada_ext 0
#define frecuencia = 1000000

int main(void){
    configuracionPuertos();
    configuracionInterrupcion();
       SysTick_Config(frecuencia);

        while(1){


        }
    return 0;
}
void configuracionPuertos(){
    // pin parpadeanto . puerto 0

    //PONGO UN 00 GPIO PARA P0.0
    LPC_PINCON->PINSEL0 &= ~(ox3);
    //LED COMO SALIDA P0.0
    LPC_GPIO0 ->FIODIR |=0X3;
// al ser salida no hace falta tocar las pull up
  
  
//esto no hace falta debo asumir que por defecto viene configurado
    //ENTRADA PUERTO 2.0
    LPC_PINCON->PINSEL4 &= ~(0X3);
    LPC_GPIO2 ->FIODIR &=~(0X3);//ENTRADA

    //ENTRADA EXTERNA 2.  PINSEL FUNCION EXT2 01
    //externa solo hay 4 pune corroborar eso no es ext2.0
    LPC_PINCON->PINSEL4 &= ~(0X1);//01
    LPC_GPIO2 ->FIODIR &=~(0X3);//ENTRADA

}

void configuracionInterrupcion(){
 
    LPC_GPIOINT->IO2IntEnR |=1; //INDICO QUE INTERRUPA POR FLANCO DE SUBIDA
    NVIC_EnableIRQ(EINT3_IRQN);//HABILITO LA INTERRUPCION EXTERNA 3



    LPC_SC-EXTMODE |=1<<2;
    LPC_SC->EXTPOLAR 1=1<<2;
    //
    NVOC_CLEAR PENDIG
    
}
void SysTick_Handler(void){

    //prender y apagar led
    LPC_GPIO0-> FIOCLR =0;//PREDER
    void delay();
    LPC_GPIO->FIOSET =0;//APAGAR
    void delay();
    //limpio bandera
    LPC_GPIOINT
}

//handler de a interrupcion externa 2
void EINT2_IRQHandler(void){


    //voolver a parpadear
    LPC_GPIO0-> FIOCLR =0;//PREDER

    LPC_GPIOINT->IO2IntClr = 0;//limpio flag
}

//porque puerto 2.x el handler que usa las interrupcines 
//por puertos es el handler 3
void EINT3_IRQHandler(void){
    //apagar led
    LPC_GPIO->FIOSET =0;//APAGAR

    LPC_GPIOINT->IO3IntClr =  0;// limpio flag
}

void delay() {
    for(uint32_t i=0; i<DELAY; i++)
        for(uint32_t j=0; j<DELAY; j++);
}