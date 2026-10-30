# Hand chunk 596 (C16BitSamplePtr .. libvorbis floor1): 9 of 10 byte-exact

Read-first list: CLAUDE.md, HANDOFF.md, EXTRA_RULES.md, purity.py and the hand594/595 batch files were read in full.
GOTCHAS.md and FINDINGS_LOG.md were read by grep (topic searches: STLport, libvorbis, LTCG, strmbase, libpk, fistp)
and tail, not cover to cover.

## What matched
- 0x00C62A50 `C16BitSamplePtr::C16BitSamplePtr(short*, long)` (337, /EHsc, 1 grade): the init list, then two libpk
  assert blocks. Each block is a CDebugStream log line in its own scope, followed by the CDebugBreak temporary (the
  landed PKCAutoFile::GetChecked 0x00C31DB0 model).
- 0x00AE3670 `CParticleProgram::~CParticleProgram` (338, 1 grade): the body is `Clear();`. Member destruction comes from
  local member models: CVectorMap / CArray over STLport vectors, the inline ~CParticleSimulation / ~CMovableResource,
  and the out-of-line ~CMemoryDataOutputStream.
- 0x00B8F4E0 `vector<CEngineOcclusionManager::COccluderFlareEntry>::_M_fill_insert` (339, 3 grades; the third only re-graded a corrected Leaf line, `...IABU34@@Z`): uses the landed
  0x00AF7570 model. C3DVector needs a user memberwise copy constructor; without it the x_copy is `rep movsd` (316v339).
- 0x00CA8B90 `CBaseInputPin::Receive(IMediaSample*)` (339, 1 grade): the DX9 BaseClasses amfilter.cpp body, verbatim,
  on the landed ReceiveMultiple 0x00CA8CF0 strmbase model. CBaseFilter::NotifyEvent is defined in the TU so it inlines
  (out of line at 0x00CA7930). `this` is the IMemInputPin subobject.
- 0x00C756C0 `CReverbDiffuser::UpdateGain` (341, 1 grade; one compile-only sweep before grading): both chunk lengths
  start at `remaining` before either wrap test.
- 0x00C75820 `CReverbDiffuser::UpdateGainRamped` (343, 1 grade): `float ramp = (float)rampLength;` must be a named
  local, which puts the fmul after the first tap compare as in retail. It uses a 16-byte-aligned float[128] scratch
  block.
- 0x00AF7730 `vector<pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>>::insert` (345, 1 grade): the
  STLport insert body, verbatim. CComponentInstance::operator= (0x00AF6690) and ~pair (0x00AF65B0) are out of line.
- 0x00C8E580 libvorbis `vorbis_synthesis_headerin` (347, /Gr /GS, 1 grade): the 1.0.1 info.c body. Its four statics
  are `static __declspec(noinline)` real bodies in `extern "C"`: _v_readstring, _vorbis_unpack_info,
  _vorbis_unpack_comment and _vorbis_unpack_books, the last two `__stdcall`. Land the leaf only.
- 0x00CA3420 libvorbis `floor1_inverse2` (347, /Gr, 3 grades, all RELOCATION_MATCH; the last two changed only the
  comment and the typedef): the 1.0.1 floor1.c body. It calls render_line 0x00CA1E80, which uses retail's private
  convention (y1 in ecx; x0/x1/y0/d on the stack). That convention comes out when render_line is a same-TU
  `static __declspec(noinline)` body, with no /GL. Land the leaf only.

## Parked for Fable 5.1
- 0x00B22F30 NParticleEngine::GFInterpolateArray<unsigned long> DIFFER(354v340):
  - Diff: retail spills the hidden result pointer to [esp+1Ch], keeps a/b in esi/edi and the sizes in ebx/ebp, and
    places the `i < sizeA` value-select arm after the epilogue with a shared b[i] load. Ours keeps the result in ebx and
    lays the arms out in source order.
  - Tried:
    - three separate GFInterpolate calls (494)
    - if/else select into valueA/valueB (354, the best)
    - `i >= sizeA` first (362)
    - sweeps: ternaries, goto, a pointer select, const sizes, plain inline helpers, `_M_start[i]`, declaration order
      (best 346, but the wrong shape)
  - Next: lane gap, register allocation over an inline-asm helper. Retail-asm evidence is in the file
    (fld/fistp 0x00B22F7F/0x00B22F83, 0x00B22FE5/0x00B22FE9: the GFFastFloatToLong `__asm` helper). If it ever matches,
    it lands as grade retail_asm.

## Skipped
- None.

## Label corrections
- 0x00C62A50 "C16BitSamplePtr_Initialize" -> the C16BitSamplePtr constructor.
- 0x00AE3670 "CLandscapeBackgroundPatch_destructor" -> CParticleProgram::~CParticleProgram.
- 0x00B8F4E0 "Vector_InsertWithCount_CameraTag" -> vector<COccluderFlareEntry>::_M_fill_insert.
- 0x00CA8B90 "AudioFile_QueryCapabilities" -> CBaseInputPin::Receive.
- 0x00B22F30 -> NParticleEngine::GFInterpolateArray<unsigned long>.
- 0x00C756C0 "Graphics_LoadTerrainChunk" -> CReverbDiffuser::UpdateGain.
- 0x00C75820 "Animation_InterpolateAndInsert" -> CReverbDiffuser::UpdateGainRamped.
- 0x00AF7730 "Vector_InsertSingle_Counted" -> vector<pair<EPSCType, CComponentInstance>>::insert.
- 0x00C8E580 "CreatureHitNotification_Construct" -> libvorbis vorbis_synthesis_headerin.
- 0x00CA3420 "Geometry_ScaleVertexArray" -> libvorbis floor1_inverse2.
- 0x00CA1E80 "CurveInterpolation_InterpolateFloats" -> libvorbis floor1.c static render_line.
- 0x013ABE28 -> libvorbis floor1.c FLOOR1_fromdB_LOOKUP: 256 floats, 1.0649863e-07 .. 1.0.
- 0x00C8E200 "C3DGenerator_ReadAnimationData" -> libvorbis info.c _vorbis_unpack_comment; 0x00C8E2E0 "C3DGenerator_ReadComplexMesh" -> _vorbis_unpack_books (the vland RELOC_NAME warnings).

## Names and evidence
- Proven (PDB, SDK or library source, two sources each):
  - CParticleProgram::~CParticleProgram: vtable 0x0129F0D8 plus Ego_r.
  - _M_fill_insert<COccluderFlareEntry>: the resize/FrameStart caller chain plus the landed __copy_backward type.
  - vector<pair<EPSCType, CComponentInstance>>::insert: the AddComponent caller plus Ego_r types.
  - GFInterpolateArray<unsigned long>: the Ego_r name set plus the InterpolateTo caller.
  - CBaseInputPin::Receive: the BaseClasses source plus the IID_IMediaSample2 / VFW_E_* constants.
  - vorbis_synthesis_headerin and floor1_inverse2: libvorbis 1.0.1 source plus the struct offsets and the
    "vorbis" / lookup-table data.
- Descriptive (libpk has no PDB or RTTI):
  - C16BitSamplePtr: from the `.\C16BitSamplePtr.cpp` assert string.
  - CReverbDiffuser::UpdateGain / UpdateGainRamped: the landed binding names, with the history parameter retyped as
    CRingBuffer (NAME_FIXES.tsv).
  - CReverbConfig::m_tapBase (+0x10) / m_tapSpacing (+0x14).
- Callees declared, with spellings checked against reloc_bindings.tsv and the landed definitions:
  - CDspGain::Apply 0x00C74CE0 and CDspGain::Crossfade 0x00C74D30: the landed definitions' own leaves.
  - CDspOps::ReadRingScaledRamp / MixRampFromRing / WriteRing: the landed leaves.
  - CDspObject::s_pOps: bound.
  - CDebugStream appenders, CDebugBreak, g_pDebugBuffer and g_pfnErrorReport: landed spellings.
  - libvorbis oggpack_* / vorbis_*_clear / vorbis_staticbook_unpack and the `_floor_P` / `_residue_P` / `_mapping_P`
    tables.
  - _IID_IMediaSample2 0x012A9A64.
  - vector<unsigned long>(size_type) 0x00A96100.
  - CRT: malloc, free, memmove; dllimport calloc.
- Header debt:
  - engine/CParticleProgram.h, CMovableResource.h and CMemoryDataOutputStream.h are flattened (`__vftable` and byte
    members), so 0x00AE3670 uses local models with `// Layout:` lines.
  - There are no engine headers for CParticleSimulation, COccluderFlareEntry, C3DVector, CArray, CComponentInstance,
    C16BitSamplePtr, the libpk reverb classes (CReverbDiffuser, CDspGain, CDspOps, CRingBuffer, SDiffuserParams), the
    strmbase classes or the libvorbis structs.
