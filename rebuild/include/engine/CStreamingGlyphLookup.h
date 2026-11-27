#ifndef FABLE_ENGINE_CSTREAMINGGLYPHLOOKUP_H
#define FABLE_ENGINE_CSTREAMINGGLYPHLOOKUP_H
#include "engine/CStreamingGlyph.h"
#include "engine/CStreamingGlyphData.h"
// Both original types: font is the low nibble, cachePos the upper 12 bits.
struct CStreamingGlyphLookup {
    unsigned short font:4, cachePos:12;
    unsigned short ch;
    CStreamingGlyph indices;
    CStreamingGlyphData data;
};
FABLE_STATIC_ASSERT_AT(12, sizeof(CStreamingGlyphLookup)==16);
#endif
