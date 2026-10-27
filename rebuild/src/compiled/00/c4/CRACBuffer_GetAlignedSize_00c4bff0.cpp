// CRACBuffer::GetAlignedSize(unsigned long count) -- the byte size of `count` alignment units: the alignment (+0xc) times the count
// Leaf: ?GetAlignedSize@CRACBuffer@@QBEKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4BFF0, 11 bytes, thiscall `ret 4`.
// Identity: called twice by the bank loader 0x00C3BCB0 (0x00C3C14D / 0x00C3C15B) on the stack buffer view it also probes
//   with IsAligned 0x00C4BFA0 / 0x00C4C030 and the getters 0x00C4C010 / 0x00C4C020: the libpk PKRACBuffer.cpp view of the
//   landed CRACBuffer::IsRangeAligned 0x00C4C050 (alignment +0xc). The catalog label "CWorld::DrawGetEnvironment" is
//   propagated. Descriptive (libpk, no PDB/RTTI).
// Shape: `mov eax,[ecx+0Ch] ; imul eax,[esp+4]` -- the member is the left operand.
struct CRACBuffer {
    unsigned char* m_pBase;        // +0x00
    unsigned long  m_size;         // +0x04
    unsigned long  m_offset;       // +0x08
    unsigned long  m_alignment;    // +0x0c
    unsigned long GetAlignedSize(unsigned long count) const;
};

unsigned long CRACBuffer::GetAlignedSize(unsigned long count) const
{
    return m_alignment * count;
}