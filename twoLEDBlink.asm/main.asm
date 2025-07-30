    .cdecls C, LIST, "msp430.h" ; give the register names
    .text
    .global RESET ; entry symbol

RESET mov.w #WDTPW | WDTHOLD, &WDTCTL ;stop watchdog

; setup GPIO
        bis.b #BIT3, &P2DIR ;;set bit 3 to output
        bic.b #BIT3, &P2OUT  ; turn LED off
        bis.b #BIT4, &P2DIR ;;set bit 4 to output
        bic.b #BIT4, &P2OUT  ; turn LED off



Loop: xor.b #BIT3, &P2OUT ; run xor ie if On(1) -> off(0 and vice versa)
      xor.b #BIT4, &P2OUT ;;same here
      mov.w #30000, R15 ; move the value 50000 into registerR15, the setup software delay loop

Delay1: dec.w R15 ; find the number in the R15 register then decrement by this
        jnz Delay1 ; jump if not 0, if R15 != 0 keep going until zero

        bic.b #BIT4, &P2OUT ; turn off
        bis.b #BIT3, &P2OUT ; turn on 

        mov.w #30000, R15

Delay2: 
        dec.w R15
        jnz Delay2
        jmp Loop

    .sect ".reset"           ; place this in the reset‐vector section
    .short RESET             ; 0xFFFE ← address of RESET
    .end

