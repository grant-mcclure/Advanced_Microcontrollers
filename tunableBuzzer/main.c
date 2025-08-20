#include <msp430.h>                 // Device register definitions for MSP430 family

volatile unsigned int adc_value = 0; // Global updated by the ADC ISR (0..1023 for 10-bit ADC)

/*--------------------------- CLOCKS -----------------------------------------
 * We use the factory calibration constants to set the DCO so that:
 *  - MCLK  (CPU clock)  ≈ 1 MHz
 *  - SMCLK (subsystem)  ≈ 1 MHz (derived from DCO by default)
 * ACLK is left at default (typically 12 kHz VLO if no crystal).
 *---------------------------------------------------------------------------*/
static inline void configure_clocks(void) {
    BCSCTL1 = CALBC1_1MHZ;          // Basic Clock System Control 1: load 1 MHz coarse trim
    DCOCTL  = CALDCO_1MHZ;          // DCO Control:             load 1 MHz fine trim
}

/*----------------------------- ADC10 ----------------------------------------
 * We read a potentiometer on P1.4 (analog input channel A4).
 * - ADC10CTL1 selects input channel and ADC clock divider.
 * - ADC10CTL0 configures sample-and-hold time, powers the ADC, and enables its interrupt.
 * - ADC10AE0 enables the analog function on the chosen pin (disconnects digital buffer).
 *---------------------------------------------------------------------------*/
static inline void configure_adc(void) {
    ADC10CTL1 = INCH_4 |            // INCH_4: select channel A4  (pin P1.4)
                ADC10DIV_3;         // Divide ADC clock by 8 (optional; helps settling)
    ADC10CTL0 = ADC10SHT_3 |        // 64 × ADC10CLK sample-and-hold window (long, stable)
                ADC10ON     |       // Power up the ADC core
                ADC10IE;            // Enable ADC conversion complete interrupt
    ADC10AE0 |= BIT4;               // Enable analog input on P1.4 (A4)
}

/*---------------------------- TIMER_A0 PWM ----------------------------------
 * We output a square wave on P1.2 using Timer_A0 CCR1 (TA0.1).
 * - P1DIR/P1SEL route the timer output to the pin.
 * - TA0CCTL1 sets the output mode (Reset/Set gives a clean 50% square).
 * - TA0CTL selects clock source and mode (SMCLK @ 1 MHz, Up mode).
 * CCR0 = period - 1, CCR1 = duty time (here half of CCR0 for 50%).
 *---------------------------------------------------------------------------*/
static inline void configure_timer(void) {
    P1DIR |= BIT2;                  // Make P1.2 an output
    P1SEL |= BIT2;                  // Select TA0.1 function on P1.2 (instead of GPIO)
    TA0CCTL1 = OUTMOD_7;            // Output mode 7: Reset/Set (PWM: low->high at CCR1, high->low at CCR0)
    TA0CTL   = TASSEL_2 |           // TASSEL_2: timer clock source = SMCLK (≈1 MHz)
               MC_1;                // MC_1: Up mode (count 0 → CCR0 → 0 ...)
}

/*----------------------- FREQUENCY SETTER (PWM) -----------------------------
 * Maps a desired frequency (Hz) to Timer_A counts assuming SMCLK = 1 MHz.
 * Limits the frequency to a safe buzzer range.
 * period_counts = (SMCLK_Hz / freq_hz).
 *---------------------------------------------------------------------------*/
static inline void set_frequency(unsigned int freq_hz) {
    if (freq_hz < 50)   freq_hz = 50;    // Clamp lower bound (avoid inaudible/very long periods)
    if (freq_hz > 4000) freq_hz = 4000;  // Clamp upper bound (avoid harsh drive)

    unsigned int period = (unsigned int)(1000000UL / freq_hz); // 1e6 counts per second @ 1 MHz
    TA0CCR0 = period - 1;               // PWM period register (top count)
    TA0CCR1 = period / 2;               // 50% duty cycle: toggle halfway through the period
}

/*--------------------------------- MAIN -------------------------------------
 * 1) Stop the watchdog.
 * 2) Configure clocks, ADC, and timer.
 * 3) Enable interrupts globally.
 * 4) Loop:
 *    - Trigger an ADC conversion, enter LPM0 (CPU off, SMCLK active),
 *      and wait for the ADC ISR to wake us.
 *    - Map adc_value (0..1023) to a frequency (200..2000 Hz),
 *      then update the timer period/duty.
 *---------------------------------------------------------------------------*/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer (prevents unwanted resets)

    configure_clocks();
    configure_adc();
    configure_timer();

    __enable_interrupt();           // Global interrupt enable (same as __bis_SR_register(GIE))

    while (1) {
        ADC10CTL0 |= ENC | ADC10SC;             // ENC: enable conversions, ADC10SC: start conversion
        __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 (CPU off) with interrupts enabled; wake in ISR

        // Linear map: 0..1023  →  200..2000 Hz
        // 200 + (adc * 1800 / 1023)
        unsigned int freq = 200 + ((unsigned long)adc_value * 1800UL) / 1023UL;
        set_frequency(freq);                     // Update PWM to new frequency
    }
}

/*-------------------------- ADC10 INTERRUPT ---------------------------------
 * Fires when a conversion completes.
 * - Read result from ADC10MEM into adc_value.
 * - Clear LPM0 on exit so main() continues.
 *---------------------------------------------------------------------------*/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
    adc_value = ADC10MEM;                        // 10-bit result (bits 9..0)
    __bic_SR_register_on_exit(LPM0_bits);        // Clear LPM0 bits on SR when exiting ISR (wake main loop)
}
