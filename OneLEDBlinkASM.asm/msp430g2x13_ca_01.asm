        .cdecls C,LIST,"msp430.h"   ; give us the register names
        .text
        .global RESET               ; entry symbol

RESET:  mov.w   #WDTPW|WDTHOLD, &WDTCTL   ; stop watchdog

Loop:   xor.b   #BIT3, &P2OUT       ; toggle LED
        mov.w   #200000, R15         ; crude delay (~20 k cycles)
Delay:  dec.w   R15
        jnz     Delay
        jmp     Loop

        ; reset vector so MCU knows where RESET label is
        .sect   ".reset" 
        .short  RESET
