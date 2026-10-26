// CCombatAbilityData::TransferBinaryIn(CMemoryDataInputStream& stream, CCombatAbilityData& defaults) -- binary def load: PercentageChance, SecondsDuration and SecondsToWaitBeforeRepeat as three 4-byte persist reads, then the Anim def string reads itself
// Leaf: ?TransferBinaryIn@CCombatAbilityData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044FADC, length 284 bytes (`ret 8`); size-optimised frame-pointer def TU.
// Pragma: #pragma optimize("y", off) + #pragma optimize("s", on) (the landed CPersistTraits read rows' TU settings).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x3, the chunk-overflow path), CDefString::
//          TransferBinaryIn(CMemoryDataInputStream*) @ 0x0044FC00 (manifest "CPersistTraits<float>::TransferBinaryIn",
//          propagated; it is called with ecx = &Anim).
// Layout: engine/CCombatAbilityData.h (PDB, 0x18): vptr, PercentageChance +4, SecondsDuration +8,
//          SecondsToWaitBeforeRepeat +0xc, Anim (CDefString TablePos) +0x10, InterfaceType +0x14 -- the same offsets,
//          modelled locally because the generated header flattens the vptr and has no member functions.
// Shape: each read is the inlined CMemoryDataInputStream::Read (64-bit position check, memcpy from the chunk, else
//          ReadWithSrcChunkOverflow) into a local of the member's type inside a __forceinline template helper: the
//          local is homed in the dead `stream` argument slot ([ebp+8], the stream itself stays in ebx), `this` is
//          parked in [ebp-4] across the movsd, and the float members copy through eax. The second argument is unused.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CMemoryDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
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

template <class T> __forceinline void TransferIn(CMemoryDataInputStream& stream, T& out)
{
    T value;
    stream.Read(&value, sizeof(value));
    out = value;
}

class CDefString { public:
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

class CCombatAbilityData {
public:
    virtual void v00();
    long       PercentageChance;           // +0x04
    float      SecondsDuration;            // +0x08
    float      SecondsToWaitBeforeRepeat;  // +0x0c
    CDefString Anim;                       // +0x10
    long       InterfaceType;              // +0x14
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream, CCombatAbilityData& defaults);
};

void CCombatAbilityData::TransferBinaryIn(CMemoryDataInputStream& stream, CCombatAbilityData&)
{
    TransferIn(stream, PercentageChance);
    TransferIn(stream, SecondsDuration);
    TransferIn(stream, SecondsToWaitBeforeRepeat);
    Anim.TransferBinaryIn(&stream);
}