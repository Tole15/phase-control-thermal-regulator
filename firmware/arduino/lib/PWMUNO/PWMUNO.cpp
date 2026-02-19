#include "PWMUNO.h"

void PWM1class::dutyCycleA(unsigned cmp) {
    OCR1A = cmp;
}

void PWM1class::dutyCycleB(unsigned cmp) {
    OCR1B = cmp;
}

void PWM1class::restart() {
    TCCR1C = B00000000;
    TCNT1 = ICR1 - 1;
    TCCR1B = B00011010;
    TCCR1A = B11100010;
}

void PWM1class::start() {
    TCCR1C = B00000000;
    TCNT1 = 0;
    TCCR1B = B00011010;
    TCCR1A = B11100010;
}

void PWM1class::disable() {
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    ICR1 = 0;
    OCR1A = 0;
    OCR1B = 0;
}

void PWM1class::initialize() {
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    OCR1A = 0;
    OCR1B = 0;
}

void PWM1class::setPeriod(unsigned long microseconds) {
    const unsigned long cycles = ((F_CPU / 100000 * microseconds) / 10);
    unsigned long period;
    unsigned char divisorBits;
    if (cycles < TIMER1_RESOLUTION) {
        divisorBits = _BV(CS10);
        period = cycles;
    } else if (cycles < TIMER1_RESOLUTION * 8) {
        divisorBits = _BV(CS11);
        period = cycles / 8;
    } else if (cycles < TIMER1_RESOLUTION * 64) {
        divisorBits = _BV(CS11) | _BV(CS10);
        period = cycles / 64;
    } else if (cycles < TIMER1_RESOLUTION * 256) {
        divisorBits = _BV(CS12);
        period = cycles / 256;
    } else if (cycles < TIMER1_RESOLUTION * 1024) {
        divisorBits = _BV(CS12) | _BV(CS10);
        period = cycles / 1024;
    } else {
        divisorBits = _BV(CS12) | _BV(CS10);
        period = TIMER1_RESOLUTION - 1;
    }
    ICR1 = period;
    TCCR1B = _BV(WGM13) | divisorBits;
}

unsigned PWM1class::getPeriod(void) {
    return ICR1;
}

PWM1class PWM1; // Instancia de la clase
