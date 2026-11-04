// CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long defIndex) const -- find the compressed block holding a definition, inflate it into the static 32 KB block cache unless that block is already cached, and point the stream at the definition's bytes (bounded by its offset table entry and the next one or the block end)
// Leaf: ?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AD5F0, 231 bytes (ret 8). Ego_r.pdb / FableWin.pdb
//   ?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z (protected, const, stream by reference).
// Callees: CVectorMap<unsigned long,unsigned long>::UpperBound const 0x009B1FF0 (CompressedOffsets), GFGetTime 0x009E1BC0 (twice,
//   result dropped: a timing probe), CCompressorZlib::GetRequiredWorkspaceSize 0x00A3A7D0, CACompressor(long) 0x00A3AEA0,
//   CCompressorZlib::Initialise 0x00A3AA10 (vtable 0x01245974), InternalDecompress 0x00A3AB00, ~CCompressorZlib 0x00A3ACC0,
//   CMemoryDataInputStream::OpenMemorySource(const void*, unsigned long) 0x00994570.
// Globals: the block cache 0x0138E190 {owner, block offset, decompressed size, 32 KB buffer whose head is the ushort offset
//   table}; no PDB name found (g_defBlockCache is descriptive).
// Layout: CompressedDefData +0x88 (CArray<unsigned char>), CompressedOffsets +0x94 (CVectorMap<unsigned long,unsigned long>,
//   sorted {first def index, compressed offset} pairs).
// Shape: the compressed-data base is read into a named local inside the block (after the decompressor's construction) and the
//   offset added in the call: that keeps the block offset in ebx, the count in ebp and the sum in eax (a direct
//   `CompressedDefData + blockOffset` argument swapped ebx/ebp and summed in ecx).
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CVectorMap.h"
#include "engine/CCompressorZlib.h"
#include "engine/CMemoryDataInputStream.h"
double __fastcall GFGetTime();                  // 0x009E1BC0

class CDefinitionManager;
struct SDefBlockCache {
    const CDefinitionManager* Owner;                       // 0x0138E190
    unsigned long  BlockOffset;                 // 0x0138E194
    unsigned long  Size;                        // 0x0138E198
    unsigned char  Data[0x8000];                // 0x0138E19C (the head is the ushort offset table)
};
extern SDefBlockCache g_defBlockCache;          // 0x0138E190

class CDefinitionManager {
public:
    unsigned char  _pad000[0x88];
    CArray<unsigned char> CompressedDefData;    // +0x88
    CVectorMap<unsigned long, unsigned long> CompressedOffsets;   // +0x94
protected:
    void DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long defIndex) const;
};

void CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long defIndex) const
{
    CVectorMap<unsigned long, unsigned long>::const_iterator next = CompressedOffsets.UpperBound(defIndex);
    unsigned long blockFirst = next[-1].first;
    unsigned long blockOffset = next[-1].second;
    unsigned long compressedSize = next->second - blockOffset;
    unsigned long count = next->first - blockFirst;
    if (g_defBlockCache.Owner != this || g_defBlockCache.BlockOffset != blockOffset) {
        GFGetTime();
        CCompressorZlib compressor;
        const unsigned char* data = &CompressedDefData[0];
        g_defBlockCache.Size = compressor.InternalDecompress(data + blockOffset, compressedSize, g_defBlockCache.Data, 0x8000);
        g_defBlockCache.Owner = this;
        g_defBlockCache.BlockOffset = blockOffset;
        GFGetTime();
    }
    const unsigned short* table = (const unsigned short*)g_defBlockCache.Data;
    unsigned long local = defIndex - blockFirst;
    unsigned long start = table[local];
    unsigned long end = (local + 1 < count) ? table[local + 1] : g_defBlockCache.Size;
    stream.OpenMemorySource(g_defBlockCache.Data + start, end - start);
}