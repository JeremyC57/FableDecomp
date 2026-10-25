# Hand chunk 442 (CImaAdpcmCodec .. CEaxContext): 39 of 40 byte-exact

The rows 0x00C5F000..0x00C646E0 are the audio wrapper's support code inside the libpk library. There is no PDB and no
RTTI, so every class and member name is descriptive. The names come from file strings, SDK sources and callers.
Every manifest label in this chunk is a propagated game-side name (see Label corrections).

Grade counts per row are in STATUS.tsv (two rows needed 2 grades, the rest 1). Probes (`probe/pv.py`, many variants
in one compile) were used before each grade.

## What matched (39)

- **CImaAdpcmCodec (7, the DirectX/Xbox SDK imaadpcm.cpp).**
  - The static helpers are `__fastcall` (/Gr TU members declared `static`). WAVEFORMATEX and IMAADPCMWAVEFORMAT are
    `#pragma pack(1)`.
  - 0x00C5F000 CalculateEncodeAlignment: the working value is a signed int (cdq/and 7/sar 3 divisions); only the
    bits-per-sample term is 16-bit.
  - 0x00C5F030 CreatePcmFormat: no cbSize store.
  - 0x00C5F070 CreateImaAdpcmFormat: bytes/sec uses the parameter rate, not a re-read field.
  - 0x00C5F110 IsValidImaAdpcmFormat: the SDK's five early returns.
  - 0x00C5F2D0 EncodeSample / 0x00C5F350 DecodeSample: the SDK bodies.
  - 0x00C5F9E0 Initialize: the converter table `apfnConverter[!!fEncoder][nChannels-1]`, then
    `m_nStepIndexL = m_nStepIndexR = 0` (stores +0x1c first).
- **CEaxContext (software EAX 4.0 context; vtable 0x012B9340), 25 rows.**
  - 0x00C5FF60 RemoveSource: walk with a trailing prev, `break`, unlink, `delete node`.
  - 0x00C609A0 AddSource: `long result = 0` declared first (edi), `new CEaxSourceNode`, -5 on null; the
    `m_sourceFlags & 0x80000000` test gives `test eax,eax; jns`.
  - 0x00C60A30 InvalidateAll: queue a type-2 environment event (0x98 bytes through CPKObject::operator new), set the
    resend flag, then MarkAllDirty each source. SHAPE: the failure exit must be `return result;` inside the loop body.
    `break`, or a loop condition on `result`, splits the live range (144 bytes) or tests at the loop top.
  - 0x00C60930 ~CEaxContext (/EHsc): ClearEvents(), then `node = m_pSources; while (m_pSources) { m_pSources =
    node->m_pNext; delete node; node = m_pSources; }`. Testing the MEMBER while the node is re-read gives retail's
    `mov ecx,[esi+8]; mov eax,ecx; test eax,eax`. Every `while (node)` form gives 96 bytes.
  - 0x00C60450 ClampFloatProperty: positive `>=`/`<=` tolerance relations (`jp`/`jnp`).
  - 0x00C62FF0 GetPropertyHandler (/GS): if/else-if slot GUID chain into one `effect` local, a memcmp `operator==`.
  - Reverb float setters 0x00C633C0 / 0x00C63690 / 0x00C638E0 / 0x00C63A60 / 0x00C63BC0: ClampFloatProperty,
    record arrays, then `float& stored`, `if (stored != value)`.
  - Long setters 0x00C635C0 / 0x00C63810: an inline in-class `ClampLongProperty(long*, ...)`.
  - Pan setters 0x00C63750 / 0x00C639A0: inline `IsAnyNaN`, `EAXVECTOR& stored`, memcmp.
  - Flag setters 0x00C63F90 / 0x00C64020 / 0x00C64380: `unsigned long& flags`, `(flags >> n) & 1` against the long
    argument, and two arms that each commit.
  - 0x00C63D20 SetSlotAirAbsorptionHF: direct member access, no reference local.
  - Context setters 0x00C645D0 SetDistanceFactor, 0x00C64630 SetAirAbsorptionHF, 0x00C646E0 SetHFReference:
    `value != m_context.x`; the commit condition is one `||` chain.
- **IsAnyNaN (1).** 0x00C60B30 IsAnyNaN(const EAXVECTOR&): `return a||b||c;` as bool, which gives `mov eax,1`.
- **CEaxSource (1).** 0x00C61B40 Set(property, value, ppEvents): ClearEvents on the +0x14 context; bit 0x100000 =
  deferred, stripped; then `*ppEvents = m_pContext->m_pEvents`. Both an if/else and a ternary match.
- **CPKArray (2).**
  - 0x00C61B90 ctor: body stores in the order capacity, count, then `new unsigned long[n]` (operator new[]
    0xBFEB22). An init list differs.
  - 0x00C61C00 GetRandom: `(unsigned long)((float)rand() * (float)count * (1/32767.0f))`, `if (index == count)
    index--`. `/ 32767.0f` compiles to the same reciprocal multiply.
- **CPKVoiceHandle (1).** 0x00C61F60 UpdateScaledParameter: emitter value times the owner's ramped scale unless
  unscaled, then voice table slot 25 (handle, value). The float lives in the dead argument slot. The lookup
  0x00C61CF0 is a thiscall member (`mov ecx,esi`).
- **CPKIntVector2 (1).** 0x00C62090 Length: `(float)sqrt((double)(x*x + y*y))`, with intrinsic fsqrt.
- **CPKDecompressedStream (2).**
  - 0x00C626A0 Seek(mode, amount, userData): a fall-through switch, `case 1: flag = true; case 2: ...`, with
    `case 5:` falling into `default:`. The 5-entry table pairing was checked by hand (retail
    0x29/0x2E/0x34/0x39/0x4C = ours).
  - 0x00C62750 ~CPKDecompressedStream (non-virtual): release the decompressor (0x00C73D90) and close the source,
    nulling both.
- **PKRASlaveObject (1).** 0x00C62E60 PassToSlave (/EHsc): chunk 436's PK_REPORT_ERROR macro (".\PKRASlaveObject.cpp"
  line 28, "Null slave object"), then `slave->OnComplete(data, 0)`.
- **GainToMillibels (1).** 0x00C62FB0: `float clamped = gain > 1e-5f ? gain : 1e-5f; return (float)log(clamped) *
  868.589f;`. The two log paths are split, and the floor loads as a float constant.

## Parked for Fable 5.1 (1)
- 00c62370 RandomVectorInSphere DIFFER(126v118): retail takes the result pointer in esi (`mov eax,esi`; the caller
  0x00C625A7 does `lea esi,[esp+14h]`), while ours reads the hidden return pointer from [esp+14h] and spills the radius
  product | tried: a C++ reference body with a same-TU noinline RandomUnit (1 grade) | next: lane gap: LTCG
  register-argument (implicit esi return pointer)

## Skipped
None.

## Label corrections
All manifest labels for this chunk are propagated game-side names:
- 0x00C5F000 Memory_ComputeAllocationSize -> CImaAdpcmCodec::CalculateEncodeAlignment
- 0x00C5F030 WAVEFORMATEX_SetupFromChannels -> CImaAdpcmCodec::CreatePcmFormat
- 0x00C5F070 Audio_AllocateCodecState -> CImaAdpcmCodec::CreateImaAdpcmFormat
- 0x00C5F110 Audio_ValidateCodecState -> CImaAdpcmCodec::IsValidImaAdpcmFormat
- 0x00C5F2D0 Audio_AdjustSampleWithDecay -> CImaAdpcmCodec::EncodeSample
- 0x00C5F350 Audio_MixWithGain -> CImaAdpcmCodec::DecodeSample
- 0x00C5F9E0 Audio_InitializeCodecBuffer -> CImaAdpcmCodec::Initialize
- 0x00C5FF60 CConsoleVariable_Unregister -> CEaxContext::RemoveSource
- 0x00C60450 Float_ClampWithFuzzyComparison -> CEaxContext::ClampFloatProperty
- 0x00C60930 CDrawGuildSeal_Destructor -> CEaxContext::~CEaxContext
- 0x00C609A0 CShadowRendererSceneData_OnHeroStatsUpdate -> CEaxContext::AddSource
- 0x00C60A30 CShadowRendererSceneData_OnAnimationLoaded -> CEaxContext::InvalidateAll
- 0x00C60B30 Math_IsAnyNaN -> IsAnyNaN(const EAXVECTOR&) (right in substance)
- 0x00C61B40 CDrawGuildSeal_Render -> CEaxSource::Set
- 0x00C61B90 CArrayBase_Initialize -> CPKArray::CPKArray (a constructor)
- 0x00C61C00 CArrayBase_GetRandomElement -> CPKArray::GetRandom
- 0x00C61F60 CRegion_UpdateMouseCursorPosition -> CPKVoiceHandle::UpdateScaledParameter
- 0x00C62090 Math_Vector2Length -> CPKIntVector2::Length (integer components)
- 0x00C62370 Quaternion_FromAxisAngle -> RandomVectorInSphere (no quaternion: three floats from rand/acos/sin/cos)
- 0x00C626A0 CWeaponVisualState_SetProperty -> CPKDecompressedStream::Seek
- 0x00C62750 IUnknown_Release -> CPKDecompressedStream::~CPKDecompressedStream
- 0x00C62E60 PKRASlaveObject_ExecuteAction -> PKRASlaveObject::PassToSlave (vtable 0x012B93D4 slot 2)
- 0x00C62FB0 ScaleValueByConstant -> GainToMillibels
- 0x00C62FF0 Audio_MatchFormatGuid -> CEaxContext::GetPropertyHandler
- 0x00C633C0..0x00C646E0 HeroAttribute_* / SetAttribute_* -> the CEaxContext EAX 4.0 reverb and context setters
  (names in STATUS.tsv)
