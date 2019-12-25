//
//  WorkingInterop-Bridging-Header.h
//  WorkingInterop
//
//  Created by Nikhil Bhargava on 12/25/19.
//  Copyright © 2019 Nikhil Bhargava. All rights reserved.
//

#import <stdio.h>

#ifndef WorkingInterop_Bridging_Header_h
#define WorkingInterop_Bridging_Header_h

void testCB(const char *name) {
    printf("Name: %s\n", name);
}

#endif /* WorkingInterop_Bridging_Header_h */
