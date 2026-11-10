// CEngineMap::PeekThemeId(long x, long y, long layer) const -- the engine theme id of a landscape cell, following the cell into the neighbouring map that holds it, else the clamped cell's; 0 without a game map
// Grade: DIFFER(320v324) -- (agent1019 regrade, unchanged) one compensation-block placement: the x<0 / y<0 GetMap exits' `mov edi,[x] ; jmp` sits after the clamp's `x = 0 ; jmp` in ours, before the other exits' reload block in retail. Measured (agent1019): a GetMap split into `if (x < 0 || y < 0) return 0; if (x >= W*32 || y >= H*32) return 0;` puts that block where retail has it (324v324) but needs /d2inlT301 to stay expanded here, and then the y reload `mov ebp,[esp+1Ch]` is hoisted above the map-null `je` (2 lines); an explicit loop instead of the tail call behaves the same.
// Leaf: ?PeekThemeId@CEngineMap@@QBEJJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B43720, 324 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb ?PeekThemeId@CEngineMap@@QBEJJJJ@Z (the catalog's "PeekLandscapeHeight" is propagated over the three Peek bodies).
// Callees: the game map's CIDrawMap::DrawGetEngineThemeAt(long, long, long) const (virtual slot 6, `call [eax+18h]`); the engine world map 0x01436E8C
//   (g_pEngineWorldMap, the landed CEngine forwarders' spelling) through the inline CEngineWorldMap::GetMap(x, y) const (FableWin.pdb,
//   opt-in CEngineWorldMap GETMAPXY); the CEngineMap inline accessors PeekWidth / PeekHeight / PeekWorldX / PeekWorldY /
//   MapHasDataAtPoint (FableWin.pdb, opt-in CEngineMap PEEK). Layout: engine/CEngineMap.h (GameMap +0x08, MapWidth / MapHeight
//   +0x5C / +0x5E, WorldPosX / WorldPosY +0x60 / +0x62).
// Shape: a cell outside this map (or a map without its game map) is looked up in the neighbouring map that holds it (the self tail
//   call VC7.1 turns into a loop: `mov ecx,eax ; jmp` back to the top), else clamped to this map's edge; one call site. The outer
//   test is the `||` chain (its 0.75 then-frequency gets the inline GetMap expanded; `!MapHasDataAtPoint(x, y)` leaves it a call).
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C (landed spelling, CEngine forwarders 0x00B23DB0..)

long CEngineMap::PeekThemeId(long x, long y, long layer) const
{
    if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
        const CEngineWorldMap* worldMap = g_pEngineWorldMap;
        long worldX = PeekWorldX() - worldMap->MapOffsetX + x;
        long worldY = PeekWorldY() - worldMap->MapOffsetY + y;
        const CEngineMap* map = worldMap->GetMap(worldX, worldY);
        if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
            return map->PeekThemeId(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
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
        return GameMap->DrawGetEngineThemeAt(x, y, layer);
    return 0;
}
