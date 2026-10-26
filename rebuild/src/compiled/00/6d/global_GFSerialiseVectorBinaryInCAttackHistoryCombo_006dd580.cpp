// GFSerialiseVectorBinaryIn<CAttackHistoryCombo>(CMemoryDataInputStream& stream, vector<CAttackHistoryCombo>& out) -- read the element count, resize the vector to it, then read every 16-byte combo in place through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DD580, 133 bytes, `__fastcall` (stream in ecx, vector in edx), plain `ret`; a
//   speed TU (no frame pointer: `sub esp,14h` = the count word at +0 and the 16-byte traits object at +4).
// Callers: CHeroExperienceDef's transfer 0x006D91B9 (load arm of its +0xA8 AttackHistoryCombos member) and the two
//   CPersistContext::Transfer<vector<CAttackHistoryCombo> > arms 0x006DD638 / 0x006DD678, whose save arms call
//   GFSerialiseVectorBinaryOut 0x006DC550. The catalog's <CBrainUpdateZone> is propagated (that record is 12 bytes; this
//   stride is 16).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path),
//   vector<CAttackHistoryCombo>::resize(size_type) @ 0x006DD4A0 (out of line here), CPersistTraits<CAttackHistoryCombo>::
//   TransferBinaryIn @ 0x006DC6A0.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC, chunkLength +0x14). PDB CAttackHistoryCombo =
//   { vector<pair<..> > AttackCombo; float Multiplier; }; retail never writes the traits frame, so its DefaultVal is a
//   plain record (the vector as three raw words).
// Shape: the landed GFSerialiseVectorBinaryIn template (0x00569B30 speed form): the inline 4-byte count Read (64-bit
//   position range check, the fast path forwards the chunk word straight into ebx), the one-argument resize called out
//   of line, then `traits.TransferBinaryIn(stream, &out[i])` with a byte offset in edi and a down-counter in ebx.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CAttackHistoryCombo { public:                                // 16 bytes
    void* AttackCombo[3];                                   // +0x00 vector<pair<EAttackHistoryType, EDamageAttribute> > words
    float Multiplier;                                       // +0x0C
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x006DD4A0
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x006DC6A0
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<CAttackHistoryCombo>(CMemoryDataInputStreamReader& stream,
                                                                        std::vector<CAttackHistoryCombo>& out);