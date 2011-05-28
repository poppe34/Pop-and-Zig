/*
 * MAC_mlme_disassociate.c
 *
 *  Created on: Oct 13, 2010
 *      Author: mpoppe
 */
#include "frame.h"

#include <mac/mac_prototypes.h>
#include "MAC/MAC_command.h"
#include "MAC/MAC_mlme.h"

void mac_mlme_disassocReq(mac_disassoc_reason_t reason){

    mac_pib_t *mpib = get_macPIB();

//Format the frame and send it out
	MAC_setTxCB(&MAC_disassoc_cb);
    MAC_disassocCommand(&(mpib->macCoordShortAddress));
	

}//end MAC_mlme_disassocReq

void MAC_disassoc_cb(phy_trac_t trac){
	switch(trac){
	case(TRAC_SUCCESS):
		MAC_mlme_disassocConf(MAC_SUCCESS);
	break;

	case(TRAC_NO_ACK):
		MAC_mlme_disassocConf(MAC_NO_ACK);
	break;

	case(TRAC_CHANNEL_ACCESS_FAILURE):
		MAC_mlme_disassocConf(MAC_ACCESS_FAILURE);
	break;
	}
}

void MAC_mlme_disassocConf(mac_status_t status){
	
}
