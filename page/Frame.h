/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999-2001 Lars Knoll <knoll@kde.org>
 *                     1999-2001 Antti Koivisto <koivisto@kde.org>
 *                     2000-2001 Simon Hausmann <hausmann@kde.org>
 *                     2000-2001 Dirk Mueller <mueller@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef Frame_h
#define Frame_h

#include "AnimationController.h"
#include "Document.h"
#include "DragImage.h"
#include "EditAction.h"
#include "Editor.h"
#include "EventHandler.h"
#include "FrameLoader.h"
#include "FrameTree.h"
#include "Range.h"
#include "ScrollBehavior.h"
#include "ScriptController.h"
#include "SelectionController.h"
#include "TextGranularity.h"

#include "FloatSize.h"

#include "KURL.h"

#if PLATFORM(WIN)
#include "FrameWin.h"
#endif

#ifndef __OBJC__
class NSArray;
class NSDictionary;
class NSMutableDictionary;
class NSString;
typedef int NSWritingDirection;
#endif

#ifdef __OBJC__
@class DOMNode;
#else
class DOMNode;
#endif

#if PLATFORM(WIN)
typedef struct HBITMAP__* HBITMAP;
#endif

namespace WebCore {

class CSSMutableStyleDeclaration;
class Editor;
class EventHandler;
class FrameLoader;
class FrameLoaderClient;
class FrameTree;
class FrameView;
class HTMLFrameOwnerElement;
class HTMLTableCellElement;
class RegularExpression;
class RenderLayer;
class RenderPart;
class ScriptController;
class SelectionController;
class Settings;
class VisibleSelection;
class Widget;

#if FRAME_LOADS_USER_STYLESHEET
    class UserStyleSheetLoader;
#endif

template <typename T> class Timer;

enum { 
    OverflowScrollNone =  0x0,
    OverflowScrollLeft =  0x1,
    OverflowScrollRight = 0x2,
    OverflowScrollUp    = 0x4,
    OverflowScrollDown  = 0x8
};

enum OverflowScrollAction { DoNotPerformOverflowScroll, PerformOverflowScroll };
typedef Node* (*NodeQualifier)(HitTestResult aHitTestResult, Node* terminationNode, IntRect* frame);


class Frame : public RefCounted<Frame> {
public:
    static PassRefPtr<Frame> create(Page* page, HTMLFrameOwnerElement* ownerElement, FrameLoaderClient* client)
    {
        return adoptRef(new Frame(page, ownerElement, client));
    }
    void setView(PassRefPtr<FrameView>);
    ~Frame();
    
    void init();
    // Creates <html (contentEditable="true")><body style="..."></body></html> doing minimal amount of work
    void initWithSimpleHTMLDocument(const String& style, bool editable, const KURL& url);

    Page* page() const;
    void detachFromPage();
    HTMLFrameOwnerElement* ownerElement() const;

    void pageDestroyed();
    void disconnectOwnerElement();

    Document* document() const;
    FrameView* view() const;

    void setDOMWindow(DOMWindow*);
    DOMWindow* domWindow() const;
    void clearFormerDOMWindow(DOMWindow*);

    Editor* editor() const;
    EventHandler* eventHandler() const;
    FrameLoader* loader() const;
    SelectionController* selection() const;
    FrameTree* tree() const;
    AnimationController* animation() const;
    ScriptController* script();

    RenderView* contentRenderer() const; // root renderer for the document contained in this frame
    RenderPart* ownerRenderer() const; // renderer for the element that contains this frame
    
    bool isDisconnected() const;
    void setIsDisconnected(bool);
    bool excludeFromTextSearch() const;
    void setExcludeFromTextSearch(bool);

    void createView(const IntSize&, const Color&, bool, const IntSize &, bool,
                    ScrollbarMode = ScrollbarAuto, ScrollbarMode = ScrollbarAuto);


    float documentScale() const;
    void setDocumentScale(float);
    
private:
    Frame(Page*, HTMLFrameOwnerElement*, FrameLoaderClient*);
    
// === undecided, would like to consider moving to another class

public:
    static Frame* frameForWidget(const Widget*);

    Settings* settings() const; // can be NULL

#if FRAME_LOADS_USER_STYLESHEET
    void setUserStyleSheetLocation(const KURL&);
    void setUserStyleSheet(const String&, bool saveStyleSheet = false);
#endif

    void setPrinting(bool printing, float minPageWidth, float maxPageWidth, bool adjustViewSize);

    bool inViewSourceMode() const;
    void setInViewSourceMode(bool = true);

    void keepAlive(); // Used to keep the frame alive when running a script that might destroy it.
#ifndef NDEBUG
    static void cancelAllKeepAlive();
#endif

    void didParse(double);
    void didLayout(bool, double);
    void didForcedLayout();
    void getPPTStats(unsigned& parseCount, unsigned& layoutCount, unsigned& forcedLayoutCount, CFTimeInterval& parseDuration, CFTimeInterval& layoutDuration);
    void clearPPTStats();

    const ViewportArguments& viewportArguments() const;
    void setViewportArguments(const ViewportArguments&);
    NSDictionary* dictionaryForViewportArguments(const ViewportArguments& arguments) const;
        
    void betterApproximateNode(const IntPoint& testPoint, NodeQualifier aQualifer, Node*& best, Node* failedNode, IntPoint &bestPoint, IntRect &bestRect, const IntRect& testRect);
    Node* qualifyingNodeAtViewportLocation(CGPoint* aViewportLocation, NodeQualifier aQualifer, bool shouldApproximate);
    
    Node* nodeRespondingToClickEvents(CGPoint* aViewportLocation);
    Node* nodeRespondingToScrollWheelEvents(CGPoint* aViewportLocation);

    int indexCountOfWordPrecedingSelection(NSString *word);
    NSArray *wordsInCurrentParagraph();    
    CGRect renderRectForPoint(CGPoint point, bool *isReplaced, float *fontSize);

    void setEmbeddedEditingMode(bool b = true);
    bool embeddedEditingMode() const;

    void setDocument(PassRefPtr<Document>);

#if ENABLE(ORIENTATION_EVENTS)
    // Orientation is the interface orientation in degrees. Some examples are:
    //  0 is straight up; -90 is when the device is rotated 90 clockwise;
    //  90 is when rotated counter clockwise.
    void sendOrientationChangeEvent(int orientation);
    int orientation() const { return m_orientation; }
#endif

    void clearTimers();
    static void clearTimers(FrameView*, Document*);

    void setNeedsReapplyStyles();
    bool needsReapplyStyles() const;
    void reapplyStyles();

    String documentTypeString() const;

    // This method -- and the corresponding list of former DOM windows --
    // should move onto ScriptController
    void clearDOMWindow();

    String displayStringModifiedByEncoding(const String& str) const 
    {
        return document() ? document()->displayStringModifiedByEncoding(str) : str;
    }

private:
    void lifeSupportTimerFired(Timer<Frame>*);

// === to be moved into FrameView

public: 
    void setZoomFactor(float scale, bool isTextOnly);
    float zoomFactor() const;
    bool isZoomFactorTextOnly() const;
    bool shouldApplyTextZoom() const;
    bool shouldApplyPageZoom() const;
    float pageZoomFactor() const { return shouldApplyPageZoom() ? zoomFactor() : 1.0f; }
    float textZoomFactor() const { return shouldApplyTextZoom() ? zoomFactor() : 1.0f; }

// === to be moved into Chrome

public:
    void focusWindow();
    void unfocusWindow();
    bool shouldClose(RegisteredEventListenerVector* alternateEventListeners = 0);
    void scheduleClose();

    void setJSStatusBarText(const String&);
    void setJSDefaultStatusBarText(const String&);
    String jsStatusBarText() const;
    String jsDefaultStatusBarText() const;

// === to be moved into Editor

public:
    String selectedText() const;  
    bool findString(const String&, bool forward, bool caseFlag, bool wrapFlag, bool startInSelection);

    const VisibleSelection& mark() const; // Mark, to be used as emacs uses it.
    void setMark(const VisibleSelection&);

    /**
     * Clears the current selection.
     */
    void clearSelection();

    void computeAndSetTypingStyle(CSSStyleDeclaration* , EditAction = EditActionUnspecified);
    String selectionStartStylePropertyValue(int stylePropertyID) const;
    void applyEditingStyleToBodyElement() const;
    void removeEditingStyleFromBodyElement() const;
    void applyEditingStyleToElement(Element*) const;
    void removeEditingStyleFromElement(Element*) const;

    IntRect firstRectForRange(Range*) const;
    
    void respondToChangedSelection(const VisibleSelection& oldSelection, bool closeTyping);
    bool shouldChangeSelection(const VisibleSelection& oldSelection, const VisibleSelection& newSelection, EAffinity, bool stillSelecting) const;

    RenderStyle* styleForSelectionStart(Node*& nodeToRemove) const;

    unsigned markAllMatchesForText(const String&, bool caseFlag, unsigned limit);
    bool markedTextMatchesAreHighlighted() const;
    void setMarkedTextMatchesAreHighlighted(bool flag);

    PassRefPtr<CSSComputedStyleDeclaration> selectionComputedStyle(Node*& nodeToRemove) const;

    void textFieldDidBeginEditing(Element*);
    void textFieldDidEndEditing(Element*);
    void textDidChangeInTextField(Element*);
    bool doTextFieldCommandFromEvent(Element*, KeyboardEvent*);
    void textWillBeDeletedInTextField(Element* input);
    void textDidChangeInTextArea(Element*);

    DragImageRef dragImageForSelection();
    
// === to be moved into SelectionController

public:
    TextGranularity selectionGranularity() const;
    void setSelectionGranularity(TextGranularity);

    bool shouldChangeSelection(const VisibleSelection&) const;
    bool shouldDeleteSelection(const VisibleSelection&) const;
    void clearCaretRectIfNeeded();
    void setFocusedNodeIfNeeded();
    void selectionLayoutChanged();
    void notifyRendererOfSelectionChange(bool userTriggered);

    void setSingleLineSelectionBehavior(bool b);
    bool singleLineSelectionBehavior() const;

    void setCaretVisible(bool = true);
    void paintCaret(GraphicsContext*, int tx, int ty, const IntRect& clipRect) const;  
    void paintDragCaret(GraphicsContext*, int tx, int ty, const IntRect& clipRect) const;

    void setCaretColor(const Color &color);

    /**
     * Scroll the selection in an overflow layer on iPhone.
     */
    void scrollOverflowLayer(RenderLayer *, const IntRect &visibleRect, const IntRect &exposeRect);
    
    void invalidateOwnerRendererLayoutIfNeeded();

    bool isContentEditable() const; // if true, everything in frame is editable

    void updateSecureKeyboardEntryIfActive();

    CSSMutableStyleDeclaration* typingStyle() const;
    void setTypingStyle(CSSMutableStyleDeclaration*);
    void clearTypingStyle();

    FloatRect selectionBounds(bool clipToVisibleContent = true) const;
    void selectionTextRects(Vector<FloatRect>&, bool clipToVisibleContent = true) const;

    HTMLFormElement* currentForm() const;

    void revealSelection(const ScrollAlignment& = ScrollAlignment::alignCenterIfNeeded, bool revealExtent = false);
    void setSelectionFromNone();
    void setCaretBlinks(bool flag = true);

    void setUseSecureKeyboardEntry(bool);

private:
    void caretBlinkTimerFired(Timer<Frame>*);

    void overflowAutoScrollTimerFired(Timer<Frame>*);
    void startOverflowAutoScroll(const IntPoint &);
    void stopOverflowAutoScroll();
    int checkOverflowScroll(OverflowScrollAction);

public:
    SelectionController* dragCaretController() const;

    String searchForLabelsAboveCell(RegularExpression*, HTMLTableCellElement*);
    String searchForLabelsBeforeElement(const Vector<String>& labels, Element*);
    String matchLabelsAgainstElement(const Vector<String>& labels, Element*);
    
    VisiblePosition visiblePositionForPoint(const IntPoint& framePoint);
    Document* documentAtPoint(const IntPoint& windowPoint);

#if PLATFORM(MAC)

// === undecided, would like to consider moving to another class

public:
    NSString* searchForNSLabelsAboveCell(RegularExpression*, HTMLTableCellElement*);
    NSString* searchForLabelsBeforeElement(NSArray* labels, Element*);
    NSString* matchLabelsAgainstElement(NSArray* labels, Element*);

#if ENABLE(DASHBOARD_SUPPORT)
    NSMutableDictionary* dashboardRegionsDictionary();
#endif

    CGImageRef selectionImage(bool forceBlackText = false) const;

private:    
    CGImageRef imageFromRect(NSRect) const;

// === to be moved into Editor

public:
    NSDictionary* fontAttributesForSelectionStart() const;
    NSWritingDirection baseWritingDirectionForSelectionStart() const;

#endif

public:
    void setVisibleSize(const FloatSize& size);
    const FloatSize& visibleSize() const;

public:
    int preferredHeight() const;
    int innerLineHeight(DOMNode *node) const;
    void updateLayout() const;
    NSRect caretRect() const;
    NSRect rectForScrollToVisible() const;
    NSRect rectForSelection(VisibleSelection&) const;
    void createDefaultFieldEditorDocumentStructure() const;
    void moveSelectionToStartOrEndOfCurrentWord();
    unsigned formElementsCharacterCount() const;
    void setTimersPaused(bool);
    bool timersPaused() const { return m_timersPausedCount; }
    void setRangedSelectionBaseToCurrentSelection();
    void setRangedSelectionBaseToCurrentSelectionStart();
    void setRangedSelectionBaseToCurrentSelectionEnd();
    void clearRangedSelectionInitialExtent();
    void setRangedSelectionInitialExtentToCurrentSelectionStart();
    void setRangedSelectionInitialExtentToCurrentSelectionEnd();
    VisibleSelection rangedSelectionBase() const;
    VisibleSelection rangedSelectionInitialExtent() const;

#if PLATFORM(WIN)

public:
    // FIXME - We should have a single version of nodeImage instead of using platform types.
    HBITMAP nodeImage(Node*) const;

#endif

private:
    Page* m_page;
    mutable FrameTree m_treeNode;
    mutable FrameLoader m_loader;

    mutable RefPtr<DOMWindow> m_domWindow;
    HashSet<DOMWindow*> m_liveFormerWindows;

    HTMLFrameOwnerElement* m_ownerElement;
    RefPtr<FrameView> m_view;
    RefPtr<Document> m_doc;

    ScriptController m_script;

    String m_kjsStatusBarText;
    String m_kjsDefaultStatusBarText;

    float m_zoomFactor;

    TextGranularity m_selectionGranularity;

    mutable SelectionController m_selectionController;
    mutable VisibleSelection m_mark;
    Timer<Frame> m_caretBlinkTimer;
    mutable Editor m_editor;
    mutable EventHandler m_eventHandler;
    mutable AnimationController m_animationController;

    RefPtr<CSSMutableStyleDeclaration> m_typingStyle;

    Color m_caretColor;
    Timer<Frame> m_overflowAutoScrollTimer;
    float m_overflowAutoScrollDelta;
    IntPoint m_overflowAutoScrollPos;
    ViewportArguments m_viewportArguments;
    bool m_embeddedEditingMode;
    VisibleSelection m_rangedSelectionBase;
    VisibleSelection m_rangedSelectionInitialExtent;
    FloatSize m_visibleSize;
    unsigned m_parseCount;
    unsigned m_layoutCount;
    unsigned m_forcedLayoutCount;
    CFTimeInterval m_parseDuration;
    CFTimeInterval m_layoutDuration;

    Timer<Frame> m_lifeSupportTimer;

#if ENABLE(ORIENTATION_EVENTS)
    int m_orientation;
#endif
    
    String m_userStyleSheet;

    bool m_caretVisible;
    bool m_caretBlinks;
    bool m_caretPaint;
    
    bool m_highlightTextMatches;
    bool m_inViewSourceMode;
    bool m_needsReapplyStyles;
    bool m_isDisconnected;
    bool m_excludeFromTextSearch;

    bool m_singleLineSelectionBehavior;
    int m_timersPausedCount;
    float m_documentScale;

#if FRAME_LOADS_USER_STYLESHEET
    UserStyleSheetLoader* m_userStyleSheetLoader;
#endif

};

} // namespace WebCore

#endif // Frame_h
