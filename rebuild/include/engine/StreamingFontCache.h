#ifdef FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#ifndef FABLE_ENGINE_STREAMINGFONT_CACHE_H
#define FABLE_ENGINE_STREAMINGFONT_CACHE_H
#include "fable_compiler.h"
// Retail opt-in (Codex pass22): original module1059 anonymous-namespace cache types and actual private identifiers.
class CStreamingFontBank;class CStreamingFontDataBank;
namespace {
struct StreamingFontCacheEntry {unsigned short older,newer;long lastUseTime;unsigned long glyphIndex:24;unsigned long loading:1;unsigned long cacheIndex;};
struct StreamingFontCacheBufferData {
 const unsigned long width,height,numEntries,entryWidth,entryHeight,entrySize;
 StreamingFontCacheEntry* const entries;
 unsigned long newestEntry,oldestEntry;
 StreamingFontCacheBufferData* const nextSmaller;
 StreamingFontCacheBufferData* const nextBigger;
};
template<unsigned long W,unsigned long H>struct StreamingFontCacheBufferData_static:StreamingFontCacheBufferData {
 StreamingFontCacheEntry entries[(1024/W)*(512/H)];
};
void FABLE_FASTCALL InitializeStreamingFontTextureCache(CStreamingFontDataBank*);
void FABLE_FASTCALL InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData&);
extern StreamingFontCacheBufferData_static<32,32> g_StreamingFontCacheBufferData;
extern StreamingFontCacheBufferData_static<64,64> g_StreamingFontCacheBufferData2;
extern CStreamingFontBank* streamingFontBankList[8];
}
#endif
#endif
