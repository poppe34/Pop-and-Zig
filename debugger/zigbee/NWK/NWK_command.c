/*
 * NWK_command.c
 *
 *  Created on: Oct 15, 2010
 *      Author: mpoppe
 */
#include <frame.h>
#include <NWK/NWK_prototypes.h>
#include "MAC/mac_prototypes.h"


nwk_status_t NWK_status_cmd(nwk_status_code_t code, uint16_t addr){
	frame_t *fr = frame_new();
	npdu_t *npdu =(npdu_t *)malloc(sizeof(npdu_t));
	mac_pib_t *mpib = get_macPIB();
	mac_fcf_t *fcf = (mac_fcf_t *)malloc(sizeof(mac_fcf_t));

	nwk_status_t status = NWK_SUCCESS;

	npdu->nwk_frame_control.NWK_dest_IEEE = no;
	npdu->nwk_frame_control.NWK_discover_rt = SUPPRESS_DISCOVER;
	npdu->nwk_frame_control.NWK_frame_type = NWK_COMMAND;
	npdu->nwk_frame_control.NWK_multicast = no;
	npdu->nwk_frame_control.NWK_protocol_ver = nwkcProtocolVersion;
	npdu->nwk_frame_control.NWK_security = no;
	npdu->nwk_frame_control.NWK_source_rt = no;
	npdu->nwk_frame_control.NWK_source_IEEE = no;

	npdu->destination.mode = SHORT_ADDRESS;
	npdu->destination.shortAddr = addr;
	npdu->destination.PANid	= mpib->macPANid;


	fcf->MAC_fcf_Frame_Type = MAC_DATA;
	fcf->MAC_fcf_Sec_enabled = no;
	fcf->MAC_fcf_Frame_Pending = no;
	fcf->MAC_fcf_Ack_Request = yes;
	fcf->MAC_fcf_PANid_Compression = yes;
	fcf->MAC_fcf_DstAddr_Mode = SHORT_ADDRESS;
	fcf->MAC_fcf_Frame_Ver = MAC_COMPATIBLE_WITH_802_15_4_2003;
	fcf->MAC_fcf_SrcAddr_Mode = SHORT_ADDRESS;

	NWK_createFrame(npdu, fr);
	NWKtoMAC_bridge(fcf, npdu, fr);
	
	SET_FRAME_DATA(fr, NWK_NETWORK_STATUS, 1);

// Add Destination Address to the payload
	SET_FRAME_DATA(fr, addr, 2);

// Add the status code to the payload
	SET_FRAME_DATA(fr, code, 1);

    frame_sendWithFree(fr);
	free(npdu);
	free(fcf);
	return status;
}//
nwk_status_t NWK_routeRequest(uint8_t addr[]){
	frame_t *fr = frame_new();
	npdu_t *npdu =(npdu_t *)malloc(sizeof(npdu_t));
	mac_pib_t *mpib = get_macPIB();
	mac_fcf_t *fcf = (mac_fcf_t *)malloc(sizeof(mac_fcf_t));

	nwk_status_t status = NWK_SUCCESS;

//	TODO:	Need to add code for multicasts

	npdu->nwk_frame_control.NWK_dest_IEEE = no;
		npdu->nwk_frame_control.NWK_discover_rt = SUPPRESS_DISCOVER;
		npdu->nwk_frame_control.NWK_frame_type = NWK_COMMAND;
		npdu->nwk_frame_control.NWK_multicast = no;
		npdu->nwk_frame_control.NWK_protocol_ver = nwkcProtocolVersion;
		npdu->nwk_frame_control.NWK_security = no;
		npdu->nwk_frame_control.NWK_source_rt = no;
		npdu->nwk_frame_control.NWK_source_IEEE = no;

		npdu->destination.mode = SHORT_ADDRESS;
		npdu->destination.shortAddr = 0xffff;

		npdu->destination.PANid	= mpib->macPANid;

		fcf->MAC_fcf_Frame_Type = MAC_DATA;
		fcf->MAC_fcf_Sec_enabled = no;
		fcf->MAC_fcf_Frame_Pending = no;
		fcf->MAC_fcf_Ack_Request = no;
		fcf->MAC_fcf_PANid_Compression = yes;
		fcf->MAC_fcf_DstAddr_Mode = SHORT_ADDRESS;
		fcf->MAC_fcf_Frame_Ver = MAC_COMPATIBLE_WITH_802_15_4_2003;
		fcf->MAC_fcf_SrcAddr_Mode = SHORT_ADDRESS;

	// Room for CRC
		SET_FRAME_DATA(fr, 0x0000, 2);

		NWK_createFrame(npdu, fr);
		NWKtoMAC_bridge(fcf, npdu, fr);

		frame_sendWithFree(fr);
		free(npdu);
		free(fcf);

		return status;

}
