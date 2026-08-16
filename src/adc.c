#include <avr/io.h>
#include "adc.h"
#include "uart.h"

void adc_init() {
    ADMUX |= (1 << REFS0); // Set Reference Voltage to AVcc (5V)

    // Enable ADC and set Prescaler to 128 (16MHz / 128 = 125kHz ADC Clock)
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Run print test
    uart_print("ADC Driver Online!\r\n");
}

uint16_t adc_read(uint8_t channel) {
    // Select channel (0 to 7) while preserving high bits of ADMUX
    channel &= 0x07; // Guarantee channel is within range 0-7
    ADMUX = (ADMUX & 0xF8) | channel;

    // Start single conversion by setting ADSC bit
    ADCSRA |= (1 << ADSC);

    // Poll ADSC bit: wait until hardware clears ADSC to 0 (conversion complete)
    while (ADCSRA & (1 << ADSC));

    // Return the full 10-bit result from the ADC data register pair
    return ADC;
}