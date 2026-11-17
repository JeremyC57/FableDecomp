// CMap::SetEngineThemeAt(long x, long y, unsigned long theme_def_index, unsigned long index, EThemePaintType PaintType) -- set one
// theme layer of a map cell: paint type 0 registers a non-zero theme definition in the map's theme palette and stores its palette
// index in the height-map cell's engine theme layer (zero clears it); paint type 1 stores the theme in the atmos theme layer of the
// game-map cell that holds the height-map position (game map units are height map units / 4).
// Leaf: ?SetEngineThemeAt@CMap@@IAEXJJKKW4EThemePaintType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081EE60, 207 bytes, `ret 14h`.
// Name: Ego_r.pdb 0x00679A70 / FableWin.pdb 0x02237B90 ?SetEngineThemeAt@CMap@@IAEXJJKKW4EThemePaintType@@@Z (protected); Ego_d.pdb
//   parameters x, y, theme_def_index, index, PaintType and the block locals theme_palette_index (unsigned long) and gpos (C2DCoordI).
//   NF81 (round 7, q10951 / q10980): re-authored on engine/CMap.h; the July row (candidates/world_map_theme.h with a local storage
//   cast model) never byte-matched (DIFFER 174 v 207: it shared one cell offset between the two engine arms).
// Callees: CMap::AddThemeDefIndexToPalette 0x008F70F0 (?AddThemeDefIndexToPalette@CMap@@IAEEK@Z); inline: CHeightMap::GetCell,
//   CHeightMapCell::SetEngineThemePaletteIndex, CMap::HeightMapUnitToGameMapUnit, CGameMap::GetCell, CGameMapCell::SetAtmosTheme.
// Layout: CMap GameMap +0x1A78, HeightMap +0x1A7C; CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208 (cell 12 bytes, engine theme
//   palette indices +4); CGameMap Cells +0x04, SizeX +0x1C, SizeY +0x20 (cell 7 bytes, atmos themes +0).
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

void CMap::SetEngineThemeAt(long x, long y, unsigned long theme_def_index, unsigned long index, EThemePaintType PaintType)
{
    if (PaintType == THEME_PAINT_ENGINE)
    {
        if (theme_def_index > 0)
        {
            unsigned long theme_palette_index = AddThemeDefIndexToPalette(theme_def_index);
            HeightMap->GetCell(C2DCoordI(x, y)).SetEngineThemePaletteIndex((unsigned char)theme_palette_index, index);
        }
        else
            HeightMap->GetCell(C2DCoordI(x, y)).SetEngineThemePaletteIndex(0, index);
    }
    else if (PaintType == THEME_PAINT_ATMOS)
    {
        C2DCoordI gpos(HeightMapUnitToGameMapUnit(x), HeightMapUnitToGameMapUnit(y));
        GameMap->GetCell(gpos).SetAtmosTheme(index, (unsigned char)theme_def_index);
    }
}
