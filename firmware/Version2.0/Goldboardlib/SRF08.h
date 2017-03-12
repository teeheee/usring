/**********************************************************************
@file       SRF08.h

@brief      Driver for the SRF08 sonar sensor

-----------------------------------------------------------------------
@author  2015 Christoph Rothermel
         Christoph.Rothermel@gmx.de
-----------------------------------------------------------------------
@History

1.00 28.01.2015 First working version

@end
***********************************************************************/

#ifndef _SRF08_h
#define _SRF08_h

#include "global.h"
#include "i2c.h"
#include <util/delay.h>

class SRF08
{
public:
	SRF08();
	
	void			init(uint8_t address);
	
    void			changeAddress(uint8_t newAddress);
	int				getValueCM();
	
	bool			isInitialized();

protected:
	bool _initialized;
    int				_address;
	unsigned int	_lastValue;
	
	uint16_t _maxValue;

	void setRange (uint16_t millimeters);
	void setGain (uint8_t gain);
};


#endif

