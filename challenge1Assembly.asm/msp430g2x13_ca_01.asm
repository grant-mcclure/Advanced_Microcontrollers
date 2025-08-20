    .cdecls C, LIST, "msp430.h"
    .text
    .global RESET

RESET mov.w #WDTPW | WDTHOLD, &WDTCTL

; setup the GPIO

;set the two red leds to output
bis.b #BIT3 &P2DIR ;set p2.3 to output
bis.b #BIT4 &P2DIR ;set p2.4 to output

;set the yellow id to outpit
bis.b #BIT5 &P2DIR

; turn all the LEDs off
bic.b #BIT3 &P2OUT
bic.b #BIT4 &P2OUT
bic.b #BIT5 &P2OUT