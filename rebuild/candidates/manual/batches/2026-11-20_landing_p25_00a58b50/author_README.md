# 0x00A58B50 NBBBMaths::CSphere::GetFirstIntersectionWithTriangle (p25I, Account B)

**Grade:** RELOCATION_MATCH (2207/2207), `// Flags: /Gr` (the TU flag, as the landed TU-mate 0x00A593F0 has it).
- **Strict vland:** passes (`LANDVERIFY_STRICT_HEADERS=1`, overlay `hdr/`, fresh LANDVERIFY_WORK; `vland.log`). Its two RELOC_NAME warnings are existing label records, repeated in NAME_FIXES.tsv: 0x0129BA3C DELTA_NOTIONAL_ZERO, and 0x00A5A9D0 std::transform, still labelled Vector_InsertMany_PairFloat in the DB vland reads.
- **Real-object test:** passes (`test/`). It links the row object with its own same-TU IsPointInsideTriangleInThePlane, transform, functor and STLport push_back. It checks 11 cases against an independent float model: crossing, vertex-inside and centre-inside paths, both x_axis branches, a second sphere radius, and five false cases.
- **Mutants:** 26 of 27 killed (`mutants/`). The survivor, `circle_rad_sqr = 0.0f` -> `1.0f`, is equivalent: the clamp cannot run, because `|distance| < Radius` is tested first.
- **fidcheck:** `$S/w26/p25H/tools/fidcheck_file.py` reports 1 copy checked, 0 differ.
- **Re-verified after the last comment edit:** payload rebuilt, strict vland into a fresh work dir, test, fidcheck and grade all rerun.
- **TU-mate check:** the landed 0x00A593F0 stays RELOCATION_MATCH with this row's four added C3DVector defines (C3DVECTOR, NORMALISE_SQUARE_HELPER, SQUARE_GROUPED_PASS23, ISNONZERO_ORIGINAL_YXZ), as it is without them.

## Fidelity line (deviation from main's wording)
The same-TU copy above the row is labelled `// Fidelity: same-TU strong definition of 0x00A58A60 (retail TU-mate)`, not `of 0x00A593F0`.
- 0x00A58A60 is IsPointInsideTriangleInThePlane's own address.
- The copy is the landed 0x00A58A60 body, verbatim.
- fidcheck resolves the body from the file of the address the line names. Run with `0x00A593F0` in the line, it reports "DIFFERS ... copy of 00a593f0 / 1 copies checked, 1 differ" (that file's copy has a trailing comment on the signature line).

## What closed it (draft v1: DIFFER(2211v2207), 159 diff lines)
1. **The same-TU IsPointInsideTriangleInThePlane is defined above the row**, with the landed text, the same EVIDENCED device the TU-mate 0x00A593F0 uses. That step alone goes from 159 to 16 lines.
2. **Ego_d shape** (local names and block nesting from `egod_lines/`; line sizes compared below, not a line-for-line fit):
   - Ego_d's names are used throughout.
   - `pt` and `int_pos` are named locals, as in Ego_d.
   - sqrt(desc) is written inside both lambda expressions: Ego_d has no sqrt line between 187 and 190.
   - The four magnitude tests are `IsNonZero()`: Ego_d lines 134 / 215 / 237 / 249 are 15 B each, and `if (v.IsNonZero())` is 15 B at /Od (the spelled-out test is 27 B). IsNonZero uses its FableWin donor body, FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ.
   - All of these are byte-neutral on their own.
3. **c_a and c_c are written through GFSqr** (`// Inferred:`, chosen by grading). Ego_d's lines are larger than either form (c_a 48 B: GFSqr form 31 B, inline 25 B; c_c 82 B: GFSqr 65 B, inline 53 B at /Od), so the line table only leans toward the call forms. GFSqr closes the c_c p1.X/p1.Y load order and the dead `lea eax,[edi+edx*8]` (the &p2) schedule at 0x00A58E3D.
4. **GFSqr's body** is the header's `(n*n)`: existing opt-ins FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + SQUARE_GROUPED_PASS23 (`// FITTED:`, chosen by grading). The gate also swaps Normalise's body, which this row never expands; the preprocessed diff against plain GFSqr is only GFSqr and Normalise.
5. **FABLE_RETAIL_C3DVECTOR**, the retail C3DVector branch used by 760 landed rows, puts c_b's two products in retail order.

## Ablation table (each = the final `src/00a58b50.cpp` minus one item, regenerated from it; sources in `evidence/abl_*.cpp`)
| change | grade |
|---|---|
| final | RELOCATION_MATCH |
| IsPointInsideTriangleInThePlane declared only (no same-TU definition) | DIFFER(2211v2207), 145 lines |
| c_c inline `p1.X*p1.X + p1.Y*p1.Y - Radius*Radius` | DIFFER(2207v2207), 10 lines (load order + lea) |
| c_a inline `line.X*line.X + line.Y*line.Y` | DIFFER(2207v2207), 2 lines (lea) |
| GFSqr `n*n` (no SQUARE_GROUPED_PASS23, or GFWorldDistance.h's copy) | DIFFER(2207v2207), 2 lines (lea) |
| no FABLE_RETAIL_C3DVECTOR (and IsNonZero spelled out) | DIFFER(2207v2207), 4 lines (c_b product order at 0x00A58E19) |
| IsNonZero with the default body (no ISNONZERO_ORIGINAL_YXZ) | DIFFER(2207v2207), 52 lines |
| IsNonZero spelled out as `GetSquaredMagnitude() > D*D` | RELOCATION_MATCH (neutral; kept for Ego_d's 15 B call lines) |
| separate `desc = sqrt(desc)` statement | RELOCATION_MATCH (neutral) |
| no `pt` / no `int_pos` / `p1 + line * lambda` | RELOCATION_MATCH (neutral) |
| `GFSqr(c_b)` in desc; spelled-out dot product for c_b; dead `circle_rad = sqrt(circle_rad_sqr)` | RELOCATION_MATCH (neutral; not added) |
| + FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE | RELOCATION_MATCH (neutral; not needed) |
| + FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE (declaration only) | DIFFER(2169v2207) |

**Inline-budget: none.** Plain C++ already calls push_back<C2DVector> (0x004428B0), so the draft's `template<> inline __declspec(noinline) push_back` specialization is dropped. This differs from main's suggestion to name NOINLINE on line 2: it is byte-neutral, so it is not a device. The draft's local back_insert_iterator / back_inserter declarations are replaced by the existing FABLE_RETAIL_STLP_BACK_INSERTER opt-in, also byte-neutral. The transform and functor are the TU-mate's landed text.

**Diagnostics (not in the source):**
- Before GFSqr, entry dummy locals moved c_b at 2..7 (mod 8), but never c_c or the lea.
- On the final source, 1..16 dummy locals at entry and 1..16 at the loop-body top all stay RELOCATION_MATCH: no symbol-number tie is left in the match.


## Ego_d /Od calibration (final text compiled /Od /GX against Ego_d line sizes; not a line-for-line fit)
| Ego_d line: bytes | ours at /Od | statement |
|---|---|---|
| 133: 82 | 80 | plane_normal cross |
| 134 / 215 / 237 / 249: 15 | 15 (IsNonZero; spelled-out 27) | the four magnitude tests |
| 137: 7, 139: 8, 165 / 168: 8 | 7, 8, 8 | return false, AccurateNormalise |
| 140 / 141: 24 / 24 | 22 / 23 | plane_dist, distance_plane_to_point |
| 143: 70 | 56 | the distance test |
| 150: 41 | 41 | fabs(X) > fabs(Y) |
| 166: 16 | 16 | y_axis cross |
| 176 / 177 / 178: 29 / 29 / 25 | 28 / 26 / 18 | p1, p2, line |
| 182 / 183 / 184: 48 / 43 / 82 | 31 / 21 / 65 (inline 25 / 21 / 53) | c_a, c_b, c_c |
| 186: 41, 187: 23 | 28, 18 | desc, if (desc > 0) |
| 190 / 191: 55 | 44 (sqrt inside; 25 without) | lambda1 / lambda2 |
| 196+197 / 202+203: 47+15 | 51+14 | int_pos, push_back(int_pos) |
| 225 / 226: 17 / 23 | 10 / 20 | pt, pt test |

**Not accounted for by the text:**
- 131 (359 B), 164 (361 B) and 167 (378 B): assert-sized lines with no retail code.
- 145..162, only partly: 147 (44), 152 (8), 154 (45) and 157 (14) have no counterpart. The then-branch of the x_axis choice is larger in Ego_d, and the clamp does not show as separate lines.
- 223 (128 B, the iterator for-header; ours 43) and 230 / 231 (15 / 10; ours one 9 B test).

Ego_d may be a later source revision here (compare 0x005BFD5E), so its sizes support shapes, never exact text.

## Header (add-only opt-in, `hdr/`, `hdr.diff`)
- **FABLE_RETAIL_CSPHERE_TRIANGLE in engine/CSphere.h:** the method declaration (Ego_r/FableWin mangled name above) and the fable_containers.h include. It is copied from the current repo header (CSphere.h is unchanged since 74380bdef).
- **Asserts:** FABLE_STATIC_ASSERT_AT(19, ...) is pinned; pin_asserts.py reports 0 further pins.
- **newly_flagged_rows.tsv:** header line only. No landed row includes CSphere.h, and the opt-in sits behind its own macro.

## Residual notes
- **circle_rad:** Ego_d lists it, but retail has no trace of it (a dead sqrt is byte-neutral), so it is left out.
- **Retail NaN:** the original takes x_axis.Y = -(0/0) for an exactly horizontal plane, giving NaN. The test rotates its cases off-axis and notes this in a comment.

## Coordinator review (2026-11-20, advisor ruling under the owner's delegation)
- The `// Fidelity:` copy's COMDAT is byte-equal to retail 0x00A58A60 (cmpcomdat on the strict-vland objects, 228 B, relocations only).
- Line 2 now names the header contracts (FABLE_VECTOR_INLINE helpers, plain-inline GFSqr, the called same-TU helper) in place of "none".
- The GFSqr label now cites the Ego_d line-length difference (34 B, matching the call form) and the unidentified common surplus on lines 182-184.
- `(n*n)` stays marked FITTED (no second witness). The one surviving mutant (`circle_rad_sqr = 0.0f` -> `1.0f`) is unreachable at run time;
  it changes the stored constant, so the bytes differ.
