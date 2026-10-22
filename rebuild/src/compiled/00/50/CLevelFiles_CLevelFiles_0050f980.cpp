// CLevelFiles::~CLevelFiles() -- release the three intrusive-count smart pointers (+0x14, +0x0c, +0x04, reverse declaration order): drop the reference, destroy the object and free its count block on the last one, then clear both words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050F980, length 129 bytes.
// Callees: the per-type destroy function stored in each count block (`__fastcall`, object in ecx),
//          operator delete @ 0x00BFE9BC.
// Shape: the smart pointer {object, count block} of the landed CCountedPointer Reset family (0x00BB64A0): the
// count is re-read after the decrement, the zero for the two clears is the shared `xor edi,edi`; the member
// destructors are inline and run last member first. Word +0x00 is not touched by the destructor.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CCountedPointer {
    void*        m_pObject;                        // +0
    SCountBlock* m_pCount;                         // +4
    ~CCountedPointer()
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
struct CLevelFiles {
    long            LevelID;                           // +0x00
    CCountedPointer m_first;                       // +0x04
    CCountedPointer m_second;                      // +0x0c
    CCountedPointer m_third;                       // +0x14
    ~CLevelFiles();
};

CLevelFiles::~CLevelFiles()
{
}