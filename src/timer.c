#include <avr/io.h>
#include "uart.h"

void timer1_init() {
    // Reset control registers to a clean state
    TCCR1A = 0;
    TCCR1B = 0;

    // Set CTC Mode 4 (WGM12 = 1) and prescaler value (1024)
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

    // Set compare value
    OCR1A = 15624;

    // Enable interrupts
    TIMSK1 |= (1 << OCIE1A);

    // Run print test
    uart_print("Timer Driver Online!\r\n");
}
