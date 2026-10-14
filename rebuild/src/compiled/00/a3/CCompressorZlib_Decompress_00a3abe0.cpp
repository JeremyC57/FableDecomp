// CCompressorZlib::Decompress(const SCompressedBlock*, unsigned long, void*, unsigned long, SCompressedBlockInfo*) -- inflate the payload that follows the 13-byte block header (unless the block is header-only) and report the header's info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3ABE0, length 87 bytes (+ int3 padding).
// Callees: CCompressorZlib::InternalDecompress(const void*, ulong, void*, ulong) @ 0x00A3AB00 (payload
// = block + 13, payload size = blockSize - 13). Slot 7 (+0x1c) of vtable 0x01245974 = CCompressorZlib
// (rtti-vtables.tsv); the 4-argument front end @ 0x00A3A950 calls it through that slot.
// The block header is 13 packed bytes: a 4-byte tag, UncompressedSize +4, CompressedSize +8 and a
// flag byte +0xc; the info written back (+4, +8, +0xc in the retail store order: compressed size,
// uncompressed size, flag) is the same 9-byte triple. The `blockSize == 13` test is folded into the
// payload-size subtraction (`sub eax,0Dh ; jne`, the difference reused as the InternalDecompress size):
// the header-only case reports the info and returns early (its own copy of the three stores, no esi
// save), the other inflates then reports.
#include "engine/CCompressorZlib.h"

#pragma pack(push, 1)
struct SCompressedBlockInfo {                                   // 9 bytes, packed
    unsigned long UncompressedSize;                             // +0x00
    unsigned long CompressedSize;                               // +0x04
    bool          Flag;                                         // +0x08
};
struct SCompressedBlock {                                       // the 13-byte header; the deflate payload follows
    unsigned long        Tag;                                   // +0x00
    SCompressedBlockInfo Info;                                  // +0x04
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(SCompressedBlock) == 13);

struct CCompressorZlib_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void Decompress(const SCompressedBlock* block, unsigned long blockSize, void* dest, unsigned long destSize, SCompressedBlockInfo* info);   // slot 7
    unsigned char _body[0x54];                                  // +0x04..+0x57 (CACompressor, z_stream, flags)
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
};

static inline void ReportInfo(SCompressedBlockInfo* info, const SCompressedBlock* block)
{
    info->CompressedSize = block->Info.CompressedSize;
    info->UncompressedSize = block->Info.UncompressedSize;
    info->Flag = block->Info.Flag;
}

void CCompressorZlib_Retail::Decompress(const SCompressedBlock* block, unsigned long blockSize, void* dest, unsigned long destSize, SCompressedBlockInfo* info)
{
    blockSize -= sizeof(SCompressedBlock);                      // the payload size, updated in the parameter's own slot
    if (blockSize == 0) {                                       // header-only block: nothing to inflate
        ReportInfo(info, block);
        return;
    }
    InternalDecompress(block + 1, blockSize, dest, destSize);
    ReportInfo(info, block);
}