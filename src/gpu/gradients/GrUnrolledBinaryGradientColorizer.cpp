/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrUnrolledBinaryGradientColorizer.fp; do not modify.
 **************************************************************************************************/
#include "GrUnrolledBinaryGradientColorizer.h"
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLProgramBuilder.h"
#include "GrTexture.h"
#include "SkSLCPP.h"
#include "SkSLUtil.h"
class GrGLSLUnrolledBinaryGradientColorizer : public GrGLSLFragmentProcessor {
public:
    GrGLSLUnrolledBinaryGradientColorizer() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrUnrolledBinaryGradientColorizer& _outer =
                args.fFp.cast<GrUnrolledBinaryGradientColorizer>();
        (void)_outer;
        auto intervalCount = _outer.intervalCount();
        (void)intervalCount;
        auto scale0_1 = _outer.scale0_1();
        (void)scale0_1;
        auto scale2_3 = _outer.scale2_3();
        (void)scale2_3;
        auto scale4_5 = _outer.scale4_5();
        (void)scale4_5;
        auto scale6_7 = _outer.scale6_7();
        (void)scale6_7;
        auto scale8_9 = _outer.scale8_9();
        (void)scale8_9;
        auto scale10_11 = _outer.scale10_11();
        (void)scale10_11;
        auto scale12_13 = _outer.scale12_13();
        (void)scale12_13;
        auto scale14_15 = _outer.scale14_15();
        (void)scale14_15;
        auto bias0_1 = _outer.bias0_1();
        (void)bias0_1;
        auto bias2_3 = _outer.bias2_3();
        (void)bias2_3;
        auto bias4_5 = _outer.bias4_5();
        (void)bias4_5;
        auto bias6_7 = _outer.bias6_7();
        (void)bias6_7;
        auto bias8_9 = _outer.bias8_9();
        (void)bias8_9;
        auto bias10_11 = _outer.bias10_11();
        (void)bias10_11;
        auto bias12_13 = _outer.bias12_13();
        (void)bias12_13;
        auto bias14_15 = _outer.bias14_15();
        (void)bias14_15;
        auto thresholds1_7 = _outer.thresholds1_7();
        (void)thresholds1_7;
        auto thresholds9_13 = _outer.thresholds9_13();
        (void)thresholds9_13;
        fScale0_1Var = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kFloat4_GrSLType,
                                                        kDefault_GrSLPrecision, "scale0_1");
        if (intervalCount > 1) {
            fScale2_3Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale2_3");
        }
        if (intervalCount > 2) {
            fScale4_5Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale4_5");
        }
        if (intervalCount > 3) {
            fScale6_7Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale6_7");
        }
        if (intervalCount > 4) {
            fScale8_9Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale8_9");
        }
        if (intervalCount > 5) {
            fScale10_11Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale10_11");
        }
        if (intervalCount > 6) {
            fScale12_13Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale12_13");
        }
        if (intervalCount > 7) {
            fScale14_15Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "scale14_15");
        }
        fBias0_1Var = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kFloat4_GrSLType,
                                                       kDefault_GrSLPrecision, "bias0_1");
        if (intervalCount > 1) {
            fBias2_3Var = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kFloat4_GrSLType,
                                                           kDefault_GrSLPrecision, "bias2_3");
        }
        if (intervalCount > 2) {
            fBias4_5Var = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kFloat4_GrSLType,
                                                           kDefault_GrSLPrecision, "bias4_5");
        }
        if (intervalCount > 3) {
            fBias6_7Var = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kFloat4_GrSLType,
                                                           kDefault_GrSLPrecision, "bias6_7");
        }
        if (intervalCount > 4) {
            fBias8_9Var = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kFloat4_GrSLType,
                                                           kDefault_GrSLPrecision, "bias8_9");
        }
        if (intervalCount > 5) {
            fBias10_11Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "bias10_11");
        }
        if (intervalCount > 6) {
            fBias12_13Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "bias12_13");
        }
        if (intervalCount > 7) {
            fBias14_15Var = args.fUniformHandler->addUniform(
                    kFragment_GrShaderFlag, kFloat4_GrSLType, kDefault_GrSLPrecision, "bias14_15");
        }
        fThresholds1_7Var = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kHalf4_GrSLType, kDefault_GrSLPrecision, "thresholds1_7");
        fThresholds9_13Var = args.fUniformHandler->addUniform(
                kFragment_GrShaderFlag, kHalf4_GrSLType, kDefault_GrSLPrecision, "thresholds9_13");
        fragBuilder->codeAppendf(
                "half t = %s.x;\nfloat4 scale, bias;\nif (%d <= 4 || t < %s.w) {\n    if (%d <= 2 "
                "|| t < %s.y) {\n        if (%d <= 1 || t < %s.x) {\n            scale = %s;\n     "
                "       bias = %s;\n        } else {\n            scale = %s;\n            bias = "
                "%s;\n        }\n    } else {\n        if (%d <= 3 || t < %s.z) {\n            "
                "scale = %s;\n            bias = %s;\n        } else {\n            scale = %s;\n  "
                "          bias = %s;\n        }\n    }\n} else {\n    if (%d <= 6 || t < %s.y) "
                "{\n        if (%d <= 5 || t <",
                args.fInputColor, _outer.intervalCount(),
                args.fUniformHandler->getUniformCStr(fThresholds1_7Var), _outer.intervalCount(),
                args.fUniformHandler->getUniformCStr(fThresholds1_7Var), _outer.intervalCount(),
                args.fUniformHandler->getUniformCStr(fThresholds1_7Var),
                args.fUniformHandler->getUniformCStr(fScale0_1Var),
                args.fUniformHandler->getUniformCStr(fBias0_1Var),
                fScale2_3Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale2_3Var)
                                       : "float4(0)",
                fBias2_3Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias2_3Var)
                                      : "float4(0)",
                _outer.intervalCount(), args.fUniformHandler->getUniformCStr(fThresholds1_7Var),
                fScale4_5Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale4_5Var)
                                       : "float4(0)",
                fBias4_5Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias4_5Var)
                                      : "float4(0)",
                fScale6_7Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale6_7Var)
                                       : "float4(0)",
                fBias6_7Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias6_7Var)
                                      : "float4(0)",
                _outer.intervalCount(), args.fUniformHandler->getUniformCStr(fThresholds9_13Var),
                _outer.intervalCount());
        fragBuilder->codeAppendf(
                " %s.x) {\n            scale = %s;\n            bias = %s;\n        } else {\n     "
                "       scale = %s;\n            bias = %s;\n        }\n    } else {\n        if "
                "(%d <= 7 || t < %s.z) {\n            scale = %s;\n            bias = %s;\n        "
                "} else {\n            scale = %s;\n            bias = %s;\n        }\n    "
                "}\n}\n%s = half4(float(t) * scale + bias);\n",
                args.fUniformHandler->getUniformCStr(fThresholds9_13Var),
                fScale8_9Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale8_9Var)
                                       : "float4(0)",
                fBias8_9Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias8_9Var)
                                      : "float4(0)",
                fScale10_11Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale10_11Var)
                                         : "float4(0)",
                fBias10_11Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias10_11Var)
                                        : "float4(0)",
                _outer.intervalCount(), args.fUniformHandler->getUniformCStr(fThresholds9_13Var),
                fScale12_13Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale12_13Var)
                                         : "float4(0)",
                fBias12_13Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias12_13Var)
                                        : "float4(0)",
                fScale14_15Var.isValid() ? args.fUniformHandler->getUniformCStr(fScale14_15Var)
                                         : "float4(0)",
                fBias14_15Var.isValid() ? args.fUniformHandler->getUniformCStr(fBias14_15Var)
                                        : "float4(0)",
                args.fOutputColor);
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrUnrolledBinaryGradientColorizer& _outer =
                _proc.cast<GrUnrolledBinaryGradientColorizer>();
        {
            pdman.set4fv(fScale0_1Var, 1, (_outer.scale0_1()).fRGBA);
            if (fScale2_3Var.isValid()) {
                pdman.set4fv(fScale2_3Var, 1, (_outer.scale2_3()).fRGBA);
            }
            if (fScale4_5Var.isValid()) {
                pdman.set4fv(fScale4_5Var, 1, (_outer.scale4_5()).fRGBA);
            }
            if (fScale6_7Var.isValid()) {
                pdman.set4fv(fScale6_7Var, 1, (_outer.scale6_7()).fRGBA);
            }
            if (fScale8_9Var.isValid()) {
                pdman.set4fv(fScale8_9Var, 1, (_outer.scale8_9()).fRGBA);
            }
            if (fScale10_11Var.isValid()) {
                pdman.set4fv(fScale10_11Var, 1, (_outer.scale10_11()).fRGBA);
            }
            if (fScale12_13Var.isValid()) {
                pdman.set4fv(fScale12_13Var, 1, (_outer.scale12_13()).fRGBA);
            }
            if (fScale14_15Var.isValid()) {
                pdman.set4fv(fScale14_15Var, 1, (_outer.scale14_15()).fRGBA);
            }
            pdman.set4fv(fBias0_1Var, 1, (_outer.bias0_1()).fRGBA);
            if (fBias2_3Var.isValid()) {
                pdman.set4fv(fBias2_3Var, 1, (_outer.bias2_3()).fRGBA);
            }
            if (fBias4_5Var.isValid()) {
                pdman.set4fv(fBias4_5Var, 1, (_outer.bias4_5()).fRGBA);
            }
            if (fBias6_7Var.isValid()) {
                pdman.set4fv(fBias6_7Var, 1, (_outer.bias6_7()).fRGBA);
            }
            if (fBias8_9Var.isValid()) {
                pdman.set4fv(fBias8_9Var, 1, (_outer.bias8_9()).fRGBA);
            }
            if (fBias10_11Var.isValid()) {
                pdman.set4fv(fBias10_11Var, 1, (_outer.bias10_11()).fRGBA);
            }
            if (fBias12_13Var.isValid()) {
                pdman.set4fv(fBias12_13Var, 1, (_outer.bias12_13()).fRGBA);
            }
            if (fBias14_15Var.isValid()) {
                pdman.set4fv(fBias14_15Var, 1, (_outer.bias14_15()).fRGBA);
            }
            pdman.set4fv(fThresholds1_7Var, 1,
                         reinterpret_cast<const float*>(&(_outer.thresholds1_7())));
            pdman.set4fv(fThresholds9_13Var, 1,
                         reinterpret_cast<const float*>(&(_outer.thresholds9_13())));
        }
    }
    UniformHandle fScale0_1Var;
    UniformHandle fScale2_3Var;
    UniformHandle fScale4_5Var;
    UniformHandle fScale6_7Var;
    UniformHandle fScale8_9Var;
    UniformHandle fScale10_11Var;
    UniformHandle fScale12_13Var;
    UniformHandle fScale14_15Var;
    UniformHandle fBias0_1Var;
    UniformHandle fBias2_3Var;
    UniformHandle fBias4_5Var;
    UniformHandle fBias6_7Var;
    UniformHandle fBias8_9Var;
    UniformHandle fBias10_11Var;
    UniformHandle fBias12_13Var;
    UniformHandle fBias14_15Var;
    UniformHandle fThresholds1_7Var;
    UniformHandle fThresholds9_13Var;
};
GrGLSLFragmentProcessor* GrUnrolledBinaryGradientColorizer::onCreateGLSLInstance() const {
    return new GrGLSLUnrolledBinaryGradientColorizer();
}
void GrUnrolledBinaryGradientColorizer::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                                              GrProcessorKeyBuilder* b) const {
    b->add32((int32_t)fIntervalCount);
}
bool GrUnrolledBinaryGradientColorizer::onIsEqual(const GrFragmentProcessor& other) const {
    const GrUnrolledBinaryGradientColorizer& that = other.cast<GrUnrolledBinaryGradientColorizer>();
    (void)that;
    if (fIntervalCount != that.fIntervalCount) return false;
    if (fScale0_1 != that.fScale0_1) return false;
    if (fScale2_3 != that.fScale2_3) return false;
    if (fScale4_5 != that.fScale4_5) return false;
    if (fScale6_7 != that.fScale6_7) return false;
    if (fScale8_9 != that.fScale8_9) return false;
    if (fScale10_11 != that.fScale10_11) return false;
    if (fScale12_13 != that.fScale12_13) return false;
    if (fScale14_15 != that.fScale14_15) return false;
    if (fBias0_1 != that.fBias0_1) return false;
    if (fBias2_3 != that.fBias2_3) return false;
    if (fBias4_5 != that.fBias4_5) return false;
    if (fBias6_7 != that.fBias6_7) return false;
    if (fBias8_9 != that.fBias8_9) return false;
    if (fBias10_11 != that.fBias10_11) return false;
    if (fBias12_13 != that.fBias12_13) return false;
    if (fBias14_15 != that.fBias14_15) return false;
    if (fThresholds1_7 != that.fThresholds1_7) return false;
    if (fThresholds9_13 != that.fThresholds9_13) return false;
    return true;
}
GrUnrolledBinaryGradientColorizer::GrUnrolledBinaryGradientColorizer(
        const GrUnrolledBinaryGradientColorizer& src)
        : INHERITED(kGrUnrolledBinaryGradientColorizer_ClassID, src.optimizationFlags())
        , fIntervalCount(src.fIntervalCount)
        , fScale0_1(src.fScale0_1)
        , fScale2_3(src.fScale2_3)
        , fScale4_5(src.fScale4_5)
        , fScale6_7(src.fScale6_7)
        , fScale8_9(src.fScale8_9)
        , fScale10_11(src.fScale10_11)
        , fScale12_13(src.fScale12_13)
        , fScale14_15(src.fScale14_15)
        , fBias0_1(src.fBias0_1)
        , fBias2_3(src.fBias2_3)
        , fBias4_5(src.fBias4_5)
        , fBias6_7(src.fBias6_7)
        , fBias8_9(src.fBias8_9)
        , fBias10_11(src.fBias10_11)
        , fBias12_13(src.fBias12_13)
        , fBias14_15(src.fBias14_15)
        , fThresholds1_7(src.fThresholds1_7)
        , fThresholds9_13(src.fThresholds9_13) {}
std::unique_ptr<GrFragmentProcessor> GrUnrolledBinaryGradientColorizer::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrUnrolledBinaryGradientColorizer(*this));
}

static const int kMaxIntervals = 8;
std::unique_ptr<GrFragmentProcessor> GrUnrolledBinaryGradientColorizer::Make(
        const GrColor4f* colors, const SkScalar* positions, int count) {
    // Depending on how the positions resolve into hard stops or regular stops, the number of
    // intervals specified by the number of colors/positions can change. For instance, a plain
    // 3 color gradient is two intervals, but a 4 color gradient with a hard stop is also
    // two intervals. At the most extreme end, an 8 interval gradient made entirely of hard
    // stops has 16 colors.

    if (count > kMaxColorCount) {
        // Definitely cannot represent this gradient configuration
        return nullptr;
    }

    // The raster implementation also uses scales and biases, but since they must be calculated
    // after the dst color space is applied, it limits our ability to cache their values.
    GrColor4f scales[kMaxIntervals];
    GrColor4f biases[kMaxIntervals];
    SkScalar thresholds[kMaxIntervals];

    int intervalCount = 0;

    for (int i = 0; i < count - 1; i++) {
        if (intervalCount >= kMaxIntervals) {
            // Already reached kMaxIntervals, and haven't run out of color stops so this
            // gradient cannot be represented by this shader.
            return nullptr;
        }

        SkScalar t0 = positions[i];
        SkScalar t1 = positions[i + 1];
        SkScalar dt = t1 - t0;
        // If the interval is empty, skip to the next interval. This will automatically create
        // distinct hard stop intervals as needed. It also protects against malformed gradients
        // that have repeated hard stops at the very beginning that are effectively unreachable.
        if (SkScalarNearlyZero(dt)) {
            continue;
        }

        auto c0 = Sk4f::Load(colors[i].fRGBA);
        auto c1 = Sk4f::Load(colors[i + 1].fRGBA);

        auto scale = (c1 - c0) / dt;
        auto bias = c0 - t0 * scale;

        scale.store(scales + intervalCount);
        bias.store(biases + intervalCount);
        thresholds[intervalCount] = t1;
        intervalCount++;
    }

    // For isEqual to make sense, set the unused values to something consistent
    for (int i = intervalCount; i < kMaxIntervals; i++) {
        scales[i] = GrColor4f::TransparentBlack();
        biases[i] = GrColor4f::TransparentBlack();
        thresholds[i] = 0.0;
    }

    return std::unique_ptr<GrFragmentProcessor>(new GrUnrolledBinaryGradientColorizer(
            intervalCount, scales[0], scales[1], scales[2], scales[3], scales[4], scales[5],
            scales[6], scales[7], biases[0], biases[1], biases[2], biases[3], biases[4], biases[5],
            biases[6], biases[7],
            SkRect::MakeLTRB(thresholds[0], thresholds[1], thresholds[2], thresholds[3]),
            SkRect::MakeLTRB(thresholds[4], thresholds[5], thresholds[6], 0.0)));
}
