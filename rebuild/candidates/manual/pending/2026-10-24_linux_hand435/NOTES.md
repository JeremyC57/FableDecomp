# Hand chunk 435 (CPanelItem .. CHandleBasedMemoryPool): 37 of 40 byte-exact

All 40 rows are `_global` catalog rows in the libpk audio library (0x00C20F60..0x00C28580). This library has no PDB
and no RTTI, its STL is VC7.1 Dinkumware in `std`, and it is built with /Gr (free functions are `__fastcall`).
Every manifest label in the chunk is propagated from an unrelated byte-identical body, so all names are descriptive.
Class names follow chunks 431/433/434 (CPanel, CPanelItem, CChickenSign, CRampedParam) and the trace strings
(PKCFrallocStatic, PKCTimerManager, PKCWorkerThread). The engine's CHandleBasedMemoryPool supplies the pool names.
Graded with gall.sh (37 MATCH/RELOCATION_MATCH). Each leaf was confirmed against `objdump -t` (exactly one symbol
per leaf). Tests pass under both g++ (tall.sh) and VC7.1 (tall.py).

## What matched (37)

**CPanelItem / CPanelItemSet** (the 0xB4-byte panel state-machine item and its owner set)
- 0x00C20F60 `CPanelItem::CountInto`: bumps the per-state counter of a statistics record. The same-TU
  `ChangeState` stand-in (auto_inline(off), with a non-empty body) lets VC7.1 keep ecx across the call. The case
  bodies come out in source order, so the source lists the cases 6, 3, 5, 4, 2, in the order of retail's
  jump-table targets.
- 0x00C21560 `CPanelItem::ReturnToIdle`: runs the exit transition of the current state (2..6), then
  `ChangeState(1)`. VC7.1 tail-duplicates the closing call into every case. Each case/callee pairing was checked
  against retail's table by hand.
- 0x00C21700 `CPanelItem::Restart`: state 3/6 goes back to idle first. The state word is a 16-bit if/else chain
  (`cmp ax,3`), not a switch.
- 0x00C21E60 `CPanelItemSet::FindItem` and 0x00C21EB0 `CPanelItemSet::FindEntry`: peek a queue's first entry and
  return it while its key is below the given key. The queue is a member OBJECT with a virtual slot-11 GetFirst.
  `key >= 0xFFFFFFFF` (unsigned) gives the `jae`; `== 0xFFFFFFFF` gives `je`.
- 0x00C22100 `CPanelItemSet::AllocateHandle` (/EHsc): hangs a stack CPanelFailedAllocationHandler in the pool's
  handler slot around `Allocate`. The handler is polymorphic with an empty inline virtual destructor (EH state 0,
  no destructor code). The handle is homed in the dead `key` slot. Because this pool variant has its handler at
  +0x34/+0x38, the pool is modelled locally.

**PKCFrallocStatic / PKCTimerManager**
- 0x00C22B40 `PKCFrallocStatic::Push` (/EHsc) and 0x00C25A50 `PKCTimerManager::InitiateShutdown` (/EHsc): the
  library's warning block (a scoped CString built by an Append chain, an optional sink call, then CDebugBreak).
  Push reads the ring's write index twice and stores the count before the new index.

**C3DVector helpers** (the library's own vector maths)
- 0x00C258A0 `operator*(const C3DVector&, float)`: a `__fastcall` free operator with the hidden result pointer in
  ecx. A user ctor makes it return through the hidden pointer. The products are evaluated right to left.
- 0x00C258D0 `C3DVector::GetMagnitudeAndNormalise`: the components are copied into float locals first, so the
  integer moves come straight from ecx. A struct copy goes through `mov eax,ecx` instead.
- 0x00C25970 `C3DVector::GetMagnitude`: an inline three-float helper loads Z, Y, X onto the FPU stack and pops the
  argument copies after fsqrt.

**CFader** (the panel's 0x50-byte fader around a CRampedParam)
- 0x00C25E20 `SetFadeInTime` and 0x00C25E50 `SetFadeOutTime`: the per-update step is `m_timeStep / time`, or 1.0f
  when `time < m_timeStep`. The 1.0f arm gets its own `ret 4`.
- 0x00C25EA0 `FadeOut`: one zero register feeds both the angle-flag compare and the zero stores. `value == 0.0f`
  keeps the value on the left.
- 0x00C25F00 `GetOutput`: the ramped value, wrapped into [-pi, pi) for angle parameters.
- 0x00C26010 `CFader::CFader(float)`: the initialiser list in declaration order. SetFadeInTime, SetFadeOutTime and
  SetMaxRate bodies sit in the same TU, so the three 250 calls run back to back with no ecx reload.

**CStreamBuffer** (the 0x2C-byte stream buffer on the panel's four queues)
- 0x00C262E0 `Attach`: the format is a POD struct assignment (five dword moves through ebx). The data size is
  assigned and then adjusted in a second statement.
- 0x00C26390 `Release`: off the loading queue (state 1) or the idle + cached queues (state 3), then the pool block
  is freed and the sample released. VC7.1 cross-jumps the two `Remove ; m_state = 0` tails.
- 0x00C26410 `AddUsers` and 0x00C26480 `RemoveUsers`: move the buffer between the busy and idle/cached queues.
  `m_lastUse = panel->m_useStamp; ++panel->m_useStamp;` reads the stamp twice (aliasing).

**PKCWorkerThread / PKCStreamWorkerThread + Dinkumware vector**
- 0x00C26B20 and 0x00C26F80 `ProcessNextRequest` (/EHsc, identical bodies over two request types): wait on the
  semaphore, then pop the front request under a scoped lock held in an inner block, then run the handler.
  `!empty()` is Dinkumware's size test. Reading the front through ONE named `begin()` iterator, which is then erased,
  reuses the test's `_Myfirst`.
- 0x00C27020 `std::vector<PKCStreamRequest*>::erase(iterator)`: `copy(_Where.base() + 1, _Mylast, _Where.base())`
  over raw pointers (the scalar memmove) gives retail's eax/ecx pair. Copying through `_Ptrit` swaps it.

**CCodaPlayback** (vtable 0x012B4034, the CODA source wrapper; landed slots 0/6 at 0x00C275E0/0x00C27670)
- 0x00C27550 `SetPosition` (/EHsc): `C3DVector v; v = position.SwapYZ();`. Declaring and then assigning gives
  retail's copy of the return temporary; direct construction builds in place.
- 0x00C27620 `SetVolume` (/EHsc): a non-3D source is scaled by the panel's master volume under the module lock.
  The CODA calls are `extern "C"` (nothrow under /EHsc), so there is no EH frame.
- 0x00C276B0 `Update` (/EHsc): the loop-point arm/disarm logic. The early `return` inside the lock scope keeps an
  EH frame with no state-0 store. `!= 0` tested first puts the stop arm off the fall-through.
- 0x00C27AF0 `CCodaPlayback::CCodaPlayback()`: the vptr, then three zero stores in member order.

**CMaskedChickenSign** (a CChickenSign with word/byte override masks)
- 0x00C27BC0 `GetWordC`: compound assignments on the argument (`base &= ~m_wordCMask; base |= ... | m_wordCMask;`)
  split the work around the call as retail does.
- 0x00C27BF0 `SetByteD`: stores the mask, then calls `m_sign.SetByteD(value)`.
- 0x00C27D40 `MergeWordC` and 0x00C27D80 `MergeByteD`: `mask |= m_mask` in place on the argument slot, then a named
  `own` word with `own |= mask & value`. A byte-typed merge gives byte ops.

**CHandleBasedMemoryPool** (the libpk build: FreeLists at +0x3C, `std::vector<CHeader*> Handles` at +0x43C)
- 0x00C27EA0 `CreateArea`: the header words go in the engine's CreateInitialFreeArea order, then the neighbours are
  linked back.
- 0x00C27F80 `CombineAreas`: `this` stays in edi, and RemoveFreeArea/InsertFreeArea stay out of line.
- 0x00C28200 `ClearFreeLists`: an intrinsic memset of the 256 buckets (`rep stosd`).
- 0x00C28240 `Initialise`: assigning the size first schedules both argument loads up front. The buffer is
  16-aligned through `/ 16 * 16`.
- 0x00C28290 `ResizeArea`: the spare is a signed compare against `sizeof(CHeader)`, and each arm has its own
  epilogue.
- 0x00C28580 `MoveArea`: the handle table store goes through Dinkumware's `operator[]`, which is
  `*(begin() + _Pos)`. Writing `_Ptrit::operator+` as `_Tmp += _Off` keeps the separate `lea`.

## Parked for Fable 5.1 (3)

- 0x00C22080 CPanelFailedAllocationHandler::OnAllocationFailed DIFFER(120v120): retail puts the shared `xor eax,eax` exit after the success epilogue (both failure tests `je`/`jne` forward to the end); ours emits it inline right after the Covers() test | tried: early-return `||`, success path as the if body with `return 0` after it, two separate early `return 0`s, `if (b && !Covers()) {} else return 0;` | next: a single `result` variable returned once, or a do/while(0)-break form; else lane gap: block placement
- 0x00C25F40 CFader::Update DIFFER(163v163): the fade-in add is `fld [esi+4] ; fadd [esi+44h]` in retail (m_level loaded first), ours loads m_fadeInStep first | tried: `m_level += m_fadeInStep`, `m_level = m_fadeInStep + m_level` | next: lane gap: x87 sum operand canonicalisation (perhaps a `(float)` cast spelling, as with the double-global gap)
- 0x00C26BC0 std::vector::erase DIFFER(49v49): eax/ecx swapped (retail `_Mylast` in ecx, `_Where + 1` in eax) | tried: `_Ptrit` copy, tag-dispatched `_Copy_opt`, unwrapping `_Ptrit` copy overload, same under /Gr | next: regrade at landing. The file now holds the raw-pointer copy body that matched the byte-identical twin 0x00C27020 (its retail bytes are identical, import slot included); it was not graded against this row because the budget was spent

## Skipped

None. No row covers two functions, and every row ends in its own `ret` before the padding (the stripped lengths are
in STATUS.tsv).

## Label corrections

Every catalog label in the chunk is a propagated game-side name:

| Address | Catalog | Should be |
|---|---|---|
| 0x00C20F60 | Weapon_IncrementSlotCounter | CPanelItem::CountInto |
| 0x00C21560 | NUISystem_CScrollableList_OnStateChange | CPanelItem::ReturnToIdle (landed 0x00C21620 calls it `Attach_00c21560` on a "CSteeringAgent_c21620") |
| 0x00C21700 | Inventory_DispatchEvent | CPanelItem::Restart |
| 0x00C21E60 | CTCInventoryItem_GetFirstAvailableIndex | CPanelItemSet::FindItem |
| 0x00C21EB0 | CTCInventoryBase_GetFirstAvailableIndex | CPanelItemSet::FindEntry |
| 0x00C22080 | CTCHero_Respawn | CPanelFailedAllocationHandler::OnAllocationFailed |
| 0x00C22100 | NUISystem_CSteeringAgent_FindAgentInList | CPanelItemSet::AllocateHandle |
| 0x00C22B40 | NScript_EventMessageSet_Push | PKCFrallocStatic::Push |
| 0x00C258A0 | GetNewCameraPosFromOcclusionParams | operator*(const C3DVector&, float) |
| 0x00C258D0 | Math_Vector3Normalize | C3DVector::GetMagnitudeAndNormalise |
| 0x00C25970 | Math_Vector3Length | C3DVector::GetMagnitude |
| 0x00C25A50 | PKCTimerManager_Close | PKCTimerManager::InitiateShutdown |
| 0x00C25E20 | Thiscall_ScaleArrayElement_0x11 | CFader::SetFadeInTime |
| 0x00C25E50 | Thiscall_ScaleArrayElement_0x12 | CFader::SetFadeOutTime |
| 0x00C25EA0 | C3DMeshFileTransformationChunk_UpdateAngle | CFader::FadeOut |
| 0x00C25F00 | Radius_ApplyModifiers | CFader::GetOutput |
| 0x00C25F40 | HeroWorksInBrothel_UpdateAnim | CFader::Update |
| 0x00C26010 | CWideString_ConstructFromResourceId | CFader::CFader(float) |
| 0x00C262E0 | Thiscall_ParticleTrailSetup | CStreamBuffer::Attach |
| 0x00C26390 | ParticleEffect_Release | CStreamBuffer::Release |
| 0x00C26410 | CTCInventoryItem_SetEquipped | CStreamBuffer::AddUsers |
| 0x00C26480 | CTCInventoryItem_SetUnequipped | CStreamBuffer::RemoveUsers |
| 0x00C26B20 | Container_TryRemoveItem | PKCWorkerThread::ProcessNextRequest |
| 0x00C26F80 | Container_TryRemoveItemDuplicate | PKCStreamWorkerThread::ProcessNextRequest |
| 0x00C26BC0 | Container_RemoveAtIndex | std::vector\<PKCRequest*\>::erase |
| 0x00C27020 | Container_RemoveAtIndexDuplicate | std::vector\<PKCStreamRequest*\>::erase |
| 0x00C27550 | CPhysicalPrimitive_InitializeFromMesh | CCodaPlayback::SetPosition (vtable slot 7) |
| 0x00C27620 | CEngineSubPrimitive_AllocateBuffer | CCodaPlayback::SetVolume (slot 1) |
| 0x00C276B0 | DebugState_UpdateDisplay | CCodaPlayback::Update |
| 0x00C27AF0 | SoundObject_Construct_c27af0 | CCodaPlayback::CCodaPlayback() |
| 0x00C27BC0 | InputState_GetPressedBitwise | CMaskedChickenSign::GetWordC |
| 0x00C27BF0 | Thiscall_NavigatorAssign | CMaskedChickenSign::SetByteD |
| 0x00C27D40 | EWeaponClass_InsertWithMask | CMaskedChickenSign::MergeWordC |
| 0x00C27D80 | Navigator_UpdateState | CMaskedChickenSign::MergeByteD |
| 0x00C27EA0 | WordWrap_SetCallback | CHandleBasedMemoryPool::CreateArea |
| 0x00C27F80 | CHandleBasedMemoryPool_MergeBlocks | CHandleBasedMemoryPool::CombineAreas |
| 0x00C28200 | $E1 | CHandleBasedMemoryPool::ClearFreeLists |
| 0x00C28240 | CHandleBasedMemoryPool_Initialize | CHandleBasedMemoryPool::Initialise (the engine spelling) |
| 0x00C28290 | Thiscall_BlockAllocationShrink | CHandleBasedMemoryPool::ResizeArea |
| 0x00C28580 | Thiscall_BlockDefragmentCopy | CHandleBasedMemoryPool::MoveArea |

**Callee labels in already-landed rows** (checked in `rebuild/src/compiled`):

| Address | Landed as | Actually |
|---|---|---|
| 0x00C1BAA0 | CManager::GetKeyPressed | CChickenSign::GetWordC |
| 0x00C1BBE0 | CThing::SetCurrentMapNumberc | CChickenSign::SetWordC |
| 0x00C1BBF0 | EnableNavigator | CChickenSign::SetByteD |
| 0x00C25E80 | AddExclusiveObserver (a `C_00c25e80` +0x40 store) | CFader::SetMaxRate |
| 0x00C25FF0 | extern "C" `__fastcall` ConstructString | CFader::SetValue(value, time) = `m_param.SetTarget(value, 0, time)` |
| 0x00C27BA0 | std::vector\<EWeaponClass\>::_Umove, a fake `__fastcall` | CMaskedChickenSign::SetWordC(mask, value) |
| 0x00C35AF0 | global "operatornew" (`New_c35af0`) | a CODA `__stdcall (source, value)` volume setter |
| 0x00C35CC0 | GetEffectiveValue | CODA source position getter |

For 0x00C35AF0, landed 0x00C49420 already calls it `CodaSource_SetDecibels`, and these rows call it
`CODA_SetSourceVolume`; reconcile the two at landing. The other callees named here are 0x00C2A090
PKCSemaphore::Wait (already correct), 0x00C35DD0 CODA stop-source and 0x00C33FD0 CODA set-position.
