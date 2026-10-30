# Hand chunk 598 (libvorbis floor0 .. CAudioObject): 9 of 10 byte-exact

9 RELOCATION_MATCH: 7 genuine C++ (src/) and 2 retail_asm (retail_asm/). 0 MATCH. 1 parked (differ/). 0 skipped.
Read-only verifier (vland.py): payload.json WINS 7, retail_asm_payload.json WINS 2, no RELOC_IDENTITY, no DIFFER.
All 8 tests (src rows + the parked row) PASS under g++ and VC7.1. The 2 retail_asm tests PASS under VC7.1 only (they hold `fld/fistp`).

## What matched (9)
- **libvorbis floor0.c -- `floor0_map_lazy_init` 0x00CA36E0** (361, `@floor0_map_lazy_init@12`, /Gr). Name **proven**: the
  CVS-2003 floor0.c body verbatim against the binary (linearmap[W] null test, toBARK macro, floor()/malloc imports, the
  "Xiph.Org libVorbis I 20030909" vendor string in the same image). Shape: a same-TU `static __declspec(noinline)` with its
  real caller floor0_inverse2 (parked hand448 0x00CA39C0) in the TU gives retail's private convention: vb in eax, look in
  edi, info on the stack, `ret 4`. Land the leaf only.
- **strmbase -- `CMediaSample::SetProperties` 0x00CA9510** (362). Name **proven**: the only reference is CMediaSample vtable
  slot 20 at 0x012BF730, right after GetProperties 0x012BF72C (landed). The body is amfilter.cpp verbatim. Callees:
  CreateMediaType 0x00CB1F70 and DeleteMediaType 0x00CB1F20. The model is taken from landed CMediaSample_GetProperties_00ca9450.
  1 grade.
- **libpk ambient -- `CAmbientChannel::Update(CAmbientSource*)` 0x00C621B0** (363). Name **descriptive**: the 8-byte channel
  of ctor 0x00C62120; only caller functor 0x00C46F60; sibling of the landed CAmbientTrigger::Play 0x00C62530. Shape:
  - 0x00C62150 must be a thiscall member that ignores `this`: retail does `mov ecx,esi` before the call, which a
    `__stdcall` free function drops.
  - The position is copied member by member: retail loads `[eax+14h]`, where a struct assignment gives `add ecx,14h`.
  3 grades.
- **libpk reverb -- `CReverbProcessor::Allocate(const CReverbConfig*, float*, unsigned*)` 0x00C6A460** (364, retail_asm).
  Class **proven** (from the landed ctor 0x00C6A650 / dtor 0x00C6A780 member offsets), method name **descriptive**.
  `// Retail-asm:` evidence: 0x00C6A4CE fld / 0x00C6A4D2 bare `fistp dword`, libpk's FloatToLong in its round-half-down.
  The comb length is `(round(rate*0.2f)+128+3)&~3`. The second grade swapped two stores (m_lineIndex=0 comes before bytes=0).
- **libpk DirectSound -- `CDSoundStreamBuffer::CreateBuffer(channels, samples, hardware3D)` 0x00C57140** (366, /GS). Name
  **descriptive** (landed binding). The DX7 0x14-byte DSBUFFERDESC; a packed WAVEFORMATEX; CWaveFormatExtensible ctor
  0x00C56EC0. Shape: the Lock out-params (ptr/bytes x2) declared in their own block scope land in the dead argument slots
  retail uses. Function-scope locals take the channels/samples slots, and swapping declaration order does nothing. 3 grades.
- **strmbase -- `CBaseVideoRenderer::SendQuality` 0x00CA56E0** (366). Name **proven**: renbase.cpp verbatim (m_trFrameAvg
  +0x10C, the 2*m_trRenderAvg tests, IID_IQualityControl 0x012A99B4, the m_pQSink / pin-sink branch). The model is taken
  from landed JoinFilterGraph_00ca6110, with IQualityControl::Notify corrected to take `Quality` by value. 1 grade.
- **libpk mixer -- `CVtable_012bed90::RebuildGain(bool)` 0x00C6DEA0** (369, retail_asm). Name **descriptive** (landed
  binding; caller SetVolume 0x00C6E020). `// Retail-asm:` evidence: 0x00C6DED1/0x00C6DED5 plus three more fld/fistp-dword
  pairs. Shape:
  - an inline helper for the front pair's gain step;
  - out-of-line ComputeGainRamp 0x00C6DE60 calls for the rear pair when g_pAudioMaster->m_outputChannels >= 4;
  - the critical section through the IAT.
  1 grade.
- **libvorbis analysis.c -- `_analysis_output_always` 0x00C90D60** (370, /Gr /GS). Name **proven**: analysis.c verbatim
  ("%s_%d.m" 0x012BF2D0, sprintf/fopen/perror/fprintf/fclose imports, wrapper _analysis_output 0x00C90EE0). Shape: the
  same-TU static noinline todB 0x00C90D40 takes its pointer in eax. Land the leaf only.
- **libpk audio -- `CAudioObject::Open(unsigned long caps)` 0x00C42DD0** (373, /EHsc). Name **descriptive** (landed binding;
  direct caller C2DAudioObject::Open 0x00C449C0). Shape:
  - `unsigned long requested = m_caps;` read after the memset of the parameter block reorders the caps load and the 1.0f
    register;
  - an inline `GetContext()` accessor on g_pEAXManager fixes the argument evaluation order for
    `new CEaxSource(ctx, 0, 0, 1, &result)` (ctor 0x00C61610, CPKObject::operator new 0x00C3C360);
  - an inline Reset member was rejected because it routed the stores through eax.
  3 grades.

## Parked for Fable 5.1 (1)
- 00c6e050 CVtable_012bed90::SetParameters DIFFER(367v369): frame layout only. Retail reserves a 0x20 frame with six unshared float temporaries (c +4, 0.5-s +8, three gains +0xC..+0x14) and homes the c+0.5 CSE in the dead `pan` argument slot. VC7.1 packs ours into 0xC (slot reuse, the CSE in a local). Every other instruction is in retail order. | tried: named intermediates, the expression form (closest), function-scope gains, `pan *= k` in place, a hoisted sin, an inline Product helper, expressions inside the conditions, QFE-4035 (same) | next: find a declaration shape that forces eight unshared float slots (an inline helper with by-value float params per quadrant, or more function-scope locals whose addresses escape to a no-op inline)

## Skipped
- None.

## Label corrections
- 0x00CA36E0 Math_PrecomputeAngleLookupTable -> libvorbis floor0_map_lazy_init (proven).
- 0x00CA9510 sub_00ca9510 -> CMediaSample::SetProperties (proven).
- 0x00C621B0 CItem_ProcessPendingEffects -> CAmbientChannel::Update (descriptive; not CItem).
- 0x00C6A460 CQ_GuildTrainingSkillScript_Init -> CReverbProcessor::Allocate (not quest code).
- 0x00C57140 CSound_AllocateBuffers -> CDSoundStreamBuffer::CreateBuffer.
- 0x00CA56E0 Audio_ProcessTimeAdjustment -> CBaseVideoRenderer::SendQuality (proven).
- 0x00C6DEA0 Audio_SetOutputVolume -> CVtable_012bed90::RebuildGain.
- 0x00C90D60 Debug_DumpMatlabFile -> libvorbis _analysis_output_always (proven).
- 0x00C42DD0 CTCHeroStats_InitializeShadowRenderer -> CAudioObject::Open (not CTCHeroStats).
- Callee labels shown wrong by vland RELOC_NAME warnings (catalog only):
  - 0x00C62150 CTools::GetWorld (see NAME_FIXES.tsv)
  - 0x00C46F40 TurnToPointer (= CAmbientSource::GetVolume)
  - 0x00C20420 CBrainOutput::SetAsPumpCombatActions / 0x00C20440 std::bind2nd (sound play-param setters)
  - 0x00C13140 CTCHeroExperience_GetBlockParryReward (sound system Play)
  - 0x00C3DF10 CTCHeroStats::CreateSoundBankResizeAction (EAX manager creation)
  - 0x00CB1F20 FreeComRecord (= strmbase DeleteMediaType)
  - 0x00C755B0 / 0x00C76AB0 / 0x00C76DD0 / 0x00C75050 CQ_GuildTrainingSkillScript_* (reverb diffuser / delay / pre-delay /
    stereo delay Allocate).

## Name fixes (NAME_FIXES.tsv)
- 0x00C62150: thiscall `CAmbientChannel::GetHandleTable(CAmbientSource*)`, not the landed `__stdcall GetHandleTable_00c62150(long)`.
- 0x00C6A460: `CReverbProcessor::Allocate`, not the landed caller 0x00C6A8B0's placeholder `BufferState::Build`.

## Header debt
- strmbase (CMediaSample, CBaseVideoRenderer, CBasePin, IMediaSample2, IQualityControl) has no engine headers, so local
  models are copied from the landed rows. vland reports SHARED_HEADER_AVAILABLE CBaseObject for 0x00CA9510. That is a
  name collision: engine/CBaseObject.h is Fable's CBaseClass-derived CBaseObject (vtable 0x0129A7D8, sizeof 8), while this
  model is the DirectShow strmbase CBaseObject (m_pName, m_cObjects). The header must not be used here.
- libpk (CAudioObject, CDSoundStreamBuffer, CAmbientChannel/Source, CReverbProcessor and members, CVtable_012bed90,
  CAudioMaster, CEaxSource) has no engine headers, so local models are used with `CUnproven*` padding where fields are
  unread.
- libvorbis (vorbis_block, vorbis_info_floor0, vorbis_look_floor0) uses local models of the 1.0.1 headers.
