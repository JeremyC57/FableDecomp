// CMap::GetEngineThemeAt(long x, long y, unsigned long index, EThemePaintType PaintType) const -- read one theme layer of a map cell:
// paint type 0 gives the theme definition index of the height-map cell's engine theme layer (through the map's theme palette), paint
// type 1 the atmos theme of the game-map cell that holds the height-map position; any other paint type gives 0.
// Leaf: ?GetEngineThemeAt@CMap@@IBEKJJKW4EThemePaintType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081F090, 147 bytes, `ret 10h`.
// Name: Ego_r.pdb 0x00679CA0 / FableWin.pdb 0x022380B0 ?GetEngineThemeAt@CMap@@IBEKJJKW4EThemePaintType@@@Z (protected const); Ego_d.pdb
//   parameters x, y, index, PaintType and the block locals theme_palette_index (unsigned long) and gpos (C2DCoordI).
//   NF81 (round 7, q10978): re-authored on engine/CMap.h; the July row (candidates/world_map_theme.h with a local storage cast model)
//   never byte-matched (DIFFER 145 v 147).
// Callees: none out of line; inline: CHeightMap::GetCell, CHeightMapCell::GetEngineThemePaletteIndex, CMap::HeightMapUnitToGameMapUnit,
//   CGameMap::GetCell, CGameMapCell::GetAtmosTheme (FableWin.pdb).
// Layout: CMap ThemePaletteEntryList +0x1A6C (8-byte CThemePaletteEntry, ThemeDefIndex +0), GameMap +0x1A78, HeightMap +0x1A7C;
//   CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208; CGameMap Cells +0x04, SizeX +0x1C, SizeY +0x20.
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


unsigned long CMap::GetEngineThemeAt(long x, long y, unsigned long index, EThemePaintType PaintType) const
{
    if (PaintType == THEME_PAINT_ENGINE)
    {
        unsigned long theme_palette_index = HeightMap->GetCell(C2DCoordI(x, y)).GetEngineThemePaletteIndex(index);
        return ThemePaletteEntryList[theme_palette_index].ThemeDefIndex;
    }
    else if (PaintType == THEME_PAINT_ATMOS)
    {
        C2DCoordI gpos(HeightMapUnitToGameMapUnit(x), HeightMapUnitToGameMapUnit(y));
        return GameMap->GetCell(gpos).GetAtmosTheme(index);
    }
    return 0;
}
