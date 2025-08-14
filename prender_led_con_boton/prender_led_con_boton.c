#include "LPC17xx.h"

void configurar_pines();
void leer_pin_P2_1();

int main(void){
    configurar_pines();
    while(1) {
        leer_pin_P2_1();
    }
    return 0;
}
void configurar_pines(){
    // P2.1 Y P2.0 COMO MODO GPIO
    //11  11 --->invierto 00 00 -->and 00 00 force un 0
    LPC_PINCON->PINSEL4 &= ~(0X33);
    //ACTIVO PULL DOWN EN P2.1 Y EN P2.0 SIN RESISTENCIA
    LPC_PINCON->PINMODE4 |= (0x3<<4);
    //SELECCIONO  P2.0 COMO SALIDA
    LPC_GPIO4->FIODIR |= (0x1);
    //SELECCIONO P2.1 COMO ENTRADA
    LPC_GPIO4->FIODIR &= ~(0x1<<1);
}
void leer_pin_P2_1(){
    //LER P0.1
    //LPC->FIOPIN =(1<<1);
    if(LPC_GPIO4->FIOPIN &(1<<1) ){
        //ACTIVO P2.1 EN ALTO
        LPC_GPIO4->FIOSET = (0x1);
    }
    else{
        //DESACTIVO P2.1 EN BAJO. 
        LPC_GPIO4->FIOCLR = ~(0x1);
    }

}



