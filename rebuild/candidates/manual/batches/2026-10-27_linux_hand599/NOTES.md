# Hand chunk 599 (libvorbis seed_curve .. PKSerialiseLoad<CBankData>): 10 of 10 byte-exact

10 RELOCATION_MATCH, all genuine C++ (src/; purity.classify = genuine for every source). 0 MATCH, 0 retail_asm, 0 parked, 0 skipped.
All 10 tests PASS under g++ (tall.sh) and VC7.1 (tall.py). gall.sh grades every row on its `// Leaf:` symbol.

## What matched (10)
- **libvorbis psy.c -- `seed_curve` 0x00C92CA0** (376, `_seed_curve@28`). Name **proven**: the libvorbis 1.0.1 psy.c body
  verbatim (P_LEVEL_0 30, EHMER_OFFSET 16, the 0.1 level step), called from seed_loop 0x00C92E20 inside the landed
  `_vp_tonemask` TU. Shape: `static __declspec(noinline) void __stdcall` inside that TU (seed in edi, `ret 18h`). Two details
  were needed. First, the C TU's min/max are VC `<stdlib.h>`'s `(((a) > (b)) ? (a) : (b))` forms, not libvorbis os.h's.
  Second, the level step is `.1f`: the retail pool constant is 0.1f widened, 0.10000000149. Bonus: seed_loop 0x00C92E20 (the
  chunk-447 parked row) also grades RELOCATION_MATCH from this TU (see Review at landing).
- **libvorbis floor1.c -- `render_line` 0x00CA1E80** (388, `@render_line@20`, /Gr). Name **proven**: floor1.c verbatim, and
  its only caller is the landed floor1_inverse2 0x00CA3420. Shape: a same-TU `static __declspec(noinline)` with
  floor1_inverse2's body in the file gives retail's private convention (y1 in ecx, `ret 10h`). This is the chunk 596 lever;
  the hand448 "LTCG register argument" gap label was wrong.
- **STLport 4.6 -- `std::__merge_adaptive<CSortElement*, int, CSortElement*, less<CSortElement>>` 0x00B862C0** (389). Name
  **proven**: the STLport _algo.c body, with element and comparator from the bound callees: merge 0x00B85510,
  __merge_backward 0x00B859A0, __lower_bound 0x00B85240, __upper_bound 0x00B85280 and __rotate_adaptive 0x00B85CD0.
  Shape: the buffer copy chain (copy, __copy_ptrs(__false_type), __copy) must be `__forceinline`; plain inline gives
  338v389.
- **Engine -- `CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars()` 0x00B99860** (386). Name **proven**:
  the Ego_r and FableWin PDB name sets in donor order, and its only caller FrameStart 0xB99B50 tail-jumps to it. It
  includes engine/CEngineCamera.h and engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h through the layout-rename
  pattern. The camera global is `g_pEngineCamera` 0x01436EA0, the dominant bound spelling. Shape: a pack(1) C3DVector with a
  user copy ctor (memberwise). Without it VC7.1 fully unrolls the 4-iteration loop (1444v386).
- **Engine -- `CParticleSimulation::ReadVectorArray(C4DVector*, unsigned long, CDataInputStream&, float)` 0x00AE1730** (391).
  Name **proven**: the Ego_r PDB, and its callers are CParticleSimulation::ReadFromFile 0xAE1BC0 and
  CParticleProgram::ReadFromFile 0xAE3C30. It uses engine/CDataInputStream.h (FABLE_CDATAINPUTSTREAM_FRIEND). `CArray<T>`
  has the PDB members GetZeroSizeArray, GetAsCArray, GetByteSize and `explicit CArray(unsigned long)` 0x7C2AB0. Shape: the
  loop body is the PDB inline `vectors[i].Initialise(temp[i], w)` (`C4DVector::Initialise(const C3DVector&, float)`). That
  inline member call is what gives retail's 4x unroll; plain field stores give 234v391.
- **libpk OpenAL -- `CAudioSource::Stop()` 0x00C72620** (385). Slot name **proven**, class descriptive (landed). It is slot 2
  of the source vtables 0x012B8CC8 / 0x012B8D48 / 0x012BEE40. Slot 2 of the voice base 0x012B5C10 is bound
  `?Stop@CVtable_012B5C10@@UAEXXZ` (0x00C59710) by two landed callers: ~CVtable_012B5C10 0x00C3F3D0 and
  CAudioPlatform::ReleaseVoice 0x00C4D070 (`voice->Stop()`). The model is the landed CAudioSource::Initialise 0x00C72930,
  plus typed alGetSourcei / alSourceStop / alSourcei / alDeleteSources resolvers. Shape: a reference local
  `CAudioContext& context = g_AudioContext;`. Direct `g_AudioContext.` calls swap esi and edi (384v385).
- **libpk -- `PKCTRArray::RemoveValue(CUnprovenArrayComparator*, const void*)` 0x00C322E0** (385, /EHsc). The class is
  **proven**: vtable 0x012B4E80 slot 6, and the PKCTRArray.cpp line 154 / 157 traces. The method name is **descriptive**.
  Shape: FindValueIndex 0x00C32160 has its body in the TU under `#pragma auto_inline(off)`. VC then knows `found` is only
  written there, homes it in a dead argument slot, and drops the `push ecx` frame (386v385 without it).
- **libpk -- `PKSerialiseLoad<CBankData>(IRemoteSource*)` 0x00C17940** (393, /EHsc). **Descriptive** template, from the
  PKSerialiseLoad.h line 34 "EMEM" trace and the 0x30-byte CBankData (ctor 0x00C2EED0). The steps are:
  1. CMemoryStream over the source.
  2. `PKCAutoDelete<CBankData>(new CBankData)`.
  3. The null assert.
  4. `PKSerialiseCParser parser(&stream)`.
  5. `object->Transfer(parser)`.
  6. `Release()`.
  Shape: GetChecked (PKCAutoDelete.h line 32) and Release (line 42) have their bodies in the TU under auto_inline(off).
  Without them the result is 395v393. CBankData is `struct` (the landed majority).
- **IMA ADPCM codec -- `CImaAdpcmCodec::CalculateError(const BYTE*, const BYTE*, UINT, UINT, UINT)` 0x00C5F150** (381). The
  class is **proven**: the row sits inside the landed codec block 0x00C5F000..0x00C5F9E0. Its three callers (0x00C5FC52,
  0x00C5FE27, 0x00C5FEF0) are the codec's step-index-search convert 0x00C5FA50 (this in ebx, m_pfnConvert +0x20). this+0x12
  is IMAADPCMWAVEFORMAT::wSamplesPerBlock. The method name is **descriptive**; the public SDK imaadpcm.cpp has no such
  method. What it computes:
  - Per block: the sum of squared (decoded - source) 16-bit differences.
  - Normalised by wSamplesPerBlock * 2^32 and scaled by ~0ULL / cTotalBlocks.
  - Uses _aulldiv, _allmul and _ftol2 (compiler-emitted).

  Shape: the block pointers are `base + i * cbBlock`. Two `+= cbBlock` pointers give a 0x34 frame and an extra register.
- **libpk reverb -- `CReverbDelay::Blend(SReverbDelayParams*, const SReverbDelayParams*, const float*, float*, unsigned,
  unsigned)` 0x00C76C20** (381). The class is **proven**: the landed CReverbDelay::Process 0x00C76B60 names this address its
  ramping twin, with the same layout and params and the same CDspObject::s_pOps callees. The method name follows the
  reverb module's Blend pattern (CCombFilter::Blend 0x00C76120, stage Blends 0x00C75AB0 / 0x00C75E90). The caller 0x00C6A080
  passes (current, target, input, output, count, BlendPosition). The constants are 1/512, -1/512, 1.0f and 2^32. Matched
  first try from the landed Process model.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00C92CA0 "Graphics_ConvertFloatToFixed" -> libvorbis psy.c seed_curve.
- 0x00C5F150 "Audio_ComputeEnergy" -> CImaAdpcmCodec::CalculateError (squared decode error, not energy).
- 0x00C76C20 "Array_PartitionAndInsert" -> CReverbDelay::Blend (not STLport).
- 0x00C322E0 "sub_00c322e0" -> PKCTRArray::RemoveValue.
- 0x00C72620 "sub_00c72620" -> CAudioSource::Stop.
- 0x00B99860 "engine_copy_global_transform_to_array" -> CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars.
- 0x00CA1E80 "CurveInterpolation_InterpolateFloats" -> libvorbis floor1.c render_line.
- 0x00B862C0 "std_introsort_merge_operation" -> std::__merge_adaptive.
- 0x00AE1730 "ReadVectorArray" (module global) -> CParticleSimulation::ReadVectorArray (static member).
- 0x00C17940 "DeleteSelectedTrackNode_Implementation" -> PKSerialiseLoad<CBankData>.
- Landed callers with wrong spellings are in NAME_FIXES.tsv: 0x00C2EE80, 0x00C17740, 0x00C17850, 0x00C31970, 0x00C32160,
  0x00C59710, 0x00B85510, 0x00C76C20, 0x00C76460 and the 0x0143CCDC global.

## Names: proven vs descriptive
- Proven (two sources): seed_curve, render_line, std::__merge_adaptive<...>,
  CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars, CParticleSimulation::ReadVectorArray.
- Proven slot or class, descriptive method:
  - CAudioSource::Stop: the slot name is bound; the class is the landed descriptive CAudioSource.
  - PKCTRArray::RemoveValue.
  - CImaAdpcmCodec::CalculateError.
  - CReverbDelay::Blend.
- Descriptive: PKSerialiseLoad<CBankData> (from the libpk trace file name).
- Inline helpers that are descriptive:
  - GetPosition and GetFrustrumPlane in 0x00B99860.
  - CDataInputReader in 0x00AE1730.
  - CAudioContext's GetSourcei, SourceStop, Sourcei and DeleteSources resolvers in 0x00C72620.

## Header debt
- No engine headers exist for CSIMDVariableBlock, CEngineCameraDesc, CPlane, C3DVector, C4DVector and CArray<T>. Local
  PDB-member models are in 0x00B99860 and 0x00AE1730. engine/CEngineCamera.h keeps CameraDesc and FrustrumPlanes opaque.
  engine/CDataInputStream.h has `void* CurrentSourcePtr`.
- libpk and libvorbis have no shared headers (no PDB). Local models were copied from landed rows:
  - CAudioContext and CVtable_012B5C10 from 0x00C72930;
  - CRingBuffer, CDspOps, CDspObject and SReverbDelayParams from 0x00C76B60;
  - the CImaAdpcmCodec and pshpack1 mmsystem structs from 0x00C5F9E0;
  - vorbis_look_psy and vorbis_info_psy from 0x00C93E30.
- CImaAdpcmCodec +0x14 is modelled as the landed m_fEncoder, but 0x00C5FA50 switches on it as a mode 0..3. Its converter
  typedef in the landed rows is the six-argument SDK form; EncodeM16's notes say retail passes five stack arguments.
- engine/CEngineSubPrimitiveRenderer.h does not declare the nested PDB class CSortElement (8 bytes: SortValue,
  SubPrimitive). vland warns SHARED_HEADER_AVAILABLE for 0x00B862C0, but `this` is not that class (free STLport template).
  The row uses the same local nested model as the landed std::__rotate 0x00B85660. Proposed header change: add
  `class CSortElement { public: unsigned long SortValue; CEngineSubPrimitiveBase* SubPrimitive; };` inside the class.

## Read-only verifier (vland.py)
WINS 10, 0 RELOC_IDENTITY, 0 DIFFER. The warnings are all RELOC_NAME label mismatches. They cover the NAME_FIXES.tsv
addresses, the CArray<C3DVector> ctor 0x007C2AB0 (bound spelling used) and the libpk CDebugStream trace helpers
0x00C0DEC0, 0x00C0DF00 and 0x00C0BA40. CDebugStream is the dominant landed spelling (59 sources); the bindings hold several
older spellings. lv/ is deleted.
