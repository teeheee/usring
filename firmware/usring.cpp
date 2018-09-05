#include "usring.h"

extern "C" {
#include "sensor.h"
#include "uart.h"
#include "twislave.h"
}

Usring::Usring()
{
  init_sensors();
  uart_init(UART_BAUD_SELECT(9600, F_CPU));
  init_twi_slave(USRING_I2C_ADDRESS);
}
	
void Usring::getDifferenceValue(int* valueBuffer)
{
	getdiffValue(valueBuffer);
}

uint8_t Usring::getAnalogValue(uint8_t id)
{
	return get_value(id);
}

void Usring::setLed(uint8_t state)
{
	if(state)
		ledOn();
	else
		ledOff();
}

void Usring::setI2C(uint8_t id, uint8_t value)
{
	txbuffer[id] = value;
}

void Usring::setI2CData(uint8_t id, uint8_t size, uint8_t* data)
{
	for(uint8_t i = 0; i < size; i++)
    {
    	txbuffer[i+id] = data[i];
    }
}

uint8_t Usring::getI2C(uint8_t id)
{
	return rxbuffer[id];
}

void Usring::print(const char* string)
{
	uart_puts(string);
}

void Usring::print(int value)
{
	char buffer[10];
	uart_puts(itoa(value,buffer,10));
}
