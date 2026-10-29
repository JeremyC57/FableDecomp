// CDataInputStream::ReadC2DVector() -- read X then Y and return the 2D vector by value
// Leaf: ReadC2DVector
// Retail: Fable.exe (TLC, VC7.1) @ 0x00995030, 188 bytes (+ int3 padding), `ret 4` (__fastcall on the stream, hidden result pointer).
// Label: the manifest's "CPersistTraits<CFloatRange>::TransferBinaryIn" is propagated; the landed callers
//   CPersistContext::Transfer<C2DVector> 0x00800B90 and CActionInputControl::TransferBinaryIn 0x00410120 call it as
//   CMemoryDataInputStream::ReadC2DVector (by-value C2DVector through the hidden pointer, `this` = the stream).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, the slow path of the inlined Read).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: two inlined Read calls into two float frame locals, then the result is built from them (X via fld/fstp, Y
//   through a GPR) and its address returned. Speed-optimised (default) TU, the same as the other 0x0099xxxx readers.
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class C2DVector { public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct CDataInputStreamReader : CDataInputStream {
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
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    C2DVector ReadC2DVector();
};

C2DVector CDataInputStreamReader::ReadC2DVector()
{
    float x = ReadFloat();
    float y = ReadFloat();
    return C2DVector(x, y);
}