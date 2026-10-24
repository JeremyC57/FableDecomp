// CCreatureAction_KrakenHighAttackLoop::Initialise() -- on the first loop (LoopNum 0) start the kraken's high attack beam and switch on its head tracking; then the base Initialise
// Leaf: ?Initialise@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924260, length 100 bytes (+ int3 padding), vtable slot 12 of 0x0129107C
// (RTTI `?A0x29922355::CCreatureAction_KrakenHighAttackLoop`; FableWin.pdb ?Initialise@CCreatureAction_KrakenHighAttackLoop@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::StartHighAttackBeam @ 0x007FC7D0,
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (absent arm reads the key word); the component stays in esi
// for the call and the HeadTrackTarget store.
// Layout: Ego_r.pdb Interrupt / LoopNum +0x12c / +0x130, retail +0x114 / +0x118 (CCombatActionBase is 0x114 in retail; the
// landed ctor 0x00925780 stores its bool and long arguments there); CTCKraken HeadTrackTarget +0x51 (PDB +0x61).
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
    void StartHighAttackBeam();                 // 0x007FC7D0
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
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {   // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_KrakenHighAttackLoop::Initialise()
{
    if (LoopNum == 0) {
        CTCKraken* kraken = GetKraken();
        kraken->StartHighAttackBeam();
        kraken->HeadTrackTarget = true;
    }
    CCombatActionBase::Initialise();
}