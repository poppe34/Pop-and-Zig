//
//  packets.m
//  debugger
//
//  Created by Matt Poppe on 4/24/11.
//  Copyright 2011 Poppe. All rights reserved.
//

#import "packets.h"
#import "zigPacket.h"

@implementation packets


@synthesize zigbeePackets, addAlarm;

- (id)init
{
    self = [super init];
    if (self) 
    {
        zigbeePackets = [[NSMutableArray alloc] init];
    }
    zigPacket *pkt = [[zigPacket alloc]init];
    
    [zigbeePackets addObject:pkt];
    
    [zigbeeView reloadData];
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}


- (int)numberOfRowsInTableView:(NSTableView *)tableView{
	
	int rows = [self.zigbeePackets count];
        
    [totalPacketsString setStringValue:[NSString stringWithFormat:@"Total Packets: %i", rows]];
    
	return rows;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    
	NSString *key = [tableColumn identifier];
	
	zigPacket *thisPacket = [self.zigbeePackets objectAtIndex:row];
    
	return[thisPacket.zigPacketDiction objectForKey:key];
	
	
}

- (void)tableViewSelectionDidChange:(NSNotification *)notification
{   
    [self verboseZigbeePacket:[self.zigbeePackets objectAtIndex:0]
                  rowSelected:[zigbeeView selectedRow]];
	
	[zigbeeView reloadData];
	
}
- (IBAction)addPacketButton:(id)sender;
{
    zigPacket *newPack = [[zigPacket alloc]init];
    
    [self.zigbeePackets addObject:newPack];
    
    [zigbeeView reloadData];
}

- (void)verboseZigbeePacket:(zigPacket *)pkt
                rowSelected:(int)row    
{
    if ([zigbeeView isRowSelected:row]) 
    {
        if(!pkt.decodedStr)
        [zigbeeOutputView setStringValue:@"I am just trying to get this to work"];
        else
        {
            [zigbeeOutputView setStringValue:(pkt.decodedStr)];
        }
    }
    else
        [zigbeeOutputView setStringValue:@""];
}
@end
