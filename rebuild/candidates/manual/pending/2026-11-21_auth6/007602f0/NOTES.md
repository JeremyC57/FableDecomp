# 0x007602F0 NShape::IsShapeConvex -- MATCH, Ego_d fully decoded (AUTH6, 2026-11-21)
Source `007602f0.cpp` (flags /Gr, overlay `../hdr` = AUTH5's GFAngles.h PAREN_CAST opt-in, hdr.diff unchanged from
$S/w22/auth5/007602f0/hdr.diff), test `007602f0_test.cpp` (vt.py PASS, links row.obj). Grade RELOCATION_MATCH.
## Route
AUTH5's text (25/27 Ego_d lines exact, MATCH with PAREN_CAST) + the two open lines decoded under VC10 SP1 (--lib fablelib --stl):
- 313 `while ((it_second != pts.end()) != false)`: 100 B exact (the bool compared with zero: movzx/neg/sbb/neg). Same IL, also 100:
  `!= 0`, `!!(..)`, `false != (..)`. The spelling inside that family is chosen, not proven.
- 342 `result -= 1.0f; return (float)(result * result) < 0.0025f;`: 122 B exact (explicit narrowing: fstp/fld via a tv slot).
  Same IL, also 122: float(..), static_cast<float>(..). Spelling inside the family chosen.
- With both, every Ego_d line (27), the length (1096), the named frame and the loop S_BLOCK32 (+0x179, 251 B) equal Ego_d.
- Object lifetime (the advisor's lead) does not fit: the 12 B equal one dtor group numerically, but a fourth live iterator needs an object
  Ego_d's frame / symbol list lacks; the decodes above are frame-neutral (tv slots below pts).
- VC7.1 3077 /O2 folds both constructs: RELOCATION_MATCH with either, both or neither.
## Opt-ins
- Inferred: FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST (overlay; the only Inferred item; ablation 12 lines; /Od-neutral).
- Existing: FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + _SQUARE_GROUPED_PASS23, FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22,
  FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING. PAREN_CAST contract regrade of the 4 landed GFArcCos users: AUTH5 ../../auth5/gfreg (all MATCH).
## Names
All locals from Ego_d (pts, result, it_first, it_second, it_finish_him, first, second, last_line, this_line). No NAME_FIXES.
