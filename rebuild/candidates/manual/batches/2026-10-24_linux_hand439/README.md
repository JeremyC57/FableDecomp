# Hand chunk 439 (CPtrArray .. PKCReadRequestQueue): 39 of 40 byte-exact

All 40 rows are `_global` catalog rows in the libpk audio/ambient middleware (no PDB/RTTI), so every class and
function name is a descriptive name. The names come from the TU strings ("Ambient cube:-", "   Apex (", the
PKCTRList.h / PKCStaticQueue asserts), the vtables (audio object 0x012B5A88 with its 2D/3D vtables 0x012B7B88 /
0x012B5AB0, the region list 0x012B7DF8, the ambient lists 0x012B7F88 / 0x012B7FD0, the queues 0x012B8024 / 0x012B80C8 /
0x012B8364) and the landed callers and callees. The result is 5 MATCH and 34 RELOCATION_MATCH. Every matched row has a
test that prints PASS under g++ (tall.sh) and under the lane's VC7.1 (tall.py). Every `// Leaf:` was checked against
`objdump -t` (one defined symbol each), and every new/new[]/delete/delete[]/ceil/_ftol2 call target was checked
against retail's import address.

## What matched (39)

- **CPtrArray<CTableEntry\*>::~CPtrArray** (0x00C42AD0): the out-of-line template destructor. It is
  `for (count = m_count; count; count = m_count) Remove(m_data[count-1]);` with Remove inlined, then
  `delete[] m_data` (import 0x00BFEB1C). `while (m_count)` re-reads the count and swaps edx/edi.
- **CAudioObject** (libpk audio object, base vtable 0x012B5A88; flags +0x10, voice +0x50, queue +0x54, current
  buffer +0x78, callback +0xDC/+0xE0, status +0xF0):
  - SetFilterParams 0x00C43000 (MATCH): needs a `__forceinline` record operator==. It compares words 0,1,2,0,4,5,
    exactly as retail does.
  - CanAcquireVoice 0x00C43390: the streamed bit is a shift accessor `(m_state>>2)&1`, not a bit-field. The late
    failure returns false with no second SetError, and `if (x) return true; return false;` avoids `xor eax,eax`.
  - CanUseHardwareVoice 0x00C43530: `if (caps & ~supported) return false; return true;` gives the byte
    `sbb al,al ; inc al`.
  - SetState 0x00C43570: the argument is masked first, then a three-case switch, then the
    `(flags & ~0x7F) | state` store, then the callback.
  - Reset 0x00C436C0: one `if (flags & 0xffff)` block. `&= 0xFFFF0000` is the `mov word ptr` store, and the status
    store comes after the block.
  - SetLoopPoints 0x00C43780 and SetVolumeParams 0x00C437A0 (slots 6 and 7): three-push forwarders.
  - CanPlay 0x00C43910 and AcquireVoice 0x00C43DE0 (MATCH): the buffer is chosen by `capacity ? Peek() : 0`, and
    Peek repeats its own test. AcquireVoice ends in the slot-8 CreateVoice, which VC7.1 tail-duplicates.
  - Stop 0x00C43980: the voice's slot-11 Stop, the state 1 and 8 exits, the uncount, then state 8 and the callback.
  - CanUseBuffer 0x00C43D30: CanUseHardwareVoice is an in-class inline here, with one shared failure block.
  - SetBuffer 0x00C43E20: ReleaseQueue, Reset, CanUseBuffer, CreateHardwareBuffer, AddRef, then Rebind or release.
- **CircularQueue::DequeueNext** (0x00C44320): `if (m_capacity == ++m_tail)`. The other operand order swaps the cmp.
- **C2DAudioObject** (2D vtable 0x012B7B88):
  - InitSpeakerMaps 0x00C448D0: both speaker loops are written out. A shared inline helper is 104 bytes.
  - Open 0x00C449C0 (slot 3): the memset intrinsic, then an if/else level. The ternary puts caps in edi, not ebx.
    Then the software-preallocation branch and `return InitSpeakerMaps()`.
- **CAmbientEmitter** (ReleaseSounds 0x00C44B80, ProcessEvents 0x00C44BD0, AcquireSounds 0x00C44CE0): the TU flag is
  `/Oa`; without it ProcessEvents and AcquireSounds rotate their registers. AcquireSounds reads the id through the
  array's by-value `GetAt` inline (or a named local).
- **CAmbientCube**:
  - UpdateDistances 0x00C45810: the difference must be a named local, so GetMagnitude gets `lea ecx`.
  - SetCentre 0x00C458D0: a countdown of 8 over 0x1C-byte apexes.
  - GetLevel 0x00C45940: the ramped-parameter GetValue inline with early returns.
- **Region map family**:
  - The CRegionSnapshot ctor 0x00C46400 (/EHsc): `new SRegionEntry[n]` plus a stack CRegionCopier visitor.
    `operator new[]` must be declared, or VC7.1 calls the scalar `??2` (0x00BFEA1A) instead of retail's 0x00BFEB22.
  - The CRegionMap dtor 0x00C46860 (/EHsc): `while ((node = m_regions.PopFirst()) != 0) delete node;`. The PopFirst
    inline stops the loop rotation, and the node allocator is a member (as a base, VC emits a CRegionMap vptr store).
  - The CGridCircle ctor 0x00C46900: `(long)ceil(radius * 1.1f)` with ceil declared
    `extern "C" __declspec(dllimport)` (`ff 15`), then nested loops over C2DVectorI cells, testing `GetLength() <= radius`.
  - The `__fastcall` operator< for SRegionLookupKey 0x00C466C0 and for C2DVector 0x00C46E20 (MATCH), and
    C2DVector::GetLength 0x00C46E90 (MATCH).
- **CAmbientSource** (ctor 0x00C47340, updated through the `__stdcall` forwarder 0x00C37970):
  - ProcessTriggers 0x00C46F90: the emitter's event loop without the muted test.
  - CreateTriggers 0x00C47050 and CreateChannels 0x00C470E0 (/EHsc): the ids come through the by-value `GetAt`, and the
    trigger/channel constructors are out of line (an inline trigger ctor gets inlined).
  - Update 0x00C47300: plain calls, with Finish as a sibling `jmp`.
- **Async decoder and streamer**:
  - The SDecoderMessage ctor 0x00C48F30: words +8..+0x10 are an embedded record with its own inline zeroing ctor.
  - The PKCMessageQueue ctor 0x00C48F80 (/EHsc): new[] plus the eh vector ctor iterator.
  - CCodaFrame::Silence 0x00C49180 (MATCH): the memset intrinsic.
  - CLockedRingBuffer::Push 0x00C49D50 (/EHsc): an early return inside the lock's scope builds the EH frame.
  - CLockedRingBuffer::GetHeadIndex 0x00C49DD0.
  - The PKCFrameQueue ctor 0x00C49E00 (declared `operator new[]`, as retail) and the PKCReadRequestQueue ctor
    0x00C4BE80 (scalar new, as retail).

## Parked for Fable 5.1 (1)

- 00c42f50 CAudioObject::SetGain DIFFER(89v96): retail rounds `2000*log10(gain)` with a bare `fstp [esp+8] ; fld [esp+8] ; fistp dword ptr [esp]` (plus a `push ecx` frame word) where C++ `(long)x` calls `__ftol2` | tried: `(long)` cast through a static inline FloatToLong (1 grade) | next: lane gap: retail_asm (inline-asm fistp float-to-long helper); land as grade retail_asm with a minimal `fld/fistp` `__asm` and a `// Retail-asm:` line

## Skipped

None.

## Label corrections

These are propagated manifest labels. Each correct name is the descriptive name in the source's line 1.

| Address | Manifest label | Correct name |
|---|---|---|
| 0x00C42AD0 | Container_RemoveLastElement | CPtrArray<CTableEntry*>::~CPtrArray |
| 0x00C42F50 | Audio_SetDecibels | CAudioObject::SetGain (linear gain, plus a derived centibel volume) |
| 0x00C43000 | Audio_SetFilterParams | CAudioObject::SetFilterParams |
| 0x00C43390 | NCreatureMode_EvaluateAnimState | CAudioObject::CanAcquireVoice |
| 0x00C43530 | GetDrawableStateFlags | CAudioObject::CanUseHardwareVoice |
| 0x00C43570 | CCombatRenderState_SetStateValue | CAudioObject::SetState |
| 0x00C436C0 | Thing_Cleanup | CAudioObject::Reset |
| 0x00C43780 | CMusicManager_SetLoop | CAudioObject::SetLoopPoints |
| 0x00C437A0 | CMusicManager_SetVolume | CAudioObject::SetVolumeParams |
| 0x00C43910 | GetDrawableShadowParameters | CAudioObject::CanPlay |
| 0x00C43980 | CCombatRenderState_OnActionComplete | CAudioObject::Stop |
| 0x00C43D30 | CShaderConstantRange_ShouldClearPrimitives | CAudioObject::CanUseBuffer |
| 0x00C43DE0 | DrawAutoPickups_SelectAndDrawPrimitive | CAudioObject::AcquireVoice |
| 0x00C43E20 | CExclusiveAssert_UpdateSelectedWeapon | CAudioObject::SetBuffer |
| 0x00C448D0 | CCreatureAbility_InitializeFlagScales | C2DAudioObject::InitSpeakerMaps |
| 0x00C449C0 | CTCHeroStats_ProcessAction | C2DAudioObject::Open |
| 0x00C44B80 | CWorldPartition_RenderComponents | CAmbientEmitter::ReleaseSounds |
| 0x00C44BD0 | CThingPlayerCreature_ProcessMovementBands | CAmbientEmitter::ProcessEvents |
| 0x00C44CE0 | World_RegisterResourceStrings | CAmbientEmitter::AcquireSounds |
| 0x00C45810 | CParticleMorphs_FindMinimumDistance | CAmbientCube::UpdateDistances |
| 0x00C458D0 | CQuadric_AccumulateAll | CAmbientCube::SetCentre |
| 0x00C45940 | CTCWeapon_GetDamageValue | CAmbientCube::GetLevel |
| 0x00C46400 | CMap_AllocateWaterGrid | CRegionSnapshot::CRegionSnapshot |
| 0x00C466C0 | ComparisonSort_FloatCompare | operator<(SRegionLookupKey) |
| 0x00C46860 | Manager_UnwindCleanupObjects | CRegionMap::~CRegionMap |
| 0x00C46900 | CGuiVarTransfer_IterateRange | CGridCircle::CGridCircle |
| 0x00C46E20 | ComparisonSort_FloatPairComparator | operator<(C2DVector) |
| 0x00C46E90 | Math_VectorMagnitude | C2DVector::GetLength |
| 0x00C46F90 | CGame_ProcessMovementLoop | CAmbientSource::ProcessTriggers |
| 0x00C47050 | CGame_CloseInputStreams | CAmbientSource::CreateTriggers |
| 0x00C470E0 | CGame_CreateSolidToThingFilters | CAmbientSource::CreateChannels |
| 0x00C47300 | CTCOnHeroPush_ProcessStateTransition | CAmbientSource::Update |
| 0x00C48F30 | CBaseIntelligentPointer_Constructor | SDecoderMessage::SDecoderMessage |
| 0x00C48F80 | CScriptDefinitionManager_AllocateElementArray | PKCMessageQueue::PKCMessageQueue |
| 0x00C49180 | CTCActivationReceptor_ClearMemory | CCodaFrame::Silence |
| 0x00C49D50 | RingBuffer_PushLocked | CLockedRingBuffer::Push |
| 0x00C49DD0 | RingBuffer_GetCountLocked | CLockedRingBuffer::GetHeadIndex (returns the head slot index, not a count) |
| 0x00C49E00 | LocalDetailPrimitive_AllocateArray | PKCFrameQueue::PKCFrameQueue |
| 0x00C4BE80 | PKCBufferedReader_SetBufferSize | PKCReadRequestQueue::PKCReadRequestQueue |
| 0x00C44320 | CircularQueue_DequeueNext | Kept. It is the CAudioObject's buffer queue (+0x54) DequeueNext. |
