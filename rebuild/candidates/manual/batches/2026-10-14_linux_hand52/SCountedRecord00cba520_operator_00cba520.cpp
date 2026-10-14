// SCountedRecord_00cba520::operator=(const SCountedRecord_00cba520& other) -- copy the two words at +0/+4 and, when the count block (+0xc) differs, release the old one (destroy and delete at zero) and take the other's object (+8) and block (counted up)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA520, length 96 bytes.
// Callees: the block's destroy (+4, the object at +8 in ecx), operator delete @ 0x00BFE9BC. The chunk label "CItem::operator=" is propagated (the landed 0x00A7FAF0 is the 12-byte form); named by address.
// Shape: the other's object in ebx and block in edi read before the compare, `this` in esi, `mov eax,esi`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_00cba520 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct SCountedRecord_00cba520 {
    long                   m_0;                    // +0x00
    long                   m_4;                    // +0x04
    void*                  m_pObject;              // +0x08
    SCountBlock_00cba520*  m_pCount;               // +0x0c
    SCountedRecord_00cba520& operator=(const SCountedRecord_00cba520& other);
};

SCountedRecord_00cba520& SCountedRecord_00cba520::operator=(const SCountedRecord_00cba520& other)
{
    m_0 = other.m_0;
    m_4 = other.m_4;
    void* object = other.m_pObject;
    SCountBlock_00cba520* count = other.m_pCount;
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
