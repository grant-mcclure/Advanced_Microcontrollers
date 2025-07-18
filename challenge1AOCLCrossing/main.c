#include <msp430.h>
#define LED1 BIT3
#define LED2 BIT4
#define B1 BIT3
#define CPUCLOCK 384500


void main(void){
    WDTCTL = WDTPW | WDTHOLD;

    P2DIR |= (LED1 | LED2); //allocate pins with leds as outputs
    P2OUT &= ~(LED1 | LED2);//turn them on

    P1DIR &= ~B1; // set button low and 
    P1REN |= B1; //pull up resistor activate
    P1OUT |= B1;

    int led_state = 0;

    //80 flashes per second - 1.3Hz, therefore period of 0.77s
    // then each on off is 0.77/2 = 0.384s
    // CPU clock of 1MHz so 0.384 *1e6 

    for(;;){
        if ((P1IN & B1) == 0){
            // wait until the button is released
            while((P1IN & B1) == 0);

            //toggle state
            led_state = !led_state; 

            if (led_state){
                while(!(P1IN & B1) == 0){
                     P2OUT ^= (LED1 | LED2);//turn them on
                    __delay_cycles(CPUCLOCK);
                }
                   
            }
            else{
                P2OUT &= ~(LED1 | LED2);//turn them on
            }


        }
        
    }

}
