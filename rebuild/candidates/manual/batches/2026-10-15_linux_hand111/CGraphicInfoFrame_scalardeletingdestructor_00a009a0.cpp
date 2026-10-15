// CGraphicInfoFrame::`scalar deleting destructor'(unsigned int) -- releases the shared block at +8 (count at +4, virtual slot 1 at zero, member nulled), runs CBaseClass::~CBaseClass on +0 and frees on bit 0
// Leaf: ??_GCGraphicInfoFrame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A009A0, length 54 bytes (vtable 0x0129C878 slot 0).
// Callees: CBaseClass::~CBaseClass @ 0x0099A2E0 (stores the CBaseClass vtable 0x01231710), operator delete @ 0x00BFE9BC.
// The catalog label "CThingHolySite::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CGraphicInfoFrame vtable. Shape: no vtable store of its own, so the frame is
// modelled (wave 24g/24m rule) as a NON-polymorphic record whose FIRST MEMBER is the polymorphic
// CBaseClass object -- VC7.1 then emits the non-virtual `??_G` for the `delete` helper with the
// implicit destructor fully inlined (a derived class with a generated destructor calls `??1`
// out of line). The shared block's release must live on the block (`Release()` keeps the block
// in ecx for the slot-1 call; a `--m_pBlock->m_references` test in the pointer re-reads the member).
struct SSharedBlock {
    virtual void v00();
    virtual void Destroy();                // slot 1: called when the count reaches zero
    long m_references;                     // +4
    void Release() { if (--m_references == 0) Destroy(); }
};
struct CSharedPointer {
    SSharedBlock* m_pBlock;
    ~CSharedPointer()
    {
        if (m_pBlock) {
            m_pBlock->Release();
            m_pBlock = 0;
        }
    }
};
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                         // 0x0099A2E0
};
struct CGraphicInfoFrame {
    CBaseClass     m_base;                 // +0x00 (vptr)
    long           m_frame;                // +0x04
    CSharedPointer m_pInfo;                // +0x08
};

void DestroyGraphicInfoFrame(CGraphicInfoFrame* frame)
{
    delete frame;
}
