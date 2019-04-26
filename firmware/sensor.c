/*
 * sensor.c
 *
 * Created: 05.02.2016 20:01:24
 *  Author: Alexander Ulbrich
 */

#include "sensor.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/delay.h>

void init_sensors()
{
	//init leds
	DDRC  |= (1 << PC2);

	//init multiplexer and adc ports
	DDR_0 |= (1 << A0) | (1 << B0) | (1 << C0);
	DDR_1 |= (1 << A1) | (1 << B1) | (1 << C1);
	DDRD |= (1 << PD5) | (1 << PD6);
	PORTD &= ~((1 << PD5) | (1 << PD6));

	//init ADC
	ADMUX = (1 << REFS0) | (1 << ADLAR);				// Versorgungsspannung AVCC als Referenz

	//OPTIMIERUNGS PARAMETER
	// je langsamer der ADC also je niedriger die Frequenz desdo rauscharmer die Messung
	// laut http://www.openmusiclabs.com/learning/digital/atmega-adc/ sind bei 8 bit Auflösung 2 Mhz möglich
	//ADCSRA = (1 << ADPS0);	 								    // Frequenzvorteiler 4 Mhz
	ADCSRA = (1 << ADPS1); 										// Frequenzvorteiler 2 Mhz
	//ADCSRA = (1 << ADPS1) | (1 << ADPS0); 		// Frequenzvorteiler 1 Mhz
	//ADCSRA = (1 << ADPS2); 										// Frequenzvorteiler 500 khz
	//ADCSRA = (1 << ADPS2) | (1 << ADPS0); 		// Frequenzvorteiler 250 khz
	ADCSRA |= (1 << ADEN);      // ADC aktivieren

	// Dummy readout
	ADC_Read(0);
}

uint8_t ADC_Read( uint8_t channel )
{
	// Kanal waehlen, ohne andere Bits zu beeinflußen
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC);          // eine Wandlung "single conversion"
	while (ADCSRA & (1 << ADSC) ) { // auf Abschluss der Konvertierung warten
	}
	return ADCH;                    // ADC auslesen und zurückgeben
}


const uint8_t mapping[SENSOR_COUNT] = {9, 2, 4, 0, 6, 5, 7, 3, 1, 10, 12, 8, 14, 13, 15, 11};

uint8_t get_value(uint8_t i)
{
	uint8_t index = mapping[i];
	if (index < 8)
	{
		PORT_0 &= ~((1 << A0) | (1 << B0) | (1 << C0));
		PORT_0 |=  (index << C0);
		return ADC_Read(0);
	}
	else if (index < 16)
	{//
		index -= 8;
		PORT_1 &= ~((1 << A1) | (1 << B1) | (1 << C1));
		PORT_1 |=  (index << C1);
		return ADC_Read(1);
	}
	return 0;

}



int abs(int a)
{
	return a>0?a:-a;
}

void getdiffValue(uint8_t* valuebuffer)
{
	uint8_t x = 0;
	uint8_t temp[SENSOR_COUNT];

	ledOn();
	_delay_us(LEDDELAY);
	for ( x = 0; x < SENSOR_COUNT; x++)
	{
		temp[x] = get_value(x);
	}
	ledOff();
	_delay_us(LEDDELAY);
	for ( x = 0; x < SENSOR_COUNT; x++)
	{
		valuebuffer[x] = abs(temp[x]-get_value(x));
	}
}

void getsingleValue(uint8_t* valuebuffer)
{
	uint8_t x = 0;
	ledOn();
	for ( x = 0; x < SENSOR_COUNT; x++)
	{
		valuebuffer[x] = get_value(x);
	}
}


void ledOn(void)
{
	PORTC |= (1 << PC2); //led on
}

void ledOff(void)
{
	PORTC &= ~(1 << PC2); //led off
}
