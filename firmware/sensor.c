/*
 * sensor.c
 *
 * Created: 05.02.2016 20:01:24
 *  Author: alexander
 */

#include "sensor.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/delay.h>

void init_sensors()
{
//leds output

	DDRC  |= (1 << PC2);

//init multiplexer

	DDR_0 |= (1 << A0) | (1 << B0) | (1 << C0);
	DDR_1 |= (1 << A1) | (1 << B1) | (1 << C1);

	DDRD |= (1 << PD5) | (1 << PD6);
	PORTD &= ~((1 << PD5) | (1 << PD6));

	// die Versorgungsspannung AVcc als Referenz wählen:
	//ADMUX = (1 << REFS0);
	// oder interne Referenzspannung als Referenz für den ADC wählen:
    ADMUX = (1<<REFS1) | (1<<REFS0);

	// Bit ADFR ("free running") in ADCSRA steht beim Einschalten
	// schon auf 0, also single conversion
	ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Frequenzvorteiler
	ADCSRA |= (1 << ADEN);                // ADC aktivieren

	/* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
	also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
	ADC_Read(0);
}

/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
	// Kanal waehlen, ohne andere Bits zu beeinflußen
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC);          // eine Wandlung "single conversion"
	while (ADCSRA & (1 << ADSC) ) { // auf Abschluss der Konvertierung warten
	}
	return ADCW;                    // ADC auslesen und zurückgeben
}


const uint8_t mapping[SENSOR_COUNT] = {9, 2, 4, 0, 6, 5, 7, 3, 1, 10, 12, 8, 14, 13, 15, 11};

uint16_t get_value(uint8_t i)
{
	uint8_t index = mapping[i];
	if (index < 8)
	{
		PORT_0 &= ~((1 << A0) | (1 << B0) | (1 << C0));
		PORT_0 |=  (index << C0);
		return ADC_Read(0);
	}
	else if (index < 16)
	{
		index -= 8;
		PORT_1 &= ~((1 << A1) | (1 << B1) | (1 << C1));
		PORT_1 |=  (index << C1);
		return ADC_Read(1);
	}
	return 0;

}

#define LEDDELAY 800

int abs(int a)
{
	return a>0?a:-a;
}

void getdiffValue(int* valuebuffer)
{
	uint8_t x = 0;
	uint16_t temp[SENSOR_COUNT];

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
		if(valuebuffer[x] > 255)
			valuebuffer[x] = 255;
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


