// CWorldMap::FindMaxThemeZInCell(const C3DVector* cell) const -- highest theme height over the four in-map corner blocks of a cell (0 when none)
// Leaf: ?FindMaxThemeZInCell@CWorldMap@@IBEMPBVC3DVector@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063B7F0, 154 bytes (+ int3 padding), `ret 4`; byte twin of
// FindMaxGroundZInCell 0x0063B750 apart from the height query.
// Identity: Ego_r.pdb `?FindMaxThemeZInCell@CWorldMap@@IBEMPBVC3DVector@@@Z` (the PDB has no "ThemeMinCamera" cell
// query; the catalog label "CWorldMap_FindMaxThemeMinCameraZ" and the landed caller's FindMaxThemeMinCameraZInCell follow
// the propagated callee label); caller the landed FindLastUnblockedThemePositionAndReportCollision 0x0063DB30.
// Callees: virtual slot 13 GetDimensions (+0x34, result unused), IsPosInMap(const C2DCoordI&) 0x004FC5B0,
// the per-block theme height 0x0063A7E0 (landed as GetThemeMinCameraSizeZOffsetAt(const C2DCoordI&), whose own note
// says it forwards to CMap::GetThemeSizeZAt and is probably the theme-height query: Ego_r.pdb's GetThemeSizeZAtBlock), __ftol2.
// Shape: the corner block is two named longs (edi/ebx) and each query gets its own C2DCoordI temporary, so both 8-byte
// temporaries have their address taken inside the loop: that is the `and esp,-8` aligned frame. `this` lives in a frame
// word, the table index walks in esi by 8.
// ---- local CWorldMap model (the landed 0x0063A660 GetGroundSizeZAtBlock model: retail offsets and vtable; the PDB
// header rebuild/include/engine/CWorldMap.h uses 16-byte STL containers where retail STLport vectors are 12 bytes, so it
// does not fit). Only the vtable up to slot 13 GetDimensions (+0x34) and the out-of-line block queries are needed.
// NF52 (round 7): moved onto engine/CWorldMap.h (FABLE_RETAIL_CWORLDMAP). The corner table at 0x0137B730 is the PDB's protected static
// ?LineCollisionAround4Square@CWorldMap@@1QBVC2DCoordI@@B (FABLE_RETAIL_CWORLDMAP_COLLISION_CORNERS_PASS23, as the landed 0x0063D0A0 /
// 0x0063D4A0); the self is the PDB's protected const FindMaxThemeZInCell (IBE; the row spelled it public) and the block queries are
// declared by the overlay opt-in FABLE_RETAIL_CWORLDMAP_FINDMAXZINCELL (GetThemeSizeZAtBlock: FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL).
// Were a local CWorldMap model, a local C2DBoxI and the made-up CCellCornerOffset g_CellCornerOffsets[4].
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_COLLISION_CORNERS_PASS23
#define FABLE_RETAIL_CWORLDMAP_FINDMAXZINCELL
#define FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CWorldMap.h"

float CWorldMap::FindMaxThemeZInCell(const C3DVector* cell) const
{
    GetDimensions();
    float maxZ = 0.0f;
    for (unsigned int i = 0; i < 4; ++i) {
        long x = (long)(LineCollisionAround4Square[i].X + cell->X);
        long y = (long)(LineCollisionAround4Square[i].Y + cell->Y);
        if (IsPosInMap(C2DCoordI(x, y))) {
            float z = GetThemeSizeZAtBlock(C2DCoordI(x, y));
            if (z > maxZ)
                maxZ = z;
        }
    }
    return maxZ;
}