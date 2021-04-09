/*
 * Copyright 2018 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkGlyphRun_DEFINED
#define SkGlyphRun_DEFINED

#include <functional>
#include <vector>

#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPoint.h"
#include "include/core/SkTypes.h"
#include "include/private/SkTemplates.h"
#include "src/core/SkSpan.h"
#include "src/core/SkZip.h"

class SkBaseDevice;
class SkGlyph;
class SkTextBlob;
class SkTextBlobRunIterator;

class SkGlyphRun {
public:
    SkGlyphRun() = default;
    SkGlyphRun(const SkFont& font,
               SkSpan<const SkPoint> positions,
               SkSpan<const SkGlyphID> glyphIDs,
               SkSpan<const char> text,
               SkSpan<const uint32_t> clusters,
               SkSpan<const SkVector> scaledRotations);

    SkGlyphRun(const SkGlyphRun& glyphRun, const SkFont& font);

    size_t runSize() const { return fSource.size(); }
    SkSpan<const SkPoint> positions() const { return fSource.get<1>(); }
    SkSpan<const SkGlyphID> glyphsIDs() const { return fSource.get<0>(); }
    SkZip<const SkGlyphID, const SkPoint> source() const { return fSource; }
    const SkFont& font() const { return fFont; }
    SkSpan<const uint32_t> clusters() const { return fClusters; }
    SkSpan<const char> text() const { return fText; }
    SkSpan<const SkVector> scaledRotations() const { return fScaledRotations; }

private:
    // GlyphIDs and positions.
    const SkZip<const SkGlyphID, const SkPoint> fSource;
    // Original text from SkTextBlob if present. Will be empty of not present.
    const SkSpan<const char> fText;
    // Original clusters from SkTextBlob if present. Will be empty if not present.
    const SkSpan<const uint32_t>   fClusters;
    // Possible RSXForm information
    const SkSpan<const SkVector> fScaledRotations;
    // Font for this run modified to have glyph encoding and left alignment.
    SkFont fFont;
};

class SkGlyphRunList {
    SkSpan<const SkGlyphRun> fGlyphRuns;

public:
    SkGlyphRunList();
    // Blob maybe null.
    SkGlyphRunList(
            const SkTextBlob* blob,
            SkPoint origin,
            SkSpan<const SkGlyphRun> glyphRunList);

    SkGlyphRunList(const SkGlyphRun& glyphRun);

    uint64_t uniqueID() const;
    bool anyRunsLCD() const;
    void temporaryShuntBlobNotifyAddedToCache(uint32_t cacheID) const;

    bool canCache() const { return fOriginalTextBlob != nullptr; }
    size_t runCount() const { return fGlyphRuns.size(); }
    size_t totalGlyphCount() const {
        size_t glyphCount = 0;
        for(const auto& run : fGlyphRuns) {
            glyphCount += run.runSize();
        }
        return glyphCount;
    }

    SkPoint origin() const { return fOrigin; }
    const SkTextBlob* blob() const { return fOriginalTextBlob; }

    auto begin() -> decltype(fGlyphRuns.begin())               { return fGlyphRuns.begin();  }
    auto end()   -> decltype(fGlyphRuns.end())                 { return fGlyphRuns.end();    }
    auto begin() const -> decltype(fGlyphRuns.cbegin())        { return fGlyphRuns.cbegin(); }
    auto end()   const -> decltype(fGlyphRuns.cend())          { return fGlyphRuns.cend();   }
    auto size()  const -> decltype(fGlyphRuns.size())          { return fGlyphRuns.size();   }
    auto empty() const -> decltype(fGlyphRuns.empty())         { return fGlyphRuns.empty();  }
    auto operator [] (size_t i) const -> decltype(fGlyphRuns[i]) { return fGlyphRuns[i];     }

private:
    // The text blob is needed to hookup the call back that the SkTextBlob destructor calls. It
    // should be used for nothing else
    const SkTextBlob*  fOriginalTextBlob{nullptr};
    SkPoint fOrigin = {0, 0};
};

class SkGlyphRunBuilder {
public:
    const SkGlyphRunList& textToGlyphRunList(const SkFont& font,
                                             const void* bytes,
                                             size_t byteLength,
                                             SkPoint origin,
                                             SkTextEncoding encoding = SkTextEncoding::kUTF8);
    void drawTextBlob(const SkPaint& paint, const SkTextBlob& blob, SkPoint origin, SkBaseDevice*);

    void textBlobToGlyphRunListIgnoringRSXForm(const SkTextBlob& blob, SkPoint origin);

    const SkGlyphRunList& useGlyphRunList();

    bool empty() const { return fGlyphRunListStorage.empty(); }

private:
    void initialize(size_t totalRunSize);
    SkSpan<const SkGlyphID> textToGlyphIDs(
            const SkFont& font, const void* bytes, size_t byteLength, SkTextEncoding);

    void makeGlyphRun(
            const SkFont& font,
            SkSpan<const SkGlyphID> glyphIDs,
            SkSpan<const SkPoint> positions,
            SkSpan<const char> text,
            SkSpan<const uint32_t> clusters,
            SkSpan<const SkVector> scaledRotations);

    void makeGlyphRunList(const SkTextBlob* blob, SkPoint origin);

    SkPoint* simplifyTextBlobIgnoringRSXForm(const SkTextBlobRunIterator& it,
                                             SkPoint* positionsCursor);

    size_t fMaxTotalRunSize{0};
    SkAutoTMalloc<SkPoint> fPositions;

    std::vector<SkGlyphRun> fGlyphRunListStorage;
    SkGlyphRunList fGlyphRunList;

    // Used as a temporary for preparing using utfN text. This implies that only one run of
    // glyph ids will ever be needed because blobs are already glyph based.
    std::vector<SkGlyphID> fScratchGlyphIDs;
};

#endif  // SkGlyphRun_DEFINED
