#include <msp430.h>

void main(void){
    //stop the watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // given the pin 2.3 has an led attached want to put to active low
    P2DIR |= BIT3 | BIT4; //set the 2.3 pin to be an output
    //now switch the LED off
    // P2OUT |= BIT3;//swtich p2.3 to active high
    // P2OUT |= BIT4;


    //make it blink 

    while(1){
        P2OUT &= ~BIT3;
        __delay_cycles(5e5);
        P2OUT &= ~BIT4; //TURN OFF

        P2OUT |= BIT3;
        __delay_cycles(5e5);
        P2OUT |= BIT4;

        // __delay_cycles(5e5);

    }
} 