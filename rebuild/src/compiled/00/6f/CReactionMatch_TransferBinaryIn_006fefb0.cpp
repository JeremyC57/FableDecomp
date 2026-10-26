// CReactionMatch::TransferBinaryIn(CMemoryDataInputStream& stream) -- read ReactionType (one inlined 4-byte stream read)
// Leaf: ?TransferBinaryIn@CReactionMatch@@UAEXAAVCMemoryDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEFB0, 133 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (proven): CReactionMatch::TransferBinaryIn; evidence: it is not in any vtable: CReactionMatch_SecondaryAxisMRShifted 0x006FF040, CReactionMatch_SecondaryAxisCentredMRShifted 0x006FF2C0, CReactionMatch_AsymmetricPyramid 0x006FF540 and CReactionMatch_Sphere 0x00701990 call it first from their own TransferBinaryIn (the qualified base call `CReactionMatch::TransferBinaryIn(stream)`), and it stores the stream's 4 bytes at +4 = Ego_r.pdb CReactionMatch::ReactionType (EOpinionReactionType, the class's only member after the vptr); the PDB name sets hold ?TransferBinaryIn@CReactionMatch@@UAEXAAVCMemoryDataInputStream@@@Z
// Label: the manifest's "ReadFromFile" (family H084: six one-member binary reads; retail has no identical-code folding, so each
//   address is its own function). Two shapes: the member at +8 (the five CAnimComponent* reads) or +4 (CReactionMatch).
// Callee: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the chunk-overflow read).
// Layout: Ego_r.pdb CReactionMatch {vptr, EOpinionReactionType ReactionType +0x4}; local model (no engine header), Review at landing.
// Shape: the landed CPhysicalPrimitiveSphere::TransferBinaryIn 0x00815620 without its name read: the inlined
//   CMemoryDataInputStream::Read(&value, 4) (64-bit position range check, chunk copy or overflow read) fills a local homed in
//   the dead stream-argument slot, then ReactionType = value in each exit (also when the position check fails).
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
enum EOpinionReactionType { OPINION_REACTION_TYPE_0 };
class CReactionMatch {                                              // 8 bytes (Ego_r.pdb)
public:
    EOpinionReactionType ReactionType;                                     // +0x4
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);   // virtual, called by the derived classes' TransferBinaryIn
};

void CReactionMatch::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    EOpinionReactionType value;
    stream.Read(&value, sizeof(value));
    ReactionType = value;
}