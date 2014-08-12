/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006, 2008, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig (sam@webkit.org)
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

#ifndef CSSGroupingRule_h
#define CSSGroupingRule_h

#include "CSSRule.h"
#include "StyleRule.h"
#include <memory>
#include <wtf/Vector.h>

namespace WebCore {

class CSSRuleList;

class CSSGroupingRule : public CSSRule {
public:
    virtual ~CSSGroupingRule();

    virtual void reattach(StyleRuleBase*) override;

    CSSRuleList* cssRules() const;

    unsigned insertRule(const String& rule, unsigned index, ExceptionCode&);
    void deleteRule(unsigned index, ExceptionCode&);
        
    // For CSSRuleList
    unsigned length() const;
    CSSRule* item(unsigned index) const;

protected:
    CSSGroupingRule(StyleRuleGroup* groupRule, CSSStyleSheet* parent);
    
    void appendCssTextForItems(StringBuilder&) const;

    RefPtr<StyleRuleGroup> m_groupRule;
    mutable Vector<RefPtr<CSSRule>> m_childRuleCSSOMWrappers;
    mutable std::unique_ptr<CSSRuleList> m_ruleListCSSOMWrapper;
};

} // namespace WebCore

#endif // CSSGroupingRule_h
