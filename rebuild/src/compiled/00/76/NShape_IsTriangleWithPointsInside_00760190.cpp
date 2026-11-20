// NShape::IsTriangleWithPointsInside(const std::vector<C3DVector>&, const std::vector<C3DVector>&, NShape::ECirculationOrder) -- count the points of p_list lying on the inner side of all three triangle edges (edge normal (Y, -X, 0), flipped for the anticlockwise order, DELTA_NOTIONAL_ZERO tolerance); true when more than three
// Inline-budget: no flag, no pragma, no new device. Header contracts only: C3DVector's operator- / operator*= / Dot / (x, y, z) constructor are FABLE_VECTOR_INLINE header bodies, expanded in retail and here; the row has no calls.
// Leaf: ?IsTriangleWithPointsInside@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@0W4ECirculationOrder@1@@Z
// Flags: /Gr
// Retail: Fable.exe 0x00760190, 340 bytes, __fastcall (triangle in ecx, p_list in edx, circ on the stack, `ret 4`), frame 0x50, no EH frame, no calls; the NShape TU (follows 0x00760020 IsLineIntersectingWithOthersInList, precedes 0x007602F0 IsShapeConvex). Reads DELTA_NOTIONAL_ZERO 0x0129BA3C (hoisted, negated on st(1) for the whole function).
// Identity: Ego_r.pdb / FableWin.pdb ?IsTriangleWithPointsInside@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@0W4ECirculationOrder@1@@Z; ECirculationOrder from Ego_d TPI 0x3F5BC. Parameter and local names, types and block nesting from Ego_d (triangle, p_list, circ, number_of_points_inside, {it, {it_trig, quit, inside_count, start_pos, {edge, perp, end_pos, line}}}).
// Inferred: `edge = *it - start_pos` is declared after `line` and before `perp` (retail computes it before the circ test, 0x0076022C..0x00760249). Ego_d lines 156..166 are 16 / 23 / 12 / 6 / 18 / 29 B; this text at VC7.1 /Od gives line 16, edge 16, perp 23, circ test 6, *= 13, dot test 28. With edge after the *= (the order that reads 157 as perp and 163 as edge): DIFFER(314v340); edge before line: DIFFER(316v340); edge after perp: DIFFER(318v340).
// Ego_d shape: the closing test is `if (number_of_points_inside > 3) return true; return false;` (lines 180 / 182 / 184: 6 / 4 / 2 B, as here); `return number_of_points_inside > 3;` gives DIFFER(332v340).
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NShape.h"
namespace NShape {
bool __fastcall IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& p_list, ECirculationOrder circ)
{
    long number_of_points_inside = 0;
    for (std::vector<C3DVector>::const_iterator it = p_list.begin(); it != p_list.end(); ++it)
    {
        std::vector<C3DVector>::const_iterator it_trig = triangle.begin();
        C3DVector start_pos = *it_trig++;
        bool quit = false;
        long inside_count = 0;
        while (!quit)
        {
            C3DVector end_pos;
            if (it_trig == triangle.end())
            {
                end_pos = *triangle.begin();
                quit = true;
            }
            else
            {
                end_pos = *it_trig++;
            }
            C3DVector line = end_pos - start_pos;
            C3DVector edge = *it - start_pos;
            C3DVector perp(line.Y, -line.X, 0.0f);
            if (circ == CIRCULATION_ANTICLOCKWISE)
                perp *= -1.0f;
            if (perp.Dot(edge) > -DELTA_NOTIONAL_ZERO)
                inside_count++;
            start_pos = end_pos;
        }
        if (inside_count == 3)
            number_of_points_inside++;
    }
    if (number_of_points_inside > 3)
        return true;
    return false;
}
}