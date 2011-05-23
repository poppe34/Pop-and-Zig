/*
 * alarm_task.c
 *
 * Created: 3/10/2011 9:43:45 AM
 *  Author: matt
 */ 
#include "list.h"
#include "alarms_task.h"
#include "stdio.h"
#include "stdarg.h"
#include "task_master.h"

/*********************************************************
/*              Variable Declarations
/********************************************************/
LIST(alarmLog);



/************************************************************************/
/* Function:    alarm_task_init

/* Description: Initializes the alarmLog

/* Arguments:   None

/* Return:      None                     
/************************************************************************/
void alarm_task_init(void)
{
	list_init(alarmLog);
	alarm("This is just a device Test Alarm");
}

/************************************************************************/
/* Function:    alarm_new

/* Description: Takes an alarm string in and allocates memory for the alarm.
                Then the alarm is added to the alarmLog list 
				
/* Arguments:   alarm string

/* Return:      nothing                      
/************************************************************************/
void alarm_new(uint8_t lvl, char *str, ...)
{

	va_list arg;
	va_start(arg, str);
	
	if(lvl <= VERBOSE_LEVEL)
	{
	  if(*str)
		{
			alarm_t *alrm = alarm_newAlrm();
            if(alrm)
			{
	            alrm->length = 0;
	            char *ptr = alrm->name;
				
		        vsprintf(alrm->name, str, arg);							
		   		    
			    while(*ptr)
			    {
		            alrm->length++;
		            ptr++;
				}
			
			    alrm->name[(alrm->length)] = 0;
			    alrm->length++;
			    //if(alrm->length > 61)
				    //alarm("Alarm Length is to Large");
			}					
		}					
	    
	}
	va_end(arg);
}


/************************************************************************/
/* Function:    alarm_free

/* Description: This frees the alarm from memory

/* Arguments:   Alarm string
/************************************************************************/

void alarm_free(alarm_t *alrm)
{
	list_remove(alarmLog, alrm);
	free(alrm);
}

void alarm_sendFirst(void)
{
	while(list_length(alarmLog))
	{
	    alarm_t *alrm = list_head(alarmLog);
	
	    if(alrm == NULL)
	    {
		    alarm("PC requested alarms when none were available");		
	    }
	    else
	    {
			if(alrm->length)
			{
		        packet_t *pkt = TM_newPacket();
				if(pkt)
				{
					pkt->dir = to_usb;
					pkt->task = task_alarm;
					pkt->subTask = alarm_first;
					pkt->len = alrm->length;

					for(uint8_t x=0; x<alrm->length; x++)
					pkt->buf[x] = alrm->name[x];

					alarm_free(alrm);
				}				
			}				
        }
	}			
}

void alarm_recQty(void)
{
	packet_t *pkt = TM_newPacket();
	pkt->dir = to_usb;
	pkt->task = task_alarm;
	pkt->subTask = alarm_qty;
	pkt->len = 1;
	pkt->buf[0] = list_length(alarmLog);
	
}

void alarm_subTaskHandler(packet_t *pkt)
{
	switch (pkt->subTask)
	{
		case alarm_qty:
		    switch(pkt->dir)
			{
				case to_device:
				
				break;
			    case from_device:
			    alarm_recQty();
				break;
				case from_usb:
				break;
				case to_usb:
				spi_sendToDev(pkt);
				break;
			}				
			
			
		break;
		case alarm_first:
		    switch(pkt->dir)
			{
				case to_device:
				
				break;
			    case from_device:
			        alarm_sendFirst();
				break;
				case from_usb:
				break;
				case to_usb:
				    spi_sendToDev(pkt);
				break;
			}			
		break;
		case alarm_clear:
		    alarm_new(5, "Request for unclear is not implemented");
		break;
		default:
		alarm_new(5, "Unknown Alarm Sub Task");
		break;
	}

}

void alarm_sendQty(void)
{
	packet_t *pkt = TM_newPacket();
	
	pkt->dir = to_usb;
	pkt->task = task_alarm;
	pkt->subTask = alarm_qty;
	
	pkt->len = 1;
	pkt->buf[0] = (list_length(alarmLog));	
}

alarm_t *alarm_newAlrm(void)
{
	alarm_t *alrm;
	
	if(list_length(alarmLog)<20)
	    {
			alrm = (alarm_t *)malloc(sizeof(alarm_t));
			
			list_add(alarmLog, alrm);
			return alrm;
		}
		
	return alrm;
}