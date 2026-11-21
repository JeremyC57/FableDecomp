// NShape::IsShapeConvex(const std::vector<C3DVector>& pts) -- add up the turning angles (in turns) between consecutive XY edge directions of the closed outline, closing edges included, and call the shape convex when they come to one full turn ((sum - 1)^2 < 0.0025)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007602F0, 1126 bytes, __fastcall (pts in ecx), returns bool; x87 throughout. Grade: RELOCATION_MATCH (/Gr).
// Leaf: ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Flags: /Gr
// Identity (proven): Ego_r.pdb 0x00771990 / FableWin.pdb 0x025D50C0 ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@...@Z sits right before
//   ?GetFirstPointThatBreaksConvexity@NShape@@ in both PDBs, as 0x007602F0 sits right before the landed NShape::GetFirstPointThatBreaksConvexity
//   0x00760760 (tc_geometric_shapes.obj). No direct caller in retail.
// Callees: _CIacos 0x00BFEBA2 (three, through GFArcCos); inline C3DVector::Normalise (GFFastOneOverSqrt: table 0x013CE558, 3.0f 0x0122DED4,
//   0.5f 0x0122F59C); 1/(2 pi) 0x01237F08 (0.15915494f); 0.0f 0x0122DEDC; 1.0f 0x0122DED8; 0.0025f 0x0126A200.
// Text = Ego_d's (VC10 SP1, odall10.py --lib fablelib --stl, standalone): ALL 27 Ego_d lines byte-exact (1096 B), named frame (S_BPREL32)
//   exact, loop S_BLOCK32 exact (+0x179, 251 B). Each point read clears its Z (`first = *it_first++; first.Z = 0.0f;`: Ego_d 307/308 79,
//   315/316 87, 326/327/336 33, 337 37), so the `second - first` edges (309/317 24, 328/338 44; the three-float ctor is 66/86) carry a
//   constant Z: retail's two-square Normalise. 305 `it_second = it_first; it_second++;` 38; 334 it_finish_him = pts.begin() 19; GFArcCos at
//   all three sites (319/330/340 29 B; raw acos 36).
// Inferred: two Ego_d-sized spellings (ADVISOR_DECISIONS_B 2026-11-21, the 0x008DBC80 size test: the plain texts cannot fit Ego_d).
//   313 (Ego_d 100 B v the plain test's 91): the loop test compares the bool with false (`!= false`: movzx/neg/sbb/neg, +9 B). Ego_d fixes
//   that IL family, not its spelling: `!= 0`, `!!(..)` and `false != (..)` are the same IL and 100 B too; `== true` is 104. Ego_d's loop
//   block starts 9 B later than the plain text's, same length.
//   342 (Ego_d 122 B v 110): the square is narrowed explicitly (`(float)(result * result)`: fstp/fld through a tv slot, +12 B); float()
//   and static_cast<float> are the same IL. Both spellings are frame-neutral (pts stays at -220) and VC7.1-neutral (/O2 folds both:
//   RELOCATION_MATCH with or without them), so retail's bytes do not select them; only Ego_d's sizes do. Not object lifetime: an extra
//   live iterator at the return needs an extra 12 B object that Ego_d's frame lacks.
// Inferred: GFArcCos body with the narrowed arc cosine in its own parentheses (add-only opt-in FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST,
//   engine/GFAngles.h, p25Q / AUTH5; ADVISOR_DECISIONS_B 0x007602F0). Ablation: the default body leaves 12 lines (loop fmul after the last_line moves 0x007604A1, epilogue
//   pops before `fadd [esp+10h]` 0x00760726). /Od sizes are equal, so Ego_d cannot tell the two bodies apart.
// Opt-ins (existing): FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + _SQUARE_GROUPED_PASS23 (Normalise body; ablation: none 30 lines,
//   HELPER only 15 lines); FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22 (STLport 4.6.2 const front(), which Ego_d 327/336 use).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#include "engine/C3DVector.h"
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST
#include "engine/GFAngles.h"
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22
#include "stlport/fable_containers.h"


namespace NShape {

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& pts);

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& pts)
{
    std::vector<C3DVector>::const_iterator it_first = pts.begin();
    std::vector<C3DVector>::const_iterator it_second = it_first; it_second++;
    C3DVector first = *it_first++; first.Z = 0.0f;
    C3DVector second = *it_second++; second.Z = 0.0f;
    C3DVector last_line = second - first; last_line.Normalise();
    float result = 0.0f;
    while ((it_second != pts.end()) != false)
    {
        first = *it_first++; first.Z = 0.0f;
        second = *it_second++; second.Z = 0.0f;
        C3DVector this_line = second - first; this_line.Normalise();
        result += GFArcCos(this_line.Dot(last_line));
        last_line = this_line;
    }
    C3DVector this_line;
    first = pts.back(); first.Z = 0.0f;
    second = pts.front(); second.Z = 0.0f;
    this_line = second - first; this_line.Normalise();
    result += GFArcCos(this_line.Dot(last_line));
    last_line = this_line;
    std::vector<C3DVector>::const_iterator it_finish_him = pts.begin();
    first = pts.front(); first.Z = 0.0f;
    second = *++it_finish_him; second.Z = 0.0f;
    this_line = second - first; this_line.Normalise();
    result += GFArcCos(this_line.Dot(last_line));
    result -= 1.0f; return (float)(result * result) < 0.0025f;
}

}