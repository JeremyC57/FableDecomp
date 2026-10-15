// CVtable_012b1a54::CVtable_012b1a54(void*, long, long) -- three-argument constructor of the no-RTTI class whose two-argument constructor is the landed 0x00C08720: second and third arguments at +4/+8, own vtable, a zero at +0xc, first argument at +0x10
// Leaf: ??0CVtable_012b1a54
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08750, length 39 bytes (`ret 0Ch`). Callers: 0x00A0B7C2,
// 0x00A0B87E, 0x00A0B92E, 0x00AAC10A.
// Label: the catalog name "CTreeElement::CTreeElement" is propagated (PDB CTreeElement is a
// non-polymorphic quad-tree node); the vtable stored, 0x012B1A54, has no RTTI locator and is the one the
// landed CVtable_012b1a54 constructor 0x00C08720 stores, so the class is named the same way.
// Shape: plain initialiser list in declaration order; VC schedules the +4 store before the vtable store
// and the zero as an immediate (`mov dword ptr [eax+0Ch],0`).
struct CVtable_012b1a54 {                          // vtable 0x012B1A54
    virtual void v00();
    long  m_index;                                 // +0x04
    long  m_frame;                                 // +0x08
    void* m_pSecond;                               // +0x0c
    void* m_pFirst;                                // +0x10
    CVtable_012b1a54(void* first, long index, long frame);
};

CVtable_012b1a54::CVtable_012b1a54(void* first, long index, long frame)
    : m_index(index)
    , m_frame(frame)
    , m_pSecond(0)
    , m_pFirst(first)
{
}