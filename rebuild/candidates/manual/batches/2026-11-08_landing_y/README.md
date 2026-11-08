# Landing 2026-11-08y: chunk 874 (libpk / CODA audio, Opus high), 3 of 7

All RELOCATION_MATCH + PASS (VC7.1 and g++), `/GS`. Hand-back: `pending/2026-11-08_linux_hand874/`.

| Row | Device |
|---|---|
| 0x00C78A10 ResampleMonoMMX | plain C++ on the VC7.1 ivec.h classes (`mul_add(in[k], c[n])`, unnamed window quads) |
| 0x00C79440 ResampleMonoRampMMX | plain C++; `short` gains, `__declspec(align(16)) Is16vec4` gain / step |
| 0x00C7AB20 ResampleStereoRampMMX | inline-budget residue solved in C++: the widen written with the `_m_punpcklwd` / `_m_punpckhwd` intrinsics (ASM_RULES rule 1, genuine) instead of an ivec expression VC7.1 called out of line |

- Parked: 0x00C79FD0 ResampleStereoMMX (one MMX register swap, 54 grades), 0x00C5B7C0 CSoftwareVoice::RenderSound (retail
  `test dword ptr [eax+18h],0Fh`; VC7.1 always narrows it to `test byte ptr` -- unreachable from C++), 0x00C51F50
  CEAXManager::SetVoiceSourceProperty (retail keeps `data` in edi across the jump-table dispatch; c2 priority -179). Not authored:
  0x00C4F600 CEAXManager::SetEAXProperty (true extent 9204; decoded in differ/).
- Extents: each resampler includes its 4-entry switch table; 0x00C51A00 (Get counterpart of SetEAXProperty) added as a boundary row.
- Names: labels `ghidra_out/labels_landing_2026-11-08y.tsv`; respellings queued (land0811y), including the ramp-resampler
  declarations in the landed MixVoiceFadeIn / MixVoiceFadeOut (short gains, `...FF@Z`).
