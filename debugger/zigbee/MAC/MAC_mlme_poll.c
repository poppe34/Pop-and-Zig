/*
 * MAC_mlme_poll.c
 *
 *  Created on: Nov 4, 2010
 *      Author: mpoppe
 */

#include <frame.h>
#include <mac/mac_prototypes.h>

typedef void (*mac_pollHandler_t)(mac_status_t status);
mac_pollHandler_t pollHandler;

void MAC_mlme_pollReq(mac_pollHandler_t *cb){
	pollHandler = (mac_pollHandler_t *)cb;

	frame_t *fr = get_frame();
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));
	uint8_t seq_num;

	phy_pib_t *ppib = get_phyPIB();
	mac_pib_t *mpib = get_macPIB();

//1. Setup FCF
	mpdu->fcf.MAC_fcf_Frame_Type = MAC_COMMAND;
	mpdu->fcf.MAC_fcf_DstAddr_Mode = NO_ADDRESS;
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = LONG_ADDRESS;
	mpdu->fcf.MAC_fcf_PANid_Compression = no;
	mpdu->fcf.MAC_fcf_Frame_Pending = no;
	mpdu->fcf.MAC_fcf_Ack_Request = yes;
	mpdu->fcf.MAC_fcf_Sec_enabled = no;

//2. setup destination

	mpdu->source = mpib->macLongAddress;

	*fr->ptr++ = 0x00;
	*fr->ptr++ = 0x00;
	fr->dataLength = 2;

	*fr->ptr++ = 0x04;
	fr->dataLength++;

	MAC_setTxCB(&MAC_assocData_cb);
	MAC_createFrame(mpdu, fr);

	free(mpdu);
	free_frame(fr);

}


void MAC_mlme_pollReq_cb(mac_status_t status){

	switch(status){
	case(TRAC_SUCCESS):
		MAC_mlme_pollConfirm(MAC_SUCCESS);
	break;

	case(TRAC_NO_ACK):
		MAC_mlme_pollConfirm(MAC_NO_ACK);
	break;

	case(TRAC_CHANNEL_ACCESS_FAILURE):
		MAC_mlme_pollConfirm(MAC_ACCESS_FAILURE);
	break;

	case(TRAC_INVALID):
		MAC_mlme_pollConfirm(MAC_INVALID_ADDRESS);
	break;

	case(TRAC_SUCCESS_DATA_PENDING):
		break;
	}

}

void MAC_mlme_pollConfirm(mac_status_t status){
	(pollHandler)(status);
}
