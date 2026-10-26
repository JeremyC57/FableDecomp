// CGraphicHeader::Load(CDataInputStream& stream) -- read the 0x1c-byte graphic header record raw, then replace its pixel format with the one decoded by ReadPixelFormat
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD160, length 120 bytes (padded to 128).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0,
//          CDataInputStream::ReadPixelFormat() @ 0x00994670 (both landed).
// Identity: the catalog label "COpinionTransientOffset::TransferBinaryIn" is wrong (that record is 0x18 bytes,
// landed at 0x004793F8). This body reads 0x1c bytes and then fixes +0x0c from ReadPixelFormat: it is the load
// twin of CGraphicHeader::Save @ 0x009FD1E0 (vtable write of 0x1c bytes + WritePixelFormat(&PixelFormat)),
// and its callers are the /Gr thunk 0x009FD210 and CGraphicInfoFrame code at 0x009FF654. PDB CGraphicHeader is
// 0x1c bytes with PixelFormat (CPixelFormat, 4 bytes) at +0x0c.
// Shape: the inlined CDataInputStream::Read(buffer, size) of the landed TransferBinaryIn family (64-bit
// position guard, `rep movsd` fast path, ReadWithSrcChunkOverflow fallback); the by-value CPixelFormat
// result is built in the dead stream-argument slot and copied as one dword.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CPixelFormat { public:                              // 4 bytes, non-POD (returned through the hidden pointer)
    long FormatIndex;
    CPixelFormat() { }
};

struct CDataInputStreamReader : CDataInputStream {   // the engine layout plus the retail member functions
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    CPixelFormat ReadPixelFormat();                                    // 0x00994670
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CGraphicHeader {
    unsigned short Width;                   // +0x00
    unsigned short Height;                  // +0x02
    unsigned short Depth;                   // +0x04
    unsigned short FrameWidth;              // +0x06
    unsigned short FrameHeight;             // +0x08
    unsigned short FrameCount;              // +0x0a
    CPixelFormat   PixelFormat;             // +0x0c
    unsigned char  TransparencyType;        // +0x10
    unsigned char  MipmapLevels;            // +0x11
    unsigned char  IsFlatAnimatedGraphic : 1;   // +0x12
    unsigned long  FrameDataSize;           // +0x14
    unsigned long  CompressedMipmapSizes[1];    // +0x18
    void Load(CDataInputStreamReader& stream);
};

void CGraphicHeader::Load(CDataInputStreamReader& stream)
{
    stream.Read(this, sizeof(CGraphicHeader));
    PixelFormat = stream.ReadPixelFormat();
}