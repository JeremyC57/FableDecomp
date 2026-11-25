// Behaviour test for CTCBuyableHouse::FindPosToCreateNewVillager (0x006C0B20), linked against the compiled row object (which also
// carries its same-TU GetAllThings<CThing, vector<CThing*>, CFilter_Bind2<HasTC, InMap> > copy).
// Checks, from retail's disassembly:
//  - the search: GetWorld().PeekThingSearchTools() (+0x20) walks the class list; only things passing the scan test (scan-included,
//    activated, alive, not in limbo) reach the filters; the filter pair is HasTC(TCI_REGION_ENTRANCE = 99) then InMap(the
//    GetWorldMap() map, the map number GetMapNumberAt (vtable slot 16) returns for the parent thing's position), InMap only after
//    HasTC passed; GetWorldMap is called twice, GetMapNumberAt once with the parent's position;
//  - the hero: GetPlayerManager().GetMainPlayer().GetPControlledCreature();
//  - the walk: entrances in list order; the first whose position the hero is not aware of (NCreatureAI::IsAwareOfPosition(hero,
//    pos) false) is copied to ret_pos and true returned, no later entrance is asked;
//  - otherwise: with a hero closer than 20 in 2D (dx*dx + dy*dy < 400, Z ignored) false, ret_pos untouched, no house awareness
//    test; else (no hero, or 2D distance >= 20) IsAwareOfPosition(hero, house position): aware -> false, else ret_pos = the house
//    position and true;
//  - many entrances (the result vector grows past its first allocations).
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FINDPOSTOCREATENEWVILLAGER
#include <stdio.h>
#include <string.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CTCBuyableHouse.h"

namespace NCreatureAI {
bool __fastcall IsAwareOfPosition(const CThingCreatureBase* creature, const C3DVector& pos);
}

// Private / protected members reached through explicit instantiation (as the landed 0x0075A7A0 test does).
template <class Tag, typename Tag::type M> struct AccessMember { friend typename Tag::type GetMember(Tag) { return M; } };
struct FindTag { typedef bool (CTCBuyableHouse::*type)(C3DVector&); friend type GetMember(FindTag); };
struct ParentTag { typedef CThing* CTCBase::*type; friend type GetMember(ParentTag); };
template struct AccessMember<FindTag, &CTCBuyableHouse::FindPosToCreateNewVillager>;
template struct AccessMember<ParentTag, &CTCBase::ParentThing>;

// ---- the world the row sees ------------------------------------------------------------------------------------------------
enum { MAX_THINGS = 40, PARENT = MAX_THINGS, HERO = MAX_THINGS + 1, NPOS = MAX_THINGS + 2 };
static double g_thingMem[MAX_THINGS][(sizeof(CThing) + 7) / 8];
static double g_parentMem[(sizeof(CThing) + 7) / 8];
static double g_heroMem[(sizeof(CThingPlayerCreature) + 7) / 8];
static double g_houseMem[(sizeof(CTCBuyableHouse) + 7) / 8];
static double g_worldMem[(sizeof(CWorld) + 7) / 8];
static double g_toolsMem[(sizeof(CThingSearchTools) + 7) / 8];
static double g_playerManagerMem[4], g_playerMem[4];
static C3DVector g_pos[NPOS];
static bool g_hasTC[MAX_THINGS], g_inMap[MAX_THINGS], g_aware[NPOS];
static int g_nThings;
static CThingPlayerCreature* g_hero;
static std::_List_node<CClass*> g_nodes[MAX_THINGS + 1];   // [MAX_THINGS] is the sentinel
static std::_List_node_base* g_listHead;                   // the list object: its one word points at the sentinel
static long g_mapNo;
static int g_fail;
static int g_worldMapCalls, g_worldCalls, g_pmCalls, g_mainPlayerCalls, g_heroCalls, g_mapNoCalls, g_classListCalls;
static int g_badThis, g_badFilter, g_inMapBeforeHasTC;
static int g_hasTCCalls[MAX_THINGS], g_inMapCalls[MAX_THINGS];
static const CThingCreatureBase* g_awareWho[64];
static int g_awareWhat[64], g_nAware;
static const C3DVector* g_mapNoArg;

static CThing* Thing(int i) { return i == PARENT ? (CThing*)g_parentMem : i == HERO ? (CThing*)g_heroMem : (CThing*)g_thingMem[i]; }
static int Index(const void* p)
{
    for (int i = 0; i < MAX_THINGS; ++i) if (p == g_thingMem[i]) return i;
    if (p == g_parentMem) return PARENT;
    if (p == (const void*)g_heroMem) return HERO;
    return -1;
}
static int PosIndex(const C3DVector* p) { for (int i = 0; i < NPOS; ++i) if (p == &g_pos[i]) return i; return -1; }
static CTCBuyableHouse* House() { return (CTCBuyableHouse*)g_houseMem; }

// vtable slot 16 of CWorldMap is GetMapNumberAt(const C3DVector&) const (retail vtable 0x01244AEC; the row calls [vtbl+40h]).
class CTestWorldMapVtable {
public:
    virtual void Slot00() { } virtual void Slot01() { } virtual void Slot02() { } virtual void Slot03() { }
    virtual void Slot04() { } virtual void Slot05() { } virtual void Slot06() { } virtual void Slot07() { }
    virtual void Slot08() { } virtual void Slot09() { } virtual void Slot10() { } virtual void Slot11() { }
    virtual void Slot12() { } virtual void Slot13() { } virtual void Slot14() { } virtual void Slot15() { }
    virtual long MapNumberAt(const C3DVector& pos) const { ++g_mapNoCalls; g_mapNoArg = &pos; return g_mapNo; }
    long Pad[64];
};
static CTestWorldMapVtable g_map;
static CWorldMap& Map() { return reinterpret_cast<CWorldMap&>(g_map); }

CWorldMap& CTCBase::GetWorldMap() { ++g_worldMapCalls; if ((void*)this != g_houseMem) ++g_badThis; return Map(); }
CWorld& CTCBase::GetWorld() const { ++g_worldCalls; if ((const void*)this != g_houseMem) ++g_badThis; return *(CWorld*)g_worldMem; }
CPlayerManager& CTCBase::GetPlayerManager() { ++g_pmCalls; if ((void*)this != g_houseMem) ++g_badThis; return *(CPlayerManager*)g_playerManagerMem; }
CPlayer& CPlayerManager::GetMainPlayer() { ++g_mainPlayerCalls; if ((void*)this != g_playerManagerMem) ++g_badThis; return *(CPlayer*)g_playerMem; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { ++g_heroCalls; if ((const void*)this != g_playerMem) ++g_badThis; return g_hero; }
const C3DVector& CThing::GetPos() const
{
    int i = Index(this);
    if (i < 0) { ++g_badThis; i = PARENT; }
    return g_pos[i];
}
const std::list<CClass*>& CThingSearchTools::PeekClassList() const
{
    ++g_classListCalls;
    if ((const void*)this != g_toolsMem) ++g_badThis;
    return *reinterpret_cast<const std::list<CClass*>*>(&g_listHead);
}
bool CThingFilter_HasTC::operator()(const CThing& thing) const
{
    int i = Index(&thing);
    if (Type != TCI_REGION_ENTRANCE || i < 0 || i >= MAX_THINGS) { ++g_badFilter; return false; }
    ++g_hasTCCalls[i];
    return g_hasTC[i];
}
bool CThingFilter_InMap::operator()(const CThing& thing) const
{
    int i = Index(&thing);
    if (WorldMap != &Map() || MapNo != g_mapNo || i < 0 || i >= MAX_THINGS) { ++g_badFilter; return false; }
    if (g_hasTCCalls[i] == g_inMapCalls[i]) ++g_inMapBeforeHasTC;
    ++g_inMapCalls[i];
    return g_inMap[i];
}
bool __fastcall NCreatureAI::IsAwareOfPosition(const CThingCreatureBase* creature, const C3DVector& pos)
{
    int i = PosIndex(&pos);
    if (g_nAware < 64) { g_awareWho[g_nAware] = creature; g_awareWhat[g_nAware] = i; }
    ++g_nAware;
    return i >= 0 && g_aware[i];
}

// The scan-state bits, found through CThing's own inline predicates (no hand-typed offsets); checked against retail's byte
// (+0x91: Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20).
static int g_stateByte = -1;
static unsigned char g_deadBit, g_scanBits, g_limboBit;
static void FindStateBits()
{
    unsigned char* p = (unsigned char*)g_thingMem[0];
    for (int b = 0; b < (int)sizeof(CThing) && g_stateByte < 0; ++b)
        for (int k = 0; k < 8; ++k) {
            memset(p, 0, sizeof(CThing)); p[b] = (unsigned char)(1 << k);
            if (!((CThing*)p)->IsAlive()) { g_stateByte = b; g_deadBit = (unsigned char)(1 << k); break; }
        }
    for (int v = 0; v < 256 && g_stateByte >= 0; ++v) {
        memset(p, 0, sizeof(CThing)); p[g_stateByte] = (unsigned char)v;
        if (((CThing*)p)->IsIncludedInThingScans()) { g_scanBits = (unsigned char)v; break; }
    }
    for (int k = 0; k < 8 && g_stateByte >= 0; ++k) {
        unsigned char bit = (unsigned char)(1 << k);
        if (bit == g_deadBit || (bit & g_scanBits)) continue;
        memset(p, 0, sizeof(CThing)); p[g_stateByte] = (unsigned char)(g_scanBits | bit);
        if (!((CThing*)p)->IsIncludedInThingScans()) { g_limboBit = bit; break; }
    }
    memset(p, 0, sizeof(CThing));
}

static void Check(int ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }

// n things, all scan-included unless told otherwise; none an entrance until the case says so.
static void Reset(int n)
{
    memset(g_thingMem, 0, sizeof(g_thingMem));
    g_nThings = n;
    for (int i = 0; i < MAX_THINGS; ++i) {
        g_hasTC[i] = g_inMap[i] = false; g_hasTCCalls[i] = g_inMapCalls[i] = 0;
        g_pos[i] = V(100.0f + i, 200.0f + 2 * i, 3.0f * i);
    }
    for (int i = 0; i < NPOS; ++i) g_aware[i] = false;
    for (int i = 0; i < n; ++i) ((unsigned char*)g_thingMem[i])[g_stateByte] = g_scanBits;
    std::_List_node_base* sentinel = &g_nodes[MAX_THINGS];
    std::_List_node_base* prev = sentinel;
    for (int i = 0; i < n; ++i) {
        g_nodes[i]._M_data = static_cast<CClass*>(Thing(i));
        prev->_M_next = &g_nodes[i]; g_nodes[i]._M_prev = prev; prev = &g_nodes[i];
    }
    prev->_M_next = sentinel; sentinel->_M_prev = prev;
    g_listHead = sentinel;
    g_pos[PARENT] = V(10.0f, 20.0f, 5.0f);
    g_pos[HERO] = V(500.0f, 500.0f, 0.0f);
    g_hero = (CThingPlayerCreature*)g_heroMem;
    g_mapNo = 7;
    g_worldMapCalls = g_worldCalls = g_pmCalls = g_mainPlayerCalls = g_heroCalls = g_mapNoCalls = g_classListCalls = 0;
    g_badThis = g_badFilter = g_inMapBeforeHasTC = 0; g_nAware = 0; g_mapNoArg = 0;
}
static void Entrance(int i) { g_hasTC[i] = g_inMap[i] = true; }
static bool Run(C3DVector& out) { return (House()->*GetMember(FindTag()))(out); }
static bool Same(const C3DVector& a, const C3DVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
static void CommonChecks(const char* c)
{
    char m[128];
    sprintf(m, "%s: GetWorldMap x2, GetMapNumberAt x1 on the parent position", c);
    Check(g_worldMapCalls == 2 && g_mapNoCalls == 1 && g_mapNoArg == &g_pos[PARENT], m);
    sprintf(m, "%s: one search (GetWorld, PeekClassList) and one hero lookup chain", c);
    Check(g_worldCalls == 1 && g_classListCalls == 1 && g_pmCalls == 1 && g_mainPlayerCalls == 1 && g_heroCalls == 1, m);
    sprintf(m, "%s: right objects, right filter values (type 99, map, map number), InMap only after HasTC passed", c);
    Check(g_badThis == 0 && g_badFilter == 0 && g_inMapBeforeHasTC == 0, m);
    for (int i = 0; i < g_nThings; ++i) {
        sprintf(m, "%s: thing %d asked HasTC once, InMap once only when HasTC passed", c, i);
        Check(g_hasTCCalls[i] == 1 && g_inMapCalls[i] == (g_hasTC[i] ? 1 : 0), m);
    }
}

int main()
{
    FindStateBits();
    Check(g_stateByte >= 0 && g_deadBit == 0x01 && g_scanBits == 0x0C && g_limboBit == 0x20, "CThing scan bits as retail (+0x91: 1 / 4|8 / 20)");
    if (g_fail) { printf("006c0b20_TEST FAIL\n"); return 1; }
    { CThing* parent = Thing(PARENT); House()->*GetMember(ParentTag()) = parent; }
    ((CWorld*)g_worldMem)->PThingSearchTools_ptr = (CThingSearchTools*)g_toolsMem;   // CWorld +0x20 (flattened scoped_ptr)
    C3DVector out;
    // A: thing 0 not an entrance, 1 entrance in another map, 2 entrance the hero sees, 3 and 4 unseen entrances: thing 3 wins.
    {
        Reset(5); g_hasTC[1] = true; Entrance(2); Entrance(3); Entrance(4); g_aware[2] = true;
        out = V(-1, -1, -1);
        bool r = Run(out);
        Check(r && Same(out, g_pos[3]), "A first unseen entrance: true, its position");
        Check(g_nAware == 2 && g_awareWhat[0] == 2 && g_awareWhat[1] == 3 && g_awareWho[0] == (const CThingCreatureBase*)g_hero
              && g_awareWho[1] == (const CThingCreatureBase*)g_hero, "A hero asked about entrances 2 then 3 only");
        CommonChecks("A");
    }
    // B: every entrance seen, hero 2D-near the house (12,15: 369 < 400) though far in Z: false, ret_pos untouched.
    {
        Reset(3); Entrance(0); Entrance(2); g_aware[0] = g_aware[2] = true;
        g_pos[HERO] = V(22.0f, 35.0f, 900.0f);
        out = V(-1, -2, -3);
        bool r = Run(out);
        Check(!r && Same(out, V(-1, -2, -3)), "B near hero: false, ret_pos untouched");
        Check(g_nAware == 2 && g_awareWhat[0] == 0 && g_awareWhat[1] == 2, "B no awareness test on the house");
        CommonChecks("B");
    }
    // C: exactly 20 away in 2D (12,16: 400, not < 400): house awareness asked; unseen: true, the house position.
    {
        Reset(2); Entrance(1); g_aware[1] = true;
        g_pos[HERO] = V(-2.0f, 4.0f, 5.0f);
        out = V(-1, -1, -1);
        bool r = Run(out);
        Check(r && Same(out, g_pos[PARENT]), "C hero at 20: true, house position");
        Check(g_nAware == 2 && g_awareWhat[1] == PARENT && g_awareWho[1] == (const CThingCreatureBase*)g_hero, "C hero asked about the house");
        CommonChecks("C");
    }
    // D: far hero who is aware of the house: false, ret_pos untouched.
    {
        Reset(2); g_aware[PARENT] = true;
        out = V(-4, -5, -6);
        bool r = Run(out);
        Check(!r && Same(out, V(-4, -5, -6)), "D hero sees the house: false");
        Check(g_nAware == 1 && g_awareWhat[0] == PARENT, "D only the house asked (no entrances)");
        CommonChecks("D");
    }
    // E: no hero (null): no distance test; IsAwareOfPosition(null, house) asked for the entrances and the house.
    {
        Reset(3); Entrance(1); g_aware[1] = true; g_hero = 0;
        out = V(-1, -1, -1);
        bool r = Run(out);
        Check(r && Same(out, g_pos[PARENT]), "E no hero: true, house position");
        Check(g_nAware == 2 && g_awareWho[0] == 0 && g_awareWho[1] == 0 && g_awareWhat[1] == PARENT, "E null hero passed through");
        CommonChecks("E");
    }
    // F: scan test: dead, limbo, inactive and not-scanned things never reach the filters even as unseen entrances.
    {
        Reset(5); for (int i = 0; i < 5; ++i) Entrance(i);
        unsigned char* s;
        s = (unsigned char*)g_thingMem[0]; s[g_stateByte] |= g_deadBit;
        s = (unsigned char*)g_thingMem[1]; s[g_stateByte] |= g_limboBit;
        s = (unsigned char*)g_thingMem[2]; s[g_stateByte] = (unsigned char)(g_scanBits & (g_scanBits - 1));   // one of the two bits off
        s = (unsigned char*)g_thingMem[3]; s[g_stateByte] = (unsigned char)(g_scanBits & ~(g_scanBits & (g_scanBits - 1)));
        out = V(-1, -1, -1);
        bool r = Run(out);
        Check(r && Same(out, g_pos[4]), "F only the live scanned thing 4 is found");
        int ok = 1; for (int i = 0; i < 4; ++i) ok &= g_hasTCCalls[i] == 0 && g_inMapCalls[i] == 0;
        Check(ok && g_hasTCCalls[4] == 1 && g_inMapCalls[4] == 1, "F filters asked only for thing 4");
        Check(g_nAware == 1 && g_awareWhat[0] == 4, "F hero asked once");
    }
    // G: 37 entrances, all but the last seen: the result vector grows; every entrance asked in order; the last one returned.
    {
        Reset(37); for (int i = 0; i < 37; ++i) { Entrance(i); g_aware[i] = i != 36; }
        out = V(-1, -1, -1);
        bool r = Run(out);
        int ok = g_nAware == 37; for (int i = 0; i < 37 && i < 64; ++i) ok &= g_awareWhat[i] == i;
        Check(r && Same(out, g_pos[36]) && ok, "G 37 entrances in order, the last one returned");
        CommonChecks("G");
    }
    // H: nothing in the class list: straight to the house test (far hero, unseen): true.
    {
        Reset(0);
        out = V(-1, -1, -1);
        bool r = Run(out);
        Check(r && Same(out, g_pos[PARENT]) && g_nAware == 1 && g_awareWhat[0] == PARENT, "H empty world: house position");
        CommonChecks("H");
    }
    printf(g_fail ? "006c0b20_TEST FAIL\n" : "006c0b20_TEST PASS\n");
    return g_fail;
}
