// CPersistTraits<CFloatRange>::TransferBinaryIn(CDataInputStream*, CFloatRange*) -- read LowValue then HighValue
// Retail: Fable.exe (TLC, VC7.1) @ 0x00458F89, 185 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, the slow path of the inlined Read).
// Layout: engine/CDataInputStream.h; PDB CFloatRange (8 bytes): HighValue +0, LowValue +4. No engine header for the
//   range; local model with the PDB member names.
// Shape: two inlined CDataInputStream::Read calls into one 4-byte local homed in the dead stream-argument slot.
//   Size-optimised TU: #pragma optimize("s", on) (ebp frame).
#pragma optimize("s", on)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
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
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

class CFloatRange { public:
    float HighValue;        // +0x00
    float LowValue;         // +0x04
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CFloatRange> {
    void TransferBinaryIn(CDataInputStream* stream, CFloatRange* value);
};

void CPersistTraits<CFloatRange>::TransferBinaryIn(CDataInputStream* stream, CFloatRange* value)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    value->LowValue = in->ReadFloat();
    value->HighValue = in->ReadFloat();
}