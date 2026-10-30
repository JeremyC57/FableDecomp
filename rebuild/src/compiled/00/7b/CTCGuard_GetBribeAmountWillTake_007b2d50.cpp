// CTCGuard::GetBribeAmountWillTake() const -- 0 when the guard's village (via its CTCVillageMember interface, 0x23) exists and the hero is a criminal there, else 50
// Leaf: GetBribeAmountWillTake
// Grade: RELOCATION_MATCH -- ternary GetInterface store (*ppOut = (cond ? last : it)->second, landed 0x007A0490 form) puts the out pointer in ecx as retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2D50, length 89 bytes (+ int3 padding).
// CTCVillage_Calls: engine/CTCVillage.h holds the PDB data layout; only the member call is needed, so a call-only struct.
// Callees: CVectorMap<ETCInterfaceType,..>::LowerBound @ 0x004365B0, CTCVillageMember::GetPVillage @ 0x007EE540,
// CTCVillage_Calls::IsHeroCriminal @ 0x0065BFCE.
// Tried (4 grades, first authoring agent): out pointer initialised to 0 + inline GetInterface (this file) and the same through
// an inline GetVillage() helper on CTCGuard (identical bytes); the sibling 0x007B2CF0 (uninitialised out pointer, bool result)
// matches. Next: an out pointer that is a CTCBase* cast after the call, or a scheduling-only artefact of the lane compiler.
class CTCBase;
struct CTCVillage_Calls {
    bool IsHeroCriminal();                                  // 0x0065BFCE
};
class CTCVillageMember { public:
    CTCVillage_Calls* GetPVillage() const;                        // 0x007EE540
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CTCGuard { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    long GetBribeAmountWillTake() const;
};

long CTCGuard::GetBribeAmountWillTake() const
{
    CTCVillageMember* member = 0;
    ParentThing->GetInterface(&member, 0x23);               // TC_VILLAGE_MEMBER
    CTCVillage_Calls* village = member->GetPVillage();
    if (village != 0 && village->IsHeroCriminal())
        return 0;
    return 50;
}