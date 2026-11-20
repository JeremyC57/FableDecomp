// Behaviour test for CWorldMap::HeightAreaChanged (0x004FEB70): links the row object (its same-TU ClipAndConvertWorldMapAreaToLocalArea,
// the inline ClipTo COMDAT and CTBaseSingleton<CEngineManager>::Get run for real) with stand-ins for the out-of-line callees. Fixture in the
// retail layout: CWorldMap vptr (slot 13 +0x34 GetDimensions, slot 20 +0x50 GetAllMapsInArea), World pointer +0x08 (CWorld: navigator
// manager pointer +0x48), Maps CArray<CMapInfo> +0x20 (0x48-byte entries: Area +0x00, CCountedPointer<CMap> +0x10).
// Checked, as one ordered event log: GetDimensions once; the area is *pworld_box or (null) the dimensions; AddChangedArea(area) on the
// singleton; GetAllMapsInArea(area, list); for each listed map in order, HeightAreaChanged(local box) on its CMap when it has one, where the
// local box is the area clipped to the map's area and shifted by its corner; UpdateAllEdges; then for each listed map in order
// UpdateNavigationMapArea(map, the area as floats). Nothing in the CWorldMap fixture is written.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Box { long TLX, TLY, BRX, BRY; };
struct BoxF { float TLX, TLY, BRX, BRY; };
struct Vec { unsigned long* start; unsigned long* finish; unsigned long* eos; };

static unsigned char g_wm[0x100], g_world[0x60], g_nav[16], g_engine[16], g_maps[6][0x48], g_cmap[6][16];
static void* g_vtbl[40];
static Box g_dims;
static unsigned long g_list[8]; static int g_listN;
static char g_log[4096]; static int g_bad;
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }

extern "C" void* g_PClass[2] = { g_engine, 0 };
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingleton@VCEngineManager@@@@0V?$CCountedPointer@VCEngineManager@@@@A=_g_PClass")

static const Box* __fastcall GetDimensions(void* self)
{
    if (self != g_wm) ++g_bad;
    Log("dims"); return &g_dims;
}
static void __fastcall GetAllMapsInArea(void* self, int, const Box* area, Vec* list)
{
    char b[96]; sprintf(b, "maps(%ld,%ld,%ld,%ld)", area->TLX, area->TLY, area->BRX, area->BRY); Log(b);
    if (self != g_wm) ++g_bad;
    list->start = (unsigned long*)malloc(sizeof(unsigned long) * (g_listN ? g_listN : 1));
    memcpy(list->start, g_list, sizeof(unsigned long) * g_listN);
    list->finish = list->start + g_listN; list->eos = list->finish;
}
extern "C" void __fastcall StubAddChangedArea(void* self, int, const Box* area)
{
    char b[96]; sprintf(b, "add(%ld,%ld,%ld,%ld)", area->TLX, area->TLY, area->BRX, area->BRY); Log(b);
    if (self != g_engine) ++g_bad;
}
#pragma comment(linker, "/alternatename:?AddChangedArea@CEngineManager@@QAEXABVC2DBoxI@@@Z=@StubAddChangedArea@12")
extern "C" void __fastcall StubMapHeightAreaChanged(void* self, int, const Box* local)
{
    int m = (int)(((unsigned char(*)[16])self) - g_cmap);
    char b[96]; sprintf(b, "map%d(%ld,%ld,%ld,%ld)", m, local->TLX, local->TLY, local->BRX, local->BRY); Log(b);
}
#pragma comment(linker, "/alternatename:?HeightAreaChanged@CMap@@QAEXPBVC2DBoxI@@@Z=@StubMapHeightAreaChanged@12")
extern "C" void __fastcall StubUpdateAllEdges(void* self) { if (self != g_wm) ++g_bad; Log("edges"); }
#pragma comment(linker, "/alternatename:?UpdateAllEdges@CWorldMap@@IAEXXZ=@StubUpdateAllEdges@4")
extern "C" void __fastcall StubUpdateNavigationMapArea(void* self, int, long map, const BoxF* area)
{
    char b[96]; sprintf(b, "nav%ld(%g,%g,%g,%g)", map, area->TLX, area->TLY, area->BRX, area->BRY); Log(b);
    if (self != g_nav) ++g_bad;
}
#pragma comment(linker, "/alternatename:?UpdateNavigationMapArea@CNavigatorManager@@QAEXJABVC2DBoxF@@@Z=@StubUpdateNavigationMapArea@16")
extern "C" void __fastcall StubNeverCalled(void*) { puts("unexpected destructor call"); exit(3); }
#pragma comment(linker, "/alternatename:??1CEngineManager@@UAE@XZ=@StubNeverCalled@4")
#pragma comment(linker, "/alternatename:??1?$CTBaseSingleton@VCEngineManager@@@@UAE@XZ=@StubNeverCalled@4")

extern "C" void __fastcall RowHeightAreaChanged(void* self, int unused, const Box* pworld_box);
#pragma comment(linker, "/alternatename:@RowHeightAreaChanged@12=?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z")

static const Box g_mapArea[6] = { { 0, 0, 64, 64 }, { 64, 0, 128, 64 }, { 0, 64, 64, 128 }, { 64, 64, 128, 128 }, { 128, 0, 192, 64 }, { -64, -64, 0, 0 } };

static long Clamp(long v, long lo, long hi) { return v < lo ? lo : (v > hi ? hi : v); }

static bool Run(const char* name, const Box* area, int n, const unsigned long* list, const bool* hasMap)
{
    memset(g_wm, 0, sizeof g_wm); memset(g_world, 0, sizeof g_world); memset(g_maps, 0, sizeof g_maps); g_log[0] = 0; g_bad = 0;
    for (int i = 0; i < 40; ++i) g_vtbl[i] = 0;
    g_vtbl[13] = (void*)GetDimensions; g_vtbl[20] = (void*)GetAllMapsInArea;
    *(void**)g_wm = g_vtbl; *(void**)(g_wm + 8) = g_world; *(void**)(g_world + 0x48) = g_nav;
    for (int i = 0; i < 6; ++i) { memcpy(g_maps[i], &g_mapArea[i], sizeof(Box)); *(void**)(g_maps[i] + 0x10) = hasMap[i] ? g_cmap[i] : 0; }
    *(void**)(g_wm + 0x20) = g_maps[0]; *(void**)(g_wm + 0x24) = g_maps[6 - 0] ; *(void**)(g_wm + 0x28) = g_maps[6 - 0];
    g_dims.TLX = -64; g_dims.TLY = -64; g_dims.BRX = 192; g_dims.BRY = 128;
    g_listN = n; memcpy(g_list, list, sizeof(unsigned long) * n);
    unsigned char wm0[sizeof g_wm]; memcpy(wm0, g_wm, sizeof wm0);
    RowHeightAreaChanged(g_wm, 0, area);
    const Box* a = area ? area : &g_dims;
    char want[4096]; want[0] = 0; char b[128];
    strcat(want, "dims;");
    sprintf(b, "add(%ld,%ld,%ld,%ld);", a->TLX, a->TLY, a->BRX, a->BRY); strcat(want, b);
    sprintf(b, "maps(%ld,%ld,%ld,%ld);", a->TLX, a->TLY, a->BRX, a->BRY); strcat(want, b);
    for (int i = 0; i < n; ++i) {
        unsigned long m = list[i]; if (!hasMap[m]) continue;
        const Box& ma = g_mapArea[m];
        Box c = { Clamp(a->TLX, ma.TLX, ma.BRX), Clamp(a->TLY, ma.TLY, ma.BRY), Clamp(a->BRX, ma.TLX, ma.BRX), Clamp(a->BRY, ma.TLY, ma.BRY) };
        sprintf(b, "map%lu(%ld,%ld,%ld,%ld);", m, c.TLX - ma.TLX, c.TLY - ma.TLY, c.BRX - ma.TLX, c.BRY - ma.TLY); strcat(want, b);
    }
    strcat(want, "edges;");
    for (int i = 0; i < n; ++i) { sprintf(b, "nav%lu(%g,%g,%g,%g);", list[i], (float)a->TLX, (float)a->TLY, (float)a->BRX, (float)a->BRY); strcat(want, b); }
    bool ok = !strcmp(want, g_log) && g_bad == 0 && !memcmp(wm0, g_wm, sizeof wm0);
    printf("%-26s %s\n", name, ok ? "ok" : "FAIL");
    if (!ok) printf("  got  %s\n  want %s\n  bad %d\n", g_log, want, g_bad);
    return ok;
}

int main()
{
    static const bool all[6] = { true, true, true, true, true, true }, some[6] = { true, false, true, false, true, true };
    static const unsigned long l1[4] = { 0, 1, 2, 3 }, l2[3] = { 3, 0, 5 }, l3[1] = { 4 };
    Box inner = { 10, 20, 100, 90 }, outside = { -100, -100, 300, 300 }, corner = { 60, 60, 70, 70 };
    bool ok = true;
    ok &= Run("null area (dimensions)", 0, 4, l1, all);
    ok &= Run("inner area, all maps", &inner, 4, l1, all);
    ok &= Run("inner area, gaps", &inner, 4, l1, some);
    ok &= Run("outside, unordered list", &outside, 3, l2, some);
    ok &= Run("corner box", &corner, 4, l1, all);
    ok &= Run("single map", &inner, 1, l3, all);
    ok &= Run("empty list", &inner, 0, l1, all);
    printf(ok ? "004feb70_TEST PASS\n" : "004feb70_TEST FAIL\n");
    return ok ? 0 : 1;
}