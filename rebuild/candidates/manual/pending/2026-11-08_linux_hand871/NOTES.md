# Chunk 871 notes

All 18 rows matched: 4 MATCH and 14 RELOCATION_MATCH. Every row has a behaviour test that passes under g++ and in the VC7.1 lane (`tall.py`). No row is parked.

## Rows: name and device

| Address | Name (leaf) | Grade | Device / shape |
|---|---|---|---|
| 0x008383D0 | CThingTrackNode::Create | RELOC | shared-header chain + `FABLE_RETAIL_CTHINGTRACKNODE_CTOR` opt-in, named physics local |
| 0x00CDBC50 | NScript::CQ_AmbushTradersScript::TeleportData::~TeleportData | RELOC | PhaseTwo include chain, empty user dtor |
| 0x00C3E890 | CAudioDevice_00c3e760::RenderFrame | RELOC | switch cases written 6,4,2,1 (retail block order); jump table checked by hand |
| 0x00C45F30 | CAmbientCube::CAmbientCube(float) | RELOC | apex ctor zeroes both vectors; level flag set in the body; free `operator*` called by name |
| 0x00C4B640 | PKCBufferedReader::CopyOutOfBuffers | RELOC | statement order from a 360-order permutation sweep (size before buffer); CString default-ctor opt-in |
| 0x00C4D9B0 | CAudioBuffer::Destroy | RELOC | sibling CAudioBuffer::Create model |
| 0x00C4E3C0 | CEAXManager::ApplyListenerProperties | RELOC | statement clamp macro (a store per branch); `(long)` on the in-range air absorption only; result word starts at 0 |
| 0x00C52E40 | DetectCpu | RELOC | /GS; retail_asm through libpk/IsCpuidSupported.h + NEW libpk/CpuIdentify.h; strcmp intrinsic; vendor and descriptors in one local struct; two-return inline helper for the last vendor test |
| 0x00C547D0 | CStereoWidener::Process | MATCH | 128-sample loop, members re-read |
| 0x00C55270 | CAudioSource::CreateProcessor | RELOC | two switches, in-class empty ctors for the 0x214 classes |
| 0x00C58880 | CMixBuffer::MixQuad16Ramped | MATCH | CMixBuffer family model |
| 0x00C58AB0 | CMixBuffer::MixSurround16 | MATCH | CMixBuffer family model |
| 0x00C58CE0 | CMixBuffer::MixSurround16Ramped | MATCH | quad rear at full gain, half step |
| 0x00C5B480 | CSoftwareVoice::ProcessSampleBlock | RELOC | two-word resampler sub-object with an inline SourceFrames; `limit` test repeated (no `most` local); queued count read before the carry memcpy |
| 0x00C5DD70 | CDSoundAccelerationQuery::GetAccelerationLevel | RELOC | early `return 0` exits; graded against the true 604-byte extent |
| 0x00C6CF40 | CVtable_012bed7c::RebuildGain | RELOC | twin of 0x00C6DEA0: inline SetTapGain front pair, out-of-line rear pair |
| 0x00C76E90 | CReverbPreDelay::Process | RELOC | retail_asm through libpk/FtoL.h; ternary round-half-down helper |
| 0x00C770B0 | CReverbPreDelay::Blend | RELOC | retail_asm through libpk/FtoL.h; `position >= offset ? ... : ... + size`; graded against the true 857-byte extent |

## Oracle rows cut wrong (coordinator: land against the true extents)
- **0x00C5DD70** is 728 bytes in the oracle, but the function ends at 604 bytes.
  - The row runs 124 bytes into the next function, 0x00C5DFD0..0x00C5E047.
  - Retail `ret 4` is at 0x00C5DFC9, followed by padding.
  - The grader uses CUT 604.
- **0x00C770B0** is 910 bytes in the oracle, but the function ends at 857 bytes.
  - The row includes two small functions with no manifest entry, 0x00C77410 and 0x00C77430.
  - The grader uses CUT 857.
- finish.py now applies both cuts to `oracle.tsv`.

## Retail-asm rows
- These rows grade `retail_asm` (the row sources contain no asm):
  - 0x00C76E90 and 0x00C770B0, through `libpk/FtoL.h`;
  - 0x00C52E40, through `libpk/IsCpuidSupported.h` and the new `libpk/CpuIdentify.h`.
- `libpk/CpuIdentify.h` and its dual-path test must land together with 0x00C52E40.

## Header debt seen, not fixed
- **CAudioContext / CSalOpenAL** are both modelled for the same OpenAL context in different landed rows.
- **CMixBuffer** is the same class as the landed `CUnprovenAudioBuffer_00C591F0`.
- **CRegistryKey** is a `struct` in the landed Create 0x00C737D0 and a `class` in the other landed CRegistryKey rows. There is no shared header. The difference changes a mangled name only where the type appears as a parameter (U vs V).
- **0x0143BC2C** (libpk master / trace sink) has several spellings across landed rows.
- **CCpuInfo::Family** has different signedness across rows:
  - DetectCpu needs `unsigned long`, because the `Family > 5` test is `ja`;
  - the landed GetIntelModelName compares it with signed tests.
  - A shared `libpk/CCpuInfo.h` would settle it.
- **C3DVector operators:** libpk calls the free `__fastcall operator*(const C3DVector&, float)` at 0x00C258A0. The shared `engine/C3DVector.h` also declares a member `operator*(float)`, so the infix form is ambiguous. The row calls `operator*(v, s)` by name.

## Names
- Names are in LABELS.tsv, each with two sources.
- Eight landed spellings are proven wrong (NAME_FIXES.tsv). The coordinator should respell their callers:
  - 0x00C4D9B0
  - 0x00C55270
  - 0x00C547D0
  - 0x00C4A3B0
  - 0x00C73820
  - 0x00C76E90
  - 0x00C770B0
  - 0x00C45F30
- These catalog names are propagated descriptions and were replaced:
  - FFT_ComputeRealTransform / FFT_ComputeComplexTransform
  - Audio_ProcessSampleBlock (its method name was kept)
  - CShaderRenderManager_SetupProjection
  - CCamera_InitializeOcclusionGrid
  - InitCPUCapabilities
  - Audio_InitializeDriver
  - Audio_MixStereoSamples
  - CTCHeroStats_CreateSpellEffect
  - AudioFormat_*
