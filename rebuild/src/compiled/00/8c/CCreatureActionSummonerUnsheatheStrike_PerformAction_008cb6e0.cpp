// CCreatureAction_SummonerUnsheatheStrike::PerformAction() -- if the creature can carry things (interface type 0x46), take whatever is in its "CARRY_SLOT_PRIMARY_SHEATHE" slot out and kill it when alive; then the unsheathe-weapons PerformAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB6E0, length 159 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface
// lookup for type 0x46 = bit 6 of the bitset byte at +0x28), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x01265458
// "CARRY_SLOT_PRIMARY_SHEATHE"), CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
// CCharString::~CCharString @ 0x0099EAE0, CTCCarrying::RemoveThingInCarrySlot(long, bool) @ 0x006A16A0, CThing::Kill(bool) @ 0x004C9B80,
// CCreatureAction_UnsheatheWeapons::PerformAction @ 0x008B7BA0 (direct base call, last).
// Shape: the out-parameter GetInterface as the if (carrying component in esi, pushed only inside); the slot name is a scoped named CCharString that
// shares the one `push ecx` frame word with the lookup key, the slot index (edi) is declared outside its scope; the removed thing is tested with the
// IS_THING_ALIVE idiom on the raw pointer. `this` rides in ebx, &PCreature in edi until the index replaces it.
// The PDB header (CCreatureAction_SummonerUnsheatheStrike.h) flattens the base; modelled locally (retail CCreatureActionBase is 0xa8 bytes).
#include "fable_string.h"
class CTCBase;
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
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
    CDefinitionManager* GetDefinitionManager();   // 0x004C7990
    void Kill(bool immediately);            // 0x004C9B80
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
class CTCCarrying { public:                        // interface type 0x46 component
    CThing* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_UnsheatheWeapons : public CCreatureActionBase { public:
    virtual void PerformAction();           // 0x008B7BA0
};
class CCreatureAction_SummonerUnsheatheStrike : public CCreatureAction_UnsheatheWeapons { public:
    virtual void PerformAction();
};

void CCreatureAction_SummonerUnsheatheStrike::PerformAction()
{
    CTCCarrying* pCarrying;
    if (PCreature.GetPItem()->GetInterface(&pCarrying, 0x46)) {
        long slot;
        {
            CCharString slotName("CARRY_SLOT_PRIMARY_SHEATHE", -1);
            slot = PCreature.GetPItem()->GetDefinitionManager()->GetDefGlobalIndexFromName(slotName);
        }
        CThing* sheathed = pCarrying->RemoveThingInCarrySlot(slot, false);
        if (sheathed != 0 && !(sheathed->m_flags91 & 1))
            sheathed->Kill(true);
    }
    CCreatureAction_UnsheatheWeapons::PerformAction();
}