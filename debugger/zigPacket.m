//
//  zPacket.m
//  debugger
//
//  Created by Matt Poppe on 4/24/11.
//  Copyright 2011 Poppe. All rights reserved.
//

#import "zigPacket.h"


@implementation zigPacket

@synthesize len, zigPacketDiction, decodedStr;
@synthesize mac_layer;
@synthesize testNum;

- (id)init
{
    self = [super init];
    if (self) {
        uint8_t x = 3;
        while (x<64) {
            frame[x] = x-2;
            x++;
        }
        frame[0] = 0x09;
        frame[1] = 0x7C;
        frame[2] = 0xcc;
            
        
        src = [[NSNumber alloc] initWithInt:0];
        dest = [[NSNumber alloc] initWithInt:0];
        srcPanId = [[NSNumber alloc] initWithInt:0];
        destPanId = [[NSNumber alloc] initWithInt:0];
        
        self.testNum = [[NSNumber alloc] initWithInt:1232234];
        
        [self zigbee_breakdown:&frame[1]];
        
        NSArray *keys = [NSArray arrayWithObjects:@"packetNum", @"packetValue",@"packetDestAddr", @"packetDestPanId", @"packetSrcAddr", @"packetSrcPanId", nil];
        NSArray *values = [NSArray arrayWithObjects:@"Pizza", self.testNum, dest, destPanId, src, srcPanId, nil];
        
        zigPacketDiction = [[NSMutableDictionary alloc] initWithObjects:values forKeys:keys];
        

        
        NSLog(@"Packet Added");
    }
    
    return self;
}
- (id)initWithData:(voidPtr)zig
{
    self = [super init];
    if(self)
    {
        self.len = packet_read(zig, uint8_t);
    }
    
    return self;
}

- (void)dealloc
{
    [decodedStr release];
    [zigPacketDiction release];
    [super dealloc];
}

- (void)release
{
    [self dealloc];
}

- (void)zigbee_macBreakdown:(voidPtr)zig
{
    mac_layer.mac_fcf = packet_read(zig, mac_fcf_t);

    [self zigbee_macFCF_breakdown];
    
    mac_layer.mac_seq_num = packet_read(zig, uint8_t);
    
    if(self.mac_layer.mac_fcf.mac_dest_addr_mode == 0x02 || self.mac_layer.mac_fcf.mac_dest_addr_mode == 0x03)
    {
        mac_layer.destPanID = packet_read(zig, uint16_t);
        
        if(mac_layer.mac_fcf.mac_dest_addr_mode == 0x02)
        {
            mac_layer.dest_sAddr = packet_read(zig, uint16_t);
            [dest release];
            
            NSLog(@"Destination ADDR: %.4x\n", mac_layer.dest_sAddr);
            
            dest = [[NSNumber alloc]initWithInt:mac_layer.dest_sAddr];
            [self.zigPacketDiction setObject:dest forKey:@"packetDestAddr"];
        } 
        else 
        {
            mac_layer.dest_lAddr = packet_read(zig, uint64_t);
            [dest release];
            
            NSLog(@"Destination ADDR: %.16llX\n", mac_layer.dest_lAddr);
            
            dest = [[NSNumber alloc]initWithInt:mac_layer.dest_lAddr];
            [self.zigPacketDiction setObject:dest forKey:@"packetDestAddr"];
        }
        [destPanId release];
        mac_layer.destPanID = packet_read(zig, uint16_t);
        
        destPanId = [[NSNumber alloc]initWithInt:mac_layer.destPanID];
        [self.zigPacketDiction setObject:dest forKey:@"packetDestAddr"];
        
    }
    if(self.mac_layer.mac_fcf.mac_src_addr_mode == 0x02 || self.mac_layer.mac_fcf.mac_src_addr_mode == 0x03)
    {
        mac_layer.srcPanID = packet_read(zig, uint16_t);
        
        if(mac_layer.mac_fcf.mac_src_addr_mode == 0x02)
        {
            mac_layer.src_sAddr = packet_read(zig, uint16_t);
            [src release];
            
            NSLog(@"Source ADDR: %.4x\n", mac_layer.src_sAddr);
            
            src = [[NSNumber alloc]initWithInt:mac_layer.src_sAddr];
            [self.zigPacketDiction setObject:src forKey:@"packetSrcAddr"];
        } 
        else 
        {
            mac_layer.src_lAddr = packet_read(zig, uint64_t);
            [src release];
            
            NSLog(@"Source ADDR: %.16llX\n", mac_layer.src_lAddr);
            
            src = [[NSNumber alloc]initWithInt:mac_layer.src_lAddr];
            [self.zigPacketDiction setObject:src forKey:@"packetSrcAddr"];
        }
        [srcPanId release];
        mac_layer.srcPanID = packet_read(zig, uint16_t);
        
        srcPanId = [[NSNumber alloc]initWithInt:mac_layer.srcPanID];
        [self.zigPacketDiction setObject:src forKey:@"packetSrcAddr"];
    }
    
    
}
- (void)zigbee_macFCF_breakdown
{
    NSString *frameType, *destMode, *srcMode;
    
    switch (self.mac_layer.mac_fcf.mac_frame_type) {
        case mac_beacon:
            frameType = [[NSString alloc]initWithString:@"\tMAC Beacon\n"];
            break;
        case mac_data:
            frameType = [[NSString alloc]initWithString:@"\tMAC Data\n"];
            break;
        case mac_ack:
            frameType = [[NSString alloc]initWithString:@"\tMAC Ackownledgment\n"];
            break;
        case mac_command:
            frameType = [[NSString alloc]initWithString:@"\tMAC Command\n"];
            break;
        default:
            frameType = [[NSString alloc]initWithString:@"\tUnknown Packet Type\n"];
            break;
    }
    
    switch (self.mac_layer.mac_fcf.mac_dest_addr_mode) {
        case mac_addr_not_pres:
            destMode = [[NSString alloc]initWithString:@"\tAddr Not Present\n"];
            break;
        case mac_short_addr:
            destMode = [[NSString alloc]initWithString:@"\tShort Addr Mode\n"];
            break;
        case mac_long_addr:
            destMode = [[NSString alloc]initWithString:@"\tLong Addr Mode\n"];
            break;
        default:
            destMode = [[NSString alloc]initWithString:@"\tERROR:Unknown Mode\n"];
            break;
    }
    switch (self.mac_layer.mac_fcf.mac_src_addr_mode) {
        case mac_addr_not_pres:
            srcMode = [[NSString alloc]initWithString:@"\tAddr Not Present\n"];
            break;
        case mac_short_addr:
            srcMode = [[NSString alloc]initWithString:@"\tShort Addr Mode\n"];
            break;
        case mac_long_addr:
            srcMode = [[NSString alloc]initWithString:@"\tLong Addr Mode\n"];
            break;
        default:
            srcMode = [[NSString alloc]initWithString:@"\tERROR:Unknown Mode\n"];
            break;
    }
    
    [self.decodedStr appendFormat:@"MAC FCF: %1$i (%1$#.4X)\n", self.mac_layer.mac_fcf];
    [self.decodedStr appendFormat:@"\tFrame Type:\t\t\t-------------%.3X",zigbee_hexToBin(self.mac_layer.mac_fcf.mac_frame_type, 3)];
    [self.decodedStr appendString:frameType];
    [self.decodedStr appendFormat:@"\tSecurity Enabled:\t\t\t------------%X---\n",self.mac_layer.mac_fcf.mac_security_enbl];
    [self.decodedStr appendFormat:@"\tFrame Pending:\t\t\t-----------%X----\n",self.mac_layer.mac_fcf.mac_frame_pending];
    [self.decodedStr appendFormat:@"\tAck Request:\t\t\t----------%X-----\n",self.mac_layer.mac_fcf.mac_ack_request];
    [self.decodedStr appendFormat:@"\tPAN ID Compression:\t\t---------%X------\n",self.mac_layer.mac_fcf.mac_panID_compress];
    [self.decodedStr appendFormat:@"\tRESERVED:\t\t\t\t------%.2X%X-------\n",(zigbee_hexToBin(self.mac_layer.mac_fcf.mac_reserved2, 2)), self.mac_layer.mac_fcf.mac_reserved1];
    [self.decodedStr appendFormat:@"\tDestination Addr Mode:\t----%.2X----------",(zigbee_hexToBin(self.mac_layer.mac_fcf.mac_dest_addr_mode, 2))];
    [self.decodedStr appendString:destMode];
    [self.decodedStr appendFormat:@"\tFrame Version:\t\t\t--%.2X------------\n",(zigbee_hexToBin(self.mac_layer.mac_fcf.mac_frame_ver, 2))];
    [self.decodedStr appendFormat:@"\tSource Addr Mode:\t\t%.2X--------------",(zigbee_hexToBin(self.mac_layer.mac_fcf.mac_src_addr_mode, 2))];
    [self.decodedStr appendString:srcMode];
    
    [frameType release];
    [destMode release];
    [srcMode release];

}
- (void)zigbee_breakdown:(voidPtr)zig
{
    self.decodedStr = [[NSMutableString alloc]init];
    
    [self zigbee_macBreakdown:zig];
}


int zigbee_hexToBin(int val, uint8_t cnt)
{
    int newValue;
    uint8_t x = 0;
    
    do
    {
        newValue |= (((val>>x) & 0x01)<<(4*x));
        x++;
    }while(x<cnt);
    
    return newValue;
}
@end
