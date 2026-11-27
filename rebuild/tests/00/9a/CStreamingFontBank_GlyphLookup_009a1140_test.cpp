#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_PASS22_STREAMING_FONT_CTOR
#include "engine/CStreamingGlyphLookup.h"
#include "fable_assert.h"
static void check(bool ok) { if (!ok) exit(1); }
struct StreamingFontCacheEntry { long lastUseTime; unsigned long glyphIndex; };
struct StreamingFontCacheBufferData { StreamingFontCacheEntry* entries; };
namespace {
StreamingFontCacheEntry small[32], big[32];
StreamingFontCacheBufferData g_StreamingFontCacheBufferData={small},g_StreamingFontCacheBufferData2={big};
}
static long frames,finds,loads,clockReads;
static unsigned short wanted;
static CStreamingGlyph glyph;
static unsigned long chunk;
static const int per=5;
static unsigned long offsets[]={0,7,21,46,77,101};
static unsigned char packed[101];
static CStreamingGlyphData decoded[per];
static void fill(int which) {
    for (int i=0;i<per;++i) {
        decoded[i].Width=(signed char)(which*8+i);
        decoded[i].BigChar=(which+i)&1;
        decoded[i].Advance=which*9+i;
        decoded[i].MemOffset=(which*23+i)*2;
    }
}
class CDisplayManager {public:long GetFrameCounter() const { ++clockReads; return frames; }};
class CSystemManager {public:CDisplayManager display; CDisplayManager* GetDisplayManager(){return &display;}};
static CSystemManager systemManager;
static CSystemManager* GFGetSystemManager(){return &systemManager;}
class CStreamingFontBank {
public:
    unsigned long GlyphFlags[2048],fontIndex;
    const CStreamingGlyph* FindGlyph(unsigned short ch) const {check(ch==wanted); ++finds; return &glyph;}
    CStreamingGlyphLookup* GlyphLookup(unsigned short ch) const;
};
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

namespace {
CStreamingGlyphLookup glyphLookup[glyphLookupNumBins][glyphLookupNumEntriesPerBin];
unsigned long numTextureCacheGlyphDatasPerChunk=per;
unsigned long glyphTextureCacheGlyphDataCacheChunk;
unsigned long* glyphTextureCacheGlyphDataIndices=offsets;
void* glyphTextureCacheGlyphData=packed;
CStreamingGlyphData* glyphTextureCacheGlyphDataCache=decoded;
}
extern "C" int FABLE_CDECL lzo1x_decompress_safe(const unsigned char* input,unsigned int size,
        unsigned char* output,unsigned int* outputSize,void* work) {
    check(input==packed+offsets[chunk] && size==offsets[chunk+1]-offsets[chunk]);
    check(output==(unsigned char*)decoded && *outputSize==per*sizeof(CStreamingGlyphData) && !work);
    fill(chunk); ++loads; return 0;
}
static void run(int choice,int slot,bool resident,bool fallback,int mixed) {
    CStreamingFontBank bank; memset(&bank,0,sizeof(bank));
    const unsigned short ask=0x14a2;
    wanted=fallback?0x25a1:ask;
    bank.GlyphFlags[wanted>>5]|=1ul<<(wanted&31); bank.fontIndex=3;
    const unsigned long bin=wanted&255;
    for (int k=0;k<8;++k) {
        CStreamingGlyphLookup& e=glyphLookup[bin][k]; memset(&e,0,sizeof(e));
        e.ch=wanted; e.font=5; e.cachePos=k; e.indices.GlyphIndex=k+60;
        e.data.BigChar=(mixed>>k)&1;
        small[k].glyphIndex=big[k].glyphIndex=k+60;
        small[k].lastUseTime=big[k].lastUseTime=-2;
    }
    CStreamingGlyphLookup& e=glyphLookup[bin][slot];
    StreamingFontCacheEntry& entry=e.data.BigChar?big[slot]:small[slot];
    int expectedSlot=slot;
    if (choice==0) e.font=bank.fontIndex;
    else if (choice==1) e.ch=0;
    else if (choice==2) entry.glyphIndex=0;
    else if (choice==3) entry.lastUseTime=-42;
    else expectedSlot=7;
    unsigned short originalFont=e.font;
    chunk=(slot+mixed)%5; int within=(choice+slot)%per;
    glyph.GlyphIndex=1+chunk*per+within; glyph.AdjustmentIndex=slot+8;
    glyphTextureCacheGlyphDataCacheChunk=resident?chunk:99;
    memset(decoded,0,sizeof(decoded)); if(resident)fill(chunk);
    finds=loads=clockReads=0; frames=mixed&1?2:0x7ffffffe;
    CStreamingGlyphLookup* p=bank.GlyphLookup(ask);
    check(p==&glyphLookup[bin][expectedSlot] && p->ch==wanted && p->font==bank.fontIndex);
    check(finds==(choice!=0) && loads==(choice!=0 && !resident) && clockReads==(choice>=3));
    if(choice!=0) {
        check(p->cachePos==0 && p->indices.GlyphIndex==glyph.GlyphIndex
              && p->indices.AdjustmentIndex==glyph.AdjustmentIndex && p->data.Width==chunk*8+within);
        check(p->data.MemOffset==(chunk*23+within)*2 && p->data.Advance==chunk*9+within);
        check(glyphTextureCacheGlyphDataCacheChunk==chunk && originalFont!=bank.fontIndex);
    }
}
int main() {
    CStreamingFontBank empty; memset(&empty,0,sizeof(empty));
    check(!empty.GlyphLookup(0x1234) && !empty.GlyphLookup(0x25a1));
    for(int c=0;c<5;++c) for(int s=0;s<8;++s) for(int r=0;r<2;++r)
      for(int f=0;f<2;++f) for(int m=0;m<256;m+=17) run(c,s,r!=0,f!=0,m);
    puts("009a1140_TEST PASS");return 0;
}