// InsertNewStreamingFontCacheEntry(StreamingFontCacheBufferData& data) -- recycles the oldest cache entry: stamps it with this frame, makes it the newest in the LRU chain and returns its index
// Leaf: ?InsertNewStreamingFontCacheEntry@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A1020, 105 bytes (+ int3 padding), plain `ret`. Ego_r PDB name:
// `?InsertNewStreamingFontCacheEntry@?A0xfe46a155@@YIGAAUStreamingFontCacheBufferData@1@@Z` -- anonymous-namespace
// `__fastcall` (data in ecx) returning the unsigned short index in ax.
// Callees: GFGetSystemManager() @ 0x009A4EC0 (+0x60 CCountedPointer<CDisplayManager>; FrameCounter at +0x25C).
// Layout: as 0x009A0F00 (StreamingFontCacheBufferData: numEntries +8, entries +0x18, newestEntry +0x1C, oldestEntry
// +0x20; StreamingFontCacheEntry: older +0, newer +2, lastUseTime +4). An index >= numEntries means "none".
// Shape: the oldest index is read as a word (bx, kept for the return) and the entry reference formed before the
// frame-counter fetch; the next-oldest entry becomes the chain end (its older = none, oldestEntry = it), then the
// recycled entry is linked as the newest (newer = none, older = the old newest, whose newer becomes `index`; VC
// forwards the just-stored `older` word).
struct CDisplayManager {
    unsigned char _pad000[0x25c];
    long          FrameCounter;                 // +0x25c
};
struct CDisplayManagerPointer {                 // CCountedPointer<CDisplayManager> (8 bytes)
    CDisplayManager* Object;                    // +0x00
    long*            Count;                     // +0x04
};
struct CSystemManager {
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

unsigned short __fastcall InsertNewStreamingFontCacheEntry(StreamingFontCacheBufferData& data)
{
    unsigned short index = (unsigned short)data.oldestEntry;
    StreamingFontCacheEntry& entry = data.entries[index];
    entry.lastUseTime = GFGetSystemManager()->PDisplayManager.Object->FrameCounter;
    data.entries[entry.newer].older = (unsigned short)data.numEntries;
    data.oldestEntry = entry.newer;
    entry.newer = (unsigned short)data.numEntries;
    entry.older = (unsigned short)data.newestEntry;
    data.entries[entry.older].newer = index;
    data.newestEntry = index;
    return index;
}

}

void* g_pInsertNewStreamingFontCacheEntry = (void*)&InsertNewStreamingFontCacheEntry;