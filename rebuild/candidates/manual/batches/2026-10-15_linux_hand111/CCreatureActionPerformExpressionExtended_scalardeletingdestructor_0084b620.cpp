// CCreatureAction_PerformExpressionExtended::`scalar deleting destructor'(unsigned int) -- intelligent pointer at +0xac destroyed, shared block at +0xa8 released and nulled, the action base destructor, flag-tested delete
// Leaf: ??_GCCreatureAction_PerformExpressionExtended
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B620, length 74 bytes (vtable 0x012744FC slot 0).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, the action base destructor @ 0x00693EF0
// (stores vtable 0x01259D24; manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated), operator delete @ 0x00BFE9BC.
// The catalog label "CThingHolySite::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CCreatureAction_PerformExpressionExtended vtable 0x012744FC.
// Shape: no vtable store of its own = the wave 24g/24m model: a non-polymorphic record whose FIRST
// member is the 0xa8-byte action base, then the shared pointer (+0xa8) and the intelligent pointer
// (+0xac), destroyed in reverse order with the implicit destructor inlined into the `??_G` of a
// `delete` helper. The shared block's release lives on the block (keeps it in ecx).
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
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    ~CBaseIntelligentPointer();            // 0x00A01C10
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                // 0x00693EF0
};
struct CCreatureAction_PerformExpressionExtended {
    CCreatureActionBase     m_base;        // +0x00
    CSharedPointer          m_pShared;     // +0xa8
    CBaseIntelligentPointer m_target;      // +0xac
};

void DestroyPerformExpressionExtended(CCreatureAction_PerformExpressionExtended* action)
{
    delete action;
}
