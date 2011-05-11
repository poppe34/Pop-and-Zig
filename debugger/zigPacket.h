//
//  zPacket.h
//  debugger
//
//  Created by Matt Poppe on 4/24/11.
//  Copyright 2011 Poppe. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "zigbee_layers.h"

@interface zigPacket : NSObject {
//@private
    uint8_t len;  
    NSMutableDictionary *zigPacketDiction;
    NSMutableString *decodedStr;
    mac_t mac_layer;
    NSNumber *testNum, *src, *dest, *srcPanId, *destPanId;
    uint8_t frame[128];
    
}
#define packet_read(packet, var) *(var *)packet;\
                                        packet+=sizeof(var)


@property(assign) uint8_t len;
@property(assign) NSMutableDictionary *zigPacketDiction;
@property(assign) NSMutableString *decodedStr;
@property(assign) mac_t mac_layer;
@property(assign) NSNumber *testNum;

- (id)init;
- (void)dealloc;
- (id)initWithData:(voidPtr)zig;

- (void)zigbee_breakdown:(voidPtr)zig;

/* MAC Layer */
- (void)zigbee_macBreakdown:(voidPtr)zig;
- (void)zigbee_macFCF_breakdown;

int zigbee_hexToBin(int val, uint8_t cnt);
@end
