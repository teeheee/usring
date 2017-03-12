/**********************************************************************
@file       CMPS03.h

@brief      Driver for U's Sensorring

-----------------------------------------------------------------------
@author  2016 Alexander Ulbrich
         alexander.ulbrich@uni-ulm.de
-----------------------------------------------------------------------
@History

1.00 1.10.2016 First working version

@end
***********************************************************************/

#ifndef _USRING_h
#define _USRING_h

#include "global.h"

#include "i2c.h"

#define USRING_LORES_REG		0     /* Register for 8 bit measurement (0-255) */
#define USRING_HIRES_REG		1     /* Register for 16 bit measurement (0-3599 (0-359,9°)) */
#define USRING_ACTIVE_SENSORS   2     /* Number of Active Sensors. Für Ecken erkennung praktisch */
#define USRING_BLOCKCOUNT		3     /* Anzahl der zusammenhängenden Blöcke */ 
#define USRING_DIRECTION 		4	  /* Richtung der Linie */
#define USRING_START_DIRECTION  5	  /* Richtung in der die Linie als erstes Gesehen wurde in 1/10 Grad*/
#define USRING_END_DIRECTION    6     /* Richtung in der die Linie als letztes Gesehen wurde in 1/10 Grad*/
#define USRING_DISTANCE			7     /* Distanz zur Linie (im test) */
#define USRING_START_DISTANCE	8		//...
#define USRING_END_DISTANCE		9       //...




#define USRING_I2C_ADDR   		66  /*!< I2C-Address of the module */
#define USRING_MAX_I2C_REG  	9


#define USRING_ERROR_TIMEOUT	100	  /* After this time, there MUST be a value */

class usring
{
	public:
		usring();
		void init();
		bool isInitialized();
		uint16_t getValue();
		uint8_t getValuebyId(uint8_t index);

	private:
		
		bool _initialized;
		uint16_t _raw;
};

#endif

