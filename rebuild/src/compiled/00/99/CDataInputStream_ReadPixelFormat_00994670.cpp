// CDataInputStream::ReadPixelFormat() -- read the 6-byte pixel-format description (zeroed if the position would leave [0, 0x7FFFFFFF]), build a CPixelFormat from it and return it by value
// Leaf: ReadPixelFormat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994670, length 137 bytes (`ret 4`: the hidden result pointer).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) when the current source
// chunk holds fewer than 6 bytes, CPixelFormat::Initialise(const CPixelFormatInit&) @ 0x009E3640.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the inlined Read(buffer, size) of the landed TransferBinaryIn family (0x007AD7F0); the 6-byte description is
// zeroed with an intrinsic memset (dword + word) and filled by the intrinsic memcpy (dword + word through ebx/di); the
// 4-byte CPixelFormat is a named local initialised out of line and then copied into the hidden result (no NRVO).
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

struct CPixelFormatInit {                          // 6-byte packed pixel-format description read from the stream
    unsigned long  m_formatBits;                   // +0 (low nibble of the first byte = format kind)
    unsigned short m_extra;                        // +4
};
struct CPixelFormat {                              // 4 bytes, non-POD (returned through the hidden pointer)
    long m_format;
    CPixelFormat() { }
    void Initialise(const CPixelFormatInit& init); // 0x009E3640
};

struct CDataInputStreamReader : CDataInputStream {   // the engine layout plus the retail member functions
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
    CPixelFormat ReadPixelFormat();
};

CPixelFormat CDataInputStreamReader::ReadPixelFormat()
{
    CPixelFormatInit init;
    memset(&init, 0, 6);
    Read(&init, 6);
    CPixelFormat format;
    format.Initialise(init);
    return format;
}