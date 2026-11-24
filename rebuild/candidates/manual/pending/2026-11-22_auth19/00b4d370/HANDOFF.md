# 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace: WORKING (context), AUTH19 2026-11-22

State: not escalated. The row is still moving: 13 grades took it from 702 to 110 under nm2, and every gain came from PDB evidence. The
long history in LOG.md. ESCALATE_draft_superseded.md is an earlier draft; the advisor said to keep working.

## Next steps (in order)
1. Start from **`best_evidence.cpp`** (= x/p7.cpp, DIFFER(7162v7176), nm2 149). It is the most evidence-faithful text:
   - p6 adds the PDB-backed `CMATRIX3X4_SETINVERSE_DONOR_PASS26` SetInverse. This makes SetInverse and frustum exact.
   - p7 adds line 598 `/ 0.5f` and line 605 `*= SceneData.ShadowFadeEnd / MaxShadowDistance`. Both now equal FableWin's /Od sizes.
   - Lowest count so far: `best.cpp` (= x/p5.cpp, DIFFER(7176v7176), nm2 110), the default SetInverse.
2. Per-line /Od check:
   `odall10.py --lib engine <src> UpdateShadowSpace` (the repo copy; needs `export S=...`), then `python3 lc.py od.txt src.cpp`.
   Lines still off, other than the systematic loop-header +6/+9, the entry, and the asserts:
   - 532 clipped_projected_y: FW 48 v 36.
   - 546 frustrum_reduction_factor: 16 v 18.
   - 590 diag: FW 96. Measured: C2DVector temporary 68, C3DVector 74, three explicit-sqrt spellings 80..82.
   - 644 PostMultiply(CMatrix4x4(local)): 39 v 33.
   - 652/654 (17 + 29 B, after CalcClipVolume, no EH): absent from retail; maybe a `#if`-removed DrawFrustrumDebugScreen call.
   Fix the size first, then grade ONE pre-registered candidate.
3. After step 2 is clean: a c2 id trace (wfE6 lk tools) of the remaining order sites. Do it only then.

## Residue of p7 (nm2 -v)
- PostMultiply(perspective_transform) column reassociation, 0x00B4E509..55E.
- Else-branch GetSquaredMagnitude operand order, 0x00B4E5E9.
- diag `D*D + h*h`, 0x00B4E74A.
- Final 26-loop PerspectiveTransformPoint term order, 0x00B4E988...
- Last PostMultiply operand order.
- Frame 0x3c0 v 0x3d0. Retail builds the last PostMultiply result in local_world_to_base's NRV home (0x100); ours builds it in
  view_mat's (0x8C).
  - d7 (declare, then assign) gives frame 0x3d0 and exact grid slots, but adds a copy that retail lacks.
  - d8 (wfE6 block) is worse.
  - Line 366 is 65 B in /Od = copy-init, so the text there is right.

## Files
- Sources: `best.cpp` (p5) and `best_evidence.cpp` (p7). Variants and logs are in x/.
- Tools in this folder:
  - `g.sh`: one grade; `QT=_a` gives a parallel work dir.
  - `nm2.py`: slot-mapped residue metric.
  - `lc.py`: /Od line-size comparison against FableWin.
  - `blk.py`: FableWin blocks to lines.
- Evidence: `egod_locals.txt`, `fw_syms_UpdateShadowSpace.txt`, `fw_lines_UpdateShadowSpace.txt`, `retail.dis`, `od_p6.txt`,
  `od_p7.txt`.
- Header patches: `hdr_GFLimit.patch` (NEW `FABLE_RETAIL_GFLIMIT_UPPER_LOWER`; pin_asserts 0 pinned), `hdr_CEngineShadowRenderer.patch`,
  `hdr_CEngineLightingManager.patch`. The overlay is in `ov/`.
- Flags: `/d2inlT314`. Stock gives DIFFER(6815v7176): PostMultiply is out of line. The flag is the same as on the landed sibling
  0x00B4FC40.
- `FABLE_RETAIL_C3DVECTOR` (on since p1, for unary minus) was checked: the operator, copy-ctor and GetMagnitude bodies the row
  expands are outside that switch.
