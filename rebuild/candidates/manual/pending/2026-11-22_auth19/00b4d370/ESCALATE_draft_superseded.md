# 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace: ESCALATE (AUTH19, 2026-11-22)

## Where it stands
- Best source: `best.cpp` (= `x/p5.cpp`). Grade **DIFFER(7176v7176)** with `/d2inlT314`; frame `sub esp,0x3c0` v retail 0x3d0.
  Stock flags: DIFFER(6815v7176), PostMultiply called out of line (AUTH17 measured the same on the old text). The flag is justified by
  the landed same-TU sibling 0x00B4FC40 (same callee, same flag).
- Metric `nm2.py` maps stack slots (ours to retail by majority vote over aligned instructions) and masks absolute operands. It counts real
  differing instructions. The acctC best (`x/base.cpp`, 7162v7176) scores **702**; p1 343; p2 164; **p5 110**.
- Overlay `ov/` = today's include + 3 opt-ins. The patches are in this folder:
  - `hdr_GFLimit.patch`: NEW `FABLE_RETAIL_GFLIMIT_UPPER_LOWER` (GFLimitUpper / GFLimitLower, default off; pin_asserts: 0 pinned; not
    combinable with engine/CAnimSet.h's GFLimitLower).
  - `hdr_CEngineShadowRenderer.patch` and `hdr_CEngineLightingManager.patch`: the two old wfE6 opt-ins AUTH17 carried (UPDATESHADOWSPACE,
    PEEKDIFFUSEVECTOR).

## What changed: the Ego_d / FableWin port (new evidence; no earlier draft used it)
FableWin.pdb mod 972 (`Engine_Shadow_Renderer2.cpp`) has the full line table, lines 358..657 (`fw_lines_UpdateShadowSpace.txt`).
Ego_d / FableWin have every local and block (`egod_locals.txt`, `fw_syms_UpdateShadowSpace.txt`, mapped to lines by `blk.py`).
The acctC draft contradicted them: u/inv/w0..3, near_y, t/start/end/f/n/limit/candidate/n1, half_extent/radius, fitted blocks, and new
else-block axes. The port uses only Ego_d's names and scopes:
- world00/10/01/11, sub_divs, point_count, new_point_id, and the bilinear weights. Per-line debug sizes put the copies in x-fastest
  order (393..396: d8/d32/d8/d32 destination operands).
- `shadow_space_corner_control_points[4]` as an aggregate of scp[1], [5], [21], [25] (lines 451..454: 18/18/15/15 bytes = [21] and [25]
  are d8). Its two 4-trip loops unroll and forward in VC7.1: the corner regions are exact.
- Each min/max is a two-register call, 14 or 17 bytes on lines 479/480/535/630..633. They are GFLimitUpper / GFLimitLower (FableWin
  `void (float&, const float&)`), and the draft's `if (a > b) a = b` texts are exactly their bodies.
- The flip is `std::swap(min_y,max_y); min_y=-min_y; max_y=-max_y;` (17/14/14 bytes). The flag `transform_setup_for_perspective_shadows`
  replaces the old `t > 0` test.
- The else branch assigns the function-scope axes: `new_shadow_space_x = scp[15]-scp[11]` is 34 bytes = a direct
  C2DVector::operator=(const C3DVector&) call (FableWin has that COMDAT), so p2 adds `FABLE_RETAIL_C2DVECTOR_ASSIGN3D`. The step was
  343 -> 164.
- Two dead-arm asserts before the final PostTranslate (`FABLE_RETAIL_ASSERT_DEAD_ARM`): FableWin lines 636/637 are 225 B each. The
  frameproc reads `has inline asm | has eh` with exactly 6 unwind funclets = 2 asserts x 3 string temporaries. 164 -> 110.
- FITTED `SETINVERSE_TRANSLATION_LOCAL` is dropped: on the port it is worse (p3 295).

## Remaining residue of p5 (`p5_residue.txt`, grid slot lines omitted)
1. **Frame / slots.** Retail builds the LAST PostMultiply(new_shadow_transform) result in local_world_to_base_shadow_space's home
   (0x100 = the NRV slot of CalcLocalWorldToBaseShadowSpace, 0x00B4EE98 / 0x00B4EF4C). Ours builds it in view_mat's home (0x8C).
   Hence 16 bytes less frame, and light/world arrays at 0x148/0x298 v 0x158/0x2A8.
   - d7 (local declared, then assigned): frame 0x3d0 and the grid slots exact, but a temp->local `rep movsd` appears that retail lacks.
   - d8 (wfE6's R1 block, contradicts Ego_d): 7144 long, 0x3d4, 165.
2. **SetInverse cofactor order** (0x00B4D40A..0x00B4D53D, 22+22) plus frustum scheduling (8+7). Retail computes E13*E32-E12*E33 first;
   ours computes E12*E23-E13*E22 first.
   - p6 = p5 + `CMATRIX3X4_SETINVERSE_DONOR_PASS26`, the PDB-backed text (`matrix`/`det`/`rec`, GetDeterminant).
   - p6 makes SetInverse and frustum exact (setinv 4+4, frustum 0+0) but moves the order elsewhere: else-axis GetSquaredMagnitude, the
     final-loop PerspectiveTransformPoint terms, tail 34+32. Result 7162 long, 127.
3. **PostMultiply(perspective_transform)** (0x00B4E509..0x00B4E54D, 7+6). Retail reassociates column 0 as `(f*a0 + a1*0) + (a3*0 + a2*0)`
   and shares `(a3*0 + a2*0)` with column 1. Ours keeps the left chain.
4. **diag** (0x00B4E74A, 6+6). Retail evaluates `D*D + half*half`; ours evaluates `half*half + D*D`. Line 590 is 96 B; the C2DVector
   temporary + GetMagnitude text accounts for ~56 B, so the true text of line 590 is not known.
5. Alignment-pad differences (`mov edi,edi` v `lea esp,[esp]`) follow from 1.

Items 2-4 are commutative-operand / reassociation orders. The wfE6 lk census showed these are keyed by c2 record ids. The port moved them
a lot (setinv 85 -> 22, frustum 128 -> 8), but no single remaining Ego_d-consistent text fixes them all.

## Grades this run (11): base, p1..p6, d7, d8, p5 stock (LOG.md)

## Questions for the advisor
1. **Frame.** The retail slot sharing needs local_world_to_base_shadow_space to stop conflicting after its last use. VC7.1 does that only
   for a block-scoped NRV local (wfE6 measurement), and that block contradicts Ego_d. Ego_d / FableWin were built from a 2012 revision
   (GOTCHAS 2026-11-19). Is a scope that differs from Ego_d admissible for one local, if it is shown to move only the frame? Or is there a
   VC7.1 mechanism to try first, e.g. a direct-init spelling or an explicit copy that c2 later elides?
2. **Which SetInverse text is the base:** p5 (the default text) or p6 (the PDB-backed DONOR_PASS26 text)? From either, do you want a
   pre-registered c2 id trace (the lk tool from wfE6) of one product, to find which population fact orders the operands?
3. Lines 652/654 (17 + 29 B, after CalcClipVolume, no EH) look like `if (<member>) DrawFrustrumDebugScreen(...)`, absent from retail.
   Should they be treated as preprocessor-removed (current port) or modelled?
