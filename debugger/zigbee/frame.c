/*
 * zigbee.c
 *
 *  Created on: Sep 13, 2010
 *      Author: mpoppe
 */

#include "frame.h"

frame_t *get_frame(void)
{
	uint8_t x;
	frame_t *fr = (frame_t *)malloc(sizeof(frame_t));

	fr->ptr = fr->frame;

	return fr;
}

void free_frame(frame_t *frame)
{
	frame->direction = EMPTY;
	frame->dataLength = 0;
	free(frame);
}



