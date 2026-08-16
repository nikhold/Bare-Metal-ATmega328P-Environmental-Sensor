#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

void uart_init();

void uart_transmit(char data);

void uart_print(const char *str);

void uart_print_num(uint16_t num);

#endif