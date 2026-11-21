// Behaviour test for CThingAICreature::DrawPreparePrimitivesForRendering (0x00833C00): links the real row object (the list clear,
// LowerBound and the CEnginePrimitiveLine / CEnginePrimitiveText constructor COMDATs run for real). Every external callee is an
// /alternatename stand-in that logs: the base pass, list insert(end()), PeekPos / GetPos, the def-name lookup, the brain texts, the
// intelligent-pointer GetPItem, GetFocalPos, GetNavigationLevelAt, GFIntToWideString and the CCharString / CWideString members
// (wide strings are 4-byte markers; every one constructed must be destroyed). The engine (SetPersistentPrimitive +0x5C,
// AddPrimitive +0x50), the perception TC (+0x2C), the volume tracker's thing and its def (+0x28 GetInstantiationName, +0x04 destroy)
// are fake objects with hand-built vtables. Checked per scenario: the call sequence, every field of each primitive handed to the
// engine (type, interpolation set, positions, colours, sizes, flags, text markers, font), the persistent handles (the list node
// just inserted), the def reference count, and that the row always returns false.
#include <stdio.h>
#include <string.h>
#include <math.h>
extern "C" bool __fastcall RowDraw(void* self, int, const void* info);
#pragma comment(linker, "/alternatename:@RowDraw@12=?DrawPreparePrimitivesForRendering@CThingAICreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z")

struct Node { Node* next; Node* prev; void* vt; unsigned long data; };
static Node g_pool[16]; static int g_npool;
static char g_log[2048]; static int g_loglen;
static void Log(const char* s) { int n = (int)strlen(s); if (g_loglen + n + 1 < (int)sizeof g_log) { memcpy(g_log + g_loglen, s, n); g_loglen += n; g_log[g_loglen++] = ';'; g_log[g_loglen] = 0; } }

static bool g_base_ret; static void* g_self; static const void* g_info;
static int g_wlive, g_wmade, g_ccdtor, g_ccmade;
static long g_defidx_seen; static float g_focal_t; static const void* g_navpos_seen;
static void* g_brain_text1; static void* g_brain_text2;
extern "C" bool g_names, g_nav, g_percept, g_linked; extern "C" void* g_ptm; extern "C" char g_deftable[16];
bool g_names, g_nav, g_percept, g_linked; void* g_ptm; char g_deftable[16];
#pragma comment(linker, "/alternatename:?g_DebugDrawCreatureNames@@3_NA=_g_names")
#pragma comment(linker, "/alternatename:?g_DebugDrawHeroNavigationLevel@@3_NA=_g_nav")
#pragma comment(linker, "/alternatename:?DrawPerceptionDebug@CThingManager@@0_NA=_g_percept")
#pragma comment(linker, "/alternatename:?DrawLinkedBuildingsDebug@CThingManager@@0_NA=_g_linked")
#pragma comment(linker, "/alternatename:?PCurrentThingManager@CThing@@0PAVCThingManager@@A=_g_ptm")
#pragma comment(linker, "/alternatename:?GDefStringTable@@3VCDefStringTable@@A=_g_deftable")

extern "C" bool __fastcall SBase(void* self, int, const void* info) { Log("base"); return self == g_self && info == g_info && g_base_ret; }
#pragma comment(linker, "/alternatename:?DrawPreparePrimitivesForRendering@CThingCreatureBase@@MAE_NABVCEnginePrimitivePrepareInfo@@@Z=@SBase@12")
extern "C" void* __fastcall SInsert(void* list, int, void* ret, Node* pos)
{
    Log("insert");
    Node* n = &g_pool[g_npool++]; n->vt = 0; n->data = 0xA000 + g_npool;
    n->next = pos; n->prev = pos->prev; pos->prev->next = n; pos->prev = n; *(Node**)ret = n; return ret;
}
#pragma comment(linker, "/alternatename:?insert@?$list@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAE?AU?$_List_iterator@VCEnginePrimitiveHandle@@U?$_Nonconst_traits@VCEnginePrimitiveHandle@@@std@@@2@U32@@Z=@SInsert@16")
extern "C" void* __fastcall SPeekPos(char* self, int) { Log("peekpos"); return self + 0x300; }
extern "C" void* __fastcall SGetPos(char* self, int) { Log("getpos"); return self + 0x300; }
#pragma comment(linker, "/alternatename:?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ=@SPeekPos@8")
#pragma comment(linker, "/alternatename:?GetPos@CThing@@QBEABVC3DVector@@XZ=@SGetPos@8")
static char g_defmgr[16];
extern "C" void* __fastcall SPeekDefMgr(void* self, int) { Log("peekdefmgr"); return g_defmgr; }
extern "C" void* __fastcall SDefName(void* self, int, long* ret, long idx) { Log("defname"); g_defidx_seen = idx; *ret = idx + 1000; return ret; }
extern "C" void* __fastcall SGetString(void* self, int, unsigned long* ret, long idx) { Log(self == g_deftable ? "getstring" : "getstring?"); *ret = 0xC000 + idx; ++g_ccmade; return ret; }
extern "C" void __fastcall SCCDtor(unsigned long* self, int) { Log("~cc"); ++g_ccdtor; }
#pragma comment(linker, "/alternatename:?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ=@SPeekDefMgr@8")
#pragma comment(linker, "/alternatename:?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z=@SDefName@16")
#pragma comment(linker, "/alternatename:?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z=@SGetString@16")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@SCCDtor@8")
extern "C" void* __fastcall SCCToWide(unsigned long* self, int, unsigned long* ret) { Log("cc>w"); *ret = 0x100000 + (*self & 0xFFFF); ++g_wlive; ++g_wmade; return ret; }
extern "C" void __fastcall SWCopy(unsigned long* self, int, const unsigned long* src) { *self = *src; ++g_wlive; ++g_wmade; }
extern "C" void __fastcall SWDtor(unsigned long* self, int) { --g_wlive; }
extern "C" void* __fastcall SIntToWide(unsigned long* ret, long v) { Log("int>w"); *ret = 0x200000 + v; ++g_wlive; ++g_wmade; return ret; }
extern "C" void* __fastcall SDefToWide(long* self, int, unsigned long* ret) { Log("def>w"); *ret = 0x300000 + *self; ++g_wlive; ++g_wmade; return ret; }
#pragma comment(linker, "/alternatename:?ToWideString@CCharString@@QBE?AVCWideString@@XZ=@SCCToWide@12")
#pragma comment(linker, "/alternatename:??0CWideString@@QAE@ABV0@@Z=@SWCopy@12")
#pragma comment(linker, "/alternatename:??1CWideString@@QAE@XZ=@SWDtor@8")
#pragma comment(linker, "/alternatename:?GFIntToWideString@@YI?AVCWideString@@J@Z=@SIntToWide@8")
#pragma comment(linker, "/alternatename:?ToWideString@CDefString@@QBE?AVCWideString@@XZ=@SDefToWide@12")
static unsigned char g_component[0x16200];
extern "C" void* __fastcall SPeekComp(void* self, int) { Log("comp"); return g_component; }
#pragma comment(linker, "/alternatename:?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ=@SPeekComp@8")
extern "C" void* __fastcall SText1(void* self, int) { Log("brain1"); return g_brain_text1; }
extern "C" void* __fastcall SText2(void* self, int) { Log("brain2"); return g_brain_text2; }
#pragma comment(linker, "/alternatename:?GetCurrentStateGroupDebugText@CAIBrain@@QBEPBVCCharString@@XZ=@SText1@8")
#pragma comment(linker, "/alternatename:?PeekCurrentStateGroupName@CAIBrain@@QBEPBVCCharString@@XZ=@SText2@8")
extern "C" void* __fastcall SGetPItem(void** self, int) { return *self; }
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ=@SGetPItem@8")
static float g_focal[3] = { 7.0f, 8.0f, 9.0f };
extern "C" void* __fastcall SFocal(void* self, int, float* ret, float t) { Log("focal"); g_focal_t = t; ret[0] = g_focal[0]; ret[1] = g_focal[1]; ret[2] = g_focal[2]; return ret; }
extern "C" long __fastcall SNavLevel(void* self, int, const void* pos) { Log("navlevel"); g_navpos_seen = pos; return 3; }
#pragma comment(linker, "/alternatename:?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z=@SFocal@16")
#pragma comment(linker, "/alternatename:?GetNavigationLevelAt@CThingCreatureBase@@QBEJABVC3DVector@@@Z=@SNavLevel@12")

// engine
struct Prim { int kind; void* handle; unsigned long flag; unsigned char bytes[64]; };
static Prim g_prims[8]; static int g_nprims; static void* g_engine;
extern "C" void __fastcall SSetPersistent(void* self, int, void* handle, const unsigned char* prim, unsigned long flag)
{ Log("setp"); Prim& p = g_prims[g_nprims++]; p.kind = self == g_engine ? 1 : -1; p.handle = handle; p.flag = flag; memcpy(p.bytes, prim, 60); }
extern "C" void __fastcall SAdd(void* self, int, const unsigned char* prim)
{ Log("add"); Prim& p = g_prims[g_nprims++]; p.kind = self == g_engine ? 2 : -2; p.handle = 0; p.flag = 0; memcpy(p.bytes, prim, 44); }
static int g_tcdraw; static const void* g_tcdraw_info;
extern "C" bool __fastcall STcDraw(void* self, int, const void* info) { Log("tcdraw"); ++g_tcdraw; g_tcdraw_info = info; return true; }
static int g_defdestroy;
extern "C" void* __fastcall SInstName(void* self, int, long* ret) { Log("instname"); *ret = 0x77; return ret; }
extern "C" void __fastcall SDefDestroy(void* self, int) { ++g_defdestroy; }

struct Fake { void** vt; long refs; unsigned char rest[0x400]; };
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static bool Vec(const unsigned char* p, float x, float y, float z) { const float* f = (const float*)p; return Near(f[0], x) && Near(f[1], y) && Near(f[2], z); }
static bool Col(const unsigned char* p, int r, int g, int b, int a) { return p[2] == r && p[1] == g && p[0] == b && p[3] == a; }
static bool Line(const Prim& p, float sx, float sy, float sz, float ex, float ey, float ez, int r, int g, int b, int a, unsigned long flag)
{
    const unsigned char* q = p.bytes;
    return p.kind == 1 && p.flag == flag && *(const unsigned long*)q == 6 && *(const unsigned long*)(q + 4) == 0 && Vec(q + 0xc, sx, sy, sz)
        && Vec(q + 0x18, ex, ey, ez) && Col(q + 0x24, r, g, b, a) && Near(*(const float*)(q + 0x28), 0.05f) && *(const unsigned long*)(q + 0x2c) == 0
        && *(const unsigned long*)(q + 0x30) == 0 && *(const unsigned long*)(q + 0x34) == 0 && q[0x38] == 1;
}
static bool Text(const Prim& p, int kind, float x, float y, float z, float scale, unsigned long text, int r, int g, int b, int a, bool in3d)
{
    const unsigned char* q = p.bytes;
    return p.kind == kind && *(const unsigned long*)q == 5 && *(const unsigned long*)(q + 4) == 0 && Vec(q + 0xc, x, y, z) && Near(*(const float*)(q + 0x18), scale)
        && Col(q + 0x1c, r, g, b, a) && *(const unsigned long*)(q + 0x20) == text && *(void* const*)(q + 0x24) == (void*)0x5F0A7
        && q[0x28] == (in3d ? 1 : 0);
}

struct World {
    unsigned char thing[0x600]; Node sentinel; unsigned char info[0x74]; Fake engine, tc, tracker, inside, def, home, work, brain;
    void* eng_vt[32]; void* tc_vt[16]; void* def_vt[16]; long comps[4][2]; unsigned long text1, text2;
};
static World w;
static void Setup(bool base)
{
    memset(&w, 0, sizeof w); memset(w.thing, 0x00, sizeof w.thing);
    g_npool = 0; g_loglen = 0; g_log[0] = 0; g_nprims = 0; g_wlive = g_wmade = g_ccdtor = g_ccmade = 0; g_tcdraw = 0; g_defdestroy = 0;
    g_names = g_nav = g_percept = g_linked = false; g_brain_text1 = g_brain_text2 = 0; g_base_ret = base;
    w.sentinel.next = w.sentinel.prev = &w.sentinel;
    *(Node**)(w.thing + 0x194) = &w.sentinel;
    *(unsigned __int64*)(w.thing + 0x18) = 0x1122334455667788ULL;
    *(unsigned short*)(w.thing + 0x8c) = 0x2A;
    float* pos = (float*)(w.thing + 0x300); pos[0] = 10.0f; pos[1] = 20.0f; pos[2] = 30.0f;
    for (int i = 0; i < 32; ++i) w.eng_vt[i] = 0; w.eng_vt[0x50 / 4] = (void*)SAdd; w.eng_vt[0x5c / 4] = (void*)SSetPersistent;
    w.engine.vt = w.eng_vt; g_engine = &w.engine;
    *(float*)(w.info + 4) = 0.375f; *(void**)(w.info + 0x34) = &w.engine;
    *(void**)(g_component + 0x16144) = (void*)0x5F0A7;
    float* hp = (float*)(w.home.rest + 0x300 - 8); hp[0] = 1.0f; hp[1] = 2.0f; hp[2] = 3.0f;
    float* wp = (float*)(w.work.rest + 0x300 - 8); wp[0] = 4.0f; wp[1] = 5.0f; wp[2] = 6.0f;
    w.tc_vt[0x2c / 4] = (void*)STcDraw; w.tc.vt = w.tc_vt;
    w.def_vt[1] = (void*)SDefDestroy; w.def_vt[0x28 / 4] = (void*)SInstName; w.def.vt = w.def_vt; w.def.refs = 2;
    *(void**)(w.tracker.rest + 0xc - 8) = &w.inside;            // tracker +0x0C: the thing inside (intelligent pointer)
    *(void**)(w.inside.rest + 0x70 - 8) = &w.def;               // thing +0x70: PBaseDef
    *(void**)(w.thing + 0x64) = (void*)0x6A0;                   // PTCGraphicAppearance (only passed as this)
    w.text1 = 0x0011; w.text2 = 0x0022;
    g_self = w.thing; g_info = w.info;
}
static void Comps(bool percept, bool tracker)
{
    int n = 0;
    if (percept) { w.comps[n][0] = 0x34; w.comps[n][1] = (long)&w.tc; ++n; *(unsigned long*)(w.thing + 0x24) |= 0x100000; }
    if (tracker) { w.comps[n][0] = 0x75; w.comps[n][1] = (long)&w.tracker; ++n; *(unsigned long*)(w.thing + 0x2c) |= 0x200000; }
    *(void**)(w.thing + 0x44) = &w.comps[0][0]; *(void**)(w.thing + 0x48) = &w.comps[n][0];
}
static bool Run(const char* name, bool expect_ok) { printf("%-44s %s\n", name, expect_ok ? "ok" : "FAIL"); if (!expect_ok) printf("   log: %s\n", g_log); return expect_ok; }

int main()
{
    bool ok = true, r;
    Setup(false); g_names = g_nav = g_percept = g_linked = true; Comps(true, true);
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("base declines: nothing else", !r && !strcmp(g_log, "base;") && g_nprims == 0);

    Setup(true); r = RowDraw(w.thing, 0, w.info);
    ok &= Run("all switches off", !r && !strcmp(g_log, "base;") && g_nprims == 0);

    Setup(true); g_names = true; r = RowDraw(w.thing, 0, w.info);
    ok &= Run("names, no brain", !r && g_nprims == 2 && g_defidx_seen == 0x2A
        && Line(g_prims[0], 10, 20, 30, 10, 20, 32, 0xff, 0xff, 0x78, 0xff, 0x40) && g_prims[0].handle == &g_pool[0].vt
        && Text(g_prims[1], 1, 10, 20, 32, 0.8f, 0x100000 + ((0xC000 + 0x2A + 1000) & 0xFFFF), 0xff, 0xff, 0x78, 0xff, false)
        && g_prims[1].handle == &g_pool[1].vt && g_prims[1].flag == 0 && g_ccmade == 1 && g_ccdtor == 1 && g_wlive == 0);

    Setup(true); g_names = true; *(void**)(w.thing + 0x1a8) = &w.brain; g_brain_text1 = &w.text1; g_brain_text2 = &w.text2;
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("names, brain with both texts", !r && g_nprims == 4 && Text(g_prims[2], 1, 10, 20, 32.3f, 0.8f, 0x100011, 0xff, 0xff, 0x78, 0xff, false)
        && Text(g_prims[3], 1, 10, 20, 32.6f, 0.8f, 0x100022, 0xff, 0xff, 0x78, 0xff, false) && g_prims[3].handle == &g_pool[3].vt && g_wlive == 0);

    Setup(true); g_names = true; *(void**)(w.thing + 0x1a8) = &w.brain; g_brain_text2 = &w.text2;
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("names, brain with the name only", !r && g_nprims == 3 && Text(g_prims[2], 1, 10, 20, 32.3f, 0.8f, 0x100022, 0xff, 0xff, 0x78, 0xff, false)
        && strstr(g_log, "brain1;brain2;") != 0 && g_wlive == 0);

    Setup(true); g_linked = true; *(void**)(w.thing + 0x168) = &w.home; *(void**)(w.thing + 0x170) = &w.home;
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("buildings, home == work", !r && g_nprims == 1 && Line(g_prims[0], 10, 20, 31.5f, 1, 2, 4.5f, 0x7f, 0x7f, 0x7f, 0xff, 0));

    Setup(true); g_linked = true; *(void**)(w.thing + 0x168) = &w.home; *(void**)(w.thing + 0x170) = &w.work;
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("buildings, home and work", !r && g_nprims == 2 && Line(g_prims[0], 10, 20, 31.5f, 1, 2, 4.5f, 0xff, 0xff, 0xff, 0xff, 0)
        && Line(g_prims[1], 10, 20, 31.5f, 4, 5, 7.5f, 0, 0, 0, 0xff, 0));

    Setup(true); g_linked = true; *(void**)(w.thing + 0x170) = &w.work;
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("buildings, work only", !r && g_nprims == 1 && Line(g_prims[0], 10, 20, 31.5f, 4, 5, 7.5f, 0, 0, 0, 0xff, 0));

    Setup(true); g_linked = true; r = RowDraw(w.thing, 0, w.info);
    ok &= Run("buildings, none", !r && g_nprims == 0);

    Setup(true); g_percept = true; Comps(true, false); r = RowDraw(w.thing, 0, w.info);
    ok &= Run("perception debug with the TC", !r && g_tcdraw == 1 && g_tcdraw_info == w.info && g_nprims == 0);

    Setup(true); g_percept = true; Comps(false, false); r = RowDraw(w.thing, 0, w.info);
    ok &= Run("perception debug without the TC", !r && g_tcdraw == 0);

    Setup(true); g_nav = true; Comps(false, false); r = RowDraw(w.thing, 0, w.info);
    ok &= Run("navigation level, no tracker", !r && g_nprims == 1 && g_focal_t == 0.375f && g_navpos_seen == w.thing + 0x300
        && Text(g_prims[0], 2, 7, 8, 9.7f, 1.0f, 0x200003, 0xff, 0xff, 0, 0xff, true) && g_wlive == 0);

    Setup(true); g_nav = true; Comps(false, true); r = RowDraw(w.thing, 0, w.info);
    ok &= Run("navigation level, tracker with a live thing", !r && g_nprims == 2 && Text(g_prims[0], 2, 7, 8, 9.7f, 1.0f, 0x200003, 0xff, 0xff, 0, 0xff, true)
        && Text(g_prims[1], 2, 7, 8, 9.9f, 0.08f, 0x300077, 0xff, 0xff, 0, 0xff, true) && w.def.refs == 2 && g_defdestroy == 0 && g_wlive == 0);

    Setup(true); g_nav = true; Comps(false, true); w.inside.rest[0x91 - 8] = 1; r = RowDraw(w.thing, 0, w.info);
    ok &= Run("navigation level, tracker with a dead thing", !r && g_nprims == 1 && w.def.refs == 2 && g_wlive == 0);

    Setup(true); g_nav = true; Comps(false, true); *(void**)(w.tracker.rest + 0xc - 8) = 0; r = RowDraw(w.thing, 0, w.info);
    ok &= Run("navigation level, empty tracker", !r && g_nprims == 1 && g_wlive == 0);

    Setup(true); g_nav = true; Comps(false, true); w.def.refs = 0; r = RowDraw(w.thing, 0, w.info);
    ok &= Run("unheld def: each temporary reference destroys it", !r && g_nprims == 2 && g_defdestroy == 2 && w.def.refs == 0);

    Setup(true); g_names = g_nav = g_percept = g_linked = true; Comps(true, true); *(void**)(w.thing + 0x168) = &w.home;
    r = RowDraw(w.thing, 0, w.info);
    ok &= Run("everything on", !r && g_nprims == 5 && g_tcdraw == 1 && g_wlive == 0 && g_ccdtor == 1);

    printf(ok ? "00833c00_TEST PASS\n" : "00833c00_TEST FAIL\n");
    return ok ? 0 : 1;
}