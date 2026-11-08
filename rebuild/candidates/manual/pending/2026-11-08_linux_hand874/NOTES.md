# Chunk 874 -- libpk / CODA audio rows (agent874)

3 matched (RELOCATION_MATCH, /GS), 3 parked with a named quantity, 1 not authored (analysis only).

| row | name | result | grades | device |
|---|---|---|---|---|
| 0x00C78A10 | ResampleMonoMMX | RELOCATION_MATCH | 52 | plain C++ (ivec.h model; window quads unnamed) |
| 0x00C79440 | ResampleMonoRampMMX | RELOCATION_MATCH | 10 | plain C++ (`short` gains; align(16) gain/step via the ivec short ctor) |
| 0x00C7AB20 | ResampleStereoRampMMX | RELOCATION_MATCH | 39 | Inline-budget, plain C++ (raw `_m_punpcklwd/_m_punpckhwd` widen) |
| 0x00C79FD0 | ResampleStereoMMX | parked | 54 | MMX colouring of s3 (copy-forwarded) -- same length, same slots |
| 0x00C5B7C0 | CSoftwareVoice::RenderSound | parked | 32 (+~60 ex.sh spellings) | c2 test-narrowing peephole: retail `test dword [eax+18h],0Fh` |
| 0x00C51F50 | CEAXManager::SetVoiceSourceProperty | parked | 30 | `data` param priority -179 split per case vs retail edi piece |
| 0x00C4F600 | CEAXManager::SetEAXProperty | not authored | 0 | analysis in differ/00c4f600.cpp |

## Extents and extra starts
- 0x00C4F600: true extent 9204 bytes (0x00C4F600..0x00C519F4): `ret 14h` at 0x00C5192A, 23-entry table at 0x00C51930,
  26-entry table at 0x00C5198C. **Extra start 0x00C51A00** (25 bytes, thiscall `ret 14h`: forwards (id, data, size) to
  CEAXManager::GetListenerProperty 0x00C4E730 and returns 0 -- the Get counterpart of SetEAXProperty). Catalogued 9248 = gap to 0x00C51A20.
- 0x00C51F50: `ret 14h` at 0x00C527CA + 28-entry jump table to 0x00C52840 (2288).
- 0x00C5B7C0: ends 0x00C5BCF2 (1344 incl. pad); 0x00C5AC80 is the out-of-line NeedsConversion helper with the same dword test.
- 0x00C78A10: 2596 bytes incl. 4-entry table at 0x00C79424. 0x00C79440: 2948 incl. table at 0x00C79FB4.
  0x00C79FD0: 2884 incl. table at 0x00C7AB04. 0x00C7AB20: 3236 incl. table at 0x00C7B7B4. No extra starts inside them.

## The MMX resamplers (0x00C78A10 / 0x00C79440 / 0x00C79FD0 / 0x00C7AB20)
- Not inline asm (the landed caller comments say "inline-asm bodies" -- wrong): Intel ivec.h class code. The model is the one
  the landed out-of-line copies 0x00C78590..0x00C78970 restate (M64 / I32vec2 / Is32vec2 / I16vec4 / Is16vec4, free
  unpack/pack_sat/mul_high/sat_add/mul_add/operator+), plus the Is16vec4(short i3, i2, i1, i0) lane constructor.
- Window quads must NOT be named: `mul_add(in[k], c[n])` with `in` a `const Is16vec4*` lets VC7.1's CSE temporaries take
  retail's spill slots; named locals (any decl order / init form, 36-point sweep) pack the slots differently.
- Gains: `short startGain, short gainStep` (long gives a different lea operand order); gain/step/pan are
  `__declspec(align(16)) Is16vec4` built by the short constructor (set_pi16 / set1_pi16 give a different shape).
- Stereo ramp: the ivec spelling of the zero-extend widen `unpack_low(Is16vec4(_mm_setzero_si64()), r)` exhausts the inline
  budget in case 3 (Is32vec2::operator+= called out of line twice, 3280 vs 3236). Raw `_m_punpcklwd/_m_punpckhwd`, or a raw
  `_m_psradi` pmaddwd input, or raw `_m_pmulhw/_m_packssdw` each restore the exact bytes; sat_add raw alone does not.
- Stereo (parked): identical except an MMX register swap of s3 vs the pan/s1 reloads in all four cases; immune to 47
  spellings and to /d2inlT/S flags (T40 makes it call out of line). See differ/00c79fd0.cpp.

## Tests
test/<addr>.cpp for the three matched rows: a copy of the source (long -> Mix32 typedef so LP64 g++ runs it, ALIGN16 macro)
against a scalar reference FIR over all four increments, three gain/pan settings and count 0; PASS under g++ and VC7.1 (tall.py).
Mutation-checked (dropping saturation / shifting one tap makes them FAIL).
