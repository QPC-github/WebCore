/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "SelectionRectGatherer.h"

#include "Editor.h"
#include "EditorClient.h"
#include "Frame.h"
#include "RenderView.h"

#if ENABLE(SERVICE_CONTROLS)

namespace WebCore {

SelectionRectGatherer::SelectionRectGatherer(RenderView& renderView)
    : m_renderView(renderView)
{
}

void SelectionRectGatherer::addRect(const LayoutRect& rect)
{
    if (!rect.isEmpty())
        m_rects.append(rect);
}

void SelectionRectGatherer::addRects(const GapRects& rects)
{
    m_gapRects.append(rects);
}

SelectionRectGatherer::Notifier::Notifier(SelectionRectGatherer& gatherer)
    : m_gatherer(gatherer)
{
}

SelectionRectGatherer::Notifier::~Notifier()
{
    if (EditorClient* client = m_gatherer.m_renderView.view().frame().editor().client())
        client->selectionRectsDidChange(m_gatherer.m_rects, m_gatherer.m_gapRects);
}

std::unique_ptr<SelectionRectGatherer::Notifier> SelectionRectGatherer::clearAndCreateNotifier()
{
    m_rects.clear();
    m_gapRects.clear();

    return std::make_unique<Notifier>(*this);
}

} // namespace WebCore

#endif // ENABLE(SERVICE_CONTROLS)
