//
//  defaultDevice.h
//  debugger
//
//  Created by Matt Poppe on 2/21/11.
//  Copyright 2011 Poppe. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "usbDriver.h"

#define kDefaultProdID	0
#define kDefaultVendID	0
#define kDefaultName	"No Device";
@interface defaultDevice : usbDriver {

}

@end
