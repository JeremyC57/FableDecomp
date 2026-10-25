# Hand chunk 437 (CChildListLoader .. CVoiceSet): 34 of 40 byte-exact

All 40 rows are `_global` catalog rows in the libpk audio/middleware library (no PDB/RTTI): every class and function
name below is descriptive, built from the strings (PKCAutoFile.cpp, PKCAutoMap.h, PKCOneShotQueue.h,
PKCASyncFileReader.h, "LHAudioWaveData"), the vtables and the landed callers/callees. 8 MATCH + 26 RELOCATION_MATCH;
every matched row has a test that prints PASS under g++ (tall.sh) and the lane's VC7.1 (tall.py).

## What matched (34)

- **CChildListLoader** (0x00C2EAB0 ProcessAllDrivers, 0x00C2EB00 Load): the trigger-driver walk calls
  ProcessDriver(id, count) with the ushort count widened through an `unsigned long` parameter (an `unsigned short`
  parameter gives `xor ecx,ecx ; mov cx`). Load runs seven steps with an IsOk check before each; a void member whose
  last statement is `if (!m_pStatus->IsOk()) return;` gives retail's tail `jmp IsOk` and no `xor al,al` exit.
- **Sub-animation names** (0x00C2ECD0 IsSubAnimationName, 0x00C2ED20 StripSubAnimationSuffix, both `__fastcall` on a
  CString): strncmp has to be a `__declspec(dllimport)` declaration (called through the IAT) with the literal "_SUB"
  first; strlen is the intrinsic. The strip is `SetAt(SafeStrlen(p) - 5, 0)`.
- **CWaveDataReader::OnRecord** (0x00C31230, slot 3 of the vtable after the "LHAudioWaveData" string): copies a wave
  record into the PKCAutoMap entry for its key. The find-or-create is an inline map member with an early return
  (`if (E* e = Find(k)) return e; return CreateEntry(k);`): both results meet in eax before `mov esi,eax`.
- **PKSerialiseCParser::TransferVersion** (0x00C31D40): an `unsigned char` result gives `sete al` with no
  `xor eax,eax` (a bool result adds it).
- **PKCAutoFile::GetChecked** (0x00C31DB0, /EHsc): agent433's CDebugStream log-line model, `.\PKCAutoFile.cpp` line 26.
- **CMemoryStream** (0x00C31F60 constructor, vtable 0x012B4E08 over the abstract base 0x012B4E00): asks the remote
  source for the size, allocates with scalar new, loads through a one-word CRemoteLoader local homed in the source's
  dead argument slot. The base's inline virtual destructor is the state-0 unwind action (0x00C31E80).
- **CRemoteLoader::LoadDataFromRemote** (0x00C32060, /EHsc): a synchronous read over the async file reader. The wait
  object is one 0x30-byte local (reply handler + mailbox) whose inline constructor makes both out-of-line calls before
  state 0; the 12-byte user-mailbox callback is built inline; the reply is the 8-byte {flag, message} record the
  handler copies out (a lone bool local made the frame 4 bytes short).
- **CSortedPtrArray** (0x00C32160 FindIndex, 0x00C32220 FindIndexAlt): binary search through a polymorphic comparator
  (slots 0/1 and 2/3); `this` spilled into the push-ecx slot; matched on the first grade.
- **Audio system API** (0x00C334D0 Create, 0x00C33590 GetDeviceInfo, 0x00C335C0 Query, 0x00C336F0 SetListenerTarget,
  0x00C33DC0 SetModeFlags, 0x00C35EC0 SetOutputFlags, 0x00C359E0 AudioObject_Play, 0x00C35A10 AudioObject_PlaySource):
  `__stdcall` wrappers over the global audio system 0x0143BB58. `cond ? 1 : 0` on a bool gives `neg al ; sbb ; neg`;
  `ok ? &g : 0` gives `neg/sbb/and`; flag bits through inline `(m_flags >> n) & 1` accessors (`shr edx,2 ; test dl,1`;
  1-bit fields give `test al,4`); the listener target goes through an inline device member with a named helper local.
- **InitTimerGlobals** (0x00C33770, catalog `$E2`): a plain function StartTimer 0x00C1C1A0 calls directly, over extern
  globals (an inline-constructed static folds into .data and leaves no `$E` symbol).
- **CVoiceParams::GetPitch** (0x00C34410): the scaled pitch is a named float local, spilled through the push-ecx slot
  and pushed from ecx after the mask (inline in the call it is pushed straight from st0).
- **CPositionHolder::SetPosition** (0x00C34530): `if (GetY() == y && GetX() == x) return false; SetX; SetY; return true;`
  gives the two `test ah,44h ; jp` to the store block with the false return on the fall-through.
- **CQuadHolder::GetValue** (0x00C34600): a 16-byte POD returned by hidden pointer.
- **Tables** (0x00C34870 CItemTable::CopyFrom, 0x00C348F0 Validate, 0x00C34930 Find, 0x00C34B80 CEntryList::~CEntryList,
  0x00C34E60 CRowTable::~CRowTable, 0x00C35080 CPtrArray::Remove): the abstract table base (vtable 0x012B3444, 12
  slots) and agent434's CPtrArray template. The destructor loops are `for (count = m_count; count; count = m_count)
  Remove(data[count - 1]);` (`while (m_count)` reloads and pads the loop heads).
- **CSampleHeader::GetFormat** (0x00C356F0): a switch with duplicated tails matched as written.
- **CResourceWorld** (0x00C36180 AllocateFor, 0x00C36290 FreeAllocation, 0x00C362F0 AllocateForAll): a discarded
  `new CResourceAllocation(this, owner)` in a void member (returning it adds a second epilogue); `delete` of an inline
  Detach. AllocateForAll walks the owner set with a stack visitor (vtable 0x012B50CC); retail keeps `this` in edx
  across the visitor's constructor 0x00C36270, which a same-TU `__declspec(noinline)` constructor body reproduces --
  but only outside `#pragma auto_inline(off)` (under it the abstract base's implicit constructor becomes a call, the
  stand-in clobbers edx and VC7.1 moves `this` to esi).
- **CVoiceSet::GetMaxValue** (0x00C37000): the same visitor shape (vtable 0x012B51E8, max kept from 0) returning the
  float field; same-TU noinline constructor body keeps edx.

## Parked for Fable 5.1 (6)

- 00c30e90 _global::HasWaveBankSegments DIFFER(209v203): retail takes the segment file in esi unsaved (no load, `mov ecx,esi` before each OpenSegment, plain `ret`); ours loads it from a stack argument (`push esi ; mov esi,[esp+28h]`) -- the rest (the `||` temporaries' ebx flags 1/3, the frame flag byte, `xor eax,eax ; sete al`) matches | tried: one reference body (cdecl parameter) | next: lane gap: ltcg register argument
- 00c34440 CVoiceParams::GetGain DIFFER(145v138): retail keeps the parameter-block result in st0 (`fst [esp+8] ; fld 0.0 ; fld st(1) ; fucompp`, `fstp st0` on the positioned path) and ends `fld attenuation ; fmul value ; fmulp`; ours `fstp`s and reloads it and ends with two memory `fmul`s | tried: one-expression `if (value != 0 && positioned) return F*value*(1-S*k)`, early return + named attenuation local (fixed the call order and layout) | next: lane gap: x87 operand order
- 00c34630 _global::DecodeAdpcmToPcm DIFFER(122v136): retail keeps two self-copies (`mov cx,[esi+0Ch] ; mov [esi+0Ch],cx` and `mov ecx,[esi+10h] ; ... ; mov [esi+10h],ecx`) and loads +8 only after the +4 store; VC7.1 deletes the self-copies and hoists the load | tried: nested-if form, early-return form (fixed the block layout; the `__fastcall` ecx/edx prologue already matched) | next: volatile candidate (the format record is shared with the mixer thread?), lane gap: volatile is banned
- 00c35990 _global::AudioObject_SetSource DIFFER(79v79): retail falls through the null-source arm (`test ; push esi ; jne <checks at the end> ; xor esi,esi`), ours hoists `xor esi,esi` above the test and keeps the checks inline | tried: bitfield flags and the direct form (both keep the source in ecx, 71 bytes), shift-and-test accessors with a `checked` local (0 or source) | next: assign the local in both arms of an if/else
- 00c35dd0 _global::AudioSample_SetPlaying DIFFER(85v89): retail re-tests the +0x5C data word (`test eax,eax ; je <error>` then a dead `test eax,eax ; je <success>`); VC7.1 folds the second test in every form | tried: `||` expression, early-return inline predicates, nested inlines, one `&&`/`||` expression | next: volatile candidate (same libpk sample records as 0x00C34630), lane gap: volatile is banned
- 00c36b90 CGridCursor::GetCell DIFFER(223v219): retail keeps 1/size in st0 after the divide (`fst [esp+8] ; fmul y`) and multiplies it by each memory coordinate (`fld inv ; fmul v`); ours pops it and loads the coordinate first (`fld v ; fmul inv`) whichever operand order the source writes | tried: x/y/z statements, y-first order with a raw floor(y) local and `inv * v` (fixed the frame and the call order) | next: lane gap: x87 operand order

## Skipped

None (no row covers two functions).

## Label corrections

Every catalog label in this chunk is propagated from unrelated bodies (no PDB covers libpk); all names are descriptive:
0x00C2EAB0 ProcessAllEventTriggerDrivers -> CChildListLoader::ProcessAllDrivers; 0x00C2EB00 NScript_ProcessDeletedTrackNode
-> CChildListLoader::Load; 0x00C2ECD0 Std_IsSpecialFunctionName -> IsSubAnimationName; 0x00C2ED20 PairStringAllocator_Construct
-> StripSubAnimationSuffix; 0x00C30E90 Audio_LoadAndValidateWaveData -> HasWaveBankSegments; 0x00C31230
CShaderResource_SetPixelShader -> CWaveDataReader::OnRecord; 0x00C31D40 CMainGameComponent_BeginInputLoading_Wrapper ->
PKSerialiseCParser::TransferVersion; 0x00C31DB0 CBankFile_ValidateOpen -> PKCAutoFile::GetChecked; 0x00C31F60
CResourceContainer_InitializeWithCapacity -> CMemoryStream::CMemoryStream; 0x00C32060 CResourceContainer_LoadDataFromRemote ->
CRemoteLoader::LoadDataFromRemote; 0x00C32160 / 0x00C32220 BinarySearch_FindInsertionPoint -> CSortedPtrArray::FindIndex /
FindIndexAlt; 0x00C334D0 CTCHeroStats_GetVectorArraysIfInitialized -> AudioSystem_Create; 0x00C33590 WordWrap_SetCallback ->
AudioSystem_GetDeviceInfo; 0x00C335C0 operator!=<char,char> -> AudioSystem_Query; 0x00C336F0 Constraint_NotifyGlobalManager ->
AudioSystem_SetListenerTarget; 0x00C33770 $E2 -> InitTimerGlobals (not a dynamic initialiser); 0x00C33DC0
PlayerGui_OnInventoryChange -> AudioSystem_SetModeFlags; 0x00C34410 Graphics_MultiplyFloatParams -> CVoiceParams::GetPitch;
0x00C34440 CDraw2DText_CalculateAlpha -> CVoiceParams::GetGain; 0x00C34530 CHeightMapCell_OnHeightChanged ->
CPositionHolder::SetPosition; 0x00C34600 GFToScreen -> CQuadHolder::GetValue; 0x00C34630 Thiscall_HeroExperienceBlockParry
-> DecodeAdpcmToPcm (`__fastcall`, not thiscall); 0x00C34870 CCreatureAction_ExecuteWeaponAnimations -> CItemTable::CopyFrom;
0x00C348F0 VectorLike_IterateNested -> CItemTable::Validate; 0x00C34930 FindInGrid -> CItemTable::Find; 0x00C34B80
Vector_RemoveDuplicates -> CEntryList::~CEntryList; 0x00C34E60 Vector_RemoveAndDestroyDuplicates -> CRowTable::~CRowTable;
0x00C35080 IntArray_RemoveByValue -> CPtrArray<CPlaybackEntry*>::Remove; 0x00C356F0 CInventoryItem_CopyProperties ->
CSampleHeader::GetFormat; 0x00C35990 Inventory_OnItemEquipped_ValidateWeapon -> AudioObject_SetSource; 0x00C359E0
CExclusiveAssert_OnQuestItemUsed_Bool -> AudioObject_Play; 0x00C35A10 Inventory_OnItemEquipped_ValidateWeaponExtended ->
AudioObject_PlaySource; 0x00C35DD0 CDrawAutoPickups_ShouldRender -> AudioSample_SetPlaying; 0x00C35EC0 InvalidatePickupDisplay
-> AudioSystem_SetOutputFlags; 0x00C36180 Alloc -> CResourceWorld::AllocateFor; 0x00C36290 Thiscall_ResourceMapEntryRemove ->
CResourceWorld::FreeAllocation; 0x00C362F0 CSBAgents_BuildAndDispatch_A -> CResourceWorld::AllocateForAll; 0x00C36B90
Grid_ComputeLocalCoordinates -> CGridCursor::GetCell; 0x00C37000 IntelligentPointer_BuildAndDispatchFloat ->
CVoiceSet::GetMaxValue.

Already-landed callees whose names are wrong (see LESSONS.md "Review at landing"): 0x00C0C200, 0x00C3B900, 0x00C31F00,
0x00C1DD50, 0x00C1D9D0, 0x00C1DCE0, 0x00C20340, 0x00C36270, 0x00C36DA0, 0x00C1BD60.
