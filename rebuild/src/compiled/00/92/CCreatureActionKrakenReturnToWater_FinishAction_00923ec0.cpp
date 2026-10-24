// CCreatureAction_KrakenReturnToWater::FinishAction() -- base FinishAction, advance the owner's boss phase, then clear the kraken's ReturnHeadToWater flag
// Leaf: ?FinishAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923EC0, length 160 bytes, vtable slot 4 of 0x012906F4
// (RTTI `?A0x29922355::CCreatureAction_KrakenReturnToWater`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenReturnToWater@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address kept in edi), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xad = CTCBoss, bit 13 of the bitset dword at
// +0x34, `test ch,20h`; then 0xe2 = CTCKraken, bit 2 of the byte at +0x3c), CTCBoss::IncrementBossPhase @ 0x007AED20.
// Shape: both lookups are UNCHECKED accessors (absent arms read the key word); the final store is duplicated into both arms.
// Layout: CTCKraken ReturnHeadToWater +0x5c (Ego_r.pdb +0x6c).
struct CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
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
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCBoss {                                // interface type 0xad component
    void IncrementBossPhase();                  // 0x007AED20
};
struct CTCKraken {                              // interface type 0xe2 component
    unsigned char _pad00[0x5c];
    bool          ReturnHeadToWater;            // +0x5c (PDB +0x6c)
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCBoss* GetBoss() const
    {
        CTCBoss* boss;
        PCreature.GetPItem()->GetInterface(&boss, 0xad);
        return boss;
    }
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenReturnToWater : CCombatActionBase {   // vtable 0x012906F4, sizeof 0x114
    virtual void FinishAction();
};

void CCreatureAction_KrakenReturnToWater::FinishAction()
{
    CCombatActionBase::FinishAction();
    GetBoss()->IncrementBossPhase();
    GetKraken()->ReturnHeadToWater = false;
}