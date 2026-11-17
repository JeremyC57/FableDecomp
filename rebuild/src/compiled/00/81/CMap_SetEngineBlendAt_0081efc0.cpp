// CMap::SetEngineBlendAt(long x, long y, unsigned char blend, unsigned long index, EThemePaintType PaintType) -- set one blend weight of
// a map cell: paint type 0 stores it in the height-map cell's engine blend layer, paint type 1 in the atmos blend layer of the game-map
// cell that holds the height-map position (game map units are height map units / 4).
// Leaf: ?SetEngineBlendAt@CMap@@IAEXJJEKW4EThemePaintType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081EFC0, 138 bytes, `ret 14h`.
// Name: Ego_r.pdb 0x00679BD0 / FableWin.pdb 0x02237F90 ?SetEngineBlendAt@CMap@@IAEXJJEKW4EThemePaintType@@@Z (protected); Ego_d.pdb
//   parameters x, y, blend, index, PaintType and the block local gpos (C2DCoordI).
//   NF81 (round 7, q10977): re-authored on engine/CMap.h; the July row (candidates/world_map_theme.h with a local storage cast model)
//   never byte-matched (DIFFER 144 v 138).
// Callees: none out of line; inline: CHeightMap::GetCell, CHeightMapCell::SetEngineBlend, CMap::HeightMapUnitToGameMapUnit,
//   CGameMap::GetCell, CGameMapCell::SetAtmosBlend.
// Layout: CMap GameMap +0x1A78, HeightMap +0x1A7C; CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208 (cell 12 bytes, engine blends
//   +7); CGameMap Cells +0x04, SizeX +0x1C, SizeY +0x20 (cell 7 bytes, atmos blends +3).
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_LOADFROMFILE          // CMap::HeightMapUnitToGameMapUnit
#define FABLE_RETAIL_CMAP_ENGINETHEMEAT         // NF81 overlay: EThemePaintType and the protected per-cell theme accessors
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CGAMEMAP
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
class CASuspendableProcess;
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
#include "engine/CGameMap.h"


void CMap::SetEngineBlendAt(long x, long y, unsigned char blend, unsigned long index, EThemePaintType PaintType)
{
    if (PaintType == THEME_PAINT_ENGINE)
        HeightMap->GetCell(C2DCoordI(x, y)).SetEngineBlend(blend, index);
    else if (PaintType == THEME_PAINT_ATMOS)
    {
        C2DCoordI gpos(HeightMapUnitToGameMapUnit(x), HeightMapUnitToGameMapUnit(y));
        GameMap->GetCell(gpos).SetAtmosBlend(index, blend);
    }
}
