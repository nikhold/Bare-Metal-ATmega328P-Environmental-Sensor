#include <avr/io.h>
#include <avr/interrupt.h>
#include "button.h"
#include "uart.h"

static volatile uint8_t button_flag = 0;// Initialize button indicator

void button_init() {

    // Set PD2 (INT0) as input
    PORTD |= (1 << PORTD2);
    DDRD |= (1 << DDD2);

    // Configure EICRA for falling edge detection (ISC01 = 1, ISC00 = 0)
    EICRA |= (1 << ISC01);
    EICRA &= ~(1<< ISC00);

    // Clear lingering register (fix for button reading pressed at startup)
    EIFR |= (1 << INTF0);

    // Enable INT0 for external interrupt
    EIMSK |= (1 << INT0);

    // Run test print
    uart_print("Button Driver Online!\r\n");
}

ISR(INT0_vect) {
    button_flag = 1;// Indicate button was pressed
}

uint8_t button_was_pressed() {
    // Check if button pressed
    if (button_flag) {
        button_flag = 0;// Reset flag
        return 1;// Indicate button was pressed
    }
    return 0;// Indicate button was not pressed
}