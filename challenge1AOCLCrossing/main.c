#include <msp430.h>
#define LED1 BIT3
#define LED2 BIT4
#define B1 BIT3


void main(void){
    WDTCTL = WDTPW | WDTHOLD;

    P2DIR |= (LED1 | LED2); //allocate pins with leds as outputs
    P2OUT |= (LED1 | LED2);//turn them off

    P1DIR &= ~B1; // set button low and 
    P1REN |= B1; //pull up resistor activate
    P1OUT |= B1;

    for(;;){
        if((P1IN & B1) == 0){
            P2OUT |= (LED1 | LED2);//turn them on
        }
        else{
            P2OUT &= ~(LED1 | LED2);//turn them off
        }
        
    }

}
