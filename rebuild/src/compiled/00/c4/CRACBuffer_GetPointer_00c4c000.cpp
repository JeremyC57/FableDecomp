// CRACBuffer::GetPointer(unsigned long offset) -- the address `offset` bytes into the buffer view (base +0)
// Leaf: ?GetPointer@CRACBuffer@@QBEPAEK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4C000, 9 bytes, thiscall `ret 4`.
// Identity: called by the bank loader 0x00C3BCB0 (0x00C3C095 / 0x00C3C0FC, both `GetPointer(0)`) on the same stack buffer
//   view as GetAlignedSize 0x00C4BFF0 (the landed CRACBuffer::IsRangeAligned 0x00C4C050: base +0). The catalog label
//   "CBasicString<char>::GetChar" is propagated. Descriptive (libpk, no PDB/RTTI).
// Shape: `mov eax,[ecx] ; add eax,[esp+4]`.
struct CRACBuffer {
    unsigned char* m_pBase;        // +0x00
    unsigned long  m_size;         // +0x04
    unsigned long  m_offset;       // +0x08
    unsigned long  m_alignment;    // +0x0c
    unsigned char* GetPointer(unsigned long offset) const;
};

unsigned char* CRACBuffer::GetPointer(unsigned long offset) const
{
    return m_pBase + offset;
}