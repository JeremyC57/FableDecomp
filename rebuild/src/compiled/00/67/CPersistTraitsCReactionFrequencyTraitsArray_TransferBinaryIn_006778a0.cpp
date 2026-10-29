// CPersistTraits<CReactionFrequencyTraitsArray>::TransferBinaryIn(CDataInputStream*, CReactionFrequencyTraitsArray*) -- read each of the 2 x 79 frequency-trait slots: its type, a new traits object of that type (none for type 0) that reads its own data
// Retail: Fable.exe (TLC, VC7.1) @ 0x006778A0, 199 bytes (+ int3 padding), `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path),
//   CReactionFrequencyTraits::CreateFromType(EReactionFrequencyType) @ 0x006FF930 (__fastcall, type in ecx; news the
//   type's traits object), CReactionFrequencyTraits virtual slot 3 (+0xC, TransferBinaryIn(CDataInputStream*)).
// Layout: engine/CDataInputStream.h; PDB CReactionFrequencyTraitsArray = { CReactionFrequencyTraits* LowHigh[2][79] }.
// Shape: two count-down loops over one walking slot pointer (counters in a frame word and the dead value-argument
//   slot). The type comes from an inline by-value ReadLong() (its local homed in the dead stream-argument slot): VC7.1
//   caches it in ebx, loaded from that slot before the loops for the range-check-failure path. `traits = 0` precedes
//   the read (`xor edi,edi` before the range check); a plain `Read(&type)` into a named local reloads type from the
//   frame instead. Speed-optimised (default) TU.
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

enum EReactionFrequencyType { REACTION_FREQUENCY_NONE = 0 };

class CReactionFrequencyTraits { public:
    virtual void v00();
    virtual void v01();
    virtual long GetType() const;                                       // +0x08
    virtual void TransferBinaryIn(CDataInputStream* stream);            // +0x0C
};
CReactionFrequencyTraits* __fastcall CreateFromType(EReactionFrequencyType type);   // 0x006FF930

class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];       // +0x000
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CReactionFrequencyTraitsArray> {
    void TransferBinaryIn(CDataInputStream* stream, CReactionFrequencyTraitsArray* value);
};

void CPersistTraits<CReactionFrequencyTraitsArray>::TransferBinaryIn(CDataInputStream* stream, CReactionFrequencyTraitsArray* value)
{
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 79; ++j) {
            CReactionFrequencyTraits* traits = 0;
            long type = ((CDataInputStreamReader*)stream)->ReadLong();
            if (type != 0) {
                traits = CreateFromType((EReactionFrequencyType)type);
                if (traits != 0)
                    traits->TransferBinaryIn(stream);
            }
            value->LowHigh[i][j] = traits;
        }
    }
}