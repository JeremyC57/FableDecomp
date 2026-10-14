// SCountedRecord_00a7faf0::operator=(const SCountedRecord_00a7faf0& other) -- copy the word at +0 and, when the count block (+8) differs, release the old one (destroy and delete at zero) and take the other's object (+4) and block (counted up)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7FAF0, length 78 bytes.
// The chunk label "CItem::operator=" is propagated (two byte-identical bodies); named by address. Shape: the other's object in ebx and block in edi, `this` in esi, `mov eax,esi`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_00a7faf0 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct SCountedRecord_00a7faf0 {
    long               m_0;                        // +0x00
    void*              m_pObject;                  // +0x04
    SCountBlock_00a7faf0*  m_pCount;                   // +0x08
    SCountedRecord_00a7faf0& operator=(const SCountedRecord_00a7faf0& other);
};

SCountedRecord_00a7faf0& SCountedRecord_00a7faf0::operator=(const SCountedRecord_00a7faf0& other)
{
    m_0 = other.m_0;
    void* object = other.m_pObject;
    SCountBlock_00a7faf0* count = other.m_pCount;
    if (m_pCount != count) {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = object;
        m_pCount = count;
        if (count)
            ++count->m_references;
    }
    return *this;
}
