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

#define A0 PD4
#define B0 PD3
#define C0 PD2
#define PORT_0 PORTD
#define DDR_0  DDRD

#define ADC_0 0

#define A1 PB2
#define B1 PB1
#define C1 PB0
#define PORT_1 PORTB
#define DDR_1  DDRB

#define ADC_1 1


void init_sensors(void);
uint16_t get_value(uint8_t index);
void init_multiplexer(void);
uint16_t ADC_Read( uint8_t channel );
void getdiffValue(int* valuebuffer);
void ledOn(void);
void ledOff(void);



#endif /* SENSOR_H_ */