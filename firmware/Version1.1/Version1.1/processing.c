/*
 * processing.c
 *
 * Created: 05.02.2016 21:41:58
 *  Author: alexander
 */ 
//#define DEBUG
#include "processing.h"

#ifdef DEBUG
	#include "uart.h"
	#include <stdio.h>
	char buffer[100];
	
	#define PRINT(x) sprintf(buffer,"%d\r\n",x);uart_puts(buffer)
	#define PRINTS(x) uart_puts(x)
#else
	#define PRINT(x)
	#define PRINTS(x)
#endif

uint8_t find_blocks(uint8_t* data,uint8_t data_size,struct blockinfo* block_info)
{
	uint8_t first_zero = 0;
	uint8_t block_flag = 0;
	uint8_t block_count = 0;

	for(;first_zero < data_size; first_zero++)
		if(data[first_zero]==0)
			break;
			
	if(data[first_zero])
		return 0xff;

	PRINTS("first_zero: ");
	PRINT(first_zero);
	int index=first_zero+1;
	for(; index!=first_zero; index=(index+1)%data_size)
	{
		PRINT(index);
		if(data[index]) //block
		{
			if(block_flag==0) //start of block
			{
				PRINTS("start: ");
				PRINT(index);
				block_info[block_count].block_size=1;
				block_info[block_count].block_start=index;
				block_flag=1;
			}
			else             // middle of block
			{
				block_info[block_count].block_size++;
			}
		}
		else           //no block
		{
			if(block_flag==1) //end of block
			{
				PRINTS("size: ");
				PRINT(block_info[block_count].block_size);
				PRINTS("end: ");
				PRINT(index);
				block_info[block_count].block_middle = block_info[block_count].block_start + block_info[block_count].block_size/2;
				block_info[block_count].block_end = index;
				block_count++;
				block_flag=0;
			}
			else               //no block
			{
				//nop
			}
		}
	}	
	if(block_flag==1) //end of block
	{
		PRINTS("size: ");
		PRINT(block_info[block_count].block_size);
		PRINTS("end: ");
		PRINT(index);
		block_info[block_count].block_middle = block_info[block_count].block_start + block_info[block_count].block_size/2;
		block_info[block_count].block_end = index;
		block_count++;
		block_flag=0;
	}
	return block_count;
}