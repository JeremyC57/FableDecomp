// CCompressorZlib::DecompressBlock(const SCompressedBlock*, unsigned long, void*, void* dest) -- copy the block header's info, then the virtual Decompress with the sizes taken from that header
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3A950, length 58 bytes.
// Callees: vtable slot 7 (+0x1c) = CCompressorZlib::Decompress(const SCompressedBlock*, ulong, void*,
// ulong, SCompressedBlockInfo*) @ 0x00A3ABE0 (vtable 0x01245974). Four stack arguments (`ret 10h`); the
// second and third are not used by this body (the block's own CompressedSize / UncompressedSize
// stand in for them), the last is the destination buffer. The 9-byte packed info (UncompressedSize,
// CompressedSize, flag) is read into three locals first (the loads precede every store, so the
// loaded registers double as the call's size arguments: a copy through the address-taken local
// would reload them), the 12-byte frame local is built from them and passed by address as the out
// parameter; its stores are interleaved with the argument pushes.
#include "engine/CCompressorZlib.h"

#pragma pack(push, 1)
struct SCompressedBlockInfo {                                   // 9 bytes, packed
    unsigned long UncompressedSize;                             // +0x00
    unsigned long CompressedSize;                               // +0x04
    bool          Flag;                                         // +0x08
    SCompressedBlockInfo(unsigned long uncompressedSize, unsigned long compressedSize, bool flag)
        : UncompressedSize(uncompressedSize), CompressedSize(compressedSize), Flag(flag) { }
};
struct SCompressedBlock {                                       // the 13-byte header; the deflate payload follows
    unsigned long        Tag;                                   // +0x00
    SCompressedBlockInfo Info;                                  // +0x04
};
#pragma pack(pop)

struct CCompressorZlib_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void Decompress(const SCompressedBlock* block, unsigned long blockSize, void* dest, unsigned long destSize, SCompressedBlockInfo* info);   // slot 7 @ 0x00A3ABE0
    unsigned char _body[0x54];                                  // +0x04..+0x57
    void DecompressBlock(const SCompressedBlock* block, unsigned long unusedSize, void* unusedSource, void* dest);
};

void CCompressorZlib_Retail::DecompressBlock(const SCompressedBlock* block, unsigned long unusedSize, void* unusedSource, void* dest)
{
    unsigned long compressedSize = block->Info.CompressedSize;
    unsigned long uncompressedSize = block->Info.UncompressedSize;
    bool flag = block->Info.Flag;
    SCompressedBlockInfo info(uncompressedSize, compressedSize, flag);
    Decompress(block, compressedSize, dest, uncompressedSize, &info);
}