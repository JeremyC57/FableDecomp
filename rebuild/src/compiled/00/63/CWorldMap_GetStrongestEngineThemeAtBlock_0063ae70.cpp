// CWorldMap::GetStrongestEngineThemeAtBlock(const C2DCoordI&, long&) const -- the strongest engine theme on a loaded map's block (0 elsewhere)
// Leaf: ?GetStrongestEngineThemeAtBlock@CWorldMap@@QBEJABVC2DCoordI@@AAJ@Z
// Retail: Fable.exe @ 0x0063AE70, 95 bytes, `ret 8`. PDB ?GetStrongestEngineThemeAtBlock@CWorldMap@@QBEJABVC2DCoordI@@AAJ@Z
// Callees: primary virtuals (vtable 0x01244AEC) slot 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44), slot 10
// IsMapLoaded(long) 0x004FC050 (+0x28), slot 19 PeekMap(long) 0x0051CE40 (+0x4C);
// CWorldMap::ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI&) const 0x004FC3C0 (out of line in this TU),
// CMap::GetStrongestEngineThemeAt(const C2DCoordI&, long&) const 0x0081F910 (PDB
// ?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z; manifest label "CMap::CopyPos" is a propagation).
// Shape: as SetGroundSizeZAtBlock 0x0063A910 (named frame block, setter arguments pushed before the map fetch); the
// two exits keep their own epilogues, the miss path returns 0.
// NF70 (round 7, land1117fu q10724 / q10735): moved onto engine/CMap.h + engine/CWorldMap.h (the local CMap / CWorldMap / C2DCoordI models are gone; NF62's 0x0063A540
//   define block); the CMap callee is the protected const PDB member, reached as CWorldMap is CMap's friend (FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP_CONVERTCOORDSTOLOCALBLOCK
#define FABLE_RETAIL_CWORLDMAP_STRONGESTTHEMEATBLOCK
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"

long CWorldMap::GetStrongestEngineThemeAtBlock(const C2DCoordI& pos, long& strength) const
{
    long map = GetMapNumberAt(pos);
    if (map && IsMapLoaded(map))
    {
        C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
        return PeekMap(map).GetStrongestEngineThemeAt(local, strength);
    }
    return 0;
}
