# Hand chunk 438 (CResourceMap_00c372c0 .. CShuffleBag_00c42a70): 34 of 40 byte-exact

All 40 rows (0x00C372C0..0x00C42A70) belong to libpk, the audio middleware library. The trace strings name its
headers (`d:\dev\audio\ver_fable_1.5_b\libpk\PKCMailbox.h`, `PKCAutoDeleteArray.h`, `.\PKSerialiseCParser.cpp`,
`.\CInstance.cpp`, `.\PKCFile.cpp`) and the error-code table (`C_ERR_*`). libpk does not end inside this chunk:
the same strings continue past 0x00C4C000 (`PKCBufferedReader.cpp`, `PKRACMasterObject.cpp`, the OpenAL
`al*`/`alc*` imports). There is no PDB and no RTTI, so every class name is descriptive and suffixed with the
address. Every manifest label in this chunk is propagated from an unrelated game function, and all of them are
wrong (see Label corrections). Grades: 30 RELOCATION_MATCH, 4 MATCH, 6 DIFFER parked (3 of them retail_asm
candidates); none skipped. Behaviour tests pass under g++ and VC7.1 (tall.sh, tall.py) for all 34 matched rows.

## What matched (34)

- **Registered entries and resource maps** (owner/registry pairs, `/EHsc` TUs):
  - 00c372c0 `CResourceMap_00c372c0` constructor. Stores owner and data, zeroes +8/+0xc, builds the container
    at +0x10 inline (root over its embedded node, vtable 0x012B51F0, node `CNode(0)` @ 0x00C194F0), zeroes three
    words, sets the +0x4c flag and registers with the owner's registry at +8 (slot 9). It is the counterpart of
    the landed destructor 0x00C37130.
    Shape: the three zero words at +0x40 are a sub-object with an inline zeroing ctor. Plain member stores let
    VC7.1 hoist the owner load above them.
  - 00c37990 `CEntryHost::CreateEntry`. Shape: a VOID function that discards `new T(this, key)`, because the
    entry's constructor registers itself. `return new T(...)` gives the null arm its own `xor eax,eax` epilogue
    (102 v 82); retail's `je` falls into the shared epilogue.
  - 00c37b90 `CEntryHost::DestroyEntry`. Shape: an inline by-value `Find(void* key) { return Lookup(&key); }`
    reproduces retail's self-copy of the argument slot. `delete` goes through the non-virtual out-of-line dtor.
- **Sound loops and emitters**:
  - 00c377f0 `CSoundLoop::DropFinishedVoice`. Sound-system slot 28 GetVoicePosition(handle, &status).
  - 00c37930 `~CSoundLoop`: drop a finished voice, stop a held one (slot 22), unregister (owner+0x38, slot 8).
  - 00c37ad0 `CSoundEmitter::SyncVoice3D`. Passes the flagged scalar/vector to the device's slot-0 update and
    copies them back. Shape: `flag ? &local : 0` gives neg/sbb/and. The 3-vector needs a memberwise inline
    `operator=`; the implicit struct copy goes through lea+ebp (200 v 192).
- **Registry layers** (one level above the landed 0x00C2C7B0):
  - 00c383d0 `CRegistryLayer::Add` (parent Add, then the own list at +0x40, return the item).
- **Ramped parameters**:
  - 00c38510 `CRampedParamOwner::GetValue` (angle-wrapped value).
  - 00c3a8f0 `CRampedParam::IsSettledAtZero`. Shape: written `if (v == 0.0f) return 1; return 0;` so that
    retail's 1-exit stays on the fall-through.
  - 00c3a950 `CInterpolatedFloat::SetTarget(target, time)`, which forwards with a zero rate.
- **Index maps**:
  - 00c3ab50 and 00c3b0e0 `CIndexMap::Add`, which logs "Index n already exists!" through the libpk CString
    `operator<<`.
  - 00c3abd0 and 00c3b160 `CIndexMap::Get`. Shape: `if (!node) return 0; return node->m_pValue;`, because the
    ternary adds an `xor eax,eax`.
- **Mailboxes and streaming**:
  - 00c38a40 `CStreamPlayer::ReceiveStatus`. Two by-value PKCMailbox `Read()`s. Shape: one `||` puts the shared
    false block at the end of the function.
  - 00c427d0 `CPKRiffReader::FindChunk`, the RIFF sub-chunk walk with even padding.
- **Serialisation** (PKSerialiseCParser):
  - 00c3b7a0 `CBankCriteria::Serialise`. The count local lives in the dead parser argument slot, and the read
    arm falls through.
  - 00c3b850 `CBankSampleInfo::Serialise`. Shape: one `return a && b && ...;` chain gives the int-width
    `mov eax,1` tail.
- **Sound buffers**:
  - 00c3c750 `CreateHardwareBuffer`. Shape: a `long result = 0` returned once, so the xor comes before the
    first test.
  - 00c3c7a0 `SetupEventNotification`: a qualified base call, then the rebuild or the drop of the device buffer.
- **Two-chain node pool** (20 bytes: owner, active head, free head, free count with negative meaning "no
  recycling", active count):
  - 00c3c8c0 `Init`. Shape: `long n = count; while (--n)` gives dec/je plus dec/jne.
  - 00c3c930 `Clear`. Shape: advance the list through the member (`m_pActiveHead = m_pActiveHead->m_pNext`).
    Advancing through the local lets VC drop the null test of `delete`.
  - 00c3c9e0 `RemoveActive` and 00c3ca50 `RemoveFree`.
  - 00c3cde0 `RecycleGlobalNode`, on the global pool at 0x0143BC00.
  - 00c3d660 and 00c3d6a0 `CAudioEventManager::DiscardMorphEvent` / `DiscardScreenEvent`.
- **Audio device and output**:
  - 00c3cfe0 `GetElapsedTicks` (QueryPerformanceCounter, start count at +0x190).
  - 00c3e760 `CAudioDevice::BeginFrame`. Shape: the flag at 0x0143BC14 is a FILE-STATIC bool (an extern blocks
    the hoist). The `&&` sits in a nested `if (s)`, and the scratch clear is an inline member `Clear()`.
  - 00c3e800 `UpdateMeters`: memsets the meters once, then Process 0x00C547D0.
  - 00c3d710 `CAudioOutput::ApplySpeakerMode`. A compare chain; the case order in the source sets the block
    order (8, 0, 0x80, default, then 0x40/0x10000/0x20000).
  - 00c3f280 `GetFallbackSpeakerMode`, a jump table. The case-to-target pairings were checked against the retail
    table by hand.
- **Vectors**:
  - 00c402e0 `CPKVector3::Normalise`. Shape: the sum is written `z*z + y*y + x*x`, and the unit test is
    `1.0f != length` (`fld st(0) ; fld [one]`).
- **Shuffle bag**:
  - 00c42a70 `CShuffleBag::PickNext`. Shape: `index = Random(n - 1) + 1;` used twice gives the folded `+4` load
    and then `inc eax` for the store.

## Parked for Fable 5.1 (6)

- 00c37c40 CSoundEmitter_00c37c40::CreateEntriesForSource DIFFER(78v75): retail keeps `this` in edx across the out-of-line visitor ctor call (`mov edx,ecx ; push edx ; call 0xC37BD0 ; mov edx,[edx+8]`), ours saves it in esi (push/pop esi) | tried: declaration-only ctor, same-TU `__declspec(noinline)` ctor body | next: lane gap: LTCG register allocation (the callee's clobber set)
- 00c3ff70 _global::RoundHalfDownToLong DIFFER(21v32): bare `fld dword [value] ; fistp dword [esp]` after `value -= 0.5f` is stored back to the argument slot | tried: C++ reference body (truncating cast) | next: retail_asm candidate (inline-asm fistp)
- 00c40ee0 _global::MillibelsToAmplitude DIFFER(38v109): hand-written x87 2^x block (`frndint ; fst ; wait ; fld1 ; fscale ; ... ; f2xm1 ; fadd ; fmulp ; fstp ; wait`) after the C prologue | tried: C++ reference body (`pow(10, mB/2000)`) | next: retail_asm candidate (inline-asm 2^x)
- 00c40f50 _global::AmplitudeToMillibels DIFFER(61v64): bare `fld dword [value] ; fistp dword [esp]` after the intrinsic `log10(x) * 2000` is stored back to the argument slot | tried: C++ reference body (truncating cast) | next: retail_asm candidate (inline-asm fistp)
- 00c418f0 CPKListener_00c418f0::NormaliseCopy DIFFER(160v160): retail `fsqrt ; fstp st(3) ; fstp st(0) ; fstp st(0) ; fstp [esp+4]`, ours `fsqrt ; fstp [esp+4]` then three pops | tried: implicit copy (168v160), memberwise operator=, sqrt written inline, QFE-4035 probe | next: lane gap: x87 spill scheduling of a float live across a call
- 00c425e0 _global::DecodeImaAdpcm DIFFER(221v217): register colouring: retail channels/blocks/size in esi/edi/ebx (edi pushed after the `div`, `cmp ebx,[esp+60h]`), ours edi/ebx/esi with the capacity loaded into eax | tried: `channels * 128 * blocks`, `blocks * (channels * 128)` (both reassociate to imul-then-shl), `int channels`, `(channels << 7) * blocks` (restores shl-then-imul) | next: a named `bytesPerBlock` local, or the size from a same-TU noinline helper

## Skipped

None. No row holds two functions.

## Label corrections

All 40 manifest labels are propagated. The descriptive names used here (descriptive name: libpk has no PDB/RTTI):

| address | manifest label | name used |
|---|---|---|
| 00c372c0 | PropertyContainerNode_Construct | CResourceMap_00c372c0 constructor (counterpart of dtor 0x00C37130) |
| 00c377f0 | Fastcall_PlayerActivityCheck | CSoundLoop::DropFinishedVoice |
| 00c37930 | Fastcall_PlayerModeDeactivate | CSoundLoop destructor |
| 00c37990 | Alloc | CEntryHost::CreateEntry |
| 00c37ad0 | Fastcall_CreatureOpinionApplyModifier | CSoundEmitter::SyncVoice3D |
| 00c37b90 | CPredicate_And_Destructor | CEntryHost::DestroyEntry |
| 00c37c40 | CSBAgents_BuildAndDispatch_CreatureOpinion | CSoundEmitter::CreateEntriesForSource |
| 00c383d0 | CTCInventoryClothing_AddClothing | CRegistryLayer::Add |
| 00c38510 | GetNormalizedAngleValue | CRampedParamOwner::GetValue |
| 00c38a40 | CTCActivationReceptor_GetWeatherState | CStreamPlayer::ReceiveStatus |
| 00c3a8f0 | AnimationCurve_CheckBoundaryTransition | CRampedParam::IsSettledAtZero |
| 00c3a950 | ConstructString | CInterpolatedFloat::SetTarget(target, time) |
| 00c3ab50 / 00c3b0e0 | Container_AddItem_Checked / ..._Checked2 | CIndexMap::Add |
| 00c3abd0 / 00c3b160 | MessageEventList_GetSize / Container_GetFirstItem | CIndexMap::Get |
| 00c3b7a0 | CGraphicsSystem_LoadShaderData | CBankCriteria::Serialise |
| 00c3b850 | CShaderGroup_LoadAllShaders | CBankSampleInfo::Serialise |
| 00c3c750 | CModeBlock_CreateAudioBuffer | CSoundBuffer::CreateHardwareBuffer |
| 00c3c7a0 | Audio_ProcessEventNotification | CSoundBuffer::SetupEventNotification |
| 00c3c8c0 | WordWrap_SetupCallbacks | CNodePool::Init |
| 00c3c930 | LinkedList_ClearAndDestructor | CNodePool::Clear |
| 00c3c9e0 | ScreenEffect_RemoveFromList | CNodePool::RemoveActive |
| 00c3ca50 | CShaderRenderManager_RemoveDisplacementRenderer | CNodePool::RemoveFree |
| 00c3cde0 | ScreenEffect_EnqueueCombatAction | RecycleGlobalNode (global pool 0x0143BC00) |
| 00c3cfe0 | GetElapsedPerformanceTime | GetElapsedTicks (on the audio device; inlined at 0x00C3D504) |
| 00c3d660 | CParticleMorph_EnqueueCombatAction | CAudioEventManager::DiscardMorphEvent |
| 00c3d6a0 | CLandscapeBackgroundPatch_EnqueueAndClear | CAudioEventManager::DiscardScreenEvent |
| 00c3d710 | Render_MapQualityLevelToSettings | CAudioOutput::ApplySpeakerMode |
| 00c3e760 | CCombatRenderer_UpdateRenderState | CAudioDevice::BeginFrame |
| 00c3e800 | Audio_ProcessMixingIfNeeded | CAudioDevice::UpdateMeters |
| 00c3f280 | Render_QualityLevelAdjustment | CAudioOutput::GetFallbackSpeakerMode |
| 00c3ff70 | GFFloatToLongFloor | RoundHalfDownToLong |
| 00c402e0 | Vector3_Normalize | CPKVector3::Normalise (close) |
| 00c40ee0 | Math_ScaleByLog2 | MillibelsToAmplitude |
| 00c40f50 | Math_ComputeLogValue | AmplitudeToMillibels |
| 00c418f0 | Math_ComputeVectorMagnitude | CPKListener::NormaliseCopy |
| 00c425e0 | Hero_ProcessExperienceBlockParry | DecodeImaAdpcm (DirectX SDK CImaAdpcmCodec) |
| 00c427d0 | StreamReader_SearchByteInRange | CPKRiffReader::FindChunk |
| 00c42a70 | Std_Erase_Adjust_Pair | CShuffleBag::PickNext |

Already-landed callees with wrong names are listed in LESSONS.md under "Review at landing".
