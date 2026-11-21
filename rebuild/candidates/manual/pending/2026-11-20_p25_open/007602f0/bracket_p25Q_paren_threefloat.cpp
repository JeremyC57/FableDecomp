// NShape::IsShapeConvex(const std::vector<C3DVector>& pts) -- add up the turning angles (in turns) between consecutive XY edge directions of the closed outline, closing edges included, and call the shape convex when they come to one full turn
// Bracket: RELOCATION_MATCH (/Gr, p25Q 2026-11-21) -- NOT landable as is (advisor 2026-11-21): the four three-float-ctor edge lines
//   contradict Ego_d (size test below: 41 / 58 B at VC10 against Ego_d's 24 / 44 B; first/second/this_line/last_line are all C3DVector, TPI
//   0x2460, so no 2D type fills the gap). Row stays reserved; this text is kept as the bracket. One header device: FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST (engine/GFAngles.h, ADD-ONLY
//   opt-in): GFArcCos's body with the narrowed arc cosine parenthesised, `((float)acos(value)) * 0.15915494f`. Without it DIFFER(1126v1126),
//   8 lines: the loop's fmul by 1/(2 pi) issues right after `call _CIacos` (retail after the three last_line copy loads, 0x007604A1) and
//   three epilogue pops precede `fadd [esp+10h]` (retail two, 0x00760726). Other header contracts: FABLE_RETAIL_C3DVECTOR with
//   NORMALISE_SQUARE_HELPER + SQUARE_GROUPED_PASS23 (existing; the 0x00A58B50 set), GFANGLES_SIGNED_ROUNDING (existing; GFArcCos), and
//   the TU-mates' set from 0x00760020 / 0x00760190 (C2DVECTOR, C2DVECTOR_FROM3D, C2DLINEF_FROM3D, C2DLINEF_INTERSECTS_LINE,
//   C3DVECTOR_CROSSWITHUP; byte-neutral here). Ablations in the batch README.
// Flags: /Gr
// Leaf: ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007602F0, 1126 bytes, __fastcall (pts in ecx), returns bool; x87 throughout.
// Identity (proven): Ego_r.pdb 0x00771990 / FableWin.pdb 0x025D50C0 ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@...@Z is the
//   function right before ?GetFirstPointThatBreaksConvexity@NShape@@ in both PDBs, as 0x007602F0 sits right before the landed
//   NShape::GetFirstPointThatBreaksConvexity 0x00760760; the body is that convexity test (the XY turning angles add up to one turn).
//   Catalogue placeholders Math_PolygonAngleSumClosureCheck / CSplineTrack_ValidateDistance are wrong. No direct caller in retail.
// Callees: _CIacos 0x00BFEBA2 (three acos calls, GFArcCos expanded); inline C3DVector::Normalise (Newton reciprocal square root:
//   GFastOneOverSqrtTable 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC); 1/(2 pi) pool literal 0x01237F08 (0.15915494f);
//   1.0f 0x0122DED8; 0.0025f 0x0126A200.
// Shape: Ego_d.pdb's locals and scopes exactly (pts, result, it_finish_him, it_first, last_line, this_line, second, it_second, first, and
//   a loop-scope this_line; it_* are const_iterators, TPI 0x8B0C) and its line table line for line: the body's 26 code lines sit at
//   Ego_d's 304..342 offsets (304 it_first 22 B, 305 it_second, 307/308 first/second, 309 last_line, 311 result, 313 the loop test
//   (100 B: an end() temporary, so no hoisted end), 315..322 the loop, 324 `C3DVector this_line;` (8 B, a default-ctor call, after the
//   loop), 326..332 closing 1, 334 it_finish_him (19 B = an iterator declaration from begin()/end() plus a byte EH-state store, as
//   GetFirstPointThatBreaksConvexity's second iterator declaration, Ego_d 363 19 B), 336..340 closing 2, 342 one return statement.
//   Ego_d has no Normalise lines (310/318/329/339) and no 341: the Normalise calls share their vector's line, the tail is one statement.
// Edge text (the open conflict): the four edge vectors are built with the three-float ctor, (dx, dy, 0), then normalised. Ego_d's lines (309/317: 24 B,
//   328/338: 44 B) decode as `second - first` plus one 8-B member call on the same line (VC10 /Od op- with RVO 16 B, disp32 temp +3 for
//   the assignments), i.e. the 2012 text `v = second - first; v.Normalise();`; the three-float ctor is 34 B there. Retail never reads
//   first.Z / second.Z and stores Z = scale * 0.0f with a two-square sum, so its text zeroes Z before the normalise. Graded on this
//   text: Ego_d's literal spelling DIFFER(1181v1126); op- + `v.Z = 0.0f;` at all four sites DIFFER(1114v1126), at the first or the loop
//   site only DIFFER(1130v1126). No 2D normalise exists to fill the 8 B: Ego_d's C3DVector field list (TPI 0x245F) has none, and no
//   Ego_d procedure is a fastcall free function taking one C3DVector& (searched `@@YI...AAVC3DVector@@@Z` and every C3DVector member).
//   Size test: VC7.1 /Od (odl2) gives the ctor declaration 30 B and the assignment 47 B, +3 at VC10 for the 0.0f ctor argument, + 8 for
//   the Normalise call on the same line = 41 / 58 B; Ego_d has 24 / 44 B. Ego_d contradicts the three-float form.
// Inferred: closing 2 holds begin+1 in a named iterator assigned inside the statement, `second = *(it_second = it_finish_him + 1)`:
//   retail reads second.X off begin+12 before materialising it (`mov ecx,[edi+0Ch]` above `add edi,0Ch`, 0x007605F3).
//   `second = *(it_finish_him + 1)` / `it_finish_him[1]` / `pts[1]` give DIFFER(1125v1126); `++it_finish_him` forms DIFFER(1127v1126).
//   it_finish_him's initialiser (begin()) and its use as closing 2's base are fitted to Ego_d 334's position and size; the bytes do not
//   fix them (an unused `= pts.end()` there, or the earlier hoisted `= pts.end()` before the loop, grade the same).
// Closings 1 and 2 accumulate through GFArcCos like the loop (Ego_d 319/330/340 are 29 B, the GFArcCos call form): its double parameter
//   is retail's qword spill of the closing-1 dot (0x007605DB), deferred past closing 2 (acos order 0x00760702 / 0x00760715).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DLINEF_FROM3D
#define FABLE_RETAIL_C2DLINEF_INTERSECTS_LINE
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#include "engine/NShape.h"
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST
#include "engine/GFAngles.h"


namespace NShape {

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& pts)
{
    std::vector<C3DVector>::const_iterator it_first = pts.begin();
    std::vector<C3DVector>::const_iterator it_second = it_first + 1;

    C3DVector first = *it_first++;
    C3DVector second = *it_second++;
    C3DVector last_line(second.X - first.X, second.Y - first.Y, 0.0f); last_line.Normalise();

    float result = 0.0f;

    while (it_second != pts.end())
    {
        first = *it_first++;
        second = *it_second++;
        C3DVector this_line(second.X - first.X, second.Y - first.Y, 0.0f); this_line.Normalise();

        result += GFArcCos(this_line.Dot(last_line));

        last_line = this_line;
    }

    C3DVector this_line;

    first = *(pts.end() - 1);
    second = *pts.begin();
    this_line = C3DVector(second.X - first.X, second.Y - first.Y, 0.0f); this_line.Normalise();

    result += GFArcCos(this_line.Dot(last_line));

    last_line = this_line;

    std::vector<C3DVector>::const_iterator it_finish_him = pts.begin();

    first = *it_finish_him;
    second = *(it_second = it_finish_him + 1);
    this_line = C3DVector(second.X - first.X, second.Y - first.Y, 0.0f); this_line.Normalise();

    result += GFArcCos(this_line.Dot(last_line));

    return (result - 1.0f) * (result - 1.0f) < 0.0025f;
}

}
