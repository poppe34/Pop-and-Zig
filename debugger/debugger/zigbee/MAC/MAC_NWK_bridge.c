/*
 * MAC_NWK_bridge.c
 *
 *  Created on: Oct 15, 2010
 *      Author: mpoppe
 */
#include <frame.h>
#include <NWK/NWK_prototypes.h>
#include <MAC/mac_prototypes.h>

mac_status_t NWKtoMAC_bridge(mac_fcf_t *fcf, npdu_t *npdu, frame_t *fr){

	mac_status_t status;
	mpdu_t *mpdu = (mpdu_t *)malloc(sizeof(mpdu_t));

	mpdu->fcf = *fcf;
	mpdu->destination = npdu->destination;
	mpdu->source.mode = MAC_SHORT_ADDRESS;

	MAC_dataRequest(mpdu, fr);
	free(mpdu);

	return status;
}
