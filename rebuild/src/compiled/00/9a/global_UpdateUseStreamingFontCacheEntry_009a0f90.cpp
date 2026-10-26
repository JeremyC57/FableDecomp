// UpdateUseStreamingFontCacheEntry(StreamingFontCacheBufferData& data, unsigned short index) -- marks a cache entry as used this frame: stamps it and moves it to the newest end of the LRU chain
// Leaf: ?UpdateUseStreamingFontCacheEntry@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0F90, 143 bytes (+ int3 padding), plain `ret`. Ego_r PDB name:
// `?UpdateUseStreamingFontCacheEntry@?A0xfe46a155@@YIXAAUStreamingFontCacheBufferData@1@G@Z` -- anonymous-namespace
// `__fastcall` (data in ecx, the unsigned short index in dx).
// Callees: GFGetSystemManager() @ 0x009A4EC0 (+0x60 CCountedPointer<CDisplayManager>; FrameCounter at +0x25C).
// Layout: as 0x009A0F00 (StreamingFontCacheBufferData: numEntries +8, entries +0x18, newestEntry +0x1C, oldestEntry
// +0x20; StreamingFontCacheEntry: older +0, newer +2, lastUseTime +4). An index >= numEntries means "none".
// Shape: the entry reference is formed before the frame-counter fetch (esi); nothing happens when the entry was
// already stamped this frame. Otherwise it is unlinked (each neighbour's back link, or the chain end when there is no
// neighbour), then linked as the newest: newer = none, older = the old newest, whose newer becomes `index`
// (VC forwards the just-stored `older` word: `movzx edx,ax`).
class CDisplayManager { public:
    unsigned char _pad000[0x25c];
    long          FrameCounter;                 // +0x25c
};
struct CDisplayManagerPointer {                 // CCountedPointer<CDisplayManager> (8 bytes)
    CDisplayManager* Object;                    // +0x00
    long*            Count;                     // +0x04
};
class CSystemManager { public:
    unsigned char          _pad000[0x60];
    CDisplayManagerPointer PDisplayManager;     // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

namespace {

struct StreamingFontCacheEntry {
    unsigned short older;                       // +0x00
    unsigned short newer;                       // +0x02
    long           lastUseTime;                 // +0x04
    unsigned long  glyphIndex : 24;             // +0x08
    unsigned long  loading    : 1;              // +0x0b bit 0
    unsigned long  cacheIndex;                  // +0x0c
};

struct StreamingFontCacheBufferData {
    unsigned long                 width;        // +0x00
    unsigned long                 height;       // +0x04
    unsigned long                 numEntries;   // +0x08
    unsigned long                 entryWidth;   // +0x0c
    unsigned long                 entryHeight;  // +0x10
    unsigned long                 entrySize;    // +0x14
    StreamingFontCacheEntry*      entries;      // +0x18
    unsigned long                 newestEntry;  // +0x1c
    unsigned long                 oldestEntry;  // +0x20
    StreamingFontCacheBufferData* nextSmaller;  // +0x24
    StreamingFontCacheBufferData* nextBigger;   // +0x28
};

void __fastcall UpdateUseStreamingFontCacheEntry(StreamingFontCacheBufferData& data, unsigned short index)
{
    StreamingFontCacheEntry& entry = data.entries[index];
    long frame = GFGetSystemManager()->PDisplayManager.Object->FrameCounter;
    if (entry.lastUseTime != frame) {
        entry.lastUseTime = frame;
        if (entry.newer < data.numEntries)
            data.entries[entry.newer].older = entry.older;
        else
            data.newestEntry = entry.older;
        if (entry.older < data.numEntries)
            data.entries[entry.older].newer = entry.newer;
        else
            data.oldestEntry = entry.newer;
        entry.newer = (unsigned short)data.numEntries;
        entry.older = (unsigned short)data.newestEntry;
        data.entries[entry.older].newer = index;
        data.newestEntry = index;
    }
}

}

void* g_pUpdateUseStreamingFontCacheEntry = (void*)&UpdateUseStreamingFontCacheEntry;