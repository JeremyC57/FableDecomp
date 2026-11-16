// 0063d0a0 CWorldMap::FindGroundCollisionPointWithinCell
// Flags: none
// Leaf: ?FindGroundCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z
#define FABLE_RETAIL_CWORLDMAP_COLLISION_CORNERS_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
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


// Original FableWin01d9e1b0; donor024a8b8c calls its fastcall thunk.
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b)
{
    float result=a.X*b.X+a.Y*b.Y+a.Z*b.Z;
    return result;
}

bool CWorldMap::FindGroundCollisionPointWithinCell(const C3DVector* start_pos, const C3DVector* end_pos, const C3DVector* cell_pos, C3DVector* total_vector, C3DVector* collision_pos) const
{
    C3DVector corner_pos[4];
    CPlane plane;
    C3DVector vector_normal;
    C3DVector plane_normal;
    C3DVector int_cell_pos;
    vector_normal = *total_vector;
    vector_normal.Normalise();
    int_cell_pos.X = (float)(long)cell_pos->X;
    int_cell_pos.Y = (float)(long)cell_pos->Y;
    for (long c1 = 0; c1 < 4; ++c1) {
        corner_pos[c1].X = (float)LineCollisionAround4Square[c1].X + int_cell_pos.X;
        corner_pos[c1].Y = (float)LineCollisionAround4Square[c1].Y + int_cell_pos.Y;
        if (IsPosInMap(corner_pos[c1]))
            corner_pos[c1].Z = GetGroundSizeZAt(corner_pos[c1]);
        else
            corner_pos[c1].Z = 0.0f;
    }
    long base_x = (long)int_cell_pos.X;
    long base_y = (long)int_cell_pos.Y;
    if ((base_x + base_y) % 2 == 1) {
        plane.Initialise(corner_pos[0], corner_pos[1], corner_pos[2]);
        plane_normal = plane.GetNormal();
        bool facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellTR(collision_pos, &int_cell_pos) == true)
            return true;
        plane.Initialise(corner_pos[0], corner_pos[2], corner_pos[3]);
        plane_normal = plane.GetNormal();
        facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellBL(collision_pos, &int_cell_pos) == true)
            return true;
    } else {
        plane.Initialise(corner_pos[0], corner_pos[1], corner_pos[3]);
        plane_normal = plane.GetNormal();
        bool facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellTL(collision_pos, &int_cell_pos) == true)
            return true;
        plane.Initialise(corner_pos[1], corner_pos[2], corner_pos[3]);
        plane_normal = plane.GetNormal();
        facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellBR(collision_pos, &int_cell_pos) == true)
            return true;
    }
    return false;
}