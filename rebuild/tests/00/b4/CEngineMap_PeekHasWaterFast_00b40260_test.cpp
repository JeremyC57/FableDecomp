// Behaviour test for row 0x00B40260 CEngineMap::PeekHasWaterFast (links the row's object): a fake map (GameMap +0x08, MapWidth /
// MapHeight +0x5C / +0x5E as unsigned short) whose draw map returns a theme id per layer, an identity definition manager and a theme
// table (+0x4B8, 0x38-byte records, WaterType +0x30). Checks each layer test (id <= 0 stops the search, a water type returns true,
// dry layers fall through), the three-layer limit and the clamp of an off-map cell (the world map has no neighbouring map).
#include <stdio.h>
#include <string.h>

class CEngineWorldMap;
class CEngineLandscapeRenderer;
class CDefinitionManager { public: long GetDefClassIndexFromGlobalIndex(long index) const; };
long CDefinitionManager::GetDefClassIndexFromGlobalIndex(long index) const { return index; }
static unsigned char g_def_obj[16];
static unsigned char g_world_obj[0x40];
static unsigned char g_renderer_obj[0x500];
CDefinitionManager* g_pEngineDefinitionManager = (CDefinitionManager*)g_def_obj;
CEngineLandscapeRenderer* g_LandscapeRenderer_01436EA8 = (CEngineLandscapeRenderer*)g_renderer_obj;
namespace NEngineInternal { CEngineWorldMap* GPEngineWorldMap = (CEngineWorldMap*)g_world_obj; }

class CEngineMap { public: bool PeekHasWaterFast(const long mapx, const long mapy) const; };

static long g_ids[4];
static long g_last_x, g_last_y, g_calls;
static long __fastcall ThemeAt(void*, void*, long x, long y, long layer)
{
    g_last_x = x; g_last_y = y; ++g_calls;
    return g_ids[layer];
}
static void* g_vtable[16];
static void* g_draw_obj[1];
static unsigned char g_themes[16][0x38];
static unsigned char g_map_obj[0x100];

static int g_fail;
static void Case(const char* name, long t0, long t1, long t2, long x, long y, bool want, long want_x, long want_calls)
{
    g_ids[0] = t0; g_ids[1] = t1; g_ids[2] = t2; g_ids[3] = 0;
    g_calls = 0;
    bool got = ((const CEngineMap*)g_map_obj)->PeekHasWaterFast(x, y);
    if (got != want || g_last_x != want_x || g_calls != want_calls) {
        printf("%s: got %d want %d (x %ld calls %ld)\n", name, got, want, g_last_x, g_calls);
        g_fail = 1;
    }
}
int main()
{
    g_vtable[6] = (void*)&ThemeAt;
    g_draw_obj[0] = g_vtable;
    memset(g_map_obj, 0, sizeof(g_map_obj));
    *(void**)(g_map_obj + 0x08) = g_draw_obj;
    *(unsigned short*)(g_map_obj + 0x5C) = 10;
    *(unsigned short*)(g_map_obj + 0x5E) = 12;
    *(unsigned char**)(g_renderer_obj + 0x4B8) = &g_themes[0][0];
    memset(g_themes, 0, sizeof(g_themes));
    *(long*)(g_themes[0] + 0x30) = 3;                 // index 0 is wet: a missing `> 0` test would report water
    *(long*)(g_themes[2] + 0x30) = 1;                 // 2 = lake
    *(long*)(g_themes[5] + 0x30) = 8;                 // 5 = ice
    // 1, 3, 4 dry
    Case("none", 0, 2, 2, 3, 4, false, 3, 1);
    Case("neg", -2, 2, 2, 3, 4, false, 3, 1);
    Case("l0", 2, 1, 1, 3, 4, true, 3, 1);
    Case("l1", 1, 5, 3, 3, 4, true, 3, 2);
    Case("l1stop", 1, 0, 2, 3, 4, false, 3, 2);
    Case("l2", 1, 3, 2, 3, 4, true, 3, 3);
    Case("l2stop", 3, 4, 0, 3, 4, false, 3, 3);
    Case("dry", 1, 3, 4, 3, 4, false, 3, 3);
    Case("clamp_hi", 2, 0, 0, 25, 4, true, 9, 1);     // off-map: no neighbour map, x clamped to MapWidth - 1
    Case("clamp_lo", 1, 2, 0, -3, 30, true, 0, 2);
    if (g_last_y != 11) g_fail = 1;
    if (g_fail) { printf("00b40260_TEST FAIL\n"); return 1; }
    printf("00b40260_TEST PASS\n");
    return 0;
}