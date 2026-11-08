# Landing 2026-11-08x: chunk 872 (libpk / CODA audio, Opus high), 12 of 14

All RELOCATION_MATCH + PASS (g++ and VC7.1 tests). No header changes, no inline-budget devices. Hand-back:
`pending/2026-11-08_linux_hand872/`.

| Row | Notes |
|---|---|
| 0x00C3CBD0 CodaGetErrorString | plain switch |
| 0x00C45990 CAmbientCube::Dump | /EHsc; early-exit apex loop; shared engine/CString.h |
| 0x00C4A3B0 PKCBufferedReader::Refill | /EHsc; register swap and rotation fixed with c2prio |
| 0x00C4E730 / 0x00C4EED0 CEAXManager::GetListenerProperty / GetVoiceBufferProperty | |
| 0x00C4EA30 CEAXManager::ApplyVoiceBufferProperties | two clamp macros, `long` return |
| 0x00C51A20 CEAXManager::QueueCommit | extent 1248 -> 1164 (0x00C51EB0 separate) |
| 0x00C53340 CDirectSoundDevice::DetectSpeakerConfig | extent 256 -> 180 (0x00C53400..0x00C53430 separate) |
| 0x00C56A40 CAudioContext::Open | /GS |
| 0x00C57800 CVtable_012b8ed4::CreateAndPlay | |
| 0x00C5C360 CSoftwareStreamVoice::MixRingBlock | /GS |
| 0x00C880A0 CAudioSource::ApplyHardware | retail_asm through libpk/FtoL.h; the coordinator added the missing `// Retail-asm:` line (0x00C88103 fld / 0x00C88107 fistp) at landing (`payload_second.json`) |

- Skipped for budget: 0x00C51F50 (EAX 4.0 source-property setter, `ret 14h` at 0x00C527CA + 28-entry jump table; retail's send loops
  store every element into one member) and 0x00C5B7C0 (software-voice render step, __int64 maths); decoded structure in NOTES.md.
- Five manifest boundary rows added; respellings queued (land0811x), e.g. CMixBuffer::MixQuad16 0x00C58740, the 3D / 2D audio
  object setters 0x00C415A0 / 0x00C415C0 / 0x00C41E40 / 0x00C44860 / 0x00C44880, CAudioObject slot 1 = Is3D.
