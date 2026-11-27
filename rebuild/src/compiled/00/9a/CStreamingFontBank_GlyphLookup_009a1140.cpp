// Leaf: ?GlyphLookup@CStreamingFontBank@@IBEPAUCStreamingGlyphLookup@@G@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_GLYPH_LOOKUP_PASS29
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP
#define FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#include "fable_assert.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CStreamingFontBank.h"
#include "engine/StreamingFontCache.h"
#include "engine/CStreamingGlyphLookup.h"

extern "C" int FABLE_CDECL lzo1x_decompress_safe(const unsigned char*, unsigned int, unsigned char*, unsigned int*, void*);
namespace {
const unsigned long glyphLookupNumBins = 256;
const unsigned long glyphLookupNumEntriesPerBin = 8;
extern CStreamingGlyphLookup glyphLookup[glyphLookupNumBins][glyphLookupNumEntriesPerBin];
extern unsigned long numTextureCacheGlyphDatasPerChunk;
extern unsigned long glyphTextureCacheGlyphDataCacheChunk;
extern unsigned long* glyphTextureCacheGlyphDataIndices;
extern void* glyphTextureCacheGlyphData;
extern CStreamingGlyphData* glyphTextureCacheGlyphDataCache;

inline bool FABLE_FASTCALL IsGlyphDirty(bool bigChar, unsigned long cachePos, unsigned long glyphIndex)
{
    StreamingFontCacheBufferData& cacheData = bigChar
        ? static_cast<StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData2)
        : static_cast<StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData);
    StreamingFontCacheEntry* pentry = &cacheData.entries[cachePos];
    return pentry->glyphIndex != glyphIndex;
}
}
CStreamingGlyphLookup* CStreamingFontBank::GlyphLookup(unsigned short ch) const
{
    if (!(GlyphFlags[ch >> 5] & (1UL << (ch & 31)))) {
        FABLE_RETAIL_ASSERT(ch != 0x25A1);
        if (ch == 0x25A1) return 0;
        return GlyphLookup(0x25A1);
    }
    const unsigned long bin = ch % glyphLookupNumBins;
    unsigned long index;
    for (index=0; index<glyphLookupNumEntriesPerBin; ++index)
        if (glyphLookup[bin][index].ch == ch && glyphLookup[bin][index].font == fontIndex) break;
    if (index == glyphLookupNumEntriesPerBin) {
        for (index=0; index<glyphLookupNumEntriesPerBin; ++index)
            if (!glyphLookup[bin][index].ch) break;
        if (index == glyphLookupNumEntriesPerBin) {
            for (index=0; index<glyphLookupNumEntriesPerBin; ++index)
                if (IsGlyphDirty(glyphLookup[bin][index].data.BigChar != 0,
                                 glyphLookup[bin][index].cachePos,
                                 glyphLookup[bin][index].indices.GlyphIndex)) break;
            FABLE_RETAIL_ASSERT(index < glyphLookupNumEntriesPerBin);
            if (index == glyphLookupNumEntriesPerBin) {
                index = 0;
                const unsigned long currTime = GFGetSystemManager()->GetDisplayManager()->GetFrameCounter();
                unsigned long lastTimeDif = 0;
                for (unsigned long i=0; i<glyphLookupNumEntriesPerBin; ++i) {
                    const CStreamingGlyphLookup& lookup = glyphLookup[bin][i];
                    StreamingFontCacheBufferData& cacheData = lookup.data.BigChar
                        ? static_cast<StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData2)
                        : static_cast<StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData);
                    StreamingFontCacheEntry& entry = cacheData.entries[lookup.cachePos];
                    const unsigned long timeDif = currTime - entry.lastUseTime;
                    if (lastTimeDif <= timeDif) { index=i; lastTimeDif=timeDif; }
                }
                if (index == glyphLookupNumEntriesPerBin) return 0;
            }
        }
        const CStreamingGlyph* pglyph = FindGlyph(ch);
        FABLE_RETAIL_ASSERT(pglyph);
        const unsigned long curChunk = (pglyph->GlyphIndex-1) / numTextureCacheGlyphDatasPerChunk;
        const unsigned long curIndex = (pglyph->GlyphIndex-1) % numTextureCacheGlyphDatasPerChunk;
        if (glyphTextureCacheGlyphDataCacheChunk != curChunk) {
            unsigned int output_size = numTextureCacheGlyphDatasPerChunk * sizeof(CStreamingGlyphData);
            lzo1x_decompress_safe(static_cast<const unsigned char*>(glyphTextureCacheGlyphData) + glyphTextureCacheGlyphDataIndices[curChunk],
                                  glyphTextureCacheGlyphDataIndices[curChunk+1]-glyphTextureCacheGlyphDataIndices[curChunk],
                                  reinterpret_cast<unsigned char*>(glyphTextureCacheGlyphDataCache), &output_size, 0);
            glyphTextureCacheGlyphDataCacheChunk = curChunk;
        }
        glyphLookup[bin][index].font = static_cast<unsigned short>(fontIndex);
        glyphLookup[bin][index].cachePos = 0;
        glyphLookup[bin][index].ch = ch;
        glyphLookup[bin][index].indices = *pglyph;
        glyphLookup[bin][index].data = glyphTextureCacheGlyphDataCache[curIndex];
    }
    return &glyphLookup[bin][index];
}