#include <msp430.h>
#define LED1 BIT3
#define LED2 BIT4
#define B1 BIT3
#define CPUCLOCK 384500
#define REDLED BIT5
#define GREENLED BIT0


void main(void){
    WDTCTL = WDTPW | WDTHOLD;

    P2DIR |= (LED1 | LED2 | REDLED | GREENLED); //allocate pins with leds as outputs
    P2OUT &= ~(LED1 | LED2);//turn them off
    // P2OUT |= (REDLED | GREENLED);

    P1DIR &= ~B1; // set button low and 
    P1REN |= B1; //pull up resistor activate
    P1OUT |= B1;



    int flash_red = 0;

    //80 flashes per second - 1.3Hz, therefore period of 0.77s
    // then each on off is 0.77/2 = 0.384s
    // CPU clock of 1MHz so 0.384 *1e6 

    unsigned long i;

    for(;;){
        if ((P1IN & B1) == 0){
            // wait until the button is released
            while((P1IN & B1) == 0);

            P2OUT |= (REDLED| GREENLED); //turn on multiled
            for(i = 0; i<3; i++){
               __delay_cycles(1e6); //3s at 1Mhz
            } 
            P2OUT &= ~(REDLED| GREENLED); //turn off multiled
            
            flash_red = !flash_red; //toggle
        }

            

            if (flash_red){
                     P2OUT ^= (LED1 | LED2);// flip the bit
                    __delay_cycles(CPUCLOCK);
                    flash_red = 1;
            }
            else{
                P2OUT &= ~(LED1 | LED2); //turn them off
            }


        
        
    }

}
