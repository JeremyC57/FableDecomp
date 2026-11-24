// Original Ego_d variable/parameter names, and original same-TU landscape member/helper.
// Leaf: ?PeekInterpolatedWaterHeight@CEngineMap@@QBEMJJJ@Z
// Flags: /d2inlT400
// Inline-budget: retail expands the original same-TU PeekLandscapeHeight; T301 calls it and T400 expands it exactly.
// Retail-asm: 00B419F4 fld dword ptr [esp+2Ch],00B419F8 fistp dword ptr [esp+30h]; shared GFFloatToLongNear.
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }

float CEngineMap::PeekLandscapeHeight(long xp, long yp) const
{
    if (!MapHasDataAtPoint(xp,yp)) {
        CEngineMap* map = NEngineInternal::GPEngineWorldMap->GetMap(
            PeekWorldX() + xp,
            PeekWorldY() + yp);
        if (map && map->MapHasDataAtPoint(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY()))
            return map->PeekLandscapeHeight(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY());
        if (xp < 0)
            xp = 0;
        else if (xp > PeekWidth() - 1)
            xp = PeekWidth() - 1;
        if (yp < 0)
            yp = 0;
        else if (yp > PeekHeight() - 1)
            yp = PeekHeight() - 1;
    }
    if (GameMap)
        return QuantiseLandscapeHeight(GameMap->DrawBlockGetSizeZAt(xp, yp, 1.0f));
    return 0.0f;
}

float CEngineMap::PeekInterpolatedWaterHeight(const long mapx, const long mapy, const long distance_to_look) const
{
    long min_i = mapx - distance_to_look;
    long min_j = mapy - distance_to_look;
    long max_i = mapx + distance_to_look;
    long max_j = mapy + distance_to_look;
    if (!PeekHasWater(mapx, mapy)) {
        bool found = false;
        for (long i = min_i; i <= max_i && !found; ++i) {
            for (long j = min_j; j <= max_j && !found; ++j) {
                if (PeekHasWaterFast(i, j)) {
                    min_i = i - distance_to_look;
                    min_j = j - distance_to_look;
                    max_i = i + distance_to_look;
                    max_j = j + distance_to_look;
                    found = true;
                }
            }
        }
        if (!found)
            return 0.0f;
    }
    float total_water_depth = 0.0f;
    float points_with_water = 0.0f;
    for (long i = min_i; i <= max_i; ++i) {
        for (long j = min_j; j <= max_j; ++j) {
            float water_depth = PeekWaterDepth(i, j);
            if (water_depth > 0.001f) {
                total_water_depth += PeekLandscapeHeight(i, j) + water_depth;
                points_with_water += 1.0f;
            }
        }
    }
    return points_with_water > 0.0f ? total_water_depth / points_with_water : 0.0f;
}