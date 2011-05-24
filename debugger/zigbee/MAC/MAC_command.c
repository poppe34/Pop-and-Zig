/*
 * mac_command.c
 *
 *  Created on: Sep 12, 2010
 *      Author: mpoppe
 */
#include "frame.h"
#include <MAC/mac_prototypes.h>

uint8_t temp;


uint8_t data_request(void){
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


	MAC_createFrame(mpdu, fr);

	seq_num = mpdu->seq_num;

	free_frame(fr);
	free(mpdu);

	return seq_num;

}
