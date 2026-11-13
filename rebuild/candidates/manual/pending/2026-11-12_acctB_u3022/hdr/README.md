# Chunk 2044 header overlay (grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2044/hdr`)

Base copies: the animation headers are agent3010's overlay copies (= pending/2026-11-11_acctB_r3004 lineage: C3DAnimation2,
C3DAnimationSequence, C3DAnimationSequenceData, C3DAnimationStats, C3DKeyframe, C4DVector, CQuaternion, CSimpleMemoryPool, C3DMesh2);
C3DAnimationManager.h is pending/2026-11-12_acctB_r3016's (= c2001's new class header); C3DAnimationBlendState.h,
C3DAnimationStateAlpha.h, C3DAnimationInfo.h are repo HEAD copies. Every change below is a NEW opt-in; default text unchanged.
CMatrix3x4.h is NOT overlaid (repo default; agent3010's in-progress CMatrix3x4 body changes are not needed by these rows).

| file | opt-in | change | evidence (two sources) | used by |
|---|---|---|---|---|
| engine/C3DAnimationManager.h | `FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS` | declares public CalcBlendedMeshTransformations (CMatrix4x4) / CalcMeshTransformations (CMatrix4x4), private const ApplyBoneMorphDisplacement / ApplyParentTransformations / ApplyBoneMorphScaling, private CombineBaseAnimations / CombineBlendedBaseAnimations | Ego_r.pdb + FableWin.pdb mangled names (QAE / ABE / AAE); retail call sites 0x00AA0991 / 0x00AA09B7 / 0x00AA09C8 / 0x00A9F5D7 / 0x00AA013D (arg counts = `ret` sizes) | both rows |
| engine/C3DAnimationBlendState.h | `FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA` (implies TYPED_DATA, CIVCOUNTEDPOINTER_CONVERT, C3DANIMATIONSTATEALPHA) | defines the nested CSharedData (CIVCountedPointeeBase base; BaseAnimation +8, CombinationBlends CArray<C3DAnimationStateAlpha> +0x0C, SkeletalMorph +0x18) and the inline const accessors PeekBaseAnimation / GetCombinationBlendCount / PeekCombinationBlend | Ego_r.pdb CSharedData members + FableWin.pdb accessor names; landed 0x0070C770 (same layout and GetCombinationBlendCount body); retail 0x00AA0142..0x00AA015E (null test, 20-byte signed divide) | both rows |
| engine/C3DAnimationStateAlpha.h | `FABLE_RETAIL_C3DANIMATIONSTATEALPHA` | retail class `: C3DAnimationState` (needs FABLE_RETAIL_C3DANIMATIONSTATE) + AlphaWeight +0x10 + inline GetAlphaWeight() | RTTI bases; Ego_r.pdb AlphaWeight +0x10 / sizeof 0x14; FableWin ?GetAlphaWeight@C3DAnimationStateAlpha@@QBEMXZ (COMDAT right after CalcMeshTransformations; retail out-of-line copy 0x00A9D700 `fld [ecx+10h] ; ret`) | both rows |
| engine/C3DAnimationInfo.h | `FABLE_RETAIL_C3DANIMATIONINFO_GETANIMATION` (generated branch) | declares GetAnimation(const C3DMesh2&) const returning CCountedPointer<const C3DAnimation2> | Ego_r.pdb ?GetAnimation@C3DAnimationInfo@@QBE?AV?$CCountedPointer@$$CBVC3DAnimation2@@@@ABVC3DMesh2@@@Z; bound in reloc_bindings (0x00A26C60, landed 0x00A9F1D0) | both rows |
| engine/C3DAnimation2.h | `FABLE_RETAIL_C3DANIMATION2_MANAGER` | inline PeekSequence(unsigned long) const (`&Sequences[index]`) and IsBoneEnabled(unsigned long) const (`BoneMask == 0 \|\| BoneMask[bone]`) | Ego_r/FableWin names (QBE); FableWin IsBoneEnabled COMDAT after CalcMeshTransformations; retail 0x00AA0590..0x00AA059B, 0x00AA0244..0x00AA024D | both rows |
| engine/C3DAnimation2.h | `FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY` (inside CALCLOCAL) | ConvertTimeToFrames(float, long&, float&) const defined in the class with landed 0x00A52650's body (+ include engine/GFFloatToLongFloor.h) | landed row 0x00A52650 text; measured: without a visible body frame / fraction escape and the twin compiles to 2249 bytes (retail 2288, loads hoisted into ebx / ebp); the call stays out of line (asm helper inside). **Review:** the landed 0x00A52650 row defines the same body out of line through a `C3DAnimation2_Methods` stand-in; folding this opt-in makes it the header inline (retail's copy is then the C3DAnimation2 TU's COMDAT) | both rows |
| engine/C3DAnimationStats.h | `FABLE_RETAIL_C3DANIMATIONSTATS_ISRELATIVE` | inline IsRelative() `(AnimType & 4) != 0`, IsPartial() `(AnimType & 2) != 0` | Ego_r.pdb / FableWin.pdb ?IsRelative / ?IsPartial@C3DAnimationStats@@QBE_NXZ (FableWin COMDATs right after CalcMeshTransformations); retail `shr ecx,2 ; test cl,1` / `shr eax,1 ; test al,1` (0x00AA01F1, 0x00AA0553); EAnimationType values (BASE_RELATIVE 4, PARTIAL 3, DELTA 5). Landed 0x00A9F2F0 has the same IsRelative body on a local C3DAnimation2 model | both rows |
| engine/C3DAnimationSequence.h | `FABLE_RETAIL_C3DANIMATIONSEQUENCE_FRAMEKEYFRAME` | inline IsIdentity() (`Data->IsIdentity()`) and GetKeyframeInterpolate(C3DKeyframe&, unsigned long, float) const forwarding to the data's out-of-line 0x00A4C1F0 | Ego_r/FableWin ?IsIdentity@C3DAnimationSequence@@QBE_NXZ, ?GetKeyframeInterpolate@C3DAnimationSequence@@QBEXAAVC3DKeyframe@@KM@Z; retail loads Data then calls 0x00A4C1F0 (0x00AA0247..0x00AA025E) | both rows |
| engine/C3DAnimationSequenceData.h | `FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY` | inline IsIdentity() `return IdentityFlag;` (bit 7 of +0x0A) | Ego_r/FableWin ?IsIdentity@C3DAnimationSequenceData@@QBE_NXZ (FableWin COMDAT after ApplyRelativeAnimation); retail `mov al,[ecx+0Ah] ; test al,al ; js` | both rows |
| engine/C3DMesh2.h | `FABLE_RETAIL_C3DMESH2_BONECOUNT` | inline GetBoneCount() `return BoneCount;` (+0x98) | Ego_r/FableWin ?GetBoneCount@C3DMesh2@@QBEKXZ; retail 0x00AA00A1 `mov ebx,[edi+98h]` | both rows |
| engine/CSimpleMemoryPool.h | `FABLE_RETAIL_CSIMPLEMEMORYPOOL_FREEALL` | declares FreeAll() | Ego_r ?FreeAll@CSimpleMemoryPool@@QAEXXZ; reloc_bindings 0x00A5C720 (landed 0x0098B420) | both rows |
| engine/CQuaternion.h | `FABLE_RETAIL_CQUATERNION_SLERPFROMIDENTITY` | declares SetSlerpFromIdentity(const CQuaternion&, float); the row defines the FABLE_FORCEINLINE body = landed 0x00AA3560's text | FableWin ?SetSlerpFromIdentity@CQuaternion@@QAEXABV1@M@Z; retail out-of-line copy 0x00AA3560 (landed, uncalled) | both rows |
| engine/C3DKeyframe.h | `FABLE_RETAIL_C3DKEYFRAME_BLEND` | declares PostApplyRelativeAnimationAlpha(const C3DKeyframe&, float) and BlendWith(const C3DKeyframe&, float); the row defines both FABLE_FORCEINLINE (BlendWith = landed 0x00AA3C50's text) | FableWin names + COMDAT order after CalcMeshTransformations; retail out-of-line BlendWith 0x00AA3C50 (landed) | both rows |

Existing opt-ins used (unchanged): FABLE_RETAIL_CQUATERNION_SLERP (c2001: Dot decl; the row defines Dot), CQUATERNION_MULASSIGN,
C4DVECTOR, C4DVECTOR_ADDASSIGN, C3DKEYFRAME_POSTAPPLY, C3DANIMATION2 / _CALCLOCAL, C3DANIMATIONSEQUENCEDATA_INTERPOLATE,
CMESHDATABANK_PEEKANIMATION (repo), BASE_OBJECT, CMOVABLERESOURCE(_MEMORYPOOL), DEF_CLASS_BASE, CSIMPLEMEMORYPOOL.
Note: C3DKeyframe.h's POSTAPPLY block defines MULASSIGN / ADDASSIGN only after CQuaternion.h is already included, so the row defines
FABLE_RETAIL_CQUATERNION_MULASSIGN and FABLE_RETAIL_C4DVECTOR_ADDASSIGN itself (header debt, not changed).
FABLE_RETAIL_C3DKEYFRAME_CTOR (empty user ctor, FableWin ??0C3DKeyframe@@QAE@XZ) is byte- and order-neutral in both rows (measured).

## Resume session (0x00AA0090) -- new opt-ins (all default text unchanged)
| file | opt-in | change | evidence | used by |
|---|---|---|---|---|
| engine/C3DAnimationState.h (repo HEAD copy) | `FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS` | inline `unsigned long GetAnimationIndex() const`, `float GetAnimationTime() const` | Ego_r.pdb / FableWin.pdb ?GetAnimationIndex@C3DAnimationState@@QBEKXZ, ?GetAnimationTime@C3DAnimationState@@QBEMXZ | 0x00AA0090 candidates (code-neutral; moves x87 orders) |
| engine/GFGetInterpolatedValue.h (repo HEAD copy) | `FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED` | body `float delta=(new_val-old_val)*interp; return old_val+delta;` | FableWin /Od ?GFGetInterpolatedValue@@YIMMMM@Z 12i/28b = exactly this text under VC7.1 /Od (one-expression body 8i/19b) | 0x00AA0090 candidates (code-neutral) |
| engine/C4DVector.h | `FABLE_RETAIL_C4DVECTOR_NAMED_RESULT` / `_NAMED_SCALE` / `_NAMED_ADDSUB` | operator* / - / + through a named result | FableWin /Od 38i / 42i / 42i (VC7.1 named result 39 / 43 / 43, `return C4DVector(...)` 30 / 34 / 34) -- BUT changes TLC release code (frame +0x20 / +0x10): NOT retail, kept only as the measured negative | none |
| engine/C3DKeyframe.h | `FABLE_RETAIL_C3DKEYFRAME_GETROTATION` | inline by-value GetRotation() | FableWin.pdb ?GetRotation@C3DKeyframe@@QBE?AVCQuaternion@@XZ | probe only (BlendWith size 52i, not FableWin's 48) |
| engine/C3DAnimationManager.h | `FABLE_RETAIL_C3DANIMATIONMANAGER_PEEKMESHBANK` | inline `const CMeshDataBank* PeekMeshBank() const` | Ego_r.pdb / FableWin.pdb ?PeekMeshBank@C3DAnimationManager@@QBEPBVCMeshDataBank@@XZ | 0x00AA0090 candidates (code-neutral) |
Row-local helper bodies changed in the candidates (BlendWith through SetSlerp + a named reference, PostApplyRelativeAnimationAlpha with a
named reference) are in the row sources, not in hdr/; the twin 0x00A9F540 needs the SAME text (cand/t_x_w7p2.cpp grades RELOCATION_MATCH).

## Session 3 (pair matched): no new opt-ins. Now USED by both final rows: FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS (GetAnimationIndex /
GetAnimationTime) and FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED (named delta, PROVEN-by-size 12i/28b; the twin does not call it).
FABLE_RETAIL_C3DANIMATIONMANAGER_PEEKMESHBANK and the C4DVector NAMED_* opt-ins remain unused. GFFastSin's new text lives in the row
sources (row-local FABLE_FORCEINLINE helper, no shared header exists for it).

## Chunk 3022 (common helper set for 0x00A9EB90 / 0x00A9F540 / 0x00AA0090): NO header changes
hdr/ is chunk 2044's overlay unchanged. The shared helper block (GFFastArcCos .. BlendWith) is still row-local text, but it is now
identical in the three rows. 0x00A9EB90 additionally defines the existing opt-ins FABLE_RETAIL_CQUATERNION_SLERPFROMIDENTITY,
_CQUATERNION_MULASSIGN, _C4DVECTOR_ADDASSIGN, _C3DKEYFRAME_POSTAPPLY, _C3DKEYFRAME_BLEND and _C4DVECTOR so the full block compiles
(byte-neutral). hdr_3021/ is not used.
