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
#include "frame.h"
#include "MAC/mac_prototypes.h"

uint8_t temp = 0;

/*================================= MACROS           =========================*/
/*================================= TYEPDEFS         =========================*/
/*================================= GLOBAL VARIABLES =========================*/
uint8_t MACreceiveBeaconOnly = 0x00;
/*================================= LOCAL VARIABLES  =========================*/
/*================================= PROTOTYPES       =========================*/
/*================================= SOURCE CODE      =========================*/
mac_status_t MAC_dataRequest(mpdu_t *mpdu, frame_t *fr){

	mac_pib_t *mpib = get_macPIB();

	mac_status_t status = MAC_SUCCESS;

	mpdu->seq_num = get_MAC_seqNum();

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

	mac_command_type_t command;
	mac_frame_type_t type;

	mac_pib_t *mpib = get_macPIB();
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));

	if(MAC_breakdownFrame(mpdu, fr)){
		free(mpdu);
		return;

	}

	type = mpdu->fcf.MAC_fcf_Frame_Type;

	if(MACreceiveBeaconOnly == yes){
		if(type != MAC_BEACON){
			free(mpdu);
			return;
		}
	}

	if(type == MAC_BEACON){
		MAC_beaconHandler(mpdu, fr);

	}

	if(type == MAC_DATA){

	}
	if(type == MAC_ACK){

//	TODO:	I need to create a ack handler...
		add_ack(mpdu->seq_num);
	}

	if(type == MAC_COMMAND){
/*
 * 		This section will have the proper location for each mac commands that it receives
 */


		command = *(fr->ptr)++;

		switch(command) {

		case(ASSOC_REQUEST):

		break;
		case(ASSOC_RESPONCE):
			temp++;
			MAC_mlme_assocData(fr);
		break;

		case(BEACON_REQUEST):

					MAC_beacon();
		break;


//	TODO:	I need to add the other types of command frames that i could receive....
		}//end switch command

	}//end if

	free(mpdu);
}//end MAC_DATA_Indication

void change_receive_mode(uint8_t status){

//	TODO:	This is completely off... I will need to do alot of work here...> I might also set Promicious mode here too... right now I don't know
	MACreceiveBeaconOnly = status;
}



/*EOF*/
