// CCreatureAction_ScorpionUnblockableLeftTurnAttack::Initialise() -- if the live target carries a physical shield spell (interface type 0x93), tell the shield an unblockable attack by this creature is starting; then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C42B0, length 126 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8 three times, PCreature at +8 once), CVectorMap::LowerBound
// @ 0x0040F020 (the inlined GetInterface lookup for type 0x93 = bit 19 of the bitset dword at +0x30, the CTCPhysicalShieldSpell component:
// its constructor is 0x0079E5F0), the shield component's unblockable-attack notification @ 0x0079E580 (attacker creature, starting=true flag; the manifest
// "CCreatureAction_SetDirection" label is propagated), CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the landed IS_THING_ALIVE idiom on PTarget (pointer set, dead bit +0x91 bit 0 clear), then the out-parameter GetInterface as the
// inner if (its presence test is the `test dword ptr [eax+30h],80000h` on the third GetPItem result); the component is loaded into esi before
// the creature argument is fetched (arguments right to left: the flag, the creature). `this` rides in edi, &PTarget in esi.
// The PDB header (CCreatureAction_ScorpionUnblockableLeftTurnAttack.h) is 0x24 bytes high against retail (CCombatActionBase is 0x114 bytes, PTarget at +0xa8),
// so `this` is modelled locally like the landed 0x008C8080.
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
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
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCPhysicalShieldSpell { public:             // interface type 0x93 component
    void NotifyUnblockableAttack(CThing* attacker, bool starting);   // 0x0079E580
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10 rest of CCreatureActionBase (0xa8 bytes in retail)
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing>
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
};

class CCreatureAction_ScorpionUnblockableLeftTurnAttack : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_ScorpionUnblockableLeftTurnAttack::Initialise()
{
    if (IS_THING_ALIVE(PTarget)) {
        CTCPhysicalShieldSpell* pShield;
        if (PTarget.GetPItem()->GetInterface(&pShield, 0x93))
            pShield->NotifyUnblockableAttack(PCreature.GetPItem(), true);
    }
    CCombatActionBase::Initialise();
}