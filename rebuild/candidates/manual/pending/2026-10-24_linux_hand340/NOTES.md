# Hand chunk 340 (NCreatureAction::CActionEventCameraShake .. NCreatureMode::CModeStunned): 40 of 40 byte-exact

All 40 rows grade RELOCATION_MATCH under RTM cl 13.10.3077 with no pragmas or flags. All 40 tests PASS under g++ (tall.sh) and under VC7.1/Wine (tall.py). Every source carries `// Leaf: ?Method@Class`, because the catalog leaf `vfunc_N` matches no symbol. The real method of each `vfunc_N` row comes from FableWin.pdb names plus the vtable slot, and is given on the source's `// Name:` line.

## What matched (40)

### NCreatureAction (6)
- **00695480 / 006954f0 / 00695560 / 00695710 `CActionEvent{CameraShake,Sound,Shot,RemoveObject}::Alloc`.** Each is `return new X(frame, thing, camera);`.
  - The CActionEventBase ctor is inline and `__declspec(novtable)`, on N_0099a2d0::CBase.
  - CameraShake sets `Global = false` in the ctor body.
  - RemoveObject builds its `Hand` CCharString through `fable_string.h`.
- **00870de0 `RemoveObject::SetDataFromString`.** `return GetDataFromEventString(...) ? true : false;` gives `test al,al ; setne al`. `!= false` and `!!` give neg/sbb instead.
- **00871a10 `RemoveObject::Dispatch`.** It looks up CTCCarrying (interface 70) with GetInterface and gets the hand slot from GetDefGlobalIndexFromName. Then it calls `RemoveThingInCarrySlot(slot, false)` and kills the thing if it is alive.

### Blocking, Briar Rose and carrying modes (9)
- **008d6d70 `CModeBlock::OnDeactivate`.** The inline `Active = false` comes first, then the resume for a live state-1 thing.
- **008d8ea0 `CModeBlock::FrameUpdate`.**
  - A named `combatManager` local makes GetWorld run before PCreature is pushed.
  - `info->AttackerInfo.Attacked` is re-read at each use; a named local moves it to eax.
- **008e8760 `CModeBriarRoseConcentrate::FrameUpdate`.** A `bool started = owner->SetCurrentAction(Temp(...))` action temporary (0x11c bytes). The fall-through path removes mode 0x3f through GetInterface 0x31.
- **008e8940 `CModeBriarRoseTeleportIntangible::FrameUpdate`.** An `&&` chain that contains the action temporary. `pack(1)` goes ONLY on the intelligent-pointer classes; packing the action class moves the frame (the action lands at +7).
- **008d6540 `CModeCarryingPlateStand::GetHoldingAnimIndex`.** Needs a named `creature` local.
- **008d8960 `CModeCarryingStandBase::OnDeactivate`.** The GetInterface 0x5a result is untested. A named `CAnimateThing&` reference is required, otherwise the push is scheduled before the getter.
- **008d87c0 `CModeCarryingWeaponStand::OnActivate`.** The first `SetGroup` is a `__forceinline` copy and the second is out of line. A same-TU `auto_inline(off)` const LowerBound stand-in lets both keys share one frame word.

### Conversation and crouch modes (4)
- **008d9350 / 008d93c0 `CModeConversation::FrameUpdate / OnDeactivate`.** Both use GetInterface 15 (CTCTalk), but they instantiate different LowerBounds: const in FrameUpdate, non-const in OnDeactivate.
- **008d89c0 / 008d8ab0 `CModeCrouch::OnActivate / OnDeactivate`.** OnActivate needs a named `weaponTC` for `SetGroups("CROUCH", ...)`. GetInterface 0x36 sets or clears the crouch bytes +0xf/+0x10.

### Dazed and dragon modes (11)
- **008de620 `CModeDazed::Recover`.** `creature->SetCurrentAction(CCombatAction_RecoverFromDazed(creature))`.
- **008e06e0 / 008e0730 / 008e0780 `DragonGetHit / DragonRearUp / DragonFireBeam::FrameUpdate`.** Each counts a timer down, then starts an "out of" action temporary. FireBeam also does:
  - the hover base update;
  - an inline `GetCurrentAction()` (+0x100) and HasPerformedAction;
  - StartBeamEffect and UpdateBeam.
- **008d6ff0 `CModeDragonHover::AnimUpdate`.** Picks a random "ST_IDLE_SUBTLE" idle, passing an inline `CAnimInit(6, 0, idx, 1, true, flags, 100)` argument.
- **008dc600 / 008dc660 / 008e07f0 `CModeDragonStrafe` OnActivate / OnDeactivate / FrameUpdate.**
  - OnActivate and OnDeactivate suspend or resume with `(long)this` and set a bitfield flag on 0x3a.
  - In FrameUpdate, `operator*` assigns its members in a body and `operator+` returns the 3-arg ctor. A named `position` local is required for frame 0x13c.
- **008dc6c0 / 008dcb00 `CModeDragonStrafeBurninate` OnActivate / OnDeactivate.** The GetInterface 9 result is untested. OnDeactivate calls EndBeamEffect when FlameActive (+0xbc) is set.

### Boss, troll, scorpion and screamer modes (7)
- **008dffd0 `CModeGuildmasterPowerUpHeal::FrameUpdate`.** `long remaining = --Timer;`. The GetInterface 0xad CTCBoss result is untested.
- **008e7600 `CModeIntangible::FrameUpdate`.** A `changed` flag, HasTC/GetTC to read the boss phase, then `(Timer > 0 && --Timer == 0) || changed`.
- **008d78e0 `CModeJackOfBladesScream::OnDeactivate`.** A block-scoped `thing` local, then IS_ALIVE emitter → Kill(true).
- **008d9480 `CModeKnockedDown::OnActivate`.** GetInterface 0x45 SetEmoteIcon, then HasTC/GetTC 0x30, then suspend, then the base activation.
- **008de6a0 / 008e0990 `CModeRockTrollVulnerable` OnActivate / FrameUpdate.**
  - OnActivate: `RecoveryCount = (long)(fps * 5.0f)`.
  - FrameUpdate: the GetTCIfPresent unset-local accessor for 0x5a.
- **008da3b0 / 008e10e0 `CModeScorpionKingExhausted` OnDeactivate / FrameUpdate.**
  - Both use the `T* p = 0; if (HasTC(k)) p = GetTC<T>(k);` preload, which gives `xor ecx,ecx` and then the store through p unconditionally.
  - OnDeactivate wakes the physics (slot 44) and sets creature flag 1.
  - FrameUpdate is `if (RecoveryCount) --RecoveryCount; else if (...) Recover();`, giving the tail-duplicated epilogue.
- **008da610 `CModeScreamerAggressive::FrameUpdate`.** `IS_ALIVE && GetInterface(&look, 0x43)` in one condition, then `LookAtPos(&target->GetPosition(), 0x67)`.

### Sit, strafe and stun modes (3)
- **008d6580 `CModeSitBase::OnActivate`** (the catalog's CModeSitChair::vfunc_5 is inherited; FableWin.pdb `?OnActivate@CModeSitBase`).
  - `IdleCounter = (range ? r % range : 0) + minFrames`, where `minFrames = (long)(f * 0.75f)` and `range = -minFrames - (long)(f * -1.5f)` (unsigned).
  - `r` comes from an inline `GFRandom(seed)` helper (two statements: `Seed = Seed*0x24a1+0x24df; Seed = GFROR13(Seed)`).
  - Open-coding the seed update in the body schedules it before the second `_ftol` and spills the seed. The helper fixes both.
- **008e9110 `CModeStrafe::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) const`.**
  - `GetStrafeAnimationName` 0x008E8E50 is a free `__fastcall` that returns CCharString by value: the hidden return slot is in ecx, the first vector in edx, and it ends with `ret 8`. The name temporary reuses the dead third-parameter slot.
  - PeekWorld is inlined (`[g_pMainGameComponent]+0x24`, mesh bank +0x3c).
  - The result is `speed / (float)g_FramesPerSecond`, with `speed` a named float.
- **008d8fe0 `CModeStunned::LeafInit`** (slot 12). An out-parameter GetInterface for type 9 (`mov eax,[eax+4] ; mov ecx,eax`) calls SetFlags(4). Then HasTC/GetTC for 0x30 calls CTCCombat::RemoveFromCombat. Both keys share one frame word.

## Parked for Fable 5.1 (0)

None.

## Skipped

None.

## Wrong bounds

None. 00871a10, 008e0780, 008d6ff0, 008e7600 (177 bytes, an odd length) and 008d6580 fill their whole catalog length and end at `ret`. Each graded byte-exact at that length.

## Label corrections

Propagated manifest labels found wrong (real identity and evidence):

| Address | Propagated label | Real identity | Evidence |
|---|---|---|---|
| 0x008E43A0 | BattleChargeTurnLeft | CCombatAction_RecoverFromDazed ctor | Called by CModeDazed::Recover. |
| 0x008E4A60 | Guildmaster_HealFollowersInto | CCreatureAction_DragonFireBeamOutOf ctor | Called by DragonFireBeam::FrameUpdate. |
| 0x008E49C0 | CCreatureAction_DragonRoar | CCreatureAction_SnapDragonOutOf ctor | Stores vtable 0x01286D94. |
| 0x008F5020 | GenericResponseToDecapitation | CCreatureAction_DragonRearUpOutOf ctor | Called by DragonRearUp::FrameUpdate. |
| 0x008E4860 | HealFollowersInto | CCombatAction_Guildmaster_HealFollowersRelease ctor | vtable 0x012869BC. |
| 0x008E4420 | BattleChargeTurnLeft | CCreatureAction_TrollVulnerableInto ctor | Called by RockTrollVulnerable::OnActivate. |
| 0x006925A0 | GetCanItemBeQuickAssigned | CCombatActionBase::HasPerformedAction | Called on the creature's current action (+0x100). |
| 0x007AEEC0 | CountEffects | CTCBoss::CountLivingHelperCreatures | — |
| 0x007AED10 | DrawGetEnvironment | CTCBoss::GetBossPhase | — |
| 0x0070B460 | DrawGetEnvironment | CTCAnimationComplex::GetAnimation | — |
| 0x0070D8C0 | AddChild | CAnimateThing::RemoveCombinationAnimsOfType | — |
| 0x008D7BB0 | CTCKraken::EndHeadEffects | CModeDragonStrafeBurninate::EndBeamEffect | — |
| 0x0062E900 | ~CTCLightningOrb | ~CCombatActionBase | — |
| 0x004C73D0 | DoSizeof | CThing::GetPosition | — |
| 0x00662410 | AddCombinationBlend | CThingCreatureBase::IsRegisteredCombatant | — |
| 0x0073A680 | GetDisplayEngine | CModeBase::GetWorld | Landed sources disagree on this name. |
| 0x006E9360 | CSurface::GetHeight | CCombatManager::GetCombatantInfo | — |
| 0x00A23CE0 | DrawGetSeaLevel | C3DAnimationInfo::GetDuration | Reads +0x14. |
| 0x007EA270 | CObservable::AddExclusiveObserver | the type-9 targeting component's SetFlags | As landed 0x008D8F30. |

Catalog names: every `<Class>::vfunc_N` row is the method named on that source's `// Name:` line. Two of them resolve to a different class or method than the catalog suggests:
- 008d6580 is CModeSitBase::OnActivate (inherited by CModeSitChair).
- 008d8fe0 is CModeStunned::LeafInit.
