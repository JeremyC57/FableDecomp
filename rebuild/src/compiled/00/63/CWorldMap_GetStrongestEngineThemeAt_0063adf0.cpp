// CWorldMap::GetStrongestEngineThemeAt(const C3DVector&, long&) -- strongest engine theme of the loaded map at a position
// Leaf: ?GetStrongestEngineThemeAt@CWorldMap@@QBEJABVC3DVector@@AAJ@Z
// NF113 (q6460): Leaf line added (the object already defines the Ego_r.pdb 0x006E84E0 / FableWin.pdb 0x024A4B90 spelling; the six landed binders bind it); body unchanged.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail 0x0063ADF0, 0x77 bytes (world-map theme/height TU). Callees: slot 16 GetMapNumberAt(C3DVector) [+0x40],
// slot 10 IsMapLoaded [+0x28], ConvertWorldMapCoordsToLocalCoords(long, C3DVector) 0x004FC2A0, __ftol2 0x00BFEA70
// (local X/Y truncated into a C2DCoordI), slot 19 PeekMap [+0x4C] called after both arguments are pushed, then
// CMap::GetStrongestEngineThemeAt 0x0081F910 (manifest label CMap::CopyPos is a propagation). Returns 0 otherwise.
// ---- local CWorldMap model (retail offsets; the PDB header rebuild/include/engine/CWorldMap.h
// uses 16-byte STL containers, retail uses 12-byte STLport vectors, so every member after +0x14
// sits 4/8/12.. bytes lower in retail: Maps +0x20 (PDB +0x24), Granularity +0x50 (PDB +0x60),
// MapPlacementPitch +0x58 (PDB +0x68), Area +0x5C..+0x68 (PDB +0x6C..+0x78), MapPlacement +0x6C
// (PDB +0x7C), PlayerPositionInfo +0x78 (PDB +0x8C), LoadedMaps +0x90 (PDB +0xAC)).
// NF70 (round 7, land1117fu q10724 / q10735): moved onto engine/CMap.h + engine/CWorldMap.h (the local CMap / CWorldMap / C2DCoordI models are gone; NF62's 0x0063A540
//   define block); the CMap callee is the protected const PDB member, reached as CWorldMap is CMap's friend (FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"

long CWorldMap::GetStrongestEngineThemeAt(const C3DVector& pos, long& strength) const
{
    long map = GetMapNumberAt(pos);
    if (map != 0 && IsMapLoaded(map))
    {
        C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
        C2DCoordI block((long)local.X, (long)local.Y);
        return PeekMap(map).GetStrongestEngineThemeAt(block, strength);
    }
    return 0;
}
