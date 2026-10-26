// CEngineStaticMapFileBlockManager::SetBankFileEntry(CCountedPointer<CBankFileAsyncData> entry) -- assign the by-value counted pointer to the member at +0 (release the old block at zero, copy both words, add a reference), then the argument's own release as it dies
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE03A0, length 88 bytes.
// Callees: the count block's destroy (slot +4, the object at +8 in ecx) and operator delete @ 0x00BFE9BC, twice (the member's old block, the argument's block).
// Shape: the inlined operator= compares the count blocks (`cmp eax,edi ; je`), releases the old one, copies the two words and `inc [edi]`; the argument's inlined
// destructor then drops its count (its field clears are dead and dropped); `ret 8`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct SCountBlock_00be03a0 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};

struct CCountedPointer_00be03a0 {                  // 8 bytes: the object and its count block
    void*                 m_pObject;               // +0x00
    SCountBlock_00be03a0* m_pCount;                // +0x04
    void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
    CCountedPointer_00be03a0& operator=(const CCountedPointer_00be03a0& other)
    {
        if (m_pCount != other.m_pCount) {
            Release();
            m_pObject = other.m_pObject;
            m_pCount = other.m_pCount;
            if (m_pCount)
                ++m_pCount->m_references;
        }
        return *this;
    }
    ~CCountedPointer_00be03a0()
    {
        Release();
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CEngineStaticMapFileBlockManager { public:
    CCountedPointer_00be03a0 m_bankFileEntry;      // +0x00
    void SetBankFileEntry(CCountedPointer_00be03a0 entry);
};

void CEngineStaticMapFileBlockManager::SetBankFileEntry(CCountedPointer_00be03a0 entry)
{
    m_bankFileEntry = entry;
}