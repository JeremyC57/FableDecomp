# Hand chunk 496 (CDSoundStreamBuffer .. CAudioSource): 37 of 40 byte-exact

21 MATCH and 16 RELOCATION_MATCH. 8 of the 37 are retail_asm rows in `retail_asm/` (7 MATCH, 1 RELOCATION_MATCH);
the other 29 are plain C++ in `src/` (14 MATCH, 15 RELOCATION_MATCH). One row is parked, two are skipped.

Checks:
- Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and under VC7.1 (tall.py).
- The retail_asm tests are `retail_asm/<addr>.test.cpp`, run with the same `behaviour_of` call.
- `purity.classify` gives `retail_asm` for all 8 retail_asm sources and `genuine` for all 29 src/ sources.
- The vland dry run over src + retail_asm together gives 37 WINS and no RELOC_IDENTITY.
  - It leaves 5 RELOC_NAME warnings: manifest labels on callees (see "Callee names").
- Every `// Leaf:` was confirmed unique with `objdump -t`.
- The retail_asm rows are also in `payload_retail_asm.json`. finish.py only packs src/.

All rows are GameCODA / libpk audio code (0xC57380..0xC727B0): no PDB and no RTTI. Nearly every row is a vtable slot.
Each slot was traced to its vtable through the constructor's vtable store, then to the class that owns that vtable.

## What matched (37)
- **CMixBuffer kernels** (vtable 0x012B8F38): the landed model 0x00C58370 / 0x00C591F0 already names these addresses.
  - MixAdd32 0x00C58000, MixAdd32Gain 0x00C58040, MixAdd16 0x00C580C0: plain `for (i < 128)` loops. VC7.1 unrolls
    them by 4 and biases the pointers itself.
  - MixAddFloat 0x00C57F20, MixAddFloatGain 0x00C57FC0, MixAddFloatGain16 0x00C57F60 (retail_asm): the same loops
    around libpk's `__forceinline` fld/fistp FloatToLong helper.
- **CLimitedMixBuffer::WriteChannel** 0x00C591C0: slot 10 of 0x012B5BE0 calls `m_limiters[channel].Process(...)`.
  - CMixBuffer's own slot 10 (0x00C590E0) shows that +0x1C is an output shift.
- **CVtable_012b5de0::MixAddFloat** 0x00C5D490: the SSE2 mix buffer's slot 8. It forwards to the cdecl SSE2 kernel
  0x00C5D2E0.
- **CCodaVoice** (vtable 0x012B5C10):
  - CheckPosition 0x00C59770: a signed `__int64` compare against a bit-field flag word.
  - SetPositionC 0x00C597B0: `shr eax,5 ; test al,1` needs an inline `unsigned char` getter of the bit field. A direct
    bit-field test gives `test byte ptr`.
- **CSoftwareVoice** (vtable 0x012B5C60):
  - SetPositionB 0x00C5A210: two `unsigned long` parameters. An `__int64 == 0` gives `or esi,edx`.
  - SeekBytes 0x00C5A250: the frame size is a named local read before `Reset`.
  - RenderBlock 0x00C5B0F0: virtual slots 25/26; `channels == 1 ? 0 : i`.
- **CSoftwareStreamVoice** (vtable 0x012B5D70):
  - Setup 0x00C5C1B0: two counted loops, which VC merges into 0x7FFF7FFF dword stores. Explicit stores give word
    stores.
  - MixBlock 0x00C5CDC0: MixMono / Mix through an inline ring ReadPointer.
- **CAudioSource::SetPositionB** 0x00C727B0: the OpenAL voice's slot 11, which pauses the source on zero.
- **CAudioRingBuffer::Reset** 0x00C5CEB0: the read and write positions to 0, and the queued count to the capacity.
- **Output drivers.** DirectSound 0x012B8D98, OpenAL 0x012B8ED4 / 0x012B8E20, waveOut 0x012B8FCC, kernel streaming
  0x012B914C and the silent output 0x012B9174 share one slot layout:
  - slot 1: start up;
  - slot 3: initialise with (channels, bytes);
  - slot 6: is a chunk ready;
  - slot 7: the chunk pointer;
  - slot 8: submit.

  Rows:
  - CDSoundStreamBuffer::CreateAndPlay 0x00C57380.
  - CVtable_012b8ed4::Initialize 0x00C57460 and IsChunkReady 0x00C57AF0 (an inline alGetSourcei).
  - CVtable_012b8e20::Create 0x00C57420.
  - CAudioWaveOut::Initialize 0x00C5D5F0 and LockChunk 0x00C5D6A0.
  - CWound_00c5eca0::LockChunk 0x00C5E2B0.
  - CVtable_012b9174::IsChunkReady 0x00C5EE60 and WriteBuffer 0x00C5EEB0.

  A byte bool result after a call (`test al,al ; setne al` or `neg ; sbb al,al ; inc al`) comes from the
  if/return-false form. `return f() != 0;` gives the dword `neg ; sbb ; neg` shape instead.
- **PKRASlaveObject::OnComplete** 0x00C62E40 and its override **CVtable_012b938c::OnComplete** 0x00C62C30: report to
  the master, then `delete this`. The override first does an intrinsic memcpy.
- **CTreePointer_00c555e0** constructor 0x00C6BFF0: an init list of the first and third words.
- **Mixer effects:**
  - CVtable_012bed7c::ClearHistory 0x00C6CAD0: an intrinsic memset, `rep stosd`.
  - The SIMD versions are retail_asm: CVtable_012b8a64::ClearHistory 0x00C6D560 (SSE2) and CVtable_012b8aa0 0x00C6F300
    (MMX).
  - The 128-byte versions are retail_asm too: CVtable_012bedcc::ClearScratch 0x00C701E0 (MMX), CVtable_012bede0
    0x00C70BC0 (SSE2) and the free ZeroBlock128MMX_00c700d0.
  - In every SIMD row the pointer goes through a C++ local, which reproduces retail's `mov [esp+4],eax ; mov edi,[esp+4]`.
  - CVtable_012bedb8::SetParameters 0x00C6EC80: `m_volume != volume` gives retail's load order (argument first).
- **CAudioContextEAX::DisableReverb** 0x00C71820: sets EAX listener ROOM to -10000 through the context's lazily
  resolved pfnEAXSet. The landed destructor 0x00C71CF0 inlines the same body.

## Parked for Fable 5.1 (1)
- 0x00C5E2C0 CWound_00c5eca0::WriteBuffer DIFFER(102v102): `lea ecx,[eax+30h] ; push ecx ; mov ecx,[eax] ; lea edx,[esp+0Ch] ; push edx ; mov edx,[esi+10h]` vs ours `lea edx,[eax+30h] ; push edx ; mov edx,[eax] ; lea ecx,[esp+0Ch] ; push ecx ; ... mov eax,[esi+10h]` (register allocation only) | tried: block local, no local, size local, early pin-handle local, an inline CKsObject::Write wrapper, `&block.Size` as the buffer | next: lane gap: register allocation (likely a compiler-build artefact).

## Skipped
- 0x00C57490: two functions. A 5-byte `mov eax,[ecx] ; jmp [eax+14h]` slot thunk, int3 padding, then
  `lea eax,[ecx+4] ; ret` at 0x00C574A0. The catalog must re-split it.
- 0x00C5EE20: two functions. The same 5-byte slot-5 thunk, then the 0x20-byte slot 3 of 0x012B9174 at 0x00C5EE30
  (`+0x604 = bytes >> 7`, clears +0x608 / +0x60C, returns true). The catalog must re-split it.

## Label corrections
- 0x00C5CEB0 "CThingSearchTools::CThingSearchTools" is CAudioRingBuffer::Reset.
  - Evidence: it is called on the voice ring at +0xB4 from 0x00C5A270.
  - The label was propagated across a byte-identical body.
- 0x00C6BFF0 "CBaseIntelligentPointer::CBaseIntelligentPointer" is the member constructor bound as
  `??0CTreePointer_00c555e0@@QAE@XZ`.
  - It zeroes +0 and +8. The engine CBaseIntelligentPointer constructors zero +0 and +4.
- 0x00C72560 (manifest "CAudioSource::PausePlayback") is a CAudioContext member.
  - It is thiscall on the global context 0x0143BDB0 and resolves "alSourcePause" into +0x74.
  - Named here CAudioContext::SourcePause(unsigned long source).
- 0x00C5A1D0 is landed as a `__fastcall` free function `CTCStealth_OnCreate`.
  - It is the software voice's virtual slot 1 (`ret 4`, one argument, a tail jump to the base slot 1 0x00C596E0).
  - The landed caller 0x00C556E0 binds it as `?Setup@CSoftwareVoice_00c5a1d0@@UAEJPAUCVoiceDesc_00c556e0@@@Z`, which
    0x00C5C1B0 reuses.
- 0x00C573E0, 0x00C56D30, 0x00C55070, 0x00C550D0 and 0x00C72600 are landed as `C3DAnimationUtil` constructors. Those
  are propagated labels of the libpk/GameCODA vtable classes 0x012B8E20, 0x012B8D98, 0x012B8A64, 0x012B8AA0 and
  0x012BEE40.
- The `_gapscan::sub_*` placeholders are named descriptively, as listed below.

## Names
### Proven
These names are fixed by existing landed declarations or reloc bindings at these exact addresses:
- 0x00C57F20 CMixBuffer::MixAddFloat
- 0x00C57F60 CMixBuffer::MixAddFloatGain16
- 0x00C57FC0 CMixBuffer::MixAddFloatGain
- 0x00C58000 CMixBuffer::MixAdd32
- 0x00C58040 CMixBuffer::MixAdd32Gain
- 0x00C580C0 CMixBuffer::MixAdd16
- 0x00C6BFF0 CTreePointer_00c555e0::CTreePointer_00c555e0

### Probable
The class is proven: a landed class owns the vtable, or a file string names it. The method name is mine:
- 0x00C57380 CDSoundStreamBuffer::CreateAndPlay
- 0x00C591C0 CLimitedMixBuffer::WriteChannel
- 0x00C59770 CCodaVoice::CheckPosition
- 0x00C597B0 CCodaVoice::SetPositionC
- 0x00C5A210 CSoftwareVoice::SetPositionB
- 0x00C5A250 CSoftwareVoice::SeekBytes
- 0x00C5B0F0 CSoftwareVoice::RenderBlock
- 0x00C5C1B0 CSoftwareStreamVoice::Setup
- 0x00C5CDC0 CSoftwareStreamVoice::MixBlock
- 0x00C5CEB0 CAudioRingBuffer::Reset
- 0x00C5D5F0 CAudioWaveOut::Initialize
- 0x00C5D6A0 CAudioWaveOut::LockChunk
- 0x00C5E2B0 CWound_00c5eca0::LockChunk
- 0x00C62E40 PKRASlaveObject::OnComplete
- 0x00C71820 CAudioContextEAX::DisableReverb
- 0x00C727B0 CAudioSource::SetPositionB

### Descriptive
The class is named by its vtable, or the function is free with an address suffix:
- 0x00C57420 CVtable_012b8e20::Create
- 0x00C57460 CVtable_012b8ed4::Initialize
- 0x00C57AF0 CVtable_012b8ed4::IsChunkReady
- 0x00C5D490 CVtable_012b5de0::MixAddFloat
- 0x00C5E2C0 CWound_00c5eca0::WriteBuffer (parked)
- 0x00C5EE60 CVtable_012b9174::IsChunkReady
- 0x00C5EEB0 CVtable_012b9174::WriteBuffer
- 0x00C62C30 CVtable_012b938c::OnComplete
- 0x00C6CAD0 CVtable_012bed7c::ClearHistory
- 0x00C6D560 CVtable_012b8a64::ClearHistory
- 0x00C6EC80 CVtable_012bedb8::SetParameters
- 0x00C6F300 CVtable_012b8aa0::ClearHistory
- 0x00C700D0 ZeroBlock128MMX_00c700d0
- 0x00C701E0 CVtable_012bedcc::ClearScratch
- 0x00C70BC0 CVtable_012bede0::ClearScratch

## Callee names
- Reused spellings:
  - CAudioRingBuffer::CommitRead 0x00C5CF20.
  - CSoftwareStreamVoice::MixMono 0x00C5C800 and Mix 0x00C5C880.
  - CSoftwareVoice_00c5a1d0::Setup 0x00C5A1D0 (the existing binding).
  - CAudioContext::GetProcAddress 0x00C4D740.
  - g_AudioContext 0x0143BDB0 (struct; the EAXSet slot is reached as `g_AudioContext.pfnEAXSet`, not as a separate
    global).
  - g_pCodaSystem 0x0143BC2C (the `struct CCodaSystem` spelling of the landed 0x00C5C800; a `class` spelling also
    exists).
- New descriptive spellings (none of these addresses had a binding):
  - CDSoundStreamBuffer::CreateBuffer 0x00C57140.
  - CAudioContext::Initialize(bool) 0x00C565F0.
  - CLimiter::Process 0x00C593C0.
  - AddFloat128BlockSSE2 0x00C5D2E0.
  - CVtable_012bedb8::UpdateGainRamp 0x00C6EBE0.
  - CAudioContext::SourcePause 0x00C72560.
  - PKRACMasterObject::ReceiveObject 0x00C4C7A0.
  - CAudioRingBuffer::Reset 0x00C5CEB0.
  - The import waveOutGetNumDevs (IAT 0x01440498).

## Header debt
None. These classes have no engine headers; `engine/CThingSearchTools.h` exists but 0x00C5CEB0 is not that class.
Every model is local and descriptive.
