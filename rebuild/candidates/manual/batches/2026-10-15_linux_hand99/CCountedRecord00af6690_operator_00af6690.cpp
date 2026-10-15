// CCountedRecord_00af6690::operator=(const CCountedRecord_00af6690& other) -- assign the counted pointer at +4, the byte at +0xc and the word at +0x10; return *this
// Leaf: ??4CCountedRecord_00af6690
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF6690, length 86 bytes; two callers: 0x00AF6CA2 inside a pair assignment
// (0x00AF6C90: key word copied, then this on the pair's +4 value) and 0x00AF7841 in the skeletal-morph map code. The chunk
// label "CTCRandomAppearanceMorph::Copy" is a propagated BSim name (the body is the same counted-pointer assignment as
// 0x00728400 at other offsets, then `mov eax,esi`). The record type is unidentified, so it is named by address; +0 is not
// copied (a vtable pointer).
// Callees: the counted block's destroy function (`mov ecx,[eax+8] ; call [eax+4]`), operator delete @ 0x00BFE9BC.
// Shape: the inlined CCountedPointer::operator= (source object then block read into ebp / edi before the block compare, the
// destination block released with the member re-read after the decrement, then adopt + add-ref), the byte and word copies in
// member order, and `return *this` (eax = esi).
struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CCountedObject;
struct CCountedPointer {
    CCountedObject* m_pObject;                               // +0
    SCountBlock*   m_pCount;                                // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCountedObject* object = other.m_pObject;
        SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

struct CCountedRecord_00af6690 {
    virtual void v00();                                     // +0x00 (not assigned)
    CCountedPointer m_pObject;                              // +0x04
    unsigned char   m_flag;                                 // +0x0c
    long            m_value;                                // +0x10
    CCountedRecord_00af6690& operator=(const CCountedRecord_00af6690& other);
};

CCountedRecord_00af6690& CCountedRecord_00af6690::operator=(const CCountedRecord_00af6690& other)
{
    m_pObject = other.m_pObject;
    m_flag = other.m_flag;
    m_value = other.m_value;
    return *this;
}
