// CAlignedRange_00c4c030::IsAligned() const -- is (base - offset % stride) a multiple of 4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C030, oracle length 20 bytes.
// Callees: none.
//
// Retail bytes:  mov eax,[ecx+8] ; xor edx,edx ; div dword ptr [ecx+0Ch] ; mov eax,[ecx] ; sub eax,edx ; and al,3 ; neg al ; sbb eax,eax ; inc eax ; ret
// Unsigned remainder (`xor edx,edx ; div`), subtraction from the first dword, and a low-two-bit
// test whose result is materialised as 0/1 in eax (`neg al ; sbb ; inc` from `== 0` on the masked
// byte).  The manifest's "CBankFile::VerifyChecksum" label is propagated; the record is named by
// address and its members by role.
struct CAlignedRange_00c4c030 {
    unsigned long m_base;        // +0x00
    unsigned long m_unused;      // +0x04
    unsigned long m_offset;      // +0x08
    unsigned long m_stride;      // +0x0c
    bool IsAligned() const;      // 0x00C4C030
};

bool CAlignedRange_00c4c030::IsAligned() const
{
    return ((m_base - m_offset % m_stride) & 3) == 0;
}
