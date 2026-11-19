# b6 (hand3c2, Account B, 2026-11-19): 0x00A567A0 CCardinalSpline::GetSplinePosition -- READY

- Grade: RELOCATION_MATCH (g.py + strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1). Purity: retail_asm (the shared
  GFFloatToLongFloor helper; `// Retail-asm:` line in the source; the row has no asm of its own).
  ATTEMPTS.log: 3 grades (SRC_FAIL -- fmod overload ambiguity, a `zs.py && g.py` chain that did not stop; DIFFER(766v766); RELOCATION_MATCH)
  + one PROBES line (~45 no-ATTEMPTS probes in probe/67a0). Flags: none.
- Leaf ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z, 766 bytes, `ret 8`, ebp frame.

## Identity
Ego_r.pdb (0x00BAE530) / FableWin.pdb (0x0324B730), same spelling; the shared header's FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
declaration. Locals exactly as Ego_d.pdb (one scope, debug length 2422): dest, s; num_points, num_points_sub_one, num_control_pts,
num_control_pts_f, from_index, to_index, from_pos, to_pos, previous_from_pos, next_to_pos, from_tan, to_tan, from_index_s,
to_index_s, this_s.

## What closed it
1. Port onto the shared CCardinalSpline.h / CArray.h / C3DVector.h with Ego_d's locals (the old parked draft returned by value, so
   its leaf was absent): 778v766.
2. FABLE_RETAIL_CARRAY_INDEX_ASSERT (existing opt-in; the header marks it INFERRED): from_pos / to_pos copied through the x87 as
   retail (`fld [ecx+eax] ; fstp [ebp-4Ch]`); 205 -> 128 differing instructions.
3. FITTED spellings (two `// Inferred:` lines): num_points_sub_one defined before the EndPointsConnected test but the open spline's
   last point indexed `num_points - 1`; both neighbour tests written wrap-case first (`if (from_index <= 0) { .. } else ..`,
   `if (to_index >= num_points_sub_one) { .. } else ..`; `!(x > 0)` / `!(x < ..)` tie), the wrapped previous point
   `from_index + num_points_sub_one`. Each checked singly after the match (p1..p7 in probe/67a0: every natural alternative DIFFERs).
4. The closing lever (advisor): GFGetHermiteSplinePosition's body in this TU ahead of the row (its retail TU-mate: 0x00A56670 + 295 =
   0x00A56797, the row at 0x00A567A0; the landed 0x00A56670 names this row its only caller). With a declaration only, the tangents'
   addresses go to an opaque callee and from_tan / to_tan never share the dead previous_from_pos / next_to_pos slots (frame 0x64 v
   0x58). It stays a call (0x00A56A90). Same device as 0x00B1D790 (same-TU DecalCollides body) and GOTCHAS 177.

## Headers
No header change: CCardinalSpline.h GETSPLINEPOSITION and CArray.h CARRAY_INDEX_ASSERT are existing opt-ins.

## Test (test/00a567a0.cpp)
Links the REAL row object (the same-TU Hermite body and the floor helper run as compiled). Raw native-layout spline; an independent
double-precision model (relative tolerance 2e-4; the GF floor emulated bit-exactly for the segment index). 5666 cases: every count
0..8 x open / closed x 4 tensions x 22 parameters (negatives, 0, 1, just inside / outside, wrap), every exact segment boundary j /
count, infinite end points that make the open clamps observable (the blend would give NaN), 4000 random splines.
Prints `00a567a0_TEST PASS`.

## Mutants (mutants.tsv / mutants.log): 45 / 47 KILLED, 2 equivalent
Survivors: `EndPointsConnected && from_index > to_index` -> drop `EndPointsConnected &&` (an open spline has from <= n-2, to = from + 1,
so from > to never holds), and `>` -> `>=` (from == to needs n == 1, which returns earlier). Both are equivalent by construction.

## Coordinator items
- The row's TU carries a copy of the landed 0x00A56670 GFGetHermiteSplinePosition text (same-TU body; one retail address, one
  name, unchanged spelling). At landing either keep both rows in one TU or accept the duplicate body as 0x00B1D790's.
- __CIfmod relocates to the MSVCR71 import thunk 0x00BFEB28 (vland RELOC_ALIAS, expected).

## Files
src/00a567a0.cpp, test/00a567a0.cpp, payload.json, oracle.tsv, mutants.tsv, mutants.log, vland.log, NAME_FIXES.tsv (empty).
