// CMap::GetThemeSizeZAt(const C2DCoordI& pos) -- the theme's minimum camera-size Z offset at pos (0x0081FA20) plus the height of the game map cell there (rows stored top-down: (maxY - y) * (width + 1) + x)
// Leaf: ?GetThemeSizeZAt@CMap@@IBEMABVC2DCoordI@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820100, length 51 bytes.
// Callees: CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI&) @ 0x0081FA20. Shape: the cell index (esi) and the cell table (edi) computed before the call, `fadd dword ptr [edi+eax*4]` after it, `ret 4`.
// NF63 (round 7): moved onto engine/CMap.h (FABLE_RETAIL_CMAP_THEMESIZEZ in the overlay; the local CMap / C2DCoordI / C2DVector /
// grid models are gone). Self ?GetThemeSizeZAt@CMap@@IBEMABVC2DCoordI@@@Z (protected const in Ego_r.pdb 0x0067B320 and FableWin.pdb 0x02232220; Ego_r order
// GetThemeSizeZAt(C3DVector) / GetThemeSizeZAt(C2DCoordI) / GetThemeMinCameraSizeZAt(C3DVector) = retail 0x008200B0 / 0x00820100 /
// 0x00820140, each calling its offset twin 0x0081FA20 / 0x0081FA20 / 0x0081FA90; the landed CWorldMap::GetThemeSizeZAt 0x0063A390
// calls 0x008200B0). Callees ?GetThemeSizeZOffsetAt@CMap@@IBEMABVC2DCoordI@@@Z 0x0081FA20; the height map cell is read in place (HeightMap +0x1A7C; CHeightMap
// GetIndexIntoCells under FABLE_RETAIL_CHEIGHTMAP_CELLINDEX, CMap a friend there): the index and the cell table are taken before the call
// and the element read after it (`fadd [edi+eax*4]`); GetCell / GetCellIncludingExtraCellsAt / a cell reference all fold the address
// before the call (DIFFER 55v53, 4 grades).
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_ENGINEACCESSORS
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/C3DVector.h"
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
float CMap::GetThemeSizeZAt(const C2DCoordI& pos) const
{
    const CHeightMap* heightMap = &*HeightMap;
    long index = heightMap->GetIndexIntoCells(pos.X, pos.Y, heightMap->SizeX, heightMap->SizeY);
    std::vector<CHeightMapCell>::const_iterator cells = heightMap->Cells.begin();
    return GetThemeSizeZOffsetAt(pos) + cells[index].Height;
}
