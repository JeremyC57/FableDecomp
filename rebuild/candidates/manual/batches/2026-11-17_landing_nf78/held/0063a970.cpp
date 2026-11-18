// CWorldMap::SetThemeAtBlock(const C2DCoordI&, unsigned long, unsigned long, EThemePaintType) -- when the block lies in a changeable map, convert to that map's local coordinates and set the engine theme there
// NF78 (round 7, q10951): CMap::SetEngineThemeAt is protected in Ego_r.pdb / FableWin.pdb (?SetEngineThemeAt@CMap@@IAEXJJKKW4EThemePaintType@@@Z);
//   the legacy header's FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED opt-in (NF78 overlay: protected members, CWorldMap a friend) gives that spelling.
#define FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED
#include "candidates/world_map_theme.h"

void CWorldMap::SetThemeAtBlock(
    const C2DCoordI& worldMapCoord,
    unsigned long themeId,
    unsigned long themeValue,
    EThemePaintType paintType)
{
    const long mapNumber = GetMapNumberAt(worldMapCoord);
    if (mapNumber != 0 && IsPosChangeable(worldMapCoord))
    {
        const C2DCoordI localCoord =
            ConvertWorldMapCoordsToLocalCoords(mapNumber, worldMapCoord);
        GetMap(mapNumber).SetEngineThemeAt(
            localCoord.x,
            localCoord.y,
            themeId,
            themeValue,
            paintType);
    }
}
