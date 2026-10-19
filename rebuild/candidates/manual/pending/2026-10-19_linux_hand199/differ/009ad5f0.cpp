// CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long defIndex) -- find the compressed block holding a definition, inflate it into the static 32 KB block cache unless that block is already cached, and point the stream at the definition's bytes (bounded by its offset table entry and the next one or the block end)
// Grade: DIFFER(231v231) -- registers: retail holds the block offset in ebx and the next first-index in ebp (ours the reverse) and adds the data base in eax (ours ecx); otherwise the cache test, the inflate and the bounds agree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD5F0, length 240 bytes (ret 8).
// Callees: CVectorMap<unsigned long, unsigned long>::UpperBound 0x009B1FF0 (CompressedOffsets), GFGetTime 0x009E1BC0 (twice,
// result dropped: a timing probe), CCompressorZlib::GetWorkspaceSize 0x00A3A7D0, CACompressor(long) 0x00A3AEA0, CCompressorZlib::
// Init 0x00A3AA10, InternalDecompress 0x00A3AB00 and ~CCompressorZlib 0x00A3ACC0 (vtable 0x01245974), CMemoryDataInputStream::
// OpenMemorySource(const void*, long) 0x00994570.
// Globals: the block cache 0x0138E190 {owner, block offset, decompressed size, 32 KB buffer whose head is the ushort offset table}.
// Layout: CompressedDefData +0x88 (begin pointer), CompressedOffsets +0x94 (sorted {first def index, compressed offset} pairs).
// Shape: the decompressor is a block-scoped CCompressorZlib local (inline constructor: base ctor, own vtable, Init; out-of-line
// destructor); the two GFGetTime calls bracket the inflate; the end bound is a ternary over the next table entry.
double __fastcall GFGetTime();                  // 0x009E1BC0

struct CACompressor {
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);           // 0x00A3AEA0
};
struct CCompressorZlib : CACompressor {         // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();             // 0x00A3A7D0
    void Init();                                // 0x00A3AA10
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
    virtual ~CCompressorZlib();                 // 0x00A3ACC0
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};

struct CMemoryDataInputStream {
    void OpenMemorySource(const void* source, long size);   // 0x00994570
};

struct SOffsetPair { unsigned long first; unsigned long second; };
struct COffsetMap {                             // CVectorMap<unsigned long, unsigned long>
    SOffsetPair* first;
    SOffsetPair* last;
    SOffsetPair* end;
    SOffsetPair* UpperBound(const unsigned long& key);   // 0x009B1FF0
};

struct SDefBlockCache {
    const void*    Owner;                       // 0x0138E190
    unsigned long  BlockOffset;                 // 0x0138E194
    unsigned long  Size;                        // 0x0138E198
    unsigned char  Data[0x8000];                // 0x0138E19C (the head is the ushort offset table)
};
extern SDefBlockCache g_defBlockCache;          // 0x0138E190

class CDefinitionManager {
public:
    unsigned char  _pad000[0x88];
    unsigned char* CompressedDefData;           // +0x88 CArray<unsigned char> begin
    unsigned char  _pad08c[0x94 - 0x8c];
    COffsetMap     CompressedOffsets;           // +0x94
    void DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long defIndex);
};

void CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream* stream, unsigned long defIndex)
{
    SOffsetPair* next = CompressedOffsets.UpperBound(defIndex);
    unsigned long blockFirst = next[-1].first;
    unsigned long blockOffset = next[-1].second;
    unsigned long compressedSize = next->second - blockOffset;
    unsigned long count = next->first - blockFirst;
    if (g_defBlockCache.Owner != this || g_defBlockCache.BlockOffset != blockOffset) {
        GFGetTime();
        CCompressorZlib compressor;
        g_defBlockCache.Size = compressor.InternalDecompress(CompressedDefData + blockOffset, compressedSize,
                                                              g_defBlockCache.Data, 0x8000);
        g_defBlockCache.Owner = this;
        g_defBlockCache.BlockOffset = blockOffset;
        GFGetTime();
    }
    const unsigned short* table = (const unsigned short*)g_defBlockCache.Data;
    unsigned long local = defIndex - blockFirst;
    unsigned long start = table[local];
    unsigned long end = (local + 1 < count) ? table[local + 1] : g_defBlockCache.Size;
    stream->OpenMemorySource(g_defBlockCache.Data + start, end - start);
}
