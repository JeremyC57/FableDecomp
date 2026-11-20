// Behaviour test for 0x00D1A740 NScript::CQ_BountyHuntScript::CBH_Hostage2::Main (links the REAL row object: the row body, the
// expanded CIsThingAlive constructor / destructor, CScriptThing::GetPos, the handle constructors, the forced-inline handle operator=,
// the expanded MovieBase destructor, the CWideScreenMagicPauseEntities pair and the TU's own map<CCharString,handle> operator[] /
// lower_bound / ~map / _M_erase all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the entity (0x1C bytes: Game +4, Me +8, ParentClass +0x14);
// the parent CQ_BountyHuntScript (0x94 bytes: LeaderKilled +0x4f, Hostage2Safe +0x57, LeaderEscaped +0x59, AllLeaderChargeGruntsKilled
// +0x60, FishComplete +0x62, LakeComplete +0x6e, BanditCutsceneDone +0x6f, BountyHuntIntroComplete +0x70, TeleportInfo +0x88); the
// global script def (BountyHuntGoldReward +0xb04, BountyHuntHostage2WanderDistance +0xb28, BountyHuntRenownHostage2 +0xb54,
// BountyHuntRewardObject +0xb78, a CDefString read through GDefStringTable); things with fake implementations (GetPos slot 6); handles
// with counted implementation records; Game's vtable slots 7, 8, 12, 70, 72, 108, 121, 126, 161, 184, 264, 360, 370, 379, 475, 483, 496,
// 517, 586, 746, 763, 766, 769, 784. A scripted world changes the regions and the parent's flags at every NewScriptFrame; termination,
// StartScriptingEntity results, distances, aliveness and possession are scripted too.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the parent's flags,
// the teleport record, the cutscene cast, and the balance of strings, things, CBaseObject parts and handle references.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); fflush(stdout); exit(3); }
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_ECScriptGameResourceObjectScriptedThingBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CScriptGameResourceObjectScriptedThingBase@@MBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?AskHeroQuestion@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDABVCCharString@@22_N3@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActions@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?ClearCommands@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Converse@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@PBD@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DropGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowActionRunning@CScriptGameResourceObjectScriptedThingBase@@UBE_NABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToAndPickUpGenericBox@CScriptGameResourceObjectScriptedThingBase@@UAEXAAVCScriptThing@@W4EScriptEntityMoveType@@_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MoveToThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PerformExpression@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N111111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayCombatAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@_N11111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCCharString@@J_N111111@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SummonerLightningOrbAttackTarget@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UnsheatheWeapons@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Wait@CScriptGameResourceObjectScriptedThingBase@@UAEXM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ=_VirtualTrap")
// callees
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@StubBaseCtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@ABV0@@Z=@StubBaseCopy@12")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:??4CBaseObject@@IAEAAV0@ABV0@@Z=@StubBaseAssign@12")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@StubThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@StubThingDtor@8")
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=@StubIsAlive@8")
#pragma comment(linker, "/alternatename:?NullCoord@CScriptThing@@1VC3DVector@@A=_g_NullCoord")
#pragma comment(linker, "/alternatename:??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ=@StubHandleDtor@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@StubIsValid@8")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z=@StubMoveToPosition@28")
#pragma comment(linker, "/alternatename:??4?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@QAEAAV0@ABV0@@Z=@StubPImpAssign@12")
#pragma comment(linker, "/alternatename:??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ=@StubMoviePImpDtor@8")
#pragma comment(linker, "/alternatename:??0TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ=@StubTeleportCtor@8")
#pragma comment(linker, "/alternatename:??1TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ=@StubTeleportDtor@8")
#pragma comment(linker, "/alternatename:?push_back@?$vector@UTeleportData@CQ_BountyHuntScript@NScript@@V?$allocator@UTeleportData@CQ_BountyHuntScript@NScript@@@std@@@std@@QAEXABUTeleportData@CQ_BountyHuntScript@NScript@@@Z=@StubPushBack@12")
#pragma comment(linker, "/alternatename:??0?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAE@XZ=@StubMapCtor@8")
#pragma comment(linker, "/alternatename:?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@2@@Z=@StubInsertUnique@20")
#pragma comment(linker, "/alternatename:?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z=@StubRunCutscene@24")
#pragma comment(linker, "/alternatename:?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z=@StubFadeOut@16")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@StubDistance@12")
#pragma comment(linker, "/alternatename:?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z=@StubDefString@16")
#pragma comment(linker, "/alternatename:?GDefStringTable@@3VCDefStringTable@@A=_g_DefStringTable")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_pGlobalScriptDef")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CBH_Hostage2@CQ_BountyHuntScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

// ---- strings (one pointer, like retail's CCharString) ---------------------------------------------------------------------------
static int liveStrings;
class CCharString { public:
    CCharString(const char* text, long length); CCharString(const CCharString& o); ~CCharString();
    const CCharString& operator=(const CCharString& o); const CCharString& operator=(const char* s); bool operator<(const CCharString& o) const;
    char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::CCharString(const CCharString& o) { Text = Dup(o.Text); ++liveStrings; }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }
const CCharString& CCharString::operator=(const CCharString& o) { char* p = Dup(o.Text); free(Text); Text = p; return *this; }
const CCharString& CCharString::operator=(const char* s) { char* p = Dup(s); free(Text); Text = p; return *this; }
bool CCharString::operator<(const CCharString& o) const { return strcmp(Text, o.Text) < 0; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
static void Bad(const char* what) { ++failures; if (failures < 40) printf("FAIL %s\n", what); }

// ---- event logs ------------------------------------------------------------------------------------------------------------------
enum { MAXLOG = 60000 };
static char got[MAXLOG + 64]; static int ngot;
static char want[MAXLOG + 64]; static int nwant;
static void Put(char* buf, int& n, const char* s) { size_t m = strlen(s); if (n + (int)m + 1 < MAXLOG) { memcpy(buf + n, s, m); n += (int)m; buf[n++] = ' '; buf[n] = 0; } }
static void Ev(const char* s) { Put(got, ngot, s); }
static void Wv(const char* s) { Put(want, nwant, s); }
static const char* termScript;     // '1' terminating; past the end: terminating
static const char* startScript;    // '1' StartScriptingEntity(Me) succeeds; past the end: succeeds
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

// ---- world -----------------------------------------------------------------------------------------------------------------------
struct World {
    int region;        // 0 neither, 1 GreatwoodEntrance, 2 GreatwoodLake
    int banditAt, lakeAt, fishAt, distAt, exitDistAt, leaderDeadAt, killedAt, escapedAt, gruntsAt;
    bool intro, exitAlive, possHero, possMe, nullMarker, nullExit;
};
static World world;
struct State { int frame; int tpos, spos; bool valid; };
static State gs, ms;
static bool Bandit(int f) { return f >= world.banditAt; }
static bool Lake(int f) { return world.region == 2 || (world.region == 1 && f >= world.lakeAt); }
static bool Entrance() { return world.region == 1; }
static bool Fish(int f) { return f >= world.fishAt; }
static bool LeaderAlive(int f) { return f < world.leaderDeadAt; }
static bool LeaderKilled(int f) { return f >= world.killedAt; }
static bool Escaped(int f) { return f >= world.escapedAt; }
static bool Grunts(int f) { return f >= world.gruntsAt; }

static union { unsigned char b[0x1C]; double align; } ent;
static union { unsigned char b[0x94]; double align; } parent;
static union { unsigned char b[0x1088]; double align; } def;
extern "C" { void* g_pGlobalScriptDef; float g_NullCoord[3] = { -7.0f, -8.0f, -9.0f }; int g_DefStringTable; }
static void SetParentFlags(int f)
{
    parent.b[0x6f] = Bandit(f); parent.b[0x62] = Fish(f); parent.b[0x4f] = LeaderKilled(f); parent.b[0x59] = Escaped(f); parent.b[0x60] = Grunts(f);
}

// things: { vptr, PImp.Data, PImp.Info }; implementations: { vptr, pos }
struct Impl { void** vtbl; float pos[3]; const char* name; };
struct Thing { void** vtbl; Impl* data; void* info; };
static void* thingVtbl[80]; static void* implVtbl[80];
static Impl meImpl, heroImpl, fadeImpl, posImpl, leaderImpl, exitImpl;
static Thing heroThing;
static Thing* Me() { return (Thing*)(ent.b + 8); }
static int liveThings, baseLive;

// handles: { vptr, IntelligentPointer, PImp.Data, PImp.Info }
struct Info { unsigned long RefCount; void* DeleteFunc; void* Data; };
struct Handle { void* vtbl; void* ip; void* data; Info* info; };
static Info meInfo, heroInfo; static int meStbImpl, heroStbImpl;
enum { REF0 = 1000 };
static void Share(Handle* h, void* data, Info* info) { if (info) ++info->RefCount; if (h->info) --h->info->RefCount; h->data = data; h->info = info; }

// teleport record (copied out by push_back)
struct Teleport { char def[32], script[32], to[32], mkr[32]; unsigned long seed; float health; void* vec; int count; };
static Teleport tele;
static void* sehMeAddr;        // the handle StartScriptingEntity(Me) binds (the row's sehMe)
static char cast[128];         // the cutscene's map contents

static const char* Name(const Impl* d) { return d ? d->name : "null"; }

extern "C" {
// CBaseObject parts
void __fastcall StubBaseCtor(void* self, void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseCopy(void* self, void*, const void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseDtor(void*, void*) { --baseLive; }
void* __fastcall StubBaseAssign(void* self, void*, const void*) { return self; }
// CScriptThing
void __fastcall StubThingCopy(void* self, void*, const void* other) { memcpy(self, other, 12); ++liveThings; }
void __fastcall StubThingDtor(void* self, void*) { --liveThings; memset(self, 0xDD, 12); }
bool __fastcall StubIsAlive(const void* self, void*)
{
    const Impl* d = ((const Thing*)self)->data; char b[48];
    bool r;
    if (d == &leaderImpl) r = LeaderAlive(gs.frame);
    else if (d == &exitImpl || (world.nullExit && d == 0)) r = world.exitAlive;
    else { Bad("IsAlive receiver"); r = false; }
    sprintf(b, "A:%s=%d", Name(d), r); Ev(b); return r;
}
// handles
void __fastcall StubHandleDtor(void* self, void*) { Handle* h = (Handle*)self; if (h->info) --h->info->RefCount; h->data = 0; h->info = 0; --baseLive; }
bool __fastcall StubIsValid(const void* self, void*) { return ((const Handle*)self)->data != 0; }
void __fastcall StubRelease(void* self, void*)
{
    Handle* h = (Handle*)self; if (self != sehMeAddr) Bad("Release receiver");
    if (h->info) --h->info->RefCount; h->data = 0; h->info = 0; Ev("R");
}
void __fastcall StubMoveToPosition(void* self, void*, const float* pos, float speed, int type, bool a, bool b)
{
    if (self != sehMeAddr || ((Handle*)self)->data != &meStbImpl) Bad("MoveToPosition receiver");
    char s[96]; sprintf(s, "MOVE:%g,%g,%g@%g/%d/%d%d", pos[0], pos[1], pos[2], speed, type, a, b); Ev(s);
}
void* __fastcall StubPImpAssign(void* self, void*, const void* other)
{
    Handle* h = (Handle*)((char*)self - 8); const Handle* o = (const Handle*)((const char*)other - 8);
    Share(h, o->data, o->info); return self;
}
void __fastcall StubMoviePImpDtor(void* self, void*) { if (((void**)self)[0] || ((void**)self)[1]) Bad("movie PImp not empty"); Ev("~MOVIE"); }
// teleport data
void __fastcall StubTeleportCtor(void* self, void*)
{
    char* p = (char*)self; new (p) CCharString("", -1); memset(p + 4, 0x55, 8);
    new (p + 0xc) CCharString("", -1); new (p + 0x10) CCharString("", -1); new (p + 0x14) CCharString("", -1);
}
void __fastcall StubTeleportDtor(void* self, void*)
{
    char* p = (char*)self; ((CCharString*)p)->~CCharString(); ((CCharString*)(p + 0xc))->~CCharString();
    ((CCharString*)(p + 0x10))->~CCharString(); ((CCharString*)(p + 0x14))->~CCharString(); Ev("~TDATA");
}
void __fastcall StubPushBack(void* self, void*, const void* v)
{
    if (self != parent.b + 0x88) Bad("push_back receiver");
    const char* p = (const char*)v;
    strncpy(tele.def, ((const CCharString*)p)->Text, 31); memcpy(&tele.seed, p + 4, 4); memcpy(&tele.health, p + 8, 4);
    strncpy(tele.script, ((const CCharString*)(p + 0xc))->Text, 31); strncpy(tele.to, ((const CCharString*)(p + 0x10))->Text, 31);
    strncpy(tele.mkr, ((const CCharString*)(p + 0x14))->Text, 31); ++tele.count; Ev("PUSH");
}
// map: header node pointer +0, count +4; node { color, parent, left, right, key +0x10, value +0x14 }, 0x24 bytes
struct Node { int color; Node* parent; Node* left; Node* right; };
static int mapNodes;
void __fastcall StubMapCtor(void* self, void*)
{
    Node* h = (Node*)malloc(0x24); h->color = 0; h->parent = 0; h->left = h; h->right = h;
    ((Node**)self)[0] = h; ((int*)self)[1] = 0; Ev("MAP");
}
void** __fastcall StubInsertUnique(void* self, void*, void** ret, void* hint, const void* v)
{
    Node* h = ((Node**)self)[0]; const CCharString* key = (const CCharString*)v; const Handle* val = (const Handle*)((const char*)v + 4);
    Node* n = (Node*)malloc(0x24); n->color = 1; n->left = n->right = 0;
    new ((char*)n + 0x10) CCharString(*key);
    Handle* hv = (Handle*)((char*)n + 0x14); memcpy(hv, val, 16); hv->ip = 0; ++baseLive; if (hv->info) ++hv->info->RefCount;
    Node* root = h->parent;
    if (!root) { h->parent = n; n->parent = h; h->left = h->right = n; }
    else {
        Node* p = root;
        for (;;) {
            bool less = strcmp(key->Text, ((CCharString*)((char*)p + 0x10))->Text) < 0;
            Node*& c = less ? p->left : p->right;
            if (!c) { c = n; n->parent = p; break; }
            p = c;
        }
        if (h->left->left == n) h->left = n;
        if (h->right->right == n) h->right = n;
    }
    ++((int*)self)[1]; ++mapNodes; (void)hint; *ret = n; return ret;
}
static void CastOf(Node* n, char* out)
{
    if (!n) return; CastOf(n->left, out);
    const Handle* hv = (const Handle*)((char*)n + 0x14); char b[48];
    sprintf(b, "%s=%s,", ((CCharString*)((char*)n + 0x10))->Text, hv->data == &meStbImpl ? "me" : hv->data == &heroStbImpl ? "hero" : hv->data ? "?" : "null");
    strcat(out, b); CastOf(n->right, out);
}
void __fastcall StubRunCutscene(const CCharString* name, void* map, void* m1, void* m2, bool a, bool b)
{
    char s[160]; cast[0] = 0; CastOf(((Node**)map)[0]->parent, cast);
    sprintf(s, "CUT:%s[%s]%d%d%d%d", name->Text, cast, m1 != 0, m2 != 0, a, b); Ev(s);
}
void __fastcall StubFadeOut(const void* thing, float t, bool a, bool b)
{ char s[64]; sprintf(s, "FADE:%s@%g/%d%d", thing == Me() ? "Me" : "?", t, a, b); Ev(s); }
bool __fastcall StubDistance(const void* a, const void* b, float d)
{
    const Impl* bi = ((const Thing*)b)->data; bool r = d == 5.0f ? gs.frame >= world.distAt : gs.frame >= world.exitDistAt;
    char s[64]; sprintf(s, "D:%s-%s<%g=%d", a == Me() ? "Me" : "?", Name(bi), d, r); Ev(s); return r;
}
void* __fastcall StubDefString(const void* self, void*, void* ret, long idx)
{ if (self != &g_DefStringTable) Bad("GetString receiver"); char b[24]; sprintf(b, "OBJ_%ld", idx); new (ret) CCharString(b, -1); return ret; }
// entity base
void __fastcall StubSetPredicate(void* self, void*, const void* pred)
{
    if (self != ent.b) Bad("SetPredicate receiver");
    void* vt; memcpy(&vt, pred, 4); if (!vt || memcmp((const char*)pred + 4, Me(), 12)) Bad("predicate thing");
    Ev("P");
}
bool __fastcall StubTerminating(void* self, void*)
{ if (self != ent.b) Bad("terminate receiver"); bool t = NextBit(termScript, gs.tpos, true); Ev(t ? "T" : "t"); return t; }
}

// Me / hero implementations
static CCharString* __fastcall T_GetDefName(const void* self, void*, CCharString* ret)
{ if (self != Me()) Bad("GetDefName receiver"); new (ret) CCharString("DEF_HOSTAGE2", -1); return ret; }
static const float* __fastcall I_GetPos(const void* self, void*) { return ((const Impl*)self)->pos; }

// ---- game interface ----------------------------------------------------------------------------------------------------------------
static void* gameVtbl[800]; static void* gameObj[1]; static bool badGame;
static void CheckGame(const void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev("N"); ++gs.frame; SetParentFlags(gs.frame); }
static bool __fastcall G_StartScripting(void* self, void*, const void* who, void* handle, int prio)
{
    CheckGame(self); if (prio != 4) Bad("StartScriptingEntity priority");
    if (who == &heroThing) { Share((Handle*)handle, &heroStbImpl, &heroInfo); Ev("H"); return true; }
    if (who != Me()) Bad("StartScriptingEntity thing");
    if (!sehMeAddr) sehMeAddr = handle; else if (handle != sehMeAddr) Bad("StartScriptingEntity handle");
    bool ok = NextBit(startScript, gs.spos, true);
    if (ok) Share((Handle*)handle, &meStbImpl, &meInfo);
    Ev(ok ? "S" : "s"); return ok;
}
static void __fastcall G_RemoveThing(void* self, void*, const void* thing, bool a, bool b)
{ CheckGame(self); char s[32]; sprintf(s, "REMOVE:%s/%d%d", thing == Me() ? "Me" : "?", a, b); Ev(s); }
static bool __fastcall G_IsRegionLoaded(void* self, void*, const CCharString* name)
{
    CheckGame(self); bool r; char s[64];
    if (!strcmp(name->Text, "GreatwoodEntrance")) r = Entrance(); else if (!strcmp(name->Text, "GreatwoodLake")) r = Lake(gs.frame); else { Bad("region name"); r = false; }
    sprintf(s, "RG:%s=%d", name->Text, r); Ev(s); return r;
}
static void __fastcall G_SetKillable(void* self, void*, const void* thing, bool a, bool b)
{ CheckGame(self); char s[32]; sprintf(s, "KILLABLE:%s/%d%d", thing == Me() ? "Me" : "?", a, b); Ev(s); }
static Thing* __fastcall G_GetThing(void* self, void*, Thing* ret, const CCharString* name)
{
    CheckGame(self); Impl* d = 0;
    if (!strcmp(name->Text, "M_BH_FadeOut")) d = world.nullMarker ? 0 : &fadeImpl;
    else if (!strcmp(name->Text, "M_BH_Hostage2Pos")) d = world.nullMarker ? 0 : &posImpl;
    else if (!strcmp(name->Text, "BH_GangLeader")) d = &leaderImpl;
    else if (!strcmp(name->Text, "M_BH_Greatwood3Exit")) d = world.nullExit ? 0 : &exitImpl;
    else Bad("thing name");
    ret->vtbl = thingVtbl; ret->data = d; ret->info = 0; ++liveThings;
    char s[64]; sprintf(s, "G:%s", name->Text); Ev(s); return ret;
}
static void __fastcall G_AppearanceSeed(void* self, void*, const void* thing, unsigned long* seed)
{ CheckGame(self); if (thing != Me()) Bad("seed thing"); *seed = 0xABCD1234ul; Ev("SEED"); }
static float __fastcall G_GetHealth(void* self, void*, const void* thing) { CheckGame(self); if (thing != Me()) Bad("health thing"); Ev("HEALTH"); return 37.5f; }
static void __fastcall G_Lightningable(void* self, void*, const void* thing, bool b)
{ CheckGame(self); char s[32]; sprintf(s, "LIGHTNING:%s/%d", thing == Me() ? "Me" : "?", b); Ev(s); }
static void __fastcall G_WanderCentre(void* self, void*, Thing t, float x, float y, float z)
{ CheckGame(self); char s[64]; sprintf(s, "WCENTRE:%s@%g,%g,%g", t.data == &meImpl ? "Me" : "?", x, y, z); --liveThings; Ev(s); }
static void __fastcall G_WanderMin(void* self, void*, Thing t, float d)
{ CheckGame(self); char s[48]; sprintf(s, "WMIN:%s@%g", t.data == &meImpl ? "Me" : "?", d); --liveThings; Ev(s); }
static void __fastcall G_WanderMax(void* self, void*, Thing t, float d)
{ CheckGame(self); char s[48]; sprintf(s, "WMAX:%s@%g", t.data == &meImpl ? "Me" : "?", d); --liveThings; Ev(s); }
static void __fastcall G_StateGroup(void* self, void*, Thing t, int g)
{ CheckGame(self); char s[48]; sprintf(s, "GROUP:%s@%d", t.data == &meImpl ? "Me" : "?", g); --liveThings; Ev(s); }
static void __fastcall G_HasInformation(void* self, void*, const void* thing, bool a, bool b, bool c)
{ CheckGame(self); char s[48]; sprintf(s, "INFO:%s/%d%d%d", thing == Me() ? "Me" : "?", a, b, c); Ev(s); }
static void __fastcall G_Facing(void* self, void*, const void* thing, const void* target, bool b)
{ CheckGame(self); char s[64]; sprintf(s, "FACE:%s-%s/%d", thing == Me() ? "Me" : "?", Name(((const Thing*)target)->data), b); Ev(s); }
static void __fastcall G_Scared(void* self, void*, const void* thing, bool b)
{ CheckGame(self); char s[32]; sprintf(s, "SCARED:%s/%d", thing == Me() ? "Me" : "?", b); Ev(s); }
static Thing* __fastcall G_GetHero(void* self, void*) { CheckGame(self); return &heroThing; }
static void __fastcall G_StartMovie(void* self, void*, const CCharString* name, void* movie)
{ CheckGame(self); char s[48]; sprintf(s, "MOVIE:[%s]", name->Text); if (((void**)movie)[2] || ((void**)movie)[3]) Bad("movie handle not empty"); Ev(s); }
static void __fastcall G_Pause(void* self, void*, bool b) { CheckGame(self); Ev(b ? "PAUSE1" : "PAUSE0"); }
static void __fastcall G_GiveGold(void* self, void*, long n) { CheckGame(self); char s[32]; sprintf(s, "GOLD:%ld", n); Ev(s); }
static bool __fastcall G_Possession(void* self, void*, const CCharString* obj, const void* thing)
{
    CheckGame(self); bool hero = thing == &heroThing; if (!hero && thing != Me()) Bad("possession thing");
    bool r = hero ? world.possHero : world.possMe; char s[64]; sprintf(s, "POSS:%s/%s=%d", obj->Text, hero ? "Hero" : "Me", r); Ev(s); return r;
}
static void __fastcall G_RemoveItem(void* self, void*, const void* thing, const CCharString* obj)
{ CheckGame(self); char s[64]; sprintf(s, "RMITEM:%s/%s", thing == Me() ? "Me" : "?", obj->Text); Ev(s); }
static void __fastcall G_GiveObject(void* self, void*, const CCharString* obj, long n, bool b)
{ CheckGame(self); char s[64]; sprintf(s, "GIVEOBJ:%s/%ld/%d", obj->Text, n, b); Ev(s); }
static void __fastcall G_Renown(void* self, void*, long n) { CheckGame(self); char s[32]; sprintf(s, "RENOWN:%ld", n); Ev(s); }

// ---- reference model -----------------------------------------------------------------------------------------------------------
static bool MT() { bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); return t; }
static void MF() { Wv("N"); ++ms.frame; }
static bool MStart() { bool ok = NextBit(startScript, ms.spos, true); if (ok) ms.valid = true; Wv(ok ? "S" : "s"); return ok; }
static void MRelease() { if (ms.valid) { Wv("R"); ms.valid = false; } }
static bool MRegion(const char* n, bool r) { char s[64]; sprintf(s, "RG:%s=%d", n, r); Wv(s); return r; }
static void MPos(char* s, const char* tag, const Impl* d, const char* tail)
{ const float* p = d ? d->pos : g_NullCoord; sprintf(s, "%s%g,%g,%g%s", tag, p[0], p[1], p[2], tail); }
struct Expect { bool safe, lake, pushed; int cuts; };
static Expect ex;
// returns after a terminate exit or at the end of a branch; the lake loop ends only by termination
static void Model()
{
    char s[160];
    if (!world.intro) { if (MT()) return; Wv("REMOVE:Me/00"); return; }
    Wv("P"); MF(); if (MT()) return;
    while (!Bandit(ms.frame)) { MF(); if (MT()) return; }
    if (MT()) return;
    if (MRegion("GreatwoodEntrance", Entrance())) {
        if (MT()) return;
        Wv("KILLABLE:Me/01");
        MRelease();
        while (!MStart()) { MF(); if (MT()) return; }
        if (MT()) return;
        Wv("G:M_BH_FadeOut");
        MPos(s, "MOVE:", world.nullMarker ? 0 : &fadeImpl, "@4/1/00"); Wv(s);
        for (;;) { bool r = ms.frame >= world.distAt; sprintf(s, "D:Me-%s<5=%d", world.nullMarker ? "null" : "fade", r); Wv(s); if (r) break; MF(); if (MT()) return; }
        if (MT()) return;
        Wv("SEED"); Wv("HEALTH"); Wv("PUSH"); Wv("FADE:Me@1/11"); ex.pushed = true;
        for (;;) {
            bool lake = MRegion("GreatwoodLake", Lake(ms.frame));
            if (lake && Fish(ms.frame)) break;
            MF(); if (MT()) { Wv("~TDATA"); return; }
        }
        MT(); Wv("~TDATA");
        return;
    }
    if (!MRegion("GreatwoodLake", Lake(ms.frame))) return;
    if (MT()) return;
    Wv("KILLABLE:Me/11"); Wv("LIGHTNING:Me/0"); Wv("G:M_BH_Hostage2Pos");
    MPos(s, "WCENTRE:Me@", world.nullMarker ? 0 : &posImpl, ""); Wv(s);
    Wv("WMIN:Me@0"); Wv("WMAX:Me@12.5"); Wv("GROUP:Me@4"); Wv("INFO:Me/010"); Wv("G:BH_GangLeader");
    if (MT()) return;
    for (;;) {
        MRelease();
        while (!MStart()) { MF(); if (MT()) return; }
        if (MT()) return;
        bool alive = LeaderAlive(ms.frame); sprintf(s, "A:leader=%d", alive); Wv(s);
        if (alive && !LeaderKilled(ms.frame)) { if (MT()) return; Wv("FACE:Me-leader/0"); }
        if (Grunts(ms.frame) && (LeaderKilled(ms.frame) || Escaped(ms.frame))) {
            if (MT()) return;
            Wv("SCARED:Me/0");
            MRelease();
            while (!MStart()) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("MAP"); Wv("H"); Wv("MOVIE:[]"); Wv("PAUSE1");
            Wv("CUT:CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO[Hero=hero,Victim=me,]0001"); ++ex.cuts;
            Wv("PAUSE0"); Wv("~MOVIE");
            ex.safe = true; Wv("GOLD:500");
            sprintf(s, "POSS:OBJ_77/Hero=%d", world.possHero); Wv(s);
            if (!world.possHero) {
                if (MT()) return;
                sprintf(s, "POSS:OBJ_77/Me=%d", world.possMe); Wv(s);
                if (world.possMe) { if (MT()) return; Wv("RMITEM:Me/OBJ_77"); }
                Wv("GIVEOBJ:OBJ_77/-1/0");
            }
            MRelease();
            while (!MStart()) { MF(); if (MT()) return; }
            if (MT()) return;
            Wv("G:M_BH_Greatwood3Exit");
            sprintf(s, "A:%s=%d", world.nullExit ? "null" : "exit", world.exitAlive); Wv(s);
            if (world.exitAlive) {
                if (MT()) return;
                if (ms.valid) { if (MT()) return; MPos(s, "MOVE:", world.nullExit ? 0 : &exitImpl, "@1/1/01"); Wv(s); }
                for (;;) { bool r = ms.frame >= world.exitDistAt; sprintf(s, "D:Me-%s<2=%d", world.nullExit ? "null" : "exit", r); Wv(s); if (r) break; MF(); if (MT()) return; }
                if (MT()) return;
                Wv("RENOWN:250"); Wv("FADE:Me@1/11");
            }
            ex.lake = true;
        }
        MF(); if (MT()) return;
    }
}

static void Run(const World& w, const char* term, const char* start, int k)
{
    world = w; termScript = term; startScript = start;
    memset(&gs, 0, sizeof gs); memset(&ms, 0, sizeof ms); ngot = nwant = 0; got[0] = want[0] = 0;
    memset(parent.b, 0, sizeof parent.b); parent.b[0x70] = w.intro; SetParentFlags(0);
    memset(ent.b, 0x11, sizeof ent.b); void* g = gameObj; memcpy(ent.b + 4, &g, 4); void* pp = parent.b; memcpy(ent.b + 0x14, &pp, 4);
    Thing* me = Me(); me->vtbl = thingVtbl; me->data = &meImpl; me->info = 0;
    meInfo.RefCount = REF0; heroInfo.RefCount = REF0;
    liveThings = baseLive = liveStrings = mapNodes = 0; badGame = false; sehMeAddr = 0; memset(&tele, 0, sizeof tele); cast[0] = 0;
    memset(&ex, 0, sizeof ex);
    Model();
    CallMain(ent.b);
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); return; }
    check(!badGame, "Game receiver", k);
    check((parent.b[0x57] != 0) == ex.safe, "Hostage2Safe", k);
    check((parent.b[0x6e] != 0) == ex.lake, "LakeComplete", k);
    check(parent.b[0x70] == (unsigned char)w.intro, "BountyHuntIntroComplete untouched", k);
    if (ex.pushed)
        check(tele.count == 1 && !strcmp(tele.def, "DEF_HOSTAGE2") && tele.seed == 0xABCD1234ul && tele.health == 37.5f && !strcmp(tele.script, "BH_Hostage2")
              && !strcmp(tele.to, "GreatwoodLake") && !strcmp(tele.mkr, "M_BH_Hostage2Pos"), "teleport record", k);
    else check(tele.count == 0, "no teleport record", k);
    check(liveThings == 0, "every CScriptThing temporary destroyed", k);
    check(baseLive == 0, "every CBaseObject part destroyed (handles, movie, map values)", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(meInfo.RefCount == REF0 && heroInfo.RefCount == REF0, "handle references balanced", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 80; ++i) { thingVtbl[i] = (void*)VirtualTrap; implVtbl[i] = (void*)VirtualTrap; }
    thingVtbl[2] = (void*)T_GetDefName; implVtbl[6] = (void*)I_GetPos;
    Impl* im[] = { &meImpl, &heroImpl, &fadeImpl, &posImpl, &leaderImpl, &exitImpl };
    const char* nm[] = { "me", "hero", "fade", "pos", "leader", "exit" };
    for (int i = 0; i < 6; ++i) { im[i]->vtbl = implVtbl; im[i]->pos[0] = 10.0f * i + 1; im[i]->pos[1] = 10.0f * i + 2; im[i]->pos[2] = 10.0f * i + 3; im[i]->name = nm[i]; }
    heroThing.vtbl = thingVtbl; heroThing.data = &heroImpl; heroThing.info = 0;
    g_pGlobalScriptDef = def.b;
    long gold = 500, renown = 250, obj = 77; float wander = 12.5f;
    memcpy(def.b + 0xb04, &gold, 4); memcpy(def.b + 0xb28, &wander, 4); memcpy(def.b + 0xb54, &renown, 4); memcpy(def.b + 0xb78, &obj, 4);
    for (int i = 0; i < 800; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[12] = (void*)G_IsRegionLoaded; gameVtbl[70] = (void*)G_GetHero;
    gameVtbl[72] = (void*)G_GetThing; gameVtbl[108] = (void*)G_RemoveThing; gameVtbl[121] = (void*)G_GiveObject; gameVtbl[126] = (void*)G_GiveGold;
    gameVtbl[161] = (void*)G_Renown; gameVtbl[184] = (void*)G_Possession; gameVtbl[264] = (void*)G_GetHealth; gameVtbl[360] = (void*)G_HasInformation;
    gameVtbl[370] = (void*)G_StartMovie; gameVtbl[379] = (void*)G_Pause; gameVtbl[475] = (void*)G_Facing; gameVtbl[483] = (void*)G_AppearanceSeed;
    gameVtbl[496] = (void*)G_Scared; gameVtbl[517] = (void*)G_SetKillable; gameVtbl[586] = (void*)G_RemoveItem; gameVtbl[746] = (void*)G_Lightningable;
    gameVtbl[763] = (void*)G_WanderCentre; gameVtbl[766] = (void*)G_WanderMin; gameVtbl[769] = (void*)G_WanderMax; gameVtbl[784] = (void*)G_StateGroup;
    int k = 0;
    static const char zeros[] = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    // targeted: each region, then a terminate at each of the first 60 tests
    for (int region = 0; region < 3; ++region)
        for (int variant = 0; variant < 8; ++variant) {
            World w; memset(&w, 0, sizeof w);
            w.region = region; w.intro = variant != 7; w.banditAt = 2; w.lakeAt = 4; w.fishAt = 6; w.distAt = 3; w.exitDistAt = 9;
            w.leaderDeadAt = 3; w.killedAt = variant == 4 ? 99 : 3; w.escapedAt = 99; w.gruntsAt = 4; w.exitAlive = (variant & 1) == 0; w.possHero = (variant & 2) != 0; w.possMe = (variant & 4) != 0;
            w.nullMarker = variant == 5; w.nullExit = variant == 6;
            if (variant == 3) { w.leaderDeadAt = w.killedAt = 99; w.escapedAt = 5; }
            if (variant == 2) w.killedAt = 1;
            char ts[128]; memcpy(ts, zeros, 100); ts[100] = 0;
            Run(w, ts, "0101", ++k);
            for (int t = 0; t < 60; ++t) { memcpy(ts, zeros, 100); ts[t] = '1'; ts[100] = 0; Run(w, ts, (t & 1) ? "1" : "001", ++k); }
        }
    unsigned long seed = 11;
    for (int r = 0; r < 6000; ++r) {
#define RND(n) (seed = seed * 1103515245ul + 12345ul, (int)((seed >> 16) % (n)))
        World w; memset(&w, 0, sizeof w);
        w.region = RND(3); w.intro = RND(8) != 0; w.banditAt = RND(5); w.lakeAt = RND(12); w.fishAt = RND(12); w.distAt = RND(8); w.exitDistAt = RND(14);
        w.leaderDeadAt = RND(10); w.killedAt = RND(3) ? w.leaderDeadAt : RND(12); w.escapedAt = RND(16); w.gruntsAt = RND(10); w.exitAlive = RND(4) != 0; w.possHero = RND(2) != 0; w.possMe = RND(2) != 0;
        w.nullMarker = RND(10) == 0; w.nullExit = RND(10) == 0;
        static char ts[300], ss[12];
        for (int i = 0; i < 299; ++i) ts[i] = RND(40) == 0 ? '1' : '0';
        ts[299] = 0;
        for (int i = 0; i < 11; ++i) ss[i] = RND(3) == 0 ? '0' : '1';
        ss[11] = 0;
        Run(w, ts, ss, 1000 + r);
    }
    if (failures) { printf("00d1a740_TEST FAIL %d\n", failures); return 1; }
    printf("00d1a740_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}