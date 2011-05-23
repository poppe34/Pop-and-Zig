/*
 * NWK_trx.c
 *
 *  Created on: Oct 14, 2010
 *      Author: mpoppe
 */

#include <frame.h>
#include <NWK/NWK_prototypes.h>
#include "MAC/MAC_prototypes.h"

uint8_t NWK_sequence_num = 0x11;
trx_cb_t trx_cb;

nwk_status_t NWK_nlde_data(frame_t *fr){

	//TODO: need to setup the status
	nwk_status_t status = NWK_SUCCESS;
	npdu_t *npdu = (npdu_t *)malloc(sizeof(npdu_t));

	npdu->nwk_frame_control.NWK_frame_type = NWK_data;
	npdu->nwk_frame_control.NWK_protocol_ver = nwkcProtocolVersion;
	npdu->nwk_frame_control.NWK_discover_rt = no;
	npdu->nwk_frame_control.NWK_multicast = 0x01;
	npdu->nwk_frame_control.NWK_security = no;
	//npdu->src_route = zig->nlde.DiscoverRoute; //TODO add logic to add route discovery
	npdu->nwk_frame_control.NWK_dest_IEEE = no;
	npdu->nwk_frame_control.NWK_source_IEEE = no;

	//npdu->multicast_control.multicastMode = MMF_mem;

	//npdu->dest = zig->nlde.dstAddr.s_addr;
	//npdu->srcAddr = SHORT_ADDR;//TODO this needs to be changed to the macShortAddress from MAC PIB
	npdu->radius = 0x06;//TODO: if the radius is not given a default value of 2x nwkMaxDepth needs to be given(using temp value)
	npdu->sequence_num = (get_NWK_Sequence_num());
	if(npdu->nwk_frame_control.NWK_dest_IEEE){
		//TODO add IEEE address and compare it from the nwkAddressMap and neighbor table
	}
	if(npdu->nwk_frame_control.NWK_source_IEEE){
		//TODO add this field initially I will never need it
	}

	if(!(npdu->nwk_frame_control.NWK_multicast = 0x01)) {
		/*TODO add code here for multicast frame not ready yet making sure everything
		else works first */
	}


	NWK_createFrame(npdu, fr);
	free(npdu);
	//MAC_mcps_data(zig);

	return status;
}

void NWK_createFrame(npdu_t *npdu, frame_t *fr){


	mac_pib_t *mpib = get_macPIB();
	nwk_nib_t *nnib = NWK_getNIB();

	npdu->source = nnib->nwkNetworkAddress;

//Add Multicast control frame if needed
	if(npdu->nwk_frame_control.NWK_multicast){
		uint8_t *temp_mc = &npdu->multicast_control;
		SET_FRAME_DATA(fr, *temp_mc, 1);
	}

//Add Source IEEE addr if needed
	if(npdu->nwk_frame_control.NWK_source_IEEE){
		SET_FRAME_DATA(fr, mpib->macLongAddress.extAddr, 8);
	}

//Add Desination IEEE addr if needed
	if(npdu->nwk_frame_control.NWK_dest_IEEE){
		SET_FRAME_DATA(fr, npdu->destExtAddr, 8);
	}
//Add sequence Num
	SET_FRAME_DATA(fr, get_NWK_Sequence_num(), 1);

// Add Radius
	SET_FRAME_DATA(fr, 0x06, 1);//TODO: this value should be temp I need a variable in its place

// Add Source Addr
	SET_FRAME_DATA(fr, npdu->source.shortAddr, 2);

// Add Dest Addr
	SET_FRAME_DATA(fr, npdu->destination.shortAddr, 2);

// Add NWK fcf
	uint16_t *tempfcf = &npdu->nwk_frame_control;
	SET_FRAME_DATA(fr, *tempfcf, 2);

	//TODO: lookup (in the spec 3.6.1.6)and figure out the routeing address and how to implement it.

	/*TODO a bunch more to add to the frame but I am not ready to do that yet...
	 * Troubleshooting needs to be done before I get to far ahead of myself.  Validate
	 * everything in section 3.6.2 of the zigbee spec.
	 */
//	RF230frameWrite((nlde->nsdu)-(nlde->NsduLength), nlde->NsduLength);
	//TODO: I need to create the pdu for the MAC sublayer.
	/*TODO: Security subclause is 4.2.2, I need to look at it and implement it.
	 * 		Add a section called security service provider (SSP)	 */
}


uint8_t get_NWK_Sequence_num(void){
	return(NWK_sequence_num++);
}

void NWK_setTxCB(trx_cb_t *tb){
	trx_cb = tb;
}

void NWK_txStatus(phy_trac_t trac){
	(trx_cb)(trac);
}
/*EOF*/
