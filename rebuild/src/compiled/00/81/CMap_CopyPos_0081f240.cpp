// CMap::CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to) -- copy one height-map cell (extra border cells
// included) from another map into this one: the whole 12-byte cell first, then each of its three theme layers re-registered in
// THIS map's theme palette (the source cell's palette index is looked up in the source map's palette and the theme definition
// index handed to SetEngineThemeAtIncludingExtraCells), then the two engine blend bytes.
// Leaf: ?CopyPos@CMap@@IAEXABV1@ABVC2DCoordI@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081F240, 392 bytes, `ret 0Ch`, no frame pointer, one frame slot (the spilled `this`).
// Identity: Ego_r.pdb / FableWin.pdb ?CopyPos@CMap@@IAEXABV1@ABVC2DCoordI@@1@Z (protected); Ego_r donor order SetEngineThemeAt ...
//   GetEngineBlendAtIncludingExtraCells / CopyPos (0x00679E50) / IsPassableAt = retail 0x0081EE60 ... 0x0081F240 / 0x0081F3D0;
//   callers 0x0063E127 / 0x0063E22B / 0x0063E298 in the landed CWorldMap::UpdateAllEdges 0x0063DFD0.
// Callees: CMap::SetEngineThemeAtIncludingExtraCells 0x0081EF30 (?SetEngineThemeAtIncludingExtraCells@CMap@@IAEXJJKK@Z, out of
//   line); inline: CMap::GetEngineThemeAtIncludingExtraCells, GetEngineBlendAtIncludingExtraCells,
//   SetEngineBlendAtIncludingExtraCells, CHeightMap::GetCell, the CHeightMapCell accessors (engine/CMap.h
//   FABLE_RETAIL_CMAP_ENGINEACCESSORS, engine/CHeightMapCell.h FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS).
// Layout: CMap ThemePaletteEntryList +0x1A6C, HeightMap +0x1A7C; CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208;
//   CHeightMapCell EngineThemePaletteIndex[3] +4, EngineBlend[2] +7. Cell (x, y) = Cells[(SizeY - y) * (SizeX + 1) + x].
// Shape: the cell copy evaluates the source cell first (0x0081F247) and is three dword moves; the layer loop is a real loop with
//   a SIGNED counter (0x0081F2FA `jl`; an unsigned long counter gives `jb`) that re-reads source.HeightMap every pass; the blend
//   copies are two explicit calls of the inline helper pair with constant indices (byte +7 at 0x0081F362, byte +8 at 0x0081F3A4).
//   Going through the PDB inline helpers (value through an unsigned char parameter, coordinates through C2DCoordI temporaries) is
//   what makes VC7.1 reload this->HeightMap three times and park the source cells base in the dead `source` argument slot in the
//   first blend copy; raw member expressions keep them in registers (the old 368-byte draft).
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_ENGINEACCESSORS
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CMap.h"
#include "engine/CHeightMap.h"

void CMap::CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to)
{
    HeightMap->GetCell(to) = source.HeightMap->GetCell(from);
    for (long i = 0; i < 3; ++i)
        SetEngineThemeAtIncludingExtraCells(to.X, to.Y, source.GetEngineThemeAtIncludingExtraCells(from.X, from.Y, i), i);
    SetEngineBlendAtIncludingExtraCells(to.X, to.Y, source.GetEngineBlendAtIncludingExtraCells(from.X, from.Y, 0), 0);
    SetEngineBlendAtIncludingExtraCells(to.X, to.Y, source.GetEngineBlendAtIncludingExtraCells(from.X, from.Y, 1), 1);
}