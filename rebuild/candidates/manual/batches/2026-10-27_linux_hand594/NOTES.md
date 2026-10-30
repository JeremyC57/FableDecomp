# Hand chunk 594 (CBaseRenderer .. CAudioPlatform): 9 of 10 byte-exact

All 9 matched rows grade RELOCATION_MATCH (0 plain MATCH, 0 retail_asm). Tests PASS under g++ (tall.sh) and VC7.1 (tall.py).
The read-only vland run reported 9 WINS with no RELOC_IDENTITY and no real DIFFER.

## What matched (9)
- **strmbase CBaseRenderer::Pause 0x00CA67B0 / ::Run 0x00CA68F0** (DX9 BaseClasses renbase.cpp, /EHsc). These are the verbatim
  renbase.cpp bodies. `this` is the IBaseFilter sub-object (+0x0C) and the full renbase.h virtual order is modelled (slots 9
  GetMediaPositionInterface .. 32 StopStreaming). Inline SetRepaintStatus, ResetEndOfStreamTimer and Ready; CAMEvent::Set;
  CRendererInputPin with m_pAllocator at +0x9C. The model base is lines 20-304 of the landed
  CBaseRenderer_GetMediaPositionInterface_00ca6450.cpp, with NotifyEvent added to CBaseFilter (bin/mk_ren.py + bin/ren_model.txt).
  Each row matched on the first grade.
- **strmbase CEnumMediaTypes::Next 0x00CAB160** (amfilter.cpp, /EHsc /GS). This is the verbatim body on the landed
  CEnumMediaTypes_Skip_00cab2b0.cpp model, with CoTaskMemAlloc as a dllimport. The vtable cell at 0x012BF6C8 proves the slot.
  Matched on 1 grade.
- **std::__unguarded_partition 0x00CBB710** over `pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>` with
  CKeyPairCompareLess (the bound spelling; the caller is introsort 0x00CBD700). The model comes from the landed std_swap_00cba7e0.cpp.
  The shape needed an inline `__fastcall swap`, an inline iter_swap and an explicit instantiation. Matched on 1 grade.
- **ComputeAzimuthElevation 0x00C599C0** (GameCODA, descriptive; the caller is 0x00C54C10). It computes atan2 and sqrt with x87
  (fpatan, fsqrt) and branches on `g_pCodaSystem->GetOutputMode() == 2`. Shape: `if (z > x)` emits `fcom st(1); test ah,41h`.
  The form `x < z` gave a different compare, so this row took 2 grades.
- **CSoundEvent::Update 0x00C21A50** (libpk, descriptive; the caller is 0x00C128C0). CheckState, GetActivePlayer and
  IsPlayingVirtual are same-TU `__declspec(noinline)` stand-ins under `auto_inline(off)`, which lets VC keep ecx/edx across
  the calls. CSoundPlayer slots are modelled. CSoundEvent slot 0 is `GetListNode` (0x00C21D50 = `lea eax,[ecx+0Ch]`), not a
  destructor. Matched on 1 grade, plus 1 regrade after a name-only fix.
- **libvorbis vorbisfile.c _ov_64_seek_lap 0x00C8C580** (`static`, vf in eax). The callers are ov_raw_seek_lap,
  ov_pcm_seek_lap and ov_pcm_seek_page_lap at 0x00C8C6D0, 0x00C8C6F0 and 0x00C8C710. The first grade was 393v325 because
  _ov_initset and ov_info were inlined. Making ov_info, _ov_splice, _ov_initset and _ov_initprime noinline (with _ov_splice
  `static __stdcall`) fixed it. Matched on 2 grades.
- **libvorbis psy.c max_seeds 0x00C93080** (`static __stdcall`, p in edi; this reopens the chunk-447 row). This is a copy of
  the landed _vp_tonemask TU with seed_chase changed to `static void __stdcall`. With cdecl seed_chase the caller kept an extra
  `add esp,0Ch`. Matched on 2 grades.
- **CAudioPlatform::ReleaseVoice 0x00C4D070** (libpk, descriptive; it sits between the ctor 0x00C4D000 and Shutdown 0x00C4D1F0).
  Retail's two-pointer pool compare and frame layout come only from an inline `bool CNodePool::operator==(const CNodePool&)
  const`. SAudioConfig g_audioConfig 0x0139C988 is modelled with slots +0/+0x18/+0x30/+0x48 and counters +0x84..+0x90.
  Matched on 3 grades.

## Parked for Fable 5.1 (1)
- 00c6b260 ConvertFloat128BlockToInt DIFFER(315v315): scalar fallback `mov ecx,[esi+edx] ; push ecx` (retail) vs `mov eax,[esi+edx] ; push eax` (ours); both retail asm blocks, the frame and the flag tests match | tried: unsigned/int index, *dst++/*src++ count loop (316v315), FtoL body placed after the caller | next: a float local / register-pressure variant of the pushed argument, or put the 0xC6B250 HasSSE getter in the TU; grade retail_asm (cvtps2dq/cvtps2pi/ldmxcsr blocks, `// Retail-asm:` line already written) once it matches

## Skipped
None.

## Label corrections
All 10 propagated manifest labels were wrong:
| address | manifest label | correct |
|---|---|---|
| 0x00CA67B0 | CAudioBank_Stop | ?Pause@CBaseRenderer@@UAGJXZ (strmbase) |
| 0x00CA68F0 | CAudioBank_Start | ?Run@CBaseRenderer@@UAGJ_J@Z (strmbase) |
| 0x00C6B260 | FloatToLongConvert_Vectorized | ConvertFloat128BlockToInt (descriptive, libpk mixer) |
| 0x00CBB710 | SortedMerge_WithRefcount | std::__unguarded_partition<pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>*, ..., CKeyPairCompareLess> |
| 0x00C599C0 | Audio_ComputePitchFromVector | ComputeAzimuthElevation (descriptive, GameCODA) |
| 0x00CAB160 | CParticleMorphs_GetEntries_Batch | CEnumMediaTypes::Next (strmbase amfilter.cpp) |
| 0x00C21A50 | Display_UpdateWithFlags | CSoundEvent::Update (descriptive, libpk) |
| 0x00C8C580 | WorldMap_ProcessWithCallback | _ov_64_seek_lap (libvorbis vorbisfile.c) |
| 0x00C93080 | FFT_ComputeWaveformEnvelope | max_seeds (libvorbis psy.c) |
| 0x00C4D070 | CCombatSequenceManager_ProcessAction | CAudioPlatform::ReleaseVoice (descriptive, libpk) |

## Names and evidence
- Proven from public source plus callers/vtables: CBaseRenderer::Pause and ::Run (renbase.cpp bodies, CTextureRenderer
  vtable 0x0129D08C slots), CEnumMediaTypes::Next (amfilter.cpp, vtable cell 0x012BF6C8), std::__unguarded_partition
  (STLport _algo.c body, caller introsort 0x00CBD700, existing binding), max_seeds (psy.c, caller _vp_tonemask 0x00C93E30),
  _ov_64_seek_lap (vorbisfile.c, three wrapper callers), vorbis_window (block.c), CBaseFilter::Pause and NotifyEvent,
  CBaseRenderer::ResetEndOfStream and ResetEndOfStreamTimer, timeKillEvent IAT.
- Descriptive (libpk and GameCODA have no PDB): ComputeAzimuthElevation; ConvertFloat128BlockToInt; CSoundEvent::Update,
  UpdateTrackedParameters, Play, IsPlayingVirtual, CheckState, GetListNode; CAudioPlatform::ReleaseVoice; the CCodaVoice slot
  names Initialise/Stop/ClearField0C/SupportsEAX/Is3D/IsPinned; the CSoundPlayer slot names
  SetPitch/SetGain/ReadLevel/Release/Update/Finish/SetDistance/SetRecord; SAudioConfig g_audioConfig.
- NAME_FIXES.tsv has 15 rows with evidence (CBaseFilter::Pause, CBaseRenderer::ResetEndOfStream/ResetEndOfStreamTimer,
  __imp__timeKillEvent@4, @vorbis_window@8, CBaseFilter::NotifyEvent, the CSoundEvent methods, the CNodePool spellings).

## Header debt
- No engine headers exist for CBaseRenderer, CBaseFilter, CEnumMediaTypes, CSoundEvent, CSoundPlayer, CAudioPlatform,
  CCodaVoice, CNodePool, C3DVector or CCountedPointer. These rows use local models (strmbase/libpk/GameCODA, not in Ego_r.pdb).
- vland SHARED_HEADER_AVAILABLE for CBaseObject is a false positive: strmbase's CBaseObject (DirectShow) is not the engine
  CBaseObject.
- CActiveEntityScriptBase has an engine header but is used only as a forward-declared template argument (no member access).
- CSoundEvent slots 1..11 and CAudioPlatform slots 1..7 are not modelled. The landed models use v00 placeholders there.
