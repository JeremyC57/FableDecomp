// CCompressorZlib::Compress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize, CCompressionInfo& info) -- write the 13-byte block header ("1PMC", sizes, method 6) and deflate the source behind it; report the sizes through info
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3AC40, 127 bytes (128 with padding). Vtable 0x01245974 slot 5.
// Callees: CCompressorZlib::InternalCompress(const void*, ulong, void*, ulong) 0x00A3AA40 (the deflate
//          worker; returns the compressed byte count).
//
// Shape: the block header is the packed 13-byte record the landed Decompress (0x00A3ABE0) reads: tag
// 0x434D5031, UncompressedSize +4, CompressedSize +8, method byte +0xc (6). An empty source writes a
// header-only block (compressed size 13) and copies the header's info to the caller; otherwise the
// header is filled (CompressedSize provisionally = the source size), the payload is deflated into
// dest + 13 / destSize - 13, CompressedSize becomes the result + 13 and the info is reported.
// The info words are written from the values (VC does not forward the header stores through the
// possibly-aliasing pointer), in retail's order: compressed size first on the header-only path.
// `sourceSize <= 0` on the unsigned size gives retail's `test ; ja` (an `== 0` test emits `jne`).
#include "engine/CCompressorZlib.h"

#pragma pack(push, 1)
class CCompressionInfo {                                        // 9 bytes, packed
public:
    unsigned long UncompressedSize;                             // +0x00
    unsigned long CompressedSize;                               // +0x04
    unsigned char Method;                                       // +0x08
};
struct SCompressedBlock {                                       // the 13-byte header; the deflate payload follows
    unsigned long    Tag;                                       // +0x00
    CCompressionInfo Info;                                      // +0x04
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(SCompressedBlock) == 13);

const unsigned long kCompressedBlockTag = 0x434D5031;           // "1PMC"
const unsigned char kZlibMethod = 6;

struct CCompressorZlib_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void Compress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize, CCompressionInfo& info);   // slot 5
    unsigned char _body[0x54];                                  // +0x04..+0x57 (CACompressor, z_stream, flags)
    unsigned long InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AA40
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == sizeof(CCompressorZlib) - 4);

void CCompressorZlib_Retail::Compress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize, CCompressionInfo& info)
{
    SCompressedBlock* block = (SCompressedBlock*)dest;
    if (sourceSize <= 0) {
        block->Tag = kCompressedBlockTag;
        block->Info.UncompressedSize = 0;
        block->Info.CompressedSize = sizeof(SCompressedBlock);
        block->Info.Method = kZlibMethod;
        info.CompressedSize = sizeof(SCompressedBlock);
        info.UncompressedSize = 0;
        info.Method = kZlibMethod;
        return;
    }
    block->Tag = kCompressedBlockTag;
    block->Info.UncompressedSize = sourceSize;
    block->Info.CompressedSize = sourceSize;
    block->Info.Method = kZlibMethod;
    unsigned long compressedSize = InternalCompress(source, sourceSize, block + 1, destSize - sizeof(SCompressedBlock)) + sizeof(SCompressedBlock);
    block->Info.CompressedSize = compressedSize;
    info.UncompressedSize = sourceSize;
    info.CompressedSize = compressedSize;
    info.Method = kZlibMethod;
}