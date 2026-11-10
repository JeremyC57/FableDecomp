// CEngineMap::PeekThemeId(long x, long y, long layer) const -- the engine theme id of a landscape cell, following the cell into the neighbouring map that holds it, else the clamped cell's; 0 without a game map
// Grade: DIFFER(320v324) -- one block placement: the reload block `mov edi,[x] ; jmp` for the inline GetMap's x<0 / y<0 exits sits after the clamp's `x = 0 ; jmp` in ours, before the other exits' reload block in retail (retail's two jge to that block are long, hence 4 bytes). Chunk 1013c (free probes, all byte-identical or worse): 18 GetMap spellings (separate ifs, nested positive, single-return, ternary, (A&&B) negation, an inline CEngineWorldMap::MapHasDataAtPoint helper: that one stops the expansion), 9 caller shapes (named mapX/mapY, nested if (map), else around the clamp, !map || ..., clamp ternaries / >= forms / reversed order, == true), no worldMap local, MapHasDataAtPoint-first returns. Measured rule: c2 puts this compensation block after the first jump-ending block found walking forward from the clamp test (ours) where retail puts it after the loop-back jmp, i.e. walking from the GetMap test that branches to it
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
