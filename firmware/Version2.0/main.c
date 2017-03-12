/*
 * Version1.c
 *
 * Created: 05.02.2016 19:38:33
 *  Author: alexander
 * TODO: i2c Adresse im eeprom speichern
 */ 




//#define DEBUG

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifdef DEBUG
	#include <stdio.h>
#endif

#include "twislave.h"
#include "uart.h"
#include "sensor.h"
#include "processing.h"



#define USRING_I2C_ADDR   		66  /*!< I2C-Address of the module */

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



#define BAUD 9600UL      // Baudrate


void init()
{
	init_twi_slave(USRING_I2C_ADDR);	
	init_sensors();		
	#ifdef DEBUG
		uart_init(UART_BAUD_SELECT(BAUD,F_CPU));
	#endif
}

void process()
{
	uint16_t val = get_value_bit();
	
	uint8_t* dat = get_value_array();
	
	struct blockinfo info[SENSOR_COUNT];
	
	uint8_t block_count = find_blocks(dat,SENSOR_COUNT,info);



	static uint8_t last_direction = 0;
	static uint8_t last_distance = 0;
	static uint8_t start_direction = 0;
	static uint8_t start_distance = 0;
	uint8_t direction = 0;
	uint8_t distance = 0;
	uint8_t active_sensors = 0;

	static uint8_t online_flag = 0;

	if(block_count==0)//no line
	{
		online_flag=0;
		distance = 0;
		direction = 0;
		active_sensors = 0;
	}
	else // line
	{
		if(block_count==1) // einfach weil nur ein block
		{
			active_sensors = info[0].block_size;
			direction = info[0].block_middle;
			distance = SENSOR_COUNT-info[0].block_size; // realy???
		}
		if(block_count>=2) // zwei bloecke muessen verbunden werden oder?
		{
			for(int i = 0; i < block_count;i++)
				active_sensors += info[i].block_size;
			
			uint8_t l = (info[0].block_middle-info[1].block_middle+SENSOR_COUNT)%SENSOR_COUNT;
			uint8_t r = (info[1].block_middle-info[0].block_middle+SENSOR_COUNT)%SENSOR_COUNT;
			if(l>r)
			{
				distance = SENSOR_COUNT - r;
				direction = info[0].block_middle + r/2;
			}
			else
			{
				distance = SENSOR_COUNT - l;
				direction = info[1].block_middle + l/2;
			}
		}	
		if(online_flag==0)
		{
			start_direction = direction;
			start_distance = distance;
		}
		online_flag = 1;
		last_direction=direction;
		last_distance=distance;
	}

	txbuffer[USRING_LORES_REG]=val>>8; 
	txbuffer[USRING_HIRES_REG]=val&0x00ff;
	txbuffer[USRING_ACTIVE_SENSORS]=active_sensors;
	txbuffer[USRING_BLOCKCOUNT]=block_count;
	txbuffer[USRING_DIRECTION]=(direction*2);
	txbuffer[USRING_START_DIRECTION]=(start_direction*2);
	txbuffer[USRING_END_DIRECTION]=(last_direction*2);
	txbuffer[USRING_DISTANCE]=distance;
	txbuffer[USRING_START_DISTANCE]=start_distance;
	txbuffer[USRING_END_DISTANCE]=last_distance;


	#ifdef DEBUG
		static uint16_t delaycounter = 0;
		delaycounter++;
		if(delaycounter==1000)
		{
		delaycounter=0;
		char buffer[100];
		uart_putc(0x1b);
		uart_puts("[2J");
		uart_putc(0x1b);
		uart_puts("[H");
		sprintf(buffer,"block_count: %d\r\n",block_count);uart_puts(buffer);
		sprintf(buffer,"start_direction: %d\r\n",start_direction);uart_puts(buffer);
		sprintf(buffer,"start_distance: %d\r\n",start_distance);uart_puts(buffer);
		sprintf(buffer,"direction: %d\r\n",direction);uart_puts(buffer);
		sprintf(buffer,"distance: %d\r\n",distance);uart_puts(buffer);
		sprintf(buffer,"last_direction: %d\r\n",last_direction);uart_puts(buffer);
		sprintf(buffer,"last_distance: %d\r\n",last_distance);uart_puts(buffer);
		uart_puts("data: ");
		for (int x = 0; x < SENSOR_COUNT;x++)
		{
			if(x%4==0)
				uart_putc('|');
			if(dat[x])
				uart_putc('1');
			else
				uart_putc('0');
		}
		uart_puts("                  \r\n");
		sprintf(buffer,"\t\t15:%d 0:%d 1:%d\r\n",dat[15],dat[0],dat[1]);uart_puts(buffer);
		sprintf(buffer,"\t14:%d\t\t\t2:%d\r\n",dat[14],dat[2]);uart_puts(buffer);
		sprintf(buffer,"13:%d\t\t\t\t\t3:%d\r\n",dat[13],dat[3]);uart_puts(buffer);
		sprintf(buffer,"12:%d\t\t\t\t\t4:%d\r\n",dat[12],dat[4]);uart_puts(buffer);
		sprintf(buffer,"11:%d\t\t\t\t\t5:%d\r\n",dat[11],dat[5]);uart_puts(buffer);
		sprintf(buffer,"\t10:%d\t\t\t6:%d\r\n",dat[10],dat[6]);uart_puts(buffer);
		sprintf(buffer,"\t\t9:%d 8:%d 7:%d\r\n",dat[9],dat[8],dat[7]);uart_puts(buffer);
			
		}
	#endif
}

int main(void)
{
	init();
	while(1)
    {
		
        process();

	}
}