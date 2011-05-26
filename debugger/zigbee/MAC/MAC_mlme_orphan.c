/*
 * MAC_mlme_orphan.c
 *
 *  Created on: Oct 25, 2010
 *      Author: mpoppe
 */
#include <frame.h>
#include <MAC/MAC_prototypes.h>

typedef void (*mac_orphanHandler_t)(mac_status_t status);
mac_orphanHandler_t orphanHandler;


void MAC_mlme_orphanReq(void *cb){

	orphanHandler = (mac_orphanHandler_t *)cb;

	mac_pib_t *mpib = get_macPIB();
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));
	frame_t *fr = frame_new();

// Setup the command frame
	mpdu->fcf.MAC_fcf_Frame_Type = MAC_COMMAND;
	mpdu->fcf.MAC_fcf_DstAddr_Mode = SHORT_ADDRESS;
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = LONG_ADDRESS;
	mpdu->fcf.MAC_fcf_PANid_Compression = yes;
	mpdu->fcf.MAC_fcf_Frame_Pending = no;
	mpdu->fcf.MAC_fcf_Ack_Request = no;
	mpdu->fcf.MAC_fcf_Sec_enabled = no;
	mpdu->fcf.MAC_fcf_Frame_Ver = 0x00;

	mpdu->source = mpib->macLongAddress;
// Set the PANid to 0xffff
	mpdu->destination.PANid = 0xffff;

	//1. Create Mac frame data
	MAC_createFrame(mpdu, fr);
    
    //2. device's command ID
	SET_FRAME_DATA(fr, ORPHAN_NOTIFY, 1); //TODO: this should be the capability field I should have a procedure on how to get this.

	MAC_setTxCB(&MAC_orphan_cb);


	free(mpdu);
	free_frame(fr);
}
void MAC_orphan_cb(phy_trac_t trac){
	switch(trac){
		case(MAC_SUCCESS):

			break;

		case(MAC_NO_ACK):
			break;

		case(MAC_TRANSACTION_EXPIRED):
			break;

//	TODO: 	I need to create a timer once it times out that I send back a failure due to no data
	}
}

void MAC_mlme_orphanConfirm(mac_status_t status){
	(orphanHandler)(status);
}
