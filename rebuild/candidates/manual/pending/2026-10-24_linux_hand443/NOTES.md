# Hand chunk 443 (CEAXSource .. MixVoice): 21 of 40 byte-exact

The whole chunk (0x00C649C0..0x00C70470) sits inside libpk, the audio middleware. It has no PDB, no RTTI and no
string references in 0x00C63000..0x00C71000. The CPU-detection strings ("Pentium III Xeon", 0x00C52Axx) come before
it and the "EAXSet" lookups (0x00C718xx) come after it, so libpk carries on past the chunk. All class and function
names are descriptive except the EAX ones. Those follow the EAX 4.0 SDK property names: the accepted ranges, flag masks
and dirty bits are the SDK's EAXSOURCEPROPERTIES / EAXFXSLOTPROPERTIES / EAXREVERBFLAGS values. The mixer effects are
named by vtable, as their landed siblings are.

9 MATCH + 12 RELOCATION_MATCH. 19 parked: 14 retail_asm candidates, 1 LTCG register-argument row and 4
scheduling-only twins. 0 skipped. Every src/ file grades MATCH/RELOCATION_MATCH through gall.sh (grades.txt), and every
test prints PASS under tall.sh (g++) and tall.py (VC7.1/Wine).

## What matched (21)

- **CEAXSource** (EAX 4.0 source property layer: current EAXSOURCEPROPERTIES at +0x18, deferred copy at +0x208,
  dirty mask +0x04, context +0x14 whose +0x3d4 holds the last error).
  - The rows: 0x00C649C0 SetDirect, 0x00C64A00 SetDirectHF, 0x00C64A40 SetRoom, 0x00C64AC0 SetObstruction,
    0x00C64D70 SetOutsideVolumeHF and 0x00C64F70 SetFlags.
  - The shape: a range check whose error block sits on the fall-through (`context->m_lastError = -2; return -3`).
    Then, on a change, the chained store `m_current.x = m_deferred.x = v` and a dirty-bit or.
  - SetRoom stores only the current copy.
  - SetFlags tests `flags & ~7UL` with the error on the fall-through.
  - Generator: gen/gen_src.py.
- **CEAXContext** (current slot +0x04, EAXFXSLOTPROPERTIES[4] at +0x84, EAXREVERBPROPERTIES[4] at +0xf4, committed
  record +0x340, dirty +0x3bc, last error +0x3d4).
  - 0x00C67430 SetSlotFlags: `long result = 0;` declared first gives the early `xor eax,eax`. The setter call is
    `SetSlotEnabled(slot, (flags & 1) == 1)`.
  - 0x00C67720 SetSlotVolume: recommits the room level of the current reverb slot. The value is floored at -10000
    and the GUID is compared with a `repe cmpsd` memcmp.
  - 0x00C67A10 SetReverbFlags: the eight EAXREVERBFLAGS bit setters (0x00C63F90..0x00C64380) are same-TU
    `__declspec(noinline)` bodies under `auto_inline(off)`. That keeps `this` in ecx across the chain of
    `if ((r = SetX(slot, (flags >> n) & 1)) < 0) return r;`.
- **CReverbProcessor** (software reverb, vtable 0x012B9594).
  - The rows: 0x00C6A180 ProcessStage94 and 0x00C6A200 ProcessStage144.
  - The fix: a stage-pointer local formed first (`CReverbStage94* stage = &m_stage94;`). This gives the early `lea` and
    moves `this` to eax.
  - Both settings pointers are read into locals before the inline `Covers(element)` range test.
- **CStateVariableFilter** (Chamberlin SVF).
  - 0x00C6BBD0 SetCutoff: a float clamp to 200..8000, then `2.0f * (float)sin(c * 7.1237933e-05f)` under
    `#pragma intrinsic(sin)`.
  - 0x00C6BC80 SetParameters and 0x00C6BD00 SetParametersWithGain need three things:
    - a same-TU noinline SetCutoff, which keeps ecx/edx live;
    - the silence test written `0.0f == m_band && 0.0f == m_low`, which loads the member first for fucompp;
    - a named `float gain` local, which stays in st0 across the `< 0.09f` fcom.
- **Delay-tap mixers** (the 0x012BED7C effect).
  - 0x00C6CAF0 MixDelayTapRampUp and 0x00C6CB80 MixDelayTapRampDown use a post-incremented history pointer next to an
    indexed accumulator. In the 4x-unrolled loop the indexed pointer is biased by two elements and the post-incremented
    one is not, which is retail's pointer bias.
  - 0x00C6CC10 MixDelayTap needs the accumulator copied into a named `out` local before the history pointer is formed.
    That puts the gain word in si, as retail.
- **CVtable_012bedb8** 0x00C6E760, the constructor: base ctor, own vtable, then the two pointer members in init-list
  order.
- **Voice dispatchers.**
  - The rows: 0x00C70240 MixVoiceFadeIn, 0x00C702E0 MixVoiceFadeOut and 0x00C70390 MixVoice.
  - They dispatch on `g_pAudioMaster->m_outputChannels >= 4` (0x0143BC2C +0x1bc), then on the voice's swap flag.
    MixVoice tests the mute flag first.
  - VC hoists the common trailing constant pushes above the branch.
  - A cdecl call with the caller's own argument count becomes a slot-rewrite tail call.

## Parked for Fable 5.1 (19)

- 00c6ad10 CSoundMixer::MixBlocksRamped DIFFER(92v89): retail `mov ecx,ebp ; call 0C6AAE0h ; add esp,10h` (start gain in ecx, four stack args, caller pops); ours pushes the start gain as a fifth cdecl argument | tried: the plain cdecl declaration (1 grade) | next: lane gap: LTCG custom calling convention (register argument)
- 00c6aef0 CVoiceEffect::Configure DIFFER(108v92): retail `fsub [0.5f] ; fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp+10h] ; test ; jge ; inc` (libpk's inline-asm truncation helper); ours rounds through C++ sign arms + `call __ftol2`; both calls, including the vtable slot-1 call through the stage pointer, and the imul tail line up | tried: C++ reference body (1 grade) | next: retail_asm candidate: land with the minimal `fld ; fistp` inline-asm helper and a `// Retail-asm:` line
- 00c6b3a0 ConvertLongToFloat DIFFER(265v237): retail runs SSE2 `cvtdq2ps/movaps` and SSE `cvtpi2ps/movlps ... emms` inline-asm blocks in an ebp frame; ours models them with scalar loops | tried: C++ reference body (1 grade) | next: retail_asm candidate (keep the x87 fallback in C++)
- 00c6b8c0 CStateVariableFilter::FilterRamped DIFFER(237v219): retail `fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+8]` truncation helper; ours `call __ftol2` with C++ rounding arms | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6bae0 CStateVariableFilter::Filter DIFFER(252v235): retail `fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+10h]` truncation helper; ours `call __ftol2` | tried: C++ reference body (1 grade) | next: retail_asm candidate (also check the target commit through `mov eax,esi`, an inlined struct assignment)
- 00c6bdc0 LookupDirectionIndex DIFFER(165v146): three retail `fstp ; fld ; fistp dword [esp/esp+0Ch]` truncation-helper expansions; ours `call __ftol2` x3 | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6be60 ComputeDirectionalGains DIFFER(250v226): retail `fistp dword [esp+4]` truncation helper, plus edx (column - 20) kept live across `call 0C6BDC0h` and the callee's full eax used; ours `call __ftol2` and reloads | tried: C++ reference body (1 grade) | next: retail_asm candidate; the edx-across-call is an LTCG register-allocation lane gap
- 00c6c000 COnePoleLowPass::SetLevel DIFFER(199v240): retail inline-asm 10^x block (`fld ; fmul ; frndint ; fst ; fwait ; fld1 ; fscale ; ... f2xm1 ; fadd ; fmulp ; fwait` over C locals) and a `fistp dword` truncation helper; ours calls pow + __ftol2 | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6cef0 CVtable_012bed7c::ComputeTapGain DIFFER(82v66): retail `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` rounding helper; ours C++ rounding arms + `call __ftol2`; the clamp and the three stores match byte for byte | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6d370 MixSamplesSSE2 DIFFER(318v156): retail SSE2 inline-asm loop (`movdqu ; pmulhw ; punpcklwd/hwd ; psrad 0Fh ; paddd ; movdqa`) in an aligned /GS frame; ours is the scalar model | tried: C++ reference body with /GS (1 grade) | next: retail_asm candidate
- 00c6d410 MixSamplesRampSSE2 DIFFER(520v240): retail SSE2 inline-asm ramp loop (`pmulhw ; paddsw`), aligned /GS frame, start gain read from ecx; ours is the scalar model with a stack argument | tried: C++ reference body with /GS (1 grade) | next: retail_asm candidate; the ecx argument stays an LTCG lane gap
- 00c6da40 MixGainRampUp DIFFER(139v139): the unrolled loop's two pointer steps are swapped (retail `add eax,10h ; add edx,8`, ours `add edx,8 ; add eax,10h`); every other byte matches | tried: index form; post-incremented source (129v139); `acc[i] = acc[i] + ...`; a `scaled` local (4 grades) | next: lane gap: induction-variable order (compiler-build artefact)
- 00c6dad0 MixGainRampDown DIFFER(139v139): the same pair of pointer steps swapped | tried: the same four forms as 0x00C6DA40 (4 grades) | next: lane gap: induction-variable order
- 00c6de60 CVtable_012bed90::ComputeGainRamp DIFFER(75v59): retail `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` rounding helper; ours `call __ftol2`; clamp and stores match | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6e790 MixGainRampUp DIFFER(139v139): byte-identical copy of 0x00C6DA40 in the 0x012BEDB8 effect's TU, the same pointer-step swap | tried: index form, do/while, an `out` local (3 grades) | next: lane gap: induction-variable order
- 00c6e820 MixGainRampDown DIFFER(139v139): byte-identical copy of 0x00C6DAD0, the same pointer-step swap | tried: `gain * source[i]`, `out`/`in` local copies (2 grades) | next: lane gap: induction-variable order
- 00c6eba0 CVtable_012bedb8::ComputeGainRamp DIFFER(74v62): retail `fstp [esp] ; fld [esp] ; fistp dword [esp+4]` rounding helper; ours `call __ftol2` | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6ebe0 CVtable_012bedb8::SetAlternate DIFFER(123v106): retail `fld [esp+10h] ; fistp dword [esp+8]` in both arms (the product homed in the dead `alternate` slot); ours one `call __ftol2` after merged arms; the lock calls, flag store and record stores line up | tried: C++ reference body (1 grade) | next: retail_asm candidate
- 00c6f0f0 MixSamplesMMX DIFFER(296v173): retail MMX inline-asm loop (`movq ; pmulhw ; punpck ; psrad ; paddd`, x2 per pass, `emms`) in an aligned /GS frame; ours is the scalar model | tried: C++ reference body with /GS (1 grade) | next: retail_asm candidate

## Skipped

None. No row holds two functions; every row's bytes end at its own `ret` plus int3 padding.

## Label corrections

Every manifest label in this chunk is propagated and wrong. The new names are descriptive (no PDB) except the EAX
property setters.

| Row | Manifest label | Correct name |
|---|---|---|
| 0x00C649C0 | HeroAttribute_SetBaseLife | CEAXSource::SetDirect |
| 0x00C64A00 | HeroAttribute_SetBaseSpell | CEAXSource::SetDirectHF |
| 0x00C64A40 | SetPitch | CEAXSource::SetRoom |
| 0x00C64AC0 | HeroAttribute_SetBaseArmour | CEAXSource::SetObstruction |
| 0x00C64D70 | CEntity_SetIntProperty_0x48 | CEAXSource::SetOutsideVolumeHF |
| 0x00C64F70 | HeroAttribute_SetUintOffset5C | CEAXSource::SetFlags |
| 0x00C67430 | CCharacter_SetSkillAcquired | CEAXContext::SetSlotFlags |
| 0x00C67720 | CCharacter_SetSkillDamage | CEAXContext::SetSlotVolume |
| 0x00C67A10 | HeroAttribute_SetBitFlags | CEAXContext::SetReverbFlags |
| 0x00C6A180 | Audio_ProcessTriggersForFrame | CReverbProcessor::ProcessStage94 |
| 0x00C6A200 | Graphics_ProcessTerrainStreamForFrame | CReverbProcessor::ProcessStage144 |
| 0x00C6AD10 | _Fill_n<unsigned_long*,unsigned_int,unsigned_long> | CSoundMixer::MixBlocksRamped |
| 0x00C6AEF0 | Audio_ConfigureChannel_Scaling | CVoiceEffect::Configure |
| 0x00C6B3A0 | LongToFloatConvert_Vectorized | ConvertLongToFloat (the name was close) |
| 0x00C6B8C0 | Audio_FilterSamples_1stOrderDirect | CStateVariableFilter::FilterRamped |
| 0x00C6BAE0 | Audio_FilterProcess | CStateVariableFilter::Filter |
| 0x00C6BBD0 | Audio_ComputeGainScale | CStateVariableFilter::SetCutoff |
| 0x00C6BC80 | Graphics_ComputeScaleConstraint | CStateVariableFilter::SetParameters |
| 0x00C6BD00 | Audio_ComputeGain | CStateVariableFilter::SetParametersWithGain |
| 0x00C6BDC0 | Audio_GetFrequencyTableEntry | LookupDirectionIndex |
| 0x00C6BE60 | Audio_ComputeFrequencyResponse | ComputeDirectionalGains |
| 0x00C6C000 | Audio_ComputeDecibelsToLinear | COnePoleLowPass::SetLevel |
| 0x00C6CAF0 | Audio_ResampleAndAccumulate | MixDelayTapRampUp |
| 0x00C6CB80 | Audio_ResampleDecayAndAccumulate | MixDelayTapRampDown |
| 0x00C6CC10 | Audio_ApplyGain | MixDelayTap |
| 0x00C6CEF0 | Audio_SetFilterCoefficients | CVtable_012bed7c::ComputeTapGain |
| 0x00C6D370 | Audio_ProcessSSEMultiply | MixSamplesSSE2 |
| 0x00C6D410 | SIMD_AccumulateWeightedSamples_16bit | MixSamplesRampSSE2 |
| 0x00C6DA40 / 0x00C6E790 | FixedPoint_AccumulateShort / DSP_AccumulateModulatedWaveform | MixGainRampUp |
| 0x00C6DAD0 / 0x00C6E820 | FixedPoint_AccumulateUshortDelta / DSP_AccumulateUnsignedModulatedWaveform | MixGainRampDown |
| 0x00C6DE60 | Audio_ConvertFloatToS15 | CVtable_012bed90::ComputeGainRamp |
| 0x00C6E760 | CConsoleCommandParameters_Constructor | CVtable_012bedb8::CVtable_012bedb8 |
| 0x00C6EBA0 | AudioGainLookup_GetValues | CVtable_012bedb8::ComputeGainRamp |
| 0x00C6EBE0 | Audio_SetVolume_Channel | CVtable_012bedb8::SetAlternate |
| 0x00C6F0F0 | Audio_ProcessSSE64BitMultiply | MixSamplesMMX |
| 0x00C70240 | Graphics_RenderSprite_32x32_Base | MixVoiceFadeIn |
| 0x00C702E0 | Graphics_RenderSprite_32x32_Alt | MixVoiceFadeOut |
| 0x00C70390 | Graphics_RenderText_Conditional | MixVoice |

Callees and neighbours outside the chunk:

| Address | Manifest label | What it is |
|---|---|---|
| 0x00C63F90..0x00C64380 | HeroAttribute_SetBitFlagBit0..7 | the eight CEAXContext EAXREVERBFLAGS bit setters |
| 0x00C6AAE0 | Audio_ProcessSamples_IIRFilter | the SSE2 ramp mixer, the same shape as 0x00C6D410 |
| 0x00C783F0 | CAudioChannel_ProcessEffects | a thiscall configure step on the voice effect's +0x260 part |
