// CCreatureAction_KrakenLowAttackLoop::Initialise() -- on the first loop (LoopNum 0) switch off the kraken's head tracking; then the base Initialise
// Leaf: ?Initialise@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923FD0, length 105 bytes (+ int3 padding), vtable slot 12 of 0x01290C64
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowAttackLoop`; FableWin.pdb ?Initialise@CCreatureAction_KrakenLowAttackLoop@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CCombatActionBase::Initialise @ 0x00857520 (last).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (absent arm reads the key word and falls into the shared
// base call; the present arm duplicates the tail).
// Layout: Ego_r.pdb Interrupt / LoopNum +0x12c / +0x130, retail +0x114 / +0x118 (landed ctor 0x009255E0); CTCKraken
// HeadTrackTarget +0x51 (PDB +0x61).
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
    unsigned char _pad00[0x51];
    bool          HeadTrackTarget;              // +0x51 (PDB +0x61)
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520 (slot 12)
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
struct CCreatureAction_KrakenLowAttackLoop : CCombatActionBase {   // vtable 0x01290C64
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_KrakenLowAttackLoop::Initialise()
{
    if (LoopNum == 0)
        GetKraken()->HeadTrackTarget = false;
    CCombatActionBase::Initialise();
}