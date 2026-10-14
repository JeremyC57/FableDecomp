// CVtable_012b1a54::CVtable_012b1a54(void* first, void* second) -- constructor: two -1 members at +4/+8, own vtable, second argument at +0xc, first at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08720, length 48 bytes.
// Callees: none (leaf constructor, ret 8).
// The manifest calls this CActionParamSLONG::CActionParamSLONG, a propagated label: the vtable
// stored, 0x012B1A54, has no RTTI locator (quest-script module), hence the CVtable_<va> name.
// Shape: `or ecx,-1` feeds both -1 stores before the vtable store (the store sinks below the
// constant stores that share one register, as in the landed CActionDropGenericBox ctor), then the
// two arguments are stored: second at +0xc, first at +0x10 (declaration order).
struct CVtable_012b1a54 {                                        // vtable 0x012B1A54
    virtual void v00();
    long  m_index;                                               // +0x04 = -1
    long  m_frame;                                               // +0x08 = -1
    void* m_pSecond;                                             // +0x0c
    void* m_pFirst;                                              // +0x10
    CVtable_012b1a54(void* first, void* second);
};

CVtable_012b1a54::CVtable_012b1a54(void* first, void* second)
    : m_index(-1)
    , m_frame(-1)
    , m_pSecond(second)
    , m_pFirst(first)
{
}
