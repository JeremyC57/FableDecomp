// NShape::IsShapeConvex(const std::vector<C3DVector>& pts) -- add up the turning angles (in turns) between consecutive XY edge directions of the closed outline, closing edges included, and call the shape convex when they come to one full turn
// Grade: DIFFER(1125v1126) -- 4 diff lines, one residue: closing2's second.X load. Retail `mov ecx,[edi+0Ch]` sits above `add edi,0Ch` (0x007605F3); ours is `mov ecx,[edi]` after the add. It is one IL-order difference in scheduling region 4 (c2 DAG dump); 25+ closing2 spellings leave it.
// Next idea: find the closing2 text whose IL loads second.X off the un-incremented begin (retail: the first copy goes through `mov eax,edi`, then second.X is loaded before the add).
// Leaf: ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007602F0, 1126 bytes, __fastcall (pts in ecx), returns bool; x87 throughout.
// Identity (proven): Ego_r.pdb 0x00771990 / FableWin.pdb 0x025D50C0 ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@...@Z is the
//   function right before ?GetFirstPointThatBreaksConvexity@NShape@@ in both PDBs, as 0x007602F0 sits right before the landed
//   NShape::GetFirstPointThatBreaksConvexity 0x00760760; the body is that convexity test (the XY turning angles add up to one turn).
//   Catalogue placeholders Math_PolygonAngleSumClosureCheck / CSplineTrack_ValidateDistance are wrong. No direct caller in retail.
// Callees: _CIacos 0x00BFEBA2 (three acos calls); inline C3DVector::Normalise (Newton reciprocal square root: 3.0f 0x0122DED4,
//   0.5f 0x0122F59C, 0.0f 0x0122DEDC); INV_TWO_PI 0x01237F08 (0.15915494); 1.0f 0x0122DED8; 0.0025f 0x0126A200.
// Shape: Ego_d locals and scopes (pts, result, it_first, it_second, it_finish_him, first, second, last_line, this_line + a loop-scope this_line);
//   the loop body post-increments both iterators (Inferred: retail adds before the acos call; Ego_d 315/316 = 87 B each, uncalibrated); it_finish_him = pts.end() is hoisted into ebx.
// Inferred: `double closing` and `float closing_angle` are not in Ego_d's local list. They are fitted: the qword spill of the closing1 dot
//   (0x007605DB) and the `fst` after its acos (0x0076071A).
// Normalise body: existing opt-ins NORMALISE_SQUARE_HELPER + SQUARE_GROUPED_PASS23 (the 0x00A58B50 set). They close the last_line copy schedule at
//   0x007605AF, which is a region-3 cut. PAREN_SQUARES_XYZ gives the same bytes.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

extern const float INV_TWO_PI;                       // 0x01237F08 (1 / (2 pi))

namespace NShape {

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& pts);

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& pts)
{
    std::vector<C3DVector>::const_iterator it_first = pts.begin();
    std::vector<C3DVector>::const_iterator it_second = it_first + 1;
    C3DVector first = *it_first++;
    C3DVector second = *it_second++;
    C3DVector last_line(second.X - first.X, second.Y - first.Y, 0.0f);
    last_line.Normalise();
    float result = 0.0f;
    C3DVector this_line;
    std::vector<C3DVector>::const_iterator it_finish_him = pts.end();
    while (it_second != it_finish_him) {
        first = *it_first++;
        second = *it_second++;
        C3DVector this_line(second.X - first.X, second.Y - first.Y, 0.0f);
        this_line.Normalise();
        result += (float)acos(this_line.Dot(last_line)) * INV_TWO_PI;
        last_line = this_line;
    }
    first = *(it_finish_him - 1);
    second = *pts.begin();
    this_line = C3DVector(second.X - first.X, second.Y - first.Y, 0.0f);
    this_line.Normalise();
    double closing = this_line.Dot(last_line);
    last_line = this_line;
    first = pts[0];
    second = pts[1];
    this_line = C3DVector(second.X - first.X, second.Y - first.Y, 0.0f);
    this_line.Normalise();
    float closing_angle = acos(closing);
    result = acos(this_line.Dot(last_line)) * INV_TWO_PI + (closing_angle * INV_TWO_PI + result);
    result -= 1.0f;
    return result * result < 0.0025f;
}

}
