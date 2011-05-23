/*
 * MAC_mlme_disassociate.c
 *
 *  Created on: Oct 13, 2010
 *      Author: mpoppe
 */
#include "frame.h"
#include <mac/mac_prototypes.h>

typedef void (*mac_disassocHandler_t)(mac_status_t);
mac_disassocHandler_t disassocHandler;

void mac_mlme_disassoc_request(mac_disassoc_reason_t reason, void *cb){
	disassocHandler = (mac_disassocHandler_t *)cb;

	frame_t *fr = get_frame();
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));

	mac_pib_t *mpib = get_macPIB();
	uint8_t seq_num;

	mac_command_type_t command = DISASSOC_NOTIFY;
	mac_status_t status;

//Setup Command Frame
	mpdu->fcf.MAC_fcf_Frame_Type = MAC_COMMAND;
	mpdu->fcf.MAC_fcf_DstAddr_Mode = MAC_SHORT_ADDRESS;
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = MAC_LONG_ADDRESS;
	mpdu->fcf.MAC_fcf_PANid_Compression = yes;
	mpdu->fcf.MAC_fcf_Frame_Pending = no;
	mpdu->fcf.MAC_fcf_Ack_Request = yes;
	mpdu->fcf.MAC_fcf_Sec_enabled = no;

	mpdu->destination = mpib->macCoordShortAddress;//TODO: This needs to point somewhere else maybe??? 9.23.10 I think this might be right

	mpdu->source = mpib->macLongAddress;

//Add room for CRC
	*fr->ptr++ = 0x00;
	*fr->ptr++ = 0x00;

	fr->dataLength = 2;
//Reason for the Disassociation
	*fr->ptr++ = reason;
	fr->dataLength++;

//Comand Frame Type
	*fr->ptr++ = command;
	fr->dataLength++;


//Format the frame and send it out
	MAC_setTxCB(&MAC_disassoc_cb);
	status = MAC_createFrame(mpdu, fr);
	seq_num = mpdu->seq_num;

// Free the memory
	free(mpdu);
	free_frame(fr);
}//end MAC_mlme_disassocReq

void MAC_disassoc_cb(phy_trac_t trac){
	switch(trac){
	case(TRAC_SUCCESS):
		MAC_disassocConfirm(MAC_SUCCESS);
	break;

	case(TRAC_NO_ACK):
		MAC_disassocConfirm(MAC_NO_ACK);
	break;

	case(TRAC_CHANNEL_ACCESS_FAILURE):
		MAC_disassocConfirm(MAC_ACCESS_FAILURE);
	break;
	}
}

void MAC_disassocConfirm(mac_status_t status){
	(disassocHandler)(status);
}
