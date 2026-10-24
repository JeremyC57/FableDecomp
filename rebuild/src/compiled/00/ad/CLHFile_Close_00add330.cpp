// CLHFile::Close() -- release the counted file data (+4) and delete this file object through its virtual destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADD330, length 67 bytes (padded to 80).
// Leaf: Close
// Label correction: the catalog "_Dest_val<..., std::pair<unsigned long, CCountedPointer<CFaction>>>" is propagated.
// The only reference is CLHFile vtable 0x0129F094 slot 3 (rtti-vtables, at 0x0129F0A0); slot 4 is the landed deleting
// destructor 0x00ADD5F0, slots 0 / 2 the landed GetSize 0x00ADD160 / GetMode 0x00ADD190. The slot name is inferred from
// the body (drop the data, then `delete this`), not from a symbol.
// Callees: the count block's destroy (+4, the object at +8 in ecx), operator delete @ 0x00BFE9BC (count block), the
//          virtual destructor (slot 4, `push 1 ; call [edx+10h]`).
// Layout: as the landed CLHFile deleting destructor: LHA::CFile base (vtable 0x0129F06C), m_data {object +4, block +8}.
// Shape: the inline counted-pointer reset nulls the block word before the object word (the destructor order is the
// reverse); `delete this` keeps its null test on `this`, scheduled among the two clears.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct CCountedPointerFileData {                   // 8 bytes: the object and its count block
    void*        m_pObject;                        // +0x00
    SCountBlock* m_pCount;                         // +0x04
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};
namespace LHA {
struct CFile {                                     // vtable 0x0129F06C
    virtual bool GetSize(unsigned long* size);     // slot 0
    virtual void v01();                            // slot 1
    virtual long GetMode();                        // slot 2
    virtual void Close();                          // slot 3
    virtual ~CFile();                              // slot 4
};
}
struct CLHFile : LHA::CFile {                      // vtable 0x0129F094
    CCountedPointerFileData m_data;                // +0x04
    virtual void Close();
};

void CLHFile::Close()
{
    m_data.Reset();
    delete this;
}