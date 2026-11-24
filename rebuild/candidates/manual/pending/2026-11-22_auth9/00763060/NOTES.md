# 0x00763060 NShape::FractureShape: RELOCATION_MATCH (1146 B, /Gr), test PASS (AUTH9, 2026-11-22)

Source `NShape_FractureShape_00763060.cpp`, test `NShape_FractureShape_00763060_test.cpp` (vt.py PASS). Graded with VC7.1 3077 through
rg1.sh (Leaf / Flags read from the file). Variant log in LOG.md, variants in t/.

## Route
- Text: Codex pass 26's FractureShape from 0x007634E0's TU, verbatim. AUTH8 (`$S/w22/auth8/RESULT.md`): Ego_d-exact on lines 195-241 and
  on all 23 named locals under VC10 (odall10 --stl); its remaining whole-function gaps (280, 283, debug-only 288/291) are the same as g1's.
- Declaration-only callees measured DIFFER (all callees declared 1164v1146; only 0x00760020 declared 1156v1146; only 0x00760190 declared
  1164v1146). So the row carries both NShape TU-mates as same-TU strong definitions (GOTCHAS nf96a), each the owner row's body verbatim
  (0x00760020 `IntersectsWith(line, C2DVector(), ...)`, 0x00760190 `line.CrossWithUp()`, owner tokens incl. `__fastcall`), which needs
  `FABLE_RETAIL_C3DVECTOR_CROSSWITHUP` (the owners' own opt-in). In this object both copies grade RELOCATION_MATCH at their own addresses,
  and their relocation lists equal the owner rows' objects (6/6 and 4/4 symbols).
- Prelude = 0x007634E0's opt-ins minus what FractureShape does not use: dropped the `__uninitialized_fill_n` FABLE_FORCEINLINE device,
  NShape_CBasicShape.h / NShape_CSubdivisionTriangle.h and NSHAPE_CONSTRUCTOR_BODIES_PASS26 / CCOUNTEDPOINTER / DESTROY_AUX_FASTCALL
  (each prune regraded and rechecked). PASS22_LOWER_BOUND_COMPARE/BODY stay: they supply STLport's random-access std::distance.
  No header change.

## Checks
- Leaf relocations (`../tools/leafrel.py`, retail target read from Fable.exe at each relocation): 9 code COMDATs/copies defined in the
  object, all EQ with retail at their targets (find_if 0x00763F50 [no table line], vector copy ctor 0x006F0810, _M_insert_overflow 0x0066E370
  / 0x00765B70, vector<vector> push_back 0x00765C80, __copy 0x0074CA20, the two TU-mates); 2 float literals (__real@3f800000 = 1.0f at
  0x0122DED8, __real@bf800000 = -1.0f at 0x0122DEE0); 1 external `_free` BOUND in reloc_bindings at 0x00BFEA14. PROBLEMS 0.
- Pick-any (codex27 pickany_vs_retail.py, new-row rule 2026-11-21): 11 function COMDATs mapped elsewhere, 11 EQ, 0 NEQ.
- Link note: the FractureShape body is also a strong TU-mate copy in 0x007634E0's object (and the two callees in both), so the full
  link gets LNK4006 on them with identical bytes, as for the other NShape TU-mates.

## Names (all two-source)
- FractureShape leaf: exact string in ghidra_out/egor_pdb_names.tsv (Ego_r, release\tc_geometric_shapes.obj S_GPROC32 00774200) and
  ghidra_out/fablewin_pdb_names.tsv (025D4510). Parameters / locals: Ego_d S_BPREL32 (AUTH8 FRAME.txt): test_shape, circ, convex_shapes,
  concave_shapes, duplicate_shape, it_start, a, it_anchor, b, found_appropriate, quit, it_result, it_begin, distance, ... (Codex's text).
- TU-mates: their owner rows' Leaf lines (Ego_r + FableWin). No descriptive names. No NAME_FIXES.
- The FableWin 025D45xx instruction claims in the source's Fidelity comment are Codex's; not re-checked on the lane (no FableWin code image).

## Test
Self-contained (NShape.h's CCountedPointer specialisations are strong, so a /Od test object cannot link beside the row object: LNK2005);
the row text verbatim plus controlled predicate / IntersectsWith stubs. Codex's triangle and square cases (now asserting the square's
splice_one) plus: a hexagon whose first candidate is rejected by IsLineIntersectingWithOthersInList (second find_if pass from it_begin,
ear distance 3, both splices pushed); a hexagon where it_begin advances one point per pass (offers p2,p3 | p3 | p4: distance 4, both
splices); a concave pentagon whose first candidate is rejected by IsTriangleWithPointsInside.

## 0x007634E0 (recheck, `../007634e0/`)
- P1.patch (required): only the Fidelity comment on its FractureShape copy (now the TU-mate of 0x00763060). RELOCATION_MATCH; pick-any 18/18 EQ.
- P2.patch + P2_test.patch (optional, coordinator's call): also replaces its two Codex callee copies with the owner-verbatim bodies (+CROSSWITHUP).
  UpdateSubdivisions RELOCATION_MATCH; in that object FractureShape, 0x00760020, 0x00760190 all RELOCATION_MATCH; pick-any 18/18 EQ;
  the patched test PASS (the unpatched repo test also PASS).
