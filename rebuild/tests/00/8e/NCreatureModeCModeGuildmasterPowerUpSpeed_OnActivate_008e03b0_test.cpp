// Behaviour test for 0x008E03B0 NCreatureMode::CModeGuildmasterPowerUpSpeed::OnActivate (links the REAL row object, including the
// object's own GetAllThingsInRoughArea<.., CFilter_Bind2<HasDefGlobalIndex, IsHoldingMeleeWeapon> > walk, GetSubDef<CSpecialEffectsDef>,
// GetEffectFromString and both vector<CIntelligentPointer<T> >::_M_insert_overflow bodies). Fixtures: raw retail layouts (mode: parent
// thing +0x04, emitters +0x78, guards +0x84; thing: type +0x10, presence bits +0x20 (TCI_MULTI_STRIKE = bit 7 of +0x30), type flags +0x6C,
// base def +0x70, dead bit 0 of +0x91; def: refcount +0x04, slot 1 Delete, slot 14 GetPSubDefInfo, manager +0x1C, effect map +0x28; thing
// map: level infos +0x04, cell vectors +0x14; mapwho: parent +0x04, search id +0x4C). Every callee the object leaves undefined is a
// recording stub bound by /alternatename (the effect-map find returns a node, or the header for a miss). Each run checks the calls, their arguments, the chosen guards, AddTC only for guards without a
// multi-strike component, the two vectors and the reference counts.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("?OnActivate@CModeStandBase@@UAEXXZ", "@StandActivate@8")
#pragma ALT("?SuspendAI@CThingAICreature@@QAEX_NPAX@Z", "@SuspendAI@16")
#pragma ALT("??0CCharString@@QAE@PBDJ@Z", "@StrCtor@16")
#pragma ALT("??1CCharString@@QAE@XZ", "@StrDtor@8")
#pragma ALT("?NullChar@CCharString@@1DB", "_g_nullChar")
#pragma ALT("?Get@CGameDefinitionManager@@SIAAV1@XZ", "@GameDefsGet@0")
#pragma ALT("?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z", "@DefIndexFromName@12")
#pragma ALT("?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z", "@PDefFromIndex@16")
#pragma ALT("?GetPos@CThing@@QBEABVC3DVector@@XZ", "@GetPos@8")
#pragma ALT("?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ", "@GetWorld@8")
#pragma ALT("?GetWorldSeed@CWorld@@QBEPAKXZ", "@GetWorldSeed@8")
#pragma ALT("?GFROR13@@YIJK@Z", "@Ror13@4")
#pragma ALT("?Calc@CCRC@@SIKKPBXK@Z", "@CrcCalc@12")
#pragma ALT("?find@?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$_Select1st@U?$pair@$$CBKJ@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QBE?AU?$_Rb_tree_iterator@U?$pair@$$CBKJ@std@@U?$_Const_traits@U?$pair@$$CBKJ@std@@@2@@2@ABK@Z", "@MapFind@16")
#pragma ALT("?GetAllThingMapsInArea@CThingSearchTools@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThingMap@NThingMap@@V?$allocator@PAVCThingMap@NThingMap@@@std@@@std@@@Z", "@GetMaps@16")
#pragma ALT("?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z", "@ClipArea@20")
#pragma ALT("?NoLookups@CThingSearchTools@@2KA", "_g_noLookups")
#pragma ALT("?NoThingsScanned@CThingSearchTools@@2KA", "_g_noScanned")
#pragma ALT("?SearchID@CThingSearchTools@@2KA", "_g_searchId")
#pragma ALT("?MaxLayers@NThingMap@@3JA", "_g_maxLayers")
#pragma ALT("?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B", "_g_layerInfo")
#pragma ALT("??RCThingFilter_HasDefGlobalIndex@@QBE_NABVCThing@@@Z", "@HasDefFilter@12")
#pragma ALT("?IsThingCarryingMeleeWeapon@CTCCarrying@@SI_NABVCThing@@@Z", "@IsHoldingMelee@4")
#pragma ALT("?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z", "@AddTC@20")
#pragma ALT("?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z", "@CreateEmitter@12")
#pragma ALT("??0CBaseIntelligentPointer@@QAE@XZ", "@IPCtor@8")
#pragma ALT("??1CBaseIntelligentPointer@@UAE@XZ", "@IPDtor@8")
#pragma ALT("?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ", "@IPGet@8")
#pragma ALT("?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z", "@IPSet@12")
#pragma ALT("??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z", "@IPDelete@12")
#pragma ALT("??_G?$CIntelligentPointer@VCThingAICreature@@@@UAEPAXI@Z", "@IPDelete@12")
#pragma ALT("@CallRow@8", "?OnActivate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct Vec { char* start; char* finish; char* eos; };
struct IPtr { void* vt; void* item; };
struct Thing { unsigned char b[0xA0]; };
struct Mode { void* vt; Thing* parent; unsigned char p08[0x78 - 8]; Vec emitters; Vec guards; };
typedef char ModeCheck[(size_t)&((Mode*)0)->guards == 0x84 ? 1 : -1];
struct Def { void** vt; long ref; unsigned char p08[0x1C - 8]; void* manager; unsigned char p20[0x28 - 0x20]; void* mapHeader; unsigned char p2c[0x40 - 0x2C]; };
typedef char DefCheck[(size_t)&((Def*)0)->mapHeader == 0x28 ? 1 : -1];
struct Node { long color; Node* parent; Node* left; Node* right; unsigned long key; long value; };
struct Box { long tlx, tly, brx, bry; };
struct LevelInfo { long size, extX, extY, scaleShift, noThings; };
struct CellVec { void** start; void** finish; void** eos; };
struct ThingMap { long pad; LevelInfo* liStart; LevelInfo* liFinish; LevelInfo* liEos; long manager; CellVec* cStart; CellVec* cFinish; CellVec* cEos; };
struct MapNode { void* mapwho; MapNode* next; MapNode* prev; void* cell; };
struct Mapwho { void* vt; Thing* parent; unsigned char p08[0x4C - 8]; unsigned long searchId; };
struct LayerInfo { long size, scaleShift; unsigned long used, unused; };
struct World { unsigned char p00[0x20]; void* searchTools; };
struct V3 { float x, y, z; };

extern "C" void __fastcall CallRow(Mode* self, void*);
extern "C" char g_nullChar = 0;
extern "C" unsigned long g_noLookups = 0, g_noScanned = 0, g_searchId = 0;
extern "C" long g_maxLayers = 1;
extern "C" LayerInfo g_layerInfo[2] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

static char g_log[2048];
static void Log(const char* s) { strcat(g_log, s); }
static int g_standActivate, g_suspend, g_suspendFlag, g_strLive; static void* g_suspendMode;
static Thing g_parent, g_guards[6], g_emitters[4];
static int g_bodyguard[6], g_melee[6], g_emitterDead, g_emitterCount, g_filterCalls[6];
static long g_effectArg[4]; static const void* g_posArg[4]; static int g_attachArg[4];
static Def g_baseDef, g_effectsDef; static void* g_baseVt[16]; static void* g_effVt[16]; static int g_deletes;
static long g_subDefIndex[2] = { 41, 0 }; static long g_defIndex = 77;
static char g_mgr[4], g_gameDefs[4]; static World g_world; static char g_tools[0x20];
static unsigned long g_seed;
static V3 g_pos = { 100.6f, -40.2f, 7.0f };
static Box g_mapsArea; static int g_mapsCalls, g_clipCalls;
static ThingMap g_map; static LevelInfo g_li; static CellVec g_cells; static void* g_cellArr[8]; static MapNode g_nodes[8]; static Mapwho g_who[8];
static ThingMap* g_mapList[1];
static Node g_effectNode; static unsigned long g_crcKey; static int g_effectMissing;

extern "C" void __fastcall StandActivate(Mode* self, void*) { ++g_standActivate; Log("Stand "); }
extern "C" void __fastcall SuspendAI(Thing* self, void*, bool b, void* mode) { CHECK(self == &g_parent); ++g_suspend; g_suspendFlag = b; g_suspendMode = mode; Log("Suspend "); }
struct StrData { const char* text; unsigned long len; };   // CCharString holds a pointer to {text, length}
typedef StrData* Str;
extern "C" void* __fastcall StrCtor(Str* self, void*, const char* text, long len)
{ CHECK(len == -1); StrData* d = (StrData*)malloc(sizeof(StrData)); d->text = text; d->len = (unsigned long)strlen(text); *self = d; ++g_strLive; return self; }
extern "C" void __fastcall StrDtor(Str* self, void*) { free(*self); *self = 0; --g_strLive; }
extern "C" void* __fastcall GameDefsGet() { return g_gameDefs; }
extern "C" long __fastcall DefIndexFromName(void* self, void*, const Str* name)
{ CHECK(self == g_gameDefs); CHECK(strcmp((*name)->text, "CREATURE_GUILDMASTER_BODYGUARD") == 0); Log("DefIdx "); return g_defIndex; }
extern "C" Def** __fastcall PDefFromIndex(void* self, void*, Def** ret, long index)
{ CHECK(self == g_mgr && index == 41); *ret = &g_effectsDef; ++g_effectsDef.ref; Log("PDef "); return ret; }
extern "C" const V3* __fastcall GetPos(Thing* self, void*) { CHECK(self == &g_parent); return &g_pos; }
extern "C" World* __fastcall GetWorld(Mode* self, void*) { return &g_world; }
extern "C" unsigned long* __fastcall GetWorldSeed(World* self, void*) { CHECK(self == &g_world); return &g_seed; }
extern "C" unsigned long __fastcall Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
extern "C" unsigned long __fastcall CrcCalc(unsigned long seed, const char* data, unsigned long len)
{ CHECK(seed == 0 && len == 13); g_crcKey = 0x1234u + (unsigned long)strlen(data); CHECK(strcmp(data, "POWER_UP_BEAM") == 0); return g_crcKey; }
extern "C" Node** __fastcall MapFind(void** self, void*, Node** ret, const unsigned long* key)
{ CHECK(self == &g_effectsDef.mapHeader && *key == g_crcKey); *ret = g_effectMissing ? (Node*)*self : &g_effectNode; Log("Find "); return ret; }   // a miss returns the header (end())
extern "C" void __fastcall GetMaps(void* self, void*, const Box* area, Vec* maps)
{ CHECK(self == g_tools); ++g_mapsCalls; g_mapsArea = *area; maps->start = (char*)g_mapList; maps->finish = (char*)(g_mapList + 1); maps->eos = maps->finish; }
extern "C" Box* __fastcall ClipArea(void* self, void*, Box* ret, const ThingMap* map, const Box* area)
{ CHECK(map == &g_map); ++g_clipCalls; ret->tlx = 0; ret->tly = 0; ret->brx = 6; ret->bry = 1; return ret; }
static int GuardIndex(const void* t) { int i = (int)((const Thing*)t - g_guards); if (i < 0 || i >= 6) { printf("bad thing\n"); exit(3); } return i; }
extern "C" bool __fastcall HasDefFilter(const long* self, void*, const Thing* t) { CHECK(*self == 77); ++g_filterCalls[GuardIndex(t)]; return g_bodyguard[GuardIndex(t)] != 0; }
extern "C" bool __fastcall IsHoldingMelee(const Thing* t) { return g_melee[GuardIndex(t)] != 0; }
extern "C" void* __fastcall AddTC(Thing* self, void*, const Str* name, bool activate, const void* params)
{ char b[64]; sprintf(b, "AddTC%d(%s,%d,%p) ", GuardIndex(self), (*name)->text, (int)activate, params); Log(b); return 0; }
extern "C" Thing* __fastcall CreateEmitter(long effect, const V3* pos, bool attach)
{
    int k = g_emitterCount++; if (k < 4) { g_effectArg[k] = effect; g_posArg[k] = pos; g_attachArg[k] = attach; }
    Thing* t = &g_emitters[k % 4]; t->b[0x91] = (unsigned char)(g_emitterDead ? 1 : 0); Log("Emit "); return t;
}
static int g_ipLive;
extern "C" void __fastcall IPCtor(IPtr* self, void*) { self->vt = 0; self->item = 0; ++g_ipLive; }
extern "C" void __fastcall IPDtor(IPtr* self, void*) { self->item = 0; --g_ipLive; }
extern "C" void* __fastcall IPGet(const IPtr* self, void*) { return self->item; }
extern "C" void __fastcall IPSet(IPtr* self, void*, void* item) { self->item = item; }
extern "C" void* __fastcall IPDelete(IPtr* self, void*, unsigned flags) { printf("unexpected delete\n"); exit(4); return 0; }
extern "C" void __fastcall DefDelete(Def* self, void*) { ++g_deletes; }
extern "C" long* __fastcall SubDefInfo(Def* self, void*, const Str* name)
{ CHECK(self == &g_baseDef && strcmp((*name)->text, "CSpecialEffectsDef") == 0); Log("SubDef "); return g_subDefIndex; }

static void Setup(int type, int nGuards, const int* bodyguard, const int* melee, const int* hasMulti)
{
    memset(g_log, 0, sizeof g_log); g_standActivate = g_suspend = g_suspendFlag = 0; g_suspendMode = 0; g_deletes = 0;
    memset(&g_parent, 0, sizeof g_parent); g_parent.b[0x10] = (unsigned char)type; *(Def**)&g_parent.b[0x70] = &g_baseDef;
    memset(g_guards, 0, sizeof g_guards); memset(g_filterCalls, 0, sizeof g_filterCalls);
    for (int i = 0; i < 6; ++i) { g_bodyguard[i] = i < nGuards ? bodyguard[i] : 0; g_melee[i] = i < nGuards ? melee[i] : 0;
        *(unsigned long*)&g_guards[i].b[0x6C] = 0xFFFFFFFFu; if (i < nGuards && hasMulti[i]) g_guards[i].b[0x30] = 0x80; g_guards[i].b[0x31] = 0x7F; }
    memset(&g_baseDef, 0, sizeof g_baseDef); g_baseDef.vt = g_baseVt; g_baseDef.ref = 1; g_baseDef.manager = g_mgr;
    g_baseVt[1] = (void*)DefDelete; g_baseVt[14] = (void*)SubDefInfo;
    memset(&g_effectsDef, 0, sizeof g_effectsDef); g_effectsDef.vt = g_effVt; g_effectsDef.ref = 1; g_effVt[1] = (void*)DefDelete;
    g_effectsDef.mapHeader = (void*)0x5150;   // the header: find returns another node, so the effect is found
    memset(&g_effectNode, 0, sizeof g_effectNode); g_effectNode.value = 913;
    g_world.searchTools = g_tools; memset(g_tools, 0, sizeof g_tools); g_seed = 12345; g_emitterCount = 0;
    // one thing map, one layer, one row of 6 cells: cell i holds guard i (cell 5 also holds guard 0 again: the search id skips it)
    memset(&g_map, 0, sizeof g_map); memset(g_nodes, 0, sizeof g_nodes); memset(g_who, 0, sizeof g_who);
    g_li.size = 0; g_li.extX = 6; g_li.extY = 1; g_li.scaleShift = 0; g_map.liStart = &g_li; g_map.liFinish = g_map.liEos = &g_li + 1;
    g_cells.start = g_cellArr; g_cells.finish = g_cells.eos = g_cellArr + 6; g_map.cStart = &g_cells; g_map.cFinish = g_map.cEos = &g_cells + 1;
    for (int i = 0; i < 6; ++i) { g_who[i].parent = &g_guards[i]; g_nodes[i].mapwho = &g_who[i]; g_cellArr[i] = i < nGuards ? &g_nodes[i] : 0; }
    g_who[6].parent = &g_guards[0]; g_nodes[6].mapwho = &g_who[0]; if (nGuards == 6) g_nodes[5].next = &g_nodes[6];
    g_mapList[0] = &g_map; g_maxLayers = 1; g_layerInfo[0].scaleShift = 0; g_searchId = 5; g_noLookups = 0; g_mapsCalls = g_clipCalls = 0;
}
static unsigned long NextIndex(unsigned long& seed, unsigned long count) { seed = seed * 0x24a1 + 0x24df; seed = Ror13(seed); return count ? seed % count : 0; }

static void Run(int type, int nGuards, const int* bodyguard, const int* melee, const int* hasMulti, int emitterDead, int existing)
{
    ++g_run; Setup(type, nGuards, bodyguard, melee, hasMulti); g_emitterDead = emitterDead;
    Mode mode; memset(&mode, 0, sizeof mode); mode.parent = &g_parent;
    static IPtr pre[4]; if (existing) { mode.guards.start = (char*)pre; mode.guards.finish = (char*)(pre + existing); mode.guards.eos = (char*)(pre + 4);
        for (int i = 0; i < existing; ++i) { pre[i].vt = 0; pre[i].item = &g_parent; } }
    // expected: candidates in cell order, then the draws
    int cand[6], n = 0; for (int i = 0; i < nGuards; ++i) if (bodyguard[i] && melee[i]) cand[n++] = i;
    char want[2048] = ""; unsigned long seed = 12345; int chosen[6], nc = 0;
    strcat(want, "Stand "); if (type == 1) strcat(want, "Suspend "); strcat(want, "DefIdx SubDef PDef Find ");
    while (n > 0 && existing + nc < 3) {
        unsigned long r = NextIndex(seed, (unsigned long)n); int g = cand[r];
        if (!hasMulti[g]) { char b[64]; sprintf(b, "AddTC%d(CTCMultiStrike,0,%p) ", g, (void*)0); strcat(want, b); }
        chosen[nc++] = g; for (unsigned long j = r; j + 1 < (unsigned long)n; ++j) cand[j] = cand[j + 1]; --n;
        strcat(want, "Emit ");
    }
    CallRow(&mode, 0);
    if (strcmp(g_log, want) != 0) { g_fail = 1; printf("run %d\n  got  %s\n  want %s\n", g_run, g_log, want); }
    CHECK(g_standActivate == 1);
    CHECK(type == 1 ? (g_suspend == 1 && g_suspendFlag && g_suspendMode == &mode) : g_suspend == 0);
    CHECK(g_mapsCalls == 1 && g_clipCalls == 1);
    CHECK(g_mapsArea.tlx == 80 && g_mapsArea.tly == -60 && g_mapsArea.brx == 120 && g_mapsArea.bry == -20);
    for (int i = 0; i < nGuards; ++i) CHECK(g_filterCalls[i] == 1);   // each mapwho seen once (search id)
    CHECK(g_noLookups == 1 && g_searchId == 6);
    CHECK((mode.guards.finish - mode.guards.start) / 8 == existing + nc);
    for (int k = 0; k < nc; ++k) CHECK(((IPtr*)mode.guards.start)[existing + k].item == &g_guards[chosen[k]]);
    CHECK(g_emitterCount == nc);
    for (int k = 0; k < nc && k < 4; ++k) CHECK(g_effectArg[k] == (g_effectMissing ? 0 : 913) && g_posArg[k] == &g_pos && g_attachArg[k] == 0);
    CHECK((mode.emitters.finish - mode.emitters.start) / 8 == (emitterDead ? 0 : nc));
    for (int k = 0; !emitterDead && k < nc; ++k) CHECK(((IPtr*)mode.emitters.start)[k].item == &g_emitters[k % 4]);
    CHECK(g_seed == seed);
    CHECK(g_baseDef.ref == 1 && g_effectsDef.ref == 1 && g_deletes == 0 && g_strLive == 0);
    CHECK(g_ipLive == (existing ? 0 : 0) + (int)((mode.guards.finish - mode.guards.start) / 8) - existing + (int)((mode.emitters.finish - mode.emitters.start) / 8));
    g_ipLive = 0;
}

int main()
{
    // 1: the player creature, six things: guards 0..4 are bodyguards, 1 and 3 hold no melee weapon, 2 already has multi-strike
    { int bg[6] = {1, 1, 1, 1, 1, 0}, ml[6] = {1, 0, 1, 0, 1, 1}, mu[6] = {0, 0, 1, 0, 0, 0}; Run(1, 6, bg, ml, mu, 0, 0); }
    // 2: not the player, five candidates, dead emitters: three chosen, no emitter kept
    { int bg[6] = {1, 1, 1, 1, 1, 1}, ml[6] = {1, 1, 1, 1, 1, 0}, mu[6] = {1, 0, 0, 1, 0, 0}; Run(3, 6, bg, ml, mu, 1, 0); }
    // 3: two guards already powered up: only one more is taken
    { int bg[6] = {1, 1, 1, 0, 0, 0}, ml[6] = {1, 1, 1, 0, 0, 0}, mu[6] = {0, 0, 0, 0, 0, 0}; Run(1, 3, bg, ml, mu, 0, 2); }
    // 4: nobody qualifies: no draw, nothing added
    { int bg[6] = {0, 1, 0, 0, 0, 0}, ml[6] = {1, 0, 1, 0, 0, 0}, mu[6] = {0, 0, 0, 0, 0, 0}; Run(1, 3, bg, ml, mu, 0, 0); }
    // 5: no things at all
    { int bg[6] = {0}, ml[6] = {0}, mu[6] = {0}; Run(2, 0, bg, ml, mu, 0, 0); }
    // 6: POWER_UP_BEAM is not in the effect map: the emitters get effect index 0
    { int bg[6] = {1, 1, 0, 0, 0, 0}, ml[6] = {1, 1, 0, 0, 0, 0}, mu[6] = {0, 1, 0, 0, 0, 0}; g_effectMissing = 1; Run(1, 2, bg, ml, mu, 0, 0); g_effectMissing = 0; }
    if (!g_fail) printf("008e03b0_TEST PASS\n");
    return g_fail;
}