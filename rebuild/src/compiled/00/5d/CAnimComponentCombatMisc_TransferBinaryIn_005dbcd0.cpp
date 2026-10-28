// CAnimComponentCombatMisc::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the MeleeFlourish and MeleeKnockdown flags (one byte each, stored as `!= 0`)
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DBCD0, length 199 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (twice, when the chunk runs out).
// Shape: two inline CPersistTraits<bool>::TransferBinaryIn reads (the landed 0x007AD7F0 Read model for one byte, the byte
// homed in the dead stream-argument slot, `test ; setne` into the member); the second read's exits duplicate the tails.
// Layout: rebuild/include/engine/CAnimComponentCombatMisc.h (MeleeFlourish +0x08, MeleeKnockdown +0x09).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

#include "engine/CAnimComponentCombatMisc.h"

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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

__forceinline void TransferBoolIn(CMemoryDataInputStream* stream, bool* out)   // CPersistTraits<bool>::TransferBinaryIn
{
    unsigned char value;
    stream->Read(&value, sizeof(value));
    *out = value != 0;
}

struct CAnimComponentCombatMisc_Transfer : CAnimComponentCombatMisc {
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CAnimComponentCombatMisc_Transfer::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    TransferBoolIn(stream, &MeleeFlourish);
    TransferBoolIn(stream, &MeleeKnockdown);
}