#include <msp430.h>
#define REDLED BIT3
#define B1 BIT3
#define TIMERCLOCKS 750000
#define OVERFLOWS30s 458 //458 overflows for 30s,

volatile unsigned int of_count = 0;

void main(void){

    WDTCTL = WDTPW | WDTHOLD; // turn of watch dod timer

    P2DIR |= REDLED; //make pin2.3 an output
    P2OUT &= ~REDLED; //off

    P1DIR &= ~B1; // this make the button an input
    P1REN |= B1;
    P1OUT |= B1;

        //flashes Period of 1.5s then the led is on for 0.75s
        //cpu clock is 1MHz * 0.75 = 

     while(!(P1IN & B1) == 0){
            P2OUT |= REDLED;
            __delay_cycles(TIMERCLOCKS); //0.75s
            P2OUT &= ~REDLED;
            __delay_cycles(TIMERCLOCKS);
        }
         P2OUT &= ~REDLED; //turn off after the button is pressed

    // now we wait 30s, so we know at 1MHz the timerA will overflow every 65ms
    // therefore we can say 30s/65ms = 458 overflows in 30s

    // set up timer A
    TA0CTL  = TASSEL_2 | MC_2 | TACLR | TAIE; //// SMCLK, continuous mode, clear TAR, enable overflow int

    //enable interrupts
    __enable_interrupt(); // Enable global interrupts

    while(of_count < OVERFLOWS30s){
        // let the interrupts do everythin
    }

    // after we wait 30s start flashing at 40 flashes per minute, which is roughly the same as we had before
    // 40 flasher per min = 0.67 Hz, T = 1.49s then LED on time is aprox 0.75s
    for(;;){
        P2OUT |= REDLED;
        __delay_cycles(TIMERCLOCKS); //0.75s
        P2OUT &= ~REDLED;
        __delay_cycles(TIMERCLOCKS);
    }
   

    
    
}
// timer a0 interrupt routine
#pragma vector=TIMER0_A1_VECTOR

__interrupt void Timer_A(void){
    if(TA0IV == 0x0A){
        of_count++;
    }
}