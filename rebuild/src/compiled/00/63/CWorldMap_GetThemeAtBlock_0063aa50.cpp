// CWorldMap::GetThemeAtBlock(const C2DCoordI& pos, unsigned long theme_index, EThemePaintType PaintType) const -- the theme layer at a
// world block, read from the map that holds it in that map's local coordinates (CMap::GetEngineThemeAt); 0 outside every map
// Leaf: ?GetThemeAtBlock@CWorldMap@@QBEKABVC2DCoordI@@KW4EThemePaintType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063AA50, `ret 0Ch`.
// Name: Ego_r.pdb 0x006E7340 / FableWin.pdb 0x024A4330 (public const; C2DCoordI is a class); Ego_d.pdb parameters pos, theme_index,
//   PaintType, locals index (long) and local_pos (C2DCoordI, in a block).
// Callees: virtual GetMapNumberAt(const C2DCoordI&) slot 17 (+0x44), ConvertWorldMapCoordsToLocalCoords 0x004FC3C0
//   (?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC2DCoordI@@JABV2@@Z, out of line), virtual PeekMap slot 19 (+0x4C), then CMap::GetEngineThemeAt 0x0081F090 (?GetEngineThemeAt@CMap@@IBEKJJKW4EThemePaintType@@@Z).
// NF81 (round 7, q10978): moved from candidates/world_map_theme.h (local struct C2DCoordI, so the row's own leaf spelled ABU; Slot-placeholder
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

unsigned long CWorldMap::GetThemeAtBlock(const C2DCoordI& pos, unsigned long theme_index, EThemePaintType PaintType) const
{
    const long index = GetMapNumberAt(pos);
    if (index != 0)
    {
        const C2DCoordI local_pos = ConvertWorldMapCoordsToLocalCoords(index, pos);
        return PeekMap(index).GetEngineThemeAt(local_pos.X, local_pos.Y, theme_index, PaintType);
    }
    return 0;
}
