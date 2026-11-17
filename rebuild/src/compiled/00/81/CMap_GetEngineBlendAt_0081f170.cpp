// CMap::GetEngineBlendAt(long x, long y, unsigned long index, EThemePaintType PaintType) const -- read one blend weight of a map cell:
// paint type 0 gives the height-map cell's engine blend, paint type 1 the atmos blend of the game-map cell that holds the height-map
// position (CGameMapCell::GetAtmosBlend, out of line); any other paint type gives 0.
// Leaf: ?GetEngineBlendAt@CMap@@IBEEJJKW4EThemePaintType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081F170, 143 bytes, `ret 10h`.
// Name: Ego_r.pdb 0x00679D80 / FableWin.pdb 0x02238200 ?GetEngineBlendAt@CMap@@IBEEJJKW4EThemePaintType@@@Z (protected const); Ego_d.pdb
//   parameters x, y, index, PaintType and the block local gpos (C2DCoordI).
//   NF81 (round 7, q10979): re-authored on engine/CMap.h; the July row (candidates/world_map_theme.h with a local storage cast model)
//   never byte-matched (DIFFER 143 v 143).
// Callees: CGameMapCell::GetAtmosBlend 0x006B74E0 (?GetAtmosBlend@CGameMapCell@@QBEEJ@Z); inline: CHeightMap::GetCell,
//   CHeightMapCell::GetEngineBlend, CMap::HeightMapUnitToGameMapUnit, CGameMap::GetCell.
// Layout: CMap GameMap +0x1A78, HeightMap +0x1A7C; CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208 (engine blends +7);
//   CGameMap Cells +0x04, SizeX +0x1C, SizeY +0x20.
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_LOADFROMFILE          // CMap::HeightMapUnitToGameMapUnit
#define FABLE_RETAIL_CMAP_ENGINETHEMEAT         // NF81 overlay: EThemePaintType and the protected per-cell theme accessors
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CGAMEMAP
#define FABLE_RETAIL_CGAMEMAPCELL_ATMOSGETTERS  // NF81 overlay: GetAtmosTheme (inline) / GetAtmosBlend (0x006B74E0)
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
class CASuspendableProcess;
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
#include "engine/CGameMap.h"


unsigned char CMap::GetEngineBlendAt(long x, long y, unsigned long index, EThemePaintType PaintType) const
{
    if (PaintType == THEME_PAINT_ENGINE)
        return HeightMap->GetCell(C2DCoordI(x, y)).GetEngineBlend(index);
    else if (PaintType == THEME_PAINT_ATMOS)
    {
        C2DCoordI gpos(HeightMapUnitToGameMapUnit(x), HeightMapUnitToGameMapUnit(y));
        return GameMap->GetCell(gpos).GetAtmosBlend(index);
    }
    return 0;
}
