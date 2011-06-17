/*
 * NWK_neighborTable.c
 *
 *  Created on: Oct 20, 2010
 *      Author: mpoppe
 */

#include <frame.h>
#include <NWK/NWK_prototypes.h>

#include "list.h"
LIST(neighbor_table);

void NWK_neighborTableInit(void){
list_init(neighbor_table);
}
/*
void NWK_addNeighbor(nwk_neigh_t *tbl){

	list_add(neightbor_table, tbl);

	if(table->count == 0){
		table->first = tbl;
		table->count = 1;
		return;
	}

	temp = table->first;

	if(table->count > 0){
		for(uint8_t x=1; x<table->count; x++){
			temp = temp->next;
		}

		temp->next = tbl;
	}


	table->count++;

}

void NWK_removeNeighbor(nwk_neigh_t *tbl){
	nwk_neigh_t *temp;

	temp = table->first;
	if(table->count == 0){ return;}
	if(table->count == 1){
		if(table->first == tbl){
			table->count--;
			free(table->first);
		}
		return;
	}
	for(uint8_t x=0; x<table->count; x++){
		if(tbl == temp->next){
			temp->next = (temp->next)->next;
			free(tbl);
			return;
		}
		temp = temp->next;
	}
}
void NWK_updateNeighTable(nwk_neigh_t *tbl){
	uint8_t x, y, equal;
	nwk_neigh_t *temp = table->first;
/*
	for(x=0; x<table->count; x++){
		if(temp->shortAddr.shortAddr == tbl->shortAddr.shortAddr){
			//TODO: I Need to add the update code
			return;
		}//end if
	}//end for
	for(x=0; x<table->count; x++){
		if(temp->extendAddr.extAddr == temp->extendAddr.extAddr){
			return;
		}//end if

		if(temp->shortAddr.shortAddr == tbl->shortAddr.shortAddr){
			return;
		}//end if
	}//end for

	NWK_addNeighbor(tbl);
}
*/

nwk_neigh_t *NWK_getBestAddrForNetwork(uint64_t extendPANid){
	uint8_t x, lqi= 0x00;
	nwk_neigh_t *current, *best;
	current = (nwk_neigh_t *)(list_head(neighbor_table));

	for(x=0; x<(list_length(neighbor_table)); x++){

		if(extendPANid == current->extendPANid){

				best = current;

		}//end if
		current = current->next;
	}//end for
	return best;
}

list_t NWK_getNeighTable(void){
	return neighbor_table;
}
/*
nwk_neigh_t *NWK_getNeighTable(void){
//	led_on_byte(table->first->extendPANid);
	return table->first;
}

uint8_t NWK_getNeighTableCount(void){
	return table->count;
}
*/
