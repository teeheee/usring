#ifndef _USRING_
#define _USRING_

#define F_CPU   8000000UL
#define USRING_I2C_ADDRESS 0x69
#define USRING_BAUDRATE 9600UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "time.h"


#ifndef USRING_I2C_ADDRESS
#warning (USRING_I2C_ADDRESS was not defined. Using 0x69 instead!!!)
#define USRING_I2C_ADDRESS 0x69
#endif

#ifndef USRING_BAUDRATE
#warning (USRING_BAUDRATE was not defined. Using 9600UL instead!!!)
#define USRING_BAUDRATE 9600UL
#endif

class Usring{
public:
	Usring();

	void getDifferenceValue(uint8_t* valueBuffer);
	void getSingleValue(uint8_t* valueBuffer);

	uint8_t getAnalogValue(uint8_t id);
	void setLed(uint8_t state);

	void setI2C(uint8_t id, uint8_t value);
	void setI2CData(uint8_t id, uint8_t size, uint8_t* data);
	uint8_t getI2C(uint8_t id);

	void print(const char* string);
	void print(int value);
private:
	int _sensorValue[16];
};


#endif
