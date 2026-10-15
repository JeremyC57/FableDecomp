// CCreatureAction_TentacleStrikeInto::Initialise() -- cache the creature's kraken-tentacle component (interface type 0xe3), set the name "TENTACLE_SMASH_INTO", start the tentacle striking, then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF3D0, length 129 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for
// type 0xe3 = bit 3 of the bitset byte at +0x3c), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x01284D78 "TENTACLE_SMASH_INTO"),
// CCombatActionBase::SetName(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0, CTCKrakenTentacle::SetIsStriking(bool) @ 0x007F0770,
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the GetInterface out pointer is the MEMBER at +0x114 (the hit path stores `mov [edi+114h],ecx`, the miss path leaves it untouched, GOTCHAS 24m);
// the scoped name string shares the `push ecx` frame word with the lookup key; the member is re-read for SetIsStriking.
// The PDB header (CCreatureAction_TentacleStrikeInto.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
#include "fable_string.h"
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
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
struct CTCKrakenTentacle {                  // interface type 0xe3 component
    void SetIsStriking(bool striking);      // 0x007F0770
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetName(const CCharString& name);  // 0x008574D0
};
struct CCreatureAction_TentacleStrikeInto : CCombatActionBase {
    CTCKrakenTentacle* PTentacle;           // +0x114
    virtual void Initialise();
};

void CCreatureAction_TentacleStrikeInto::Initialise()
{
    PCreature.GetPItem()->GetInterface(&PTentacle, 0xe3);
    {
        CCharString name("TENTACLE_SMASH_INTO", -1);
        SetName(name);
    }
    PTentacle->SetIsStriking(true);
    CCombatActionBase::Initialise();
}
