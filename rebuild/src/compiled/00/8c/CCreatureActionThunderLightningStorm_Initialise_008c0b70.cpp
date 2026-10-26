// CCreatureAction_ThunderLightningStorm::Initialise() -- set the name "LIGHTNING_STORM_INTO", run the base Initialise, then put the creature into mode 0x36 through its mode manager (interface type 0x31)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0B70, length 133 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x012802A8 "LIGHTNING_STORM_INTO"), CCombatActionBase::SetName(const CCharString&)
// @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10 (bool result dropped).
// Shape: one `push ecx` frame word shared by the scoped name string and the lookup key; the mode manager is an UNINITIALISED out-parameter used
// unconditionally, so the AddMode call and the epilogue are duplicated into the miss path (which reads the never-written frame word).
// The PDB header (CCreatureAction_ThunderLightningStorm.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
#include "fable_string.h"
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
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
class CTCCreatureModeManager { public:             // interface type 0x31 component
    bool AddMode(unsigned long mode);       // 0x0073AE10
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetName(const CCharString& name);  // 0x008574D0
};
class CCreatureAction_ThunderLightningStorm : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_ThunderLightningStorm::Initialise()
{
    {
        CCharString name("LIGHTNING_STORM_INTO", -1);
        SetName(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->AddMode(0x36);
}