# Landing 2026-11-08u: chunk 871 (libpk + two game rows, Opus high), 18 of 18

4 MATCH + 14 RELOCATION_MATCH, all PASS. Hand-back: `pending/2026-11-08_linux_hand871/`.

| Row | Device |
|---|---|
| 0x008383D0 CThingTrackNode::Create | shared headers + new opt-in `FABLE_RETAIL_CTHINGTRACKNODE_CTOR` (ctor 0x00837C70 declaration) |
| 0x00CDBC50 NScript::CQ_AmbushTradersScript::TeleportData::~TeleportData | PhaseTwo include chain, empty user dtor |
| 0x00C3E890 CAudioDevice_00c3e760::RenderFrame | switch cases in retail block order |
| 0x00C45F30 CAmbientCube::CAmbientCube(float) | free `operator*` called by name (C3DVector member operator makes infix ambiguous) |
| 0x00C4B640 PKCBufferedReader::CopyOutOfBuffers | statement order from a permutation sweep; new opt-in `FABLE_RETAIL_CSTRING_DEFAULT_CTOR` |
| 0x00C4D9B0 CAudioBuffer::Destroy | modelled on CAudioBuffer::Create |
| 0x00C4E3C0 CEAXManager::ApplyListenerProperties | statement clamp macro (store per branch) |
| 0x00C52E40 DetectCpu | `/GS`; retail_asm through `libpk/IsCpuidSupported.h` and the new `libpk/CpuIdentify.h` (eight CPUID blocks; VC7.1 has no `__cpuid` intrinsic; FABLE_PORTABLE twin; dual-path test `rebuild/tests/portable/test_CpuIdentify.cpp`, g++ portable PASS) |
| 0x00C547D0 CStereoWidener::Process, 0x00C58880 / 0x00C58AB0 / 0x00C58CE0 CMixBuffer mixers | plain C++ (MATCH) |
| 0x00C55270 CAudioSource::CreateProcessor | two switches |
| 0x00C5B480 CSoftwareVoice::ProcessSampleBlock | resampler sub-object with an inline accessor (keeps retail's reloads) |
| 0x00C5DD70 CDSoundAccelerationQuery::GetAccelerationLevel | early `return 0` exits; extent 728 -> 604 |
| 0x00C6CF40 CVtable_012bed7c::RebuildGain | twin of 0x00C6DEA0 |
| 0x00C76E90 / 0x00C770B0 CReverbPreDelay::Process / Blend | retail_asm through `libpk/FtoL.h`; Blend extent 910 -> 857 |

- Coordinator fix at landing: `libpk/CpuIdentify.h`'s `// Retail-asm-helper:` names and its `// Retail-asm:` evidence were spread over
  continuation lines; purity reads only the tagged line, so DetectCpu graded NOT_GENUINE. Joined onto the tagged lines (comment only).
- Extents: six manifest boundary rows added (0x00C5DFD0 / 0x00C5DFE0 / 0x00C5DFF0 / 0x00C5E000, 0x00C77410 / 0x00C77430); BOUNDS_FIX rows written.
- Names: labels in `ghidra_out/labels_landing_2026-11-08u.tsv`; eight respellings queued (land0811u).
- Header debt noted by the agent (not fixed): CAudioContext / CSalOpenAL model one OpenAL context; CMixBuffer = landed
  CUnprovenAudioBuffer_00C591F0; CRegistryKey struct vs class; 0x0143BC2C spellings; CCpuInfo::Family signedness.
