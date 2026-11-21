// NShape::IsShapeConvex(const std::vector<C3DVector>& pts) -- add up the turning angles (in turns) between consecutive XY edge directions of the closed outline, closing edges included, and call the shape convex when they come to one full turn
// Grade: DIFFER(1126v1126) -- 2 diff lines, one residue in the epilogue: retail `pop esi ; fadd [esp+10h] ; pop ebp` (0x00760726), ours `pop ebp ; fadd [esp+0Ch]`.
//   It flips only with closing1's acos result held in a float variable that is multiplied by K later (bracket_closing_angle.cpp, which also needs a double local): both locals are absent from Ego_d.
//   Flat: 13 tail / return shapes, operand orders, all matching closing2 spellings, every Normalise opt-in, literal v extern K, /Gr; 1..40 literal int dummies at entry and tail.
// Leaf: ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007602F0, 1126 bytes, __fastcall (pts in ecx), returns bool; x87 throughout.
// Identity (proven): Ego_r.pdb 0x00771990 / FableWin.pdb 0x025D50C0 ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@...@Z is the
//   function right before ?GetFirstPointThatBreaksConvexity@NShape@@ in both PDBs, as 0x007602F0 sits right before the landed
//   NShape::GetFirstPointThatBreaksConvexity 0x00760760; the body is that convexity test (the XY turning angles add up to one turn).
//   Catalogue placeholders Math_PolygonAngleSumClosureCheck / CSplineTrack_ValidateDistance are wrong. No direct caller in retail.
// Callees: _CIacos 0x00BFEBA2 (three acos calls); inline C3DVector::Normalise (Newton reciprocal square root: 3.0f 0x0122DED4,
//   0.5f 0x0122F59C, 0.0f 0x0122DEDC); 1/(2 pi) pool literal 0x01237F08 (0.15915494f); 1.0f 0x0122DED8; 0.0025f 0x0126A200.
// Shape: Ego_d locals and scopes (pts, result, it_first, it_second, it_finish_him, first, second, last_line, this_line + a loop-scope this_line);
//   the loop body post-increments both iterators (Inferred: retail adds before the acos call; Ego_d 315/316 = 87 B each, uncalibrated); it_finish_him = pts.end() is hoisted into ebx.
// Inferred: closing2 holds begin+1 in the named iterator it_second (Ego_d local), assigned inside the statement so Ego_d's two lines 336/337 stay: retail
//   reads second.X off begin+12 before materialising it (`mov ecx,[edi+0Ch]` above `add edi,0Ch`, 0x007605F3). Plain `second = pts[1]` /
//   `*(pts.begin() + 1)` give DIFFER(1125v1126); any named iterator (it_first / it_second / it_finish_him) assigned after first's read matches.
// Inferred: closing1 and closing2 accumulate through GFArcCos (engine/GFAngles.h; FableWin.pdb ?GFArcCos@@YIMN@Z, retail copy 0x005DB180 18 B,
//   our COMDAT SAME): its double parameter is retail's qword spill of the closing1 dot (0x007605DB), deferred past closing2 (acos order
//   0x00760702 / 0x00760715). The loop keeps the raw (float)acos(...) * K: all three through GFArcCos = 8 lines (loop fmul placed at the call,
//   retail after the moves, 0x007604A1); all three raw = 234 lines (dword spill, frame 0x4C). Ego_d's 29-B lines 319/330/340 fit either spelling.
// Normalise body: existing opt-ins NORMALISE_SQUARE_HELPER + SQUARE_GROUPED_PASS23 (the 0x00A58B50 set). They close the last_line copy schedule at
//   0x007605AF, which is a region-3 cut. PAREN_SQUARES_XYZ gives the same bytes.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#include "engine/C3DVector.h"
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#include "engine/GFAngles.h"
#include "stlport/fable_containers.h"


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
        result += (float)acos(this_line.Dot(last_line)) * 0.15915494f;
        last_line = this_line;
    }
    first = *(it_finish_him - 1);
    second = *pts.begin();
    this_line = C3DVector(second.X - first.X, second.Y - first.Y, 0.0f);
    this_line.Normalise();
    result += GFArcCos(this_line.Dot(last_line));
    last_line = this_line;
    first = *pts.begin();
    second = *(it_second = pts.begin() + 1);
    this_line = C3DVector(second.X - first.X, second.Y - first.Y, 0.0f);
    this_line.Normalise();
    result += GFArcCos(this_line.Dot(last_line));
    result -= 1.0f;
    return result * result < 0.0025f;
}

}
