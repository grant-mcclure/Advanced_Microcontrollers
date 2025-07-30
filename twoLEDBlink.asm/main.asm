    .cdecls C, LIST, "msp430.h" ; give the register names
    .text
    .global RESET ; entry symbol

RESET mov.w #WDTPW | WDTHOLD, &WDTCTL ;stop watchdog

Loop: xor.b #BIT3, &P2OUT ; run xor ie if On(1) -> off(0 and vice versa)
      mov.w #20000, R15 ; move the value 20000 into registerR15, the setup software delay loop

