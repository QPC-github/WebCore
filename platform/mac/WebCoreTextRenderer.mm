/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#import "config.h"
#import "WebCoreTextRenderer.h"

#import "Font.h"
#import "SimpleFontData.h"
#import "GraphicsContext.h"
#import "IntPoint.h"
#import "WebFontCache.h"

#import "WKGraphics.h"

using namespace WebCore;

void WebCoreDrawTextAtPoint(const UniChar* buffer, unsigned length, NSPoint point, GSFontRef font, CGColorRef textColor)
{
    FontPlatformData f(font);
    Font renderer(f, false);
    TextRun run(buffer, length);
    run.disableRoundingHacks();
    
    GraphicsContext graphicsContext(static_cast<PlatformGraphicsContext*>(WKGetCurrentGraphicsContext()));
    graphicsContext.setPenFromCGColor (textColor);
    renderer.drawText(&graphicsContext, run, FloatPoint(point.x, point.y));
}

float WebCoreTextFloatWidth(const UniChar* buffer, unsigned length , GSFontRef font)
{
    FontPlatformData f(font);
    Font renderer(f, false);    
    TextRun run(buffer, length);
    run.disableRoundingHacks();
    return renderer.floatWidth(run);
}

static bool gShouldUseFontSmoothing = true;

void WebCoreSetShouldUseFontSmoothing(bool smooth)
{
    gShouldUseFontSmoothing = smooth;
}

bool WebCoreShouldUseFontSmoothing()
{
    return gShouldUseFontSmoothing;
}

static CGFontSmoothingStyle gFontSmoothingStyle = kCGFontSmoothingStyleMedium;
static CGFontAntialiasingStyle gFontAntialiasingStyle = kCGFontAntialiasingStyleUnfiltered;

void WebCoreSetFontSmoothingStyle(CGFontSmoothingStyle newStyle)
{
    gFontSmoothingStyle = newStyle;
}

CGFontSmoothingStyle WebCoreFontSmoothingStyle()
{
    return gFontSmoothingStyle;
}

void WebCoreSetFontAntialiasingStyle(CGFontAntialiasingStyle newStyle)
{
    gFontAntialiasingStyle = newStyle;
}

CGFontAntialiasingStyle WebCoreFontAntialiasingStyle()
{
    return gFontAntialiasingStyle;
}


void WebCoreSetAlwaysUseATSU(bool useATSU)
{
    Font::setCodePath(useATSU ? Font::Complex : Font::Auto);
}

GSFontRef WebCoreFindFont(NSString* familyName, GSFontTraitMask traits, int size)
{
    return 0;
}