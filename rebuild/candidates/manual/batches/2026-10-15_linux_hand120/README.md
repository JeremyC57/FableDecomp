# Hand chunk 120 (CAIStateGroup_GazeAtHome .. CAIStateGroup_MeleeCombatNearbyBase): 25 of 40 byte-exact

All 25 matched sources grade RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under g++
(tall.sh) and the lane's VC7.1 cl.exe (tall.py). No flags were needed; two constructors carry
`#pragma optimize("s", on)`. Every source models `this` locally: the generated engine headers for these
classes (and CAIStateGroupBase.h / CAIBrain.h) are flattened data-only layouts without the vptr chain, and
every row needs virtual slots or member functions on the base.

## What matched (25)
- **CAIStateGroup_GazeAtHome / GazeOutside** -- `InitStateGroup` 0x00918560 / 0x00918480: `GazeMarker =
  CIntelligentPointer<CThingGameObject>(0)` (stack temporary: base ctor, push 0, vtable, SetPItem; copied with
  SetPItem(GetPItem()); base dtor) then the inlined out-parameter GetInterface(0xd3) guarding
  `RegisterIdleStateGroup(this, 7 / 8)`; the mangled `MAEXABVCWorld@@AAVCThingAICreature@@@Z` needs `class`
  forward declarations and a protected virtual. `IsRunnable` 0x009189C0: nested `if (GetInterface) { if
  (!IsPreferredIdle) return false; }` (returns the call's own `al`) then `return GetOwner() &&
  FindFreeGazePos();` (`&&` = full-register `mov eax,1` / `xor eax,eax`).
- **CAIStateGroup_GoToPub** -- ctor 0x00900173: base ctor, vtable, five `CIntelligentPointer<T>()` members;
  the vtable shared by PBar/PTable is cached in ebx only under `optimize("s")`. `SitIdle` 0x00947CA0 /
  `Laugh` 0x00947D30: named CCharString, CSoundPair() temporary, CCreatureAction_PlayAnimation temporary in
  the PerformAction call; the creature MUST come through an inline `GetCreature()` accessor (a direct
  `PBrain->PCreature` swaps eax/ecx/edx); Laugh is `bool failed = .. != 2; return !failed;`.
  `InitStateGroup` 0x00947DB0: body stores in source order + the intelligent-pointer temporary.
  `Listen` 0x0094A190: three explicit `CCharString("..", -1)` temporaries in the PlayAnims call.
- **CAIStateGroup_HoverHornetQueen** -- `PlayIdle` 0x0095B680: named string in an inner block, `false`
  pushed before the out-of-line PlayCombatAnimation ctor, base dtor (0x0062E900 = ~CCombatActionBase), then
  `if (!IsStillRunnable()) Terminate();`.
- **CAIStateGroup_JackDragon** -- ctor 0x00900647 (seven pointer members, `optimize("s")`). `StayIdle`
  0x0094FE30: do/while with `GetPWorld()->GetWorldFrame()` (static still calls the accessor) and
  `(float)(frame - start) / g_framesPerSecond < seconds`. `SetHeadTurning` 0x00950BB0: GetInterface with the
  result ignored, then `if (type == 0x4e) store; else if (type == 0x47) store;` (the duplicated tails are
  cross-jumped; the `||` form leaves `mov ecx,esi` after the `je`). Row 0x00624800 (catalogued as
  `~CAIStateGroup_JackDragon`) is the SGI `__pop_heap<CTestQuest*, CTestQuest, CTestQuestNameCompare>` (/Gr
  `__fastcall`, `ret 28h`, the 28-byte value destroyed by the callee: six CCharString dtors).
- **CAIStateGroup_JackOfBladesBase** -- `Initialise` 0x00972040: base Initialise forwarded, GetInterface(0xad)
  result ignored, `SetNextBossPhase(0, false)`, boss +0x10 cleared. Row 0x00D72160 (catalogued as the
  JackOfBladesBase ctor) is `CHTG_Audience::CHTG_Audience` (the landed CPermanentBarrier entity-script shape
  with three CScriptThing members; CBase must own the vptr).
- **CAIStateGroup_KidReactToBlockage** -- `InitStateGroup` 0x00941CF0 / `OnReplace` 0x00941D70: the landed
  KidGroupTag random helper with a float span: `(long)(g_framesPerSecond * 5.0f + (float)RandomFramesWithin(10.0f))`;
  OnReplace reads the frame as `GetPThingWorld()->GetCurrentFrame()`.
- **CAIStateGroup_KrakenTentacleAttack / Idle / Rise** -- `Update` 0x009251E0 / 0x009250F0 / 0x00925170: an
  inline derived action ctor over the out-of-line `CCombatActionBase(creature, type, CSoundPair())` inside
  PerformAction[AndWait]; Idle is `while (!IsStopping()) { if (PerformAction(..) != 2) break; } Terminate();`.
- **CAIStateGroup_LightHomeCandle** -- `ShouldBeLit` 0x00946690: const GetInterface (0x004365B0), `if (villager
  && villager->GetPVillage())`, the village def copied into a pointer local with a null-tested AddRef and a
  hand-written release, world as a named local, time manager through an inline accessor.
- **CAIStateGroup_MazeShieldedChase** -- `Cleanup` 0x00954900: GetInterface(8) result ignored, component +0x40
  `= CIntelligentPointer<CThingCreatureBase>(0)` (the temporary shares the key's frame slot).
- **CAIStateGroup_MazeTutorialBattleRespondToLightning** -- `FindHitEvent` 0x00954C60: nested `if
  (GetInterface(0x42)) { if (HasEvent(CIsEventOfTypeCreatedAfter(0x1b, FrameOfLastHit))) return true; }`.
- **CAIStateGroup_MazeTutorialBattleUseShield** -- `Update` 0x00956460: `if (IS_THING_ALIVE(PTargetThing) &&
  !IsStopping()) { PerformActionAndWait(&WillShieldInto(GetCreature(), 3.0f, 5.0f), false); FrameOfLastShield
  = GetPWorld()->GetCurrentFrame(); } Terminate();` (GetCreature() accessor again fixes eax vs ecx).
- **CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose** -- `Cleanup` 0x0093E560:
  `creature->GetCombatManager()->DeregisterCombatant(creature)` then GetInterface(0x31) + RemoveMode(0x19).

## Parked for Fable 5.1 (15)
- 00918a40 CAIStateGroup_GazeAtWork::IsRunnable DIFFER(114v116): `mov ecx,[eax+4]` + `pop esi` after GetShopTC (retail `mov eax,[eax+4] ; pop esi ; mov ecx,eax`), shared `xor al,al` tail placed mid-body | tried: out-param GetInterface, HasTC + pointer GetTC (x2), bool local | next: `&&` single-expression form; else lane gap: shrink-wrap pop placement
- 00947e20 CAIStateGroup_GoToPub::GetPaymentRequestReplyMessage DIFFER(132v132): `lea ecx,[esp+0Ch] ; mov [esp+24h],ecx` (Bind3.F1 = &IsOfType) stored right before the WaitForMessage pushes (retail: right after the first filter ctor) | tried: left-assoc `&&` chain, right-nested `A && (B && C)`, direct 3-ref Bind3 (best), by-value Bind2 | next: lane gap: argument/temporary scheduling of the filter chain
- 009492a0 CAIStateGroup_GoToPub::GetPayPoint DIFFER(118v116): component kept in eax and creature in ecx then `mov ecx,eax` (retail brain in edx, component straight into ecx, creature in eax) | tried: direct PBrain->PCreature arg, HasTC + GetTC, creature local (best), GetTC local + creature local | next: lane gap: object-first evaluation of the member call
- 009777a0 CAIStateGroup_Guildmaster::IsRunnable DIFFER(128v128): `lea edi,[esi+18h]` hoisted above the temporary's GetPItem call | tried: operator= from temp, optimize("s"), explicit SetPItem(temp.GetPItem()), scoped named temp | next: lane gap: scheduler hoists the CSE'd member lea above a call
- 0095a2c0 CAIStateGroup_HoverAttackArea::IsRunnable DIFFER(159v159): `lea edi,[esi+14h]` hoisted above GetPItem (same as 009777a0) | tried: explicit temp, implicit conversion, void operator= | next: lane gap: as 009777a0
- 0095a420 CAIStateGroup_HoverHornetQueen::IsStillRunnable DIFFER(97v97): this in esi / target in edi (retail swapped) and the merged `return false` placed after the second alive test | tried: nested-if alive helper, macro (re-reads the vector), `&&` helper + ternary tail (best), named target pointer | next: lane gap: callee-saved register choice + tail-merge placement
- 00972110 CAIStateGroup_JackOfBladesBase::Cleanup DIFFER(143v141): `mov eax,[eax+4Ch] ; push edi ; mov ecx,eax` (retail `mov ecx,[eax+4Ch] ; push edi`) | tried: combat-manager local (best), direct chain (pushes before the world call, 139) | next: lane gap: object-first evaluation of `GetPWorld()->m_pCombatManager->Deregister(local)`
- 0093e240 CAIStateGroup_MeleeCombatNearby2::Cleanup DIFFER(143v141): same as 00972110 (byte-identical shape) | tried: state-group GetPCombatManager() accessor | next: lane gap: as 00972110
- 00954a20 CAIStateGroup_MazeDefensiveMeleeCombat::OnReplace DIFFER(131v129): same combat-manager instruction pair as 00972110; the HasMode/RemoveMode half matches | tried: manager local (1 grade, shape known) | next: lane gap: as 00972110
- 00972340 CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush DIFFER(168v142): `sub esp,8` (key temp and the GetSubDef pointer local in separate slots; retail one `push ecx` slot) and the releases go through `mov esi,[eax+4] ; add eax,4 ; dec esi` | tried: release on the address-taken local (166), pointer copy (168), Release() member + hits local | next: lane gap: QFE never shares a frame slot between the inline key temporary and a later address-taken local
- 009191b0 CAIStateGroup_LookAtInterestingThings::OnReplace DIFFER(144v140): `xor edx,edx ; div` sunk below the GFGetCurrentFrame call | tried: one expression, random + fps locals (best), precomputed delay (136), frame local | next: lane gap: div scheduled past a call
- 00954980 CAIStateGroup_MazeDefensiveMeleeCombat::IsRunnable DIFFER(159v157): GetInterface phi in eax then `mov ecx,eax` before the virtual call (retail both arms load ecx) | tried: out-param form, pointer-returning GetTC<T>() | next: lane gap: phi register preference
- 00954ab0 CAIStateGroup_MazeTutorialBattle::IsRunnable DIFFER(159v157): byte-identical twin of 00954980, same diff | tried: the 00954980 forms (1 grade) | next: lane gap: as 00954980
- 00954740 CAIStateGroup_MazeShieldedChase::IsStillRunnable DIFFER(136v136): this in esi / &PTargetThing + map in edi (retail swapped), everything else equal | tried: flat HasTC return, nested HasTC (best), template IS_THING_ALIVE + split tests, base IsStillValid() helper | next: lane gap: as 0095a420
- 0093d720 CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack DIFFER(132v128): add-ref split into `mov ecx,[esi+4] ; ... inc ecx ; mov [esi+4],ecx` around the +0x9d flag load (retail `inc dword ptr [esi+4]`) | tried: `++def->m_refCount`, inline AddRef, volatile count, CDefPointeeBase __forceinline AddRef | next: lane gap: RMW split; maybe the CDefPointer copy-constructor form with a hand guard

## Skipped
- None: no row in this chunk contains two functions.

## Label corrections
- 0x00624800 "CAIStateGroup_JackDragon::~CAIStateGroup_JackDragon" -> `__pop_heap<CTestQuest*, CTestQuest, CTestQuestNameCompare>` (SGI, /Gr); callers 0x0062494E/0x00624D4E/0x00625054/0x00625270.
- 0x00D72160 "CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase" -> `CHTG_Audience::CHTG_Audience(CQ_HangingTreeGoodScript*, CQ_SunnyvaleMasterData*, const CScriptThing&)` (factory 0x00D720E0 allocates 0x40; vtable 0x012D292C has no RTTI).
- 0x0062E900 "CTCLightningOrb::~CTCLightningOrb" -> `CCombatActionBase::~CCombatActionBase` (follows the 0x00858030 CCombatActionBase ctor in the Kraken rows).
- 0x0091A670 "CCOE_AltarEvil::Main" -> `CAIStateGroupBase::AIAction_WaitForMessage<CFilter_Bind3<IsOfType, IsOriginatedByThing, IsTargettedAtThing>>(long, const Filter&, CMessageEvent**)`.
- 0x00956760 "CCombatAction_WillEnflameInto" ctor -> `CCombatAction_WillShieldInto(CThingCreatureBase*, float, float)` (stores vtable 0x0129461C).
- 0x008FCF90 "CPixelShader::CountReferences" -> `CAIStateGroupBase::GetPPlayerManager` (PBrain->+0x20).
- 0x00487DC0 "_Cons_val<..>" -> `CPlayer::GetPCharacter` (`add ecx,2Ch ; jmp GetPItem`).
- 0x006621A0 "CPersistContext_TransferableSubComponent<CCreatureStatsDef>::AreDefaultValsApplied" -> a creature flag query (bit 1 of `[this+0E0h]+148h`), used as "uses creature modes".
- 0x004FB150 "CWorld::DrawGetEnvironment" -> CWorldMap current-region index getter (`[this+9Ch]`).
- 0x008FCFC0 "CPlayer::GetCurrentMode" -> `CAIStateGroupBase::GetPWorld` (as landed sources already call it).
- 0x0099EAE0 "_Cons_val<..>" -> `CCharString::~CCharString`; 0x00693EF0 "CPhysicsMeshInfo::~CPhysicsMeshInfo" -> `CCreatureActionBase::~CCreatureActionBase` (already noted by landed sources).
- 0x006AF290 is named IsOriginatedByThing by its landed source; the PDB `CFilter_Bind3<IsOfType, IsOriginatedByThing, IsTargettedAtThing>` and its use in 0x00947E20 (creature filter in F3) indicate it is `CMessageEventFilter_IsTargettedAtThing(const CThing*)` (bodies are identical, so only the label differs).
