.cdecls C,LIST,"msp430.h"
.text
.global Reset

Reset:
    mov.w   #WDTPW|WDTHOLD, &WDTCTL
    bis.b   #BIT3,     &P2DIR    ; P1.3 output
Loop:
    xor.b   #BIT3,     &P2OUT    ; toggle P1.3
    call    #Delay
    jmp     Loop

Delay:
    mov.w   #50000, R15
D1:
    dec.w   R15
    jnz     D1
    ret

.sect   ".reset"
.short  Reset
