// CRACBuffer::IsRangeAligned() const -- are both the byte offset (+8) and the byte size (+4) exact multiples of the alignment (+0xc)
// Leaf: ?IsRangeAligned@CRACBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C050, length 35 bytes (+ int3 padding).
// Callees: none. Caller: the libpk buffer user at 0x00C3BFA4, right after the sibling IsAligned 0x00C4C030 on the same
// stack-built buffer view (constructor 0x00C4C080 stores base +0, size +4, offset +8, alignment +0xc).
// Module: the libpk PKRACBuffer.cpp TU (the landed 0x00C4C1D0 CheckAlignmentOffset reads the same +0xc alignment);
// no PDB/RTTI, so the class name is descriptive. The catalog label "CBankFile_IsAligned" is propagated.
// Shape: the alignment is loaded once into esi (`div esi` twice, unsigned); the `&&` of two `== 0` tests returns a
// bool through `mov eax,1` / `xor eax,eax` with separate `pop esi ; ret` exits.
struct CRACBuffer {
    unsigned char* m_pBase;        // +0x00
    unsigned long  m_size;         // +0x04
    unsigned long  m_offset;       // +0x08
    unsigned long  m_alignment;    // +0x0c
    bool IsRangeAligned() const;
};

bool CRACBuffer::IsRangeAligned() const
{
    return m_offset % m_alignment == 0 && m_size % m_alignment == 0;
}