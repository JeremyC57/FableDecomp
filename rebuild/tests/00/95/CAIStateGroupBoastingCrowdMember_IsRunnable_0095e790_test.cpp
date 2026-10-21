// Behaviour test for 0095e790: src/0095e790.cpp copied verbatim; the player chain, the boasting-crowd component and
// the crowd group stubbed so the hero/dead/component gates, the group state and acceptance tests and the boaster
// assignment are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_BoastingCrowdMember::IsRunnable() -- the live hero runs a boasting crowd (interface 0x73) whose current group is active (state not 0 or 10) and accepts this creature: target the group's boaster
// Retry 10b (combined getter sweep, FIXES B-15): the player manager and the brain's creature are read through inline getters (`g_pEngine->GetPlayerManager()`, `PBrain->GetCreature()`); plain member reads give the parked register difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E790, length 209 bytes (+ int3 padding).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 on the singleton 0x013B86A0's +0x1c, CPlayer::GetHeroThing @ 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 0x73 = word 3 bit 19),
// CTCBoastingCrowdControl::GetCrowdGroup(CThing*, CCrowdGroup**) @ 0x00783730, the group's virtual slot 28 (+0x70),
// CIntelligentPointer<..>::CIntelligentPointer(CThing*) @ 0x004A96F0 and ::operator= @ 0x004A9760 (out of line in this TU),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: out-parameter GetInterface (component in eax), the group out-pointer is the first frame word; the pointer
// assignment goes through a named local (retail re-forms its address with `lea`).
struct CThing;
struct CTCBase;
struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CPlayer { CThing* GetHeroThing(); };                     // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };            // 0x00449970
struct CEngine
{
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                  // +0x1c
    CPlayerManager* GetPlayerManager() const { return PlayerManager; }
};
extern CEngine* g_pEngine;                          // 0x013B86A0

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
    CThing* GetCreature() const { return Creature; }
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer
{
    CIntelligentPointerThing(CThing* item);                         // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};

struct CCrowdGroup
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool AcceptsMember(CThing* creature);   // slot 28 (+0x70)
    CThing* Boaster;                                // +0x04
    unsigned char _pad08[0x2c - 0x08];
    long State;                                     // +0x2c
};

struct CTCBoastingCrowdControl                      // interface type 0x73
{
    bool GetCrowdGroup(CThing* hero, CCrowdGroup** group);   // 0x00783730
};

struct CAIStateGroup_BoastingCrowdMember
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointerThing PTargetThing;          // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_BoastingCrowdMember::IsRunnable()
{
    CThing* hero = g_pEngine->GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    CCrowdGroup* group;
    CTCBoastingCrowdControl* crowd;
    if (hero && !(hero->m_flags91 & 1) && hero->GetInterface(0x73, &crowd))
    {
        if (crowd->GetCrowdGroup(hero, &group) && group->State != 0 && group->State != 10
            && group->AcceptsMember(PBrain->GetCreature()))
        {
            CIntelligentPointerThing boaster(group->Boaster);
            PTargetThing = boaster;
            return true;
        }
    }
    return false;
}
// ---- end source ----
void CBaseIntelligentPointer::v00() {}
void CCrowdGroup::v00() {} void CCrowdGroup::v01() {} void CCrowdGroup::v02() {} void CCrowdGroup::v03() {} void CCrowdGroup::v04() {} void CCrowdGroup::v05() {} void CCrowdGroup::v06() {} void CCrowdGroup::v07() {} void CCrowdGroup::v08() {} void CCrowdGroup::v09() {} void CCrowdGroup::v10() {} void CCrowdGroup::v11() {} void CCrowdGroup::v12() {} void CCrowdGroup::v13() {} void CCrowdGroup::v14() {} void CCrowdGroup::v15() {} void CCrowdGroup::v16() {} void CCrowdGroup::v17() {} void CCrowdGroup::v18() {} void CCrowdGroup::v19() {} void CCrowdGroup::v20() {} void CCrowdGroup::v21() {} void CCrowdGroup::v22() {} void CCrowdGroup::v23() {} void CCrowdGroup::v24() {} void CCrowdGroup::v25() {} void CCrowdGroup::v26() {} void CCrowdGroup::v27() {}
void CAIStateGroup_BoastingCrowdMember::v00() {}
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
CThing* CPlayer::GetHeroThing() { return g_heroPtr; }
static CPlayer g_player; CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
static CPlayerManager g_pm; static CEngine g_engine; CEngine* g_pEngine = &g_engine;
static CTCBoastingCrowdControl g_crowd;
static CInterfaceMap::SEntry g_entries[1] = { { 0x73, (CTCBase*)&g_crowd } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0x73 ? first : last; }
static CCrowdGroup* g_group = 0; static bool g_haveGroup = true;
bool CTCBoastingCrowdControl::GetCrowdGroup(CThing* hero, CCrowdGroup** group) { if (!g_haveGroup || hero != &g_hero) return false; *group = g_group; return true; }
static bool g_accept = true; static CThing* g_asked = 0;
bool CCrowdGroup::AcceptsMember(CThing* c) { g_asked = c; return g_accept; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CIntelligentPointerThing::CIntelligentPointerThing(CThing* item) { m_pItem = item; }
CIntelligentPointerThing& CIntelligentPointerThing::operator=(const CIntelligentPointerThing& o) { m_pItem = o.m_pItem; return *this; }
int main()
{
    g_engine.PlayerManager = &g_pm;
    memset(&g_hero, 0, sizeof(g_hero)); g_hero.m_interfaces.first = g_entries; g_hero.m_interfaces.last = g_entries + 1;
    g_hero.m_presentBits[0x73 >> 5] |= 1UL << (0x73 & 31);
    static CThing boaster, me; static CCrowdGroup group; group.Boaster = &boaster; group.State = 2; g_group = &group;
    static CAIBrain brain; brain.Creature = &me;
    static unsigned char raw[sizeof(CAIStateGroup_BoastingCrowdMember)]; memset(raw, 0, sizeof(raw));
    CAIStateGroup_BoastingCrowdMember* g = (CAIStateGroup_BoastingCrowdMember*)raw; g->PBrain = &brain;
    CHECK(g->IsRunnable() == true && g->PTargetThing.m_pItem == &boaster && g_asked == &me);
    g->PTargetThing.m_pItem = 0;
    group.State = 10; CHECK(g->IsRunnable() == false && g->PTargetThing.m_pItem == 0);
    group.State = 0; CHECK(g->IsRunnable() == false);
    group.State = 1; g_accept = false; CHECK(g->IsRunnable() == false);
    g_accept = true; g_haveGroup = false; CHECK(g->IsRunnable() == false);
    g_haveGroup = true; g_hero.m_flags91 = 1; CHECK(g->IsRunnable() == false);
    g_hero.m_flags91 = 0; g_heroPtr = 0; CHECK(g->IsRunnable() == false);
    if (fails == 0) printf("0095e790_TEST PASS\n"); else printf("0095e790_TEST FAIL\n");
    return fails;
}