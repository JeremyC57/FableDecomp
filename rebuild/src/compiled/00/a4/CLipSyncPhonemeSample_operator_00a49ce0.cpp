// CLipSyncPhonemeSample::operator=(const CLipSyncPhonemeSample& other) -- the name (+0, CCharString::operator= @ 0x0099EFB0) and the dword at +4 copied, then the counted pointer at +8/+0xc assigned: when the count blocks differ the old block is released (references decremented, object destroyed and block freed at zero), the new object and block stored and the new block's references incremented
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A49CE0, length 96 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0, operator delete @ 0x00BFE9BC. Shape: the source in edi (its object loaded into edi between the block compare and its branch: both source fields are read into locals before the `if`), the new block in ebx, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CPhonemeData_00a49ce0;
struct SCountBlock_00a49ce0 {                           // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CPhonemeData_00a49ce0*);       // +0x04
    CPhonemeData_00a49ce0* m_pObject;                              // +0x08
};
struct CCountedPointer_00a49ce0 {                       // {object, count block}
    CPhonemeData_00a49ce0*           m_pObject;                    // +0x00
    SCountBlock_00a49ce0* m_pCount;                     // +0x04
    __forceinline ~CCountedPointer_00a49ce0()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
class CCharString { public:
    void* m_pStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CLipSyncPhonemeSample { public:
    CCharString               m_name;              // +0x00
    long                      m_4;                 // +0x04
    CCountedPointer_00a49ce0  m_data;              // +0x08
    CLipSyncPhonemeSample& operator=(const CLipSyncPhonemeSample& other);
};

CLipSyncPhonemeSample& CLipSyncPhonemeSample::operator=(const CLipSyncPhonemeSample& other)
{
    m_name = other.m_name;
    m_4 = other.m_4;
    SCountBlock_00a49ce0* block = other.m_data.m_pCount;
    CPhonemeData_00a49ce0* object = other.m_data.m_pObject;
    if (m_data.m_pCount != block) {
        if (m_data.m_pCount) {
            --m_data.m_pCount->m_references;
            if (m_data.m_pCount->m_references == 0) {
                m_data.m_pCount->m_pfnDestroy(m_data.m_pCount->m_pObject);
                delete m_data.m_pCount;
            }
        }
        m_data.m_pObject = object;
        m_data.m_pCount = block;
        if (block)
            ++block->m_references;
    }
    return *this;
}