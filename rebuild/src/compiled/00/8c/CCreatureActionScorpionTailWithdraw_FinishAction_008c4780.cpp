// CCreatureAction_ScorpionTailWithdraw::FinishAction() -- resume the creature's suspended animation, drop creature mode 47 through the mode manager (interface type 0x31), then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4780, length 139 bytes (+ int3 padding). FinishAction is vtable slot 4.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CThingAnimatable::SetSuspendAnimation @ 0x00833A10
// (two zero arguments; called only while the thing's suspend byte at +0x10 equals 1), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
// CCombatActionBase::FinishAction @ 0x00859A20 (direct base call).
// Shape: the mode manager is an UNINITIALISED out-parameter of the inlined GetInterface, used unconditionally: on the miss path retail
// reads the never-written frame word (`mov eax,[esp+8]`, the slot shared with the lookup key) and the RemoveMode + base call tail is
// duplicated into both arms. The creature for SetSuspendAnimation is fetched BEFORE the two pushes, so it is a named local
// (a chained `GetPItem()->Set(0, 0)` pushes first). `this` rides in edi, &PCreature in esi until the lookup re-uses esi for the map.
// The PDB header (CCreatureAction_ScorpionTailWithdraw.h, 0x138 bytes) sits 0x24 bytes high against retail (CCombatActionBase is 0x114
// here), so `this` is modelled locally like the landed CCombatActionBase relatives (0x008CB4A0).
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char m_animationSuspended;     // +0x10 (1 while the animation is suspended)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    void SetSuspendAnimation(bool suspend, bool immediate);   // 0x00833A10
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
struct CTCCreatureModeManager {             // interface type 0x31 component
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_ScorpionTailWithdraw : CCombatActionBase {
    virtual void FinishAction();
};

void CCreatureAction_ScorpionTailWithdraw::FinishAction()
{
    if (PCreature.GetPItem()->m_animationSuspended == 1) {
        CThing* creature = PCreature.GetPItem();   // fetched before the argument pushes: a named local
        creature->SetSuspendAnimation(false, false);
    }
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(47);
    CCombatActionBase::FinishAction();
}