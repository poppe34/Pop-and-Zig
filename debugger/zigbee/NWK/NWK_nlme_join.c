/*
 * NWK_nlme_rejoin.c
 *
 *  Created on: Oct 27, 2010
 *      Author: mpoppe
 */
#include <frame.h>
#include <NWK/NWK_prototypes.h>
#include "MAC/mac_prototypes.h"
#include "MAC/MAC_mlme.h"

nwk_join_t *join;
typedef void (*nwk_joinHandler_t)(nwk_status_t status);
nwk_joinHandler_t joinHandler;

uint64_t joinExtPANid;

void NWK_nlme_joinReq(nwk_join_t *jn, void *cb){
	joinHandler = (nwk_joinHandler_t *)cb;
	mlme_assoc_t *assoc = (mlme_assoc_t *)malloc(sizeof(mlme_assoc_t));
	nwk_neigh_t *neighbor;
	mac_scan_t *scan = (mac_scan_t *)malloc(sizeof(mac_scan_t));

	switch(jn->rejoinNetwork){
	case(JOINED_THRU_ASSOC):

		neighbor = NWK_getBestAddrForNetwork(jn->extedPANid);

		assoc->LogicalChannel = neighbor->logicalChannel;
		assoc->Coord = neighbor->shortAddr;

		//MAC_mlme_assocReq(assoc);

		free(assoc);
	break;
	case(JOINED_DIRECTLY):
	break;
	case(JOINED_NWK_REJOIN):
		scan->ScanChannels = jn->scanChannels;
		scan->ScanDuration = jn->duration;
		scan->type = ACTIVE_SCAN;

		//TODO: add the logic to do a MAC scan of the channels given in jn
		joinExtPANid = jn->extedPANid;
		NWK_beaconFilterControl(joinExtPANid);
		MAC_mlme_scanReq(scan);
	break;
	}//end switch
//TODO:	add the logic if the network is already joined to issue an indication saying INVALID_REQUEST
	free(assoc);
	free(scan);
}//end NWK_nlme_joinReq

void NWK_nlme_rejoinScan_cb(mac_scanResult_t *result){

	nwk_neigh_t *parent = NWK_getBestAddrForNetwork(joinExtPANid);
	nwk_nib_t *nnib = NWK_getNIB();
	npdu_t *npdu = (npdu_t *)malloc(sizeof(npdu_t));
	frame_t *fr = frame_new();

	npdu->nwk_frame_control.NWK_dest_IEEE = no;
	npdu->nwk_frame_control.NWK_discover_rt = SUPPRESS_DISCOVER;
	npdu->nwk_frame_control.NWK_frame_type = NWK_COMMAND;
	npdu->nwk_frame_control.NWK_multicast = no;
	npdu->nwk_frame_control.NWK_protocol_ver = nwkcProtocolVersion;
	npdu->nwk_frame_control.NWK_security = no;
	npdu->nwk_frame_control.NWK_source_rt = no;
	npdu->nwk_frame_control.NWK_source_IEEE = yes;

	if(nnib->nwkNetworkAddress.shortAddr == 0xffff){
		if(nnib->nwkAddrAlloc == 0x02){
			 NWK_setShortAddr((uint16_t)rand());
		}
	}

	npdu->source = nnib->nwkNetworkAddress;

	npdu->destination = parent->shortAddr;
	npdu->destExtAddr = parent->extendAddr.extAddr;
	npdu->sequence_num = nnib->nwkSequenceNumber++;
//	TODO:	fix the delay time below I don't have aresponsewaittime declared
//	add_to_time_qsm(&NWK_nlme_rejoinUnsuccess_cb, null, aResponseWaitTime);

	MAC_setTxCB(&NWK_nlme_joinTx_cb);
	mac_fcf_t fcf;

	fcf.MAC_fcf_Frame_Type = MAC_DATA;
	fcf.MAC_fcf_DstAddr_Mode = SHORT_ADDRESS;
	fcf.MAC_fcf_SrcAddr_Mode = SHORT_ADDRESS;
	fcf.MAC_fcf_PANid_Compression = yes;
	fcf.MAC_fcf_Frame_Pending = no;
	fcf.MAC_fcf_Ack_Request = yes;
	fcf.MAC_fcf_Sec_enabled = no;
	fcf.MAC_fcf_Frame_Ver = 0x00;

	nwk_capInfo_t capInfo;
	capInfo.allocAddr = yes;//0x01 means that it will issue and addr
	capInfo.altPanCoord = no;
	capInfo.devType = 0x00;
	capInfo.powerSource = 0x01;//power is from mains
	capInfo.recWhenIdle = yes;
	capInfo.secEnabled = no;
	uint8_t *tempCapInfo = &capInfo;


//Add room for CRC
	SET_FRAME_DATA(fr, 0x0000, 2);

//Add Capability info
	SET_FRAME_DATA(fr, *tempCapInfo, 1);

	SET_FRAME_DATA(fr, NWK_REJOIN_REQUEST, 1);

	NWK_createFrame(npdu, fr);

	NWKtoMAC_bridge(&fcf, npdu, fr);
}

void NWK_nlme_rejoinUnsuccess_cb(void){

}

void NWK_nlme_joinNetDiscov_cb(nwk_netDisc_t *netDisc){

	nwk_neigh_t *parent;


}

void NWK_nlme_joinTx_cb(uint8_t num){

}
void NWK_nlme_joinMacAssoc_cb(mac_status_t status){
	(joinHandler)(status);
}

void NWK_nlme_joinConfirm(nwk_status_t status){
	(joinHandler)(status);
}
