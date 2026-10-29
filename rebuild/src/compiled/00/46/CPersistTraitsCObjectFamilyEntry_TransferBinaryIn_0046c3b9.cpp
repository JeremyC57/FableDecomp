// Grade: RELOCATION_MATCH (119/119). Fix (2026-10-24 diag): the long traits reader 0x0040FE60 gets its REAL body in the TU
//   (`__declspec(noinline)`, no auto_inline pragma -- the pragma also pushes the in-class traits constructor out of line);
//   VC7.1 then forms the empty traits `this` on the live value-argument slot ([ebp+0Ch]) exactly as retail. Previous grade:
//   DIFFER(126v119) -- ours homed the traits at [ebp+8] / its own slot.
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCObjectFamilyEntry
// CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn(CDataInputStream*, CObjectFamilyEntry*) -- read the Object index through the long traits, then the Probability (stored as a float, truncated to long)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046C3B9, 119 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60 (manifest "CPersistTraits<float>": it moves 4 raw bytes,
//   the member is a long), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path),
//   _ftol @ 0x00BFEA70 (the float -> long conversion).
// Layout: engine/CDataInputStream.h; PDB CObjectFamilyEntry (8 bytes): Object +0, Probability +4 (longs). No engine
//   header for the entry; local model.
// Shape: the long traits is a temporary (4 bytes: its DefaultVal long, left unset by the empty user ctor) that VC7.1
//   homes in the value-argument slot (`lea ecx,[ebp+0Ch]`; a 1-byte empty class lands at [ebp+0Bh]); the float read lands in the dead stream-argument slot. Size-optimised TU:
//   #pragma optimize("s", on) (ebp frame).
#pragma optimize("s", on)
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

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
};

class CObjectFamilyEntry { public:
    long Object;            // +0x00
    long Probability;       // +0x04
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    long DefaultVal;                                                    // never initialised here
    CPersistTraits() {}
    void TransferBinaryIn(CDataInputStream* stream, long* value);      // 0x0040FE60, real body below
};
// The long traits reader 0x0040FE60 with its REAL body in the TU (GOTCHAS 24l recipe: auto_inline(off) + noinline): a 4-byte
// stream Read. With the body visible VC7.1 forms the empty traits `this` on the live value-argument slot as retail does.
// Not landed at 0x0040FE60.

__declspec(noinline) void CPersistTraits<long>::TransferBinaryIn(CDataInputStream* stream, long* value)
{
    ((CDataInputStreamReader*)stream)->Read(value, sizeof(long));
}

template<> struct CPersistTraits<CObjectFamilyEntry> {
    void TransferBinaryIn(CDataInputStream* stream, CObjectFamilyEntry* value);
};

void CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn(CDataInputStream* stream, CObjectFamilyEntry* value)
{
    CPersistTraits<long>().TransferBinaryIn(stream, &value->Object);
    value->Probability = (long)((CDataInputStreamReader*)stream)->ReadFloat();
}