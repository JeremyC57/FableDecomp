// CWorldMap::SetThemeAtBlock(const C2DCoordI& pos, unsigned long theme, unsigned long theme_index, EThemePaintType PaintType) -- when the
// block lies in a changeable map, convert it to that map's local coordinates and set the theme layer there (CMap::SetEngineThemeAt)
// Leaf: ?SetThemeAtBlock@CWorldMap@@QAEXABVC2DCoordI@@KKW4EThemePaintType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063A970, 99 bytes, `ret 10h`.
// Name: Ego_r.pdb 0x006E7260 / FableWin.pdb 0x024A41D0 (public; C2DCoordI is a class); Ego_d.pdb parameters pos, theme, theme_index,
//   PaintType, locals index (long) and local_pos (C2DCoordI, in a block).
// Callees: virtual GetMapNumberAt(const C2DCoordI&) slot 17 (+0x44), IsPosChangeable slot 25 (+0x64), ConvertWorldMapCoordsToLocalCoords 0x004FC3C0
//   (?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC2DCoordI@@JABV2@@Z, out of line), virtual GetMap slot 18 (+0x48), then CMap::SetEngineThemeAt 0x0081EE60 (?SetEngineThemeAt@CMap@@IAEXJJKKW4EThemePaintType@@@Z).
// NF81 (round 7, q10976 / q10980 / q10987): moved from candidates/world_map_theme.h (local struct C2DCoordI, so the row's own leaf spelled ABU; Slot-placeholder
//   CWorldMap vtable; public CMap theme members) onto engine/CMap.h + engine/CWorldMap.h (0x0063A7E0 define block) with the NF81 overlay
//   opt-ins FABLE_RETAIL_CWORLDMAP_THEMEATBLOCK and FABLE_RETAIL_CMAP_ENGINETHEMEAT; CWorldMap reaches the protected CMap member as CMap's
//   friend (FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND). Body unchanged apart from the Ego_d.pdb names (index, local_pos).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_CONVERTCOORDSTOLOCALBLOCK
#define FABLE_RETAIL_CWORLDMAP_THEMEATBLOCK         // NF81 overlay: the four block accessors (public, ABV C2DCoordI)
#define FABLE_RETAIL_CMAP_ENGINETHEMEAT             // NF81 overlay: EThemePaintType and the protected CMap per-cell theme accessors
#define FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"

void CWorldMap::SetThemeAtBlock(const C2DCoordI& pos, unsigned long theme, unsigned long theme_index, EThemePaintType PaintType)
{
    const long index = GetMapNumberAt(pos);
    if (index != 0 && IsPosChangeable(pos))
    {
        const C2DCoordI local_pos = ConvertWorldMapCoordsToLocalCoords(index, pos);
        GetMap(index).SetEngineThemeAt(local_pos.X, local_pos.Y, theme, theme_index, PaintType);
    }
}
