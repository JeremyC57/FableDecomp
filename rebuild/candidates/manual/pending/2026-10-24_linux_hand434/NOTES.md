# Hand chunk 434 (CAudioDevice .. CSoundEvent): 39 of 40 byte-exact

The rows sit in the libpk audio library (no PDB, no RTTI; chunk 433 found the `d:\dev\audio\ver_fable_1.5_b\libpk`
trace paths) and in the small UI / item-action helpers that follow it. No PDB names exist for any of them, so every
class and method name below is descriptive (named from callers, vtables, offsets and the sound-event describer's
strings at 0x00C21040), except `Atan2` / `CMatrix3x4::RotateA` / `RotateB`, which keep the names of the landed caller
GetRotationAngles 0x00C1CC10. Grades: 34 of the 39 matched on the first grade (see STATUS.tsv for the counts).

## What matched
- **Audio device / listener / resources** (lock 0x0143BAFC via CScopedLock 0x00C0FFC0 / 0x00C0FFE0, the `__stdcall`
  library calls 0x00C33400..0x00C33920): `CAudioDevice::GetDefaultRate` 0x00C1C030 (settings block +0x60),
  `GetDefaultRateRatio` 0x00C1C0B0 (unsigned rate / unsigned base, two 2^32 fix-ups), `CAudioListener::Update`
  0x00C1C280 (Y/Z-swapped position, front, top; the locals are declared first and ASSIGNED from `SwapYZ()` so the three
  hidden-result temporaries are built in one frame slot and copied), `CAudioResources` constructor 0x00C1C700.
- **Matrix math** (x87, /O2 intrinsics): `CMatrix3x4::RotateB` 0x00C1C900 (Y/Z of every row) and `RotateA` 0x00C1C990
  (X/Z): the cross product that is overwritten first (`s*Y` / `s*X`) is a named local formed before the stores;
  `Atan2(float x, float y)` 0x00C1CAB0 (`__stdcall`, four octant returns through intrinsic `atan` = `fld1 ; fpatan`,
  `__real@` constants pi/2, 0, pi, -pi/2 checked by value). All three on the first grade.
- **Script module timer**: `CScriptModuleTimer::GetRemainingTimeSeconds` 0x00C1D820 (unsigned frames / fps),
  `UpdateState` 0x00C1D8C0 (needs a named `bool handled = target->Dispatch(...)`; the direct `?:` reloads the target
  after the argument calls), `CScriptRequest` constructor 0x00C1DF50.
- **Blend owner / pointer arrays**: `CBlendOwner::AddBlend` 0x00C1E780 (if/else over two inline `CPtrArray::Add`s: VC
  merges the arms behind a selected `this`), `RemoveAllBlends` 0x00C1E800, the two `CPtrArray<T>::Remove`
  instantiations 0x00C1E900 (CBlendEntry*) and 0x00C1EBF0 (CDisplayElement*), `CombatSequence_AlignDuration` 0x00C1E990
  (`__fastcall`, manifest name kept because the landed caller uses it), `CUIComponentBase` constructor 0x00C1EAA0.
- **Visited item / item actions**: `CVisitedItem::MatchesKey` 0x00C1EC70, `ContainsValue` 0x00C1ECF0
  (`value == GetAt(...)`, value on the left, gives retail's `cmp ebp,eax`), the two `CItemAction` constructors
  0x00C1EE60 (key) and 0x00C1EEE0 (CString by value + named flag; both /EHsc), `SetTypeBits` 0x00C1F320, `Run`
  0x00C1F480, `Handle` 0x00C1F4E0 (the factory replace is an inline `ReplaceItem` helper).
- **CSoundEvent** (0xB4 bytes, vtable 0x012B3674; chunk 433's name; owner `CSoundEventManager`; states 2 "Waiting for
  data", 3 "Waiting for physical player", 4 "Waiting for memory", 5 "Playing using virtual player", 6 "Playing using
  physical player"): `GetActivePlayer` 0x00C20750 (jump table checked by hand: 1, 2 -> 0; 3..5 -> +0x78; 6 -> +0xA0),
  `IsWaitingOrPlayingPhysical` 0x00C207C0, `WaitForData` 0x00C20850 / `StopWaitingForData` 0x00C20920 (+0x134 list),
  `WaitForMemory` 0x00C20880 (+0x194 list), `WaitForPhysicalPlayer` 0x00C20950 / `StopWaitingForPhysicalPlayer`
  0x00C209C0 (+0x164 list, channel), `PlayPhysical` 0x00C20A20 / `StopPhysical` 0x00C20B50, `PlayVirtual` 0x00C20B00 /
  `StopVirtual` 0x00C20BB0, `GetLevel` 0x00C20BE0, `IsAudible` 0x00C20C10, `ResetLevel` 0x00C20DF0, `MarkIfMatches`
  0x00C20E80, `IsWaiting` 0x00C20EF0. Shape: every method that calls CheckState carries a same-TU
  `#pragma auto_inline(off)` + `__declspec(noinline)` CheckState body above it (VC7.1 then knows the callee leaves ecx
  alone and keeps `this` in ecx across the call, as retail does); GetLevel / ResetLevel also need GetActivePlayer's
  body in the TU. PlayPhysical needed float `0.0f` trailing request members (integer zeros were CSE'd into a zero
  register) and `m_pChannel = channel; m_pPhysicalPlayer = player;` in that order to get retail's +0xA0-first stores.
  The sources are generated from one model (gen/voice_model.inc.txt + gen/rows_voice.py via gen/gen_voice.py), the
  tests likewise (gen/gen_voice_tests.py).

## Parked for Fable 5.1
- 00c20720 CSoundEvent::CheckState DIFFER(3v43): retail `movzx eax,word [ecx+0ACh] ; cmp eax,6 ; ja ; movzx eax,byte [eax+0C20744h] ; jmp [eax*4+0C20740h]` with all seven byte entries 0 and the one dword entry (and the default) the same `ret 4`; ours is just `ret 4` | tried: break-only cases, `__noop` / `__assume` bodies, dead stores (local, address-taken local, parameter), `if (cond) {}`, `(void)(cond)`, empty inline Check(bool), empty loops, inner switch, empty out-of-line Trace defined later in the TU, value switch feeding an empty loop, `#pragma optimize("g", off)` (2 grades of the natural body, the second only re-checking the class rename; the rest were probes) | next: lane gap: LTCG -- libpk was built with link-time code generation (chunk 433's register-argument helpers), so the cases most likely called an empty cross-module trace/assert that LTCG removed only after the switch had been lowered; the row is unique in the exe (no other all-zero byte table). The parked source is the natural C++ body; the same body is the same-TU stand-in in every CSoundEvent source.

## Skipped
None.

## Label corrections
Every catalog label in this chunk is propagated; none names the libpk / UI code. Corrections (all descriptive names):
- 0x00C1C030 `GetDefaultVal<CAnimationSet>` -> `CAudioDevice::GetDefaultRate`; 0x00C1C0B0 `C3DMeshFileBoneChunk_GetDeterminantRatio` -> `CAudioDevice::GetDefaultRateRatio`; 0x00C1C280 `CPhysicalPrimitiveMesh_ComputeLighting` -> `CAudioListener::Update`; 0x00C1C700 `CParticleMorphs_InitializeEntry` -> `CAudioResources::CAudioResources`.
- 0x00C1C900 `CMatrix3x4_RotateYaw` -> `CMatrix3x4::RotateB` (Y/Z rotation, i.e. about X); 0x00C1C990 `CMatrix3x4_RotateYawSparse` -> `CMatrix3x4::RotateA` (X/Z, about Y); 0x00C1CAB0 `Math_Atan2Normalized` -> `Atan2` (names of the landed caller 0x00C1CC10; neither rotation is a yaw in Fable's Z-up frame).
- 0x00C1D820 `CScriptModuleSelfTerminate_GetDurationMsec` -> `CScriptModuleTimer::GetRemainingTimeSeconds` (frames / fps: seconds, not msec); 0x00C1D8C0 `CScriptModuleSelfTerminate_UpdateState` -> `CScriptModuleTimer::UpdateState`; 0x00C1DF50 `CAIStateGroup_SitForMeal_constructor` -> `CScriptRequest::CScriptRequest`.
- 0x00C1E780 `CUIEventQueue_EnqueueKeyInput` -> `CBlendOwner::AddBlend`; 0x00C1E800 `C3DAnimationBlendState_RebuildCombinations` -> `CBlendOwner::RemoveAllBlends`; 0x00C1E900 `EHeroMorphType_FindAndRemoveEntry` -> `CPtrArray<CBlendEntry*>::Remove`; 0x00C1EBF0 `Vector_RemoveByValue` -> `CPtrArray<CDisplayElement*>::Remove`; 0x00C1EAA0 `NUISystem_CDisplayElement_Constructor` -> `CUIComponentBase::CUIComponentBase`.
- 0x00C1EC70 `Container_IterateAndCount` -> `CVisitedItem::MatchesKey`; 0x00C1ECF0 `Container_SearchByValue` -> `CVisitedItem::ContainsValue`.
- 0x00C1EE60 `CCreatureAction_WriteUnsheatheWeapon` / 0x00C1EEE0 `CAnimationSet_LoadAndUnsheatheWeapons` -> `CItemAction::CItemAction` (key / name overloads); 0x00C1F320 `Component_UpdateState` -> `CItemAction::SetTypeBits`; 0x00C1F480 `ActionHandler_ReplaceWithUnsheatheWeapons` -> `CItemAction::Run`; 0x00C1F4E0 `CQueueProcessor_HandleProcessError` -> `CItemAction::Handle`.
- 0x00C20720 `EmptyStub` -> `CSoundEvent::CheckState`; 0x00C20750 `InputEvent_GetScreenCoordinate` -> `GetActivePlayer`; 0x00C207C0 `Thiscall_ValidateInventoryRange` -> `IsWaitingOrPlayingPhysical`; 0x00C20850 `Thiscall_SetStateValue_2` -> `WaitForData`; 0x00C20920 `Thiscall_SetStateValue_1` -> `StopWaitingForData`; 0x00C20880 `CTCInventory_OnKeyPressRebootGame` -> `WaitForMemory`; 0x00C20950 `Thiscall_WeaponSetupInitialize` -> `WaitForPhysicalPlayer`; 0x00C209C0 `CTCInventory_OnItemUnequipped` -> `StopWaitingForPhysicalPlayer`; 0x00C20A20 `CTCInventory_OnItemEquipped` -> `PlayPhysical`; 0x00C20B50 `CTCInventory_OnMapBrushApplied` -> `StopPhysical`; 0x00C20B00 `Thiscall_WeaponDrawSetState` -> `PlayVirtual`; 0x00C20BB0 `UIStateHandler_MarkInitialized` -> `StopVirtual`; 0x00C20BE0 `InputEvent_GetScreenCoordinates` -> `GetLevel`; 0x00C20C10 `InputEvent_ValidateForContext` -> `IsAudible`; 0x00C20DF0 `Thiscall_ActionLightningStateHandler` -> `ResetLevel`; 0x00C20E80 `CThing_SetPhysicsFlags` -> `MarkIfMatches`; 0x00C20EF0 `IsStateStreaming_2to4` -> `IsWaiting` (all `CSoundEvent::`).
- 0x00C1E990 `CombatSequence_AlignDuration` kept (the landed caller 0x00C1E9F0 uses it; it describes the body).

Already-landed sources whose names or models look wrong:
- 0x00C1E7D0 `CBlendOwner_00c1e7d0::AddCombinationBlend` is `CBlendOwner::RemoveBlend`: its callee 0x00C1E900 is `CPtrArray::Remove` (find, shift down, `--count`), which the landed comment calls `CBlendList::Add`; RemoveAllBlends 0x00C1E800 calls it until both counts reach zero.
- The sound event (vtable 0x012B3674) is landed under several placeholder names: `EnterStateAndAttach` 0x00C208E0 (it is StopWaitingForMemory: CheckState(4), list slot 8 = Remove, virtual player ReadLevel, state 1 -- the comment reads CheckState(4) as "switch to state 4" and slot 8 as "register"), `CSteeringAgent_c21620::OnCreate` 0x00C21620, `CPlayer::IsFirstPersonTargeting` 0x00C21330 (a CSoundEvent query: parameter-block mode +0x48, then the record flag byte from GetRecord 0x00C34600), chunk 431's `CPanelItem`. List slots: 8 (+0x20) = Remove, 9 (+0x24) = Add (WaitForData adds, StopWaitingForData removes, StopListening 0x00C20810 clears the flag and calls slot 8).
- 0x00C20230 `CIntelligentPointer_00c20230(void* source, void* item)` is the named item's constructor `CNamedItem(const CString& name, unsigned long hash)`: hash at +4, the CString copy (0x00C0DAB0) at +0x10 (called from 0x00C1EEE0 as `new CNamedItem(name, name.GetHash())`).
- Chunk 431's `CPanel::UpdateItems` 0x00C11CB0 (landed) declares `bool IsHandled(CPanel*)` for 0x00C20C10; retail returns a full-register long (IsAudible), and CPanel is chunk 433's CSoundEventManager.
- 0x00C20390 `CNavigationLayer::CNavigationLayer` is the 16-byte sound-event record constructor (zeroes three words and a flag byte) built inside PlayPhysical's request.
