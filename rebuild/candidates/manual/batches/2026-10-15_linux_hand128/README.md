# Hand chunk 128 (CCreatureAction_CastStrengthMultiStrike .. CCreatureAction_GoUnderground): 37 of 40 byte-exact

All 37 matched rows grade RELOCATION_MATCH with default flags and no pragmas; every
matched row has a behaviour test that prints PASS under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py).
Retail base sizes used throughout: CCreatureActionBase 0xa8 (PDB 0xb4), CCombatActionBase 0x114 (PDB 0x12c); the
generated engine headers for these classes are the PDB layouts, so every source models the class locally and says so.

## What matched (37)

- **Combat-action constructors with a GetSoundPair sound** (`CCombatActionBase(owner[, target], code, GetSoundPair(owner, "LIT"))`,
  the landed SnapDragon family: implicit literal temporary, hidden-pointer CSoundPair):
  - 009535b0 CCreatureAction_DragonFireBeamInto(owner, target, float timer) -- code 250; Timer at +0x114 is an
    initialiser after the vtable store (catalog: CastStrengthMultiStrike).
  - 00953120 DragonRearUpFull / 00953630 DragonFireBlastBase -- default CIntelligentPointer member at +0x114
    (out-of-line base ctor, inline vtable store); FireBlastBase adds a CEnginePrimitiveHandle (CBase ctor, vtable,
    zeroed id) at +0x11c.
  - 009533e0 DragonTakeOffIntoFlying(owner, const vector<C3DVector>&) -- out-of-line 12-byte vector copy ctor
    (0x006F0810) at +0x138, UsePreviousPoint byte, CCountedPointer zeroed from one eax.
  - 009717a0 DuckAndCover(owner, const C3DVector&) -- the three dwords copied through ecx/edx/eax need THREE SCALAR
    members (a C3DVector member copies through `lea ecx,[esi+114h]`).
  - 008b8110 GoUnderground / 008ca600 NymphSummonOutOf / 008e4770 ScorpionRecover -- base call then a block-scoped named
    animation string and SetAnimationName (0x008574D0); a NULL sound name is `GetSoundPair(owner, (const char*)0)`.
- **Combat-action constructors with a silent sound pair**: 008bb0b0 CastWillForceSpell (ebx zero shared by the
  CSoundPair and the two bool initialisers, LevelToCast -1), 008bbf20 CastWillGhostSwordSpell (immediate zeros),
  008bc5e0 CastWillReclaimUndeadFollower (inline converting `CIntelligentPointer(T*)` = base ctor, vtable,
  SetPItem), 008f4f40 DragonSwoop (animation reference forwarded, three POD C3DVector element assignments).
- **CCreatureActionBase constructors** (`CCreatureActionBase(owner, -1.0f, speed, GetRandomLoadedAnim(owner, name),
  CSoundPair(), c, d)`, 0x00693B30 + the `__fastcall` 0x006924B0):
  - 007ee7e0 OpenChest / 00860060 GetOutOfBed / 00916590 PickUpGoods (name by reference, thing pointer member);
    a `0.0f` speed keeps every zero an immediate (`push 0` x3 + immediate CSoundPair stores), a non-zero speed lets one
    eax zero serve them.
  - 00843510 Drink, 00843470 LearnExpression, 007ed450 GiveMoneyToThing, 00945410 GetItemOutOfPocket -- implicit
    literal temporary built first; LearnExpression keeps the player in ebx so edi carries the zero (`push edi` for
    0.0f too).
  - 008d5200 / 008d5280 FireMissileTest -- every base argument straight from the argument slots; PTarget(target) /
    PTarget(0).
- **Copy constructors** (base copy 0x0062EBC0, `CIntelligentPointer(const&)` = base ctor + vtable + SetPItem(GetPItem())):
  0084c540 DowseFire, 008d6320 FireMissileWeapon (CDefPointer copy = null-tested `inc [p+4]`; `mutable` count),
  0092e810 GetInBed (out-of-line CCharString copy ctor 0x0099EC30, modelled locally).
- **Component lookups (vector map at +0x44, LowerBound 0x0040F020)**:
  - 008ba970 CastWillHarvestSoulSpell::PerformAction, 008baa00 CastWillReclaimUndeadFollower::PerformAction --
    IS_THING_ALIVE inline; the void out-parameter GetInterface with the out pointer left uninitialised gives the
    tail-duplicated component call and the dead-frame-word read on the absent path.
  - 008dba90 NCreatureMode::CModeDragonFireBeam::KillBeamEffect -- `BeamActive = false` then the
    CTCSpecialAbilities::FadeOrKillEffect body on PBeamWeaponEffect through a REFERENCE local, emitter zeroed before
    the lookup (edi).
  - 008bab70 FinishFireballSpell::SetAnims (bool GetInterface in the `if`; the returned CCharString temporary reuses the
    key's frame word), 00848d10 DropGenericBox::PerformAction (two void lookups on one cached PCreature address),
    008b7920 / 008b79a0 GetUpFromSitting::Initialise / PerformAction (SittingMode + RemoveOnPerformAction gate),
    008d58d0 FireMissileWeapon::ResetCreatureUseProjectileWeaponState (block-scoped ReleaseMissile locals whose dead
    slot is reused by the key), 008ba0b0 ForcePushOutOf::PerformAction (block-scoped AddTC name string, component in
    ebx, IgnoreFriendlies at +0x24), 008f3c00 DragonLand::FinishAction (`or byte ptr [tc+50h],1`, creature local
    fetched before the SetSuspendAnimation pushes).
  - 008bf6a0 GenericLoadProjectileWeapon::FinishAction -- second-level lookup on the carried weapon = `unsigned long
    HasTC()` (flags dword loaded, `test ch,1`) plus a pointer-returning lookup whose result goes straight into ecx.
- **Physics TC** (CThing +0x60): 008f2fb0 DragonHoverOutOf::FinishAction, 008f2f30 DragonTakeOff::FinishAction --
  the raised position is built FIELD BY FIELD from the GetPosition reference (copy-then-`+=` goes through the frame),
  the creature for `PhysicsTC->ResetPosition` (slot 31) is a named local; TakeOff first calls slot 36 (true).

## Parked for Fable 5.1 (3)

- 0072abd0 CTCSpecialAbilities::FadeOrKillEffect DIFFER(98v98): esi/edi swapped (retail effect pointer in esi, emitter in edi; lane the reverse), every instruction otherwise identical | tried: null-default GetTC returning a pointer (94v98), out-parameter lookup into a local zeroed before it (98v98, best), pointer-returning wrapper (94v98) | next: bind the parameter to a reference local `CIntelligentPointer<CThing>& effect = ...` -- that exact form matched the inlined copy in 0x008DBA90 KillBeamEffect
- 008b6e60 CCreatureAction_CombatBoast::Initialise DIFFER(117v114): one dead `add eax,44h` after `mov ecx,[eax+44h]` (the inlined CDefPointer copy constructor's reference parameter) | tried: reference copy (117v114, best), raw-pointer constructor (116v114, def moves to eax), body-assigned copy (117v114), by-value accessor temporary (117v114, `add eax,38h`) | next: lane gap: dead address of an inlined copy-ctor reference argument
- 005ee3b3 CTCInventoryAbilities::GetMiscThingsDef DIFFER(111v101): `lea eax,[ebp-4]` before `push 1`, and after the add-ref the def local is re-read from [ebp-4] for the destructor (`dec [eax+4] ; cmp [eax+4],0`) where retail keeps ecx with one shared null test (`inc [ecx+4] ; dec [ecx+4] ; jne`) | tried: one outer def local (112v101), sibling-block locals (111v101, best), AddRef/Release on the def object (111v101) | next: lane gap: aliasing of an address-escaped counted local across the add-ref store (`optimize("y",off)` + `("s",on)` frame TU)

## Skipped

- none (no row spans two functions).

## Label corrections

- 009535b0 "CCreatureAction_CastStrengthMultiStrike" ctor -> CCreatureAction_DragonFireBeamInto ctor (vtable 0x01293E54; float Timer).
- 005ee3b3 "CCreatureAction_CastStrengthMultiStrikeProper" ctor -> CTCInventoryAbilities::GetMiscThingsDef() const (Ego_r
  `?GetMiscThingsDef@CTCInventoryAbilities@@ABE?BV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@XZ`; callers 0x005EEEB5..).
- 0072abd0 "CCreatureAction_CastWillReclaimUndeadFollower::PerformAction" -> CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&)
  (static __fastcall; called by CTCDivineWrath::FinishMove and other spell TCs).
- 008dba90 "CCreatureAction_CastWillReclaimUndeadFollower::PerformAction" -> NCreatureMode::CModeDragonFireBeam::KillBeamEffect
  (PDB offsets PBeamWeaponEffect +0x84, BeamActive +0x90; only caller OnDeactivate 0x008DBB00).
- 007ee7e0 "CCreatureAction_GetOutOfBed" -> CCreatureAction_OpenChest ctor (vtable 0x0127173C); 00916590 "CCreatureAction_GetOutOfBed"
  -> CCreatureAction_PickUpGoods ctor (vtable 0x0128FBF4); 00843470 "CCreatureAction_GiveItemToThing" -> CCreatureAction_LearnExpression
  ctor (vtable 0x0127461C); 008ca600 "CCreatureAction_GoUnderground" -> CCreatureAction_NymphSummonOutOf ctor (vtable 0x01282354);
  008e4770 "CCreatureAction_GoUnderground" -> CCreatureAction_ScorpionRecover ctor (vtable 0x01286724).
- Callee labels: 0x006924B0 "CMainGameComponent::BeginInputLoading" = `__fastcall` GetRandomLoadedAnim(owner, const CCharString&)
  (`push 1 ; push edx ; call GetRandomLoadedAnimFromName`); 0x008574D0 "CTCInGameMenu::SetPDef" = CCombatActionBase::SetAnimationName;
  0x00856B10 "_Dest_val<...>" = CCombatActionBase::GetCombatManager; 0x0072A840 "$E2" = an empty CTCSpecialAbilities method (soul
  harvested hook); 0x0072B3B0 "CCreatureAction_KillReclaimedUndead" = CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject&);
  0x006E05F0 "CTCDParticleEmitter::EnterTimelineEnd" = the emitter TC's fade-or-kill (fade emitter +0x14, else Kill(true) on the
  parent); 0x009AD390 "COptimisedPrimitive::ActualToUniqueIndex" = definition manager name -> global index (const CCharString&,
  bool); 0x0044C6B0 "CGameDefinitionManager::Get" is a plain static getter (`mov eax,[g] ; ret`), not __fastcall;
  0x006F0810 "Vector_CopyFrom" = the 12-byte vector copy constructor; 0x0099EAE0 "_Cons_val<...>" = ~CCharString.
