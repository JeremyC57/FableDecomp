// Grade: DIFFER(1004v982) -- drain loop, text loop, glyph lookup, fresh-slot streaming and the pending ring byte-identical in shape and registers; residue: the fresh entry glyphIndex/loading merge loads the lookup indices after the entry (retail before), and the mip-propagation loops colour x/y and scale/row the other way round (retail: x in ebp, scale in ebx, row in a frame slot; ours keeps scale in a frame slot), so the frame is one slot larger
// CStreamingFontBank::Preload(const CWideString& text, bool allowStreaming) const -- retire finished glyph uploads from the pending ring, then for every printable character of the text look up its glyph and make sure it is resident in the font texture cache (stream it in when the cached slot holds another glyph), refreshing the use stamp and clearing the coarser/finer mip cells; false when anything is still loading
// Leaf: Preload
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A2000, 992 bytes, `ret 8` (virtual, Ego_r ?Preload@CStreamingFontBank@@UBE_NABVCWideString@@_N@Z).
// Callees: GFGetSystemManager 0x009A4EC0 (font preload state +0x84), CBankFileAsyncData::IsFinished 0x009D54E0,
//   CWideString::operator const wchar_t* 0x0099AF10, CStreamingFontBank::GlyphLookup 0x009A1140,
//   InsertNewStreamingFontCacheEntry 0x009A1020 / UpdateUseStreamingFontCacheEntry 0x009A0F90 (__fastcall, anonymous namespace),
//   CStreamingFontDataBank::StreamGlyph 0x00A5F010, CCountedPointer<CBankFileAsyncData>::operator= 0x009A4440 / dtor 0x009A4400.
// Globals: pending ring head/tail 0x013C5A74/0x013C5A78 over g_PendingLoadedGlyphs 0x013C82F8 (512 x PendingLoadedGlyph),
//   texture-dirty flag 0x013C5A70, per-entry dirty bytes 0x013BD808, cache memory 0x013BDA24, glyph file base 0x013BDA4C,
//   small/big caches 0x013C5A98/0x013C7AC8.
// Layout: engine/CStreamingFontBank.h (data_bank +0x10); Ego_r.pdb StreamingFontCacheBufferData (44), StreamingFontCacheEntry
//   (16), PendingLoadedGlyph (16), CStreamingGlyphLookup (16); the preload state (+0x34 pending count, +0x38 streaming flag) is
//   descriptive.
#include <string.h>
#pragma intrinsic(memset)
#include "engine/CStreamingFontBank.h"

struct CBankFileAsyncData {
    bool IsFinished();                                  // 0x009D54E0
};
struct CCountInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CAsyncDataPointer {                              // CCountedPointer<CBankFileAsyncData>
    CBankFileAsyncData* Data;
    CCountInfo*         Info;
    ~CAsyncDataPointer();                               // 0x009A4400
    CAsyncDataPointer& operator=(const CAsyncDataPointer& other);   // 0x009A4440
    void Clear()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct CStreamingFontDataBank {
    CAsyncDataPointer StreamGlyph(void* dest, unsigned long offset, unsigned long size, bool wait);   // 0x00A5F010
};
struct CWideString {
    operator const wchar_t*() const;                    // 0x0099AF10
};

struct CStreamingGlyph {
    unsigned long GlyphIndex : 24;
    unsigned long AdjustmentIndex : 8;
};
struct CStreamingGlyphData {
    signed char   OffsetX, OffsetY, Width, Height;
    unsigned long MemOffset : 23;
    unsigned long Advance : 8;
    unsigned long BigChar : 1;
};
struct CStreamingGlyphLookup {
    unsigned short  font : 4;
    unsigned short  cachePos : 12;
    unsigned short  ch;
    CStreamingGlyph indices;
    CStreamingGlyphData data;
};
struct StreamingFontCacheEntry {
    unsigned short older;
    unsigned short newer;
    long           lastUseTime;
    unsigned long  glyphIndex : 24;
    unsigned long  loading : 1;
    unsigned long  cacheIndex;
};
struct StreamingFontCacheBufferData {
    unsigned long width, height, numEntries, entryWidth, entryHeight, entrySize;
    StreamingFontCacheEntry*      entries;              // +0x18
    unsigned long                 newestEntry, oldestEntry;
    StreamingFontCacheBufferData* nextSmaller;          // +0x24
    StreamingFontCacheBufferData* nextBigger;           // +0x28
};
struct PendingLoadedGlyph {
    CAsyncDataPointer data;
    bool              big;
    unsigned long     index;
};
void __fastcall UpdateUseStreamingFontCacheEntry(StreamingFontCacheBufferData& cache, unsigned short index);   // 0x009A0F90
unsigned short __fastcall InsertNewStreamingFontCacheEntry(StreamingFontCacheBufferData& cache);              // 0x009A1020

extern StreamingFontCacheBufferData g_SmallFontCache;    // 0x013C5A98
extern StreamingFontCacheBufferData g_BigFontCache;      // 0x013C7AC8
extern PendingLoadedGlyph g_PendingLoadedGlyphs[512];    // 0x013C82F8
extern unsigned long g_PendingHead;                      // 0x013C5A74
extern unsigned long g_PendingTail;                      // 0x013C5A78
extern bool g_FontTextureDirty;                          // 0x013C5A70
extern bool g_FontEntryDirty[];                          // 0x013BD808
extern unsigned char* g_FontCacheMemory;                 // 0x013BDA24
extern unsigned long g_GlyphFileBase;                    // 0x013BDA4C

__forceinline unsigned char* GetCacheMemoryOffset(StreamingFontCacheBufferData& cache, unsigned short index)
{
    return g_FontCacheMemory + cache.entrySize * index;
}
struct CFontPreloadState {
    unsigned char _pad00[0x34];
    long          PendingGlyphLoads;                     // +0x34
    bool          AllowStreaming;                        // +0x38
};
struct CSystemManager { unsigned char _pad[0x84]; CFontPreloadState* FontPreload; };
CSystemManager* __fastcall GFGetSystemManager();         // 0x009A4EC0

struct CStreamingFontBankX : CStreamingFontBank {
    CStreamingGlyphLookup* GlyphLookup(unsigned short ch) const;   // 0x009A1140
    bool Preload(const CWideString& text, bool allowStreaming) const;
};

bool CStreamingFontBankX::Preload(const CWideString& text, bool allowStreaming) const
{
    bool ok = true;
    CFontPreloadState* state = GFGetSystemManager()->FontPreload;
    while (g_PendingHead != g_PendingTail) {
        PendingLoadedGlyph& pending = g_PendingLoadedGlyphs[g_PendingHead];
        if (pending.data.Data && !pending.data.Data->IsFinished())
            break;
        StreamingFontCacheBufferData* cache = pending.big ? &g_BigFontCache : &g_SmallFontCache;
        StreamingFontCacheEntry& done = cache->entries[pending.index];
        done.loading = false;
        pending.data.Clear();
        g_FontEntryDirty[pending.index] = true;
        g_FontTextureDirty = true;
        --state->PendingGlyphLoads;
        g_PendingHead = (g_PendingHead + 1) & 0x1ff;
    }
    bool oldAllow = state->AllowStreaming;
    state->AllowStreaming = allowStreaming;
    const wchar_t* p = text;
    while (*p) {
        wchar_t c = *p++;
        if (c == L'\n' || c == L' ' || c == 0xa0)
            continue;
        CStreamingGlyphLookup* lookup = GlyphLookup(c);
        if (!lookup)
            continue;
        unsigned short pos = lookup->cachePos;
        StreamingFontCacheBufferData* cache = lookup->data.BigChar ? &g_BigFontCache : &g_SmallFontCache;
        StreamingFontCacheEntry& entry = cache->entries[pos];
        if (entry.glyphIndex != lookup->indices.GlyphIndex) {
            lookup->cachePos = InsertNewStreamingFontCacheEntry(*cache);
            unsigned short index = lookup->cachePos;
            StreamingFontCacheEntry& fresh = cache->entries[index];
            fresh.glyphIndex = lookup->indices.GlyphIndex;
            fresh.loading = 1;
            unsigned char* memory = GetCacheMemoryOffset(*cache, index);
            memset(memory, 0, cache->entrySize);
            g_FontTextureDirty = true;
            g_FontEntryDirty[index] = true;
            PendingLoadedGlyph& pending = g_PendingLoadedGlyphs[g_PendingTail];
            pending.data = data_bank->StreamGlyph(memory, g_GlyphFileBase + lookup->data.MemOffset * 0x400, cache->entrySize, false);
            pending.big = lookup->data.BigChar;
            pending.index = index;
            if (pending.data.Data && !pending.data.Data->IsFinished()) {
                g_PendingTail = (g_PendingTail + 1) & 0x1ff;
                ++state->PendingGlyphLoads;
                ok = false;
            } else {
                fresh.loading = 0;
            }
        } else {
            if (entry.loading)
                ok = false;
            UpdateUseStreamingFontCacheEntry(*cache, pos);
        }
        {
            unsigned long x = pos % cache->width;
            unsigned long y = pos / cache->width;
            for (StreamingFontCacheBufferData* bigger = cache->nextBigger; bigger; bigger = bigger->nextBigger) {
                y >>= 1;
                x >>= 1;
                unsigned long index = bigger->width * y + x;
                UpdateUseStreamingFontCacheEntry(*bigger, (unsigned short)index);
                bigger->entries[index].glyphIndex = 0;
            }
        }
        {
            unsigned long x = pos % cache->width;
            unsigned long y = pos / cache->width;
            unsigned long scale = 1;
            for (StreamingFontCacheBufferData* smaller = cache->nextSmaller; smaller; smaller = smaller->nextSmaller) {
                x <<= 1;
                y <<= 1;
                scale <<= 1;
                for (short dy = 0; dy < scale; ++dy)
                    for (short dx = 0; dx < scale; ++dx) {
                        unsigned long index = smaller->width * (y + dy) + dx + x;
                        UpdateUseStreamingFontCacheEntry(*smaller, (unsigned short)index);
                        smaller->entries[index].glyphIndex = 0;
                    }
            }
        }
    }
    state->AllowStreaming = oldAllow;
    return ok;
}
