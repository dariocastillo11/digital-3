#define DELAY_TIME 100000
// LED 3.25
void delay();
void configGPIO();
int main(void){
    configGPIO();
    WHILE(1){
        LPC_GPIO03->FIOCLR = (1 << 25); //P3.25 ENCIENDO
        delay();
        LPC_GPIO03->FIOSET = (1 << 25); //P3.25 APAGO
        delay();
    }
}
void delay(){
    for (volatile int i = 0; i < DELAY_TIME; i++);
}

void configGPIO(){
    //3.25 DEBO ocnfigurar pines 18 y 19
    //REGISTRO  7 Y 8 SON DEL PORT 3
    ////xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
    //DESPLAZO b11  18 LUGARES PARA QUE QUEDEN LOS PINES 18 Y 19 EN 1
    //0000 0000 0000 0110 0000 0000 0000 0000
    //INVIERTO 
    //1111 1111 1111 1001 1111 1111 1111 1111
    //
    //USO AND 
    //xxxx xxxx xxxx x00x xxxx xxxx xxxx xxxx //ME QUEDAA ASI
    LPC_PINCON->PINSEL7 &= ~(0b11 << 18); //P3.25 COMO GPIO
    //xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
    //0000 0000 0000 0000 0110 0000 0000 0000
    //USO or .. para poner unos 1 no se invierte
    //xxxx xxxx xxxx xxxx x11x xxxx xxxx xxxx //ME QUEDAA ASI
    LPC_GPIO03->FIODIR |= (1 << 25); //P3.25 COMO SALIDA

    LPC_GPIO03->FIOSET = (1 << 25); //P3.25 APAGO
}