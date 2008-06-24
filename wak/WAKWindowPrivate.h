//
//  WAKWindowPrivate.h
//
//  Copyright (C) 2005, 2006, 2007, Apple Inc.  All rights reserved.
//
#import "WAKWindow.h"

@interface WAKWindow (WAKPrivate)
- (WKWindowRef)_windowRef;
+ (WAKWindow *)_wrapperForWindowRef:(WKWindowRef)_windowRef;
@end

