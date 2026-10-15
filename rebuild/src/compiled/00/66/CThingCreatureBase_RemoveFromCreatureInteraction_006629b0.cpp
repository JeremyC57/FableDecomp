// CThingCreatureBase::RemoveFromCreatureInteraction() -- take this creature out of its current creature interaction and drop the counted reference to the interaction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006629B0, length 76 bytes (+ int3 padding).
// Callees: CCreatureInteraction::RemoveParticipant(CThingCreatureBase&) @ 0x008631B0 (Ego_r.pdb name; the manifest
// label "CTCInventoryAbilities::AddLearnedExpression" is propagated), the count block's destroy function (`mov ecx,[blk+8] ;
// call [blk+4]`), operator delete @ 0x00BFE9BC.
// Layout (retail, local model): the CCountedPointer<CCreatureInteraction> at +0x12c {object, count block +0x130}
// (engine/CThingCreatureBase.h has PCreatureInteraction at the PDB offset +0x150).
// Shape: the interaction is used without a null test; the counted pointer is cleared by an inline release that
// re-reads the block member after the decrement (`dec [eax] ; mov eax,[esi+130h] ; cmp [eax],edi`), then the block and
// the object are zeroed in that order (the inline `Reset(0)` shape of the out-of-line 0x00669440).
struct CThingCreatureBase;

struct CCreatureInteraction {
    void RemoveParticipant(CThingCreatureBase& creature);   // 0x008631B0
};

struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};

struct CInteractionPointer {                    // CCountedPointer<CCreatureInteraction>
    CCreatureInteraction* m_pObject;            // +0
    SCountBlock*          m_pCount;             // +4
    void Clear()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
    CCreatureInteraction* operator->() const { return m_pObject; }
};

struct CThingCreatureBase {
    unsigned char       _pad00[0x12c];
    CInteractionPointer m_creatureInteraction;  // +0x12c
    void RemoveFromCreatureInteraction();
};

void CThingCreatureBase::RemoveFromCreatureInteraction()
{
    m_creatureInteraction->RemoveParticipant(*this);
    m_creatureInteraction.Clear();
}