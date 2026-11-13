# acctB hand-back, near-miss lane 3010: 0x00A4E300 matches -- CONDITIONAL on one fitted header body

Three successive Opus 5.5 agents worked this row in one directory (the owner's rule: the near-miss lane stays on a row
until it grades as a match); the last step is the acctB local coordinator's. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00A4E300 `?GetDeltaMovement@C3DAnimation2@@UBEXAAVCMatrix3x4@@MM@Z` | **RELOCATION_MATCH 1619v1619, test PASS (g++ and VC7.1) -- only with the FITTED `CMatrix3x4::SetMultiplyNoTranslation` body below** | supersedes `2026-11-11_acctB_c2014/rows/parked/00a4e300.cpp` and `2026-11-11_acctB_r3004/rows/parked/00a4e300.cpp`. With the 9-sum body the row is DIFFER 1615v1619 (`rows/parked_9sum/`) |

Coordinator verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no warnings). Purity and padded-struct greps are clean; no flags, no pragma. The three landed
users of the changed `TransformVector(C3DVector&)` text regrade RELOCATION_MATCH under `hdr/` (0x00A11AE0, 0x00A11C20,
0x00A22F80). No landed row uses `SetMultiplyNoTranslation`.

## The ruling this needs

**The fitted body** (`hdr/engine/CMatrix3x4.h`, under the existing `FABLE_RETAIL_CMATRIX3X4_MULTIPLYNOTRANSLATION`
opt-in, which only this row uses; it now also needs `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY`):

```cpp
FABLE_FORCEINLINE void SetMultiplyNoTranslation(const CMatrix3x4& m1, const CMatrix3x4& m2) {
    SetMultiply(m1, m2);
    E41 = E42 = E43 = 0.0f;
}
```

instead of nine explicit 3x3 sums followed by the same clear.

**For it**
- Compiled out of line it equals retail's unreferenced copy 0x00A4B820 byte for byte (240 bytes; coordinator's run of
  the agents' `oolcmp.py`). The 9-sum body does too, so that copy cannot tell them apart.
- The row can. With the 9-sum body everything is equal except 13 x87 commutative operand orders (nine product sums at
  stores 0x00A4E6FB..0x00A4E810, the second quaternion norm at 0x00A4E578) and 4 bytes. The agents traced the decision
  to c2's add-chain linearizer (0x1075FC1C; operands merge-sorted by a key whose low 16 bits hash the operands' record
  ids, so the order depends on how many c2 records the function has allocated before it) and built a model that
  reproduces our orders and predicts retail's when the late record ids shift by +59..+88. A dead-statement diagnostic
  confirms it (never delivered). This body supplies that IL: the three translation sums are computed, then overwritten,
  and c2 removes them after the linearizer has run. Row: 399 of 399 instructions equal, 1619 bytes.
- It is ordinary source: "multiply, then clear the translation" is how one would write a NoTranslation variant on top
  of an existing SetMultiply.

**Against it**
- FableWin.pdb's debug COMDAT `?SetMultiplyNoTranslation@CMatrix3x4@@QAEXABV1@0@Z` (0x032289C0) is 160 /Od
  instructions. Our 9-sum body gives 157 (plus a float artefact); this body would be about 15; a 12-sum-then-clear text
  (the last agent's own candidate, `candidate_g04` in the lane, also RELOCATION_MATCH) would be 211. So in FableWin's
  revision the function is the 9-sum body. FableWin is a later (VC10) revision of the source, and its debug
  GetDeltaMovement (119 instructions) already differs from what TLC retail needs, so this is strong evidence about that
  revision, not proof about TLC's.
- The agents measured about 470 natural spellings and header variants with the 9-sum body; none moves the orders. Other
  untried sources of the missing records are listed in `NOTES.md` ("Ranked ideas after session 3": TLC-era assert macros
  or counted-pointer accessors that leave IL in release, more inline layers in the quaternion conversion).

If you rule against the body, the row is parked at `rows/parked_9sum/` (source, test, the 9-sum header) with exactly
that residue, and everything else in this folder still stands.

## Proven on the way (all in `hdr/`, `LESSONS.md`, `LABELS.tsv`)

- `Transpose()` as three `SWAP<float>` calls (`template<class T> inline void FABLE_FASTCALL SWAP(T& a, T& b)`):
  VC7.1 /Od gives 22 + 18 instructions, exactly FableWin's debug COMDATs 0x025C21B0 / 0x025C2DA0; in the row it is the
  shape that spills E12 / E13 and reloads E23 (0x00A4E567 / 0x00A4E571) and gives retail's frame 0x104.
- `PreScale(float, float, float)` is expanded inside retail's out-of-line `C3DKeyframe::ToMatrix` 0x00A52450 (447 v 447
  relocation-equal when forced; 341 as plain inline), so it is `FABLE_FORCEINLINE`.
- `TransformVector` X row grouped `xx*E11+(yy*E21+zz*E31)` (retail 0x00A4E8A5; same grouping as TransformPoint).
- Clamps through `GFLimit(start_time, 0.0f, duration)` (both PDBs).
- `GetNonLoopingDuration` has a retail out-of-line copy at 0x00A4BE10 (relocation-equal); the header comment saying it
  has none is stale.
- Still unproven, carried over from chunk 2014: `FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED`.

## How to land (if accepted)

`rows/00a4e300.cpp` + `rows/00a4e300.test.cpp` with `hdr/engine/` (CMatrix3x4.h is the file that matters; the others
are the chunk 2014 / 2024 overlay this row has always used), then `payload.json` through your usual vland.
