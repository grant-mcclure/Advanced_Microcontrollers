#include <msp430.h>

#define RED BIT2
#define YELLOW BIT3
#define FLASH1 BIT4
#define FLASH2 BIT5
#define BUTTON BIT3
#define FLASHDELAY 375000 // 0.375s at 1MHz
#define YELLOWDELAY 3000000 // 3s at 1MHz

// Weeks 1-3 Challenge
// Implement the traffic lights for an AOCL level crossing. Use the red LED and the multi-colour LED to show a yellow light and then two flashing red lights.
// The sequence should trigger when switch P1.3 is pressed, and continue for as long as the switch is depressed
// A yellow light should show for 3 seconds from the time the switch is depressed
// Two red LEDs should then flash at a rate of 80 flashes/minute
// Include a design in your log book (design pages from log book should be uploaded by end of week 2)
// Demonstration should take place in the lab in Week 3

void FlashLEDs(unsigned int LED1, unsigned int LED2);
void yellow_led_on(unsigned int YellowLED);

void main(void){

    WDTCTL = WDTPW | WDTHOLD;
    //set up the outputs for the LEDs
    P2DIR |= (RED | YELLOW | FLASH1 | FLASH2); //set the inputs
    P2OUT &= ~(RED | YELLOW | FLASH1 | FLASH2);

    P1DIR &= ~(BUTTON); //set button to input
    P1REN |= BUTTON;
    P1OUT |= BUTTON; //pull up

    // logic so check first if the button is depressed

    for(;;){
        if((P1IN & BUTTON) == 0){
            yellow_led_on(YELLOW); //3 secs yellow
            while ((P1IN & BUTTON) == 0) {
                
                __delay_cycles(50000); //let catch up
                FlashLEDs(FLASH1, FLASH2);
            
            }

            //when released ensure they are off
            P2OUT &= ~(RED | YELLOW | FLASH1 | FLASH2);

        }
    }


}

void FlashLEDs(unsigned int LED1, unsigned int LED2){
    // we want LEDs to flash at 80 flashes per minute, f = N/t = 80/60 = 1.33Hz, T = 1/f = 0.75s
    // but time off = 0.75/2 = 0.375s of on time
    // then since the clock is 1MHz the CPUCLOCK should be 0.375 * 1MHz

    P2OUT |= (LED1| LED2); //on
    __delay_cycles(FLASHDELAY);
    P2OUT &= ~(LED1 | LED2); //turn off
    __delay_cycles(FLASHDELAY);
        
}

void yellow_led_on(unsigned int YellowLED){

    // need to turn on the yellow led for 3 seconds then turn off
    // The yellow led will have a period of three seconds, three seconds at 1Mhz
        P2OUT |= (YellowLED); //turn yellow on
        __delay_cycles(YELLOWDELAY);
        P2OUT &= ~(YellowLED); //turn off

}

    


