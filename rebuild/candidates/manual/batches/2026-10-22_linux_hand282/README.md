# Hand chunk 282 (CCombatActionBase .. CCreatureAction_DragonRearUpFull): 37 of 40 byte-exact

All 37 src/ files were re-graded by the Finish agent with gall.sh: every one is RELOCATION_MATCH. Every test prints
`<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). The purity grep (__asm / naked / _emit / volatile) over
src/, test/ and differ/ is clean.

## What matched (37)
- **CCombatActionBase**
  - 00858a20 SetupCombatInterrupts: reads the strike-interrupt, reset-combo, movement-interrupt and related frames from the action def. Carried over from the earlier run.
  - 008597a0 ProcessStrikeEvents: runs the active strike event (damage, weapon aug effect). Carried over from the earlier run.
  - 00859a70 SetupCombatStrikeEvents: builds one CStrikeEvent per strike-start event. Levers: a same-TU noinline parser stand-in, a named GetPItem() local, inline begin()/end(), the literal 10000000000 rather than a cast, and the functor passed by value through a for_each-shaped template.
- **CCombatAction_ControlledStrafeJump**
  - 0085bd70 Initialise: ResetRHSet (slot 32) takes a TEMPORARY CRightHandedSet, which gives retail's early vptr push. The dot product is written as sequenced statements (Y, then +=Z, then +=X). The animation name is a MOVEMENT_STRAFE_JUMP_* CCharString.
- **CCombatAction_KickThingOnGround**
  - 0087e860 Initialise: block-scoped CCharString animation names, a user C3DVector copy ctor (drops the dead Z word), a named suspend-target local, and the exact MoveToPos operand order. The epsilon is global 0x0129BA3C, squared at run time.
- **CCombatAction_StrikeResponseJackOfBladesIntoRush**
  - 008c1600 GetValidRushPosition(C3DVector*), ret 4.
  - 008c2550 FinishAction: same-TU stand-ins for GetValidRushPosition and the CCountedPointer ctor 0x00695880. The def is a CDefPointer fetched through an inline GetSubDef. The counter bump is written as a named local. Uses `delta = a - b; direction = Normalise(delta); distance = delta.Get2DLength();`.
- **CCombatManager**
  - 006ea770 GetPreferredPos: a same-TU `__declspec(noinline)` body for the combatant-map find 0x006F0250 frees the first UID key slot, giving retail's `sub esp,5Ch`.
  - 006eb390 SetupNextSequenceForAttacker: the out-pointer `GetInterface(&combat, 0x30)` replaces HasTC + GetTC. The longer live range flips `this` into ebx and the sequence into ebp. The def AddRef/Release are inline CThingDef members.
  - 006ec4f0 FillCombatSequenceList. Carried over from the earlier run; operator delete goes to 0xBFE9BC.
  - 006ed510 DeregisterAttackingCombatant: two nested `__forceinline` by-value trace helpers, vector clear() written as erase(begin(), end()), an inline def Release(), the stealth lookup as HasTC plus GetTC, and the 0x46 lookup through the const LowerBound 0x004365B0.
- **NUISystem::CComponent**
  - 0052f5c0 UpdateZoom: C2DVector locals declared first and assigned later, operator/ written as `*this * (1.0f/f)`, componentwise RenderZoom stores, and an inline pow(float,int) loop.
  - 0052ffd0 UpdatePosition: the += goes through a free inline helper `Add(C2DVector&, const C2DVector&)`, which loads the added operand first as retail does.
  - 00531ba0 AddPositionChild: copy-then-`*=` instead of `a * s`, named screen-space locals, and an explicit C2DVector(x,y).
  - 005331a0 GetDataFromDef: a same-TU, non-template explicit specialisation of `_Rb_tree<...CUIState>::insert_unique(iterator, const value_type&)` (0x0042D2C6) under auto_inline(off). Its body only reads the value, which makes VC7.1 store the pair key last.
  - 005334a0 CComponent(long) and 00533720 CComponent(const CComponent&).
- **CConsole**
  - 009ead60 DrawHistoryScrollBar: implicit double->float initialisations (`float f = g * -2.0;`). An explicit (float) cast reschedules the prologue.
- **CCrateDeathMarker / CCrateTeamMember**
  - 00ef12f0 Main: the villager is scoped in a nested block inside `while(!IsDone())`. In-body `#pragma inline_depth(0)` / `#pragma inline_depth()` surrounds the four out-of-line ~CScriptThing (0x004AA840) sites.
  - 00dcdf60 Init: a CCharString == literal comparison gives the null-pointer arm. The map insert goes through a same-TU insert_unique stand-in.
- **CCreatureActionBase**
  - 00694430 FinishAction. free() goes to 0xBFEA14.
- **CCreatureAction_* actions**
  - 008beee0 Ambush::Initialise: `C3DVector dir(C2DVector(a - b)); dir.Normalise();` with a Z=0 converting ctor and a FastInvSqrt Normalise.
  - 008ccd70 BalverineCorneredJumpIntoSky::FinishAction: a same-TU GetSubDef stand-in keeps the def in ebp.
  - 008c8c30 BanditKingLunge::FinishAction: the bytes were already matched. The test's pointer-destroy count was corrected to 3.
  - 008c8f70 BanditKingStuck::FinishAction: the && condition is written in inverted order.
  - 008c89d0 BanditKingThrowWeapons::PerformAction: the release is `if (--count == 0) Destroy()`, and the carry slots are a two-element array.
  - 008c8550 BanditKingUnstuck::Initialise.
  - 008cda30 BattleChargeMiss::FinishAction and 008cd820 BattleChargeImpact::FinishAction share three levers: an inline `bool IsDead() const`, CIntelligentPointer operator-> / operator* (evaluated before the pushes, owner first), and a ternary GetInterface out-store.
  - 00855500 BlockRespond::Initialise: `v *= FastInvSqrt(v.GetLengthSquared())` with a plain-inline length (the reciprocal spill pattern), the dot written out explicitly with the normalised vector first, and a noinline LowerBound in the same TU.
  - 008c1f70 BriarRoseInvulnerableTeleport::Initialise: the CSoundInit ctor assigns PlaybackType in its body.
  - 008c0900 BriarRoseShotFarOutof::PerformAction and 008f3100 DragonLandFireball::PerformAction (see the label corrections).
  - 00849380 BuyThingFromShop::PerformAction: the const-CThing GetInterface (the const LowerBound 0x004365B0), an inline CreateThingAt helper, a named CMainGameComponent local, and an inline CTCShop::GetShopKeeper.
  - 008ce6f0 CastSkillMultiArrow ctor and 008ba3c0 CastWillEnflameSpell::FinishAction.
  - 008f3630 DragonRearUpFull::PerformAction: inline CThing::GetDef() and GetPhysics() accessors, plus a `__forceinline` GetDef wrapper on the manager. The Newton step is written `0.5f * (r * (3.0f - lengthSquared * (r * r)))`; it emits the same bytes and carries 2 more hidden IR ops, which reaches retail's x87/push interleave.

## Parked for Fable 5.1 (3)
- 008c12d0 CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise DIFFER(809v808): retail keeps &PCreature in ebp, edi = map/boss/def/&PThingThatHit, ebx = 0/hitter, and reloads the def only for the final release (frame 0x3c). Ours puts &PCreature in edi and boss/def/hitter in ebp, and reuses the def slot for the angle (frame 0x38) | tried: extern GetSubDef (850; retail's ebp, but the def becomes address-exposed), escaping stand-ins (850/856), Release()/local-copy dtors, def declared first, PCreature reference/accessor, PThingThatHit reference, battle-def local, stand-in defined after the caller, the real GetSubDef template body inline and noinline | next: lane gap: register allocation (the in-TU non-escaping def takes ebp ahead of &PCreature)
- 006e99b0 CCombatManager::GetDefenderZone DIFFER(852v852): `mov dword ptr [esp+2Ch],0` (toPredicted.Z = 0) lands between `fsub [eax]` and `fstp [esp+24h]`; retail puts it after `fstp [esp+24h]; fld [esp+64h]` | tried (~200 grades): member stores in every order with and without casts, ctor C3DVector(x,y,0), Set/ClearZ/Flatten/operator[]/pointer writes, -= on a copy and in place, free/NRVO/__forceinline operator- and ctors, a trivial copy ctor (866), /Oa, QFE 4035 | next: lane gap: immediate-store scheduling (the slot follows block-level context such as the defenderPos copy shape)
- 008f3310 CCreatureAction_DragonFireBlastBase::PerformAction DIFFER(796v796): at 0x8f3535 retail has `fld [eax+8]; fsub [esp+2Ch]; mov ecx,edi`, ours has `fld [eax+8]; mov ecx,edi; fsub [esp+2Ch]`; every other byte is identical | tried: about 120 forms, including __fastcall/__stdcall/inline FastInvSqrt (?GFFastOneOverSqrt@@YIMM@Z), union/pointer puns, accessors, named result locals, operand swaps, stand-in body variants, unrelated TU functions, and the flags /Oa /Ow /GS /Ob1 /Ox /Gy- /G5-/G7 plus QFE 4035. Byte-changing edits in the block flip the slot but break other bytes | next: lane gap: list-scheduler state carried across the call-spanning if-body block (possible lead: the real creature-actions TU's header-defined callees)

## Skipped
None. No row holds two functions.

## Label corrections
- 008f3100: CCreatureAction_DragonLandFireball::PerformAction. The catalog repeats 008c0900's name, but the body fires from DRAGON_MOUTH_DUMMY, Ego_r.pdb has ?PerformAction@CCreatureAction_DragonLandFireball@@UAEXXZ, and it grades under that leaf.
- 00856b10 CCombatActionBase::GetCombatManager; 00692940 CCreatureActionBase::GetPWorld; 00692910 CCreatureActionBase::GetPDefinitionManager; 0078b230 CDefinitionManager::GetDef<CThingShotDef>.
- 005d9030 CTCInventoryItem::GetInventoryType (def +0x50); 005d9050 CTCInventoryItem::GetMaxNumberItems (def +0x40). Both were labelled CCategory::GetCategoryIdentifier through byte-identical propagation.
- 006e9700 CCombatManager::IsCreatureOnRotatingCombatRing (not CCombatWheel_ValidateSequencePosition); 006e99b0 CCombatManager::GetDefenderZone.
- 00662410 CThingCreatureBase::IsInMeleeAsAttacker; 00662430 CThingCreatureBase::IsInMeleeAsAttacked (the earlier "IsCombatant" guess is superseded); 006e92c0 CCombatManager::IsAttacker; 006e9270 CCombatManager::IsAttacked. The catalog labels are propagated.
- 005da4e0 CTCWeapon::ResetAiming (FableWin export; not CGhostGrannySon::Init, not ResetProjectileState); 006ed030 CCombatManager::UpdateAttackingCombatantsList(CCombatantInfo&, bool).
- 006f3200 / 006f50b0: std::map<unsigned __int64,...>::erase(const key&) for the Attackers and Combatants maps. 0099f690 / 0099f600 / 0099f570: the fastcall operator+ overloads of CCharString.
- 00814bf0 CTCHitLocations::GetHitLocation; 00815370 CHitLocation::GetClosestPointToPos2D; 00a14440 C3DVector::Normalise (not GetScaled).
- 005cc091 CCreatureAction_UnsheatheItemFromInventory::~CCreatureAction_UnsheatheItemFromInventory (the catalog's _Dest_val<...> is propagated).
- 0052e580 ConvertToScreen (__fastcall C2DVector by value); 009de3f0 CRenderManager2D::Draw2DBox; 009a4ec0 GFGetSystemManager.
- 008ccc10 / 008ccbd0: the CCreatureAction_BalverineLungeAttack and BalverineDropInFromSky ctors; 007f8860: attached-particles Detach; 008cd0d0 GetSubDef<CBalverineBattleDef>; 0042b1a6 GetSubDef<CWeaponDef>.
- 0066ace0 CTCSoundPlayer::PlaySound(long, const CSoundInit&, long, long); 0065fba4 CSoundMap::GetPSound; 00857dc0 CCombatActionBase::SetCombatAnimations.
