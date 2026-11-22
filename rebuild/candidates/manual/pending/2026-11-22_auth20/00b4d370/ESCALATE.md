# ESCALATE 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace (AUTH20, 2026-11-22)

Leaf `?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z`, retail 7176 B, flags `/d2inlT314` (as the landed sibling 0x00B4FC40;
stock gives DIFFER(6815v7176), PostMultiply out of line). Overlay `ov/` (AUTH19's three patches, copied unchanged).

## Best sources
- Evidence base: `best_evidence.cpp` (= x/p9.cpp): **DIFFER(7162v7176)**, nm2 147. It is the Ego_d / FableWin-faithful port.
- Lowest residue: `best.cpp` (= AUTH19 p5): **DIFFER(7176v7176)**, nm2 110. Regraded today as a control. It uses the default SetInverse and
  lacks FW lines 598 / 605.

## What AUTH20 did (4 grades + 1 probe; each costs about 1 s)
- The /Od line pass (VC10 odall10 engine preset v FableWin line table). Every statement line now matches FW except the ones listed under
  "Open Ego_d contradictions".
  - 408..412: weights order x0, y0, x1, y1.
  - 546: `1.0f + perspective_shift` (16 B).
  - 532: `Y * max_projected_range_x / fabs(X)` (48 B).
  - C2DVector::GetMagnitude default text (52 B = FW). FW lists no `squared` local, so FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL is dropped.
- Results:
  - p8 = p7 + weights, 546 and GetMagnitude: byte-identical to p7.
  - p9 = p8 + 532: nm2 147. One order site in the PostMultiply(perspective_transform) expansion flipped toward retail; the corner code is
    unchanged.
  - FW-shaped PostMultiply probe: byte-identical.
- **The order sites are id-driven.** A text change in the corner pass, whose own code VC7.1 emits identically, moved a site about 0x400 bytes
  later. This is the bit-A/B mechanism from the acctC wfE6 LESSONS: commutative operand order follows CSE temp ids.

## Remaining residue of p9 (nm2 -v in `p9_residue.txt`)
1. **x87 operand-order picks, all inside inline expansions.**
   - persp/else 13+12 (0x00B4E509..0x00B4E55E): the PostMultiply(perspective_transform) column association. Retail shares
     `(D3*0 + D2*0)` across columns; we add per column.
   - 0x00B4E5E9: both C2DVector::GetSquaredMagnitude in the else branch. Retail squares Y first (`fld st(0); fmul st(1)`), we square X first.
   - tail1 34+32:
     - diag C2DVector::GetMagnitude at 0x00B4E74A: retail Y*Y + X*X with `fsqrt; fstp st(2); fstp st(0)`.
     - The 26-point PerspectiveTransformPoint loop, 0x00B4E97E..0x00B4EAAF: the term order of `fld [E] ; fmul st(i)`.
   - end, 0x00B4EDC6..0x00B4EECB: in both last PostMultiply expansions retail adds the `Data[i][0]*m[0][j]` product last.
   - setinv 4+4 (0x00B4D464..0x00B4D476): the cofactor term order of the DONOR_PASS26 SetInverse.
2. **One stack-slot placement**, which also gives the frame 0x3c0 v 0x3d0, the grid26 displacements and the tail `lea`s.
   - Retail homes the LAST PostMultiply's 64-byte result at 0x100..0x13F, over `local_world_to_base_shadow_space`'s NRV home
     (0x100..0x12F, dead by then). Ours homes it at 0x8C, with the identity temp, the PostMultiply(CMatrix4x4(local)) result and the
     PostMultiply(perspective_transform) result. Retail also homes those three at 0x8C.
   - Frame offsets 0x74..0xD8 are slot-for-slot identical (slots.py). 0xD8..0x143 is the same set with local / small floats swapped. The
     16 B is the overhang of 64 over 48.
   - AUTH19 d7 (declare, then assign) gets frame 0x3d0 and exact grid slots, but adds a temp->local copy that retail lacks.

## Open Ego_d / FableWin contradictions (none closes a retail site)
- 590 diag: FW 96 v 68. 10 C2DVector / C3DVector / sqrt spellings measured 68..112, none 96.
- 644: FW 39 = a `this`-member 3x4 (stand-in measured 39). Retail reads the local's frame home, so FW's tail is a later revision.
- Loop headers: FW +6 (short) / +9 (near). Every counter type and spelling measures 35..41, so this is a build difference.
- 652 / 654 (17 + 29 B after CalcClipVolume): retail has nothing after the call at 0x00B4EF66 (an editor-only `#if`).
- 636 / 637: dead-arm asserts (225 B each in FW). Our /Od assert form is smaller. The release bytes are the dead arm.

## Header findings (needs header claim; not applied)
- FableWin ?GetMagnitude@C2DVector (52 B), ?GetSquaredMagnitude@C2DVector (43 B) and ?GetMagnitude@C3DVector (66 B) list only `this`, with
  no local. The texts that add a local contradict FW's local list:
  - `FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL`, `FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX`;
  - `FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQUARED_LOCAL` / `_NAMED_SQUARED` / `_XY_THEN_Z` / `_X_THEN_YZ` (named `squared`).
  - The C2D default (`sqrtf(X*X+Y*Y)`) measures 52 = FW.
- FableWin ?PostMultiply@CMatrix4x4 uses `matrix`, `temp_matrix` and `long v` / `long u` (braced). The shared header uses `m` / `result` /
  `int i,j`. Measured byte-neutral here (probe overlay `ovpm/`, opt-in `FABLE_RETAIL_CMATRIX4X4_POSTMULTIPLY_FW`, not handed back).
- `hdr_GFLimit.patch` (`FABLE_RETAIL_GFLIMIT_UPPER_LOWER`) redefines `GFLimitLower<T,L>`, which engine/CAnimSet.h line 6 defines unguarded.
  - TUs that define the macro cannot include engine/CAnimSet.h, either directly or through engine/CAnimateThing.h,
    engine/CTCHeroExperience.h, engine/CTCAnimationComplex.h or engine/NGraphicAppearance_CTCGraphicAppearance_HelperPoint.h.
  - This row includes none of them, and no shared header includes GFLimit.h.
  - Fix if wanted: a shared `FABLE_GFLIMITLOWER_DEFINED` guard in both headers. It is preprocessor-only, so the default token stream is
    unchanged.

## Question for the advisor
Every remaining site is either a commutative-operand pick inside an inline body or the packer's slot choice for one inline temporary. Both
follow c2 ids and record counts, not any FW-visible text.
- (a) Can the lane get the wfE6 lk comparator tools (lkb2.py / lka.sh / fsolve.py, or the c2.dll comparator address they hook)? With them
  we can solve the id shifts per site. Without them, the only lever is a blind sweep of tail text.
- (b) Or rule on the row: is p9 (FW-faithful, 7162) or p5 (7176 equal length, less faithful) the one to keep as the DIFFER record? And may it
  close as a TERMINAL proof in the 0x00CF4EC0 style, where the per-site cause is an id-driven inline pick?
