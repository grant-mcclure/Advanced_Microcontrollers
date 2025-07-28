        .cdecls C,LIST,"msp430.h"   ; give us the register names
        .text
        .global RESET               ; entry symbol

RESET:  mov.w   #WDTPW|WDTHOLD, &WDTCTL   ; stop watchdog

        ; --- GPIO setup ---
        bis.b   #BIT0, &P1DIR       ; P1.0 = output (LED on LaunchPad)
        bic.b   #BIT0, &P1OUT       ; start with LED off

Loop:   xor.b   #BIT0, &P1OUT       ; toggle LED
        mov.w   #20000, R15         ; crude delay (~20 k cycles)
Delay:  dec.w   R15
        jnz     Delay
        jmp     Loop

        ; reset vector so MCU knows where RESET label is
        .sect   ".reset" 
        .short  RESET
