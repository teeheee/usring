/*
 * sensor.h
 *
 * Created: 05.02.2016 20:01:00
 *  Author: alexander
 */


#ifndef SENSOR_H_
#define SENSOR_H_


#include <avr/io.h>

//OPTIMIERUNGS PARAMETER
//#define LEDDELAY 800 //ein delay in Microsekunden damit die LEDS auch wirklich an sind und die Spannung stabil ist wenn die Messung gestartet wird
#define LEDDELAY 0 //man kann es weglassen... dann gibt es aber nicht so gute Messwerte

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
uint8_t get_value(uint8_t index);
void init_multiplexer(void);
uint8_t ADC_Read( uint8_t channel );
void getdiffValue(uint8_t* valuebuffer);
void getsingleValue(uint8_t* valuebuffer);
void ledOn(void);
void ledOff(void);



#endif /* SENSOR_H_ */
