// NShape::IsLineIntersectingWithOthersInList(const C2DLineF&, const std::vector<C3DVector>&) -- walk the closed polygon points_list edge by edge (the last edge back to the first point) and count the edges that are the line itself, in either direction, or that cross it; true when more than four
// Inline-budget: no flag, no pragma, no new device. Header contracts only: C2DVector's operator- / GetSquaredMagnitude and the C2DLineF / C2DVector from-C3DVector constructors are plain inline header bodies, expanded in retail and here; IntersectsWith is a declaration only (called out of line, 0x00A56C80).
// Leaf: ?IsLineIntersectingWithOthersInList@NShape@@YI_NABVC2DLineF@@ABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Flags: /Gr
// Retail: Fable.exe 0x00760020, 360 bytes, __fastcall (line in ecx, points_list in edx), plain `ret`, frame 0x40, no EH frame; the NShape TU (0x0075FE30 functor operator(), 0x0075FF00 IsVectorBetween2OthersInThePlane, this row, 0x00760190 IsTriangleWithPointsInside, 0x007602F0 IsShapeConvex, ... 0x00763060 FractureShape). Calls C2DLineF::IntersectsWith 0x00A56C80; reads DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Identity: Ego_r.pdb ?IsLineIntersectingWithOthersInList@NShape@@YI_NABVC2DLineF@@ABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z; callee ?IntersectsWith@C2DLineF@@QBE_NABV1@AAVC2DVector@@M@Z (FableWin + Ego_r name lists). Parameter and local names, types and block nesting from Ego_d (line, points_list, number_of_intersections, it, start_pos, quit, {end_pos, this_line}).
// Ego_d shape: lines 87..128 (egod_lines/): one counting statement per branch (113: 14 B = ++ and jmp, 117: 11 B, 121: 9 B), the IntersectsWith test on its own `else if` (119: 32 B) with no intersection local in Ego_d -- the out-parameter is an unnamed C2DVector temporary (retail passes an uninitialised slot, `lea eax,[esp+24h]` at 0x0076013D).
// Inferred: the end-point tests as (this_line.PointN - line.PointM).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO (FableWin/Ego_r ?GetSquaredMagnitude@C2DVector@@QBEMXZ; no point-compare helper in the NShape name set): retail keeps the dead difference vector on the x87 stack and pops it under the compare (`fstp st(0)` x2, 0x007600B3), as the header's GetMagnitude note describes for that inline body. The squared sum order is byte-neutral here (FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX gives the same bytes).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DLINEF_FROM3D
#define FABLE_RETAIL_C2DLINEF_INTERSECTS_LINE
#include "engine/NShape.h"
namespace NShape {
bool __fastcall IsLineIntersectingWithOthersInList(const C2DLineF& line, const std::vector<C3DVector>& points_list)
{
    long number_of_intersections = 0;
    std::vector<C3DVector>::const_iterator it = points_list.begin();
    C3DVector start_pos = *it++;
    bool quit = false;
    while (!quit)
    {
        C3DVector end_pos;
        if (it == points_list.end())
        {
            end_pos = *points_list.begin();
            quit = true;
        }
        else
        {
            end_pos = *it++;
        }
        C2DLineF this_line(start_pos, end_pos);
        if ((this_line.Point1 - line.Point1).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO && (this_line.Point2 - line.Point2).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO)
            number_of_intersections++;
        else if ((this_line.Point1 - line.Point2).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO && (this_line.Point2 - line.Point1).GetSquaredMagnitude() < DELTA_NOTIONAL_ZERO)
            number_of_intersections++;
        else if (this_line.IntersectsWith(line, C2DVector(), DELTA_NOTIONAL_ZERO))
            number_of_intersections++;
        start_pos = end_pos;
    }
    return number_of_intersections > 4;
}
}