/*
 * Version1.c
 *
 * Created: 05.02.2016 19:38:33
 *  Author: alexander
 */ 




#define DEBUG

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


#define BAUD 9600UL      // Baudrate


void init()
{
	cli();
	init_twi_slave(66);			
	uart_init(UART_BAUD_SELECT(BAUD,F_CPU));
	DDRD|=(1<<PD1);
	DDRD&=~((1<<PD0)|(1<<PD4)|(1<<PD5));
	init_sensors();
	sei();	
}

void process()
{
	uint16_t val = get_value_bit();
	txbuffer[0]=val>>8;
	txbuffer[1]=val&0xff00;
	
	
	
	uint8_t* dat = get_value_array();
	
	struct blockinfo info[SENSOR_COUNT];
	
	uint8_t block_count = find_blocks(dat,SENSOR_COUNT,info);

	static uint8_t last_direction = 0;
	static uint8_t last_distance = 0;
	static uint8_t start_direction = 0;
	static uint8_t start_distance = 0;
	uint8_t direction = 0;
	uint8_t distance = 0;

	static uint8_t online_flag = 0;

	if(block_count==0)//no line
	{
		online_flag=0;
		distance = 0;
		direction = 0;
	}
	else // line
	{
		if(block_count==1) // einfach weil nur ein block
		{
			direction = info[0].block_middle;
			distance = SENSOR_COUNT-info[0].block_size; // realy???
		}
		if(block_count>=2) // zwei bloecke muessen verbunden werden oder?
		{
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

	txbuffer[3]=start_direction;
	txbuffer[4]=start_distance;
	txbuffer[5]=direction;
	txbuffer[6]=distance;
	txbuffer[7]=last_direction;
	txbuffer[8]=last_distance;

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
/*			1

	3				4


			2
*/
int main(void)
{
	init();
	
	while(1)
    {
        process();
	}
}