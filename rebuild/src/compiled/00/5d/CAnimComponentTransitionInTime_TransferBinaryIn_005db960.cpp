// CAnimComponentTransitionInTime::TransferBinaryIn(CMemoryDataInputStream& stream) -- read TransitionInTime (one inlined 4-byte stream read)
// Leaf: ?TransferBinaryIn@CAnimComponentTransitionInTime@@UAEXAAVCMemoryDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB960, 133 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (proven): CAnimComponentTransitionInTime::TransferBinaryIn; evidence: it is slot 6 of vtable 0x01255670 (RTTI .?AVCAnimComponentTransitionInTime@@) and stores the stream's 4 bytes at +8 = Ego_r.pdb CAnimComponentTransitionInTime::TransitionInTime (long, the class's only member after the CAnimComponent base); the PDB name sets hold ?TransferBinaryIn@CAnimComponentTransitionInTime@@UAEXAAVCMemoryDataInputStream@@@Z
// Label: the manifest's "?TransferBinaryIn@CAnimComponentTransitionInTime@@UAEXAAVCMemoryDataInputStream@@@Z" (family H084: six one-member binary reads; retail has no identical-code folding, so each
//   address is its own function). Two shapes: the member at +8 (the five CAnimComponent* reads) or +4 (CReactionMatch).
// Callee: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the chunk-overflow read).
// Layout: Ego_r.pdb CAnimComponentTransitionInTime {vptr, base member +4, long TransitionInTime +0x8}; local model (no engine header), Review at landing.
// Shape: the landed CPhysicalPrimitiveSphere::TransferBinaryIn 0x00815620 without its name read: the inlined
//   CMemoryDataInputStream::Read(&value, 4) (64-bit position range check, chunk copy or overflow read) fills a local homed in
//   the dead stream-argument slot, then TransitionInTime = value in each exit (also when the position check fails).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CMemoryDataInputStream {
public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;             // +0x10
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

class CAnimComponentTransitionInTime {                                              // 12 bytes (Ego_r.pdb)
public:
    void* _base4;                                          // +0x04 (CAnimComponent base member)
    long TransitionInTime;                                     // +0x8
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);   // slot 6 of vtable 0x01255670
};

void CAnimComponentTransitionInTime::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    long value;
    stream.Read(&value, sizeof(value));
    TransitionInTime = value;
}