// InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData& data) -- when the cache is empty, link every entry into one LRU chain (entry 0 newest, the last entry oldest) and age them all
// Leaf: ?InitializeStreamingFontCacheEntries@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0F00, 137 bytes (+ int3 padding), plain `ret`. Ego_r PDB name:
// `?InitializeStreamingFontCacheEntries@?A0xfe46a155@@YIXAAUStreamingFontCacheBufferData@1@@Z` -- an
// anonymous-namespace `__fastcall` helper of the streaming-font module taking the buffer by reference (ecx).
// Callees: GFGetSystemManager() @ 0x009A4EC0 (the system record; +0x60 is the CCountedPointer<CDisplayManager>
// PDisplayManager, CDisplayManager::FrameCounter is at +0x25C as in the landed SwapScreens 0x009BEEB0).
// Layout (Ego_r PDB, both anonymous-namespace structs): StreamingFontCacheBufferData {width, height, numEntries +8,
// entryWidth, entryHeight, entrySize, entries +0x18, newestEntry +0x1C, oldestEntry +0x20, nextSmaller, nextBigger};
// StreamingFontCacheEntry (16 bytes) {older +0 (ushort), newer +2 (ushort), lastUseTime +4, glyphIndex:24 +8,
// loading:1 (bit 24 of +8), cacheIndex +0xC}.
// Shape: only an empty cache (newest == oldest) is initialised. Every entry gets lastUseTime = FrameCounter -
// 0x40000000 (so a fresh entry always looks old) and glyphIndex = 0 (the `and [x+8],0FF000000h` bitfield store;
// `loading` is kept). Entry 0: older 1, newer numEntries (the "none" index), written through a pointer local
// (`entries` loaded once; `data.entries[0].x` re-reads it per store). The frame counter is read into a local and
// the 0x40000000 is subtracted at each use (`lea edx,[ecx-40000000h]` after the newer store; a precomputed `time`
// local loads straight into edx and reorders the stores). The counted display-manager pointer is read through
// `.Object` (an inline `operator->` leaves a dead `add eax,60h`). The loop index is an `unsigned short`
// (`movzx eax,ax`; VC strength-reduces `i - 1` into ecx and re-reads `entries` per pass because the word stores
// may alias it). oldestEntry = numEntries - 1.
struct CDisplayManager {
    unsigned char _pad000[0x25c];
    long          FrameCounter;                 // +0x25c
};
struct CDisplayManagerPointer {                 // CCountedPointer<CDisplayManager> (8 bytes)
    CDisplayManager* Object;                    // +0x00
    long*            Count;                     // +0x04
    CDisplayManager* operator->() const { return Object; }
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

void __fastcall InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData& data)
{
    if (data.newestEntry == data.oldestEntry) {
        long frame = GFGetSystemManager()->PDisplayManager.Object->FrameCounter;
        StreamingFontCacheEntry* first = data.entries;
        first->older = 1;
        first->newer = (unsigned short)data.numEntries;
        first->lastUseTime = frame - 0x40000000;
        first->glyphIndex = 0;
        data.newestEntry = 0;
        for (unsigned short i = 1; i < data.numEntries; ++i) {
            StreamingFontCacheEntry& entry = data.entries[i];
            entry.older = i + 1;
            entry.newer = i - 1;
            entry.lastUseTime = frame - 0x40000000;
            entry.glyphIndex = 0;
        }
        data.oldestEntry = data.numEntries - 1;
    }
}

}

void* g_pInitializeStreamingFontCacheEntries = (void*)&InitializeStreamingFontCacheEntries;