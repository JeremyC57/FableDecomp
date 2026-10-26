// CCreatureAction_JackFloatUp::FinishAction() -- make sure Jack's mode manager holds mode 0x31 (floating), then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C1180, length 101 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 0x31 = bit 17 of the presence word at +0x24), CTCCreatureModeManager::HasMode @ 0x0073A8A0,
// CTCCreatureModeManager::AddMode @ 0x0073AE10, CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: the GetInterface result is NOT tested -- the absent path reads the uninitialised out pointer from the lookup-key
// frame word (`mov esi,[esp+8]`), the landed out-parameter GetInterface template (0x007504E0) reproduces that; the mode
// manager rides in esi for both calls. Retail CCombatActionBase is 0x114 bytes (the PDB header sits 0xc high), so `this`
// is modelled locally like the landed 0x008CB5C0.
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
class CTCCreatureModeManager { public:             // interface type 0x31
    bool HasMode(unsigned long mode);       // 0x0073A8A0
    bool AddMode(unsigned long mode);       // 0x0073AE10
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_JackFloatUp : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFloatUp::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    if (!modes->HasMode(0x31))
        modes->AddMode(0x31);
    CCombatActionBase::FinishAction();
}