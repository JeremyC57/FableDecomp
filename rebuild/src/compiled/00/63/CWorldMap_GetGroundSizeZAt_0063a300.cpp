// CWorldMap::GetGroundSizeZAt(const C3DVector&) -- height query: forwards to CMap::GetGroundSizeZAt of the loaded map holding the position, else 0
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail 0x000063a300, 0x83 bytes (world-map theme/height TU, 0x0063Axxx). Callees: IsPosInMap(C3DVector) 0x004FC600,
// __ftol2 0x00BFEA70 (block coord), slot 17 GetMapNumberAt(C2DCoordI) [+0x44], slot 10 IsMapLoaded [+0x28],
// ConvertWorldMapCoordsToLocalCoords(long, C3DVector) 0x004FC2A0 (out of line here, hidden result pointer),
// slot 19 PeekMap [+0x4C], CMap::GetGroundSizeZAt 0x0081EA50. Default 0.0f from the float pool (0x0122DEDC). 
// ---- local CWorldMap model (retail offsets; the PDB header rebuild/include/engine/CWorldMap.h
// uses 16-byte STL containers, retail uses 12-byte STLport vectors, so every member after +0x14
// sits 4/8/12.. bytes lower in retail: Maps +0x20 (PDB +0x24), Granularity +0x50 (PDB +0x60),
// MapPlacementPitch +0x58 (PDB +0x68), Area +0x5C..+0x68 (PDB +0x6C..+0x78), MapPlacement +0x6C
// (PDB +0x7C), PlayerPositionInfo +0x78 (PDB +0x8C), LoadedMaps +0x90 (PDB +0xAC)).
// NF70 (round 7, land1117fu q10725): moved onto engine/CMap.h + engine/CWorldMap.h (the local CMap / CWorldMap / C2DCoordI models are gone; NF62's 0x0063A540
//   define block); the CMap callee is the protected const PDB member, reached as CWorldMap is CMap's friend (FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"

float CWorldMap::GetGroundSizeZAt(const C3DVector& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
        if (map != 0 && IsMapLoaded(map))
        {
            C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& theMap = PeekMap(map);
            return theMap.GetGroundSizeZAt(local);
        }
    }
    return 0.0f;
}
