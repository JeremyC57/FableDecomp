// COpinionPersonalityTraits::TransferBinaryIn(CMemoryDataInputStream& stream) -- binary def load of the nine opinion personality scaling floats, each a 4-byte persist read
// Leaf: ?TransferBinaryIn@COpinionPersonalityTraits
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E8ED, length 784 bytes (`ret 4`); size-optimised frame-pointer def TU.
// Pragma: #pragma optimize("y", off) + #pragma optimize("s", on) (the landed CCombatAbilityData::TransferBinaryIn 0x0044FADC
//          TU settings).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x9, the chunk-overflow path).
// Layout: Ego_r.pdb COpinionPersonalityTraits (0x24, no vtable): BaseOffset +0, HeroStatScaling +4,
//          VillageOpinionScaling +8, TimeScalingRunIn +0xc, TimeScalingRunOut +0x10, PeakScalingIfPos +0x14,
//          PeakScalingIfNeg +0x18, PersistScalingIfPos +0x1c, PersistScalingIfNeg +0x20. No engine header.
// Shape: the landed 0x0044FADC model: each read is the inlined CMemoryDataInputStream::Read into a local of the member's
//          type inside a __forceinline template helper (homed in the dead `stream` argument slot [ebp+8]; `this` parked
//          in [ebp-4]).
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

class COpinionPersonalityTraits {
public:
    float BaseOffset;                      // +0x00
    float HeroStatScaling;                 // +0x04
    float VillageOpinionScaling;           // +0x08
    float TimeScalingRunIn;                // +0x0c
    float TimeScalingRunOut;               // +0x10
    float PeakScalingIfPos;                // +0x14
    float PeakScalingIfNeg;                // +0x18
    float PersistScalingIfPos;             // +0x1c
    float PersistScalingIfNeg;             // +0x20
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void COpinionPersonalityTraits::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    TransferIn(stream, BaseOffset);
    TransferIn(stream, HeroStatScaling);
    TransferIn(stream, VillageOpinionScaling);
    TransferIn(stream, TimeScalingRunIn);
    TransferIn(stream, TimeScalingRunOut);
    TransferIn(stream, PeakScalingIfPos);
    TransferIn(stream, PeakScalingIfNeg);
    TransferIn(stream, PersistScalingIfPos);
    TransferIn(stream, PersistScalingIfNeg);
}