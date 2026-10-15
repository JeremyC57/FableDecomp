// CCreatureAction_ScorpionRecover::FinishAction() -- take the scorpion's mode manager out of mode 0x38, resume its animation if its +0x10 state byte is 1, then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4B00, length 123 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_ScorpionRecover.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CThingAnimatable::SetSuspendAnimation(bool, long)
// @ 0x00833A10, CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: the mirror of ScorpionSummon::Initialise (0x008C4A80); the GetInterface result is ignored; `this` rides in ebx, PCreature in edi.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
struct CTCCreatureModeManager {             // interface type 0x31
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};
struct CThingCreatureBase {
    unsigned char _pad00[0x10];
    unsigned char m_state;                  // +0x10 creature state byte (compared with 1; meaning not pinned down)
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};
struct CCreaturePointer : CBaseIntelligentPointer {
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)GetPItem(); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CCreaturePointer PCreature;             // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_ScorpionRecover : CCombatActionBase {
    virtual void FinishAction();
};

void CCreatureAction_ScorpionRecover::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x38);
    if (PCreature.GetCreature()->m_state == 1) {
        CThingCreatureBase* creature = PCreature.GetCreature();
        creature->SetSuspendAnimation(false, 0);
    }
    CCombatActionBase::FinishAction();
}
