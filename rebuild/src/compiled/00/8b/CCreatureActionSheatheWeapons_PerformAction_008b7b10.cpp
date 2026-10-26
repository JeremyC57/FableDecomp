// CCreatureAction_SheatheWeapons::PerformAction() -- sheathe all of the creature's weapons, drop creature mode 4 through the mode manager (interface type 0x31), then make the "SIMPLE_NOISE" noise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7B10, length 133 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CThingCreatureBase::SheatheAllWeapons @ 0x006647F0,
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x01268A5C
// "SIMPLE_NOISE"), CThingCreatureBase::MakeNoise(const CCharString&) @ 0x00662110, CCharString::~CCharString @ 0x0099EAE0.
// Shape: one `push ecx` frame word shared by the lookup key and the noise-name string; the string is a scoped named local (its address is
// re-derived with `lea` for the argument, GOTCHAS); esi is pushed only around the lookup. &PCreature rides in edi.
// No engine header for this class; modelled locally (retail CCreatureActionBase is 0xa8 bytes).
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
    void SheatheAllWeapons();               // 0x006647F0 (CThingCreatureBase)
    void MakeNoise(const CCharString& noise);   // 0x00662110 (CThingCreatureBase)
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
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_SheatheWeapons : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_SheatheWeapons::PerformAction()
{
    PCreature.GetPItem()->SheatheAllWeapons();
    CTCCreatureModeManager* pModes;
    if (PCreature.GetPItem()->GetInterface(&pModes, 0x31))
        pModes->RemoveMode(4);
    {
        CCharString noise("SIMPLE_NOISE", -1);
        PCreature.GetPItem()->MakeNoise(noise);
    }
}