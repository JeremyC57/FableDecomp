// CEngineWorldMap::UnlinkMapFromWorld(CEngineMap* map, long unused) -- clear (to -1) every world-grid cell (32-unit cells) the map's world rectangle covers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3EE80, 119 bytes.
// Callees: none.
// Layout: `this` is engine/CEngineWorldMap.h (Map +0x0c as short*, MapArrayWidth +0x10, MapOffsetX +0x18, MapOffsetY +0x1c --
//   the retail offsets equal the PDB's this low). CEngineMap is modelled locally: its retail MapWidth / MapHeight / WorldPosX /
//   WorldPosY (unsigned short) sit at +0x5c/+0x5e/+0x60/+0x62, 4 below the PDB (the retail STLport list iterator before them
//   is narrower), so engine/CEngineMap.h does not fit.
// Shape: the four cell bounds are signed shifts (`sar 5`) of long differences of zero-extended words; the row index is
//   spilled to the dead argument slot across the inner loop, the row base `MapArrayWidth * y` is re-multiplied for every
//   cell (both members re-read after each store through `short*`).
#include "engine/CEngineWorldMap.h"

struct CEngineMap_00b3ee80 {
    unsigned char  _pad0[0x5c];
    unsigned short MapWidth;                     // +0x5c
    unsigned short MapHeight;                    // +0x5e
    unsigned short WorldPosX;                    // +0x60
    unsigned short WorldPosY;                    // +0x62
};

struct CEngineWorldMapUnlink : CEngineWorldMap {
    void UnlinkMapFromWorld(CEngineMap_00b3ee80* map, long unused);
};

void CEngineWorldMapUnlink::UnlinkMapFromWorld(CEngineMap_00b3ee80* map, long unused)
{
    long startX = (map->WorldPosX - MapOffsetX) >> 5;
    long startY = (map->WorldPosY - MapOffsetY) >> 5;
    long endX = (map->WorldPosX + map->MapWidth - MapOffsetX) >> 5;
    long endY = (map->WorldPosY + map->MapHeight - MapOffsetY) >> 5;
    for (long y = startY; y < endY; ++y)
        for (long x = startX; x < endX; ++x)
            static_cast<short*>(Map)[MapArrayWidth * y + x] = -1;
}