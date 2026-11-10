// CEngineMap::PeekLandscapeHeight(long x, long y) const -- the quantised landscape height of a cell, following the cell into the neighbouring map that holds it, else the clamped cell's; 0 without a game map
// Grade: DIFFER(352v356) -- one block placement: the reload block `mov edi,[x] ; jmp` for the inline GetMap's x<0 / y<0 exits sits after the clamp's `x = 0 ; jmp` in ours, before the other exits' reload block in retail (retail's two jl's are long, hence 4 bytes); every instruction, register and slot otherwise retail's Chunk 1013c: same lever as 0x00B43720 (see its differ header for the probes).
// Leaf: ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B439C0, 356 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z; the private inline ?QuantiseLandscapeHeight@CEngineMap@@ABEMM@Z (FableWin) snaps to 1/128 through GFFloatToLongNear.
// Callees: the game map's CIDrawMap::DrawBlockGetSizeZAt(unsigned long, unsigned long, float) const (virtual slot 5, `call [eax+14h]`); the engine world map 0x01436E8C
//   (g_pEngineWorldMap, the landed CEngine forwarders' spelling) through the inline CEngineWorldMap::GetMap(x, y) const (FableWin.pdb,
//   opt-in CEngineWorldMap GETMAPXY); the CEngineMap inline accessors PeekWidth / PeekHeight / PeekWorldX / PeekWorldY /
//   MapHasDataAtPoint (FableWin.pdb, opt-in CEngineMap PEEK). Layout: engine/CEngineMap.h (GameMap +0x08, MapWidth / MapHeight
//   +0x5C / +0x5E, WorldPosX / WorldPosY +0x60 / +0x62).
// Shape: a cell outside this map (or a map without its game map) is looked up in the neighbouring map that holds it (the self tail
//   call VC7.1 turns into a loop: `mov ecx,eax ; jmp` back to the top), else clamped to this map's edge; one call site. The outer
//   test is the `||` chain (its 0.75 then-frequency gets the inline GetMap expanded; `!MapHasDataAtPoint(x, y)` leaves it a call).
// Retail-asm: 0x00B43B00 `fistp dword ptr [esp+18h]` (after 0x00B43AFC `fld dword ptr [esp+1Ch]`) -- the shared engine/GFFloatToLongNear.h helper expanded in QuantiseLandscapeHeight.
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C (landed spelling, CEngine forwarders 0x00B23DB0..)

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
