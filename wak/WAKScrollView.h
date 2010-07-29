//
//  WAKScrollView.h
//  WebCore
//
//  Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc.  All rights reserved.
//

#ifndef WAKScrollView_h
#define WAKScrollView_h

#import "WAKView.h"
#import "WebCoreFrameView.h"
#import <Foundation/Foundation.h>

@class WAKClipView;

@interface WAKScrollView : WAKView <WebCoreFrameScrollView>
{
    WAKView *_documentView;  // Only here so the ObjC instance stays around.
    id _delegate;
}

- (CGRect)documentVisibleRect;
- (void)setContentView:(WAKClipView *)aView;
- (WAKClipView *)contentView;
- (id)documentView;
- (void)setDocumentView:(WAKView *)aView;
- (void)setHasVerticalScroller:(BOOL)flag;
- (BOOL)hasVerticalScroller;
- (void)setHasHorizontalScroller:(BOOL)flag;
- (BOOL)hasHorizontalScroller;
- (void)reflectScrolledClipView:(WAKClipView *)aClipView;
- (void)setDrawsBackground:(BOOL)flag;
- (float)verticalLineScroll;
- (void)setLineScroll:(float)aFloat;
- (BOOL)drawsBackground;
- (float)horizontalLineScroll;

- (void)setDelegate:(id)delegate;
- (id)delegate;

- (CGRect)actualDocumentVisibleRect;
- (void)setActualScrollPosition:(CGPoint)point;

@end

@interface NSObject (WAKScrollViewDelegate)
- (BOOL)scrollView:(WAKScrollView *)scrollView shouldScrollToPoint:(CGPoint)point;
@end

#endif // WAKScrollView_h
