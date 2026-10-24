// CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn(CDataInputStream*, CWeaponTrailGraphicSet*) -- read the Attack and Knockdown trail-graphic indices
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9021, 185 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
//   0x00469D47 and 0x004E9021 are the same instantiation emitted in two TUs (identical bytes but for the call).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, the slow path of the inlined Read).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14); PDB
//   CWeaponTrailGraphicSet (8 bytes): Attack +0, Knockdown +4 (longs). No engine header for the set; local model.
// Shape: two inlined CDataInputStream::Read calls (64-bit position range check, chunk fast path with `movsd`, overflow
//   reader) into one 4-byte local homed in the dead stream-argument slot, each followed by its member store (the
//   value pointer is re-read from its slot). Size-optimised TU: #pragma optimize("s", on) (ebp frame).
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

struct CWeaponTrailGraphicSet {
    long Attack;            // +0x00
    long Knockdown;         // +0x04
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CWeaponTrailGraphicSet> {
    void TransferBinaryIn(CDataInputStream* stream, CWeaponTrailGraphicSet* value);
};

void CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn(CDataInputStream* stream, CWeaponTrailGraphicSet* value)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    value->Attack = in->ReadLong();
    value->Knockdown = in->ReadLong();
}