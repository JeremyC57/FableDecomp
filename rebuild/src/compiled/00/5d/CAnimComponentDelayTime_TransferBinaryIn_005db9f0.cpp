// CAnimComponentDelayTime::TransferBinaryIn(CMemoryDataInputStream& stream) -- read Delay (one inlined 4-byte stream read)
// Leaf: ?TransferBinaryIn@CAnimComponentDelayTime@@UAEXAAVCMemoryDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB9F0, 133 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (proven): CAnimComponentDelayTime::TransferBinaryIn; evidence: it is slot 6 of vtable 0x01255698 (RTTI .?AVCAnimComponentDelayTime@@) and stores the stream's 4 bytes at +8 = Ego_r.pdb CAnimComponentDelayTime::Delay (long, the class's only member after the CAnimComponent base); the PDB name sets hold ?TransferBinaryIn@CAnimComponentDelayTime@@UAEXAAVCMemoryDataInputStream@@@Z
// Label: the manifest's "?TransferBinaryIn@CAnimComponentDelayTime@@UAEXAAVCMemoryDataInputStream@@@Z" (family H084: six one-member binary reads; retail has no identical-code folding, so each
//   address is its own function). Two shapes: the member at +8 (the five CAnimComponent* reads) or +4 (CReactionMatch).
// Callee: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the chunk-overflow read).
// Layout: Ego_r.pdb CAnimComponentDelayTime {vptr, base member +4, long Delay +0x8}; local model (no engine header), Review at landing.
// Shape: the landed CPhysicalPrimitiveSphere::TransferBinaryIn 0x00815620 without its name read: the inlined
//   CMemoryDataInputStream::Read(&value, 4) (64-bit position range check, chunk copy or overflow read) fills a local homed in
//   the dead stream-argument slot, then Delay = value in each exit (also when the position check fails).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream {
public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;             // +0x10
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

class CAnimComponentDelayTime {                                              // 12 bytes (Ego_r.pdb)
public:
    void* _base4;                                          // +0x04 (CAnimComponent base member)
    long Delay;                                     // +0x8
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);   // slot 6 of vtable 0x01255698
};

void CAnimComponentDelayTime::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    long value;
    stream.Read(&value, sizeof(value));
    Delay = value;
}