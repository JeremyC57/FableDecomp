// CCreatureAction_UndeadRiseUp::FinishAction() -- if still rising: flag the creature's type-9 component (+0x10 = 1), drop the underground mode and clear the rising flag; then un-suspend the creature's animation and run the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC470, length 119 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CVectorMap::LowerBound @ 0x0040F020 (the
// inlined GetInterface lookup for type 9 = bit 9 of the bitset dword at +0x20, `test ch,2`), CCreatureAction_RiseUp::
// RemoveUndergroundMode @ 0x008B7640 (landed), CThingAnimatable::SetSuspendAnimation(bool, bool) @ 0x00833A10 (two zero
// pushes), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, kept a call by the push-ecx frame).
// Shape: the FrameUpdate twin (0x008EC3F0) without the progress test; the component pointer comes from the landed
// out-parameter GetInterface template. The rising flag at +0x118 follows the retail CCreatureAction_RiseUp base
// (0x118 bytes); modelled locally (the generated headers flatten the bases).
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    void SetSuspendAnimation(bool suspend, bool immediate);   // 0x00833A10 (CThingAnimatable)
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
struct CTCType9 {                           // the creature's interface-type-9 component
    unsigned char _pad[0x10];
    unsigned char m_flag;                   // +0x10
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void FinishAction();                    // 0x00859A20
};

struct CCreatureAction_RiseUp : CCombatActionBase {   // retail sizeof 0x118
    unsigned char _pad114[4];               // +0x114
    void RemoveUndergroundMode();           // 0x008B7640
};

struct CCreatureAction_UndeadRiseUp : CCreatureAction_RiseUp {
    unsigned char m_rising;                 // +0x118
    void FinishAction();
};

void CCreatureAction_UndeadRiseUp::FinishAction()
{
    if (m_rising) {
        CTCType9* component;
        if (PCreature.GetPItem()->GetInterface(&component, 9))
            component->m_flag = 1;
        RemoveUndergroundMode();
        m_rising = 0;
    }
    CThing* creature = PCreature.GetPItem();      // a named local: retail fetches the creature before the argument pushes
    creature->SetSuspendAnimation(false, false);
    CCombatActionBase::FinishAction();
}