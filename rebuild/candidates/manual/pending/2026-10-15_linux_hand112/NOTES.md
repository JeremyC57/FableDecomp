# Hand chunk 112 (CThingPhysical .. CThingTrackNode): 38 of 40 byte-exact

All 38 matched rows graded MATCH/RELOCATION_MATCH from the exact file in `src/` (`grades.txt`) and their
behaviour tests print `<addr>_TEST PASS` under g++ (`tall.sh`) and the lane's VC7.1 cl.exe (`tall.py`).
Layout note used throughout: the retail CThing is the Ego_r.pdb layout minus 0x10 from +0x48 on
(PhysicsTC +0x60, ThingTypeFlags +0x6c, PBaseDef +0x70, flag bytes +0x91/+0x92, sizeof 0x98);
CThingPhysical is PDB minus 0x18 after its 4-byte HealthOverTimeEffects list (Scale +0x9c, MaxHealth +0xb0,
Health +0xb4, CanComeBetweenCameraAndObservedThing +0xb8); CThingCreatureBase::PDef +0xe0; the player
creature's movement speeds/defs sit 0x28/0x2c below the PDB; CDefBase-derived defs are 4 bytes narrower.
The generated `CThingPhysical.h` mixes PDB and retail offsets in that range, so those rows use local models.

## What matched (38)
- **CThingPhysical** (7): `CanBlockNavigatingCreature` 006a4d90 (the physics TC in a named local loads straight
  into ecx; `&&` chain with explicit true/false); `IsInRange` 006a4df0 (an inline C3DVector `operator-` temporary held
  on the x87 stack, `GetLengthSquared() < range*range`, long result); `vfunc_35` 006a4e70 (void slot-88 call +
  `return true`); `GetNoNavCollisionLineLists` 006a4f10 (by-value CPhysicsMeshObject temporary from a hidden-pointer
  getter, chained call, out-of-line destructor); `GetNavCollisionLineList` 006a4f60 (the member re-read for every use
  through an inline accessor, named mesh-object local); `SetMaxHealth` 006a5260 (the final clamp is an inline helper
  taking the maximum by const reference -- a plain `if (Health > MaxHealth)` forwards the stored parameter);
  `IsThingAbleToComeBetweenCameraAndObservedThing` 006a5ab0 (by-value CDefPointer copy through an inline `GetDef()`,
  null-tested AddRef, release WITHOUT null test, one-bit def flag, `return setting == 1` full-register bool).
- **CSoundManagerLH** (1, catalogued as CThingPhysical): `vfunc_2` 00a3e3e0 (vtable 0x0129D2C4 slot 2; the member
  re-read for the second slot-3 call; `!flag` forwarded by a sibling `jmp`).
- **CThingPhysicalSwitch** (1): `SerialiseGameState` 0083d940 (base call as the `if` condition with the body inside and
  `return false` after it -- an early `return false` drops retail's `xor al,al`; `Mode != 1 && Mode != 3`).
- **CThingPlayerCreature** (11): `vfunc_75` 00661fd0 (one-bit unsigned char bitfield through the def pointer);
  `vfunc_69` 006635d0 (GetAnimFromName(name, 0)->first word into slot 68); `OnDie` 006a9590 (player local, base call
  tail jump); `GetCurrentRelativeMovementAngleXY` 006a9640 (engine local, by-value 0x38-byte transform temporary);
  `vfunc_4` 006a9960 (base call in the `if`, `fst` + `fucompp` for `Health == 0.0f`, Drawable bitfield `or`);
  `GetMaxTurnSpeed` 006a99d0; `ResetMaxWalkingSpeedToDefault` 006a9b70 / `ResetMaxRunningSpeedToDefault` 006a9bc0 (named
  CCharString local, member address in edi); `UpdatePlayerControl` 006ad970 (the landed out-parameter GetInterface
  template, type 0x1f); `ResolveMovementBandType` 00c61ca0 / 00c62400 (particle initialisers, label propagated; the
  second calls a helper of the same TU that must be defined above it under `#pragma auto_inline(off)`).
- **CThingPropertyDialog** (1): `SetAsAttachingWithMode` 006fd170 -- really a 12-byte-element vector `resize(n, T())`:
  zero stores as IMMEDIATES after the pushes = float members; the trailing `push 0 ; call delete[]` reproduced by a
  never-filled scoped array holder (see LESSONS review item).
- **CThingSearchTools** (2): `GetPFirstLeafThing<Bind2<HasDefName,HasScriptData>,CThingMarker>` 005800d6 and
  `<CThingFilter_HasTC,CThingBuilding>` 005c0d9d -- one member template under `#pragma optimize("s", on)` (unaligned loop
  head, `jmp` to the shared epilogue); the Bind2 `operator()` must be `__forceinline` under /Os; list type via a
  `TThing::ThingListType` trait.
- **CThingSoundEmitter** (5): constructor 008367b0 (`#pragma pack(1)`: PAttachedThing at +0x9d; the intelligent
  pointer's inline derived constructor stores its vtable after the out-of-line base constructor);
  `PlaySample`/`PlaySoundEvent`/`IsSamplePlaying`/`IsSampleLoadedAndPlaying` 00836810..008368a0 (inline
  `g_pGameState && !Dead` guard, sibling `jmp` through the +0x98 sound object, invalid-handle / false fallback).
- **Deleting destructors** (10, all catalogued CThingSwitch/CThingTrackNode): CThingSwitch 00529ba0 (no user destructor:
  generated one stores no vtable; CThing class `static void __fastcall operator delete`); non-polymorphic `??_G` from a
  `delete` helper for CBoast 004b72f0, pair<CCharString, quest CPersistInfo> 004b77a0, pair<long, CBoast> 0056cba0,
  pair<CCharString, CSymbolMap::CSymbolInfo> 006fb730 (inner record's inline destructor puts its address in edi);
  polymorphic `??_G` with a `new` factory for NUISystem::CChangingStateComponent 0052d770 (three vtables),
  NCameraModes::CCameraPointModeBase 0056e2f0, CTCRegionDisplay 0063a230 and CTCActionUseScriptedHook 0072a450 (the
  destructor body calls CTCBase::RemoveFromGlobalTCs and must be `__forceinline`), CTCHighlightItem 0077d140 (body
  calls DoStopRunning).

## Parked for Fable 5.1 (2)
- 00666850 CThingPlayerCreature::vfunc_71 DIFFER(76v74): `mov eax,[esp+8] ; lea esi,[eax+60h] ; mov edi,eax` where retail loads `mov edi,[esp+8] ; lea esi,[edi+60h]` (the def pointer kept in edi across the map find, released with no null test) | tried: CDefPointer local with `--m_pDef->m_refCount` destructor (83v74), local-copy destructor + `!=` ternary (84v74), null-testing Release destructor (88v74), explicit release through a raw local + cleared member (76v74, kept) | next: lane gap: register copy (the natural CDefPointer form re-reads the frame slot after the find call; retail CSEs it -- RTM alias assumption for an address-taken local)
- 00436500 CThingSearchTools::GetMapwhoArea DIFFER(64v66): `shl eax,4 ; add eax,137DAD8h ; mov ecx,[eax+4]` (+ `push ebx` scheduled after it) where the lane folds `mov ecx,[eax+137DADCh]` | tried: four-argument constructor return (64v66, field order wrong), named local built field by field + memberwise copy (64v66, only the address fold left, kept), by-value copy of the table entry (92v66) | next: lane gap: RTM materialised table-entry address (GOTCHAS 24l "vector slot address" family)

## Skipped
- none (no row holds two functions).

## Label corrections
- 00a3e3e0 "CThingPhysical::GetNoNavCollisionLineLists" -> CSoundManagerLH::vfunc_2 (vtable 0x0129D2C4 slot 2).
- 006a9b70 "ResetMaxRunningSpeedToDefault" -> ResetMaxWalkingSpeedToDefault (stores +0x190 from def +0x90, passes "ST_WALK";
  the running twin 006a9bc0 passes "ST_RUN").
- 00c61ca0 / 00c62400 "CThingPlayerCreature::ResolveMovementBandType" -> particle-component initialisers in the 0x00C6xxxx
  particle code (no creature member used); line 1 keeps the catalog name, the model is `CInitialiser`.
- 006fd170 "CThingPropertyDialog::SetAsAttachingWithMode" -> a 12-byte-element `vector::resize(n)` wrapper (`this` handed
  to 0x006FCF90 resize(n, const T&)); line 1 keeps the catalog name.
- Deleting destructors catalogued "CThingSwitch::`vector_deleting_destructor'": 00529ba0 is CThingSwitch (vtable 0x012453FC)
  but 004b72f0 = CBoast, 004b77a0 = pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>,
  0056cba0 = pair<long, CBoast> (inferred), 006fb730 = pair<CCharString, CSymbolMap::CSymbolInfo> (all non-virtual `??_G`).
- "CThingTrackNode::`vector_deleting_destructor'": 0052d770 = NUISystem::CChangingStateComponent, 0056e2f0 =
  NCameraModes::CCameraPointModeBase, 0063a230 = CTCRegionDisplay, 0072a450 = CTCActionUseScriptedHook, 0077d140 =
  CTCHighlightItem (all from rtti-vtables.tsv slot 0).
- Callee labels seen wrong: 0x004C79A0 "CAnimateThing::GetMeshBank" (returns a world sub-object, ignores `this`),
  0x0052D5C0 "CGuiControlTreePane::SortTreeRecursively" (a list destructor), 0x0056E0E0 "~CTCActionUseBed" (camera-mode
  base destructor), 0x00686830 "~CChunkedFileChunk" (= ~CTCBase), 0x00C623F0 "list iterator operator--" (a `this`-unused
  pointer-chain helper), 0x00C2D100 "CTCLook::UpdateLookTimer" and 0x00C1BB00 "CGraphicFrameData::GetFloatTLX" (particle
  helpers), 0x006A80A0 "Bitset_Test" (= CThingFilter_HasTC::operator()).
