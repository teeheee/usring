
#include "usring.h"
#include "Goldboard4.h"



usring::usring()
{
	_initialized = false;
}

void usring::init()
{
	_initialized = true;
}

uint16_t usring::getValue()
{
	if (!_initialized)
		return 0;

	i2cRead (USRING_I2C_ADDR, USRING_LORES_REG, (uint8_t*)&_raw, 2);
	while (!i2cIsComplete());
	if (i2cGetLastError () != TW_NO_INFO)
		return 0xffff;

	return (uint16_t) _raw;
}

uint8_t usring::getValuebyId(uint8_t index)
{
	if (!_initialized)
		return 0;

	if(index > USRING_MAX_I2C_REG)
		return 0;

	uint8_t data = 0;

	i2cRead (USRING_I2C_ADDR, index , (uint8_t*)&data, 1);
	while (!i2cIsComplete());
	if (i2cGetLastError () != TW_NO_INFO)
		return 0xff;

	return (uint8_t) data;	
}


bool usring::isInitialized()
{
	return _initialized;
}
