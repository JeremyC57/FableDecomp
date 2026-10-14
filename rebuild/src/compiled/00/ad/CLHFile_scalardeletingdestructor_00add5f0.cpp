// CLHFile::`scalar deleting destructor'(unsigned int) -- this class's vtable, the counted file data at +4 released (destroy and delete its block at zero, both words cleared), the LHA::CFile base vtable, the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADD5F0, length 84 bytes.
// Callees: the count block's destroy (slot +4, the object at +8 in ecx), operator delete @ 0x00BFE9BC (the block, then the object). Vtables 0x0129F094 = CLHFile,
// 0x0129F06C = LHA::CFile (rtti-vtables.tsv). Shape: the inlined CCountedPointer destructor with its two clears scheduled after the flag test. The chunk label
// "CEventHitBy" is propagated. Both the pointer release and the virtual destructor are `__forceinline` (see 0x00811C30).
// Leaf: ??_GCLHFile@@UAEPAXI@Z
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_00add5f0 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};

struct CCountedPointer_00add5f0 {                     // 8 bytes: the object and its count block
    void*                m_pObject;                // +0x00
    SCountBlock_00add5f0*    m_pCount;                // +0x04
    __forceinline ~CCountedPointer_00add5f0()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

namespace LHA {
struct CFile {                                     // vtable 0x0129F06C
    virtual ~CFile() { }
};
}

struct CLHFile : LHA::CFile {                      // vtable 0x0129F094
    CCountedPointer_00add5f0 m_data;               // +0x04
    __forceinline virtual ~CLHFile() { }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_00add5f0()
{
    return new CLHFile;
}