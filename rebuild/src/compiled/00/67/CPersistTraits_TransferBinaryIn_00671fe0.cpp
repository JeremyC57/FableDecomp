// CPersistTraits<EControlledMovementType>::TransferBinaryIn(CMemoryDataInputStream&, EControlledMovementType&) -- binary persist read of one 4-byte enum: inlined CDataInputStream::Read, ReadWithSrcChunkOverflow @ 0x00993CA0 when the chunk runs out
// Leaf: ?TransferBinaryIn@?$CPersistTraits@W4EControlledMovementType@@@@QAEXAAVCMemoryDataInputStream@@AAW4EControlledMovementType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00671FE0, oracle length 129 bytes, `ret 8` (traits `this` unused).
// Identity: the landed caller 0x00671F90 calls this address as this symbol (rebuild/oracles/reloc_bindings.tsv), and
//   Ego_r.pdb / FableWin.pdb name this exact symbol.
// Body: the generic template body (the shape of engine/CPersistTraits.h FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY): a local filled by the
//   inline CDataInputStream::Read and copied to the output.
// Model: the landed row's own CMemoryDataInputStream method view (kept as landed: its inline range predicate gives retail's
//   exit-tail order in this no-frame TU; the shared header's Read orders the tails the other way), with the generic template body (engine/CPersistTraits.h
//   FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY) written as this T's member specialisation over that stream view.
#include "fable_compiler.h"
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

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
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
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
enum EControlledMovementType { };   // the Ego_r enumeration (only its 4-byte size matters here)

template <> void CPersistTraits<EControlledMovementType>::TransferBinaryIn(CMemoryDataInputStream& stream, EControlledMovementType& value)
{
    EControlledMovementType number;
    stream.Read(&number, sizeof(number));
    value = number;
}
