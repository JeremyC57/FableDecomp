// SNamedRecord_00a16960::SNamedRecord_00a16960(const SNamedRecord_00a16960& other) -- copy a 0x20-byte record: the word at +0, the counted block pointer at +4 (its count incremented), five words, and the CCharString at +0x1c (copy constructor @ 0x0099EC30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A16960, length 80 bytes.
// The chunk label "CItem::CItem" is propagated; named by address. Shape: `other` in eax, `this` in esi, `inc dword ptr [ecx]`, `add eax,1Ch` for the string, `mov eax,esi`, `ret 4`.
struct SCountBlock_00a16960 { long m_references; };
struct CCountedRef_00a16960 {                      // 4 bytes: a count block, counted up on copy
    SCountBlock_00a16960* m_pCount;
    CCountedRef_00a16960(const CCountedRef_00a16960& other) : m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};
struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};
struct SNamedRecord_00a16960 {
    long                 m_0;                      // +0x00
    CCountedRef_00a16960 m_ref;                    // +0x04
    long                 m_8;                      // +0x08
    long                 m_c;                      // +0x0c
    long                 m_10;                     // +0x10
    long                 m_14;                     // +0x14
    long                 m_18;                     // +0x18
    CCharString          m_name;                   // +0x1c
    SNamedRecord_00a16960(const SNamedRecord_00a16960& other);
};

SNamedRecord_00a16960::SNamedRecord_00a16960(const SNamedRecord_00a16960& other)
    : m_0(other.m_0), m_ref(other.m_ref), m_8(other.m_8), m_c(other.m_c), m_10(other.m_10), m_14(other.m_14), m_18(other.m_18), m_name(other.m_name)
{
}
