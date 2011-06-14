/*
 * NWK_command.c
 *
 *  Created on: Oct 15, 2010
 *      Author: mpoppe
 */
#include <frame.h>

#include <NWK/NWK_prototypes.h>
#include "nwk/NWK_nlde.h"

#include "MAC/mac_prototypes.h"
#include "mac/MAC_mcps.h"

nwk_status_t NWK_status_cmd(nwk_status_code_t code, uint16_t addr){
	frame_t *fr = frame_new();
	fr->payload = frame_hdr(payload);
	npdu_t *npdu =(npdu_t *)malloc(sizeof(npdu_t));
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));
	
	mac_pib_t *mpib = get_macPIB();
	mac_fcf_t *fcf = (mac_fcf_t *)malloc(sizeof(mac_fcf_t));

	nwk_status_t status = NWK_SUCCESS;

	npdu->fcf.NWK_dest_IEEE = no;
	npdu->fcf.NWK_discover_rt = SUPPRESS_DISCOVER;
	npdu->fcf.NWK_frame_type = NWK_COMMAND;
	npdu->fcf.NWK_multicast = no;
	npdu->fcf.NWK_protocol_ver = nwkcProtocolVersion;
	npdu->fcf.NWK_security = no;
	npdu->fcf.NWK_source_rt = no;
	npdu->fcf.NWK_source_IEEE = no;

	npdu->destination.mode = SHORT_ADDRESS;
	npdu->destination.shortAddr = addr;
	npdu->destination.PANid	= mpib->macPANid;


	mpdu->fcf.MAC_fcf_Frame_Type = MAC_DATA;
	mpdu->fcf.MAC_fcf_Sec_enabled = no;
	mpdu->fcf.MAC_fcf_Frame_Pending = no;
	mpdu->fcf.MAC_fcf_Ack_Request = yes;
	mpdu->fcf.MAC_fcf_PANid_Compression = yes;
	mpdu->fcf.MAC_fcf_DstAddr_Mode = SHORT_ADDRESS;
	mpdu->fcf.MAC_fcf_Frame_Ver = MAC_COMPATIBLE_WITH_802_15_4_2003;
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = SHORT_ADDRESS;

	NWK_createFrame(npdu, fr);
	
	MAC_mcps_dataReq(mpdu, fr);
	
	SET_FRAME_DATA(fr->payload, NWK_NETWORK_STATUS, 1);

// Add Destination Address to the payload
	SET_FRAME_DATA(fr->payload, addr, 2);

// Add the status code to the payload
	SET_FRAME_DATA(fr->payload, code, 1);

    frame_sendWithFree(fr);
	free(npdu);
	free(mpdu);
	
	return status;
}//
nwk_status_t NWK_routeRequest(uint8_t addr[]){
	frame_t *fr = frame_new();
	fr->payload = frame_hdr(payload);
	
	npdu_t *npdu =(npdu_t *)malloc(sizeof(npdu_t));
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));
	mac_pib_t *mpib = get_macPIB();
	mac_fcf_t *fcf = (mac_fcf_t *)malloc(sizeof(mac_fcf_t));

	nwk_status_t status = NWK_SUCCESS;

//	TODO:	Need to add code for multicasts

	npdu->fcf.NWK_dest_IEEE = no;
		npdu->fcf.NWK_discover_rt = SUPPRESS_DISCOVER;
		npdu->fcf.NWK_frame_type = NWK_COMMAND;
		npdu->fcf.NWK_multicast = no;
		npdu->fcf.NWK_protocol_ver = nwkcProtocolVersion;
		npdu->fcf.NWK_security = no;
		npdu->fcf.NWK_source_rt = no;
		npdu->fcf.NWK_source_IEEE = no;

		npdu->destination.mode = SHORT_ADDRESS;
		npdu->destination.shortAddr = 0xffff;

		npdu->destination.PANid	= mpib->macPANid;

		mpdu->fcf.MAC_fcf_Frame_Type = MAC_DATA;
		mpdu->fcf.MAC_fcf_Sec_enabled = no;
		mpdu->fcf.MAC_fcf_Frame_Pending = no;
		mpdu->fcf.MAC_fcf_Ack_Request = no;
		mpdu->fcf.MAC_fcf_PANid_Compression = yes;
		mpdu->fcf.MAC_fcf_DstAddr_Mode = SHORT_ADDRESS;
		mpdu->fcf.MAC_fcf_Frame_Ver = MAC_COMPATIBLE_WITH_802_15_4_2003;
		mpdu->fcf.MAC_fcf_SrcAddr_Mode = SHORT_ADDRESS;

	// Room for CRC
		NWK_createFrame(npdu, fr);
		
		MAC_mcps_dataReq(mpdu, fr);

		frame_sendWithFree(fr);
		free(npdu);
		free(mpdu);
		free(fcf);

		return status;

}
