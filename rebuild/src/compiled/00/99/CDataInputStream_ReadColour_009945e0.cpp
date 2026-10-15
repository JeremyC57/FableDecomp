// CDataInputStream::ReadColour() -- read a 4-byte CRGBColour from the stream (zero colour if the position would leave [0, 0x7FFFFFFF]) and return it by value
// Leaf: ReadColour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009945E0, length 138 bytes (`ret 4`: the hidden result pointer).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) when the current source
// chunk holds fewer than 4 bytes.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the inlined Read(buffer, size) of the landed TransferBinaryIn family (0x007AD7F0: 64-bit position range check
// through an inline predicate, chunk memcpy, `+=` bookkeeping); the colour local is zeroed byte by byte by its default
// constructor (one zero register, ebx, which then serves the range check) and returned through CRGBColour's user copy
// constructor, which copies R, G, B, A in that order (stores +2, +1, +0, +3; the D3DCOLOR layout is B, G, R, A).
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
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
    CRGBColour ReadColour();
};

CRGBColour CDataInputStreamReader::ReadColour()
{
    CRGBColour colour;
    Read(&colour, 4);
    return colour;
}