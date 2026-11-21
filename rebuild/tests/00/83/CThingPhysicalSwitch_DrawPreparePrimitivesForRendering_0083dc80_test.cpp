// Behaviour test for CThingPhysicalSwitch::DrawPreparePrimitivesForRendering (0x0083DC80): links the real row object (its CEnginePrimitiveStaticMesh ctor
// COMDAT included). CThingPhysical::DrawPreparePrimitivesForRendering and ?GetCurrentGraphic@CThingPhysicalSwitch@@IBEABVCEngineGraphic@@XZ are /alternatename stand-ins that log;
// the physics TC (GetRHSet(float), slot +0x11C), the engine (AddPrimitive, slot +0x50) and the mesh effect (Destroy, slot +4) are fake
// objects with hand-built vtables. When the base returns false the row returns false and calls nothing else. Otherwise it fetches the
// graphic once, builds one static-mesh primitive (type 8, interpolation set 0, mesh = graphic BankIndex, position OldPosition +
// (Position - OldPosition) * t with t = info.Interpolate.Current.WFInterpolate, the RH set GetRHSet(t) returned, size = RenderSizeX *
// Scale, fade 0, null mesh effect, zero theme channels, white tint, alpha 0xff, lit, bullet-time cloth), hands it to info.Engine once,
// releases the temporary's mesh effect (Destroy only when the count drops to 0) and returns true. Neither `this` nor `info` changes.
#include <stdio.h>
#include <string.h>
#include <math.h>
extern "C" bool __fastcall RowDraw(void* self, int, const void* info);
#pragma comment(linker, "/alternatename:@RowDraw@12=?DrawPreparePrimitivesForRendering@CThingPhysicalSwitch@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z")
static int g_base, g_graph, g_rh, g_add, g_destroy; static bool g_base_ret; static void* g_base_this; static const void* g_base_info;
static void* g_graph_this; static void* g_rh_this; static float g_rh_t; static void* g_add_this; static unsigned char g_prim[0x4c];
static void* g_destroy_this; static void* g_effect_to_set; static unsigned char g_graphic[0x100];
extern "C" bool __fastcall StubBase(void* self, int, const void* info) { ++g_base; g_base_this = self; g_base_info = info; return g_base_ret; }
extern "C" const void* __fastcall StubGraphic(void* self, int) { ++g_graph; g_graph_this = self; return g_graphic - 0; }
#pragma comment(linker, "/alternatename:?DrawPreparePrimitivesForRendering@CThingPhysical@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z=@StubBase@12")
#pragma comment(linker, "/alternatename:?GetCurrentGraphic@CThingPhysicalSwitch@@IBEABVCEngineGraphic@@XZ=@StubGraphic@8")
static float RH(int i, float t) { return (float)(i + 1) * 1.5f + t * 10.0f; }
extern "C" void* __fastcall StubRH(void* self, int, void* ret, float t)
{ ++g_rh; g_rh_this = self; g_rh_t = t; float* r = (float*)ret; for (int i = 0; i < 6; ++i) r[i] = RH(i, t); return ret; }
extern "C" void __fastcall StubAdd(void* self, int, void* prim)
{ ++g_add; g_add_this = self; memcpy(g_prim, prim, sizeof g_prim); if (g_effect_to_set) *(void**)((char*)prim + 0x3c) = g_effect_to_set; }
extern "C" void __fastcall StubDestroy(void* self, int) { ++g_destroy; g_destroy_this = self; }
struct Fake { void** vt; long count; unsigned char rest[0x40]; };

static bool Run(const char* name, bool base_ret, float t, float scale, int effect_count)
{
    static void* tc_vt[0x48]; static void* eng_vt[0x18]; static void* eff_vt[2];
    for (int i = 0; i < 0x48; ++i) tc_vt[i] = 0; tc_vt[0x11c / 4] = (void*)StubRH;
    for (int i = 0; i < 0x18; ++i) eng_vt[i] = 0; eng_vt[0x50 / 4] = (void*)StubAdd;
    eff_vt[0] = 0; eff_vt[1] = (void*)StubDestroy;
    Fake tc; memset(&tc, 0x5a, sizeof tc); tc.vt = tc_vt;
    float P[3] = { 10.0f, -4.0f, 2.5f }, O[3] = { 6.0f, 1.0f, -3.0f };
    memcpy((char*)&tc + 0x0c, P, 12); memcpy((char*)&tc + 0x18, O, 12);
    Fake eng; memset(&eng, 0x6b, sizeof eng); eng.vt = eng_vt;
    Fake eff; memset(&eff, 0, sizeof eff); eff.vt = eff_vt; eff.count = effect_count;
    unsigned char thing[0x200]; memset(thing, 0x33, sizeof thing);
    *(unsigned __int64*)(thing + 0x18) = 0x1122334455667788ULL; *(void**)(thing + 0x60) = &tc; *(float*)(thing + 0x9c) = scale;
    unsigned char info[0x74]; memset(info, 0x44, sizeof info); *(float*)(info + 4) = t; *(void**)(info + 0x34) = &eng;
    memset(g_graphic, 0x77, sizeof g_graphic); *(unsigned long*)(g_graphic + 0) = 0x2a5; *(float*)(g_graphic + 8) = 3.25f;
    unsigned char thing0[0x200], info0[0x74], tc0[sizeof tc]; memcpy(thing0, thing, sizeof thing); memcpy(info0, info, sizeof info); memcpy(tc0, &tc, sizeof tc);
    g_base = g_graph = g_rh = g_add = g_destroy = 0; g_base_ret = base_ret; g_base_this = g_graph_this = g_rh_this = g_add_this = g_destroy_this = 0;
    g_base_info = 0; g_rh_t = -99.0f; memset(g_prim, 0xcd, sizeof g_prim); g_effect_to_set = effect_count ? (void*)&eff : 0;
    bool r = RowDraw(thing, 0, info);
    bool ok = !memcmp(thing0, thing, sizeof thing) && !memcmp(info0, info, sizeof info) && !memcmp(tc0, &tc, sizeof tc)
              && g_base == 1 && g_base_this == thing && g_base_info == info;
    if (!base_ret) {
        ok = ok && !r && g_graph == 0 && g_rh == 0 && g_add == 0 && g_destroy == 0;
    } else {
        ok = ok && r && g_graph == 1 && g_graph_this == thing && g_rh == 1 && g_rh_this == &tc && fabs(g_rh_t - t) == 0.0f && g_add == 1 && g_add_this == &eng;
        const unsigned char* p = g_prim;
        ok = ok && *(const unsigned long*)(p + 0) == 8 && *(const unsigned long*)(p + 4) == 0 && p[8] == 0 && p[9] == 0 && p[10] == 0;
        ok = ok && *(const unsigned long*)(p + 0x0c) == 0x2a5;
        for (int i = 0; i < 3; ++i) { float want = (P[i] - O[i]) * t + O[i]; if (fabs(*(const float*)(p + 0x10 + 4 * i) - want) > 1e-4f) { ok = false; printf("   pos %d %f want %f\n", i, *(const float*)(p + 0x10 + 4 * i), want); } }
        ok = ok && fabs(*(const float*)(p + 0x1c) - 3.25f * scale) <= 1e-5f * (1.0f + fabs(3.25f * scale)) && *(const float*)(p + 0x20) == 0.0f;
        for (int i = 0; i < 6; ++i) { float want = RH(i, t); ok = ok && *(const float*)(p + 0x24 + 4 * i) == want; }
        ok = ok && *(void* const*)(p + 0x3c) == 0 && p[0x40] == 0 && p[0x41] == 0 && p[0x42] == 0;
        ok = ok && p[0x44] == 0xff && p[0x45] == 0xff && p[0x46] == 0xff && p[0x47] == 0xff && p[0x48] == 0xff && (p[0x49] & 1) == 1 && p[0x4a] == 1;
        if (effect_count == 1) ok = ok && g_destroy == 1 && g_destroy_this == &eff && eff.count == 0;
        else if (effect_count > 1) ok = ok && g_destroy == 0 && eff.count == effect_count - 1;
        else ok = ok && g_destroy == 0;
    }
    printf("%-36s ret %d base %d graph %d rh %d add %d destroy %d %s\n", name, (int)r, g_base, g_graph, g_rh, g_add, g_destroy, ok ? "ok" : "FAIL");
    return ok;
}

int main()
{
    bool ok = true;
    ok &= Run("base declines", false, 0.5f, 1.0f, 0);
    ok &= Run("t 0.5, scale 1", true, 0.5f, 1.0f, 0);
    ok &= Run("t 0 (old position)", true, 0.0f, 2.0f, 0);
    ok &= Run("t 1 (current position)", true, 1.0f, 0.75f, 0);
    ok &= Run("t 0.3, scale 1.6", true, 0.3f, 1.6f, 0);
    ok &= Run("effect set, last reference -> destroy", true, 0.25f, 1.0f, 1);
    ok &= Run("effect set, shared -> no destroy", true, 0.8f, 1.2f, 3);
    printf(ok ? "0083dc80_TEST PASS\n" : "0083dc80_TEST FAIL\n");
    return ok ? 0 : 1;
}