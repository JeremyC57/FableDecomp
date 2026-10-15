# Hand chunk 140 (CGameScriptInterface .. CGameScriptInterface): 32 of 40 byte-exact

All 40 rows are CGameScriptInterface methods (labels verified against the vtable 0x01260F0C where a slot exists and the
bodies' behaviour). Every matched row grades RELOCATION_MATCH with default flags (no pragma); tests PASS under g++ and
the lane VC7.1 (tall.sh / tall.py).

## What matched (32)
- **Msg family (15: 008933f0 MsgOnRegionLoaded, 00893470 MsgOnRegionUnloaded, 008934f0 MsgOnRegionPreunload, 00893610
  MsgOnAnyQuestCompleted, 00893870 MsgOnQuestAccepted, 008938f0 MsgOnFeatAccepted, 00893960 MsgIsBoastMade, 008939d0
  MsgOnBoastMade, 00893b00 MsgOnExpressionPerformed, 00893d80 MsgOnHeroUsedTeleporter, 00893ff0 MsgOnHeroPickedPocket,
  00894080 MsgOnHeroPickedLock, 00894110 MsgOnFishingGameFinished, 00894250 MsgOnHeroRewardedWithItemsFrom, 008943c0
  MsgIsTutorialClickedPast)** -- find the most recent message of one type seen/heard between the current frame (0x006E7510)
  and the max frame (0x006E7530) and copy its payload word (+0x3c: region index -> CWorldMap::GetRegion name, quest index ->
  CQuestManager::GetRegisteredQuestFromIndex, feat id, boast record, def global index, CThing* -> GetScriptThing, reward
  record name +0x74). Shape: the landed MsgIsCutSceneSkipped filter model (`IsOfType(t) && CanBeSeenOrHeard(f, m)`, Bind2 of
  pointers) as is; `- 1` on either frame bound reproduces the `dec edi` / `dec eax` variants; MsgIsBoastMade is the Bind3
  chain `IsOfType && HasExtraData(boast) && OccuredBetween` (0x008ABF20) and VC builds the IsOfType temporary in the dead
  argument slot by itself; a GetString result copied with `lea ; push` is a NAMED CCharString local (an unnamed temporary
  pushes eax); MsgIsTutorialClickedPast's `setne` tail is `return message ? true : false;` (`message != 0` gives
  neg/sbb/neg), the tutorial switch is the global byte 0x01375740 and an inline profile accessor `+0xc ? +0xd7 : +0x17`.
- **Hero component queries (6: 008953a0 IsHeroPerformingExpression, 00895430 IsDPadButtonHeldForExpression, 00895a10
  GetBestFishWeight, 00895a90 HeroGoFishing, 00895bc0 IsHeroPlayingOracleMinigame, 008966c0
  GetNumberOfKeysNeededToUnlockChest)** -- the landed GetHeroFishingLevel model (hero via 0x00449970/0x00487DC0, live bit,
  `__forceinline` GetInterface) unchanged; interface types 0x8f expression (current expression +0xc, slot 10 GetDefName ==
  name via 0x006AEB10, an empty name short-circuits to true), 0x11 inventory, 0xb1 fishing, 0x107 oracle (`+0x14 != 0`
  gives `xor ecx,ecx ; setne cl`), 0x3c chest (no live test, the key in the dead argument slot).
- **Script-thing setters (8: 0088eb30 EntitySetAsKillable, 00890470 EntitySetAsPickPocketed, 008904e0 EntitySetAsStolen,
  00890ba0 EntitySheatheWeapons, 00890c10 EntityUnsheatheWeapons, 00894dd0 EntitySetAsMarryable, 00895010
  EntitySetAsReceivingEvents, 006e7480 CleanupActions)** -- AddMessage (0x00687540) callers need `C3DVector position(0,0,0)`
  declared before `CMessageEventManager* messages = World->...` (zero stores before the manager load, pushes after);
  Unsheathe needs `if (!IsPlayer()) {...} else UnsheatheMeleeWeapon();` (the player tail sits last) while Sheathe returns early;
  AddTC("CTCEntityEvents"/"CTCWife") takes a NAMED `CCharString name("...", -1)` so the string is built before the 0,0 pushes
  (VC homes it in the dead bool argument slot); Marryable stores one bool into two adjacent bit fields (+0x1c9 bits 3/4);
  Killable stores +0xbc bit 1 and calls vslots 526/527 then `SetAsGivingHeroStatChangesOnBeingHit(killable ? true : flag)`;
  CleanupActions inlines UnfreezeControls (0x006E73E0) because it is called QUALIFIED (`CGameScriptInterface::Unfreeze...`),
  while EndLetterBox (slot 378) stays virtual; the camera is CWorld+0x34 PCurrentGameCamera slot 14 ScriptEnd.
- **Region queries (3: 00891db0 IsRegionDefLoaded, 00891e30 GetWanderingPopulationScriptDefNameInRegion, 00892d80
  IsCreatureGenerationEnabledForRegion)** -- the region record's counted def pointer (+0x24) copied into a local
  `CCountedPointer<CRegionDef> def(region.m_pDef.Data)` (constructing from the raw pointer; copying the member by reference
  keeps `add eax,24h`) with an inline `AddRef()` member (`inc dword ptr [esi+4]`; a bare `++m_refCount` in the ctor gives
  load/inc/store); the region def's +0x74 CDefString index feeds CDefStringTable::GetString into a named local;
  IsCreatureGenerationEnabledForRegion uses the out-of-line `CCharString::operator!=(const char*)` 0x0099E960 and returns
  the region record's +0x55 flag through CWorld slot 12 (GetWorldMap) + 0x004FB490.

## Parked for Fable 5.1 (8)
- 0088e150 CGameScriptInterface::RespawnHero DIFFER(111v111): `sub esp,30h ; lea ecx,[esp+4] ; mov byte ptr [esp+0Ch],al` vs retail `sub esp,2Ch ; lea ecx,[esp+34h] ; mov byte ptr [esp+3Ch],al` (AddUBYTE's by-value byte gets a new frame byte instead of the dead flag argument slot; every frame offset shifts by 4) | tried: inline AddUBYTE(unsigned char), __forceinline AddUBYTE, inline AddBOOL(bool), __forceinline AddBOOL(bool); all /GS | next: copy the flag into a by-value parameter of a non-member __forceinline helper taking (CGameEvent&, bool), or `#pragma optimize("y", off)` variants of the landed 0x005BC527 recipe
- 0088f570 CGameScriptInterface::Play2DSound DIFFER(150v147): retail `push edi ; mov edi,1` reused for NoPlays, PlaybackType and the `attached` push, CSoundInit stores in the order +0,+4,+8,+c,+18,+1c,+10,+14, engine global loaded before the symbol argument; ours stores immediates in declaration order and pushes `1` | tried: inline default ctor, argument-taking ctor with defaults, optimize("s") | next: a `long one = 1`-style shared local or a ctor taking (NoPlays, PlaybackType) as one variable; otherwise lane gap: constant-register CSE
- 00892cf0 CGameScriptInterface::SetCreatureGeneratorsEnabled DIFFER(117v117): `xor eax,eax ; repe cmpsb ; sete al ; test al,al` (ours) vs `xor eax,eax ; repe cmpsb ; pop edi ; pop esi ; je` (retail branches on the strcmp flags) with the 0x004115A0 call path moved to the end | tried: inline operator==(const char*) as if/return, ternary, `== 0 ? true : false`, inverted if/else at the call site | next: an `operator==` that returns `!strcmp(...)` or a free `operator==(const CCharString&, const char*)`; check other landed callers of 0x004115A0 with the inline null-block strcmp
- 00894bf0 CGameScriptInterface::IsGiftRomantic DIFFER(170v159): `push ecx ... sete byte ptr [esp+0Ch]` and `lea eax,[ecx+4] ; dec edx ; mov [eax],edx` vs retail `push ebx ; sete bl` and `dec dword ptr [ecx+4] ; jne` on the failure-path gift release | tried: CDefPointer dtors, hand releases via an inline helper, scoped class-def slot, one shared out-slot (fixed the slot and class-def releases), gift pointer read before the branch, direct load/dec/store success release (fixed it), nested failure release | next: release the gift through a CDefPointer-like local whose destructor skips the null test, or compute the result into a `long` before the release
- 00894c90 CGameScriptInterface::IsGiftFriendly DIFFER(171v158): same as 0x00894BF0 (`test edx,edx ; sete bl` in retail) | tried: as 0x00894BF0 (the last three forms were tried on this row) | next: as 0x00894BF0
- 00894d30 CGameScriptInterface::IsGiftOffensive DIFFER(170v159): same as 0x00894BF0 (`cmp edx,2 ; sete bl`) | tried: the best 0x00894BF0 form (one grade) | next: as 0x00894BF0
- 008955c0 CGameScriptInterface::IsEntityFollowingHero DIFFER(138v136): `mov eax,[eax+4] ; test eax,eax ; je ; push ebx ; mov ecx,eax` vs retail `mov ecx,[eax+4] ; test ecx,ecx ; je ; push ebx` | tried: GetInterface + `&& followed`, __forceinline accessor returning the component (out-pointer and direct LowerBound forms) | next: lane gap: register choice for an inlined component used as `this` (also 0x00896120)
- 00896120 CGameScriptInterface::TryToRespawnDefNamed DIFFER(145v143): `mov ecx,[esp+10h] ; mov eax,[eax+4] ; push edx ; push ecx ; mov ecx,eax` vs retail `mov ecx,[eax+4] ; mov eax,[esp+10h] ; push edx ; push eax` | tried: checked GetInterface, HasInterface + unchecked GetInterface, HasInterface + lookup-only __forceinline accessor, non-forceinline GetInterface (not inlined) | next: lane gap: register choice (same as 0x008955C0); the empty-CScriptThing return (CBase ctor + vtable + two zeros) already matches

## Skipped
- none (no row spans two functions).

## Label corrections
- 0x004FB150 "CWorld::DrawGetEnvironment" is CWorldMap::GetCurrentRegionNumber; 0x004FB490 "CTCInventoryBase::GetCategoryWithIndex" is a CWorldMap region-record getter (same body as GetRegion 0x004FC180, reached through CWorld slot 12).
- CWorld vtable slot 12 is GetWorldMap and slot 13 PeekWorldMap (rtti 0x012390F0); older models call slot 13 GetWorldMap.
- 0x004AFC60 "COptimisedPrimitive::DetectFoldOver" is a bool CQuestManager method on the global 0x013B89FC (walks +0x40..+0x44).
- 0x006AEB10 "std::_Iterator_base12::_Getcont" is CDefString::operator==(const CCharString&) (twin of 0x004B7980).
- 0x004C7E50 "GFPredicateAnd<...>" is CThing::GetScriptThing; 0x004AA840 "~C3DClothPrimitive" is ~CScriptThing; 0x0099EAE0 "_Cons_val<...>" is ~CCharString.
- 0x004C9840 "CBaseObject::OnPropertyChanged" is CThing::RemoveTC(long) (called with the interface type 0x42).
- 0x00449700 "CWorld::DrawGetEnvironment" is CPlayerManager::GetMainPlayerNumber (`mov eax,[ecx+1Ch]`); 0x00487DC0 / 0x00487DD0 "_Cons_val<...>" are CPlayer::GetHeroThing / GetControlledCreature.
- 0x006E7510 is labelled GetMaxWorldFrameForMessages but reads +0x18 (the current frame); 0x006E7530 reads +0x1c (the max frame).
