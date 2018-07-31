/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrMagnifierEffect.fp; do not modify.
 **************************************************************************************************/
#include "GrMagnifierEffect.h"
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLProgramBuilder.h"
#include "GrTexture.h"
#include "SkSLCPP.h"
#include "SkSLUtil.h"
class GrGLSLMagnifierEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLMagnifierEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrMagnifierEffect& _outer = args.fFp.cast<GrMagnifierEffect>();
        (void)_outer;
        auto bounds = _outer.bounds();
        (void)bounds;
        auto srcRect = _outer.srcRect();
        (void)srcRect;
        auto xInvZoom = _outer.xInvZoom();
        (void)xInvZoom;
        auto yInvZoom = _outer.yInvZoom();
        (void)yInvZoom;
        auto xInvInset = _outer.xInvInset();
        (void)xInvInset;
        auto yInvInset = _outer.yInvInset();
        (void)yInvInset;
        fBoundsUniformVar = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "boundsUniform");
        fXInvZoomVar = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kFloat_GrSLType, kDefault_GrSLPrecision, "xInvZoom");
        fYInvZoomVar = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kFloat_GrSLType, kDefault_GrSLPrecision, "yInvZoom");
        fXInvInsetVar = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kFloat_GrSLType, kDefault_GrSLPrecision, "xInvInset");
        fYInvInsetVar = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kFloat_GrSLType, kDefault_GrSLPrecision, "yInvInset");
        fOffsetVar = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kHalf2_GrSLType, kDefault_GrSLPrecision, "offset");
        SkString sk_TransformedCoords2D_0 = fragBuilder->ensureCoords2D(args.fTransformedCoords[0]);
        fragBuilder->codeAppendf(
                "float2 coord = %s;\nfloat2 zoom_coord = float2(%s + half2(coord * "
                "float2(half2(half(%s), half(%s)))));\nfloat2 delta = (coord - %s.xy) * "
                "%s.zw;\ndelta = min(delta, float2(half2(1.0, 1.0) - half2(delta)));\ndelta *= "
                "float2(half2(half(%s), half(%s)));\nhalf weight = 0.0;\nif (delta.x < 2.0 && "
                "delta.y < 2.0) {\n    delta = float2(half2(2.0, 2.0) - half2(delta));\n    half "
                "dist = half(length(delta));\n    dist = half(max(2.0 - float(dist), 0.0));\n    "
                "weight = half(min(float(dist * dist), 1.0));\n} else {\n    ",
                sk_TransformedCoords2D_0.c_str(),
                args.fUniformHandler->getUniformCStr(fOffsetVar),
                args.fUniformHandler->getUniformCStr(fXInvZoomVar),
                args.fUniformHandler->getUniformCStr(fYInvZoomVar),
                args.fUniformHandler->getUniformCStr(fBoundsUniformVar),
                args.fUniformHandler->getUniformCStr(fBoundsUniformVar),
                args.fUniformHandler->getUniformCStr(fXInvInsetVar),
                args.fUniformHandler->getUniformCStr(fYInvInsetVar));
        fragBuilder->codeAppendf(
                "float2 delta_squared = delta * delta;\n    weight = half(min(min(delta_squared.x, "
                "delta_squared.y), 1.0));\n}\n%s = texture(%s, mix(coord, zoom_coord, "
                "float(weight))).%s;\n",
                args.fOutputColor,
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[0]).c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[0]).c_str());
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrMagnifierEffect& _outer = _proc.cast<GrMagnifierEffect>();
        {
            pdman.set1f(fXInvZoomVar, _outer.xInvZoom());
            pdman.set1f(fYInvZoomVar, _outer.yInvZoom());
            pdman.set1f(fXInvInsetVar, _outer.xInvInset());
            pdman.set1f(fYInvInsetVar, _outer.yInvInset());
        }
        GrSurfaceProxy& srcProxy = *_outer.textureSampler(0).proxy();
        GrTexture& src = *srcProxy.priv().peekTexture();
        (void)src;
        auto bounds = _outer.bounds();
        (void)bounds;
        UniformHandle& boundsUniform = fBoundsUniformVar;
        (void)boundsUniform;
        auto srcRect = _outer.srcRect();
        (void)srcRect;
        UniformHandle& xInvZoom = fXInvZoomVar;
        (void)xInvZoom;
        UniformHandle& yInvZoom = fYInvZoomVar;
        (void)yInvZoom;
        UniformHandle& xInvInset = fXInvInsetVar;
        (void)xInvInset;
        UniformHandle& yInvInset = fYInvInsetVar;
        (void)yInvInset;
        UniformHandle& offset = fOffsetVar;
        (void)offset;

        SkScalar invW = 1.0f / src.width();
        SkScalar invH = 1.0f / src.height();

        {
            SkScalar y = srcRect.y() * invH;
            if (srcProxy.origin() != kTopLeft_GrSurfaceOrigin) {
                y = 1.0f - (srcRect.height() / bounds.height()) - y;
            }

            pdman.set2f(offset, srcRect.x() * invW, y);
        }

        {
            SkScalar y = bounds.y() * invH;
            if (srcProxy.origin() != kTopLeft_GrSurfaceOrigin) {
                y = 1.0f - bounds.height() * invH;
            }

            pdman.set4f(boundsUniform,
                        bounds.x() * invW,
                        y,
                        SkIntToScalar(src.width()) / bounds.width(),
                        SkIntToScalar(src.height()) / bounds.height());
        }
    }
    UniformHandle fBoundsUniformVar;
    UniformHandle fOffsetVar;
    UniformHandle fXInvZoomVar;
    UniformHandle fYInvZoomVar;
    UniformHandle fXInvInsetVar;
    UniformHandle fYInvInsetVar;
};
GrGLSLFragmentProcessor* GrMagnifierEffect::onCreateGLSLInstance() const {
    return new GrGLSLMagnifierEffect();
}
void GrMagnifierEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                              GrProcessorKeyBuilder* b) const {}
bool GrMagnifierEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrMagnifierEffect& that = other.cast<GrMagnifierEffect>();
    (void)that;
    if (fSrc != that.fSrc) return false;
    if (fBounds != that.fBounds) return false;
    if (fSrcRect != that.fSrcRect) return false;
    if (fXInvZoom != that.fXInvZoom) return false;
    if (fYInvZoom != that.fYInvZoom) return false;
    if (fXInvInset != that.fXInvInset) return false;
    if (fYInvInset != that.fYInvInset) return false;
    return true;
}
GrMagnifierEffect::GrMagnifierEffect(const GrMagnifierEffect& src)
        : INHERITED(kGrMagnifierEffect_ClassID, src.optimizationFlags())
        , fSrc(src.fSrc)
        , fBounds(src.fBounds)
        , fSrcRect(src.fSrcRect)
        , fXInvZoom(src.fXInvZoom)
        , fYInvZoom(src.fYInvZoom)
        , fXInvInset(src.fXInvInset)
        , fYInvInset(src.fYInvInset)
        , fSrcCoordTransform(src.fSrcCoordTransform) {
    this->setTextureSamplerCnt(1);
    this->addCoordTransform(&fSrcCoordTransform);
}
std::unique_ptr<GrFragmentProcessor> GrMagnifierEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrMagnifierEffect(*this));
}
const GrFragmentProcessor::TextureSampler& GrMagnifierEffect::onTextureSampler(int index) const {
    return IthTextureSampler(index, fSrc);
}
GR_DEFINE_FRAGMENT_PROCESSOR_TEST(GrMagnifierEffect);
#if GR_TEST_UTILS
std::unique_ptr<GrFragmentProcessor> GrMagnifierEffect::TestCreate(GrProcessorTestData* d) {
    sk_sp<GrTextureProxy> proxy = d->textureProxy(0);
    const int kMaxWidth = 200;
    const int kMaxHeight = 200;
    const SkScalar kMaxInset = 20.0f;
    uint32_t width = d->fRandom->nextULessThan(kMaxWidth);
    uint32_t height = d->fRandom->nextULessThan(kMaxHeight);
    SkScalar inset = d->fRandom->nextRangeScalar(1.0f, kMaxInset);

    SkIRect bounds = SkIRect::MakeWH(SkIntToScalar(kMaxWidth), SkIntToScalar(kMaxHeight));
    SkRect srcRect = SkRect::MakeWH(SkIntToScalar(width), SkIntToScalar(height));

    auto effect = GrMagnifierEffect::Make(std::move(proxy),
                                          bounds,
                                          srcRect,
                                          srcRect.width() / bounds.width(),
                                          srcRect.height() / bounds.height(),
                                          bounds.width() / inset,
                                          bounds.height() / inset);
    SkASSERT(effect);
    return effect;
}
#endif
