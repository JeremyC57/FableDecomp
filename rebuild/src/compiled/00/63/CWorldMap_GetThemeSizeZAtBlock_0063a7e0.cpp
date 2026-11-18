// CWorldMap::GetThemeSizeZAtBlock(const C2DCoordI& pos) const -- per-block query: forwards to CMap::GetThemeSizeZAt of the loaded map holding the block, else 0
// Leaf: ?GetThemeSizeZAtBlock@CWorldMap@@QBEMABVC2DCoordI@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail 0x0063A7E0, 0x68 bytes (world-map theme/height TU, 0x0063Axxx). Callees: IsPosInMap(C2DCoordI) 0x004FC5B0,
// slot 17 GetMapNumberAt(C2DCoordI) [+0x44], slot 10 IsMapLoaded [+0x28], ConvertWorldMapCoordsToLocalCoords(long,
// C2DCoordI) 0x004FC3C0 (out of line here), slot 19 PeekMap [+0x4C], CMap::GetThemeSizeZAt 0x00820100. Default 0.0f (pool 0x0122DEDC).
// Retail calls PeekMap after the conversion and only then forms &local, so the map reference is a named local.
// Renamed from CWorldMap::GetThemeMinCameraSizeZOffsetAt (FINDINGS chunk 382: "probably GetThemeSizeZAtBlock"): that
//   catalog name belongs to the byte twin 0x0063A5F0, which forwards to CMap 0x0081FA90 instead; retail has no
//   identical-code folding, so the two are different members. Ego_r.pdb / FableWin.pdb: the CWorldMap members taking
//   (const C2DCoordI&) and returning float are GetThemeMinCameraSizeZOffsetAt, GetThemeMinCameraSizeZOffsetAtBlock and
//   GetThemeSizeZAtBlock; the one forwarding to CMap::GetThemeSizeZAt(const C2DCoordI&) (landed 0x00820100 under that
//   PDB name) is GetThemeSizeZAtBlock (CWorldMap::GetThemeSizeZAt takes a C3DVector). Its callers fit a theme height:
//   CWorldMap::FindMaxThemeZInCell 0x0063B7F0 and CWorldMap::GetThemeFaceNormalAt 0x0063AB90 (x4).
// ---- local CWorldMap model (retail offsets; the PDB header rebuild/include/engine/CWorldMap.h
// uses 16-byte STL containers, retail uses 12-byte STLport vectors, so every member after +0x14
// sits 4/8/12.. bytes lower in retail: Maps +0x20 (PDB +0x24), Granularity +0x50 (PDB +0x60),
// MapPlacementPitch +0x58 (PDB +0x68), Area +0x5C..+0x68 (PDB +0x6C..+0x78), MapPlacement +0x6C
// (PDB +0x7C), PlayerPositionInfo +0x78 (PDB +0x8C), LoadedMaps +0x90 (PDB +0xAC)).
// NF70 (round 7, land1117fu q10727): moved onto engine/CMap.h + engine/CWorldMap.h (the local CMap / CWorldMap / C2DCoordI models are gone; NF62's 0x0063A540
//   define block); the CMap callee is the protected const PDB member, reached as CWorldMap is CMap's friend (FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL
#define FABLE_RETAIL_CWORLDMAP_FINDMAXZINCELL
#define FABLE_RETAIL_CWORLDMAP_CONVERTCOORDSTOLOCALBLOCK
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"

float CWorldMap::GetThemeSizeZAtBlock(const C2DCoordI& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(pos);
        if (map != 0 && IsMapLoaded(map))
        {
            C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& theMap = PeekMap(map);
            return theMap.GetThemeSizeZAt(local);
        }
    }
    return 0.0f;
}
