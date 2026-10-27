// `anonymous namespace'::$E_g_StreamingFontCacheBufferData2() -- dynamic initializer of the <64,64> streaming-font cache buffer record: 16 x 8 cells of 64 x 64 pixels (128 entries), its entry array, the <32,32> buffer as the next smaller one, no bigger buffer
// Grade: DIFFER(71v93) -- VC7.1 RTM folds both buffer records into .data and emits no `_$E1`/`_$E2` initializer (the 71 bytes graded are the out-of-line <32,32> constructor); retail kept them as dynamic stores. Not folded by: an empty user ctor on the entry or base type, external linkage, the ctor defined after the objects (probes fc1/fc2).
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223F60, 93 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (probable): FableWin.pdb orders `anonymous namespace'::dynamic initializer for 'g_StreamingFontCacheBufferData'` and
//   `...'g_StreamingFontCacheBufferData2'` right before `NStreamingFontBankGlobals::...'g_PendingLoadedGlyphs'`, whose retail
//   initializer is 0x01223FC0; retail 0x01223F00 / 0x01223F60 are the two initializers before it and build two
//   StreamingFontCacheBufferData records linked to each other (nextBigger / nextSmaller), read together by
//   CStreamingFontBank::Preload 0x009A2000 and CRenderManagerCore::AttachTextureToStage 0x009A0CF0. Ego_r.pdb has the two
//   instantiations ??0?$StreamingFontCacheBufferData_static@$0CA@$0CA@@?A0x...@@QAE@PAUStreamingFontCacheBufferData@1@0@Z
//   (<32,32>) and the <64,64> one; the entry counts (512, 128) match the record sizes 0x2030 / 0x830.
// Label: the manifest's "sub_01223f60" is a gap-scan placeholder.
// Callee: none. Data: 0x013C5A98 = <32,32> buffer (0x2030 bytes, entries +0x2c), 0x013C7AC8 = <64,64> buffer (0x830 bytes),
//   the 512-record g_PendingLoadedGlyphs right after it at 0x013C82F8.
// Layout: Ego_r.pdb StreamingFontCacheBufferData (0x2c): width, height, numEntries, entryWidth, entryHeight, entrySize,
//   entries (StreamingFontCacheEntry*), newestEntry, oldestEntry, nextSmaller, nextBigger; the _static<W,H> templates add
//   StreamingFontCacheEntry entries[] (16 bytes each; 512 for <32,32>, 128 for <64,64>) at +0x2c.
// Shape: no `.data` folding because the constructor is user-written; the cache texture is 1024 x 512 (width = 1024 / W,
//   height = 512 / H); VC7.1 groups the stores of equal constants through eax. Both objects are defined here, in source
//   order, so the <32,32> initializer is `_$E1` and the <64,64> one `_$E2`; the second is declared ahead because the first
//   links to it.
namespace {
struct StreamingFontCacheEntry {                          // 16 bytes
    unsigned short older;                                 // +0x00
    unsigned short newer;                                 // +0x02
    long           lastUseTime;                           // +0x04
    unsigned long  glyphIndex : 24;                       // +0x08
    unsigned long  loading : 1;
    unsigned long  cacheIndex;                            // +0x0c
};

struct StreamingFontCacheBufferData {                     // 0x2c bytes
    unsigned long                 width;                  // +0x00
    unsigned long                 height;                 // +0x04
    unsigned long                 numEntries;             // +0x08
    unsigned long                 entryWidth;             // +0x0c
    unsigned long                 entryHeight;            // +0x10
    unsigned long                 entrySize;              // +0x14
    StreamingFontCacheEntry*      entries;                // +0x18
    unsigned long                 newestEntry;            // +0x1c
    unsigned long                 oldestEntry;            // +0x20
    StreamingFontCacheBufferData* nextSmaller;            // +0x24
    StreamingFontCacheBufferData* nextBigger;             // +0x28
};

const unsigned long FONT_CACHE_TEXTURE_WIDTH = 1024;
const unsigned long FONT_CACHE_TEXTURE_HEIGHT = 512;

template <unsigned long W, unsigned long H>
struct StreamingFontCacheBufferData_static : public StreamingFontCacheBufferData {
    StreamingFontCacheEntry entries[(FONT_CACHE_TEXTURE_WIDTH / W) * (FONT_CACHE_TEXTURE_HEIGHT / H)];   // +0x2c
    StreamingFontCacheBufferData_static(StreamingFontCacheBufferData* smaller, StreamingFontCacheBufferData* bigger)
    {
        width = FONT_CACHE_TEXTURE_WIDTH / W;
        height = FONT_CACHE_TEXTURE_HEIGHT / H;
        numEntries = (FONT_CACHE_TEXTURE_WIDTH / W) * (FONT_CACHE_TEXTURE_HEIGHT / H);
        entryWidth = W;
        entryHeight = H;
        entrySize = W * H;
        StreamingFontCacheBufferData::entries = entries;
        newestEntry = 0;
        oldestEntry = 0;
        nextSmaller = smaller;
        nextBigger = bigger;
    }
};

extern StreamingFontCacheBufferData_static<64, 64> g_StreamingFontCacheBufferData2;
StreamingFontCacheBufferData_static<32, 32> g_StreamingFontCacheBufferData(0, &g_StreamingFontCacheBufferData2);    // 0x013C5A98
StreamingFontCacheBufferData_static<64, 64> g_StreamingFontCacheBufferData2(&g_StreamingFontCacheBufferData, 0);    // 0x013C7AC8
}

// Stand-in for the TU's real readers (CStreamingFontBank, CRenderManagerCore): without a reference VC7.1 drops the
// internal-linkage buffers together with their initializers.
StreamingFontCacheBufferData* GetStreamingFontCacheBuffers()
{
    return &g_StreamingFontCacheBufferData;
}
