// Behaviour test for 0065b349: src/0065b349.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCVillage::TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCVillage::TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position) -- finds a live village dummy whose dummy-villager component stands for a dead villager of that def and has it create the villager again; null when none does
// Leaf: TryToRespawnDefNamed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B349, 183 bytes (`ret 8`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb
// `?TryToRespawnDefNamed@CTCVillage@@QAEPAVCThingAICreature@@ABVCCharString@@ABVC3DVector@@@Z`.
// Callees: CGameDefinitionManager::Get() 0x0044C6B0 (static `__fastcall`); CDefinitionManager::GetDefGlobalIndexFromName(
//          const CCharString&) 0x009AD410; CBaseIntelligentPointer::GetPItem() const 0x00A01B50; IS_THING_ALIVE(
//          CBaseObject* const&) 0x0040DCE0 and IS_THING_DEAD(CThing* const&) 0x0065D639 (`__fastcall`); std::bitset<>::
//          test(size_t) const 0x00410DE0 (interface 0xD6, the dummy villager); CVectorMap<ETCInterfaceType, CTCBase*>::
//          LowerBound 0x0040F020 (inlined GetTC); CTCDummyVillager::CreateVillager(CTCVillage*, const C3DVector&, bool)
//          0x0072F750.
// Layout: retail CTCVillage VillageDummies +0x74 (bare list head pointer, PDB +0xf8; as the landed sibling
//         DetachFromDummyVillager 0x0065B2C4); CTCDummyVillager PVillager +0x30, the villager def index +0x0c.
// Shape: `this` is spilled to [ebp-4] for the end re-read; the dummy binds IS_THING_ALIVE's const reference through a
//        derived-to-base temporary homed in the dead `defName` slot, which also holds the GetTC key (0xD6, hoisted into
//        ebx) and the villager pointer temporary of IS_THING_DEAD; the matching dummy's CreateVillager result is returned.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CCharString { void* m_data; };

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CBaseObject {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThing : CBaseObject { };
struct CThingAICreature : CThing { };
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0
bool ThingAliveBit_s7(const CBaseObject* thing);   // stand-in: the dead-flag test (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU body of the IS_THING_ALIVE<CBaseObject> instantiation (0x0040DCE0): reads the pointer through its const reference
// only, so the caller's derived-to-base temporary does not escape.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing)
{
    if (thing == 0)
        return false;
    return ThingAliveBit_s7(thing);
}
#pragma auto_inline(on)
bool __fastcall IS_THING_DEAD(CThing* const& thing);         // 0x0065D639
#pragma auto_inline(off)
// Same-TU body of the IS_THING_DEAD<CThing> instantiation (0x0065D639): reads the pointer through its const reference only.
__declspec(noinline) bool __fastcall IS_THING_DEAD(CThing* const& thing)
{
    if (thing == 0)
        return true;
    return !ThingAliveBit_s7(thing);
}
#pragma auto_inline(on)

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);  // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};

struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCVillage;
struct CTCDummyVillager {                       // interface 0xD6
    unsigned char _pad_0x00[0x0c];
    long          VillagerDefIndex;             // +0x0c
    unsigned char _pad_0x10[0x20];
    CBaseIntelligentPointer PVillager;          // +0x30
    CThingAICreature* CreateVillager(CTCVillage* village, const C3DVector& position, bool flag);   // 0x0072F750
};
CThingAICreature* CreateVillagerSink_s7(CTCDummyVillager* dummy, CTCVillage* village, float x, float y, float z, bool flag);
#pragma auto_inline(off)
// Same-TU stand-in for CreateVillager (0x0072F750): reads the position through its const reference, lets nothing escape.
__declspec(noinline) CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage* village, const C3DVector& position, bool flag)
{
    return CreateVillagerSink_s7(this, village, position.X, position.Y, position.Z, flag);
}
#pragma auto_inline(on)
struct SThingNode {                             // STLport list node
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CBaseIntelligentPointer _M_data;            // +0x08
};

struct CTCVillage {
    unsigned char _pad_0x00[0x74];
    SThingNode*   VillageDummies;               // +0x74 list<CIntelligentPointer<CThing> > head node
    CThingAICreature* TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position);
};

CThingAICreature* CTCVillage::TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position)
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    for (SThingNode* it = VillageDummies->_M_next; it != VillageDummies; it = it->_M_next) {
        CThing* dummy = it->_M_data.GetPItem();
        if (IS_THING_ALIVE(dummy) && dummy->HasTC(0xd6)) {
            CTCDummyVillager* villager = (CTCDummyVillager*)dummy->GetTC(0xd6);
            if (IS_THING_DEAD(villager->PVillager.GetPItem()) && villager->VillagerDefIndex == defIndex)
                return villager->CreateVillager(this, position, true);
        }
    }
    return 0;
}

// ---- stubs and driver ----
static CGameDefinitionManager g_dm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_dm; }
static const CCharString* g_name = 0;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_name = &name; return 42; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static CThing g_things[4]; static bool g_hasTC[4]; static bool g_alive[4];
static int Index(const CBaseObject* t) { return (int)((const CThing*)t - g_things); }
bool CBitset::test(unsigned long position) const
{
    const CBaseObject* t = (const CBaseObject*)((const char*)this - 0x20);
    return position == 0xd6 && g_hasTC[Index(t)];
}
bool ThingAliveBit_s7(const CBaseObject* thing) { return g_alive[Index(thing)]; }
static int g_create = 0; static CTCDummyVillager* g_createdBy = 0; static CTCVillage* g_village = 0; static float g_x; static bool g_flag;
static CThingAICreature g_newVillager;
CThingAICreature* CreateVillagerSink_s7(CTCDummyVillager* dummy, CTCVillage* village, float x, float y, float z, bool flag)
{ ++g_create; g_createdBy = dummy; g_village = village; g_x = x; g_flag = flag; return &g_newVillager; }
int main()
{
    static CTCDummyVillager dv[3]; static CInterfaceMap::SEntry entries[3][2];
    for (int i = 0; i < 3; ++i) {
        entries[i][0].type = 0x10; entries[i][0].component = 0; entries[i][1].type = 0xd6; entries[i][1].component = (CTCBase*)&dv[i];
        g_things[i].m_interfaces.first = entries[i]; g_things[i].m_interfaces.last = entries[i] + 2;
    }
    // dummy 0: alive with the component, villager alive; dummy 1: dead; dummy 2: alive, villager dead, def index matches
    static SThingNode head, n0, n1, n2;
    head._M_next = &n0; n0._M_next = &n1; n1._M_next = &n2; n2._M_next = &head;
    n0._M_data.m_pItem = &g_things[0]; n1._M_data.m_pItem = &g_things[1]; n2._M_data.m_pItem = &g_things[2];
    g_hasTC[0] = g_hasTC[1] = g_hasTC[2] = true; g_alive[0] = true; g_alive[1] = false; g_alive[2] = true; g_alive[3] = false;
    dv[0].PVillager.m_pItem = &g_things[3]; g_alive[3] = true; dv[0].VillagerDefIndex = 42;          // villager alive: skip
    dv[1].PVillager.m_pItem = 0; dv[1].VillagerDefIndex = 42;                                        // dummy dead: skip
    dv[2].PVillager.m_pItem = 0; dv[2].VillagerDefIndex = 41;                                        // wrong def first
    static CTCVillage village; village.VillageDummies = &head;
    CCharString name; C3DVector pos = { 1.5f, 2.0f, 3.0f };
    CHECK(village.TryToRespawnDefNamed(name, pos) == 0); CHECK(g_name == &name); CHECK(g_create == 0);
    dv[2].VillagerDefIndex = 42;
    CHECK(village.TryToRespawnDefNamed(name, pos) == &g_newVillager);
    CHECK(g_create == 1); CHECK(g_createdBy == &dv[2]); CHECK(g_village == &village); CHECK(g_x == 1.5f); CHECK(g_flag == true);
    g_alive[3] = false;                                                                               // dummy 0's villager now dead
    CHECK(village.TryToRespawnDefNamed(name, pos) == &g_newVillager); CHECK(g_createdBy == &dv[0]);
    g_hasTC[0] = false; g_hasTC[2] = false; CHECK(village.TryToRespawnDefNamed(name, pos) == 0);       // no dummy-villager component
    head._M_next = &head; CHECK(village.TryToRespawnDefNamed(name, pos) == 0);                         // empty list
    if (fails == 0) printf("0065b349_TEST PASS\n"); else printf("0065b349_TEST FAIL\n");
    return fails;
}