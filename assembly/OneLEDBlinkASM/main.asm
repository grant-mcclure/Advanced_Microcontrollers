    .cdecls C,LIST,"msp430.h"
    .text
    .global Reset

Reset:
    mov.w   #WDTPW|WDTHOLD, &WDTCTL    ; Stop watchdog timer
    bis.b   #0x08, &P2OUT              ; Set P2.3 high (turn on LED if wired)
Loop:
    jmp     Loop                       ; Infinite loop

    .sect   ".reset"
    .short  Reset
    