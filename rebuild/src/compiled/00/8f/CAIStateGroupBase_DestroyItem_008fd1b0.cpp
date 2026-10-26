// CAIStateGroupBase::DestroyItem(const CCharString& itemName) -- kills the creature's carried item of that definition and empties its carry slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD1B0, length 111 bytes, `ret 4`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface,
// interface 0x46 = CTCCarrying: bitset word 2, bit 6 -> `test byte ptr [eax+28h],40h`),
// CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall singleton),
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
// CTCCarrying::GetThingInCarrySlot(long) @ 0x006A11F0, CThing::Kill(bool) @ 0x004C9B80,
// CTCCarrying::RemoveThingInCarrySlot(long, bool) @ 0x006A16A0.
// Model: engine/CAIStateGroupBase.h (PBrain +0x04) + engine/CAIBrain.h (Creature +0x18); the creature is viewed
// through the landed GetInterface out-parameter template (0x009543B0 family). The definition name is
// pushed before the singleton call (plain chained `Get().GetDefGlobalIndexFromName(name)`); the index is a
// named local (edi) used twice.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CTCBase;
class CCharString { public: void* m_pData; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThingView {                             // CThingAICreature: component table
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

class CThing { public:
    void Kill(bool removeNow);                  // 0x004C9B80
};

class CTCCarrying { public:
    CThing* GetThingInCarrySlot(long defIndex);                 // 0x006A11F0
    CThing* RemoveThingInCarrySlot(long defIndex, bool drop);   // 0x006A16A0
};

struct CAIStateGroupBase_Methods : CAIStateGroupBase {
    void DestroyItem(const CCharString& itemName);
};

void CAIStateGroupBase_Methods::DestroyItem(const CCharString& itemName)
{
    CTCCarrying* carrying;
    if (((CThingView*)PBrain->Creature)->GetInterface(&carrying, 0x46)) {
        long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(itemName);
        CThing* item = carrying->GetThingInCarrySlot(defIndex);
        if (item)
            item->Kill(true);
        carrying->RemoveThingInCarrySlot(defIndex, false);
    }
}