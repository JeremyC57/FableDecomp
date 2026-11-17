// NF81 (round 7): the row moved onto engine/CWorldMap.h + engine/CMap.h, so it defines the PDB class spelling ABVC2DCoordI@@ and
//   calls the protected CMap theme member (both PDBs). This test keeps the legacy header with the landed opt-in
//   FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED and the NF81 opt-in FABLE_RETAIL_WORLDMAPTHEME_C2DCOORDI_SHARED (engine/C2DCoordI.h,
//   members X / Y), so its stubs and its call carry the same names as the row.
#define FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED
#define FABLE_RETAIL_WORLDMAPTHEME_C2DCOORDI_SHARED
#include <stdio.h>
#include <string.h>
#include "candidates/world_map_theme.h"

static char g_calls[12];
static int g_callCount;
static long g_mapNumber;
static CMap g_map;
static int g_peekSawNumber;
static long g_getX;
static long g_getY;
static unsigned long g_getThemeId;
static EThemePaintType g_getPaintType;

static void Record(char call)
{
    g_calls[g_callCount++] = call;
    g_calls[g_callCount] = 0;
}

static long FABLE_FASTCALL GetMapNumberAtShim(
    const CWorldMap*, void*, const C2DCoordI&)
{
    Record('R');
    return g_mapNumber;
}

static const CMap* FABLE_FASTCALL PeekMapShim(
    const CWorldMap*, void*, long mapNumber)
{
    Record('G');
    g_peekSawNumber = mapNumber == g_mapNumber;
    return &g_map;
}

C2DCoordI CWorldMap::ConvertWorldMapCoordsToLocalCoords(
    long mapNumber, const C2DCoordI& worldMapCoord) const
{
    Record('C');
    C2DCoordI result;
    result.X = worldMapCoord.X - mapNumber;
    result.Y = worldMapCoord.Y - mapNumber * 2;
    return result;
}

unsigned char CMap::GetEngineBlendAt(
    long x,
    long y,
    unsigned long themeId,
    EThemePaintType paintType) const
{
    Record('S');
    g_getX = x;
    g_getY = y;
    g_getThemeId = themeId;
    g_getPaintType = paintType;
    return 213;
}

int main()
{
    void* vtable[26];
    memset(vtable, 0, sizeof(vtable));
    vtable[0x44 / 4] = (void*)&GetMapNumberAtShim;
    vtable[0x4C / 4] = (void*)&PeekMapShim;
    struct FakeWorldMap { void** vtable; } fake;
    fake.vtable = vtable;
    const CWorldMap* worldMap = reinterpret_cast<const CWorldMap*>(&fake);
    C2DCoordI worldCoord;
    worldCoord.X = 37;
    worldCoord.Y = 81;

    g_callCount = 0;
    g_calls[0] = 0;
    g_mapNumber = 0;
    if (worldMap->GetThemeBlendAtBlock(worldCoord, 12, EThemePaintType_Replace) != 0) return 1;
    if (strcmp(g_calls, "R") != 0) return 2;

    g_callCount = 0;
    g_calls[0] = 0;
    g_mapNumber = 5;
    g_peekSawNumber = 0;
    if (worldMap->GetThemeBlendAtBlock(worldCoord, 12, EThemePaintType_Replace) != 213) return 3;
    if (strcmp(g_calls, "RCGS") != 0) return 4;
    if (!g_peekSawNumber || g_getX != 32 || g_getY != 71) return 5;
    if (g_getThemeId != 12 || g_getPaintType != EThemePaintType_Replace) return 6;
    printf("WORLD_MAP_GET_THEME_BLEND_TEST PASS\n");
    return 0;
}
