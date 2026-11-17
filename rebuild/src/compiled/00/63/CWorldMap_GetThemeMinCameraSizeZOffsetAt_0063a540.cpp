// CWorldMap::GetThemeMinCameraSizeZOffsetAt(const C3DVector&) -- per-block query: forwards to CMap::GetThemeMinCameraSizeZOffsetAt of the loaded map holding the block, else 0
// Leaf: ?GetThemeMinCameraSizeZOffsetAt@CWorldMap@@QBEMABVC3DVector@@@Z
// Retail 0x0063a540, 161 bytes (world-map theme/height TU, 0x0063Axxx). Callees: IsPosInMap(C3DVector) 0x004FC600,
// slot 17 GetMapNumberAt(C2DCoordI) [+0x44], slot 10 IsMapLoaded [+0x28], ConvertWorldMapCoordsToLocalCoords(long,
// C3DVector) 0x004FC2A0 (out of line here), slot 19 PeekMap [+0x4C], CMap::GetThemeMinCameraSizeZOffsetAt 0x0081FA90. Default 0.0f (pool 0x0122DEDC).
// Retail calls PeekMap after the conversion and only then forms the C2DCoordI of the local position, so the map reference is a named local.
// NF62 (2026-11-17): ported onto the shared engine/CWorldMap.h + engine/CMap.h (the local C2DBoxI / CMap / CMapInfo / CMapInfoArray /
//   CWorldMap models deleted). The CMap callee now binds the Ego_r.pdb protected const spelling
//   ?GetThemeMinCameraSizeZOffsetAt@CMap@@IBEMABVC2DCoordI@@@Z (engine/CMap.h FABLE_RETAIL_CMAP_THEMESIZEZ, NF63; overlay opt-in
//   FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND makes CWorldMap a friend); the self declaration is FABLE_RETAIL_CWORLDMAP_THEMEMINCAMERASIZEZOFFSET.
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_CWORLDMAP_THEMEMINCAMERASIZEZOFFSET
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"

float CWorldMap::GetThemeMinCameraSizeZOffsetAt(const C3DVector& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
        if (map != 0 && IsMapLoaded(map))
        {
            C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& theMap = PeekMap(map);
            return theMap.GetThemeMinCameraSizeZOffsetAt(C2DCoordI((long)local.X, (long)local.Y));
        }
    }
    return 0.0f;
}
