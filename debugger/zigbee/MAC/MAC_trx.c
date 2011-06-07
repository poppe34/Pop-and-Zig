
// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief
 *
 * \par Application note:
 *
 *
 * \par Documentation
 *
 *
 *
 * \author
 *
 *
 *
 * $Id:
 *
 * Copyright (c) 2008 ,
 *
 *
 *****************************************************************************/

/*================================= INCLUDES         =========================*/

#include "frame.h"
#include <mac/mac_prototypes.h>
#include "phy/rc_rf230.h"

#include "alarms_task.h"

uint8_t temp;

/*================================= MACROS           =========================*/
/*================================= TYEPDEFS         =========================*/

/*================================= GLOBAL VARIABLES =========================*/

trx_cb_t trx_cb;
/*================================= LOCAL VARIABLES  =========================*/
/*================================= PROTOTYPES       =========================*/
/*================================= SOURCE CODE      =========================*/


/*--------------------------------------------------------------------------------
*   function:     Mac_createFrame
*
*   Description:  This function populates the frame with MAC data
*
*   Argument 1:   MAC data
*
*   Argument 2:   Frame
--------------------------------------------------------------------------------*/

mac_status_t MAC_createFrame(mpdu_t *mpdu, frame_t *fr) {
	mac_status_t status = MAC_SUCCESS;

//		TODO: 	I am not 100% which should go first I guess it doesn't matter yet.
//				9.09.10:  I am adding on to the MAC to make it more complete,, I
//				will still have more work to do.
//
//	Found Error: 	I had the frame setup backwards... I decided just to add the frame from the tail to the front..
//					FCF is the first to be transmiteed my frame is FILO (first in last out)
//                  
//                  I reversed the frame again(first byte is at fr[0]) now it should look cleaner for the programmer 

//*******************************
//
//  FCF
//
//*******************************
    
	uint16_t *tempfcf = (uint16_t *)&mpdu->fcf;
    
	SET_FRAME_DATA(fr, *tempfcf, 2);

//*******************************
//
//  Sequence Number
//
//*******************************
    
    if(mpdu->fcf.MAC_fcf_Frame_Type != MAC_ACK)
		mpdu->seq_num = get_MAC_seqNum();
    
	SET_FRAME_DATA(fr, mpdu->seq_num, 1);

//*******************************************
//
//  Destination Address & PAN ID
//
//******************************************    
	switch(mpdu->fcf.MAC_fcf_DstAddr_Mode){
            
        case(MAC_NO_ADDRESS): //TODO Finish the source address switch
			break;
        case(MAC_none):
			break;
        case(MAC_SHORT_ADDRESS):
            SET_FRAME_DATA(fr, mpdu->destination.PANid, 2);
            SET_FRAME_DATA(fr, mpdu->destination.shortAddr, 2);
            break;
            
        case(MAC_LONG_ADDRESS):
            SET_FRAME_DATA(fr, mpdu->destination.PANid, 2);
            SET_FRAME_DATA(fr, mpdu->destination.extAddr, 8);
            break;
            
	}
    
//*******************************
//
//  Source Address & PAN ID
//
//*******************************

    
	switch(mpdu->fcf.MAC_fcf_SrcAddr_Mode){

	case(MAC_NO_ADDRESS): //TODO Finish the source address switch
			break;
	case(MAC_none):
			break;
	case(MAC_SHORT_ADDRESS):
	    if((mpdu->fcf.MAC_fcf_PANid_Compression) == 0){
            SET_FRAME_DATA(fr, mpdu->source.PANid, 2);
		}		
		SET_FRAME_DATA(fr, mpdu->source.shortAddr, 2);
		break;
	case(MAC_LONG_ADDRESS):
	    if((mpdu->fcf.MAC_fcf_PANid_Compression) == 0){
            SET_FRAME_DATA(fr, mpdu->source.PANid, 2);

		}		
		SET_FRAME_DATA(fr, mpdu->source.extAddr, 8);
	break;

	}
//	TODO:	I need to go thought this and have better status detection. If possible

// CHANGE IN FUNCTIONALITY: I got rid of the send to radio command at the end.  I will now add
// a function in the frame section to send then free
}


void MAC_incomingFrame(frame_t *fr)
{
	//mac_command_type_t command;
	mac_frame_type_t type;

	mac_pib_t *mpib = get_macPIB();
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));

	if(MAC_breakdownFrame(mpdu, fr))
	{
		free(mpdu);
		return;

	}

	type = mpdu->fcf.MAC_fcf_Frame_Type;

	/*if(MACreceiveBeaconOnly == yes)
	{
		if(type != MAC_BEACON)
		{
			free(mpdu);
			return;
		}
	}*/

	if(type == MAC_BEACON)
	{
		MAC_beaconHandler(mpdu, fr);
	}

	if(type == MAC_DATA){

	}
	if(type == MAC_ACK){

//	TODO:	I need to create a ack handler...
		add_ack(mpdu->seq_num);
	}

	if(type == MAC_COMMAND)
	{

	}//end if
}
//--------------------------------------------------------------------------------
//function:     Mac_breakdownFrame
//
//Discription:  This function decodes the MAC data from an incoming frame
//
//Argument 1:   MAC data reference
//
//Argument 2:   Frame
//--------------------------------------------------------------------------------
mac_filter_t MAC_breakdownFrame(mpdu_t *mpdu, frame_t *fr){
	mac_fcf_t fcf_temp;
	mac_pib_t *mpib = get_macPIB();
	mac_filter_t filtered = NOT_FILTERED;

	/****************************
	 * Incoming fcf
	 *****************************/
	/*fcf_temp = GET_FRAME_DATA(fr, 2); I am trying to get the fcf without all the shifting

	mpdu->fcf.MAC_fcf_Frame_Type = ((fcf_temp >> MAC_FCF_FRAME_TYPE_SHIFT)& 0x07);
	mpdu->fcf.MAC_fcf_Sec_enabled = ((fcf_temp >> MAC_FCF_SEC_ENABLE_SHIFT)& 0x01);
	mpdu->fcf.MAC_fcf_Frame_Pending = ((fcf_temp >> MAC_FCF_FRAME_PENDING_SHIFT)& 0x01);
	mpdu->fcf.MAC_fcf_Ack_Request = ((fcf_temp >> MAC_FCF_ACK_REQUEST_SHIFT)& 0x01);
	mpdu->fcf.MAC_fcf_PANid_Compression = ((fcf_temp >> MAC_FCF_PAN_ID_COMPRESS_SHIFT)& 0x01);
	mpdu->fcf.MAC_fcf_DstAddr_Mode = ((fcf_temp >> MAC_FCF_DEST_ADDR_MODE_SHIFT)& 0x03);
	mpdu->fcf.MAC_fcf_Frame_Ver = ((fcf_temp >> MAC_FCF_FRAME_VERSION_SHIFT)& 0x03);
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = ((fcf_temp >> MAC_FCF_SRC_ADDR_MODE_SHIFT)& 0x03);
     */
    
    mpdu->fcf = *((mac_fcf_t *)(fr->ptr));
    fr->ptr += sizeof(mac_fcf_t);
    
	//TODO: see if there is any way to set the mpdu->fcf equal to the frame so I won't have to break down the bits
	uint16_t *temp = &mpdu->fcf;

	mpdu->source.mode = mpdu->fcf.MAC_fcf_SrcAddr_Mode;
	mpdu->destination.mode = mpdu->fcf.MAC_fcf_DstAddr_Mode;
/***************************************
 * Sequence Number
 ***************************************/
	mpdu->seq_num = GET_FRAME_DATA(fr, 1);

/***************************************
* Incoming dest PAN ID and Address
***************************************/
	switch(mpdu->fcf.MAC_fcf_DstAddr_Mode){

	case(MAC_NO_ADDRESS):
// 		no action needed
	break;

	case(MAC_SHORT_ADDRESS):
		mpdu->destination.mode = MAC_SHORT_ADDRESS;

		mpdu->destination.PANid = GET_FRAME_DATA(fr, 2);

		mpdu->destination.shortAddr = GET_FRAME_DATA(fr, 2);

		if(mpdu->fcf.MAC_fcf_PANid_Compression){
			mpdu->source.PANid = GET_FRAME_DATA(fr, 2);
		}
	break;
	case(MAC_LONG_ADDRESS):
		mpdu->destination.mode = MAC_LONG_ADDRESS;

		mpdu->destination.PANid = GET_FRAME_DATA(fr, 2);

		mpdu->destination.extAddr = GET_FRAME_DATA(fr, 8);


		if(mpdu->fcf.MAC_fcf_PANid_Compression){
			mpdu->source.PANid = GET_FRAME_DATA(fr, 2);
		}
	break;

	}

	if(mpib->macPromiscuousMode == 0){
		filtered = MAC_secondLevelFilter(mpdu);
			if(filtered != NOT_FILTERED){
			//	led_on_byte(filtered);
				return FILTERED;
			}//end if
		if(mpdu->fcf.MAC_fcf_Ack_Request && mpib->macAutoRequest){
				MAC_issueACK(mpdu->seq_num);
			}
	}//end if


	/***************************************
	* Incoming source PAN ID and Address
	 ***************************************/

	switch(mpdu->fcf.MAC_fcf_SrcAddr_Mode){

	case(MAC_NO_ADDRESS):
// 		no action needed
	break;

	case(MAC_SHORT_ADDRESS):
		if(mpdu->fcf.MAC_fcf_PANid_Compression == 0){
			mpdu->source.PANid = GET_FRAME_DATA(fr, 2);
		}
		mpdu->source.shortAddr = GET_FRAME_DATA(fr, 2);

	break;
	case(MAC_LONG_ADDRESS):

		if(mpdu->fcf.MAC_fcf_PANid_Compression == 0){
			mpdu->source.PANid = GET_FRAME_DATA(fr, 2);
		}

		mpdu->source.extAddr = GET_FRAME_DATA(fr, 8);
	break;
	}

	if(mpdu->fcf.MAC_fcf_Sec_enabled){
//	TODO:	Need to add the security package info
	}

	return NOT_FILTERED;
}

//--------------------------------------------------------------------------------
//function:     Mac_secondLevelFilter
//
//Discription:  This function filters out any and all un neccessary frames
//
//Argument 1:   MAC data
//--------------------------------------------------------------------------------
mac_filter_t MAC_secondLevelFilter(mpdu_t *mpdu){
	mac_filter_t filter = NOT_FILTERED;
	mac_pib_t *mpib = get_macPIB();


//1. frame type field is not reserved
	if(mpdu->fcf.MAC_fcf_Frame_Type > MAC_COMMAND){
		return level1;
	}//end if
//2. frame version is not reserved
	if(mpdu->fcf.MAC_fcf_Frame_Ver > MAC_NOT_COMPATIBLE_WITH_802_15_4_2003){
		//led_on_byte(mpdu->fcf.MAC_fcf_Frame_Ver);
		return level2;
	}//end if
//3. Destination PAN is included and equals either 0xffff or my joined PAN
	if(mpdu->fcf.MAC_fcf_DstAddr_Mode > none){
		if(mpdu->destination.PANid != mpib->macPANid){
			if(mpdu->destination.PANid != 0xffff){
				return level3;
			}//end if
		}//end if
	}//end if

//4. Short add is included in the frame and it shall match 0xffff or macShortAddress.. if extened addr is used macExtendedAddr
	if(mpdu->fcf.MAC_fcf_DstAddr_Mode == SHORT_ADDRESS){
		if(mpdu->destination.shortAddr != mpib->macShortAddress.shortAddr){
			if(mpdu->destination.shortAddr == 0xffff){
				return level4;
			}//end if
		}//end if
	}//end if

	else if (mpdu->fcf.MAC_fcf_DstAddr_Mode == LONG_ADDRESS){

		if(mpdu->destination.extAddr != mpib->macLongAddress.extAddr){
				if(mpdu->destination.extAddr != 0xffffffffffffffff){
						return level4;
				}//end if
		}//end if
	}//end else if

//5. if the frame type is a beacon the source PAN shall equal macPANid unless macPANid is equal to 0xffff.. if equal 0xffff accept all
	if(mpdu->fcf.MAC_fcf_Frame_Type == MAC_BEACON){
		if(mpib->macPANid != 0xffff){
			if(mpdu->source.PANid != mpib->macShortAddress.PANid){

				return level5;
			}//end if
		}//end if
	}//end if

//6. 	If only the souce fields are included in a data or command the frame shall be accepted
//		only if the device is the PAN coord and the source PANid matches macPANid
	if(mpdu->fcf.MAC_fcf_Frame_Type == MAC_DATA || mpdu->fcf.MAC_fcf_Frame_Type == MAC_COMMAND){
		if(mpdu->fcf.MAC_fcf_DstAddr_Mode <  SHORT_ADDRESS || mpdu->fcf.MAC_fcf_DstAddr_Mode > LONG_ADDRESS){
			if(mpib->macAssociatedPANCoord){
				if(mpdu->source.PANid != mpib->macPANid){
					return level6;
				}//end if
			}//end if
			else{
				return FILTERED;
			}//end else
		}//end if
	}//end if


	return filter;
}

//--------------------------------------------------------------------------------
//function:     Mac_issueAck
//
//Discription:  This function issues the ack back for a frame
//
//Argument 1:   sequence number of the incoming frame
//--------------------------------------------------------------------------------
void MAC_issueACK(uint8_t seq_num){
// I am creating a new space in memory so I don't have to use up space in the the frame pool
	frame_t *fr = (frame_t *)malloc(sizeof(frame_t));

    //add fcf
	SET_FRAME_DATA(fr, 0x0002, 2);
    
    //add sequence num
	SET_FRAME_DATA(fr, seq_num, 1);
    
	//room for CRC
	SET_FRAME_DATA(fr, 0x0000, 2);

	rc_send_frame(fr->dataLength, NO);

	free(fr);
}
//--------------------------------------------------------------------------------
//function:     Mac_setTxCB
//
//Description:  This function is sets the call back to MAC knows what function to 
//              executed after completion
//
//Argument 1:   Incoming function callback
//--------------------------------------------------------------------------------
void MAC_setTxCB(voidPtr tb){
	trx_cb = (trx_cb_t *)tb;//TODO work on the description and make sure this is correct
}
//--------------------------------------------------------------------------------
//function:     Mac_txStatus
//
//Description:  This function executes callback function
//
//Argument 1:   status of tx
//--------------------------------------------------------------------------------
void MAC_txStatus(phy_trac_t trac){
	if(trx_cb)
	{
		(trx_cb)(trac);
	}
	else
	{
		alarm("Mac Status called with no callback set (MAC_trx.c)");
	}
	
}

/*EOF*/
