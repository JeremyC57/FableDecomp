# Chunk 3004 header overlay (grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3004/hdr`)

This overlay is the merge of the two prior hand-backs this retry pass starts from; chunk 3004 made NO header change of its own (every
probe macro added during measurement was removed again; the files below are byte-identical copies of their prior-chunk versions).

- From chunk 2024 (`prior/2024/hdr`; its README copied as hdr/README_chunk2024.md): engine/CEngineLightingManager.h, CEngineScreenEffectOutlineGlow.h,
  CEngineShadowRenderer.h, CGPUScratchBufferTextureManager.h, CMatrix4x4.h, CRenderStateCaptureBlock.h, CRenderTarget.h,
  CShadowClipVolume.h, CShadowRendererSceneData.h. Used by 0x00B4FC40 (RELOCATION_MATCH): CMatrix4x4.h
  (`FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4`), CEngineShadowRenderer.h (`_MEMBERS`), CEngineLightingManager.h (`_PEEKGLOBALSHADOWMODE`),
  CShadowRendererSceneData.h, CShadowClipVolume.h, CRenderStateCaptureBlock.h.
- From chunk 2014 (`prior/2014/hdr`; its README copied as hdr/README_chunk2014.md): engine/C3DAnimation2.h, C3DAnimationBuilder.h, C3DAnimationSequence.h,
  C3DAnimationSequenceData.h, C3DAnimationStats.h, C3DBone2.h, C3DKeyframe.h, C3DMesh2.h, C4DVector.h, CMatrix3x4.h, CQuaternion.h,
  CSimpleMemoryPool.h, N3DAnimationBuilder_COutputAnimation.h (the acctB_r3001 base files + chunk 2014 opt-ins).
- No file name exists in both. 0x00B4FC40 now also sees chunk 2014's C4DVector.h / CMatrix3x4.h (supersets of the repo files: additions
  under opt-in macros only); it was regraded with the merged overlay (RELOCATION_MATCH).

See /opt/fable_lane/w22/agent3004/prior/2024/hdr/README.md and /opt/fable_lane/w22/agent3004/prior/2014/hdr/README.md for the per-opt-in
evidence tables (copied unchanged).

## Chunk 3010 changes (row 0x00A4E300 C3DAnimation2::GetDeltaMovement; all in engine/CMatrix3x4.h)
- `FABLE_RETAIL_CMATRIX3X4_TRANSPOSE`: one body now -- the upper triangle through three named copies (`e12 / e13 / e23`). Evidence:
  retail 0x00A4E567 `fxch st(1) ; fstp [esp+8] ; fstp [esp+18h]` (E12 / E13 spilled) and 0x00A4E571 `fld [esp+84h]` (E23 reloaded)
  right after the first ToMatrix; Ego_r.pdb / FableWin.pdb ?Transpose@CMatrix3x4@@QAEXXZ (no retail out-of-line copy). The old
  `_TRANSPOSE_LOWER` and swap variants are gone (only this row ever used the opt-in; frame 0x100 / other spill sets with them).
- `FABLE_RETAIL_CMATRIX3X4_MULTIPLYNOTRANSLATION`: PreScale(float,float,float) is FABLE_FORCEINLINE. Evidence: retail's out-of-line
  C3DKeyframe::ToMatrix 0x00A52450 expands it (plain inline: a call, 341 v 447 bytes; forced: 447 relocation-equal,
  probe/ool/kt.cpp); FableWin.pdb ?PreScale@CMatrix3x4@@QAEXMMM@Z 0x03228C60 (a member, emitted after ToMatrix). Only this row's
  opt-in uses it. The header SetMultiplyNoTranslation body is byte-identical to retail's COMDAT 0x00A4B820 (probe/ool/smn.cpp).
- `FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR` (shared, landed users): the X row grouped `xx*E11+(yy*E21+zz*E31)` like TransformPoint's
  X row. Evidence: retail 0x00A4E8A5..0x00A4E8BC (zz*E31 + yy*E21 first, xx*E11 last) under the record-diagnostic builds (z4 / q32);
  the landed users 0x00A11AE0 GetBoundingBoxCentre, 0x00A11C20 GetBoundingBoxXEdge and 0x00A22F80 CRightHandedSet::Rotate regrade
  RELOCATION_MATCH with it (and with the old text). Default path of the opt-in; no other file changed.
- Session 3 (chunk 3010): `FABLE_RETAIL_CMATRIX3X4_TRANSPOSE` body is now `SWAP(E12, E21); SWAP(E13, E31); SWAP(E23, E32);` with
  `template<class T> inline void FABLE_FASTCALL SWAP(T& a, T& b) { T temp = a; a = b; b = temp; }` declared ahead of the class under the
  same opt-in. Evidence: FableWin.pdb ??$SWAP@M@@YIXAAM0@Z (0x025C2DA0) + ?Transpose@CMatrix3x4@@QAEXXZ (0x025C21B0); VC7.1 /Od of this
  text = 18 / 22 instructions = FableWin's debug COMDATs exactly (FableWin.exe.signatures.tsv); the row's code is unchanged (1615B, same
  spill set). Only this row uses the opt-in (repo: only older parked copies of 0x00A4E300). Previous body: tmp/CMatrix3x4.h.bak_s3.
- candidate_g04/hdr (NOT the default overlay): additionally SetMultiplyNoTranslation computes the translation row like SetMultiply and
  then clears it. Out of line byte-identical with retail 0x00A4B820; makes the row RELOCATION_MATCH (g06) but contradicts FableWin
  (debug 160 instr = 9-sum body; ours 157, this text 211).
