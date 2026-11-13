# DOSSIER 0x00AA0FA0 -- C3DAnimationManager::CalcBoneTransformation (4460 bytes, `ret 14h`)

## 1. Retail shape
- thiscall, 5 stack args, `ret 14h` (0x00AA2109). Args: [ebp+8] blend_state, [ebp+0Ch] mesh, [ebp+10h] bone (unsigned long),
  [ebp+14h] CMatrix3x4& out matrix, [ebp+18h] C3DVector& out scale.
- Prologue `push ebp ; mov ebp,esp ; and esp,-16 ; sub esp,214h ; push ebx ; push esi ; push edi`. The ebp frame comes from the
  16-byte realignment (`__declspec(align(16))` CMatrix4x4 locals), NOT /Oy-: identical prologue in the matched sibling
  0x00A9E1E0 ApplyParentTransformations (agent2047/src/00a9e1e0.cpp line 4: "16-byte realigned frame"). Epilogue
  `pop edi ; pop esi ; pop ebx ; mov esp,ebp ; pop ebp ; ret 14h`.
- No SEH frame although two CCountedPointer locals with destructors live across calls: the TU is built WITHOUT /EHsc
  (same in the matched siblings 0x00A9F540 `sub esp,14Ch` and 0x00AA0C50 `sub esp,8`, which have no `// Flags:` line). Do not
  add /EHsc (HINTS' FW_EXTRA=/EHsc is only for the fwsize oracle of other rows).
- No /GS cookie. /O2 speed idioms (loop unrolled x4 at 0x00AA198C, `lea esp,[esp]` / `lea ecx,[ecx]` alignment pads); no
  optimize("s") signs.
- Retail-asm inside: the GF_SSE_MATRIX_MULTIPLY kernel (0x00AA1EF8..0x00AA1FD9 movaps/shufps/mulps/addps, after homing the
  three pointers to [esp+3Ch]/[esp+14h]/[esp+1Ch] and re-reading them 0x00AA1EEC..0x00AA1EF4) = the inline
  CMatrix4x4::AlignedSetMultiply SSE arm (engine/CMatrix4x4AlignedSSE.h, guarded by g_SSEAvailable byte 0x013D2880 at
  0x00AA1E1D); and GFFloatToLongNear `fistp dword` sites 0x00AA12CB / 0x00AA1342 / 0x00AA13B5 / 0x00AA1627 / 0x00AA1695 /
  0x00AA16FF (GFFastArcCos / GFFastSin inside SetSlerpFromIdentity and SetSlerp). Same helpers + Retail-asm lines as 0x00A9F540
  (agent3022/src/00a9f540.cpp) and 0x00A9E1E0 (agent2047).

## 2. Identity (manifest label is RIGHT)
- Leaf `?CalcBoneTransformation@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@ABVC3DMesh2@@KAAVCMatrix3x4@@AAVC3DVector@@@Z`
  - Ego_r.pdb: ghidra_out/egor_pdb_names.tsv 00be5220 (and egor_signatures.tsv).
  - FableWin.pdb: _fablewin_api.tsv 0180f03e (thunk) and FableWin.exe.signatures.tsv real body 0337f160 (2411 B / 642 instr).
  - Caller: landed rebuild/src/compiled/00/a8/C3DMesh2_GetHelperPointPositionAnimated_00a89740.cpp line 7-8/63 names it
    `manager.CalcBoneTransformation(state, *this, helper.BoneIndex, transform, scale)`. xref callers: 0x00A897EC, 0x00A89BF1, 0x00A8A260.
- Signature: `void C3DAnimationManager::CalcBoneTransformation(const C3DAnimationBlendState& blend_state, const C3DMesh2& mesh,
  unsigned long bone, CMatrix3x4& transform, C3DVector& scale);` public, non-const (QAE). Parameter names are descriptive.
- PDB order (Ego_r): ctor 00be51e0, CalcBoneTransformation 00be5220, CalcBlendedBoneTransformation 00be6570 (= retail
  0x00AA0F60 ctor, 0x00AA0FA0 this, 0x00AA2110 twin). The twin 0x00AA2110 is in this same wave (bwf3/00aa2110).

## 3. Callees (retail address -> landed row -> PDB)
| addr | what | landed row | PDB / note |
|---|---|---|---|
| 0x00464931 | vector<unsigned long>::resize(n, const unsigned long&) (from `BoneIndicesBuffer.resize(1)`) | 00/46/CArray_resize_00464931.cpp (local stand-in `CArrayElem::resize`; manifest "CArray::resize") | STLport `vector<unsigned long>::resize(size_type,const T&)`. Out of line in retail: use `FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE` (stlport/fable_containers.h:540) or declaration. FLAG: landed spelling is a stand-in |
| 0x00464ACE | vector<unsigned long>::_M_insert_overflow(pos,x,__true_type,1,true) (push_back overflow) | 00/46/stdvectorunsignedlong_Minsertoverflow_00464ace.cpp | STLport; declared-only in fable_containers.h:565 (default) -> stays a call |
| 0x00A5C910 | CSimpleMemoryPool::Allocate(unsigned long) via inline AllocateType<C3DKeyframe> / AllocateType<C3DVector> | 00/a5/CSimpleMemoryPool_Allocate_00a5c910.cpp | ok |
| 0x00AA0C50 | C3DAnimationManager::CombineBaseAnimationsBoneList(const C3DAnimationStateBase&, C3DKeyframe*, unsigned long, unsigned long, const C3DMesh2&, const CArray<unsigned long>&) | 00/aa/C3DAnimationManager_CombineBaseAnimationsBoneList_00aa0c50.cpp | Ego_r 00be34d0 `AAE` (private); spelling agrees |
| 0x00A242C0 | CMeshDataBank::PeekAnimation(unsigned long) const | 00/a2/CMeshDataBank_PeekAnimation_00a242c0.cpp | ok |
| 0x00A26C60 | C3DAnimationInfo::GetAnimation(const C3DMesh2&) const -> CCountedPointer<const C3DAnimation2> (hidden ret) | 00/a2/C3DAnimationInfo_GetAnimation_00a26c60.cpp | ok |
| 0x00A52650 | C3DAnimation2::ConvertTimeToFrames(float, long&, float&) const | 00/a5/C3DAnimation2_ConvertTimeToFrames_00a52650.cpp | FLAG: landed defines it on a `C3DAnimation2_Methods` stand-in; use overlay FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY (body must be visible: fraction hoisted into ebx at 0x00AA1173) |
| 0x00A4C1F0 | C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, unsigned long, float) const | not landed | Ego_r 00ba1d80 `?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@KM@Z`; overlay C3DAnimationSequence.h forwards to it |
| 0x00A88C10 | CQuaternion::operator*=(const CQuaternion&) | 00/a8/CQuaternion_operator_00a88c10.cpp | `??XCQuaternion@@QAEABV0@ABV0@@Z` ok |
| 0x00BFE9BC | operator delete (CCountedPointer info block) | CRT | -- |
| [Info+4] | CCPPointerInfo::DeleteFunc indirect call (`mov ecx,[eax+8] ; call [eax+4]`) | engine/CCountedPointer.h dtor | -- |
| 0x00A45260 | C3DSkeletalMorph::PeekCombinedMorph() const -> CCountedPointer<const C3DSkeletalMorphTarget> (hidden ret, `ret 4`) | 00/a4/C3DSkeletalMorph_PeekCombinedMorph_00a45260.cpp | FLAG: landed spells it NON-const returning CCountedPointer<C3DSkeletalMorphTarget>; Ego_r 00ba0200 / FableWin 0321c960 `?PeekCombinedMorph@C3DSkeletalMorph@@QBE?AV?$CCountedPointer@$$CBVC3DSkeletalMorphTarget@@@@XZ`. -> NAME_FIXES candidate |
| 0x00A44CD0 | C3DSkeletalMorphTarget::GetBoneScaling(C3DVector&, unsigned long) const | 00/a4/C3DSkeletalMorphTarget_GetBoneScaling_00a44cd0.cpp | Ego_r 00b9fcd0 `QBEXAAVC3DVector@@K@Z` agrees |
| 0x00A5C720 | CSimpleMemoryPool::FreeAll() | 00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp | Ego_r `?FreeAll@CSimpleMemoryPool@@QAEXXZ` |
Globals: g_SSEAvailable 0x013D2880 (bool), g_ArcCosTable 0x013CB530 (float[1026]), g_CosTable 0x013CD550 (float[1025]);
float constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 0.05f 0x0125A15C, 512.5f 0x0129CD1C, 0.5f 0x0122F59C,
0.15915494f 0x01237F08, 1024.0f 0x01230010 (all as in agent3022/agent2047 rows).

## 4. Members (retail offsets; Ego_r offsets differ by build -- follow the overlay headers, not struct_layouts_egor.tsv raw)
| access | member | header / opt-in |
|---|---|---|
| this+4 | C3DAnimationManager::MeshBank (CMeshDataBank*) | overlay engine/C3DAnimationManager.h (agent3022/hdr; NOT in repo) |
| this+8 | BoneIndicesBuffer CArray<unsigned long> (begin +8, finish +0xC, end_of_storage +0x10) | same; Ego_r name BoneIndicesBuffer |
| this+0x14 | MemoryPool CSimpleMemoryPool | same (Ego_r +24 is the later build) |
| blend+4 | C3DAnimationBlendState::Data (CIVCountedPointer<CSharedData>) | engine/C3DAnimationBlendState.h FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA (overlay copy) |
| Data+8 / +0xC..+0x14 / +0x18 | CSharedData::BaseAnimation / CombinationBlends (20-byte C3DAnimationStateAlpha) / SkeletalMorph (CCountedPointer<const C3DSkeletalMorph>, Data at +0x18) | same; PeekBaseAnimation / GetCombinationBlendCount / PeekCombinationBlend exist. MISSING: HasSkeletalMorph() (FableWin 02d870a0, 72 B) and PeekSkeletalMorph() (02e8ea30, `?PeekSkeletalMorph@C3DAnimationBlendState@@QBEABVC3DSkeletalMorph@@XZ`) -> new opt-in needed |
| state+8 / +0xC / +0x10 | C3DAnimationStateAlpha AnimationIndex / AnimationTime / AlphaWeight | C3DAnimationState.h FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS + C3DAnimationStateAlpha.h FABLE_RETAIL_C3DANIMATIONSTATEALPHA |
| anim+8 | C3DAnimation2 AnimType (C3DAnimationStats; IsRelative bit2, IsPartial bit1) | C3DAnimationStats.h FABLE_RETAIL_C3DANIMATIONSTATS_ISRELATIVE |
| anim+0x5C / +0x68 | Sequences (8-byte C3DAnimationSequence, Data at +0) / BoneMask (unsigned char*) | C3DAnimation2.h FABLE_RETAIL_C3DANIMATION2_MANAGER (PeekSequence, IsBoneEnabled) |
| seqdata+0xA bit7 | C3DAnimationSequenceData IdentityFlag | FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY |
| mesh+0x9C, stride 0x3C | C3DMesh2::Bones (C3DBone2*): NameCRC +0, Parent +4 | C3DMesh2.h FABLE_RETAIL_C3DMESH2_BONES (repo) + overlay C3DBone2.h FABLE_RETAIL_C3DBONE2_PEEK (agent2047/hdr; GetParent). MISSING: C3DBone2::GetNameCRC() (`?GetNameCRC@C3DBone2@@QBEKXZ`, FableWin 02e898a0) -> add to the C3DBONE2 opt-in |
| keyframe 0x30 | C3DKeyframe Rotation (CQuaternion +0) / Position (C4DVector +0x10) | C3DKeyframe.h overlay: POSTAPPLY, BLEND, TOMATRIX4_INLINE (agent2047), GetPosition() by value |
| CMatrix4x4 0x40 | E11..E44, Data[4][4] | CMatrix4x4.h overlay (agent2047/hdr): FABLE_RETAIL_CMATRIX4X4, _ELEMENT_ACCESS, CMatrix4x4AlignedSSE.h FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS. MISSING: CopyTo(CMatrix3x4&) const (FableWin 0337f090 `?CopyTo@CMatrix4x4@@QBEXAAVCMatrix3x4@@@Z` 155 B/55 i, emitted right before CalcBoneTransformation; also expanded in the CMatrix3x4 CalcMeshTransformations overload 0x00A9FE80 loop) -> new opt-in |
| out matrix | CMatrix3x4 (12 floats E11..E43) | repo engine/CMatrix3x4.h |
| C3DSkeletalMorph / C3DSkeletalMorphTarget | no shared header (landed rows 0x00A45260 / 0x00A44CD0 model them locally) | declare locally (declaration-only methods) or add a new header |
Classes with NO repo header but an overlay: C3DAnimationManager, C3DBone2. The coordinator's $W/hdr is EMPTY: merge
agent3022/hdr (animation set) with agent2047/hdr (CMatrix4x4.h, CMatrix4x4AlignedSSE.h, C3DBone2.h, C3DKeyframe.h TOMATRIX4,
CQuaternion.h MATRIX4 / _FORCEINLINE). Overlapping files to merge by hand: C3DKeyframe.h, CQuaternion.h, C3DMesh2.h,
C3DAnimationManager.h, C3DAnimation2.h, C3DAnimationBlendState.h, C3DAnimationSequence*.h, C3DAnimationStats.h, C4DVector.h,
CSimpleMemoryPool.h (diff both against each other; 2047's README says it is based on 2044's overlay, 3022 is later).
C3DAnimationManager.h needs a new opt-in (e.g. FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM) declaring public
CalcBoneTransformation and private CombineBaseAnimationsBoneList (Ego_r spellings above).

## 5. Callee-body visibility
- Expanded inline (must be visible / forced): AllocateType<T> (CSimpleMemoryPool.h, FableWin 03385bf0), vector::push_back
  (STLport; overflow path stays a call), GetCombinationBlendCount/PeekCombinationBlend/PeekBaseAnimation, IsRelative/IsPartial,
  PeekSequence/IsIdentity/IsBoneEnabled/GetKeyframeInterpolate forwarder, the four Apply* file-static helpers of 0x00A9F540
  (FABLE_FORCEINLINE; same TU, FableWin lists them after CalcMeshTransformations), PostApplyRelativeAnimation(Alpha),
  SetSlerpFromIdentity, SetSlerp, BlendWith, GFFastArcCos/GFFastSin (agent3022 common helper block, copy it verbatim),
  C3DKeyframe::ToMatrix(CMatrix4x4&,bool) + CQuaternion::ToMatrix 4x4 (FORCEINLINE opt-ins of agent2047; FableWin emits both
  COMDATs right AFTER CalcBoneTransformation 0337fd30/0337fd70 = this is their first user in the TU), AlignedSetMultiply
  (FORCEINLINE, header), SWAP<CMatrix4x4*> (FableWin `??$SWAP@PAVCMatrix4x4@@@@YIXAAPAVCMatrix4x4@@0@Z` 03385dd0, fastcall template,
  inline; no repo definition -> define `template<class T> inline void __fastcall SWAP(T& a, T& b) { T t = a; a = b; b = t; }`
  or plain swap), CMatrix4x4::CopyTo, C3DVector::GetScaled (repo opt-in FABLE_RETAIL_C3DVECTOR_GETSCALED),
  C3DKeyframe::SetPosition/GetPosition (FableWin `?SetPosition@C3DKeyframe@@QAEXABVC3DVector@@@Z`; W written 0 ->
  `Position = C4DVector(v, 0.0f)`, overlay C4DVector.h FABLE_RETAIL_C4DVECTOR has that ctor), HasSkeletalMorph,
  PeekSkeletalMorph, GetBone, GetParent, GetNameCRC, CCountedPointer dtor (header, Data-first release then Data=Info=0).
- Out of line (calls): resize 0x00464931 (body visible but noinline: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE -- test vs
  default; if the default inlines resize the row explodes), _M_insert_overflow, Allocate, FreeAll, CombineBaseAnimationsBoneList
  (same TU, defined EARLIER at 0x00AA0C50: declare only, it is not inlined), PeekAnimation, GetAnimation, ConvertTimeToFrames
  (body visible, stays a call: holds the asm floor helper), GetKeyframeInterpolate, operator*=, PeekCombinedMorph, GetBoneScaling.

## 6. Sibling rows to copy idioms from
- agent3022/src/00a9f540.cpp CalcMeshTransformations (RELOCATION_MATCH): the opt-in list, the helper block, the combination-blend
  loop, `if (keyframe_count < 0 || > 0x7fffffff) return;` range check, `C3DKeyframe& keyframe = keyframes[bone];` named refs
  (fixes the BlendWith dot Y,Z,W,X order -- retail here 0x00AA158D..0x00AA15C1 is also Y,Z,W,X). No Flags line, no pragmas
  besides `#pragma intrinsic(sqrt/fabs)`.
- agent3022/src/00aa0090.cpp CalcBlendedMeshTransformations (same helper block, the twin loop spelling without the named ref).
- agent2047/src/00a9e1e0.cpp ApplyParentTransformations: `__declspec(align(16)) CMatrix4x4 local; keyframes[i].ToMatrix(local,
  false); m.AlignedSetMultiply(a, b);` and its opt-ins (CMATRIX4X4, _ELEMENT_ACCESS, ALIGNEDSETMULTIPLY_ACCESSORS,
  CQUATERNION_MATRIX4(+_FORCEINLINE), C3DKEYFRAME_TOMATRIX4_INLINE, C3DMESH2_BONES, C3DBONE2_PEEK) + its Retail-asm line text.
- landed 00/aa/C3DAnimationManager_CombineBaseAnimationsBoneList_00aa0c50.cpp (local models, no flags).
- landed 00/aa/C3DKeyframe_ToMatrix_00aa39a0.cpp (`// Flags: /d2inlT308` for the out-of-line copy; with the forced opt-ins no flag).
- landed 00/a8/C3DMesh2_GetHelperPointPositionAnimated_00a89740.cpp (caller; uses FABLE_RETAIL_C3DVECTOR_GETSCALED).
