/*
 * MAC_ack.c
 *
 *  Created on: Sep 23, 2010
 *      Author: mpoppe
 */
#include "conf_zigbee.h"
#include <frame.h>

#include "MAC/MAC_mlme.h"
#include "MAC/mac_prototypes.h"
#include "MAC/MAC_command.h"

#include "MISC/qsm.h"

#include "alarms_task.h"

typedef void (*mac_assocHandler_t)(mac_status_t status);
mac_assocHandler_t assocHandler;

void MAC_mlme_assocReq(addr_t *destAddr, uint32_t page, uint8_t channel, uint8_t capabilites, security_t *sec)
{
    frame_t *fr = frame_new();
	
	alarm_new(9, "MLME associate Request initiated");
	
    MAC_setCurrentChannel(channel);

// TODO: I need to add the security values...
// TODO: I need to set up the page methods....

	MAC_setTxCB(&MAC_mlme_assocReq_cb);
	MAC_assocRequestCommand(destAddr, capabilites, sec);	
}

void MAC_mlme_assocInd(frame_t *fr)
{
    
}

void MAC_mlme_assocData(frame_t *fr){

}

void MAC_mlme_assocReq_cb(phy_trac_t trac){
	
	switch(trac){
	case(TRAC_SUCCESS):
     //  add_to_time_qsm(&) 
        
	break;

	case(TRAC_NO_ACK):
//		MAC_mlme_assocConf(MAC_NO_ACK);
	break;

	case(TRAC_CHANNEL_ACCESS_FAILURE):
//		MAC_mlme_assocConf(MAC_ACCESS_FAILURE, void);
	break;

	case(TRAC_INVALID):
//		MAC_mlme_assocConf(MAC_INVALID_ADDRESS);
	break;

	case(TRAC_SUCCESS_DATA_PENDING):
		break;
	}
}

void MAC_mlme_assocHandler(frame_t *fr)
{
	alarm_new(9, "Received an Assoc Request");
	//	TODO:	Write to code to receive a long addr too....
	uint8_t	command_status = GET_FRAME_DATA(fr, 1);
    short_addr_t addr = GET_FRAME_DATA(fr, 2);

    
}

void MAC_assocConf(mac_status_t status, short_addr_t *addr, security_t *sec)
{
	alarm_new(9, "Received an Associate Confirm with the status: %x", status);

	status = status;
	addr = addr;
	sec = sec;
	
}

