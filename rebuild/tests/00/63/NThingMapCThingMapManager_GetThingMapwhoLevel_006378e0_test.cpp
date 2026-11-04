// Behaviour test for NThingMap::CThingMapManager::GetThingMapwhoLevel (0x006378E0): the diameter (2 * radius) is floored with
// GFFloatToLongFloor; a diameter at or beyond the largest cell size takes the last level, otherwise the first level whose cell size
// exceeds it.
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
struct SMapwhoLevel { long CellSize; long _pad[3]; };
static SMapwhoLevel g_MapwhoLevels[4] = { { 4 }, { 16 }, { 64 }, { 256 } };
static long g_NoMapwhoLevels = 4;
static long g_MaxMapwhoCellSize = 256;
static unsigned char GetThingMapwhoLevel(float radius)
{
    long size = GFFloatToLongFloor(radius + radius);
    if (size >= g_MaxMapwhoCellSize)
        return (unsigned char)(g_NoMapwhoLevels - 1);
    unsigned char level = 0;
    for (SMapwhoLevel* info = g_MapwhoLevels; size >= info->CellSize; ++info)
        ++level;
    return level;
}
int main()
{
    bool ok = GetThingMapwhoLevel(0.5f) == 0 && GetThingMapwhoLevel(1.9f) == 0 && GetThingMapwhoLevel(2.0f) == 1
        && GetThingMapwhoLevel(7.9f) == 1 && GetThingMapwhoLevel(8.0f) == 2 && GetThingMapwhoLevel(127.9f) == 3
        && GetThingMapwhoLevel(500.0f) == 3;
    printf(ok ? "006378e0_TEST PASS\n" : "006378e0_TEST FAIL\n");
    return ok ? 0 : 1;
}