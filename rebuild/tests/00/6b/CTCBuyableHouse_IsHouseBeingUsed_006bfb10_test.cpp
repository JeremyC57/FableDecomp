// Behaviour test for 006bfb10: src/006bfb10.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCBuyableHouse::IsHouseBeingUsed(bool asWorkplace)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBuyableHouse::IsHouseBeingUsed(bool asWorkplace) -- true when the house's village has at least one villager using this building (as workplace only, or as home or workplace)
//
// Retail 0x006BFB10, 113 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (the thing's interface map at +0x44), CTCVillageMember::GetPVillage @ 0x007EE540
// (catalog label "IS_THING_ALIVE<..>" is propagated), STLport count_if over the village's
// list<CIntelligentPointer<CThingAICreature>> with predicate HasBuildingAsWork @ 0x006C1C70 /
// HasBuildingAsHomeOrWork @ 0x006600EC (both `ret 0Ch`: __stdcall instantiations taking first,
// last and the 4-byte predicate by value).
// Layout: engine/CTCBuyableHouse.h keeps CTCBase +0x04 (ParentThing) as padding and declares no methods, so the
// 8-byte CTCBase prefix is modelled locally (ParentThing +0x04, read at 0x006BFB14 and 0x006BFB5B).
// Shape: the inline CThing::GetInterface of the landed 0x007B2CF0 family (interface 0x23 =
// CTCVillageMember, word 1 bit 3 -> `test byte [eax+24h],8`); a clear bit falls straight into the
// shared `xor al,al` exit. The village people list sits at +0x2c in retail (STLport list: its
// sentinel node pointer; the PDB list members are 12 bytes, retail's are smaller, so a local model is
// used). The two count_if calls take identical argument lists, so VC merges the pushes above the
// branch; begin()/end() are the STLport inline accessors returning the 4-byte node iterator by value
// (raw node-pointer arguments give the same instructions with edx/ecx swapped).

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

struct SPeopleNode { SPeopleNode* _M_next; SPeopleNode* _M_prev; };
struct SPeopleIter { SPeopleNode* _M_node; SPeopleIter(SPeopleNode* n) : _M_node(n) {} };
struct CPeopleList {
    SPeopleNode* _M_node;
    SPeopleIter begin() const { return SPeopleIter(_M_node->_M_next); }
    SPeopleIter end() const { return SPeopleIter(_M_node); }
};

struct HasBuildingAsWork { CThingInterfaces* Building; HasBuildingAsWork(CThingInterfaces* b) : Building(b) {} };
struct HasBuildingAsHomeOrWork { CThingInterfaces* Building; HasBuildingAsHomeOrWork(CThingInterfaces* b) : Building(b) {} };

long __stdcall CountIfWork(SPeopleIter first, SPeopleIter last, HasBuildingAsWork pred);              // 0x006C1C70
long __stdcall CountIfHomeOrWork(SPeopleIter first, SPeopleIter last, HasBuildingAsHomeOrWork pred);  // 0x006600EC

struct CTCVillage_Model {
    unsigned char    _pad_0x00[0x2c];
    CPeopleList People;                          // +0x2c list<CIntelligentPointer<CThingAICreature>> node
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
            long count;
            if (asWorkplace)
                count = CountIfWork(village->People.begin(), village->People.end(), HasBuildingAsWork(ParentThing));
            else
                count = CountIfHomeOrWork(village->People.begin(), village->People.end(), HasBuildingAsHomeOrWork(ParentThing));
            if (count > 0)
                return true;
        }
    }
    return false;
}

// ---- stubs and driver ----
static CInterfaceMap::SEntry g_entries[2];
static long g_lbType = -1;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbType = *type; return &g_entries[0]; }
static CTCVillage_Model g_village; static CTCVillage_Model* g_retVillage = &g_village; static const CTCVillageMember* g_memberThis;
CTCVillage_Model* CTCVillageMember::GetPVillage() const { g_memberThis = this; return g_retVillage; }
static int g_work = 0, g_home = 0; static SPeopleNode* g_first; static SPeopleNode* g_last; static CThingInterfaces* g_pred; static long g_count = 0;
long __stdcall CountIfWork(SPeopleIter first, SPeopleIter last, HasBuildingAsWork pred) { ++g_work; g_first = first._M_node; g_last = last._M_node; g_pred = pred.Building; return g_count; }
long __stdcall CountIfHomeOrWork(SPeopleIter first, SPeopleIter last, HasBuildingAsHomeOrWork pred) { ++g_home; g_first = first._M_node; g_last = last._M_node; g_pred = pred.Building; return g_count; }
int main()
{
    static CThingInterfaces thing; static CTCBuyableHouse_Model house; static SPeopleNode sentinel, node1;
    static CTCVillageMember member;
    sentinel._M_next = &node1; node1._M_next = &sentinel;
    g_village.People._M_node = &sentinel;
    g_entries[0].type = 0x23; g_entries[0].component = (CTCBase*)&member;
    thing.m_interfaces.first = &g_entries[0]; thing.m_interfaces.last = &g_entries[1];
    house.ParentThing = &thing;
    // interface absent -> false, nothing called
    CHECK(house.IsHouseBeingUsed(true) == false && g_work == 0 && g_home == 0 && g_lbType == -1);
    thing.m_presentBits[0x23 >> 5] = 1UL << (0x23 & 31);
    g_count = 2;
    CHECK(house.IsHouseBeingUsed(true) == true && g_work == 1 && g_home == 0 && g_lbType == 0x23 && g_memberThis == &member);
    CHECK(g_first == &node1 && g_last == &sentinel && g_pred == &thing);
    CHECK(house.IsHouseBeingUsed(false) == true && g_work == 1 && g_home == 1 && g_first == &node1 && g_last == &sentinel && g_pred == &thing);
    g_count = 0;
    CHECK(house.IsHouseBeingUsed(false) == false && g_home == 2);
    g_count = -1;
    CHECK(house.IsHouseBeingUsed(true) == false && g_work == 2);
    g_retVillage = 0; g_count = 5;
    CHECK(house.IsHouseBeingUsed(true) == false && g_work == 2 && g_home == 2);
    if (fails == 0) printf("006bfb10_TEST PASS\n");
    return fails;
}
