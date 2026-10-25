# Hand chunk 441 (COpenALVoice .. CThread): 33 of 40 byte-exact

Region 0x00C57C90..0x00C5E180 is the GameCODA (Sensaura "CODA") audio library, statically linked next to libpk
(strings "C_ERR_CODACREATED", "/OpenAL32.dll", "/wrap_oal.dll"). It holds the OpenAL voice, the software mixer's
block accumulator and output limiter, the voice base (envelope / vibrato / ramps), the software and stream voices,
the planar ring buffer, the SSE2 kernels, the platform probe and the thread wrapper. No PDB or RTTI covers it, so
every class and member name is descriptive. All 33 src/ rows re-grade MATCH (11) or RELOCATION_MATCH (22) through
gall.sh. All 33 tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1/Wine (tall.py). Every leaf was checked
with `objdump -t`, and every call target against retail's import or relocation (tools/relid.py).

The voice rows share one local model (gen/voice_model.inc.txt, written into each source by gen/gen_voice.py):
- CCodaVoice: vtable 0x012B5C10, 19 slots, ctor 0x00C59BA0, 0xB4 bytes.
- CSoftwareVoice: vtable 0x012B5C60, the ring +0xB4, two CVoiceFilter +0xDC, step/phase +0x1E8/+0x1EC.
- CSoftwareStreamVoice: vtable 0x012B5D70, slot 22 overridden.
- CAudioRingBuffer: vtable 0x012B8FC4.

No engine header exists for any of these classes.

## What matched (33)
- **COpenALVoice**
  - 00c57c90 `UnqueueBuffer()`
    - What it does: clears alGetError, then `alSourceUnqueueBuffers(source, 1, &buffers[readIndex])` through the
      global OpenAL wrapper 0x0143BDB0. It then advances the buffer ring's read index and drops the queued count.
    - Shape lesson: both lazy entry-point resolvers are inline accessors on the global instance. The call arguments
      are evaluated before the inline body, so they sit in ebx/edi across the resolve, and GetProcAddress is hoisted
      into ebp once for both resolvers.
- **CMixBuffer** (the software mixer's block accumulator; vtable 0x012B8F38, six planar 128-long accumulators)
  - 00c591f0 constructor.
    - Shape: `(m_storage + 15) & ~15`, then a counted `for (i < 6) m_pChannel[i] = p + 128 * i` loop. VC7.1 unrolls
      and schedules it (+8/+0xC/+0x10 before +4). Six explicit assignments keep source order and differ.
  - 00c58210 `MixMono16Ramped`: one mono 16-bit block into every channel under a ramped Q15 gain.
    - Shape: `unsigned short` gains push `xor ecx,ecx ; mov cx,[edi]`. `short` steps push `movsx`.
    - The step array is addressed as `steps - gains`, homed in the dead argument slot.
  - 00c58270 `MixStereoFloat` and 00c582f0 `MixStereo32`: a stereo block pair into channels 0/1 (and 2/3), or into
    channel 0 at half gain for a mono mix.
    - Shape: the mono arm returns early. `src + 128` stays in ebx on the stereo arm.
    - Overloaded virtuals are laid out in reverse by VC7.1, so the kernels need distinct names.
  - 00c58370 `MixStereo16`.
    - Shape: `gains[0]` and the vtable load come before the channel-count test.
    - The mono halving `gains[i] >> 1` into an `unsigned short` parameter is the 16-bit `shr cx,1`.
- **CLimiter** (the output limiter, vtable 0x012B8F64, 0x17C bytes; six of them sit in the output device 0x00C3EFA0)
  - 00c59570 constructor.
    - Shape: the vtable store comes first, then the members are assigned in the body. `this` moves to edx to free
      ecx for `rep stosd`.
  - 00c592f0 `UpdateEnvelope`: a peak-hold envelope follower 44 samples back in the delay line.
    - The negative magnitude is `-1 - s`, converted as UNSIGNED.
    - `level > m_peak || m_holdCount == 44` keeps the hold increment on the fall-through.
    - `m_envelope *= k` reuses the `fst` value for the compare.
- **Free functions**
  - 00c595e0 `exp(float)`: the VC7.1 <math.h> float overload emitted out of line. Under /O2 it is the true
    intrinsic `fldl2e ; fmulp ; ... f2xm1 ; fscale` (precedent: landed `?ceil@@YAMM@Z` 0x00689370).
  - 00c59960 `ComputePanGains`: a linear pan law.
    - Shape: `pan = 0.0f` for mode 2 threads into the `pan <= 0` arm, and the `pan > 0` arm is written first.
  - 00c59d40 `LinearRampStep` and 00c59d80 `ExponentialDecayStep`.
    - Shape: `seconds = (float)SecondsToMixBlocks(seconds)` reuses the dead parameter slot (`mov [esp+10h],eax ;
      fild`). A named local takes a fresh slot.
    - `exp` and `log` are true intrinsics, and log(32768.0) is not folded.
  - 00c59de0 `OctaveRampStep`: 2^((to - from) / blocks).
    - Shape: each arm returns a NAMED float local in its own scope.
    - `return (float)pow(...)` is tail-jumped to _CIpow by VC7.1 (66v64). Retail always calls it.
  - 00c5c980 `SpeakerToChannelIndex`: SPEAKER_* bit to mixer slot (L, R, BL, BR, C, LFE).
    - Shape: the sparse switch lowers to a byte index table plus a jump table. Case bodies follow SOURCE order, so the
      cases are written in retail's body order.
    - The table targets were checked by hand.
  - 00c5da70 `IsWindows9x`.
    - Shape: four SEPARATE `bool` globals. As an array or a struct, VC7.1 merges the first two tests into one dword
      load.
- **CCodaVoice** (the voice base)
  - 00c59c20 `StartEnvelope`: `m_flags &= ~0x5E`, then either the delay counter and level 0, or level 1.
    - Release bit 0x20 survives the mask. The float delay is pushed through eax.
  - 00c59890 `UpdateVibrato`: a switch on `m_flags & 0x180`, where case 0 falls into the oscillating cases.
    - Shape: `state = 0x80` / `state ^= 0x180` must come BEFORE the counter store in both arms. That keeps the phase
      in esi across the `cdq`; the store-first order puts the phase in edx.
  - 00c5a480 `UpdateEnvelope`: in the release stage, multiply by the decay factor until the level drops under 1/32768,
    then enter the done stage. Every other stage goes to UpdateEnvelopeStages 0x00C59E20.
    - Shape: the release arm is the `if` body that falls through.
    - The level is zeroed before the flag store, and the product is written `delta * level`.
- **CSoftwareVoice**
  - 00c5a3c0 `SizeRing(bool streaming)`.
    - Shape: the read pointer comes from an inline ring accessor (`lea ecx,[esi+0B4h]` materialised early), and the
      difference is a signed `__int64` of the two pointers.
    - The three SetCapacity calls stay separate, with `push 1` hoisted.
  - 00c5af50 `ClearMix`: silences every channel of the global 6-channel mix ring 0x0143BF68, commits, and resets the
    phase.
    - Shape: the inline /Oi `memset` is recomputed per channel.
  - 00c5afc0 `ApplyFilter`: a two-case switch into one shared `setne dl` bool. The pending test reuses eax, and the
    ramped kernel 0x00C6BAE0 is the fall-through call.
  - 00c5b030 `Resample`: a linear-interpolating 20.12 resampler.
    - Shape: the output pointer is `(short*)(pData + writePos*frameBytes + capacity*channel*2)`, homed in the dead
      `out` slot.
    - `frac` is computed before `index`, and the product is `src[i]*(0x1000-frac) + src[i+1]*frac`.
  - 00c5bd00 `SetParameters` (slot 22).
    - Shape: the buffer's format flags are read into a local FIRST.
    - The pitch-envelope start and the second pitch-LFO start are inline helpers.
- **CSoftwareStreamVoice**
  - 00c5c2d0 `SetParameters` override.
    - Shape: `&(IsStream() ? 0 : sample)->m_info` gives `neg al ; sbb ; not ; and ; add 0FCh`.
    - The buffer flag must be an `unsigned char` member to give `test byte ptr`. A dword `& 0x10000` stays a dword
      test.
  - 00c5c800 `MixMono` and 00c5c880 `Mix`: hand a block to the software mixer, ramping to a pending gain over 128
    samples.
    - Shape: the signed `/ 128` gives `cdq ; and edx,7Fh ; add ; sar 7`.
    - The gain is an `unsigned short` parameter pushed from `cx`.
- **CAudioRingBuffer** (planar ring cursor; Init 0x00C5CE00 and ctor 0x00C5D0A0 already landed)
  - 00c5ce60 `SetCapacity`: the quotient is kept in esi for the second `div`.
  - 00c5cf00 `CommitWrite`: a memory-destination `add`.
  - 00c5cf20 `CommitRead`.
    - Shape: `m_queued -= frames;` BEFORE `m_readPos += frames;` gives retail's edx/edi allocation and the zero
      stores through edx.
  - 00c5cf90 `CopyChannel`: one named stride local.
  - 00c5cfd0 `ClearChannel`: `(capacity*channel + writePos) * frameBytes`.
  - 00c5d010 `CopyFrom`.
    - Shape: exactly one of the 120 orders of the five set-up locals matches: src pointer, dst pointer, src stride,
      dst stride, byte count.
    - Both strides use THIS ring's frame size.
- **CThread**
  - 00c5e180 `Create`: CreateThread(security, stackSize, start, param, flags, &m_threadId).
    - Shape: `return m_handle != 0` gives `setne cl ; mov al,cl`.

## Parked for Fable 5.1 (7)
- 00c59b00 SecondsToMixBlocks DIFFER(129v155): retail rounds with the inline-asm ROUND idiom `fstp [esp+18h] ; fld [esp+18h] ; fistp dword [esp+8]` (0x00C59B80) where C++ gives `call __ftol2` | tried: the C++ reference body with a noinline RoundToNearest stand-in (1 grade) | next: retail_asm (bare `fistp dword`, owner exception 2026-10-24): C++ body plus a minimal `__asm { fld x ; fistp n }` and a `// Retail-asm:` line
- 00c59c60 CCodaVoice::SetVibrato DIFFER(210v210): after the second _CIpow call retail emits `pop edi ; fstp dword ptr [esi+44h]`, ours emits `fstp dword ptr [esi+44h] ; pop edi`; every other byte matches | tried: 20+ pv probes (statement orders, named float locals per result, scoped locals, a shared tail for the null/zero-period arms, `(float)m_vibratoPeriod` per division), plus the QFE-4035 compiler (same bytes) (2 grades) | next: lane gap: scheduling (epilogue `pop` hoisted past an x87 result store)
- 00c5a500 CSoftwareVoice::UpdateStep DIFFER(171v171): the same retail ROUND idiom `fstp [esp+4] ; fld [esp+4] ; fistp dword [esp+8]` (0x00C5A57C) against our RoundToNearest stand-in call | tried: the C++ reference body (1 grade) | next: retail_asm (bare `fistp dword`): C++ body plus a minimal `__asm` ROUND and a `// Retail-asm:` line
- 00c5d2e0 AccumulateFloatBlockSSE2 DIFFER(42v141): retail is an inline-SSE2 body (ebp frame, `stmxcsr`/`ldmxcsr` forcing round-to-nearest, eight `cvtps2dq`/`paddd`/`movdqa` passes) against our scalar C++ loop | tried: the C++ reference body (1 grade) | next: retail_asm (inline SSE2 with MXCSR control)
- 00c5d370 AccumulateBlockSSE2 DIFFER(57v84): retail is an inline-SSE2 body (ebp frame, `mov ecx,8`, `mov edi,edi` loop alignment, `movdqa`/`paddd`) against our scalar loop | tried: the C++ reference body (1 grade) | next: retail_asm (inline SSE2); emmintrin intrinsics cannot give the `mov edi,edi` alignment
- 00c5d3f0 AccumulateBlockSSE2Std DIFFER(59v86): the `ret 8` copy of the inline-SSE2 accumulator (byte twin of 0x00C5D4B0) against our scalar loop | tried: the C++ reference body (1 grade) | next: retail_asm (inline SSE2), the same body as 0x00C5D4B0
- 00c5d4b0 CMixBufferSSE2::MixAdd32 DIFFER(59v86): slot 5 of the SSE2 mixer vtable 0x012B5DE0, the inline-SSE2 accumulator with `this` unused and `ret 8`, against our scalar loop | tried: the C++ reference body (1 grade) | next: retail_asm (inline SSE2); land with 0x00C5D3F0

## Skipped
None. Every row is one function, and the row cuts agree with the bodies.

## Label corrections
Every catalog label in this chunk is propagated. GameCODA has no PDB or RTTI, so the new names are descriptive:
- 0x00C57C90 `CAudioSource_UnqueueProcessedBuffer` is `COpenALVoice::UnqueueBuffer`.
- 0x00C58210 `AudioFormat_DecodeSamples` is `CMixBuffer::MixMono16Ramped`.
- 0x00C58270 / 0x00C582F0 `Audio_RouteChannels2` are `CMixBuffer::MixStereoFloat` / `CMixBuffer::MixStereo32`.
- 0x00C58370 `AudioFormat_DecodeStereo` is `CMixBuffer::MixStereo16`.
- 0x00C591F0 `CArenaRoundDef_InitializeBuffers` is `CMixBuffer::CMixBuffer`.
- 0x00C592F0 `FollowCamera_UpdateDamping` is `CLimiter::UpdateEnvelope`.
- 0x00C59570 `Ctor_ZeroInit_012b8f64` is `CLimiter::CLimiter`.
- 0x00C595E0 `Math_Exp2` is `exp(float)` (`?exp@@YAMM@Z`). It computes e^x, not 2^x.
- 0x00C59890 `CAudioSource_UpdateDamping` is `CCodaVoice::UpdateVibrato`.
- 0x00C59960 `GUI_ComputeAlphaBlend` is `ComputePanGains`.
- 0x00C59B00 `Graphics_ConvertTimeToPixels` is `SecondsToMixBlocks`.
- 0x00C59C20 `CAudioSource_SetInitialTimeOffset` is `CCodaVoice::StartEnvelope`.
- 0x00C59C60 `GraphicalElement_ComputeAnimationTransform` is `CCodaVoice::SetVibrato`.
- 0x00C59D40 `Animation_ComputeFrameRate` is `LinearRampStep`.
- 0x00C59D80 `Math_ComputeExponent` is `ExponentialDecayStep`.
- 0x00C59DE0 `Graphics_TimeToPixelsWrapper` is `OctaveRampStep`.
- 0x00C5A3C0 `CAudioSource_ComputeBufferTime` is `CSoftwareVoice::SizeRing`.
- 0x00C5A480 `CAudioSource_UpdateTimeOffsetAdvanced` is `CCodaVoice::UpdateEnvelope`.
- 0x00C5A500 `CSound_ComputeEffectiveVolume` is `CSoftwareVoice::UpdateStep`.
- 0x00C5AF50 `RenderTarget_ClearBuffer` is `CSoftwareVoice::ClearMix`.
- 0x00C5AFC0 `CAudioSource_ApplyFilter` is `CSoftwareVoice::ApplyFilter`.
- 0x00C5B030 `Audio_InterpolateAndPlaySample` is `CSoftwareVoice::Resample`.
- 0x00C5BD00 `CTCTavernGameCoinGolf_PrepareRender` is `CSoftwareVoice::SetParameters` (vtable 0x012B5C60, slot 22).
- 0x00C5C2D0 `CTCTavernGameCoinGolf_Update` is `CSoftwareStreamVoice::SetParameters` (vtable 0x012B5D70, slot 22).
- 0x00C5C800 `CTCCarryable_OnCarryableKill` is `CSoftwareStreamVoice::MixMono`.
- 0x00C5C880 `CTCCarryable_OnDamageOverTime` is `CSoftwareStreamVoice::Mix`.
- 0x00C5C980 `Direction_MapBitmaskToIndex` is `SpeakerToChannelIndex`.
- 0x00C5CE60 `Audio_ComputeTimeOffset` is `CAudioRingBuffer::SetCapacity`.
- 0x00C5CF00 `Timer_Advance` is `CAudioRingBuffer::CommitWrite`.
- 0x00C5CF20 `AudioBuffer_AdjustPlaybackPosition` is `CAudioRingBuffer::CommitRead`.
- 0x00C5CF90 `Image_CopyRow` is `CAudioRingBuffer::CopyChannel`.
- 0x00C5CFD0 `Image_FillRow` is `CAudioRingBuffer::ClearChannel`.
- 0x00C5D010 `ImageData_CopyRows` is `CAudioRingBuffer::CopyFrom`.
- 0x00C5D2E0 `Math_AccumulateFloatArray` is `AccumulateFloatBlockSSE2`.
- 0x00C5D370 / 0x00C5D3F0 `IntBlock128_AddInPlace` are `AccumulateBlockSSE2` (`__cdecl`) /
  `AccumulateBlockSSE2Std` (`ret 8`).
- 0x00C5D4B0 `IntBlock128_AddInPlace` is `CMixBufferSSE2::MixAdd32` (vtable 0x012B5DE0, slot 5).
- 0x00C5DA70 `Audio_IsDirectSoundInitialized` is `IsWindows9x`. 0x00C5DA60 is the matching IsWindows95.
- 0x00C5E180 `CThread_Create` is `CThread::Create`. The name is close; the parameter order comes from the
  CreateThread pushes.
