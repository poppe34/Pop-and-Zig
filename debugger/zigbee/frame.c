/*
 * zigbee.c
 *
 *  Created on: Sep 13, 2010
 *      Author: mpoppe
 */

#include "frame.h"

#include "rc.h"

#include "list.h"

#include "alarm_task.h"

//Track all the allocated frames
LIST(frames);

/*--------------------------------------------------------------------------------
 *   function:     frame_init
 *
 *   Discription:  This function inits the frames list to track how many frames are 
 *                  currently allocated
 *
 *   Argument 1:   none
 *
 *   return:       none
 --------------------------------------------------------------------------------*/
void frame_init(void)
{
    list_init(frames);
}

/*--------------------------------------------------------------------------------
 *   function:     frame_newFrame
 *
 *   Discription:  This function allocates memory for a frame and directs the pointer
 *                  to the begining of the memory buffer
 *
 *   Argument 1:   none
 *
 *   return:        starting address for a newly allocated memory
 --------------------------------------------------------------------------------*/
frame_t *frame_newFrame(void)
{
    frame_t *fr;
    
    if(list_count(frames)<kFRAME_POOL_SIZE)
    {
        fr = (frame_t *)malloc(sizeof(frame_t));
        fr->ptr = fr->frame;
    }
    else
        alarm("Exceded the max number of frames");
    
	return fr;
}
/*--------------------------------------------------------------------------------
 *   function:     Mac_createFrame
 *
 *   Discription:  This function populates the frame with MAC data
 *
 *   Argument 1:   MAC data
 *
 *   Argument 2:   Frame
 --------------------------------------------------------------------------------*/
void frame_free(frame_t *fr)
{
    list_remove(frames, fr);
    free(fr);
}

/*--------------------------------------------------------------------------------
 *   function:     frame_sendWithFree
 *
 *   Discription:  This function sends the frame to the phy layer to be sent out 
 *                  then frees the allocated memory for that frame
 *
 *   Argument 1:   MAC data
 *
 *   Argument 2:   Frame
 --------------------------------------------------------------------------------*/
void frame_sendWithFree(frame_t *fr)
{
//****************************
//  Add CRC 
//****************************
    
    *((uint16_t *)fr->ptr) = 0x0000;
    fr->ptr += 2;
    fr->dataLength += 2;
    
    rc_send_data(fr->dataLength, fr->frame);
    frame_free(frame_t *fr);
}
/*--------------------------------------------------------------------------------
 *   function:     Mac_clearAll
 *
 *   Discription:  This function deallocates all the data frames
 *
 *   Argument 1:   none
 *
 *   Argument 2:   none
 --------------------------------------------------------------------------------*/
void frame_clearAll(void)
{
    //TODO: add a clear all data frames section
}
/* EOF */
