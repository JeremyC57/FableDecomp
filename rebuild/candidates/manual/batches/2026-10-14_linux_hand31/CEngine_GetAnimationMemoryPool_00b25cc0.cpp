// CEngine::GetAnimationMemoryPool() -- the memory pool held by the engine data's counted pointer at +0x8f0, read through a by-value copy of the counted pointer (add-ref then release folded together)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B25CC0, length 55 bytes.
// Callees: the pointer info's deleter at +4 (called with the object at +8 in ecx when the count reaches zero), operator delete @ 0x00BFE9BC for the info block.
// Shape: the CCountedPointer temporary's copy constructor (`++count`) and destructor (`--count`, destroy at zero) are both inlined and share one null test of
// the info pointer; the compiler keeps both count stores (`mov [esi],ecx ; mov [esi],eax`). The object pointer (edi) is the result. A static member.
extern "C" void __cdecl operator_delete_00bfe9bc(void* block);   // 0x00BFE9BC

struct CMemoryPool;

struct SCountedInfo {                              // CCountedPointer's shared block
    long         m_count;                          // +0x00
    void (__thiscall* m_pfnDestroy)(void*);        // +0x04
    void*        m_pObject;                        // +0x08
};

struct CCountedPointer_Pool {
    CMemoryPool*  m_pObject;                       // +0x00
    SCountedInfo* m_pInfo;                         // +0x04
    CCountedPointer_Pool(const CCountedPointer_Pool& other)
        : m_pObject(other.m_pObject), m_pInfo(other.m_pInfo)
    {
        if (m_pInfo)
            ++m_pInfo->m_count;
    }
    ~CCountedPointer_Pool()
    {
        if (m_pInfo) {
            if (--m_pInfo->m_count == 0) {
                m_pInfo->m_pfnDestroy(m_pInfo->m_pObject);
                operator_delete_00bfe9bc(m_pInfo);
            }
        }
    }
};

struct CEngineData_01436e98 {
    unsigned char        _pad[0x8f0];
    CCountedPointer_Pool m_pool;                   // +0x8f0
};
extern CEngineData_01436e98* g_pEngineData_01436e98;   // 0x01436E98

struct CEngine {
    static CMemoryPool* GetAnimationMemoryPool();
};

CMemoryPool* CEngine::GetAnimationMemoryPool()
{
    CCountedPointer_Pool pool(g_pEngineData_01436e98->m_pool);
    return pool.m_pObject;
}
