#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "button.h"

#define LIGHT_THRESHOLD 300

volatile uint8_t timer1_flag = 0; // Initialize timer flag

ISR(TIMER1_COMPA_vect) {
    timer1_flag = 1; // Flag that 1 second has elapsed
}

int main() {
    // Initialize drivers
    uart_init();
    adc_init();
    timer1_init();
    button_init();

    // Configure PB5 (Pin D13) as Output
    DDRB |= (1 << DDB5);
    PORTB &= ~(1 << PORTB5); // Start with LED OFF

    // Enable interrupts
    sei();

    // Initialize alarm flag
    uint8_t alarm_acknowledged = 0;

    // Print ready message
    uart_print("System Ready!\r\n");

    while (1) {
        //Check if button was pressed
        if (button_was_pressed()) {
            uart_print("Button Pressed! Resetting LED...\r\n");
            PORTB &= ~(1 << PORTB5); // Turn LED off
        }
        // Check if Timer1 fired
        if (timer1_flag) {
            timer1_flag = 0; // Clear the flag
            // Execute non-blocking scheduled tasks
            uint16_t ldr_val = adc_read(0);// Read raw voltage level on Pin A0 (ADC0)
            uart_print("LDR Value: ");// Print voltage levels
            uart_print_num(ldr_val);
            uart_print("\r\n");
            // Check if light level below threshold
            if (ldr_val < LIGHT_THRESHOLD) {
                if (!alarm_acknowledged) {
                    alarm_acknowledged = 1; // Set alarm
                    PORTB |= (1 << PORTB5);// Turn LED on
                    uart_print("Low Light Detected!\r\n");
                }
                else {
                    alarm_acknowledged = 0; // Clear alarm
                    PORTB &= ~(1 << PORTB5); // Reset LED
                }
            }

        }
    }
}