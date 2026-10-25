# Hand chunk 440 (CRACBuffer .. CAudioContext): 36 of 40 byte-exact

All 40 rows sit in the libpk audio library (no PDB, no RTTI). Every catalog label in the chunk was either propagated
or descriptive, so each row is named from its callers, vtables, strings and imports (descriptive names). 36 rows grade
MATCH (4) or RELOCATION_MATCH (32) through gall.sh, and every test passes under both g++ and VC7.1. 35 rows matched on
their first real grade. 00c56ec0 took two, and 00c55f60 was re-graded once (same body) after its leaf was made a
mangled prefix.

## What matched (36)
- **CAudioContext**, the OpenAL dynamic loader (17 rows). Rows: 00c4d790 Suspend, 00c4d7d0 Process, 00c4d910
  BufferData, 00c4d960 GetError, 00c4dbe0 CheckError, 00c55980 LoadEntryPoints, 00c559c0 DestroyContext, 00c55a10
  IsDeviceExtensionPresent, 00c55a60 GetDeviceString, 00c55aa0 OpenDevice, 00c55b00 CreateContext, 00c55f60
  GetEAXGlobalProperty (slot 9), 00c55fe0 SuspendContext (slot 10), 00c561f0 IsExtensionPresent, 00c562f0 SupportsEAX3,
  00c56420 Release, 00c57690 SourceQueueBuffers.
  - Each entry point is resolved on first use with one ternary,
    `pfn = (PFN)(UseALGetProcAddress ? GetProcAddress("x") : ::GetProcAddress(Module, "x"))`. The member flag is at
    +0x104, the IAT GetProcAddress at 0x0143FE44 and the alGetProcAddress member at 0x00C4D740.
  - CheckError and GetEAXGlobalProperty inline GetError plus a `static inline ConvertALError`.
  - SupportsEAX3 declares `strncmp` as extern "C" without dllimport, so the call goes to the 0x00BFEAF8 thunk.
- **CAudioPlatform**, the platform layer (vtable 0x012B5B80). 00c4d1f0 Shutdown (slot 3) clears the four pools and
  restores the 0xa0-byte config record 0x0139C988 from 0x012B5AD8 by struct assignment (`mov ecx,28h ; rep movsd`).
  00c4d520 / 00c4d5f0 AcquireVoiceA / AcquireVoiceB recycle a free-chain voice while the slot's spare count (0x0139C9BC /
  0x0139C9D4) is positive, else create one through slot 6 / 7. Shape: `CVoicePool* pool = &m_poolA;` taken once (retail
  holds `lea edi,[ecx+2Ch]` for the whole body) and a single exit that VC7.1 tail-duplicates into every arm.
- **CEAXManager** (global 0x0143BB70):
  - 00c4e380 GetVoiceProperty and 00c51f00 SetVoiceProperty: the result accumulator (`long result = -4; if (...) result =
    call; return result;`) puts `mov eax,-4` ahead of the `je`. The out local is homed in the dead `voice` argument slot
    without any help.
  - 00c52980 SetListenerProperty: a switch on mode with `break`s gives the `xor eax,eax ; sub ecx,eax` dispatch, and the
    hardware case becomes `pop esi ; jmp 0x00C3EB60`.
- **CAudioSoundBuffer**: 00c4e1b0 SetData picks the AL format (0x1100-0x1103) through an inline helper that takes the
  sample format and channel count by value. Both are then read after GetError, so the channel load sits once inside the
  `dec eax` chain.
- **CAudioSource**, the software mixer voice:
  - 00c54ea0 ApplyVolume: the scaler is re-read after the `m_ramping = false` byte store, and the signed `/128` is
    `cdq ; and edx,7Fh`.
  - 00c55680 Open: one ebx zero register.
  - 00c551d0 Start (slot 10): inline Reset, the landed 0x00C55030. Its capability word is `| 0x1BF8000`.
- **CDecodedStream**: 00c546e0 PumpBlock. `__declspec(align(16)) float samples[4][128]` gives the `and esp,-16` frame,
  with an intrinsic memset.
- **CDirectSoundDevice**: 00c53260 Initialise runs CoInitializeEx, CoCreateInstance(CLSID_DirectSound), Initialize and
  SetCooperativeLevel, with an inline ReleaseDirectSound on each failure.
- **CDSoundStreamBuffer**: 00c57070 IsChunkReady. `if (played < chunk) return false; return true;` gives the byte form
  `sbb al,al ; inc al`, where `return played >= chunk` gives the full-register form.
- **CWaveFormatExtensible**: 00c56ec0 ctor. Stores follow WAVEFORMATEX fill order (tag, channels, rate, bits, cbSize,
  blockAlign, avgBytes, validBits, mask, SubFormat).
- **CCpuInfo**: 00c52ad0 GetIntelModelName. A family switch with an inner BrandIndex switch; every case-to-string pairing
  and jump-table entry was checked by hand with relmap. The row is 220 bytes: 186 of code plus the table.
- **Free functions**:
  - 00c52da0 RoundCpuSpeed: three rounding candidates, each distance made absolute as `if (d < 0) d = -d`
    (`sub ; jns ; neg`).
  - 00c55750 StringCopyWorkerA and 00c56090 StringCatWorkerA: verbatim strsafe workers as `extern "C" long __stdcall`,
    with StringLengthWorkerA `extern "C" __inline` in the same TU.
- **CRACBuffer**: 00c4c050 IsRangeAligned, two modulo tests.

## Parked for Fable 5.1 (4)
- 00c52aa0 IsCpuidSupported DIFFER(49v47): retail `pushfd ; pop eax ; mov ecx,eax ; xor eax,200000h ; push eax ; popfd ; pushfd ; pop eax ; xor eax,ecx` inside a `push ebp ; mov ebp,esp ; push ecx` frame, where ours calls flag stand-ins | tried: C++ reference with ReadEFlags/WriteEFlags stand-ins (1 grade) | next: retail_asm candidate (VC7.1 has no pushfd/popfd intrinsic): the minimal flag block with a `// Retail-asm:` line
- 00c52bb0 CCpuInfo::GetAMDModelName DIFFER(140v140): family-5 arm `mov ecx,[ecx+0Ch] ; cmp ecx,3 ; ja` where retail uses eax; every other byte and every literal pairing match | tried: signed/unsigned Model, `<= 3` / `< 4` / range test, explicit `==` chains, else-if, Model and reference locals, nested switch, inline helpers, `__fastcall` free function, optimize s/y/g pragmas, the QFE 4035 compiler (1 grade, the rest probes) | next: lane gap: register allocation
- 00c52ce0 MeasureCpuFrequency DIFFER(172v186): retail `rdtsc ; mov [lo],eax ; mov [hi],edx` twice inside an `and esp,-8` ebp frame (QueryPerformanceCounter/Sleep(10) loop, then __allmul and __aulldiv), where ours calls a TSC stand-in | tried: C++ reference (1 grade) | next: retail_asm candidate (no __rdtsc in VC7.1): two minimal rdtsc blocks with a `// Retail-asm:` line
- 00c53b10 Pow10 DIFFER(96v75): retail `fld [log2_10] ; fmul [x] ; frndint ; fst ; fwait ; fld1 ; fscale ; ... ; f2xm1 ; fadd [one] ; fmulp` block, where ours calls the CRT pow and floor | tried: C++ reference (1 grade) | next: retail_asm candidate (frndint / fscale / f2xm1 have no VC7.1 intrinsic)

## Skipped
None.

## Label corrections
Every catalog label in this chunk is propagated or descriptive; the names used here are descriptive (libpk has no PDB).
- 00c4c050 CBankFile_IsAligned -> CRACBuffer::IsRangeAligned
- 00c4d1f0 SetCurrentInterpolateInfo -> CAudioPlatform::Shutdown (vtable 0x012B5B80 slot 3)
- 00c4d520 CombatSequence_ProcessActionRound -> CAudioPlatform::AcquireVoiceA
- 00c4d5f0 CombatSequence_ProcessActionRound_Alt -> CAudioPlatform::AcquireVoiceB
- 00c4d790 CAudioBuffer_SuspendContext -> CAudioContext::Suspend
- 00c4d7d0 CAudioBuffer_ResumeContext -> CAudioContext::Process (alcProcessContext)
- 00c4d910 Audio_SetupBufferData -> CAudioContext::BufferData
- 00c4d960 OpenAL_InitAlGetError -> CAudioContext::GetError
- 00c4dbe0 CAudioDriver_GetErrorCode -> CAudioContext::CheckError
- 00c4e1b0 CAudioSoundBuffer_SetupFromStream -> CAudioSoundBuffer::SetData
- 00c4e380 ScriptEvent_DispatchIfActive -> CEAXManager::GetVoiceProperty
- 00c51f00 Model_WritePropertyIfValid -> CEAXManager::SetVoiceProperty
- 00c52980 NormalizeProjectionAndSerialize -> CEAXManager::SetListenerProperty
- 00c52ad0 CPU_GetIntelBrandName -> CCpuInfo::GetIntelModelName
- 00c52bb0 CPU_GetAMDBrandName -> CCpuInfo::GetAMDModelName
- 00c52ce0 CPU_CalibrateTSCFrequency -> MeasureCpuFrequency (returns Hz; the caller divides by 1000000)
- 00c52da0 CPU_FindNearestRoundedValue -> RoundCpuSpeed
- 00c53260 Audio_InitDirectSound3D -> CDirectSoundDevice::Initialise
- 00c53b10 Pow2_Float -> Pow10 (wrong base: the multiplier is log2 10, 0x40549A78)
- 00c546e0 Audio_ApplyEffectChain -> CDecodedStream::PumpBlock
- 00c54ea0 CAudioSource_OnKill -> CAudioSource::ApplyVolume
- 00c551d0 CTCTavernGameCoinGolf_UpdateRotation -> CAudioSource::Start
- 00c55680 CAudioSource_SetSpellEffect -> CAudioSource::Open
- 00c55980 OpenAL_InitAudioFunctions -> CAudioContext::LoadEntryPoints
- 00c559c0 AudioContext_Destroy -> CAudioContext::DestroyContext
- 00c55a10 CAudioContext_GetExtensionProc -> CAudioContext::IsDeviceExtensionPresent
- 00c55a60 CAudioContext_GetStringProc -> CAudioContext::GetDeviceString
- 00c55aa0 OpenAL_InitAlcOpenDevice -> CAudioContext::OpenDevice
- 00c55b00 OpenAL_CreateAndActivateContext -> CAudioContext::CreateContext
- 00c55f60 CAudioContext_GetEAXProc -> CAudioContext::GetEAXGlobalProperty
- 00c55fe0 CAudioContext_GetSuspendProc -> CAudioContext::SuspendContext
- 00c56090 StringCopy_ValidateLength -> StringCatWorkerA (strsafe)
- 00c561f0 CAudioContext_CheckExtensionPresent -> CAudioContext::IsExtensionPresent
- 00c562f0 OpenAL_CheckCreativeEAX3Support -> CAudioContext::SupportsEAX3
- 00c56420 CEngineWaterRenderer_ReleaseAudioResources -> CAudioContext::Release
- 00c56ec0 Audio_FormatWaveHeader -> CWaveFormatExtensible::CWaveFormatExtensible
- 00c57070 Stream_CheckBytesAvailable -> CDSoundStreamBuffer::IsChunkReady
- 00c57690 CAudioContext_CallAlSourceQueueBuffers -> CAudioContext::SourceQueueBuffers
- Kept: 00c55750 _StringCopyWorkerA@12 (correct), 00c52aa0 CPU_IsCpuidSupported (descriptive, right).
- Callees named by this chunk:
  - 0x00C66C90 "ScriptEvent_DispatchByID" -> the EAX effect's GetProperty.
  - 0x00C68510 "CDrawGuildSeal_SetProperty" -> the EAX effect's SetProperty.
  - 0x00C68F90 "CDrawGuildSeal_SetPropertyEx" -> the EAX listener effect's SetProperty.
  - 0x00C51A20 "PropertySerializer_WriteProperty" -> CEAXManager::QueueCommit.
  - 0x00C4DC40 "CAudioBuffer::Create" -> CAudioSoundBuffer::Create.
  - 0x00C4DE00 "CAudioSoundBuffer_TransferPCMData" -> its KeepShadowCopy.
