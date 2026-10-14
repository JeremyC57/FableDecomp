// CCombatActionBase::SetupCombatAnimation() -- let the creature pick an entry from CombatAnimations and play it from -1 to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856AC0, length 52 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingCreatureBase::
// GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>&) @ 0x00666B50 (returns the
// index), CCreatureActionBase::SetAnimation(CAnimationEntry*, float, float) @ 0x00693030 (-1.0f, 1.0f).
// CombatAnimations is the vector whose first-pointer sits at +0xf0 (Ego_r.pdb name; +0x104 in the PDB build;
// retail's CCombatActionBase is 0x114 bytes, modelled locally).
// Shape: the vector address is pushed (edi) before the creature is fetched, the index call's result indexes
// the vector's first pointer (`mov ecx,[edi] ; mov edx,[ecx+eax*4]`), then the two float immediates and the
// entry are pushed right to left.
struct CAnimationEntry;

struct CAnimationEntryVector {                  // std::vector<const CAnimationEntry*>
    const CAnimationEntry** m_pFirst;           // +0x00
    const CAnimationEntry** m_pLast;            // +0x04
    const CAnimationEntry** m_pEnd;             // +0x08
    const CAnimationEntry* operator[](long index) const { return m_pFirst[index]; }
};

struct CThingCreatureBase_Calls {              // engine/CThingCreatureBase.h exists but is not used: only its GetAppropriateCombatAnimationFromList call is used
    long GetAppropriateCombatAnimationFromList(const CAnimationEntryVector& animations);   // 0x00666B50
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Calls* GetPItem() const;       // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xf0 - 0x10];          // +0x10
    CAnimationEntryVector CombatAnimations;     // +0xf0
    unsigned char _padfc[0x114 - 0xfc];         // +0xfc
    void SetAnimation(const CAnimationEntry* entry, float start, float end);   // 0x00693030 (CCreatureActionBase)
    void SetupCombatAnimation();
};

void CCombatActionBase::SetupCombatAnimation()
{
    SetAnimation(CombatAnimations[PCreature.GetPItem()->GetAppropriateCombatAnimationFromList(CombatAnimations)], -1.0f, 1.0f);
}