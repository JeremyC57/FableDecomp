// CWorldMap::FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const -- intersect the line with the two theme-surface triangles of the cell holding cellPos (split diagonal by cell parity); true when the hit lies on the segment and inside that triangle
// Grade: DIFFER(1017v1017) -- the 0x0063D0A0 twin's scheduling residue: `fld dir.X` before `push esi`, and the start/end/hit pointer loads hoisted above the first plane-normal product. Chunk 1013c probes (inert or worse): DOT_GROUPED / DOT_ORDERED_XYZ (25-27 hunks), without SQUAREDMAGNITUDE_XYZ (33), plane.Normal for GetNormal(), normal declared before dir, per-plane named normals (worse)
// Leaf: ?FindThemeCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z
// Chunk 1013b: not regraded; the twin of 0x0063D0A0, same residue.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063D4A0, 1017 bytes, thiscall, `ret 14h`. Twin of 0x0063D0A0 (GetGroundSizeZAt corners).
// Identity: Ego_r.pdb 0x006E9F10 ?FindThemeCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z (protected const).
// Callees: _ftol 0x00BFEA70, IsPosInMap(const C3DVector&) 0x004FC600, GetThemeSizeZAt(const C3DVector&) 0x0063A390,
//   CPlane::Initialise 0x00A42170, CPlane::GetIntersectionWithLine 0x00A422C0, PointIsWithinLineSegment 0x0063BAB0,
//   PointIsWithinCellTR/BL/TL/BR 0x0063B890 / 0x0063B920 / 0x0063B9B0 / 0x0063BA30; the cell-corner offsets 0x0137B730
//   {(0,1),(1,1),(1,0),(0,0)} (the landed 0x0063B750 / 0x0063B7F0 name); GFFastOneOverSqrt's table 0x013CE558.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CPlane.h"
#include "engine/CWorldMap.h"

extern C2DCoordI g_CellCornerOffsets[4];                            // 0x0137B730 {(0,1),(1,1),(1,0),(0,0)}

bool CWorldMap::FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const
{
    C3DVector dir;
    dir = *direction;
    dir.Normalise();
    C3DVector normal;
    C3DVector cell;
    CPlane plane;
    cell.X = (float)(long)cellPos->X;
    cell.Y = (float)(long)cellPos->Y;
    C3DVector corners[4];
    for (long i = 0; i < 4; ++i) {
        corners[i].X = (float)g_CellCornerOffsets[i].X + cell.X;
        corners[i].Y = (float)g_CellCornerOffsets[i].Y + cell.Y;
        if (IsPosInMap(corners[i]))
            corners[i].Z = GetThemeSizeZAt(corners[i]);
        else
            corners[i].Z = 0.0f;
    }
    if (((long)cell.Y + (long)cell.X) % 2 == 1) {
        plane.Initialise(corners[0], corners[1], corners[2]);
        normal = plane.GetNormal();
        if (normal.Dot(dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellTR(hit, &cell) == true)
            return true;
        plane.Initialise(corners[0], corners[2], corners[3]);
        normal = plane.GetNormal();
        if (normal.Dot(dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellBL(hit, &cell) == true)
            return true;
    } else {
        plane.Initialise(corners[0], corners[1], corners[3]);
        normal = plane.GetNormal();
        if (normal.Dot(dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellTL(hit, &cell) == true)
            return true;
        plane.Initialise(corners[1], corners[2], corners[3]);
        normal = plane.GetNormal();
        if (normal.Dot(dir) > 0.0f
            && plane.GetIntersectionWithLine(*start, dir, hit) == true
            && PointIsWithinLineSegment(hit, start, end) == true
            && PointIsWithinCellBR(hit, &cell) == true)
            return true;
    }
    return false;
}
