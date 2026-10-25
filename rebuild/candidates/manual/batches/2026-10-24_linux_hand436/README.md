# Hand chunk 436 (CHandleBasedMemoryPool .. CBankProcessor): 36 of 40 byte-exact

Region 0x00C28B80..0x00C2EA70 is the libpk audio library (the second build of the handle-based memory pool, steering
groups, PKCSemaphore, the async thread entry, several keyed registries, the music CInstanceManager, CAudioFormat, the
segment file and the sound-bank processor). No PDB or RTTI covers it, so every name below is descriptive unless noted.
Containers are VC7.1 Dinkumware in `std` (chunk 433), not STLport. All 36 src/ rows re-graded MATCH / RELOCATION_MATCH
through gall.sh; all 36 tests print PASS under g++ (tall.sh) and VC7.1/Wine (tall.py). The two int3-after-noreturn
tests in differ/ also pass under both.

## What matched (36)
- **CHandleBasedMemoryPool** (the 0x00C27xxx build, FailureHandler +0x38, Dinkumware handle vector +0x43c)
  - 00c28c20 `Allocate(long*, long)`: the failure-handler retry loops, two PartialDefragment rounds, then
    `InternalAllocate`. Write `if (EnableFullDefragIfPartialFails) Defragment(); else return 0;`. This moves the
    `return 0` block to the function end, where it merges with the first `return 0`.
- **std helpers for the pool's vectors**
  - 00c29250 `std::copy_backward<CHandleInfo*>`: a `__fastcall` template with the `_Nonscalar_ptr_iterator_tag` loop,
    explicitly instantiated.
  - 00c29290 `copy_backward<CHeader**>`: the scalar `memmove` form.
  - 00c294b0 `std::_Uninit_fill_n<CHandleInfo*, unsigned int, CHandleInfo, allocator<CHandleInfo> >`: the
    non-scalar tag, with try/catch, under /EHsc.
- **CSteeringGroup** (vtable 0x012B40A4)
  - 00c29ee0 `RemoveAgent(world, agent)`: unlinks the agent through +0x34. When the group empties it notifies the
    world listener (+0x104, slot 8), sets `m_index = -1` and hands the group to `world->m_groups.AddGroup` (0x00C222B0).
  - 00c29f50 the constructor: `m_index(-1), m_pFirst(0)` plus an empty inline destructor.
- **PKCSemaphore** (`.\PKCSemaphore.cpp`): 00c29fc0 ctor, 00c2a090 `Wait`, 00c2a150 `Release`. All are /EHsc and
  matched on the first grade.
  - The libpk error macro is a CDebugStream over the nil buffer 0x0139C900, written as
    `file << "(" << (long)line << ") : " << text << "\n"`. Then comes the report hook 0x0143BB2C and the stream
    destructor, and last a `CDebugBreak()` temporary homed in a dead argument slot.
- **AsyncThreadProc** 00c2a210: a `__stdcall` thread entry, `while (!launcher->Process());` then
  `launcher->m_finished.Release()`.
- **CResourceRegistry** (00c2a300 SetEntryValue, 00c2a320 HasEntry, 00c2a640 ReleaseOwnerEntries),
  **CInstanceRegistry** 00c2bc10 DestroyOwnerInstances and **CSharedEntryTable** 00c2a3d0 Acquire.
  - The libpk keyed lists are polymorphic with Find at slot 17, taking `const unsigned long&`.
  - The `(unsigned long)ptr` conversion temporary lives in the dead argument slot and is re-stored before each Find.
  - `HasEntry` gives `neg ; sbb ; neg`.
- **CHeightSource::Evaluate** 00c2a390: forwards an `SPosition2D` by value to the provider's slot 0, else returns `0.0f`.
- **CNavigatorSwitch::SendState** 00c2a7a0 (/EHsc): builds an 8-byte message (vtable 0x012B4178) from the switch's own
  state byte.
  - With the message constructor's body in the same TU (`auto_inline(off)`), VC keeps `this` in edx across it.
  - An `unsigned char` parameter pushes `xor eax,eax ; mov al,[m] ; push eax`; an int parameter gives `movzx`.
- **CFlagOverride::ApplyTo** 00c2c670: two masked setters, group A then group B.
  - `unsigned short` arguments push as `xor eax,eax ; mov ax,[m]`.
- **CInstanceManager** (`.\CInstanceManager.cpp`): 00c2d160 StartIdleInstances, 00c2d210 FadeOutAll,
  00c2d420 GetCurrentInfo, 00c2d490 Update, 00c2d520 FindQuietestInstance.
  - GetCurrentInfo needs a same-TU `GetCurrentInstance` (0x00C2D2B0) under `auto_inline(off)` so that ecx survives.
    The if/else form that falls through to one shared `return 1` keeps the branch; an early `return 1` gives setne.
  - FindQuietestInstance inlines the ramped parameter's angle wrap.
- **CAudioFormat**
  - 00c2d790 GetSamplesPerBlock and 00c2d7d0 GetBlockAlign: a switch on the codec word lowers to
    `test ; je ; jle ; cmp 2 ; jg`.
  - 00c2d890 BytesToSeconds and 00c2d970 GetBytesPerSample: with the callee bodies in the same TU, VC keeps ecx, edx and
    the x87 stack live across the calls. Each unsigned-to-float conversion takes the `fild` + 2^32 fixup.
- **CSegmentFile::OpenSegment** 00c2d9c0: segment table Find (slot 17, by value), then `SetRange(...) >= 0 ? 0 : -1`.
- **CBankProcessor**, the libpk sound-bank loader. Its ctor is 0x00C2E0C0, 0x00C2EB00 runs its steps and 0x00C16D5A
  calls that. Layout: data +0, manager +4, log +8, driver map +0xC, sample map +0x3C, criteria-trigger registry +0x6C,
  number-trigger registry +0x70, option bytes +0x74..+0x7A.
  - 00c2e190 AddSample: WAVE format tag 1/2/0x69 maps to codec 0/2/1; any other tag logs "Unknown sample format for
    index ". The inline CAudioFormat constructor stores the codec word before the channel word, so it assigns in that
    order in its body. The created sample is a named local, so the index is read after CreateSample.
  - 00c2e480 AddDrivers, 00c2ea30 AddEvents and 00c2ea70 AddSubEvents: the landed 0x00C2E270 loop shape over the lists
    at +0x10 / +0x20 of the data object.
  - 00c2e4d0 AddNumberTriggers: the 8-byte trigger reference is a loop local whose address is taken. This gives
    retail's `and esp,-8` aligned frame, with ebp reused as the index, on the first grade.
  - 00c2e5a0 AddDriverToGroup: find the group or create it, then add the driver.
  - 00c2e8d0 AddEvent and 00c2e960 AddSubEvent, both /EHsc.
    - The "_SUB" test 0x00C2ECD0 needs a same-TU body. It calls only CRT functions, so VC knows it cannot throw and
      stores no EH state around it (AddEvent).
    - It must have external linkage. As a `static` function, VC7.1 gave it a custom convention, with the argument in
      eax.

## Parked for Fable 5.1 (4)
- 00c28b80 CHandleBasedMemoryPool::InternalAllocate DIFFER(145v147): the handle-slot store is retail `lea eax,[edx+eax*4] ; mov [eax],edi` against ours `mov [edx+eax*4],edi`; everything else matches | tried: a slot reference local, `Handles[*handle].Header = area`, an inline SetHeader helper (4 grades) | next: index through a Dinkumware `vector<CHandleInfo>::operator[]` written as `*(begin() + pos)`; else lane gap: RTM vector-slot address materialisation (GOTCHAS wave 24l)
- 00c28dc0 std::vector::_Xlen DIFFER(113v112): int3-after-noreturn. Every instruction matches; VC7.1 owns the 0xCC after the final `call _CxxThrowException` that the cutter stripped | tried: chunk 433's 0x00C13840 model (1 grade) | next: land against the 113-byte span (differ/00c28dc0.cpp + .test.cpp, PASS under g++ and VC)
- 00c28e50 std::vector::_Xlen DIFFER(113v112): int3-after-noreturn, the vector<CHeader*> twin of 0x00C28DC0 | tried: the same model (1 grade) | next: land against the 113-byte span (differ/00c28e50.cpp + .test.cpp, PASS under g++ and VC)
- 00c2cbd0 CEventScript::CEventScript DIFFER(132v132): scheduling only. Retail loads the owner argument (`mov eax,[esp+18h]`) after `push esi ; mov esi,ecx ; mov [esp+4],esi`; ours loads it (`[esp+14h]`) before `push esi` | tried: owner through the inlined root constructor's init list, and assigned in the body (2 grades) | next: lane gap: scheduling (compiler-build artefact)

## Skipped
None. No row spans two functions; the row cuts agree with the bodies, apart from the two int3-after-noreturn rows above.

## Label corrections
Every catalog label in this chunk is propagated or guessed. The names used are descriptive (libpk has no PDB):
- 0x00C28B80 `CHandleBasedMemoryPool_AllocateHandleFromFreeArea` is `CHandleBasedMemoryPool::InternalAllocate`. Its
  0x00A0xxxx twin carries the Ego_r.pdb name `?InternalAllocate@CHandleBasedMemoryPool@@AAE_NPAJK@Z`.
- 0x00C28C20 is `CHandleBasedMemoryPool::Allocate`.
- 0x00C28DC0 / 0x00C28E50 `std_vector_RaiseOverflowException` are `std::vector<CHandleInfo>::_Xlen` /
  `std::vector<CHeader*>::_Xlen`.
- 0x00C29250 `_Copy_backward<CPixelFormat*>` / 0x00C29290 `_Copy_backward<EWeaponClass*>` are
  `std::copy_backward<CHandleInfo*>` / `<CHeader**>`.
- 0x00C294B0 `_Fill_n<unsigned long*>` is `std::_Uninit_fill_n<CHandleInfo*, ...>` with the non-scalar tag.
- 0x00C29EE0 `LinkedList_RemoveNode` is `CSteeringGroup::RemoveAgent`.
- 0x00C29F50 `Ctor_Init_012b40a4` is `CSteeringGroup::CSteeringGroup`.
- 0x00C29FC0 / 0x00C2A090 / 0x00C2A150 are `PKCSemaphore::PKCSemaphore` / `Wait` / `Release`. The class name comes
  from the file string.
- 0x00C2A210 `SpinWaitUntilCondition` is `AsyncThreadProc`.
- 0x00C2A300 `CTCMapwho_SetPropertyFromMessage` is `CResourceRegistry::SetEntryValue`.
- 0x00C2A320 `CTCMapwho_HasContainmentVolume` is `CResourceRegistry::HasEntry`.
- 0x00C2A390 `Thiscall_GraphicsCallbackEvaluate` is `CHeightSource::Evaluate`.
- 0x00C2A3D0 `CWStringParser_FindOrCreate` is `CSharedEntryTable::Acquire`.
- 0x00C2A640 `CTCParticleAttacher_ProcessAllMessages` is `CResourceRegistry::ReleaseOwnerEntries`.
- 0x00C2A7A0 `LoadSea_EnableNavigator` is `CNavigatorSwitch::SendState`.
- 0x00C2BC10 `CTCMapwho_DeleteAllInstances` is `CInstanceRegistry::DestroyOwnerInstances`.
- 0x00C2C670 `CClothingItem_ValidateGeometry` is `CFlagOverride::ApplyTo`.
- 0x00C2CBD0 `CQ_GuildTrainingPreMeleeScript_Initialize` is `CEventScript::CEventScript`.
- 0x00C2D160 `Fastcall_FindAnimationTriggers` is `CInstanceManager::StartIdleInstances`.
- 0x00C2D210 `CTCCreature_InitializeAnimationNames` is `CInstanceManager::FadeOutAll`.
- 0x00C2D420 `CNavSwitchableLeafNode_GetDrawEnvironment` is `CInstanceManager::GetCurrentInfo`.
- 0x00C2D490 `OpenInventory_ProcessAnimationFilters` is `CInstanceManager::Update`.
- 0x00C2D520 `FindMaxAnimationEntry` is `CInstanceManager::FindQuietestInstance`.
- 0x00C2D790 `Shield_GetBlockBonus` is `CAudioFormat::GetSamplesPerBlock`.
- 0x00C2D7D0 `GetCombatDamageModifier` is `CAudioFormat::GetBlockAlign`.
- 0x00C2D890 `Traits_CalcBlockDamageReduction` is `CAudioFormat::BytesToSeconds`.
- 0x00C2D970 `InventoryItem_CalcBlockDamageReduction` is `CAudioFormat::GetBytesPerSample`.
- 0x00C2D9C0 `Accessor_GetAndSetEnvironmentBounds` is `CSegmentFile::OpenSegment`.
- 0x00C2E190 `C3DAnimationSequence_ProcessSampleFormatChunk` is `CBankProcessor::AddSample`.
- 0x00C2E480 `ProcessResourceChunksUntilQuickAssignFails` is `CBankProcessor::AddDrivers`.
- 0x00C2E4D0 `EventTrigger_ProcessDrivers` is `CBankProcessor::AddNumberTriggers`.
- 0x00C2E5A0 `EventTrigger_ProcessSingleDriver` is `CBankProcessor::AddDriverToGroup`.
- 0x00C2E8D0 `AnimationEvent_Process` is `CBankProcessor::AddEvent`.
- 0x00C2E960 `AnimationEvent_ProcessMapping` is `CBankProcessor::AddSubEvent`.
- 0x00C2EA30 `ProcessAllAnimationEvents` is `CBankProcessor::AddEvents`.
- 0x00C2EA70 `ProcessAllAnimationEventMappings` is `CBankProcessor::AddSubEvents`.
