// CCombatActionBase::Initialise() -- resets the combat state, plays the combat animation the creature picks from the candidates, runs the base Initialise and, for a strike action, advances the creature's combo chain
// Leaf: ?Initialise@CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00857520, 145 bytes of code (virtual slot 12).
// Callees: the combat-state reset @ 0x008561C0 (clears the strike-event flags/times; manifest "CV_ChickenKickingScript::Init"
//          is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice through one
//          `lea ebx,[esi+8]`), CThingCreatureBase::GetAppropriateCombatAnimationFromList(const vector&) @ 0x00666B50,
//          CCreatureActionBase::SetAnimation(const CAnimationEntry*, float, float) @ 0x00693030,
//          CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), virtual IsStrikeAction (slot 15, +0x3c),
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x30 = bit 16 of the bitset dword at +0x24),
//          CTCCombat::SetNextComboChainPos(CCombatActionBase*) @ 0x00734570.
// Shape: the candidate vector's address (edi) is pushed before the creature is fetched (an inline object expression);
//          the chosen entry is read the STLport way, `*(begin() + index)`; SetAnimation gets -1.0 / 1.0; the landed
//          out-parameter GetInterface template (0x008CB4A0 family) feeds the combat component.
// Members (retail layout, Ego_r.pdb names): CombatAnimations +0xf0 (PDB +0x104), so `this` is modelled locally.
class CTCBase;
class CAnimationEntry;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CAnimationEntryVector {              // STLport vector<const CAnimationEntry*>
    const CAnimationEntry** m_pFirst;
    const CAnimationEntry** m_pLast;
    const CAnimationEntry** m_pEnd;
    const CAnimationEntry** begin() const { return m_pFirst; }
    const CAnimationEntry* const& operator[](long n) const { return *(begin() + n); }
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    long GetAppropriateCombatAnimationFromList(const CAnimationEntryVector& animations);   // 0x00666B50 (CThingCreatureBase)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

class CCombatActionBase;
class CTCCombat { public:
    void SetNextComboChainPos(CCombatActionBase* action);   // 0x00734570
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00694D10
    virtual void v13(); virtual void v14();
    virtual bool IsStrikeAction();          // slot 15 (+0x3c)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
    void SetAnimation(const CAnimationEntry* entry, float blendIn, float speed);   // 0x00693030
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pada8[0xf0 - 0xa8];
    CAnimationEntryVector CombatAnimations; // +0xf0
    unsigned char _padfc[0x114 - 0xfc];
    void ResetCombatState();                // 0x008561C0
    virtual void Initialise();
};

void CCombatActionBase::Initialise()
{
    ResetCombatState();
    long index = PCreature.GetPItem()->GetAppropriateCombatAnimationFromList(CombatAnimations);
    SetAnimation(CombatAnimations[index], -1.0f, 1.0f);
    CCreatureActionBase::Initialise();
    if (IsStrikeAction()) {
        CTCCombat* pCombat;
        if (PCreature.GetPItem()->GetInterface(&pCombat, 0x30))
            pCombat->SetNextComboChainPos(this);
    }
}