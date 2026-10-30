# Hand chunk 585 (std::vector<C3DGroup2> .. NParticleEngine::CPSCOrbit::COrbitState): 8 of 10 byte-exact

Ten engine-range rows (catalog class `global`). 8 matched (1 MATCH, 7 RELOCATION_MATCH), 2 parked in differ/
(one is a retail_asm-grade row), 0 skipped. All 10 behaviour tests PASS under g++ and VC7.1 (tall.py; the two
differ tests run through the same verify_and_land.behaviour_of harness).

## What matched (8)
- **std::vector<C3DGroup2>::operator= 0x00AE7FE0** (RELOCATION_MATCH, first grade): STLport 4.6 vector.c operator=
  verbatim. `_M_allocate_and_copy` and `__copy_ptrs` inline (malloc and the `__copy(..., random_access_iterator_tag(),
  (int*)0)` call show in the body), the element helpers declared only: `__uninitialized_copy<const C3DGroup2*>`
  0x00AE7530, `__copy` 0x00AE7660, `_Destroy` 0x00AE7EC0 (fastcall), `_M_clear` 0x00AE7FA0. The tags land on the
  dead `__x` slot without same-TU bodies here. Relocations checked in call order against retail.
- **std::vector<pair<EPSCType,CComponentInstance>>::_M_insert_overflow 0x00AF7460**: STLport 4.6 `_vector.c`. Retail
  homes the `__false_type` temporaries on a live argument slot, and that needs same-TU noinline bodies for the
  tag-taking helpers (`__uninitialized_copy` 0x00AF6920, `__uninitialized_fill_n` 0x00AF6980, `__destroy_aux`
  0x00AF6F90); `_M_clear` is a forceinline destroy + deallocate. CComponentInstance derives from CBaseClass
  (`engine/CBaseClass.h`) and holds a `CCountedPointer<CPSCBase>` whose copy ctor addrefs.
- **std::vector<CArray<C3DVector>>::_M_insert_overflow 0x00BCDB70**: STLport 4.6, with plain `inline __fastcall`
  `__uninitialized_copy`. That gives retail's split: the prefix copy inlined, the suffix copy out of line (0x00BCD8A0).
  `_M_clear` 0x00BCDA90 and `vector<C3DVector>(const vector&)` 0x006F0810 are declared only. The element is
  WeaponTrail::Trail's CArray<CArray<C3DVector>>; the caller is `_M_fill_insert` 0x00BCDCA0.
- **CMatrix3x4::SetFromVectorAndAngle 0x00C1D0B0**: the Rodrigues axis-angle matrix (`(1-xx)c+xx`, `xy-xy*c+sz`, ...)
  with E41..E43 zeroed. It is a `__fastcall` member (QAI), following the landed PreMultiply/PostMultiply precedent. Each
  product local is its own declaration, and their order sets the load order.
- **libvorbis ov_time_seek 0x00C8B770 / ov_time_seek_page 0x00C8B880**: vorbisfile 1.0.x verbatim (OV_EINVAL -131,
  OV_ENOSEEK -138). They follow the landed ov_time_tell model: a same-TU noinline `ov_time_total` body under
  auto_inline(off) and `extern "C" __fastcall`. Callees: ov_pcm_total 0x00C8AA80, ov_time_total 0x00C8AAF0, __ftol2
  0x00BFEA70, and ov_pcm_seek 0x00C8B490 or ov_pcm_seek_page 0x00C8AE90.
- **libvorbis mdct_butterfly_first 0x00C9C1E0** (MATCH): Xiph mdct.c verbatim as a `static __declspec(noinline)`
  function, with its verbatim caller `mdct_butterflies` (0x00C9C400) in the same TU. At plain /O2 VC7.1 gives the
  TU-local static a private register convention (x in edi, points in ecx, T in edx) that matches retail without /GL.
  `mdct_butterfly_generic` 0x00C9C2F0 and `mdct_butterfly_32` 0x00C9C000 are declared only.
- **NParticleEngine::CPSCOrbit::COrbitState::GetSplinePosition 0x00B177C0**: the cubic Hermite position
  `From*h1 + To*h2 + FromTangent*h3 + ToTangent*h4`. VC7.1 has no NRVO, so the named `C3DVector result` local is what
  matches (returning the expression directly graded 267v265).

## Parked for Fable 5.1 (2)
- 00bebaa0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix DIFFER(255v257): VC7.1 emits `pop esi` right after the last esi copy where retail keeps esi pushed to the epilogue (0x00BEBB9A), so every later [esp+N] is 4 lower | tried: six local orders, CRightHandedSet copies (implicit and user copy ctor), cross product on copies / on rhset / per component, GetUp/GetForward accessors, a 12-float Set, Set(4 vectors) with operator- temporaries, explicit translation stores, scaling orders, RTM and QFE 4035, /Oa /G6 | next: lane gap: callee-saved shrink-wrap (find a source that keeps esi live to the end, e.g. the real CRightHandedSet::GetRight inline from the shared header once it exists)
- 00c77c80 CReverbProcessor::UpdateLineAndTapLengths DIFFER(259v259): in the tap loop retail keeps the store pointer in edx and the rate/length temporary in esi; VC7.1 picks esi/edx (one register swap, everything else matches) | tried: unsigned/int counter mixes, separate tap/k counters, `!=` bounds, a named length temporary, a tap pointer, a table pointer, record references, a forceinline AlignedLength helper, RTM and QFE 4035 | next: lane gap: register allocation (try the landed UpdateChorusLength helper split into a noinline same-TU function). This row is retail_asm grade: its `// Retail-asm:` line cites the fistp at 0x00C77CEF / 0x00C77D55. When it matches it goes in retail_asm/, not src/.

## Skipped
None.

## Names and evidence
- 0x00AE7FE0 `??4?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@std@@@std@@QAEAAV01@ABV01@@Z`, **proven**. Every callee is
  a landed vector<C3DGroup2> helper (0xAE7530, 0xAE7660, 0xAE7EC0, the bound `_M_clear` 0xAE7FA0), and the body is the
  STLport operator= (the twin of the landed C3DVertexBlend2 operator= 0x00AE6690). Callers: 0xAE39E7 and 0xAE81A8.
- 0x00AF7460 `?_M_insert_overflow@?$vector@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@...`,
  **proven**. Its callees are the landed pair<EPSCType,CComponentInstance> helpers; callers are `_M_fill_insert`
  0xAF7570 and `insert` 0xAF7730.
- 0x00BCDB70 `?_M_insert_overflow@?$vector@V?$CArray@VC3DVector@@@@...`, **proven**. Caller `_M_fill_insert` 0xBCDCA0; the
  resize chain runs on CEngineInternalPrimitiveWeaponTrail::Trail (CArray<CArray<C3DVector>>).
- 0x00C1D0B0 `?SetFromVectorAndAngle@CMatrix3x4@@QAIXABVC3DVector@@M@Z`, **proven**: Ego_r.pdb and FableWin.pdb, and the
  body is the axis-angle rotation. The PDBs spell it QAE; retail passes `this` in ecx and axis in edx, so it is QAI
  like the landed PreMultiply/PostMultiply.
- 0x00C8B770 `@ov_time_seek@12` and 0x00C8B880 `@ov_time_seek_page@12`, **proven**: vorbisfile source order between
  the landed ov_pcm_seek / ov_time_tell, the bound callee names, and the OV_* constants.
- 0x00C9C1E0 `_mdct_butterfly_first`, **proven**: Xiph mdct.c body and its caller mdct_butterflies 0x00C9C400 (calls
  first, generic, 32 in source order).
- 0x00B177C0 `?GetSplinePosition@COrbitState@CPSCOrbit@NParticleEngine@@QBE?AVC3DVector@@M@Z`, **proven**: FableWin.pdb
  and the COrbitState member offsets (FromPosition +8, ToPosition +0x14, FromTangent +0x2C, ToTangent +0x38) in the
  CPSCOrbit TU.
- 0x00BEBAA0 `?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z`,
  **proven**: the PDB static `SI` fastcall signature matches the registers, and the callers fit.
- 0x00C77C80 `CReverbProcessor::UpdateLineAndTapLengths`, **descriptive**. libpk has no PDB. The dispatcher 0xC783F0
  calls it for flag 0x10000 and after 0xC77B30 for 0x12000.

## Callee names
All callees are bound or landed names, except `mdct_butterfly_generic` (0x00C9C2F0) and `mdct_butterfly_32`
(0x00C9C000), which are Xiph names proven by the mdct_butterflies call order. The tables in 0x00C77C80 are descriptive:
`g_reverbLineDescs` 0x012BF0E0 and `g_reverbTapDelays` 0x012BF178.

## Label corrections
- 0x00AE7FE0 Vector_InsertRange_Pair128Bit -> std::vector<C3DGroup2>::operator=
- 0x00AF7460 Vector_InsertWithCount_24Byte -> std::vector<pair<EPSCType,CComponentInstance>>::_M_insert_overflow
- 0x00BCDB70 std_vector_realloc_with_fill_12byte -> std::vector<CArray<C3DVector>>::_M_insert_overflow
- 0x00B177C0 Matrix_TransformVector -> NParticleEngine::CPSCOrbit::COrbitState::GetSplinePosition
- 0x00BEBAA0 CalcObjectMatrix (free function) -> CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (class static)
- 0x00C1D0B0 Matrix_RotateAroundAxis -> CMatrix3x4::SetFromVectorAndAngle
- 0x00C8B770 Audio_SeekToSample -> libvorbis ov_time_seek
- 0x00C8B880 Audio_SeekToSampleWithNotification -> libvorbis ov_time_seek_page
- 0x00C77C80 Audio_Channel_ComputePitch_Modulation -> CReverbProcessor::UpdateLineAndTapLengths (descriptive)
- 0x00C9C1E0 FFT_Radix2MixedStride -> libvorbis mdct_butterfly_first. Neighbours: 0x00C9C2F0 FFT_Radix2VariableStride ->
  mdct_butterfly_generic; 0x00C9C000 C3DBoundingBox::TransformByRotation -> mdct_butterfly_32; 0x00C9C400 -> mdct_butterflies.
- 0x00C8B9D0 Audio_GetCurrentSamplePosition is the landed ov_time_tell (the label was never updated).

## Header debt
- No `rebuild/include/engine/` header exists for C3DVector, CMatrix3x4, CRightHandedSet,
  CEngineInternalPrimitiveMeshBase, NParticleEngine::CPSCOrbit / COrbitState, CComponentInstance, CCountedPointer or
  CArray. Every row uses local models with the retail layout, as the landed siblings do. CComponentInstance does use
  `engine/CBaseClass.h` for its base.
- C3DGroup2: the Ego_r size is 20, retail is 16 (CArray is a 12-byte STLport vector, not the 16-byte Dinkumware one).
  The same Dinkumware-size problem affects every Ego_r CArray-holding class, e.g. the CEngineInternalPrimitiveWeaponTrail
  member offsets.
- CComponentInstance has a retail vtable (0x0129F478, `??_7CComponentInstance@NParticleEngine@@6B@`, stored by the inlined
  element copy in 0x00AF7460). The model gets its virtuals from the shared `engine/CBaseClass.h` base (RTTI: CBaseClass
  is the single base), not from a flattened `__vftable`. The Ego_r size 0x14 agrees with retail here.
- libpk reverb: the landed SReverbState models put the tap array (SReverbTap) 8 bytes off. 0x00C77C80 re-lays it at
  state +0x88: `long m_delayLength[2]`, panA +8, panB +0xC, level +0x2C, record 0x30.
