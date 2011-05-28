// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief
 *
 * \par Application note:
 *
 *
 * \par Documentation
 *
 *
 *
 * \author
 *
 *
 *
 * $Id:
 *
 * Copyright (c) 2008 ,
 *
 *
 *****************************************************************************/

/*================================= INCLUDES         =========================*/
#include "compiler.h"
#include "frame.h"

#include "MAC/mac_prototypes.h"
#include "MAC/mac.h"
#include "MAC/MAC_command.h"

uint8_t temp = 0;

/*================================= MACROS           =========================*/
/*================================= TYEPDEFS         =========================*/
/*================================= GLOBAL VARIABLES =========================*/
uint8_t MACreceiveBeaconOnly = 0x00;
/*================================= LOCAL VARIABLES  =========================*/
/*================================= PROTOTYPES       =========================*/
/*================================= SOURCE CODE      =========================*/
mac_status_t MAC_dataRequest(addr_t *destAddr, addr_mode_t srcMode, frame_t *fr)
{

	mac_pib_t *mpib = get_macPIB();
    mpdu_t *mpdu = ((mpdu_t *)malloc(sizeof(mpdu_t)));
	
	mac_status_t status = MAC_SUCCESS;

	mpdu->seq_num = get_MAC_seqNum();

    
    //Setup for the FCF
	mpdu->fcf.MAC_fcf_Frame_Type = MAC_COMMAND;
	mpdu->fcf.MAC_fcf_DstAddr_Mode = mpdu->destination.mode;
	mpdu->fcf.MAC_fcf_SrcAddr_Mode = srcMode;
	mpdu->fcf.MAC_fcf_PANid_Compression = NO;
	mpdu->fcf.MAC_fcf_Frame_Pending = NO;
	mpdu->fcf.MAC_fcf_Ack_Request = YES;
	mpdu->fcf.MAC_fcf_Sec_enabled = NO;
    
	if(mpdu->source.mode == MAC_SHORT_ADDRESS){
		mpdu->source = mpib->macShortAddress;
	}
	else{
		mpdu->source = mpib->macLongAddress;
	}

	//mpdu->security = //TODO:I need to add the security header... For now there will be no header

	MAC_createFrame(mpdu, fr);

    
    frame_sendWithFree(fr);
    free(mpdu);
    
	return status;

}
void MAC_dataIndication(frame_t *fr){


}//end MAC_DATA_Indication

void change_receive_mode(uint8_t status){

//	TODO:	This is completely off... I will need to do alot of work here...> I might also set Promicious mode here too... right now I don't know
	MACreceiveBeaconOnly = status;
}



/*EOF*/
