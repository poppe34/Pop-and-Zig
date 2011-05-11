//
//  appController.m
//  debugger
//
//  Created by Matt Poppe on 2/18/11.
//  Copyright 2011 Poppe. All rights reserved.
//

#import "appController.h"
#import "usbDriver.h"
#import	"usbCentral.h"
#import "usbInfo.h"
#import "alarmLog.h"
#import "debug.h"

@implementation appController 

@synthesize driver, central, led1, led2, led3, usbName, usbCheck;

- (void)awakeFromNib
{
	isAlarmChecking = NO;
}

- (id)init
{
	if ((self = [super init])) 
	{
		[usbName setStringValue:@""];
		
	}
	return self;
}
- (BOOL)initButtons{
	if([driver ledsAvail])
	{
		[led1 setHidden:NO];
		[led2 setHidden:NO];
		[led3 setHidden:NO];
	}
	return YES;
}

- (BOOL)deinitButtons
{
	[led1 setHidden:YES];
	[led2 setHidden:YES];
	[led3 setHidden:YES];
	return YES;
}
					  

- (IBAction)ledToggle1:(id)sender{
	if ([[sender title] isEqualTo:@"LED1 ON"] ) 
	{
		if ([driver ledsAvail]) 
		{
			[driver ledOn:1];
			[sender setTitle:@"LED1 OFF"];
		}
	} else
	{
		if ([driver ledsAvail]) 
		{
		[sender setTitle:@"LED1 ON"];
		[driver ledOff:1];
		}
	}
}
-(IBAction)ledToggle2:(id)sender{
	if ([[sender title] isEqualTo:@"LED2 ON"] ) 
	{
		if ([driver ledsAvail]) 
		{
			[driver ledOn:2];
			[sender setTitle:@"LED2 OFF"];
		}
	} else 
	{
		if ([driver ledsAvail]) 
		{
		[sender setTitle:@"LED2 ON"];
		[driver ledOff:2];
		}
	}
}
-(IBAction)ledToggle3:(id)sender{
	if ([[sender title] isEqualTo:@"LED3 ON"] ) 
	{
		if ([driver ledsAvail]) 
		{
			[driver ledOn:3];
			[sender setTitle:@"LED3 OFF"];
		}
	} else 
	{
		if ([driver ledsAvail]) 
		{
		[sender setTitle:@"LED3 ON"];
		[driver ledOff:3];
		}
	}
}

-(IBAction)connectToUSB:(id)sender
{
	//central
}

- (IBAction)readUSB:(id)sender
{
	//[NSThread detachNewThreadSelector:@selector(runningCheckAlarms) toTarget:self withObject:nil];
        
   /* NSTimer *twoSecondTimer = [[NSTimer scheduledtimerWithTimeInterval:2.0
                                                           target:self
                                                         selector:@selector(runningCheckAlarms)
                                                         userInfo:nil
                                                          repeats:YES] retain];
    */
    [NSTimer scheduledTimerWithTimeInterval:20.0 target:self selector:@selector(runningCheckAlarms) userInfo:nil repeats:YES];
}

-(IBAction)checkZigbee:(id)sender
{
    uint8_t packet[128];
    uint8_t len;
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    NSLog(@"Checking Zigbee");
 
    if([driver zigbeeAvail]) [driver checkZigbee];
    
    [driver readUSB_TEMP:packet];
    
    len = packet[3];
    printf("The packet count is:%i \n", len);
    
    [pool drain];
    
}

- (void)runningCheckAlarms
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
	isAlarmChecking = YES;
	NSLog(@"RunningCheckAlarms");
	[usbCheck setHidden:NO];
	char test[64];
	NSUInteger len = 0;

    if([driver alarmsAvail])  [driver checkForAlarms];
    
    [driver readUSB_TEMP:test];
	
    len = test[3];
    printf("The alarm count is:%lu \n", len);

    while (len) 
    {
        [driver getFirstAlarm];
        [driver readUSB_TEMP:test];
        test[(test[0]-1)] = 0;
        [log newAlarmWithString:&test[3] length:test[0]];
        
        for (uint8_t x = 0; x<64; x++) {
            test[x] = 0;
        }
        len--;
    }

	[usbCheck setHidden:YES];
	[pool drain];
	
}
- (void)debuggerFound:(usbInfo *)info
{
	self.driver = info.driver;
	[usbName setStringValue:info.name];
	[self initButtons];
}
- (void)debuggerLost:(usbInfo *)info
{
	[self deinitButtons];
}
@end
