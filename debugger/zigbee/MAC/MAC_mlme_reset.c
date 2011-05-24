/*
 * MAC_mlme_reset.c
 *
 *  Created on: Oct 14, 2010
 *      Author: mpoppe
 */

#include <frame.h>
#include <mac/mac_prototypes.h>

mac_status_t mlme_reset_request(void){
	return(mac_init());
}
