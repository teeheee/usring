/*
 * sensor.c
 *
 * Created: 05.02.2016 20:01:24
 *  Author: alexander
 */

#include "sensor.h"

void init_sensors()
{
	DDR_S0 &= ~(1 << S0);
	DDR_S1 &= ~(1 << S1);
	DDR_S2 &= ~(1 << S2);
	DDR_S3 &= ~(1 << S3);
	DDR_S4 &= ~(1 << S4);
	DDR_S5 &= ~(1 << S5);
	DDR_S6 &= ~(1 << S6);
	DDR_S7 &= ~(1 << S7);
	DDR_S8 &= ~(1 << S8);
	DDR_S9 &= ~(1 << S9);
	DDR_S10 &= ~(1 << S10);
	DDR_S11 &= ~(1 << S11);
	DDR_S12 &= ~(1 << S12);
	DDR_S13 &= ~(1 << S13);
	DDR_S14 &= ~(1 << S14);
	DDR_S15 &= ~(1 << S15);
	
	PORT_S0 &= ~(1 << S0);
	PORT_S1 &= ~(1 << S1);
	PORT_S2 &= ~(1 << S2);
	PORT_S3 &= ~(1 << S3);
	PORT_S4 &= ~(1 << S4);
	PORT_S5 &= ~(1 << S5);
	PORT_S6 &= ~(1 << S6);
	PORT_S7 &= ~(1 << S7);
	PORT_S8 &= ~(1 << S8);
	PORT_S9 &= ~(1 << S9);
	PORT_S10 &= ~(1 << S10);
	PORT_S11 &= ~(1 << S11);
	PORT_S12 &= ~(1 << S12);
	PORT_S13 &= ~(1 << S13);
	PORT_S14 &= ~(1 << S14);
	PORT_S15 &= ~(1 << S15);
}

uint8_t* get_value_array()
{
	static uint8_t data[16];
	data[0]=(PIN_S0&(1<<S0))?1:0;
	data[1]=(PIN_S1&(1<<S1))?1:0;
	data[2]=(PIN_S2&(1<<S2))?1:0;
	data[3]=(PIN_S3&(1<<S3))?1:0;
	data[4]=(PIN_S4&(1<<S4))?1:0;
	data[5]=(PIN_S5&(1<<S5))?1:0;
	data[6]=(PIN_S6&(1<<S6))?1:0;
	data[7]=(PIN_S7&(1<<S7))?1:0;
	data[8]=(PIN_S8&(1<<S8))?1:0;
	data[9]=(PIN_S9&(1<<S9))?1:0;
	data[10]=(PIN_S10&(1<<S10))?1:0;
	data[11]=(PIN_S11&(1<<S11))?1:0;
	data[12]=(PIN_S12&(1<<S12))?1:0;
	data[13]=(PIN_S13&(1<<S13))?1:0;
	data[14]=(PIN_S14&(1<<S14))?1:0;
	data[15]=(PIN_S15&(1<<S15))?1:0;
	return data;
}

uint16_t get_value_bit()
{
	uint8_t* data = get_value_array();
	uint16_t ret = 0;
	for(int x = 0; x < 16;x++)
	{
		ret+=data[x];
		ret=ret<<1;
	}
	return ret;
}

