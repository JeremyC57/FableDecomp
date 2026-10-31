# Hand chunk 603 (CBasePin .. CBaseRenderer): 8 of 8 byte-exact

All eight rows are RELOCATION_MATCH at the retail length with behaviour tests PASS under g++ and VC7.1. Seven are in src/;
CLimiter::Process 0x00C593C0 is in retail_asm/ (purity.classify: retail_asm). Every relocation was checked against the
retail operand at the same offset (COFF parse of the grader object): each one resolves to the intended retail address.

## What matched (8)
- **CBasePin::ReceiveConnection 0x00CA8120** (strmbase amfilter.cpp, IPin slot 4): E_POINTER for either null argument;
  under the pin lock refuse VFW_E_ALREADY_CONNECTED / VFW_E_NOT_STOPPED; CheckConnect, then CheckMediaType with the
  S_FALSE / E_FAIL / E_INVALIDARG -> VFW_E_TYPE_NOT_ACCEPTED mapping; AddRef the connector, SetMediaType, CompleteConnect,
  undo on failure. Shape: the SDK text with release-build macros (EXECUTE_ASSERT keeps only BreakConnect()), in the landed
  CBasePin::Connect 0x00CAB470 model. First grade.
- **CBaseRenderer::PrepareReceive 0x00CA6C40** (strmbase renbase.cpp, renderer primary slot 38, called first by the landed
  Receive 0x00CA6E10): the SDK body verbatim, in the landed CBaseInputPin::Receive 0x00CA8B90 model plus CAMEvent,
  CRendererPosPassThru and the CBaseRenderer members +0x50..+0xB8. The pin call is the qualified
  `m_pInputPin->CBaseInputPin::Receive(...)` (direct call 0x00CA8B90). First grade.
- **CDecodedStream::Initialise 0x00C54510** (libpk, bound by the landed caller Open 0x00C547A0): initialise the voice effect
  for a 44100 Hz, 4-unit format; copy the 0x6C-byte default source properties (rep movsd 0x1B); reset both pans and the room /
  room LF / room HF words with their dirty bits; run the sixteen clamped setters; Configure(&block, 1). Shape (the lever
  that matched after about 8 grades): the five inline setters are reached through an inline accessor
  `SVoiceEffectParameters* GetPendingParameters() { return &m_pending; }`. Each call yields a fresh pointer temp, so VC7.1
  reloads the dirty word after every setter as retail does (`or [esi],800h` / `or [esi],4000h` after the pan copies, then
  load / or / store per level word). Through one reference `p` the five updates fold into one register RMW
  (`or eax,40E0h`), giving DIFFER(439v456).
- **CCodaVoice::UpdateEnvelopeStages 0x00C59E20** (GameCODA, bound by the landed caller UpdateEnvelope 0x00C5A480): a
  switch on flags & 0x5E where delay falls into attack and hold falls into decay. The 405 bytes include the 25-byte case
  tables (dword table 0x00C59F8C, byte table 0x00C59FA4); the six targets were checked by hand. First grade.
- **CReverbStage94::Process 0x00C75CE0** (libpk, bound by the landed caller ProcessStage94 0x00C6A180): the SIMD path reads
  each tap's ring, runs one four-wide gain filter (s_pOps->FilterGain4), then MixHistory per tap; the x87 path calls
  CReverbFilterTap::Process; then Feedback x4 with g_reverbMixMatrix. Shape: in the SIMD loop store `states[i]` before
  `params[i]`. That statement order fixed both the store order and the loop alignment.
- **libvorbis ov_crosslap 0x00C8C3E0** (`@ov_crosslap@8`): vorbisfile.c (CVS 20030909) verbatim, including the two
  `_analysis_output_always("pcmL"/"pcmR", ...)` dumps of that snapshot. It is added in upstream order to the landed
  _ov_64_seek_lap 0x00C8C580 TU, where the same-TU noinline statics give retail's private register conventions. First grade.
- **CParticleProgram::ReadGroups 0x00AE3A60** (Ego_r PDB, private static __fastcall): read the count; groups.resize; per
  group SetBoneIndex(ReadSLONG), Resize(blend count), then a raw Read of count*8 bytes into GetVertexBlends(). Shape: the inline
  CDataInputStream Read helper of the landed ReadVectorArray 0x00AE1730; a `ReadType<T>()` that returns by value keeps
  the count in a register.
- **CLimiter::Process 0x00C593C0** (retail_asm/, GameCODA, bound by the landed caller WriteChannel 0x00C591C0): a
  three-state 128-sample limiter. The Retail-asm line cites 0x00C59526 `fstp / fld / fistp dword` (libpk's FloatToLong,
  the same block as the landed retail_asm CVoiceEffect::Configure 0x00C6AEF0). Shape: `float gain = 32767.0f / m_envelope;`
  as a named local gives retail's `fld c ; fdiv ; fimul` order. m_releaseCount is unsigned (`jbe`).

## Names and evidence
Proven from two sources:
- **CBasePin::ReceiveConnection**: IPin slot 4 of both pin IPin tables (0x012BF3D0 in 0x012BF3C0, 0x012BF770 in
  0x012BF760), between the landed Connect (slot 3) and Disconnect (slot 5); plus the amfilter.cpp body (every HRESULT and
  the virtual order).
- **CBaseRenderer::PrepareReceive**: renderer primary slot 38, which the landed Receive 0x00CA6E10 calls first; plus the
  renbase.cpp body (E_FAIL / E_UNEXPECTED / VFW_E_SAMPLE_REJECTED, both locks, SampleProps fields).
- **CParticleProgram::ReadGroups**: the Ego_r PDB symbol; its two callers in ReadFromFile 0x00AE3C30; the mirror of the
  landed WriteGroups.
- **ov_crosslap**: the upstream body and strings; upstream file order between _ov_getlap 0x00C8C240 and
  _ov_64_seek_lap 0x00C8C580.

Bound by a landed caller (exact spelling reused) and descriptive (libpk / GameCODA have no PDB or RTTI):
- CDecodedStream::Initialise
- CCodaVoice::UpdateEnvelopeStages
- CReverbStage94::Process
- CLimiter::Process

New descriptive names introduced (no landed spelling exists):
- CVoiceEffect::Initialise 0x00C6B050: thiscall on +0x2F8 that takes the format object.
- CDspOps::FilterGain4 0x00C88E60: the SIMD table's four-wide gain filter.
- CDecodedStream::GetPendingParameters: an inline accessor that emits no symbol.
- Globals g_zeroVector_012b89e8 and g_defaultSourceProperties_012b89f8.
- SSourceProperties.

Callees reused with landed spellings that NAME_FIXES.tsv proves wrong: 0x00C89B40, 0x00AC41D0, 0x00AC4080, 0x00CADC10.
This TU uses the corrected names; the landed definers need respelling. Also fixed: 0x00C763E0 and 0x0143CCDC, both bound
under wrong spellings from 0x00C89B40.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
These manifest labels are propagated and wrong:
- 0x00CA8120 "sub_00ca8120": CBasePin::ReceiveConnection.
- 0x00C59E20 "CAudioSource_UpdateTimeOffset": CCodaVoice::UpdateEnvelopeStages.
- 0x00C8C3E0 "WorldMap_CompareAndBlend": ov_crosslap.
- 0x00C593C0 "AudioStream_ProcessAudioSamples": CLimiter::Process.
- 0x00C75CE0 "Audio_ProcessTriggerActivations": CReverbStage94::Process.
- 0x00AE3A60 "ReadGroups" is correct.
- 0x00C54510 "Audio_InitializeParameters": CDecodedStream::Initialise.
- 0x00CA6C40 "CEngine_InitializeRender_WithSurface": CBaseRenderer::PrepareReceive.

## Header debt
- `engine/CParticleProgram.h` is data-only: no member functions, and its CArray members are `unsigned char[0xc]`. ReadGroups
  is a private static that never touches `this`, so the TU declares a local `class CParticleProgram` holding only the
  static, as the landed WriteGroups 0x00AE0680 does. C3DGroup2 / C3DVertexBlend2 / CArray<T> have no engine headers; they
  are local and STLport-sized (C3DGroup2 16 bytes).
- The strmbase classes (CBasePin, CBaseRenderer, ...) and the libpk / GameCODA classes have no engine headers. They use the
  landed local models.
