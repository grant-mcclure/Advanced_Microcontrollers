#include <msp430.h>
#define RED BIT5
#define YELLOW BIT4
#define GREEN BIT3
#define BUZZER BIT0
#define GREENON 5000000 //5s at 1Mhz Clock
#define YELLOWON 2000000 //2s at 1Mhz Clock
#define REDON 5000000 //5s at 1Mhz Clock
#define BUTTONON 500000 //0.5s at 1MHz clock
#define SAMPLECYCLES 50000 //sample every 50ms at 1MHz
#define BUTTON BIT3

void traffic_signal(unsigned int GreenMask, unsigned int RedMask, unsigned int YellowMask, 
                     unsigned int ButtonMask, unsigned int BuzzerMask);
int button_pressed_edge(void);
void delay_and_poll(unsigned long cycles, unsigned int ButtonMask);
void BuzzerTimer(unsigned int BuzzerMask, unsigned int GreenMask);

volatile unsigned char pedRequested = 0; // the value of the char can change at anytime
volatile unsigned int i;

void main(void){
WDTCTL = WDTPW | WDTHOLD; //turn off watchdog timer

///make all the leds into an output
P2DIR |= (RED|YELLOW|GREEN|BUZZER);
//turn them off
P2OUT &= ~((RED|YELLOW|GREEN|BUZZER));

// set the button as input
P1DIR &= ~BIT3; //set button as input
P1REN |= BIT3; //pull up resistor
P1OUT |= BIT3; //out put 

for(;;){
   traffic_signal(GREEN, RED, YELLOW, BUTTON, BUZZER);
}

}

void traffic_signal(unsigned int GreenMask, unsigned int RedMask, unsigned int YellowMask, unsigned int ButtonMask, unsigned int BuzzerMask){
      //moving down logic
    P2OUT |= GreenMask;
    delay_and_poll(GREENON, ButtonMask); //check if button is pressed
    P2OUT &= ~GreenMask; //green off


    P2OUT |= (YellowMask | RedMask); //yellow and red on
    delay_and_poll(YELLOWON, ButtonMask); //check if button is pressed
    P2OUT &= ~(YellowMask|RedMask); //yellow and red off

    P2OUT |= RedMask;
     delay_and_poll(REDON, ButtonMask); //check if button is pressed

    if (pedRequested == 1){
      BuzzerTimer(BuzzerMask, GreenMask);
    }
    P2OUT &= ~RedMask;  //red off

    //moving back up

    P2OUT |= YellowMask; //yellow back on
     delay_and_poll(YELLOWON, ButtonMask); //check if button is pressed
    P2OUT &= ~YellowMask;

    //moving down logic
    P2OUT |= GreenMask;
     delay_and_poll(GREENON, ButtonMask); //check if button is pressed
    P2OUT &= ~GreenMask; //green off

}

// Returns 1 only on the instant the button is pressed (active low).
int button_pressed_edge(void){

   static unsigned char wasPressed = 0; //remembers last state
   unsigned char now = !(P1IN & BUTTON); //remembers when button was pressed

   if (now && !wasPressed) { //transition up down
    wasPressed =1;
    return 1;
   }
   if(!now){
      //button released
      wasPressed = 0;
   }
   return 0;
}

//sample the button more often in otder to constatly check for presses
void delay_and_poll(unsigned long cycles, unsigned int ButtonMask){
   const unsigned long step = 50000; //50ms at 1Mhz
   unsigned long elapsed = 0;

   while(elapsed<cycles){
      __delay_cycles(step);
      elapsed += step;
      if (button_pressed_edge()){
         pedRequested =1;
      }
   }
}

void BuzzerTimer(unsigned int BuzzerMask, unsigned int GreenMask){
for(i = 0; i< 10; i++){ //allow pedesrtian 10s to cross
      P2OUT |= (BuzzerMask | GreenMask);
      __delay_cycles(BuzzerOn);
      P2OUT &= ~(BuzzerMask|GreenMask);
      __delay_cycles(BuzzerOn);
      }
      P2OUT &= ~BuzzerMask;
      pedRequested = 0; //clear the request

}