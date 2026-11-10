// CEngineMap::PeekInterpolatedWaterHeight(long x, long y, long radius) const -- the mean water surface height (landscape height + water depth) over the wet cells of the square of the given radius round a cell; when the cell itself is dry the square is re-centred on the first wet cell found in it, 0 when there is none
// Grade: DIFFER(402v777) -- inlining: retail expands the same-TU CEngineMap::PeekLandscapeHeight (0x00B439C0) into the averaging loop, VC7.1 calls it here (its cost is over the /Ob1 budget even as `inline`); with the body forced inline the rest is 26 hunks (retail keeps the map width in [esp+2Ch], frame 0x24 v 0x20)
// Leaf: ?PeekInterpolatedWaterHeight@CEngineMap@@QBEMJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B41770, 777 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb ?PeekInterpolatedWaterHeight@CEngineMap@@QBEMJJJ@Z; callees ?PeekHasWater@CEngineMap@@QBE_NJJ@Z
//   0x00B412A0 (the centre test), the inline PeekHasWaterFast (search), ?PeekWaterDepth@CEngineMap@@QBEMJJ@Z 0x00B40BD0 and
//   ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z 0x00B439C0 (expanded in retail; its body is repeated here because it is the same TU).
// Retail-asm: 0x00B419F8 `fistp dword ptr [esp+30h]` (after 0x00B419F4 `fld dword ptr [esp+2Ch]`) -- GFFloatToLongNear in the expanded PeekLandscapeHeight (QuantiseLandscapeHeight); the out-of-line copy's is 0x00B43B00.
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

float CEngineMap::PeekLandscapeHeight(long x, long y) const
{
    if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
        const CEngineWorldMap* worldMap = g_pEngineWorldMap;
        long worldX = PeekWorldX() - worldMap->MapOffsetX + x;
        long worldY = PeekWorldY() - worldMap->MapOffsetY + y;
        const CEngineMap* map = worldMap->GetMap(worldX, worldY);
        if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
            return map->PeekLandscapeHeight(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y);
        if (x < 0)
            x = 0;
        else if (x > PeekWidth() - 1)
            x = PeekWidth() - 1;
        if (y < 0)
            y = 0;
        else if (y > PeekHeight() - 1)
            y = PeekHeight() - 1;
    }
    if (GameMap)
        return QuantiseLandscapeHeight(GameMap->DrawBlockGetSizeZAt(x, y, 1.0f));
    return 0.0f;
}

float CEngineMap::PeekInterpolatedWaterHeight(long x, long y, long radius) const
{
    long minX = x - radius;
    long minY = y - radius;
    long maxX = x + radius;
    long maxY = y + radius;
    if (!PeekHasWater(x, y)) {
        bool found = false;
        for (long i = minX; i <= maxX && !found; ++i) {
            for (long j = minY; j <= maxY && !found; ++j) {
                if (PeekHasWaterFast(i, j)) {
                    minX = i - radius;
                    minY = j - radius;
                    maxX = i + radius;
                    maxY = j + radius;
                    found = true;
                }
            }
        }
        if (!found)
            return 0.0f;
    }
    float total = 0.0f;
    float count = 0.0f;
    for (long i = minX; i <= maxX; ++i) {
        for (long j = minY; j <= maxY; ++j) {
            float depth = PeekWaterDepth(i, j);
            if (depth > 0.001f) {
                total += PeekLandscapeHeight(i, j) + depth;
                count += 1.0f;
            }
        }
    }
    return count > 0.0f ? total / count : 0.0f;
}
