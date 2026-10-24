// Grade: DIFFER(ours v retail) -- the long-traits temporary: retail forms `this` at [ebp+0Ch] (the value-argument slot); a 4-byte traits lands at [ebp+8], a 1-byte empty one at [ebp+0Bh], a named local at [ebp-4]/[ebp-1]
// CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn(CDataInputStream*, CObjectFamilyEntry*) -- read the Object index through the long traits, then the Probability (stored as a float, truncated to long)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046C3B9, 119 bytes, `ret 8` (__thiscall, the empty traits `this` is unused).
// Callees: CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60 (manifest "CPersistTraits<float>": it moves 4 raw bytes,
//   the member is a long), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path),
//   _ftol @ 0x00BFEA70 (the float -> long conversion).
// Layout: engine/CDataInputStream.h; PDB CObjectFamilyEntry (8 bytes): Object +0, Probability +4 (longs). No engine
//   header for the entry; local model.
// Shape: the long traits is a temporary (4 bytes: its DefaultVal long, left unset by the empty user ctor) that VC7.1
//   homes in the value-argument slot (`lea ecx,[ebp+0Ch]`; a 1-byte empty class lands at [ebp+0Bh]); the float read lands in the dead stream-argument slot. Size-optimised TU:
//   #pragma optimize("s", on) (ebp frame).
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

struct CObjectFamilyEntry {
    long Object;            // +0x00
    long Probability;       // +0x04
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    long DefaultVal;                                                    // never initialised here
    CPersistTraits() {}
    void TransferBinaryIn(CDataInputStream* stream, long* value);      // 0x0040FE60
};
template<> struct CPersistTraits<CObjectFamilyEntry> {
    void TransferBinaryIn(CDataInputStream* stream, CObjectFamilyEntry* value);
};

void CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn(CDataInputStream* stream, CObjectFamilyEntry* value)
{
    CPersistTraits<long>().TransferBinaryIn(stream, &value->Object);
    value->Probability = (long)((CDataInputStreamReader*)stream)->ReadFloat();
}
