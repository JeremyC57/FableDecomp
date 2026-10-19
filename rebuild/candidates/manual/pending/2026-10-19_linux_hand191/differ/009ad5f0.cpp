// CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long index) -- point the stream at the binary data of one definition: inflate (once, cached across calls) the compressed block holding it and open the def's slice of that block
// Grade: DIFFER(231v231) -- register colouring only: retail keeps the block offset in ebx and the def count in ebp (and forms the source pointer in eax), the lane swaps ebx/ebp and uses ecx; four declaration orders tried
// Leaf: DecompressDefinitionData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD5F0, length 231 bytes (+ int3 padding), `ret 8`.
// Callees: CVectorMap<ulong, ulong>::UpperBound(const ulong*) @ 0x009B1FF0 (catalog names another instantiation),
// GFGetTime @ 0x009E1BC0 (`__fastcall`, double; both results discarded), the workspace-size getter @ 0x00A3A7D0
// (returns 0; catalog "CEngineInternalPrimitiveBase::AddChildPrimitive"), CACompressor::CACompressor(long) @ 0x00A3AEA0,
// CCompressorZlib::Init @ 0x00A3AA10 (catalog "CVO_OracleMarker::Init"), CCompressorZlib::InternalDecompress(const void*,
// ulong, void*, ulong) @ 0x00A3AB00, ~CCompressorZlib @ 0x00A3ACC0, CMemoryDataInputStream::OpenMemorySource(void*,
// ulong) @ 0x00994570. Globals: the cache owner @ 0x0138E190, the cached block offset @ 0x0138E194, the inflated size
// @ 0x0138E198, the 0x8000-byte inflate buffer @ 0x0138E19C (it starts with the block's 16-bit offset table).
// Layout: retail CDefinitionManager (engine/CDefinitionManager.h has the PDB offsets): CompressedDefData (CArray<uchar>)
// first at +0x88, CompressedOffsets (CVectorMap<ulong, ulong>: first def index of each block -> block offset) at +0x94.
// CCompressorZlib (vtable 0x01245974, retail 0x58 bytes) over CACompressor.
// Shape: the block is bracketed by the upper-bound entry and the one before it; the compressor is a local in the
// inflate block (inline ctor: CACompressor(workspace size), vtable, Init(); destructor out of line); the def's slice end
// is the next table entry or, for the block's last def, the inflated size.
struct CBaseClass {
    CBaseClass();                                  // 0x0099A2D0
    virtual void v00();
};
struct CACompressor : CBaseClass {                 // vtable 0x0129B424
    void*         Workspace;                       // +0x04
    unsigned long WorkspaceSize;                   // +0x08
    bool          CompressionOn;                   // +0x0c
    unsigned char* OwnWorkspace[3];                // +0x10
    CACompressor(long workspaceSize);              // 0x00A3AEA0
};
struct CCompressorZlib : CACompressor {            // vtable 0x01245974
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                // 0x00A3A7D0
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
    virtual ~CCompressorZlib();                    // 0x00A3ACC0
    void Init();                                   // 0x00A3AA10
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
};

double __fastcall GFGetTime();                     // 0x009E1BC0

struct CMemoryDataInputStream {
    void OpenMemorySource(void* data, unsigned long size);   // 0x00994570
};

struct SOffsetEntry {
    unsigned long FirstIndex;                      // first global def index in the block
    unsigned long Offset;                          // byte offset of the block in the compressed data
};
struct COffsetMap {                                // CVectorMap<unsigned long, unsigned long>
    SOffsetEntry* UpperBound(const unsigned long* key);   // 0x009B1FF0
};

class CDefinitionManager;
extern CDefinitionManager* g_DecompressedDefsOwner;   // 0x0138E190
extern unsigned long g_DecompressedDefsOffset;        // 0x0138E194
extern unsigned long g_DecompressedDefsSize;          // 0x0138E198
extern unsigned char g_DecompressedDefs[0x8000];      // 0x0138E19C

class CDefinitionManager {
public:
    unsigned char _pad0[0x88];
    unsigned char* CompressedDefData;              // +0x88 CArray<unsigned char> first
    unsigned char _pad1[8];
    COffsetMap CompressedOffsets;                  // +0x94
    void DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long index);
};

void CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long index)
{
    SOffsetEntry* next = CompressedOffsets.UpperBound(&index);
    unsigned long firstIndex = next[-1].FirstIndex;
    unsigned long offset = next[-1].Offset;
    unsigned long compressedSize = next->Offset - offset;
    unsigned long count = next->FirstIndex - firstIndex;
    if (g_DecompressedDefsOwner != this || g_DecompressedDefsOffset != offset) {
        GFGetTime();
        CCompressorZlib zlib;
        g_DecompressedDefsSize = zlib.InternalDecompress(CompressedDefData + offset, compressedSize, g_DecompressedDefs, sizeof(g_DecompressedDefs));
        g_DecompressedDefsOwner = this;
        g_DecompressedDefsOffset = offset;
        GFGetTime();
    }
    unsigned long slot = index - firstIndex;
    const unsigned short* table = (const unsigned short*)g_DecompressedDefs;
    unsigned long start = table[slot];
    unsigned long end = slot + 1 < count ? table[slot + 1] : g_DecompressedDefsSize;
    stream->OpenMemorySource(g_DecompressedDefs + start, end - start);
}
