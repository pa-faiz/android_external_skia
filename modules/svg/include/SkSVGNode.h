/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkSVGNode_DEFINED
#define SkSVGNode_DEFINED

#include "include/core/SkRefCnt.h"
#include "modules/svg/include/SkSVGAttribute.h"

class SkCanvas;
class SkMatrix;
class SkPaint;
class SkPath;
class SkSVGLengthContext;
class SkSVGRenderContext;
class SkSVGValue;

enum class SkSVGTag {
    kCircle,
    kClipPath,
    kDefs,
    kEllipse,
    kFilter,
    kG,
    kLine,
    kLinearGradient,
    kPath,
    kPattern,
    kPolygon,
    kPolyline,
    kRadialGradient,
    kRect,
    kStop,
    kSvg,
    kText,
    kUse
};

#define SVG_PRES_ATTR(attr_name, attr_type, attr_inherited)             \
    const attr_type* get##attr_name() const {                           \
        return fPresentationAttributes.f##attr_name.getMaybeNull();     \
    }                                                                   \
    void set##attr_name(const attr_type& v) {                           \
        if (!attr_inherited || v.type() != attr_type::Type::kInherit) { \
            fPresentationAttributes.f##attr_name.set(v);                \
        } else {                                                        \
            /* kInherited values are semantically equivalent to         \
               the absence of a local presentation attribute.*/         \
            fPresentationAttributes.f##attr_name.reset();               \
        }                                                               \
    }                                                                   \
    void set##attr_name(attr_type&& v) {                                \
        if (!attr_inherited || v.type() != attr_type::Type::kInherit) { \
            fPresentationAttributes.f##attr_name.set(std::move(v));     \
        } else {                                                        \
            /* kInherited values are semantically equivalent to         \
               the absence of a local presentation attribute.*/         \
            fPresentationAttributes.f##attr_name.reset();               \
        }                                                               \
    }

class SkSVGNode : public SkRefCnt {
public:
    ~SkSVGNode() override;

    SkSVGTag tag() const { return fTag; }

    virtual void appendChild(sk_sp<SkSVGNode>) = 0;

    void render(const SkSVGRenderContext&) const;
    bool asPaint(const SkSVGRenderContext&, SkPaint*) const;
    SkPath asPath(const SkSVGRenderContext&) const;
    SkRect objectBoundingBox(const SkSVGRenderContext&) const;

    void setAttribute(SkSVGAttribute, const SkSVGValue&);
    bool setAttribute(const char* attributeName, const char* attributeValue);

    void setClipPath(const SkSVGClip&);
    void setClipRule(const SkSVGFillRule&);
    void setColor(const SkSVGColorType&);
    void setFill(const SkSVGPaint&);
    void setFillOpacity(const SkSVGNumberType&);
    void setFillRule(const SkSVGFillRule&);
    void setOpacity(const SkSVGNumberType&);
    void setStroke(const SkSVGPaint&);
    void setStrokeDashArray(const SkSVGDashArray&);
    void setStrokeDashOffset(const SkSVGLength&);
    void setStrokeOpacity(const SkSVGNumberType&);
    void setStrokeLineCap(const SkSVGLineCap&);
    void setStrokeLineJoin(const SkSVGLineJoin&);
    void setStrokeMiterLimit(const SkSVGNumberType&);
    void setStrokeWidth(const SkSVGLength&);
    void setVisibility(const SkSVGVisibility&);

    SVG_PRES_ATTR(FontFamily, SkSVGFontFamily, true)
    SVG_PRES_ATTR(FontStyle , SkSVGFontStyle , true)
    SVG_PRES_ATTR(FontSize  , SkSVGFontSize  , true)
    SVG_PRES_ATTR(FontWeight, SkSVGFontWeight, true)
    SVG_PRES_ATTR(TextAnchor, SkSVGTextAnchor, true)
    SVG_PRES_ATTR(Filter    , SkSVGFilterType, false)

protected:
    SkSVGNode(SkSVGTag);

    // Called before onRender(), to apply local attributes to the context.  Unlike onRender(),
    // onPrepareToRender() bubbles up the inheritance chain: overriders should always call
    // INHERITED::onPrepareToRender(), unless they intend to short-circuit rendering
    // (return false).
    // Implementations are expected to return true if rendering is to continue, or false if
    // the node/subtree rendering is disabled.
    virtual bool onPrepareToRender(SkSVGRenderContext*) const;

    virtual void onRender(const SkSVGRenderContext&) const = 0;

    virtual bool onAsPaint(const SkSVGRenderContext&, SkPaint*) const { return false; }

    virtual SkPath onAsPath(const SkSVGRenderContext&) const = 0;

    virtual void onSetAttribute(SkSVGAttribute, const SkSVGValue&);

    virtual bool hasChildren() const { return false; }

    virtual SkRect onObjectBoundingBox(const SkSVGRenderContext&) const {
        return SkRect::MakeEmpty();
    }

private:
    SkSVGTag                    fTag;

    // FIXME: this should be sparse
    SkSVGPresentationAttributes fPresentationAttributes;

    using INHERITED = SkRefCnt;
};

#undef SVG_PRES_ATTR // presentation attributes are only defined for the base class

#define SVG_ATTR(attr_name, attr_type, attr_default)                      \
    private:                                                              \
        attr_type f##attr_name = attr_default;                            \
    public:                                                               \
        const attr_type& get##attr_name() const { return f##attr_name; }  \
        void set##attr_name(const attr_type& a) { f##attr_name = a; }     \
        void set##attr_name(attr_type&& a) { f##attr_name = std::move(a); }

#endif // SkSVGNode_DEFINED
