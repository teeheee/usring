/*
 * processing.h
 *
 * Created: 05.02.2016 21:42:10
 *  Author: alexander
 */ 


#ifndef PROCESSING_H_
#define PROCESSING_H_

#include <avr/io.h>

struct blockinfo{
	uint8_t block_start;
	uint8_t block_middle;
	uint8_t block_end;
	uint8_t block_size;
};


// returns number of blocks 0 -> no block found, 0xff -> all on
uint8_t find_blocks(uint8_t* data,uint8_t data_size,struct blockinfo* block_info);


#endif /* PROCESSING_H_ */