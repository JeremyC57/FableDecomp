// CAnimComponentCombatComboChain::TransferBinaryIn(CMemoryDataInputStream* stream) -- read ComboStage then ComboID
// Leaf: TransferBinaryIn
// (4 bytes each) from the memory stream.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DBB60, length 205 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (twice, when the chunk runs out).
// Shape: two inline CPersistTraits<long>::TransferBinaryIn reads of the landed 0x007AD7F0 model (64-bit position
// range check, chunk memcpy fast path, overflow call); each value is homed in the dead stream-argument slot; the
// second read's fast path forwards straight into the member and its exits duplicate the return tails.
// Layout: rebuild/include/engine/CAnimComponentCombatComboChain.h (ComboStage +0x08, ComboID +0x0c); the method is
// declared on a local view of the header's layout.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

#include "engine/CAnimComponentCombatComboChain.h"

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

__forceinline void TransferLongIn(CMemoryDataInputStream* stream, long* out)   // CPersistTraits<long>::TransferBinaryIn
{
    long value;
    stream->Read(&value, sizeof(value));
    *out = value;
}

struct CAnimComponentCombatComboChain_Transfer : CAnimComponentCombatComboChain {
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CAnimComponentCombatComboChain_Transfer::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    TransferLongIn(stream, &ComboStage);
    TransferLongIn(stream, &ComboID);
}