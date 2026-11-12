# acctB hand-back, chunk 2047 (1 row matched)

Authored by an Opus 5.5 agent from chunk 2044's headers, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00A9E1E0 `?ApplyParentTransformations@C3DAnimationManager@@ABEXPBVC3DKeyframe@@PAVCMatrix4x4@@ABVC3DMesh2@@_N@Z` | RELOCATION_MATCH 2472v2472, test PASS (g++ and VC7.1); grade `retail_asm` | no flags; 5 grades, all in pass 1 (matched at the third) |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no warnings). Purity and padded-struct greps are clean. Regraded under this folder's `hdr/`:
chunk 2044's row 0x00A9F540 (RELOCATION_MATCH) and the landed 0x00AA39A0 (RELOCATION_MATCH with its `/d2inlT308`) and
0x00AA3AF0 (RELOCATION_MATCH), so this header set serves 0x00A9F540 too.

## Review items for landing

1. **`retail_asm`: the existing shared macro `GF_SSE_MATRIX_MULTIPLY`** (`engine/CMatrix4x4AlignedSSE.h`), reached
   through the inline `AlignedSetMultiply` at two sites (0x00A9E528, 0x00A9E9CC). ASM_RULES rule 4 table, per site on
   the site's own inputs under the row's flags, /arch:SSE and /arch:SSE2, with a positive control (the macro itself is
   detected at both sites): 35 plain-C++ spellings (x87 only, scalar SSE under /arch, never a packed instruction) and 13
   intrinsic spellings (packed SSE with the compiler's own registers and scheduling, never re-reading the homed
   pointers). Summary in the row's `// Retail-asm:` block, full table in `NOTES.md`.
2. **Inline budget** (`// Inline-budget:` line with c2 scores): `FABLE_FORCEINLINE` on the 4x4
   `CQuaternion::ToMatrix` / `ToMatrixTransposed`, the matching `C3DKeyframe` wrappers and
   `AlignedSetMultiplyTransposed` (scores 283 / 286, then 297..313, and 592). The flag alternative `/d2inlT593` gives
   identical bytes; the forced form was chosen because with the conversions forced BOTH `C3DKeyframe` 4x4 inline bodies
   grade RELOCATION_MATCH against retail's uncalled out-of-line copies 0x00AA39A0 and 0x00AA3AF0 with no flag (so the
   landed 0x00AA39A0 could drop its `/d2inlT308` if the opt-in is folded in; your call).
3. **PARTLY FITTED -- the x87 arm of `AlignedSetMultiply` through the `CMatrix4x4` element accessors**
   (`FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS`). For: FableWin emits both `operator()` copies right after
   `AlignedSetMultiply` (their first user), and this change alone made the transposed loop match. Against / open: no
   retail out-of-line copy of the body exists; FableWin's debug count is 179 instructions against /Od 168 for this body
   (148 for the old one) -- closer, not exact. As a free cross-check it improves all four chunk-748 parked
   `AlignedSetMultiply` users (equal instructions 174->177, 186->195, 212->216, 206->207 for 0x00989230, 0x00AE1320,
   0x00AE29D0, 0x00988E80); none matches yet. Whether to make it the shared default is your decision.
4. **FITTED spelling:** `unsigned long i;` declared once at function scope and a named
   `const C3DBone2* bone = mesh.GetBone(i);`. The second multiply loop's term order follows the outer function's
   named-symbol count mod 8 (diagnostic sweep, never delivered); a named `const CMatrix4x4& parent_matrix` screened
   equal too. The names of parameters and locals are descriptive (the PDB sets carry none).
5. **Name fix** (`NAME_FIXES.tsv`): 0x00AA3AF0 is `?ToMatrixTransposed@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z`; it is
   landed as `?ToMatrix`, the same name as 0x00AA39A0 (two addresses share one name). 0x00AA39A0's manifest parameter
   list is wrong.
6. **Headers** (`hdr/README.md`; new opt-ins, default text unchanged; built on chunk 2044's overlay with C3DBone2.h and
   CMatrix4x4.h from r3010): `CQUATERNION_MATRIX4_TRANSPOSED`, `CQUATERNION_MATRIX4_FORCEINLINE`,
   `C3DKEYFRAME_TOMATRIX4_INLINE`, `CMATRIX4X4_SETMULTIPLYTRANSPOSED` (+ `_FORCEINLINE`),
   `CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS`; in C3DMesh2.h the duplicate `GetBoneCount` of the BONES block is guarded
   so it combines with BONECOUNT. `AlignedSetMultiplyTransposed` = `AlignedSetMultiply(b, a)`: VC7.1 /Od gives 13
   instructions / 29 bytes, exactly FableWin's debug copy. The aligned frame (`and esp,-16`) comes from the
   `__declspec(align(16)) CMatrix4x4 local;` locals, as in 51 landed rows.

## How to land

`rows/00a9e1e0.cpp` + `rows/00a9e1e0.test.cpp` with `hdr/engine/` (also serves 0x00A9F540 of c2044), then `payload.json`
through your usual vland.
