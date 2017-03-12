
#define DEBUG

#include "Goldboard4.h"
#include "usring.h"

Goldboard4 gb;
usring u;



#define USRING_BLOCKCOUNT		3     /* Anzahl der zusammenhängenden Blöcke */ 
#define USRING_DIRECTION 		4	  /* Richtung der Linie */
#define USRING_START_DIRECTION  5	  /* Richtung in der die Linie als erstes Gesehen wurde in 1/10 Grad*/
#define USRING_END_DIRECTION    6     /* Richtung in der die Linie als letztes Gesehen wurde in 1/10 Grad*/
#define USRING_DISTANCE			7     /* Distanz zur Linie (im test) */
#define USRING_START_DISTANCE	8		//...
#define USRING_END_DISTANCE		9       //...


int main(void)
{
	u.init();
	while(1)
	{
		uint16_t data = u.getValue();
		SERIAL_PRINT("\e[1;1H\e[2J");
		SERIAL_PRINT("Value_binary: ");
		for(uint16_t x = 0; x < 16 ; x++)
		{

				if(data&(1<<x))
					SERIAL_PRINT(1);
				else
					SERIAL_PRINT(0);
		}
		SERIAL_PRINTLN();
		SERIAL_PRINT("Value_dez: ");
		SERIAL_PRINTLN(data);
		SERIAL_PRINT("USRING_ACTIVE_SENSORS: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_ACTIVE_SENSORS));
		SERIAL_PRINT("USRING_BLOCKCOUNT: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_BLOCKCOUNT));
		SERIAL_PRINT("USRING_DIRECTION: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_DIRECTION));
		SERIAL_PRINT("USRING_START_DIRECTION: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_START_DIRECTION));
		SERIAL_PRINT("USRING_END_DIRECTION: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_END_DIRECTION));
		SERIAL_PRINT("USRING_DISTANCE: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_DISTANCE));
		SERIAL_PRINT("USRING_START_DISTANCE: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_START_DISTANCE));
		SERIAL_PRINT("USRING_END_DISTANCE: ");
		SERIAL_PRINTLN(u.getValuebyId(USRING_END_DISTANCE));
		delay(500);
	}
}

