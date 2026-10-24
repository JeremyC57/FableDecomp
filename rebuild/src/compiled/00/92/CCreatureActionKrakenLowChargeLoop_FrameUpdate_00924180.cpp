// CCreatureAction_KrakenLowChargeLoop::FrameUpdate() -- when the owner lost health since the charge started while the kraken is in boss phase 2, flag the kraken's head as hit and finish the action (virtual FinishAction); then the base FrameUpdate
// Leaf: ?FrameUpdate@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924180, length 126 bytes (+ int3 padding), vtable slot 56 of 0x012909C4
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowChargeLoop`; FableWin.pdb ?FrameUpdate@CCreatureAction_KrakenLowChargeLoop@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice: its address is CSE'd in ebx),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), the action's
// virtual FinishAction (slot 4, +0x10), CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call, last).
// Shape: the kraken is fetched first into a named local (UNCHECKED lookup in an inline accessor, absent arm reads the key word);
// `health < StartHealth` loads the owner's health first (`fcomp ; test ah,5 ; jp` = ordered less), then the phase test.
// Layout: Ego_r.pdb LoopNum / StartHealth +0x12c / +0x130, retail +0x114 / +0x118 (the landed ctor 0x009254C0 copies the owner's
// +0xb4 health into +0x118); CTCKraken HeadHasBeenHit +0x52 and Phase +0x60 (PDB +0x62 / +0x70).
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
struct CTCKraken {                              // interface type 0xe2 component
    unsigned char _pad00[0x52];
    bool          HeadHasBeenHit;               // +0x52 (PDB +0x62)
    unsigned char _pad53[0x60 - 0x53];
    long          Phase;                        // +0x60 EKrakenBossPhase (PDB +0x70)
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void FrameUpdate();                 // 0x00859D80 (slot 56)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenLowChargeLoop : CCombatActionBase {   // vtable 0x012909C4
    long  LoopNum;                              // +0x114 (PDB +0x12c)
    float StartHealth;                          // +0x118 (PDB +0x130)
    virtual void FrameUpdate();
};

void CCreatureAction_KrakenLowChargeLoop::FrameUpdate()
{
    CTCKraken* kraken = GetKraken();
    if (PCreature.GetPItem()->m_health < StartHealth && kraken->Phase == 2) {
        kraken->HeadHasBeenHit = true;
        FinishAction();
    }
    CCombatActionBase::FrameUpdate();
}