/*
 * zigbee_task.c
 *
 * Created: 4/29/2011 3:08:32 PM
 *  Author: mpoppe
 */ 
#include "zigbee_task.h"


#ifdef DEBUG_SERVER
LIST(zPackets);

void zigbee_task_init(void)
{
	list_init(zPackets);
}

void zigbee_SubtaskHandler(packet_t *pkt)
{
	switch (pkt->subTask)
	{
		case zigbeePacketQty:
		    switch(pkt->dir)
			{
				case to_usb:
				spi_sendToDev(pkt);
				break;
				case from_device:
				zigbee_packetQTY();
				break;
			}
		break;
		
		case zigbeePacketFirst:
		    switch(pkt->dir)
			{
				case from_device:
				zigbee_newPacket(pkt->buf, pkt->len)
				break;
				case from_usb:
				zigbee_packetFirst();
				break;
				case to_usb:
				spi_sendToDev(pkt);
				break;
			}
		break;
	}
}
void zigbee_packetQTY(void)
{
	packet_t *pkt = TM_newPacket();
	if(pkt)
	{
		uint8_t test[128];
		
		for(uint8_t x = 0, x<64, x++)
		{
			test[x] = x;
		}		
		zigbee_newPacket(test, 64);
			
	    pkt->len = BYTES_NEEDED_FOR_LENGTH;
	    pkt->task = task_zigbee;
	    pkt->subTask = zigbeePacketQty;
	    pkt->buf[0] = list_count(zPackets);
	    pkt->dir = to_usb;
	}	
}

void zigbee_packetFirst(void)
{
	if(list_length(zPackets))
	{
		packet_t *pkt = list_pop(zPackets);
		
		if(pkt)
		{
			spi_sendToDev(pkt);
		}
		else alarm("Request for first Zigbee Packet with non avail");
		
	}
}
#endif



void zigbee_newPacket(uint8_t *buf, uint8_t zlen)
{
	packet_t *pkt = TM_newPacket();
	
	pkt->len = zlen;
	pkt->ptr = pkt->buf;
	
	while(zlen)
	{
		*pkt->ptr++ = *buf++;
		zlen--;
	}
#ifdef DEBUG_CLIENT
	pkt->dir = to_usb;
#endif
	pkt->task = task_zigbee;
	pkt->subTask = zigbeePacketFirst;
	
	spi_sendToDev(pkt);
#ifdef DEBUG_SERVER
    list_add(zPacket,pkt);
	TM_removeTask(pkt);
#endif
}
