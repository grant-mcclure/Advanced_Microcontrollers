#include <msp430.h>
#define LED1 BIT3 //0x04
#define LED2 BIT4 // 0x08
#define B1 BIT3 // 0x01

void main(void){

    WDTCTL = WDTPW | WDTHOLD;

    P2DIR |= LED1 | LED2; // assign the LED pins as output

    P2OUT |= (LED1 |LED2); // drive both LEDs low to turn on

    //set logic for the botton press
    P1DIR &= ~B1; //set 1.1 as input
    P1REN |= B1; //enable the resistor
    P1OUT |= B1; //pull-up so unpressed = active high

    for(;;){
        if(!(P1IN & B1) == 0){ //is button not pressed then turn on the lights
            P2OUT &= ~LED1; // turn on
            P2OUT &= ~LED2; //turn on
            
        }
        else{
            P2OUT |= LED1;
            P2OUT |= LED2;
        }

        // __delay_cycles(5e6);
    }

}

