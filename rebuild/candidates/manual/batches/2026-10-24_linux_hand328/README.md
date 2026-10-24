# Hand chunk 328 (CCreatureAction_KrakenHighAttackInto .. CDynamicObjectHeap<156,2100>): 32 of 40 byte-exact

All 32 graded MATCH / RELOCATION_MATCH (gall.sh) and their tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
Lane: RTM cl 13.10.3077.

## What matched (32)

- **Kraken creature actions** (anonymous namespace of the kraken TU; FableWin.pdb names the methods; vtable slots 3/4/12/56 =
  PerformAction / FinishAction / Initialise / FrameUpdate): 0x00924120 LowAttackInto::PerformAction, 0x00924260 HighAttackLoop::Initialise,
  0x00923FD0 LowAttackLoop::Initialise, 0x009242D0 / 0x00924040 High/LowAttackLoop::FrameUpdate, 0x009243B0 / 0x00923F60
  High/LowAttackOutOf::Initialise, 0x00924180 LowChargeLoop::FrameUpdate, 0x00923CF0 / 0x00923DA0 RiseUp::Initialise / FinishAction,
  0x00923E10 / 0x00923EC0 ReturnToWater::Initialise / FinishAction, 0x00924A90 / 0x00924670 High/LowAttackInto::FinishAction,
  0x00924B40 RoarInto::FinishAction. Shapes: the landed TentacleStrike family models (0x008CF460 / 0x008CF610 / 0x008CF700): the
  UNCHECKED out-parameter GetInterface in an inline accessor (0xe2 CTCKraken, 0x31 mode manager, 9, 0xad CTCBoss); the
  next-action temporary built inside `Creature().SetCurrentAction(CNext(Creature(), *PTarget.GetPItem(), 0, false))` behind a
  two-fetch inline alive test; FrameUpdate re-tests `Interrupt` in a nested `if` after the bool call; the ImmuneToAttack bit-field
  (CThingAICreature +0x1c8 bit 6) must be written through a NAMED local; RoarInto calls the grand-base CCreatureActionBase::FinishAction.
  Retail CTCKraken offsets are Ego_r.pdb - 0x10 after the two std::lists (HeadTrackTarget +0x51, HeadHasBeenHit +0x52,
  ReturnHeadToWater +0x5c, Phase +0x60).
- **CCreatureAction_SummonerChargeSuccess::Initialise** 0x009703D0: scoped "CHARGE_SUCCESS" name to SetCombatAnimation, then base.
- **CArray<T> (engine STLport vector) DeallocateMemory / DeallocateUnusedMemory**: 0x0051A350 / 0x0051B560 (CMapInfo),
  0x0052C580 (NThingManager::CEntityScriptInfo), 0x005E3170 / 0x005E3920 (pair<ulong, CAnimationEntry>), 0x006FC620 / 0x006FD0B0
  (pair<ulong, CSmallVector<CEvent,32>>), 0x009E7C70 (pair<CCharString, CSymbolInfo>). DeallocateMemory is the landed SElement12
  form plus the out-of-line tail destroy; DeallocateUnusedMemory is `temp.reserve(size()); temp.insert(temp.begin(), begin(), end());
  DeallocateMemory();` then the old start/finish detached with `Exchange(slot, temp.x)` and destroyed + freed (gen_carray.py).
- **ArgPasser::ArgPasser** 0x005B336A (MATCH, `#pragma optimize("s", on)`): Ego_r.pdb ArgPasser, declaration-order initialisers.
- **NCameraModes::CScriptedControl::CScriptedControl** 0x0069DFE0: base CBase(manager, def), inline CCountedPointer copy ctor,
  float 1.0f, CCamera member's inline ctor clearing HeightLocked / ViewVecZLocked (PDB offsets - 0x24).
- **C3DAnimationInfo::CAnimationData::OnResourceLoaded** 0x00A259F0: pool ShutdownFlag test, DiscardAsyncData on the bank, counted
  pointer `= 0` (release + Info/Data zeroed) and the CIVCountedPointer `operator=(T*)` of landed 0x00A25AE0; else tail BuildSharedAnimation.
- **CBasicString<char>::Insert** 0x009A06B0: two inline strlen loops, ExtendData, `n = StringLength + len; StringLength = n;
  PData[n] = 0;` (the `+=` form adds a register copy), intrinsic memset tail clear, memmove, intrinsic memcpy.
- **QuadricOptimiserInternals::CContractionReference::operator=** 0x00AD1FC0: CCountedPointer operator= with the source words in
  locals and the data store inside the different-block arm, then SubIndex.
- **CSubTitle::`scalar deleting destructor'** 0x009BDF10: non-polymorphic `??_G` via a `delete` helper; CIVCountedPointer dtor.
- **CDynamicObjectHeap<156,2100>::CreateMoreBlocks** 0x00AF88F0: link the new block, then `entry->NextEntry = FreeList; Free(entry);`
  per entry (the doubled link store).
- **CEngineLandscapeMemoryManager::AllocArrayOfObjects<CLocalDetailPrimitiveMeshZSpriteBatch::CObject>** 0x00BFBC80: __int64 size
  check wrapped as `if (valid) { ...; return objects; } return 0;`, placement-new loop over the Ego_r.pdb CObject.

## Parked for Fable 5.1 (7)

- 00924880 CCreatureAction_KrakenLowPoseInto::FinishAction DIFFER(173v172): retail keeps &PTarget in edi and pushes ebx only inside the alive arm; ours puts &PTarget in ebx (prologue push, else-arm frame offsets +4) | tried: the exact 4-argument twin form (0x00924A90 / 0x00924670 match), IsValid() on the pointer, named creature-reference local | next: lane gap: register tie (3-argument constructor); maybe compile the LowChargeLoop ctor 0x009254C0 earlier in the same TU
- 00924b90 CCreatureAction_KrakenRoarLoop::FinishAction DIFFER(219v219): retail `inc esi ; store ; mov ecx,[kraken+0Ch] ; mov edx,[ecx+48h] ; mov eax,esi ; cmp eax,edx` with this in edi; ours loads the def before the increment and swaps esi/edi | tried: `++LoopNum < def`, `LoopNum++;` statement, named def local (best, same length), CDefPointer operator->, inline GetRoarLoops | next: `++LoopNum;` then the def local declared after it (`if (LoopNum < def->RoarLoops)`)
- 009350c0 CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired DIFFER(159v159): frame slots of the variation copy (retail [esp+8]) and `1.0f + variation` (retail [esp+0Ch]) swapped, plus `fld` before `push ecx` | tried: scale from the local, scale from the member before/after the variation local, named `delay` local (fixed the final `add eax,esi`) | next: declare both float locals up front in retail slot order, or a by-value inline scale helper
- 009b79b0 CArray<std::pair<unsigned long,CDefClassBase::CSubDefInfo>>::DeallocateUnusedMemory DIFFER(153v153): retail loads the old start and does the free's null compare before loading the temporary's start word; ours one slot later | tried: Exchange, plain assign, swap + inline temp dtor (171v153) | next: lane gap: scheduling
- 009b6eb0 CArray<CInstantiatedDefInfo*>::DeallocateUnusedMemory DIFFER(135v135): same early `test eax,eax` scheduling | tried: the 0x009B79B0 forms | next: lane gap: scheduling
- 00a725d0 CArray<SElement12>::DeallocateUnusedMemory DIFFER(153v153): same as 0x009B79B0 | tried: Exchange form | next: lane gap: scheduling
- 009b5db0 CArray<unsigned char>::DeallocateUnusedMemory DIFFER(129v129): same as 0x009B6EB0 | tried: Exchange form | next: lane gap: scheduling

## Skipped

- 00ca53c0 CConsoleCommand<CNewFrontendGameComponent>::CallCommandFunction: the 39-byte row holds two functions -- the 15-byte
  thunk at 0x00CA53C0 (`mov eax,[esp+4] ; mov eax,[eax-8] ; mov ecx,[eax] ; mov [esp+4],eax ; jmp [ecx]`), one int3, then a second
  23-byte body at 0x00CA53D0 (stores both arguments at +0x150 / +0x154, `ret 8`). Correct length of the row's function: 15.

## Label corrections

- Kraken `vfunc_N` rows: slot 3 PerformAction, 4 FinishAction, 12 Initialise, 56 FrameUpdate (FableWin.pdb + the landed TentacleStrike vtables).
- 0x007FC7A0 is CTCKraken::StartLowAttackBeam (DelayBeforeLowBeam +0x4c, mode 4); the manifest's StartHighAttackBeam is 0x007FC7D0.
- 0x008574D0 is CCombatActionBase::SetCombatAnimation(const CCharString&) (FableWin.pdb); manifest "SetPDef", landed files call it SetName / SetAnimationName.
- 0x00925780 / 0x00925820 / 0x009255E0 / 0x00925680 / 0x009254C0 / 0x00925920 / 0x009259B0 are the Kraken HighAttackLoop / HighAttackOutOf /
  LowAttackLoop / LowAttackOutOf / LowChargeLoop / RoarLoop / RoarOutOf constructors (manifest: BattleChargeTurnLeft, CastWillGhostSwordSpell...).
- 0x0069DFE0 is NCameraModes::CScriptedControl::CScriptedControl (vtable 0x0125C58C), not CActionParamCCountedPointer<NUISystem::CComponent>.
- 0x009BDF10 is CSubTitle::`scalar deleting destructor' (CMovieSubtitles TU), not CDefPointer<CHeroAbilityDef const>.
- 0x00AD1FC0 is QuadricOptimiserInternals::CContractionReference::operator=, not CCountedPointer<CDiskFileWin32>::operator=.
- 0x00A259F0 is C3DAnimationInfo::CAnimationData::OnResourceLoaded (vfunc_5).
- 0x00AF88F0 is the TLC instantiation CDynamicObjectHeap<156,2100> (block 0x4FFB4); Ego_r.pdb has <164,1998>.
- CArray element types: 0x0051A350 / 0x0051B560 CMapInfo; 0x0052C580 NThingManager::CEntityScriptInfo; 0x009B6EB0 CInstantiatedDefInfo*;
  0x009B79B0 pair<ulong, CDefClassBase::CSubDefInfo>; 0x009E7C70 pair<CCharString, CSymbolInfo>; 0x00A725D0 SElement12 (CPhysicsMesh).
- 0x005B336A is the ArgPasser constructor (Ego_r.pdb ArgPasser, CTCInventoryClothing TU).
