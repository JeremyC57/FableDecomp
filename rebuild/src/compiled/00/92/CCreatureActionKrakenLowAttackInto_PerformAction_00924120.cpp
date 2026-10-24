// CCreatureAction_KrakenLowAttackInto::PerformAction() -- start the kraken's low attack beam and switch on its head tracking
// Leaf: ?PerformAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924120, length 91 bytes (+ int3 padding), vtable slot 3 of 0x01290B0C
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowAttackInto`, anonymous namespace of the kraken TU; FableWin.pdb names
// ?PerformAction@CCreatureAction_KrakenLowAttackInto@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface for type 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::StartLowAttackBeam @ 0x007FC7A0
// (manifest says StartHighAttackBeam: that is 0x007FC7D0; this one scales CKrakenDef::DelayBeforeLowBeam +0x4c and sets mode 4).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (the absent arm reads the uninitialised out pointer = the
// shared key word); the component stays in esi for both the call and the HeadTrackTarget store, so both arms duplicate the tail.
// Layout: CTCKraken retail HeadTrackTarget +0x51 (Ego_r.pdb +0x61: the two std::list members are 4 bytes smaller each in retail).
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
    void StartLowAttackBeam();                  // 0x007FC7A0
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void PerformAction();               // slot 3 (+0x0c)
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
struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase {   // vtable 0x01290B0C, sizeof 0x114
    virtual void PerformAction();
};

void CCreatureAction_KrakenLowAttackInto::PerformAction()
{
    CTCKraken* kraken = GetKraken();
    kraken->StartLowAttackBeam();
    kraken->HeadTrackTarget = true;
}