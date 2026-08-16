#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

void uart_transmit(char data) {
    // Poll status register: loop until UDRE0 bit is 1 (buffer empty)
    while (!(UCSR0A & (1 << UDRE0))); 
    
    // Write byte directly to the Hardware Transmit Buffer register
    UDR0 = data; 
}

void uart_print(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

void uart_init() {
    UBRR0 = 104;             // Set baud rate register (9600 baud at 16MHz)
    UCSR0B |= (1 << TXEN0);  // Set bit TXEN0 to enable the hardware transmitter
    uart_print("UART Driver Online!\r\n");
}

void uart_print_num(uint16_t num) {
    char buffer[10];
    itoa(num, buffer, 10);
    uart_print(buffer);
}