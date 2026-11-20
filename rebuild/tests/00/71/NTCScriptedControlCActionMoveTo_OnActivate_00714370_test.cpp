// Behaviour test for 0x00714370 NTCScriptedControl::CActionMoveTo::OnActivate (links the REAL row object, with its expanded GetTC / PeekTC
// look-ups over real CVectorMap component tables and the expanded CTCParamsPhysicsBase ctor). Fixtures: the action (0x50 bytes), its
// CTCScriptedControl (ParentThing +4), things (Type +0x10, UsingComponents +0x20, Components +0x44, PhysicsTC +0x60, ThingTypeFlags +0x6c),
// a physics TC (slots 22 GetClassTypeName / 72 GetRHSet / 83 IsPushableByHero / 84 SetAsPushableByHero, Position +0xc, Radius +0x34),
// script things (slots 11 GetPThing / 75 IsAlive); out-of-line callees are recording __fastcall stubs bound by /alternatename.
#include <stdio.h>
#include <string.h>
#pragma comment(linker, "/alternatename:@CallRow@8=?OnActivate@CActionMoveTo@NTCScriptedControl@@UAEXXZ")
#pragma comment(linker, "/alternatename:?IsAbleToMove@NCreatureMode@@YI_NABVCThing@@@Z=@StubAble@4")
#pragma comment(linker, "/alternatename:?RemoveMovementBlockingModes@NCreatureMode@@YIXAAVCThing@@@Z=@StubRemove@4")
#pragma comment(linker, "/alternatename:?Clone@CTCBase@@QAEPAV1@XZ=@StubClone@8")
#pragma comment(linker, "/alternatename:?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z=@StubAddTC@20")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StubStr@16")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StubStrDtor@8")
#pragma comment(linker, "/alternatename:??9CCharString@@QBE_NPBD@Z=@StubStrNe@12")
#pragma comment(linker, "/alternatename:??0CBaseClassNonCopyable@@IAE@XZ=@StubBaseCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseClassNonCopyable@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:??_GCTCParamsBase@@UAEPAXI@Z=_StubNever")
#pragma comment(linker, "/alternatename:??_GCTCParamsPhysicsBase@@UAEPAXI@Z=_StubNever")
#pragma comment(linker, "/alternatename:?CleanupAllActions@CTCScriptedControl@@QAEXXZ=@StubCleanup@8")
#pragma comment(linker, "/alternatename:?GetFreeActionPoint@CTCUserActionPointBase@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ@Z=@StubFree@24")
#pragma comment(linker, "/alternatename:?HasRoute@CTCPreCalculatedNavigationRoute@@QBE_NXZ=@StubHasRoute@8")
#pragma comment(linker, "/alternatename:?GetRouteDestination@CTCPreCalculatedNavigationRoute@@QBE?AVC3DVector@@XZ=@StubDest@12")
#pragma comment(linker, "/alternatename:?GetPos@CThing@@QBEABVC3DVector@@XZ=@StubGetPos@8")
#pragma comment(linker, "/alternatename:?InitialiseFromPreCalculatedRoute@CTCCreatureNavigation@@QAEXABVC3DVector@@ABV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@0@Z=@StubInit@20")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static char g_log[512];
static void L(const char* t) { strcat(g_log, t); strcat(g_log, " "); }

struct V3 { float x, y, z; };
struct Entry { long key; void* tc; };
struct Thing { void* vt; unsigned char p04[0x10 - 4]; unsigned char type; unsigned char p11[0x20 - 0x11]; unsigned long using_[9];
               Entry* begin; Entry* end; Entry* cap; unsigned char p50[0x60 - 0x50]; struct Phys* physics; unsigned char p64[8];
               unsigned long typeFlags; unsigned char p70[0x98 - 0x70]; Entry table[4]; };
typedef char OffThing[(size_t)&((Thing*)0)->physics == 0x60 && (size_t)&((Thing*)0)->typeFlags == 0x6c && (size_t)&((Thing*)0)->begin == 0x44 ? 1 : -1];
struct Phys { void** vt; unsigned char p04[8]; V3 pos; unsigned char p18[0x34 - 0x18]; float radius; const char* name; bool pushable; float rhs[6]; };
struct SThing { void** vt; void* a; void* b; };
struct Control { void* vt; Thing* parent; };
struct Action { void* vt; Control* script; bool activated, terminated; unsigned char p0a[2]; V3 pos; long move; bool face; unsigned char p1d[3];
                float proximity; SThing target; bool useTarget; unsigned char p31[3]; void* oldPhysics; SThing route; bool useRoute,
                avoid, prevPush, useActionPoint; long apIndex; bool gotAp, backup, cancel; unsigned char p4f; };
typedef char OffAction[sizeof(Action) == 0x50 && (size_t)&((Action*)0)->route == 0x38 && (size_t)&((Action*)0)->apIndex == 0x48 ? 1 : -1];
struct Route { unsigned char p[0x20]; void* routeVec[3]; bool has; V3 dest; };
struct Str { const char* p; };
struct Params { void* vt; V3 pos; float rhs[6]; };

static Thing g_parent, g_target, g_routeThing; static Phys g_phys, g_tphys; static Control g_ctl; static Action g_a;
static SThing* g_alive[2]; static bool g_aliveVal[2]; static Route g_route; static int g_carrier[8]; static char g_nav[16], g_ap[16];
static bool g_able, g_freeOk; static void* g_cloneResult = (void*)0x1234;
static void* g_physVt[90]; static void* g_sVt[80];

static void AddTC(Thing& t, long key, void* tc)
{
    t.using_[key >> 5] |= 1UL << (key & 31);
    int n = (int)(t.end - t.begin); int i = n;
    while (i > 0 && t.table[i - 1].key > key) { t.table[i] = t.table[i - 1]; --i; }
    t.table[i].key = key; t.table[i].tc = tc; t.end = t.begin + n + 1;
}
static void ResetThing(Thing& t, Phys* p) { memset(&t, 0, sizeof t); t.begin = t.end = t.table; t.cap = t.table + 4; t.physics = p; }

extern "C" void StubNever() { printf("unexpected call\n"); g_fail = 1; }
extern "C" bool __fastcall StubAble(const Thing* t) { CHECK(t == &g_parent); L("Able"); return g_able; }
extern "C" void __fastcall StubRemove(Thing* t) { CHECK(t == &g_parent); L("Remove"); }
extern "C" void* __fastcall StubClone(Phys* p, void*) { CHECK(p == &g_phys); L("Clone"); return g_cloneResult; }
extern "C" Str* __fastcall StubStr(Str* s, void*, const char* t, long n) { CHECK(n == -1); s->p = t; return s; }
extern "C" void __fastcall StubStrDtor(Str* s, void*) { s->p = 0; }
extern "C" bool __fastcall StubStrNe(const Str* s, void*, const char* t) { return strcmp(s->p, t) != 0; }
extern "C" void* __fastcall StubBaseCtor(void* s, void*) { return s; }
extern "C" void __fastcall StubBaseDtor(void*, void*) { L("~Params"); }
extern "C" void* __fastcall StubAddTC(Thing* t, void*, const Str* name, bool b, const Params* p)
{
    CHECK(t == &g_parent && strcmp(name->p, "CTCPhysicsNavigator") == 0 && b);
    CHECK(p->pos.x == g_phys.pos.x && p->pos.y == g_phys.pos.y && p->pos.z == g_phys.pos.z && memcmp(p->rhs, g_phys.rhs, sizeof p->rhs) == 0);
    L("AddTC"); return 0;
}
extern "C" void __fastcall StubCleanup(Control* c, void*) { CHECK(c == &g_ctl); L("Cleanup"); }
extern "C" bool __fastcall StubFree(void* ap, void*, Thing* t, V3* pos, float* rhs, long* idx)
{ CHECK(ap == g_ap && t == &g_parent && pos == &g_a.pos && idx == &g_a.apIndex); L("Free"); if (g_freeOk) { pos->x = 7; *idx = 3; } return g_freeOk; }
extern "C" bool __fastcall StubHasRoute(const Route* r, void*) { CHECK(r == &g_route); L("HasRoute"); return r->has; }
extern "C" V3* __fastcall StubDest(const Route* r, void*, V3* out) { CHECK(r == &g_route); *out = r->dest; return out; }
extern "C" const V3* __fastcall StubGetPos(const Thing* t, void*) { CHECK(t == &g_parent); static V3 p = { 1, 2, 3 }; return &p; }
extern "C" void __fastcall StubInit(void* nav, void*, const V3* from, void* routeVec, const V3* to)
{ CHECK(nav == g_nav && from->x == 1 && routeVec == g_route.routeVec && to == &g_a.pos); L("Init"); }
extern "C" void __fastcall CallRow(Action* self, void*);
// virtuals
extern "C" Str* __fastcall PhysName(Phys* p, void*, Str* out) { out->p = p->name; return out; }
extern "C" const float* __fastcall PhysRHS(Phys* p, void*) { return p->rhs; }
extern "C" bool __fastcall PhysIsPush(Phys* p, void*) { L("IsPush"); return p->pushable; }
extern "C" void __fastcall PhysSetPush(Phys* p, void*, bool b) { CHECK(!b); L("SetPush0"); }
extern "C" Thing* __fastcall SGetThing(SThing* s, void*) { return s == &g_a.target ? &g_target : &g_routeThing; }
extern "C" bool __fastcall SIsAlive(SThing* s, void*) { return s == &g_a.target ? g_aliveVal[0] : g_aliveVal[1]; }

static void Base()
{
    g_log[0] = 0; ResetThing(g_parent, &g_phys); ResetThing(g_target, &g_tphys); ResetThing(g_routeThing, 0);
    memset(&g_a, 0, sizeof g_a); g_a.script = &g_ctl; g_ctl.parent = &g_parent; g_a.target.vt = g_sVt; g_a.route.vt = g_sVt;
    g_a.avoid = true; g_able = true; g_aliveVal[0] = g_aliveVal[1] = true; g_freeOk = true;
    g_phys.name = "CTCPhysicsNavigator"; g_phys.pos.x = 4; g_phys.pos.y = 5; g_phys.pos.z = 6; g_phys.radius = 0.5f;
    for (int i = 0; i < 6; ++i) g_phys.rhs[i] = (float)(i + 10);
    g_tphys.radius = 1.25f; g_route.has = true; g_route.dest.x = 9; g_route.dest.y = 8; g_route.dest.z = 7; memset(g_carrier, 0, sizeof g_carrier);
}
static void Expect(const char* want) { if (strcmp(g_log, want) != 0) { g_fail = 1; printf("run %d\n  got  %s\n  want %s\n", g_run, g_log, want); } }

int main()
{
    g_physVt[22] = (void*)PhysName; g_physVt[72] = (void*)PhysRHS; g_physVt[83] = (void*)PhysIsPush; g_physVt[84] = (void*)PhysSetPush;
    g_phys.vt = g_physVt; g_tphys.vt = g_physVt; g_sVt[11] = (void*)SGetThing; g_sVt[75] = (void*)SIsAlive;
    // 1: navigator already, nothing else on: activated, old physics cleared
    g_run = 1; Base(); g_a.oldPhysics = (void*)1; CallRow(&g_a, 0); Expect("Able "); CHECK(g_a.activated && g_a.oldPhysics == 0 && !g_a.terminated);
    // 2: cannot move, other physics: blocking modes removed, physics cloned and the navigator added with the old position / rhs
    g_run = 2; Base(); g_able = false; g_phys.name = "CTCPhysicsStandard"; CallRow(&g_a, 0);
    Expect("Able Remove Clone AddTC ~Params "); CHECK(g_a.oldPhysics == g_cloneResult);
    // 3: action point, parent carried: terminated (+ cleanup when asked), nothing after
    g_run = 3; Base(); g_a.useTarget = true; g_a.useActionPoint = true; g_a.avoid = false; g_carrier[4] = 1; AddTC(g_parent, 0x47, g_carrier);
    g_a.cancel = true; CallRow(&g_a, 0); Expect("Able Cleanup "); CHECK(g_a.terminated);
    // 4: action point, carryable but not carried, free point found
    g_run = 4; Base(); g_a.useTarget = true; g_a.useActionPoint = true; AddTC(g_parent, 0x47, g_carrier); AddTC(g_target, 0x2d, g_ap);
    CallRow(&g_a, 0); Expect("Able Free "); CHECK(g_a.gotAp && !g_a.useTarget && g_a.pos.x == 7 && g_a.apIndex == 3 && !g_a.terminated);
    // 5: no free point / no action-point TC: terminated, no cleanup unless asked
    g_run = 5; Base(); g_a.useTarget = true; g_a.useActionPoint = true; AddTC(g_target, 0x2d, g_ap); g_freeOk = false; CallRow(&g_a, 0);
    Expect("Able Free "); CHECK(g_a.terminated && !g_a.gotAp);
    g_run = 6; Base(); g_a.useTarget = true; g_a.useActionPoint = true; g_a.cancel = true; CallRow(&g_a, 0); Expect("Able Cleanup "); CHECK(g_a.terminated);
    // 7: proximity: creature-type flag, type 5, type 3 widen to the radii sum when smaller; other types / dead target do not
    for (int k = 0; k < 5; ++k) {
        g_run = 70 + k; Base(); g_a.useTarget = true; g_a.proximity = k == 4 ? 3.0f : 1.0f;
        if (k == 0) g_target.typeFlags = 8; else if (k == 1) g_target.type = 5; else if (k == 2) g_target.type = 3; else if (k == 3) g_target.type = 4; else g_target.type = 5;
        CallRow(&g_a, 0); float want = k == 3 ? 1.0f : (k == 4 ? 3.0f : 1.75f); CHECK(g_a.proximity == want);
    }
    g_run = 8; Base(); g_a.useTarget = true; g_aliveVal[0] = false; g_target.type = 5; g_a.proximity = 1; CallRow(&g_a, 0); CHECK(g_a.proximity == 1);
    // 9: dynamic obstacles not avoided: pushable remembered and switched off
    g_run = 9; Base(); g_a.avoid = false; g_phys.pushable = true; CallRow(&g_a, 0); Expect("Able IsPush SetPush0 "); CHECK(g_a.prevPush);
    // 10: route thing with a route: destination taken, route started; without one: terminated; no navigation TC: nothing
    g_run = 10; Base(); g_a.useRoute = true; AddTC(g_parent, 7, g_nav); AddTC(g_routeThing, 0xa6, &g_route); CallRow(&g_a, 0);
    Expect("Able HasRoute Init "); CHECK(g_a.pos.x == 9 && g_a.pos.z == 7 && !g_a.terminated);
    g_run = 11; Base(); g_a.useRoute = true; AddTC(g_parent, 7, g_nav); AddTC(g_routeThing, 0xa6, &g_route); g_route.has = false; CallRow(&g_a, 0);
    Expect("Able HasRoute "); CHECK(g_a.terminated);
    g_run = 12; Base(); g_a.useRoute = true; AddTC(g_routeThing, 0xa6, &g_route); CallRow(&g_a, 0); Expect("Able "); CHECK(!g_a.terminated);
    g_run = 13; Base(); g_a.useRoute = true; g_aliveVal[1] = false; AddTC(g_parent, 7, g_nav); CallRow(&g_a, 0); Expect("Able ");
    if (!g_fail) printf("00714370_TEST PASS\n");
    return g_fail;
}