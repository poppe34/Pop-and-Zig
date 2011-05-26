/*
 * MAC_ack.c
 *
 *  Created on: Sep 23, 2010
 *      Author: mpoppe
 */
#include "conf_zigbee.h"
#include <frame.h>

#include <mac/mac_prototypes.h>
typedef void (*mac_assocHandler_t)(mac_status_t status);
mac_assocHandler_t assocHandler;

void MAC_mlme_assocReq(mlme_assoc_t *assoc, void *cb){
	assocHandler = (mac_assocHandler_t *)cb;

//	variables used in the function
	mac_status_t status = MAC_INIT;

//	Reset the MAC
	mac_init();

//	Start a frame and get the PIB data needed to generate an assoc req.
	frame_t *fr = frame_new();
	mpdu_t	*mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));

	mac_pib_t *mpib = get_macPIB();

// Setup the channel
	MAC_setCurrentChannel(assoc->LogicalChannel);

// Setup Coord address
	mpdu->destination = assoc->Coord;

	mpdu->source = mpib->macLongAddress;

//	TODO:	I need to add a function that place our PAN id the coord's after the ack. not before like below
	MAC_setPANid(assoc->Coord.PANid);
	MAC_setCoordPANid(assoc->Coord.PANid);

	if(assoc->Coord.mode == SHORT_ADDRESS){
		MAC_setCoordShortAddr(assoc->Coord.shortAddr);
	}
	else{
		MAC_setCoordLongAddr(assoc->Coord.extAddr);
	}


	mpdu->fcf.MAC_fcf_Frame_Type = MAC_COMMAND;
	mpdu->fcf.MAC_fcf_DstAddr_Mode = assoc->Coord.mode;
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = MAC_LONG_ADDRESS;
	mpdu->fcf.MAC_fcf_PANid_Compression = no;
	mpdu->fcf.MAC_fcf_Frame_Pending = no;
	mpdu->fcf.MAC_fcf_Ack_Request = yes;
	mpdu->fcf.MAC_fcf_Sec_enabled = no;
	mpdu->fcf.MAC_fcf_Frame_Ver = 0x00;



//	TODO: 	this needs to point to my PAN Table choice (I think) It might be to early to have this as the PANid  we haven't joined yet
//			9.23.10 I made this an argument of the function to pass what the panID we are talking to is.



//1. get the MAC setup of the frame

	MAC_createFrame(mpdu, fr);

//2. device's capabilities
	//command type
	*(fr->ptr)++ = 0x01; //TODO: create a defined function for 0x01
	fr->dataLength++;
    //capibilities fo the command type
    *(fr->ptr)++ = assoc->CapabilityInfo; //TODO: this should be the capability field I should have a procedure on how to get this.
	fr->dataLength++;
    
	MAC_setTxCB(&MAC_assocReq_cb);

	frame_sendWithFree(fr);
    free(mpdu);
}

mac_status_t MAC_mlme_assocInd(mlme_assoc_t assoc){
	mac_status_t status = MAC_SUCCESS;
	mac_pib_t *mpib = get_macPIB();

		if(DEFAULT_COORD){
			if(mpib->macAssociationPermit == no){
				return status;
			}
			else{

			}
		}
	return status;
}
mac_status_t mlme_assoc_response(mlme_assoc_t assoc){
	mac_status_t status = MAC_SUCCESS;
		return status;
}
mac_status_t mlme_assoc_confirm(mlme_assoc_t assoc){
/*	TODO:	This section will be added in the future to allow the NHL to know the status of the assocation.  For now the status will be handled in the
 * 			req section.  This is due to the fact that right now i don't have a NHL and I have now where to send it.  The req will have loops to allow me to
 * 			wait to get the ack and the data.
 */

	mac_status_t status = MAC_SUCCESS;
		return status;
}

void MAC_mlme_assocData(frame_t *fr){

//	TODO:	Write to code to receive a long addr too....
	uint8_t	command_status;

	MAC_setShortAddr(*((uint16_t *)(fr->ptr)));
    fr->ptr += 2;
    
//	TODO:	Somehow I need to get this error code returned as well I think I could add that to the data confirm....
	command_status = *fr->ptr++;
}

void MAC_assocReq_cb(phy_trac_t trac){
	switch(trac){
	case(TRAC_SUCCESS):
		MAC_assocConfirm(MAC_SUCCESS);
	break;

	case(TRAC_NO_ACK):
		MAC_assocConfirm(MAC_NO_ACK);
	break;

	case(TRAC_CHANNEL_ACCESS_FAILURE):
		MAC_assocConfirm(MAC_ACCESS_FAILURE);
	break;

	case(TRAC_INVALID):
		MAC_assocConfirm(MAC_INVALID_ADDRESS);
	break;

	case(TRAC_SUCCESS_DATA_PENDING):
		break;
	}
}

void MAC_assocData_cb(mac_status_t status){
	MAC_assocConfirm(status);
	//	TODO: 	I need to create a timer once it times out that I send back a failure due to no data

}

void MAC_assocConfirm(mac_status_t status){
	(assocHandler)(status);
}

