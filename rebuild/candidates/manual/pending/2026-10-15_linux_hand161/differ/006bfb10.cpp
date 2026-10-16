// CTCBuyableHouse::IsHouseBeingUsed(bool asWorkplace) -- true when the house's village has at least one villager using this building (as workplace only, or as home or workplace)
// Grade: DIFFER(113v113) -- register swap only: retail takes the bool in cl, first in edx and ParentThing in ecx (pushes ecx,eax,edx); ours uses dl/ecx/edx (pushes edx,eax,ecx)
//
// Retail 0x006BFB10, 113 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (the thing's interface map at +0x44), CTCVillageMember::GetPVillage @ 0x007EE540
// (catalog label "IS_THING_ALIVE<..>" is propagated), STLport count_if over the village's
// list<CIntelligentPointer<CThingAICreature>> with predicate HasBuildingAsWork @ 0x006C1C70 /
// HasBuildingAsHomeOrWork @ 0x006600EC (both `ret 0Ch`: __stdcall instantiations taking first,
// last and the 4-byte predicate by value).
// Shape: the inline CThing::GetInterface of the landed 0x007B2CF0 family (interface 0x23 =
// CTCVillageMember, word 1 bit 3 -> `test byte [eax+24h],8`); a clear bit falls straight into the
// shared `xor al,al` exit. The village people list header sits at +0x2c in retail (the node
// pointer; the PDB list members are 12 bytes, retail's are smaller, so a local model is used). The
// two count_if calls take identical argument lists, so VC merges the pushes above the branch.

struct CTCBase;
struct CThing;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x004365B0
};

struct CThingInterfaces {                                   // CThing: the interface bitset + map
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
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

struct SPeopleNode;                                         // list<CIntelligentPointer<CThingAICreature>> node
struct SPeopleNodeHead { SPeopleNode* next; };

struct HasBuildingAsWork { CThingInterfaces* Building; HasBuildingAsWork(CThingInterfaces* b) : Building(b) {} };
struct HasBuildingAsHomeOrWork { CThingInterfaces* Building; HasBuildingAsHomeOrWork(CThingInterfaces* b) : Building(b) {} };

long __stdcall CountIfWork(SPeopleNode* first, SPeopleNodeHead* last, HasBuildingAsWork pred);              // 0x006C1C70
long __stdcall CountIfHomeOrWork(SPeopleNode* first, SPeopleNodeHead* last, HasBuildingAsHomeOrWork pred);  // 0x006600EC

struct CTCVillage_Model {
    unsigned char    _pad_0x00[0x2c];
    SPeopleNodeHead* m_peopleHead;                          // +0x2c list<CIntelligentPointer<CThingAICreature>> node
};

struct CTCVillageMember {
    CTCVillage_Model* GetPVillage() const;                  // 0x007EE540
};

struct CTCBuyableHouse_Model {
    void*             __vftable;                            // +0x00 (CTCBase)
    CThingInterfaces* ParentThing;                          // +0x04 (CTCBase)
    bool IsHouseBeingUsed(bool asWorkplace) const;
};

bool CTCBuyableHouse_Model::IsHouseBeingUsed(bool asWorkplace) const
{
    CTCVillageMember* member;
    if (ParentThing->GetInterface(&member, 0x23)) {         // TC_VILLAGE_MEMBER
        CTCVillage_Model* village = member->GetPVillage();
        if (village) {
            SPeopleNodeHead* last = village->m_peopleHead;
            SPeopleNode* first = last->next;
            long count;
            if (asWorkplace)
                count = CountIfWork(first, last, HasBuildingAsWork(ParentThing));
            else
                count = CountIfHomeOrWork(first, last, HasBuildingAsHomeOrWork(ParentThing));
            if (count > 0)
                return true;
        }
    }
    return false;
}
