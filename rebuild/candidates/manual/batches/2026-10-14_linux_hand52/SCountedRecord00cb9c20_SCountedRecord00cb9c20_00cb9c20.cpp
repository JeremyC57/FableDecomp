// SCountedRecord_00cb9c20::SCountedRecord_00cb9c20(const SCountedRecord_00cb9c20& other) -- copy a 0x10-byte record: two words, the object pointer at +8 and the count block at +0xc (its reference count incremented when present)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB9C20, length 48 bytes.
// The chunk label "CItem::CItem" is propagated (a CItem-like record without the +0x10 word); named by address. Shape: `this` returned in eax, `other` in ecx, `inc dword ptr [ecx]`, `ret 4`.
struct SCountBlock_00cb9c20 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct SCountedRecord_00cb9c20 {
    long                   m_0;                    // +0x00
    long                   m_4;                    // +0x04
    void*                  m_pObject;              // +0x08
    SCountBlock_00cb9c20*  m_pCount;               // +0x0c
    SCountedRecord_00cb9c20(const SCountedRecord_00cb9c20& other);
};

SCountedRecord_00cb9c20::SCountedRecord_00cb9c20(const SCountedRecord_00cb9c20& other)
    : m_0(other.m_0), m_4(other.m_4), m_pObject(other.m_pObject), m_pCount(other.m_pCount)
{
    if (m_pCount)
        ++m_pCount->m_references;
}
