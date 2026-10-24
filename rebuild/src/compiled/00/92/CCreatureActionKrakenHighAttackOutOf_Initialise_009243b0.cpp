// CCreatureAction_KrakenHighAttackOutOf::Initialise() -- end the kraken's attack beam, then the base Initialise
// Leaf: ?Initialise@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00009243B0, length 101 bytes (+ int3 padding), vtable slot 12 of 0x012911DC
// (RTTI `?A0x29922355::CCreatureAction_KrakenHighAttackOutOf`; FableWin.pdb ?Initialise@CCreatureAction_KrakenHighAttackOutOf@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::EndAttackBeam @ 0x007FC800 (landed),
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (absent arm reads the key word); VC duplicates the two-call
// tail into both arms. The class adds no members (sizeof 0x114, ctor 0x00925820).
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
    void EndAttackBeam();                       // 0x007FC800
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
struct CCreatureAction_KrakenHighAttackOutOf : CCombatActionBase {   // vtable 0x012911DC
    virtual void Initialise();
};

void CCreatureAction_KrakenHighAttackOutOf::Initialise()
{
    GetKraken()->EndAttackBeam();
    CCombatActionBase::Initialise();
}