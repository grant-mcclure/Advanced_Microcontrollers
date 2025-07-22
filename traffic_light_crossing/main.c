#include <msp430.h>
#define RED BIT5
#define YELLOW BIT4
#define GREEN BIT3
#define BUZZER BIT0
#define GREENON 5000000 //5s at 1Mhz Clock
#define YELLOWON 2000000 //2s at 1Mhz Clock
#define REDON 5000000 //5s at 1Mhz Clock

void main(void){
WDTCTL = WDTPW | WDTHOLD; //turn off watchdog timer

///make all the leds into an output
P2DIR |= (RED|YELLOW|GREEN|BUZZER);
//turn them off
P2OUT &= ~((RED|YELLOW|GREEN|BUZZER));

for(;;){
    P2OUT |= GREEN;
    __delay_cycles(GREENON); // green led on for 5as
    P2OUT |= YELLOW;
    __delay_cycles(YELLOWON); // yellow led on for 2as
    P2OUT |= RED;
    __delay_cycles(REDON); // green led on for 5as
    
}








}

//set up timer A routine
#pragma vector=TIMER0_A1_VECTOR

__interrupt void Timer_A(void){
    if(TA0IV == 0x0A){
        of_count++;
    }
}
