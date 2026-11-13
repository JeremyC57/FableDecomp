# DOSSIER 0x00AA2110 -- C3DAnimationManager::CalcBlendedBoneTransformation (4556 bytes, `ret 1Ch`)

Read with OUTLINE.md (block map). The non-blended TWIN 0x00AA0FA0 CalcBoneTransformation has its own scout dossier at
/opt/fable_lane/w22/bwf3/00aa0fa0/DOSSIER.md -- nearly every header/opt-in fact there applies here verbatim (summarised below).
Instruction-level diff (mnemonics, immediates masked; difflib):
- this row = twin 0x00AA0FA0 except the head of the blend loop: equal runs twin 0x00AA0FA0..1034 = row 0x00AA2110..21A4 (bone list),
  twin 1169..11FE = row 2319..23AE, twin 121F..14B9 = row 23D3..266D, twin 1561..1887 = row 271D..2A43 (the four bone loops),
  twin 18F9..2109 = row 2AC1..32D9 (morph scaling, matrix chain, CopyTo, FreeAll, epilogue: ~576 instructions identical).
- this row = matched 0x00AA0090 CalcBlendedMeshTransformations (agent3022/src/00aa0090.cpp, RELOCATION_MATCH) in the blend-loop head:
  aa0090 013D..0162 = row 2235..225A (GetCombinationBlendCount, down-counter), 01AB..01C9 = row 2299..22B7 (GetAnimation +
  GFGetInterpolatedValue time), 01D1..01F6 = 22BB..22E0, 0200..0224 = 22E6..230A (alpha interp, IsRelative, fabsf(alpha-1)<eps),
  030E..0493 = 2418..25BE, 0601..07C8 = 2744..290B (SetSlerpFromIdentity / BlendWith expansions).
=> Write it as: 0x00AA0090's blend loop (two states, GFGetInterpolatedValue, hoisted blend_count, down-count) + the twin's bone-list
   prologue and morph/matrix tail; the per-bone loops index `keyframes[bone]` but take the sequence / mask index from
   `BoneIndicesBuffer[bone]`.

## 1. Retail shape (python3 $S/w17/xdis.py 00aa2110 0x11dc; $W/dis_all.txt)
- __thiscall, 7 stack args, `ret 1Ch` (0x00AA32D9). ecx=this saved to [esp+3Ch] (0x00AA2127).
  Args: [ebp+8] start_blend (const C3DAnimationBlendState&), [ebp+0Ch] end_blend, [ebp+10h] blend (float), [ebp+14h] mesh
  (const C3DMesh2&), [ebp+18h] bone (unsigned long), [ebp+1Ch] CMatrix3x4& out transform, [ebp+20h] C3DVector& out scale.
- Prologue `push ebp ; mov ebp,esp ; and esp,0FFFFFFF0h ; sub esp,214h ; push ebx ; push esi ; push edi` (0x00AA2110..211E).
  The ebp frame is the 16-byte realignment for `__declspec(align(16)) CMatrix4x4` locals (same prologue in twin 0x00AA0FA0 and
  matched 0x00A9E1E0 ApplyParentTransformations), NOT /Oy-. Epilogue `pop edi ; pop esi ; pop ebx ; mov esp,ebp ; pop ebp ; ret 1Ch`.
  All locals are addressed esp-relative; args ebp-relative.
- NO SEH frame (no fs:0) although CCountedPointer locals with dtors live across calls -> TU built without /EHsc (same as the
  matched siblings 0x00A9F540 / 0x00AA0090 / 0x00AA0C50, which carry no `// Flags:` line). No /GS cookie. /O2 speed code: the
  scaling loop is unrolled x4 (0x00AA2B42..2C86) + remainder (2C90..2CF1); alignment pads `lea esp,[esp]` / `lea ebx,[ebx]`
  / `mov edi,edi` / `nop`. No optimize("s") idioms.
- Retail-asm inside (all from shared headers, none in the row): GF_SSE_MATRIX_MULTIPLY kernel 0x00AA30C8..31A9 (movaps/shufps/
  mulps/addps) after homing the three pointers [esp+14h]/[esp+40h]/[esp+20h] and re-reading them 0x00AA30A9..30C4 =
  CMatrix4x4::AlignedSetMultiply SSE arm (engine/CMatrix4x4AlignedSSE.h, guard g_SSEAvailable byte 0x013D2880 at 0x00AA2FED);
  GFFloatToLongNear `fistp dword` at 0x00AA247F, 0x00AA24F6, 0x00AA2569 (SetSlerpFromIdentity), 0x00AA27E3, 0x00AA2851,
  0x00AA28BB (SetSlerp in BlendWith). Copy the `// Retail-asm:` line style of agent3022/src/00aa0090.cpp + agent2047/src/00a9e1e0.cpp.

## 2. Identity (manifest label is RIGHT)
- Leaf `?CalcBlendedBoneTransformation@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MABVC3DMesh2@@KAAVCMatrix3x4@@AAVC3DVector@@@Z`
  - Ego_r.pdb: ghidra_out/egor_pdb_names.tsv + egor_signatures.tsv @ Ego_r 00be6570 (right after CalcBoneTransformation 00be5220,
    ctor 00be51e0 -> retail order ctor 0x00AA0F60, twin 0x00AA0FA0, this 0x00AA2110).
  - FableWin.pdb: _fablewin_api.tsv 017ef24d (thunk) and FableWin.exe.signatures.tsv real body 03381340 (3351 B / 851 instr).
  - 7 args = `ret 1Ch`; arg uses match (ebp+14h is dereferenced as C3DMesh2: Bones +0x9C stride 0x3C; ebp+1Ch receives 12 floats;
    ebp+20h receives 3 floats).
  - Callers (python3 $S/w23/xref.py 00aa2110): 0x00A89989 in 0x00A898D0 `?GetHelperPointPositionAnimated@C3DMesh2@@UBE?AVC3DVector@@ABVC3DHelperPoint@@ABVC3DAnimationBlendState@@1MAAVC3DAnimationManager@@@Z`
    (labels tsv), 0x00A89FD1 in 0x00A89E40 and 0x00A8A640 in 0x00A8A5F0 (the blended GetDummyObjectPositionAnimated /
    GetDummyObjectTransformAnimated overloads, rebuild/manifest/functions.tsv) -- all three are the two-blend-state + float
    overloads. None landed.
- Declaration: `void CalcBlendedBoneTransformation(const C3DAnimationBlendState& start_blend, const C3DAnimationBlendState& end_blend,
  float blend, const C3DMesh2& mesh, unsigned long bone, CMatrix3x4& transform, C3DVector& scale);` public, non-const (QAE).
  Parameter names descriptive (match the 0x00AA0090 names start_blend/end_blend/blend/mesh).

## 3. Callees (retail -> landed row -> PDB)
| addr | at | what | landed row | note |
|---|---|---|---|---|
| 0x00464931 | 2137 | vector<unsigned long>::resize(1, const unsigned long& = 0) on BoneIndicesBuffer | 00/46/CArray_resize_00464931.cpp | FLAG: landed spelling is a stand-in "CArray::resize"; real = STLport `std::vector<unsigned long>::resize(size_type,const unsigned long&)` (stays a CALL: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE, stlport/fable_containers.h:540, or the one-arg resize(n) inline forwarding `resize(n,_Tp())` -- the temp 0 at [esp+20h] is that `_Tp()`) |
| 0x00464ACE | 218B | vector<unsigned long>::_M_insert_overflow(pos, x, __true_type, 1, true) (push_back slow path) | 00/46/stdvectorunsignedlong_Minsertoverflow_00464ace.cpp | declared-only in fable_containers.h:566 -> call |
| 0x00A5C910 | 2201, 2AE3 | CSimpleMemoryPool::Allocate via inline AllocateType<C3DKeyframe>(count*5) and AllocateType<C3DVector>(count) | 00/a5/CSimpleMemoryPool_Allocate_00a5c910.cpp | engine/CSimpleMemoryPool.h:38 AllocateType (inline, matches) |
| 0x00AA0DC0 | 2235 | C3DAnimationManager::CombineBlendedBaseAnimationsBoneList(const C3DAnimationStateBase&, const C3DAnimationStateBase&, float, C3DKeyframe*, unsigned long, unsigned long, const C3DMesh2&, const CArray<unsigned long>&) | 00/aa/C3DAnimationManager_CombineBlendedBaseAnimationsBoneList_00aa0dc0.cpp | Ego_r 00be37c0 / FableWin 017cc365 `?CombineBlendedBaseAnimationsBoneList@C3DAnimationManager@@AAEXABVC3DAnimationStateBase@@0MPAVC3DKeyframe@@KKABVC3DMesh2@@ABV?$CArray@K@@@Z` (PRIVATE AAE); spelling agrees. Same TU, defined EARLIER (0x00AA0DC0 < 0x00AA2110) -> declare only; retail calls it |
| 0x00A242C0 | 2291 | CMeshDataBank::PeekAnimation(unsigned long) const | 00/a2/CMeshDataBank_PeekAnimation_00a242c0.cpp | FLAG: manifest row label propagated (functions.tsv calls it CTCInventoryAbilities::GetAbilityLevel); landed leaf ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z is right |
| 0x00A26C60 | 22A1 | C3DAnimationInfo::GetAnimation(const C3DMesh2&) const -> CCountedPointer<const C3DAnimation2> (hidden ret ptr = &[esp+28h]) | 00/a2/C3DAnimationInfo_GetAnimation_00a26c60.cpp | ok |
| 0x00A52650 | 22C7 | C3DAnimation2::ConvertTimeToFrames(float, long&, float&) const | 00/a5/C3DAnimation2_ConvertTimeToFrames_00a52650.cpp | FLAG: landed defines it on a `C3DAnimation2_Methods` stand-in; use overlay FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY (visible body, stays a call) as 0x00AA0090 |
| 0x00A4C1F0 | 2359, 23FB, 26E6, 2744 | C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, unsigned long, float) const | NOT landed | Ego_r ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@KM@Z; reached through the overlay C3DAnimationSequence::GetKeyframeInterpolate forwarder (FRAMEKEYFRAME) |
| 0x00A88C10 | 2369, 2607 | CQuaternion::operator*=(const CQuaternion&) | 00/a8/CQuaternion_operator_00a88c10.cpp | `??XCQuaternion@@QAEABV0@ABV0@@Z` ok (out of line: MULASSIGN declares only) |
| [Info+4] / 0x00BFE9BC | 2A6C/2A74, 2D1C/2D24 | CCountedPointer dtor: --RefCount; if 0 `ecx=Info->Data ; call [Info+4]` then operator delete(Info) | engine/CCountedPointer.h default dtor; global_operatordelete_00bfe9bc.cpp | as 0x00AA0090 |
| 0x00A45260 | 2AFC | C3DSkeletalMorph::PeekCombinedMorph() const -> CCountedPointer<const C3DSkeletalMorphTarget> (hidden ret = &[esp+28h], ecx = Data->SkeletalMorph.Data) | 00/a4/C3DSkeletalMorph_PeekCombinedMorph_00a45260.cpp | FLAG: landed spells it NON-const returning CCountedPointer<C3DSkeletalMorphTarget>; Ego_r/FableWin `?PeekCombinedMorph@C3DSkeletalMorph@@QBE?AV?$CCountedPointer@$$CBVC3DSkeletalMorphTarget@@@@XZ` (NAME_FIXES candidate, same as twin dossier) |
| 0x00A44CD0 | 2B31 | C3DSkeletalMorphTarget::GetBoneScaling(C3DVector&, unsigned long) const | 00/a4/C3DSkeletalMorphTarget_GetBoneScaling_00a44cd0.cpp | agrees |
| 0x00A5C720 | 32CE | CSimpleMemoryPool::FreeAll() | 00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp | Ego_r ?FreeAll@CSimpleMemoryPool@@QAEXXZ (FREEALL opt-in) |
Globals: g_SSEAvailable 0x013D2880 (bool), g_ArcCosTable 0x013CB530, g_CosTable 0x013CD550; float consts 1.0f 0x0122DED8,
0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 0.05f 0x0125A15C, 0.0001f 0x0129BA3C (also in 0x00AA0090 0x00AA021C), 512.5f 0x0129CD1C,
0.5f 0x0122F59C, 0.15915494f 0x01237F08, 1024.0f 0x01230010.

## 4. Members (retail offsets)
| access | member | header / opt-in |
|---|---|---|
| this+4 | C3DAnimationManager::MeshBank | overlay engine/C3DAnimationManager.h (agent3022/hdr; NO repo header) |
| this+8/+0xC/+0x10 | BoneIndicesBuffer (CArray<unsigned long>: _M_start/_M_finish/_M_end_of_storage) | same overlay; Ego_r name BoneIndicesBuffer (struct_layouts_egor.tsv) |
| this+0x14 | MemoryPool (CSimpleMemoryPool) | same (struct_layouts_egor says +24 = later build; overlay asserts 0x14) |
| blend+4 | C3DAnimationBlendState::Data -> CSharedData: BaseAnimation +8, CombinationBlends +0xC/+0x10 (20-byte C3DAnimationStateAlpha), SkeletalMorph.Data +0x18 | repo engine/C3DAnimationBlendState.h: FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA (PeekBaseAnimation, GetCombinationBlendCount, PeekCombinationBlend) + FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH (HasSkeletalMorph / PeekSkeletalMorph, chunk Q32, already in repo -- needs SHAREDDATA too). The agent3022 overlay copy predates SKELETALMORPH: merge |
| state+8/+0xC/+0x10 | C3DAnimationStateAlpha AnimationIndex / AnimationTime / AlphaWeight | C3DAnimationState.h FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS + C3DAnimationStateAlpha.h FABLE_RETAIL_C3DANIMATIONSTATEALPHA (overlay) |
| anim+8 | C3DAnimationStats AnimType (IsRelative bit 2, IsPartial bit 1) | FABLE_RETAIL_C3DANIMATIONSTATS_ISRELATIVE (overlay) |
| anim+0x5C / +0x68 | Sequences (8 bytes each, Data +0) / BoneMask (unsigned char*) | C3DAnimation2.h FABLE_RETAIL_C3DANIMATION2_MANAGER (PeekSequence, IsBoneEnabled) |
| seqdata+0xA bit7 | IdentityFlag | FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY |
| mesh+0x9C, stride 0x3C | C3DMesh2::Bones -> C3DBone2 NameCRC +0, Parent +4 | repo C3DMesh2.h FABLE_RETAIL_C3DMESH2_BONES (GetBone); C3DBone2: NO repo header -- overlay agent2047/hdr/engine/C3DBone2.h FABLE_RETAIL_C3DBONE2_PEEK has GetParent but NOT GetNameCRC; landed 00/bd/..._InitialiseBoneScalings_00bd2a90.cpp models C3DBone2 locally WITH GetNameCRC (Ego_r ?GetNameCRC@C3DBone2@@QBEKXZ) -> add GetNameCRC to the overlay opt-in |
| keyframe (0x30) | C3DKeyframe Rotation CQuaternion +0, Position C4DVector +0x10 | overlay C3DKeyframe.h: POSTAPPLY, BLEND (agent3022) + TOMATRIX4_INLINE (agent2047); GetPosition() by value exists; SetPosition(const C3DVector&) MISSING (FableWin ?SetPosition@C3DKeyframe@@QAEXABVC3DVector@@@Z 017aa404) |
| C3DVector | X/Y/Z | repo engine/C3DVector.h FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_C3DVECTOR_GETSCALED (`?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z`, FableWin 017fe9aa; componentwise; `fld this.k ; fmul scale.k`) |
| CMatrix4x4 0x40 | 4x4 floats | overlay agent2047/hdr CMatrix4x4.h: FABLE_RETAIL_CMATRIX4X4, _ELEMENT_ACCESS, _ALIGNEDSETMULTIPLY_ACCESSORS; CQuaternion.h FABLE_RETAIL_CQUATERNION_MATRIX4 (+_FORCEINLINE). MISSING: CopyTo(CMatrix3x4&) const (FableWin ?CopyTo@CMatrix4x4@@QBEXAAVCMatrix3x4@@@Z 017a3294) |
| CMatrix3x4 out | 12 floats, rows of 3 | repo engine/CMatrix3x4.h |
Classes with no repo header: C3DAnimationManager, C3DBone2, C3DSkeletalMorph, C3DSkeletalMorphTarget (model the last two locally,
declaration-only methods, exactly as landed 0x00BD2A90 does). $W/hdr is EMPTY and there is no draft: merge agent3022/hdr (animation
set, latest) with agent2047/hdr (CMatrix4x4.h, CMatrix4x4AlignedSSE.h, C3DBone2.h, C3DKeyframe.h TOMATRIX4, CQuaternion.h MATRIX4);
then add a C3DAnimationManager opt-in (e.g. FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM) declaring public
CalcBoneTransformation / CalcBlendedBoneTransformation and private CombineBaseAnimationsBoneList / CombineBlendedBaseAnimationsBoneList
(Ego_r spellings above) -- coordinate the opt-in name with the twin 0x00AA0FA0's author (same wave bwf3).

## 5. Callee-body visibility
- Must expand inline (bodies visible / FORCEINLINE): resize's one-arg form is NOT expanded (call), push_back fast path IS expanded
  (0x00AA2160..2175: `cmp finish,eos ; je overflow ; test eax,eax ; je ; mov [eax],esi ; add [edi+4],4`), AllocateType<T>,
  size() (sar 2), operator[] of BoneIndicesBuffer, GetCombinationBlendCount (/20 via 0x66666667) / PeekCombinationBlend /
  PeekBaseAnimation, GFGetInterpolatedValue (FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED), IsRelative/IsPartial, PeekSequence /
  IsIdentity / IsBoneEnabled / GetKeyframeInterpolate forwarder, the four static FABLE_FORCEINLINE Apply* helpers +
  PostApplyRelativeAnimation(Alpha), SetSlerpFromIdentity, SetSlerp, BlendWith, GFFastArcCos/GFFastSin (agent3022 COMMON HELPER
  BLOCK -- copy verbatim), HasSkeletalMorph / PeekSkeletalMorph, GetBone / GetParent / GetNameCRC, GetPosition / GetScaled /
  SetPosition, C3DKeyframe::ToMatrix(CMatrix4x4&, bool) + CQuaternion::ToMatrix 4x4 (forced; FableWin emits both COMDATs right
  after CalcBoneTransformation), AlignedSetMultiply (forced, header), SWAP<CMatrix4x4*> (FableWin
  `??$SWAP@PAVCMatrix4x4@@@@YIXAAPAVCMatrix4x4@@0@Z` 01802307, __fastcall template, inline; no repo definition), CMatrix4x4::CopyTo,
  CCountedPointer dtor.
- Out of line (calls): resize 0x00464931, _M_insert_overflow 0x00464ACE, Allocate, FreeAll, CombineBlendedBaseAnimationsBoneList
  (same TU, earlier -- declare only), PeekAnimation, GetAnimation, ConvertTimeToFrames (body visible via CONVERTTIME_BODY, still a
  call), GetKeyframeInterpolate (data), operator*=, PeekCombinedMorph, GetBoneScaling.

## 6. Sibling rows (idioms to copy)
- agent3022/src/00aa0090.cpp CalcBlendedMeshTransformations (RELOCATION_MATCH; THE model for the blend loop): opt-in list (29
  defines incl. GFGETINTERPOLATEDVALUE_NAMED, CONVERTTIME_BODY, SLERPFROMIDENTITY, BLEND), helper block, `fabsf(alpha - 1.0f) <
  0.0001f`, `#pragma intrinsic(sqrt)` / `(fabs)`, no Flags line, no optimize pragmas.
- agent3022/src/00a9f540.cpp CalcMeshTransformations: loops written `{ C3DKeyframe& keyframe = keyframes[bone]; ApplyX(keyframe,...); }`
  -- fixes the BlendWith dot order Y,Z,W,X; THIS row's BlendWith dot is also Y,Z,W,X (0x00AA2749..277D: [124h],[128h],[12Ch],[120h]),
  so try the named-keyframe spelling first (the twin 0x00AA0FA0 has the same bytes there). 0x00AA0090's unnamed spelling is
  the fallback.
- agent2047/src/00a9e1e0.cpp ApplyParentTransformations: `__declspec(align(16)) CMatrix4x4 local; keyframes[i].ToMatrix(local,false);
  m.AlignedSetMultiply(a, b);` + opt-ins CMATRIX4X4, _ELEMENT_ACCESS, ALIGNEDSETMULTIPLY_ACCESSORS, CQUATERNION_MATRIX4(_FORCEINLINE),
  C3DKEYFRAME_TOMATRIX4_INLINE, C3DMESH2_BONES, C3DBONE2_PEEK, and its Retail-asm line.
- landed 00/bd/CEngineInternalPrimitiveAnimatedMesh_InitialiseBoneScalings_00bd2a90.cpp: HasSkeletalMorph/PeekSkeletalMorph/
  PeekCombinedMorph/GetBoneScaling/GetNameCRC idiom and local C3DBone2 / C3DSkeletalMorph(Target) models.
- landed 00/aa/C3DAnimationManager_CombineBlendedBaseAnimationsBoneList_00aa0dc0.cpp (same TU; local models, no flags).
- twin scout dossier /opt/fable_lane/w22/bwf3/00aa0fa0/DOSSIER.md.
