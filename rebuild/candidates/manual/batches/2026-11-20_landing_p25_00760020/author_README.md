# 0x00760020 NShape::IsLineIntersectingWithOthersInList (p25L, Account B)

**Grade:** RELOCATION_MATCH (360/360), `// Flags: /Gr` (the NShape TU's fastcall default; FableWin obj `debug\tc_geometric_shapes.obj`).
- **Strict vland:** passes (`LANDVERIFY_STRICT_HEADERS=1`, overlay `hdr/`, fresh LANDVERIFY_WORK; `vland.log`). Its one RELOC_NAME warning is the existing DELTA_NOTIONAL_ZERO label record, repeated in NAME_FIXES.tsv.
- **Real-object test:** passes (`test/`). It links the row object, with C2DLineF::IntersectsWith stubbed as a segment-crossing test that logs every edge it is called on. It checks 10 polygon/line cases against an independent model: crossing counts 6 / 5 / 4 / 3 / 2, a line equal to an edge in both directions (within the tolerance), the closing edge, and a degenerate list of equal edges. For each case it checks the result, the call count (equal edges must not call IntersectsWith) and each called edge's start and end.
- **Mutants:** 18 of 18 killed (`mutants/`).
- **Header:** `hdr/` is the current repo header plus add-only opt-ins (`hdr.diff`), and it is shared with batch 00760190:
  - **New engine/NShape.h**, built from Ego_d TPI 0x3F5BC (ECirculationOrder) and 0x403B5/0x403B6 (the predicate: PointA / PointB `const C3DVector&`, UpDirection C3DVector, sizeof 0x14, asserted), and Ego_r.pdb `?...@NShape@@YI...` declarations. No landed row includes it.
  - **C2DLineF.h:** FABLE_RETAIL_C2DLINEF_FROM3D (p25J; FableWin ??0C2DLineF@@QAE@ABVC3DVector@@0@Z) and new FABLE_RETAIL_C2DLINEF_INTERSECTS_LINE. The latter declares `bool IntersectsWith(const C2DLineF&, C2DVector&, float) const`, ?IntersectsWith@C2DLineF@@QBE_NABV1@AAVC2DVector@@M@Z, which is in the FableWin and Ego_r name lists and agrees with the landed 0x00A56C80.
  - C2DLineF.h has no static asserts, so nothing was pinned. `newly_flagged_rows.tsv` is empty: both opt-ins are gated, and the new header has no includers.
- **Where it came from:** this is a fresh port with Ego_d names, which matched on the first grade. The old draft (pending/2026-10-30_parked_retry2_sec2b/differ/00760020.cpp, DIFFER(367v360)) used a local IsSamePoint helper and a named intersection local, neither of which is in Ego_d. FractureShape 0x00763060 needs this body as a same-TU copy (see that row's park notes).

## Ablation table (final source minus one item; sources in `evidence/A*.cpp`; they were cut from the pre-final text that still had the neutral SQUAREDMAGNITUDE_YX define, A2 being the final text)
| change | grade |
|---|---|
| final | RELOCATION_MATCH |
| no FABLE_RETAIL_C2DVECTOR (the retail packed C2DVector branch, which declares DELTA_NOTIONAL_ZERO) | SRC_FAIL |
| + FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX | RELOCATION_MATCH (neutral, not added) |
| named `C2DVector intersection;` local instead of the temporary | RELOCATION_MATCH (neutral; Ego_d has no such local) |
| `start_pos = *it; ++it;` / `end_pos = *it; ++it;` | RELOCATION_MATCH (neutral; Ego_d line 90 is 68 B, the post-increment temporary) |
| `if (count > 4) return true; return false;` | DIFFER(358v360), 5 lines |

**Inline-budget: none.**
