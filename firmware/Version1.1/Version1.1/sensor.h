/*
 * sensor.h
 *
 * Created: 05.02.2016 20:01:00
 *  Author: alexander
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_


#include <avr/io.h>

#define SENSOR_COUNT 16

#define DDR_S0		DDRD
#define PORT_S0		PORTD
#define PIN_S0		PIND
#define S0			PD2

#define DDR_S1		DDRC
#define PORT_S1		PORTC
#define PIN_S1		PINC
#define S1			PC3

#define DDR_S2		DDRD
#define PORT_S2		PORTD
#define PIN_S2		PIND
#define S2			PD4

#define DDR_S3		DDRD
#define PORT_S3		PORTD
#define PIN_S3		PIND
#define S3			PD3

#define DDR_S4		DDRD
#define PORT_S4		PORTD
#define PIN_S4		PIND
#define S4			PD6

#define DDR_S5		DDRD
#define PORT_S5		PORTD
#define PIN_S5		PIND
#define S5			PD5

#define DDR_S6		DDRB
#define PORT_S6		PORTB
#define PIN_S6		PINB
#define S6			PB0

#define DDR_S7		DDRD
#define PORT_S7		PORTD
#define PIN_S7		PIND
#define S7			PD7

#define DDR_S8		DDRB
#define PORT_S8		PORTB
#define PIN_S8		PINB
#define S8			PB2

#define DDR_S9		DDRB
#define PORT_S9		PORTB
#define PIN_S9		PINB
#define S9			PB1

#define DDR_S10		DDRB
#define PORT_S10	PORTB
#define PIN_S10		PINB
#define S10			PB4

#define DDR_S11		DDRB
#define PORT_S11	PORTB
#define PIN_S11		PINB
#define S11			PB3

#define DDR_S12		DDRC
#define PORT_S12	PORTC
#define PIN_S12		PINC
#define S12			PC0

#define DDR_S13		DDRB
#define PORT_S13	PORTB
#define PIN_S13		PINB
#define S13			PB5

#define DDR_S14		DDRC
#define PORT_S14	PORTC
#define PIN_S14		PINC
#define S14			PC2

#define DDR_S15		DDRC
#define PORT_S15	PORTC
#define PIN_S15		PINC
#define S15			PC1

void init_sensors();

uint8_t* get_value_array();

uint16_t get_value_bit();



#endif /* SENSOR_H_ */