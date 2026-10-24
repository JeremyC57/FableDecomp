# Hand chunk 433 (_global .. _global): 31 of 40 byte-exact

All 40 rows sit in the libpk audio library (`d:\dev\audio\ver_fable_1.5_b\libpk\...`, 0x00C123B0..0x00C1BF00):
no PDB names and no RTTI, so every class name below is descriptive (named from trace strings, vtables and
callers) unless it is a VC7.1 Dinkumware STL name. Two more rows (0x00C13840, 0x00C138F0) match every
instruction and only own the int3 VC7.1 emits after their final noreturn call; they land against the 113-byte span.

## What matched (31)
- **CSoundEventManager** (vtable 0x012B2748; event pool +0x5c, playback pool +0x224, module lock 0x0143BB14):
  `Shutdown` 0x00C123B0 (stop every active event, then detach listeners until the event set reports none),
  `CountPlayingEvents` 0x00C12680, `TraceEvents` 0x00C12720, `TracePlaybacks` 0x00C127C0. Shape: the pool walk
  only gets retail's registers (this esi, index edi, event ebx) through inline `GetNumEvents()` / `GetEvent(i)`
  accessors wrapping `m_events.m_count` / `m_events.Get(i)`; `IsPlaying()` is an inline `physical || virtual`;
  the release trace macro is an inline empty `TraceOutput(const CString&[, const CString&])`, which keeps the
  Describe/GetStatus temporaries (the two-argument form builds them right to left).
- **std::copy_backward** 0x00C13D40 / 0x00C13D70 (`unsigned long*` / `long*`): Dinkumware `_Copy_backward` with
  the scalar-pointer tag inlined into a 3-argument `__fastcall` `copy_backward`: one memmove import. The tag-taking
  `_Copy_backward` out of line would be `ret 8`; retail's `ret 4` is the 3-argument wrapper.
- **PKCWorkerQueue** destructors 0x00C14560 / 0x00C14630 (critical section +0, `vector<PKCRequest*>` +0x18,
  semaphore +0x28, thread +0x2c): under a scoped lock push the null stop request and release the semaphore, then
  Join and delete the thread. Shape: `operator delete(void*) throw()` (as `<new>` declares it); without the
  `throw()` an extra EH state store lands before the inline vector `_Tidy`.
- **CDataOwner::DeleteData** 0x00C14A50 (two registries' Remove, then the virtual deleting destructor) and
  **CEntryOwner::ReleaseTarget** 0x00C14AD0 (find-and-unlink loop through registry slot 17). Shape for the latter:
  an inline helper that takes the key BY VALUE (`FindEntry(void* key) { return Find(&key); }`) gives retail's fresh
  frame copy of the key for each call.
- **CSoundInstance::SetPlaybackValue** 0x00C15CD0: lock + `SetValue((float)unsigned)` (fild + 2^32 fixup).
- **WideToMultiByte** 0x00C16760 (`__fastcall`, WideCharToMultiByte with flags 0x400, default char " ", NUL at the
  written length, "" on failure; `#pragma function(wcslen)` keeps the import call) and **PKCWideString::Assign**
  0x00C16820 (resize only when length+1 differs, intrinsic memcpy through `operator[]`).
- **CAutoDeletePointer::GetChecked** 0x00C17740: the PKCAutoDelete.h line 32 "Should have checked first..." twin
  of landed 0x00C09920.
- **MoveAllItems** 0x00C17B70 (`__fastcall`): Reserve(both counts), then remove/add until GetFirst is null.
- **PKCTRRedblackTree** (vtable 0x012B2F0C; sentinel +0xc, root +0x20): `RotateLeft` 0x00C18A80, `RotateRight`
  0x00C18AD0, `VisitInOrder` 0x00C18D50 (recursion left, loop right), `FindNth` 0x00C18D90 (tail-recursive form:
  `if (node == &Sentinel) return 0; ... return FindNth(node->Right, ...)` -- the while form merges the zero return),
  `CountNodes` 0x00C18DF0.
- **PKCTRRedblackMap** (sentinel +4, root +0x1c, last-found cache +0x20): `RotateLeft` 0x00C1B200, `RotateRight`
  0x00C1B250, `Find` 0x00C1AE50 (0 hit + cache, 3 miss), `Insert` 0x00C1B720 (2 duplicate, 1 no memory, 0; fixup
  0x00C1B470 then the cache is cleared).
- **CObjectMap::Find** 0x00C196A0 (the Find of landed Resolve rows 0x00C19AD0/...): scope = key record +0xc or the
  owner default; by name (string temporary) or by id / alternate id. Shape: ONE `result = 0` variable (retail zeroes
  eax before the name test, so the name pointer lands in ecx); per-path returns give a fourth epilogue.
- **CFileManager::GetFileSize** 0x00C1A070: lock, open-file map Find, Win32 GetFileSize. Shape: the map's Find body
  in the same TU under `auto_inline(off)` removes the EH state-0 store and restores the `mov eax, fs:[0]`-first prologue.
- **PKCStaticQueue** constructor 0x00C1B340 (named from Pop 0x00C1B370's PKCStaticQueue.h line 0x43 trace "Nothing in
  this queue - you should have checked first!"): capacity, scalar `operator new(capacity * 0x1c)`, count/head zero.
- **SetDefaultRange** 0x00C1B9A0 (`__stdcall`, two globals), **CChickenSign::GetLevel** 0x00C1B9C0 (`byte * (1/127)`),
  **DecodeFixed16** 0x00C1BA40 (named `scaled` local keeps both fmuls), **CDistanceFalloff::GetAttenuation**
  0x00C1BDA0 (`<= 0` -> `test ah,41h; jp`, `>= span` -> `test ah,1; jne`, `(float)pow(...)` under
  `#pragma intrinsic(pow)` -> `_CIpow`).

## Parked for Fable 5.1 (9)
- 00c1b7d0 PKCTRRedblackMap::Remove DIFFER(213v213): ecx/edx swap only -- retail keeps the node z in edx (cache test, search loop, found path) and the key/scratch in ecx | tried: plain Niemann deleteNode, /Oa, same-TU noinline Release body (fixed the y reloads), inline DeleteNode helper | next: declare/test the cache through a separate inline FindNode helper; else lane gap: register assignment
- 00c16e10 _global::ReadBankInfoCount DIFFER(225v225): block placement only -- the merged `or eax,-1; jmp` block sits after the LAST test (ours) instead of right after the open test (retail; later failures jump back to it) | tried: four separate ifs, ternary tail, `||` for the two name reads, positive final test (`||`/positive tail move it to the function end) | next: an early-return-free form (result variable + nested ifs), or a do/while(0)-break shape
- 00c17640 _global::OpenBankFile DIFFER(203v203): `mov ebx,[esp+0x154]` / esi=path are implicit register arguments of the loader 0x00C174A0 (it pushes/uses ebx and esi without setting them) | tried: reference `__fastcall` body passing them on the stack (1 grade) | next: lane gap: LTCG register arguments
- 00c16c10 _global::HasBankSegments DIFFER(207v203): retail takes the segment file in esi (`mov ecx, esi` before each OpenSegment, esi never set/saved); ours `push esi; mov esi, ecx` -- the rest of the `||`-with-two-conditional-temporaries body lines up | tried: `__fastcall` reference body (1 grade) | next: lane gap: LTCG register arguments
- 00c16ce0 _global::RunProcessor DIFFER(173v166): retail reads the settings record from esi (never set/saved; both callers load esi) on top of ecx/edx/one stack argument | tried: reference body with the record as a second stack argument (1 grade) | next: lane gap: LTCG register arguments
- 00c16d90 SProcessorSettings::ProcessEntry DIFFER(129v122): retail takes the bank file in edi (pushed / moved to ecx, never set/saved) and hands `this` to 0x00C16CE0 in esi | tried: thiscall reference body with the file as a third stack argument (1 grade) | next: lane gap: LTCG register arguments
- 00c1bf00 _global::BuildViewMatrix DIFFER(194v174): retail takes the matrix in esi, `up` in edi and `forward` in ebx (none set/saved; caller 0x00C1C4DD loads them by lea) | tried: `__fastcall` reference body (1 grade) | next: lane gap: LTCG register arguments
- 00c13840 std::vector::_Xlen DIFFER(113v112): int3-after-noreturn only -- every instruction matches; VC7.1 emits the 0xCC after the final `_CxxThrowException` call (verified 0xCC at 0x00C138B0) | tried: the Dinkumware `throw length_error("vector<T> too long")` body (1 grade) | next: land against the 113-byte span (differ/00c13840.test.cpp PASS under g++ and VC7.1)
- 00c138f0 std::vector::_Xlen DIFFER(113v112): int3-after-noreturn only (0xCC at 0x00C13960), the twin instantiation | tried: same body (1 grade) | next: land against the 113-byte span (differ/00c138f0.test.cpp PASS)

## Skipped
None.

## Label corrections
Every catalog label in this chunk is propagated; none names the libpk code. Corrections (descriptive names unless STL):
- 0x00C1B9A0 `$E2` -> `SetDefaultRange`; 0x00C1B9C0 `ByteToFloatScale` -> `CChickenSign::GetLevel`; 0x00C1BA40 `WeaponClassToFloatScale` -> `DecodeFixed16`.
- 0x00C13D40 / 0x00C13D70 `_Copy_backward<EWeaponClass*,EWeaponClass*>` -> `std::copy_backward<unsigned long*/long*>` (3-argument wrapper, `ret 4`).
- 0x00C18A80 / 0x00C18AD0 `List_RemoveNode` / `List_RemoveIntNode` -> `PKCTRRedblackTree::RotateLeft/RotateRight`; 0x00C1B200 / 0x00C1B250 `..._Variant2` -> `PKCTRRedblackMap::RotateLeft/RotateRight`.
- 0x00C18D50 / 0x00C18D90 / 0x00C18DF0 `Tree_Preorder{Traverse,Find,Count}` -> `PKCTRRedblackTree::VisitInOrder/FindNth/CountNodes` (in-order, not preorder).
- 0x00C1AE50 `BTree_Find` / 0x00C1B720 `BTree_InsertNode` / 0x00C1B7D0 `CGameMap_RemoveByKey` -> `PKCTRRedblackMap::Find/Insert/Remove`.
- 0x00C1B340 `CGameMap_AllocateTiles` -> `PKCStaticQueue` constructor.
- 0x00C1A070 `FileManager_GetFileSizeByKey` -> `CFileManager::GetFileSize`.
- 0x00C17740 `NScript_MurderTwist_CheckCondition` -> `CAutoDeletePointer::GetChecked`.
- 0x00C123B0 `C3DMesh2_OnDestructorCleanup` -> sound-event manager `Shutdown`; 0x00C12680 `Inventory_CountEquippedItems` -> `CountPlayingEvents`; 0x00C12720 `Inventory_DebugLogState` -> `TraceEvents`; 0x00C127C0 `CTCInventory_DumpCombatActionDebug_c127c0` -> `TracePlaybacks`.
- 0x00C14560 / 0x00C14630 `DetailCache_Cleanup(_Variant)` -> `PKCWorkerQueue` destructors; 0x00C13840 / 0x00C138F0 `StdVector_ThrowTooLongException(Variant)` -> `std::vector<PKCRequest*>::_Xlen`.
- 0x00C14AD0 `CTCHero_ProcessAllCombatBlockParryEvents` -> `ReleaseTarget`; 0x00C15CD0 `Combat_ProcessAnimationByValue` -> `SetPlaybackValue`.
- 0x00C16760 `String_WideCharToMultiByte` -> `WideToMultiByte`; 0x00C16820 `CDefString_SetFromWide` -> `PKCWideString::Assign`.
- 0x00C17B70 `MergeContainers_ProcessQueues` -> `MoveAllItems`; 0x00C1BDA0 `Render_CalculateAlphaFalloff` -> `CDistanceFalloff::GetAttenuation`.
- 0x00C196A0 `TemplateConstruct_AndFindAnimation` -> `CObjectMap::Find`.
- 0x00C16C10 `LHAudioBank_InitializeBankData` -> `HasBankSegments`; 0x00C16E10 `LHFileSegment_ReadMurderTwistData` -> `ReadBankInfoCount`; 0x00C17640 `Process_BankFileWithGraphicsSetup` -> `OpenBankFile`; 0x00C16CE0 `NScript_DeleteTrackNode` -> `RunProcessor`; 0x00C16D90 `Script_ProcessInventoryItem` -> `ProcessEntry`; 0x00C1BF00 `C3DMeshObject_UpdateQuadricMatrix` -> `BuildViewMatrix`.
