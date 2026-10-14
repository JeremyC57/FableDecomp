// CLHFile::~CLHFile() -- this class's vtable (0x0129F094), the counted pointer at +0x4 released (the count block's references decremented, the object destroyed through the block's function and the block freed at zero; object and block then cleared), then the CFile base's inline destructor stores its vtable (0x0129F06C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADD1F0, length 80 bytes.
// Leaf: ??1CLHFile@@QAE@XZ
// Shape: a non-virtual destructor in a polymorphic class, `this` in esi, the counted pointer's destructor inlined, `ret`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CFileObject_00add1f0;
struct SCountBlock_00add1f0 {                           // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CFileObject_00add1f0*);       // +0x04
    CFileObject_00add1f0* m_pObject;                              // +0x08
};
struct CCountedPointer_00add1f0 {                       // {object, count block}
    CFileObject_00add1f0*           m_pObject;                    // +0x00
    SCountBlock_00add1f0* m_pCount;                     // +0x04
    __forceinline ~CCountedPointer_00add1f0()
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
namespace LHA {
struct CFile {                                  // vtable 0x0129F06C
    virtual void v00();
    ~CFile() { }                                // inline: the vtable store
};
}
struct CLHFile : LHA::CFile {                        // vtable 0x0129F094
    CCountedPointer_00add1f0 m_pointer;             // +0x4
    ~CLHFile();
};

CLHFile::~CLHFile()
{
}
