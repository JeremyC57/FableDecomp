# Hand chunk 444 (CVtable_012bedcc .. CReverbPreDelay): 32 of 40 byte-exact

Rows 0xC70750..0xC76DD0 all sit inside libpk (the audio middleware; no PDB/RTTI). Strings and imports place them in
four modules: the software mixer's resampling voice effects (0xC70750..0xC71750), the OpenAL source/sample layer
(0xC71F10..0xC73830), the stream decompressor interface (0xC73D90, ".\CDecompressorInterface.cpp") and the FDN
reverb's DSP parts (0xC74420..0xC76DD0). libpk carries on past this chunk (the reverb's SIMD kernels at 0xC88A30..).
Every catalog label in the chunk was propagated and wrong. All names here are descriptive names (from strings,
vtables and callers); class names follow the landed siblings (CVtable_<vtable> for the mixer effects,
CAudioContextEAX from the landed 0x00C71CF0 destructor).

## What matched (32)
- **CVtable_012bedcc / CVtable_012bede0 (mixer voice effects, vtables 0x012BEDCC / 0x012BEDE0)**:
  - UpdateVoices 0x00C70750 / 0x00C71120: under the +8 lock, store the cross-fade flag and rebuild the two voice records of the pending set (cross-fade) or of the current set, giving both the `rear > front` output swap, then SetupVoice each one.
    - Shape: write `set[0].swap = set[1].swap = rear > front` in each arm through the set-pointer member. VC7.1 head-hoists the common fld/fcomp/fnstsw above the branch and tail-merges the second call.
    - The twin is the same body over 0x2a4-byte records (all offsets moved up by 0x600 / 0x180).
- **Filtered-voice mixers (free __cdecl, effect 0x012BEDE0)**: MixFilteredVoiceFadeIn 0x00C70C10, MixFilteredVoiceFadeOut 0x00C70CB0 and MixFilteredVoice 0x00C70D60.
  - These are chunk 443's MixVoice* shapes moved to +0x290.., with a 0x10-quad block and the FIR resamplers 0xC843F0 / 0xC7E420 / 0xC812F0 / 0xC7BAF0.
  - The hoisted common pushes and the in-place tail call came straight from the ported sources.
- **CAudioContextEAX::WriteOutput 0x00C71750 (EAX 3.0 environment, vtable 0x012BEDF4 slot 7)**: saturate the base's 128-sample long mix block (+0x21c) into shorts.
  - Shape: a hand-unrolled ×4 loop with an unsigned `i < 128; i += 4` counter, and an inline copy of the LTCG clamp helper 0x00C716E0.
- **CAudioSource (OpenAL source)**:
  - SetPitchProperties 0x00C71F10: the flag mask is written in both places.
  - StartLinearFade 0x00C72130: the length comes from the stored member.
  - QueueNextBuffer 0x00C72C40: one positive `&&`/`||` guard around a block that ends in its own `return 0`, with a positive `time <= now` test inside. This puts every `return 0` epilogue at the end and keeps ebx shrink-wrapped.
- **CSampleData::GetPlaybackData 0x00C732F0 / CAudioDeviceOpenAL::GetSampleData 0x00C73340**: return the stereo source, or convert it once (MakeStereo 0x00C4DE00) and return the converted block.
  - Shape: one `long result = 0` returned once. The device version is a virtual that inlines the accessor on `sample + 0x30`.
- **CRegistryKey::Create 0x00C737D0**: `RegCreateKeyExA(parent, subKey, 0, "", 0, access, 0, &m_hKey, 0) == 0` returned as an int.
- **DestroyDecompressor 0x00C73D90 (__fastcall, /EHsc)**: chunk 436's PK_REPORT_ERROR (".\CDecompressorInterface.cpp", line 58, "Null decompressor") in the null arm of an if/else, otherwise the virtual scalar deleting destructor.
- **CDspObject::CDspObject 0x00C74420**: the shared ops table is built on first use (0x00C74360), otherwise the refcount goes up.
- **CDspGain::Apply 0x00C74CE0**: ApplyRamps 0x00C74510, then `Scale` when `master != 1.0f`. That compare is the integer `cmp 3F800000h`.
- **CDelayLine / CStereoDelayLine**:
  - CDelayLine ctor 0x00C74F60: an init list of data and size only.
  - CStereoDelayLine ctor 0x00C75410: /EHsc, giving the `eh vector constructor iterator`.
  - CStereoDelayLine::Clear 0x00C75010: a counted loop over an inline `if (m_data) memset; m_pos = 0`.
- **CReverbDiffuser**:
  - Reset 0x00C75670: two counted loops with an inline stage Reset.
  - Process 0x00C75A20: three UpdateGain calls, `memset(output, 0, count*sizeof(float))` (rep stosd), then two stage Process calls on `inputs[j % channels]`.
- **CReverbTapBank**:
  - Allocate 0x00C75C30: the channel count is kept on the memory pass, and the index is re-read from `*pIndex` each time.
  - Reset 0x00C75CA0: an inline tap Reset.
  - Process 0x00C75E90: a 16-byte-aligned `float blocks[4][128]` local, a named `channel = i % m_channels`, ProcessRamp for the tap whose index is changing, then four Feedback calls with the 0x012BF088 mix-matrix rows.
- **CDspOps ring helpers** 0x00C763E0 / 0x00C76460 / 0x00C764E0 / 0x00C76560 / 0x00C76680 / 0x00C766F0 / 0x00C76760 / 0x00C767E0:
  - Each is one kernel call when `[pos, pos+count)` fits, else a tail call plus a wrapped head call.
  - memcpy of `count*sizeof(float)` on a plain variable gives `rep movsd`.
  - The ramp variants need a signed `int first` (plain fild) and `gain + first*step`.
- **CReverbDelay::Process 0x00C76B60**: an aligned 128-float scratch block; an if/else read position; ReadRingScaled, WriteRing, AddToRing, then an optional Mix; an inline ring advance.

## Parked for Fable 5.1 (8)
- 00c720c0 CAudioSource::StartExponentialFade DIFFER(111v111): retail `fld dword [0.0f] ; fld st(1) ; fucompp`, ours `fld st(0) ; fld dword [0.0f] ; fucompp` | tried: float length local; no local; a double local with a double compare; `(float)length != 0.0f` | next: `0.0f != (float)length`
- 00c73830 CRegistryKey::QueryDWord DIFFER(72v72): retail keeps the merged `xor eax,eax ; pop ecx ; ret 8` right after the RegQueryValueExA test; ours puts it after the type test | tried: two separate ifs; `return type == 4 && size == 4`; `if (type == 4) return size == 4; return 0;`; the QFE-4035 compiler | next: lane gap: merged-return block placement
- 00c74ee0 IsPrime DIFFER(101v95): retail takes `n` in ebx with no stack argument, and its loop bound is a bare `fld ; fistp dword` float-to-int; ours takes `n` on the stack and calls _ftol | tried: a C++ reference body (1 grade) | next: lane gap: LTCG register argument + retail_asm (fistp)
- 00c75050 CStereoDelayLine::Allocate DIFFER(237v232): retail passes the IsPrime candidate in ebx (`or ebx,1 ; call ; add ebx,2 ; call`) with no push or cleanup; ours pushes it | tried: a C++ reference with an inline NextPrime (1 grade) | next: lane gap: LTCG register argument
- 00c755b0 CReverbDiffuser::Allocate DIFFER(166v178): retail truncates the stage length with `fsub 0.5f ; fstp ; fld ; fistp dword ; if (i<0) ++i`; ours calls _ftol | tried: a C++ reference with a `(long)` cast (1 grade) | next: lane gap: retail_asm candidate (bare fistp; a C++ body plus a minimal `__asm fistp` with a `// Retail-asm:` line)
- 00c75f80 CReverbTap::Allocate DIFFER(111v111): retail keeps the block in eax (`lea edi,[eax+esi*4]`, the memset reloads m_data, size `mov ecx,esi ; shl ecx,2`); ours CSEs length*4 and keeps the block in edi | tried: a block local; `m_data = *pMemory; *pMemory += length`; `*pMemory += length` before the memset; an inline TakeFloats helper | next: clear through an inline ring Clear on `m_size * sizeof(float)` (the member, not the argument)
- 00c76ab0 CReverbDelay::Allocate DIFFER(150v175): retail computes the length as `(trunc(rate * 0.25f) + 3) & ~3` with the bare-fistp truncation; ours calls _ftol | tried: a C++ reference (1 grade) | next: lane gap: retail_asm candidate (bare fistp)
- 00c76dd0 CReverbPreDelay::Allocate DIFFER(157v186): the same bare-fistp truncation of `rate * 0.5f`; ours calls _ftol | tried: a C++ reference (1 grade) | next: lane gap: retail_asm candidate (bare fistp)

## Skipped
- None. No row holds two functions, and no row is int3-after-noreturn.

## Label corrections
All of these catalog labels are propagated; the replacements are descriptive names.

| Catalog label | Address | Correct name |
|---|---|---|
| Audio_SetChannelOutputMode | 0x00C70750 | CVtable_012bedcc::UpdateVoices |
| Audio_SetChannelOutputMode_Alt | 0x00C71120 | CVtable_012bede0::UpdateVoices |
| Graphics_RenderSprite_16x16 | 0x00C70C10 | MixFilteredVoiceFadeIn |
| Graphics_RenderSprite_16x16_Alt | 0x00C70CB0 | MixFilteredVoiceFadeOut |
| Graphics_RenderText_Conditional2 | 0x00C70D60 | MixFilteredVoice |
| AudioBuffer_ClampAndConvert16bit | 0x00C71750 | CAudioContextEAX::WriteOutput |
| Audio_InitSource_Properties | 0x00C71F10 | CAudioSource::SetPitchProperties |
| Math_ConvertToPowerOfTwo_or_ComputeScale | 0x00C720C0 | CAudioSource::StartExponentialFade |
| Audio_SetSource_Gain | 0x00C72130 | CAudioSource::StartLinearFade |
| Audio_ProcessQueue_NavNode | 0x00C72C40 | CAudioSource::QueueNextBuffer |
| CAudioSoundBuffer_ConvertStereoSample | 0x00C732F0 | CSampleData::GetPlaybackData |
| CAudioSoundBuffer_GetPosition | 0x00C73340 | CAudioDeviceOpenAL::GetSampleData (vtable 0x012B8D48 slot 19) |
| AudioDriver_LoadRegistry_Key | 0x00C737D0 | CRegistryKey::Create |
| Registry_QueryDWord | 0x00C73830 | CRegistryKey::QueryDWord |
| MessageDecompressor_Release | 0x00C73D90 | DestroyDecompressor |
| CRefCountedObject_AddRef | 0x00C74420 | CDspObject::CDspObject |
| DisplayProgress | 0x00C74CE0 | CDspGain::Apply |
| Math_IsPrimeNumber | 0x00C74EE0 | IsPrime (a libpk LTCG static) |
| Ctor_Init_012bf074 | 0x00C74F60 | CDelayLine::CDelayLine |
| MemBuffer_ClearDoubleBuffered | 0x00C75010 | CStereoDelayLine::Clear |
| CQ_GuildTrainingSkillScript_InitializePrimeHashTables | 0x00C75050 | CStereoDelayLine::Allocate |
| CTCKraken_Construct | 0x00C75410 | CStereoDelayLine::CStereoDelayLine |
| CQ_GuildTrainingSkillScript_InitializeSkillSlots | 0x00C755B0 | CReverbDiffuser::Allocate |
| CQ_GuildTrainingSkillScript_ResetAll | 0x00C75670 | CReverbDiffuser::Reset |
| Graphics_LoadTerrainChunksAndStream | 0x00C75A20 | CReverbDiffuser::Process |
| CQ_GuildTrainingSkillScript_InitializeEffectArray | 0x00C75C30 | CReverbTapBank::Allocate |
| CQ_GuildTrainingSkillScript_DestroyEffectArray | 0x00C75CA0 | CReverbTapBank::Reset |
| Audio_ProcessMultipleTriggerEvents | 0x00C75E90 | CReverbTapBank::Process |
| CQ_GuildTrainingSkillScript_AllocateSkillBuffer | 0x00C75F80 | CReverbTap::Allocate |
| CArray_CopyCircular_Wrap | 0x00C763E0 | CDspOps::ReadRing |
| Vector_RangeInsert | 0x00C76460 | CDspOps::WriteRing |
| Array_CopyWrapped | 0x00C764E0 | CDspOps::ReadRingScaled |
| StreamWriter_WriteWrapped | 0x00C76560 | CDspOps::ReadRingScaledRamp |
| CArrayWrapper_CopyWraparound | 0x00C76680 | CDspOps::AddFromRing |
| Array_CopyWrapped_Variant | 0x00C766F0 | CDspOps::AddToRing |
| WaveformBuffer_ProcessWrapped | 0x00C76760 | CDspOps::MixFromRing |
| Partition_Copy_Split | 0x00C767E0 | CDspOps::MixRampFromRing |
| CQ_GuildTrainingSkillScript_ActivateSkillByIndex | 0x00C76AB0 | CReverbDelay::Allocate |
| Array_CopyStrided | 0x00C76B60 | CReverbDelay::Process |
| CQ_GuildTrainingSkillScript_DeactivateSkillByIndex | 0x00C76DD0 | CReverbPreDelay::Allocate (vtable 0x012B9580) |

Catalog rows outside the chunk:
- engine_api.tsv lists 0x00C70A30 and 0x00C71400 as `CLandscapeBackgroundPatch` vector deleting destructors. They are the deleting destructors of the voice effects 0x012BEDCC / 0x012BEDE0.
- engine_api.tsv lists 0x00C70120 and 0x00C70B00 as `CPersistContext_TransferableVector<CReverbEnvironmentEntry>::Resize`. They are those two effects' constructors.
