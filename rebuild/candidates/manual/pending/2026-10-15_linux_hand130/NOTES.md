# Hand chunk 130 (CCreatureAction_ScorpionTailWithdraw .. CCreatureAction_TrollTunnel): 36 of 40 byte-exact

All 36 sources grade RELOCATION_MATCH under the Wine lane with no flags or pragmas (`grades.txt`); every test prints PASS under g++
(`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). The PDB engine headers for these classes flatten the base and sit 0x0c (CCreatureActionBase
derived) or 0x24 (CCombatActionBase derived) bytes high against retail (retail CCreatureActionBase = 0xa8, CCombatActionBase = 0x114), so `this` is
modelled locally with the landed relatives' shapes, keeping PDB member names where the layout could be matched.

## What matched (36)
- **Inlined GetInterface (out-parameter template of 0x008CB4A0) family** -- CCreatureAction_ScorpionTailWithdraw::FinishAction 008c4780,
  ScreamerAdvance::FrameUpdate 008b8cb0, ScreamerDie::FinishAction 008b8f30, SheatheItemToInventory::Initialise 0069f480 / FinishAction 0069f670,
  SheatheWeapons::PerformAction 008b7b10, SpiritHitResponse::Initialise 00854810, StartSneaking::FinishAction 00845c70, SummonerUnsheatheStrike::PerformAction
  008cb6e0, TentacleStrikeInto::Initialise 008cf3d0, ThrowGameObject::PerformAction 00846090, ThunderLightningStorm::Initialise 008c0b70,
  TrollGroundPound::FinishAction 008c7480, TrollTunnel::PerformAction 008c63c0. Shape lessons: an UNINITIALISED out pointer used unconditionally gives the
  miss-path garbage read; the tail is DUPLICATED when only the call (+ base call) follows, and the paths JOIN when more code follows (008c63c0, 00846090);
  a NULL-initialised out pointer (`T* p = 0;`) lives in a callee-saved register (`xor esi,esi` / `xor edi,edi`) instead of the frame word (008b8cb0, 008b8f30);
  the out pointer may be a MEMBER (`GetInterface(&PTentacle, 0xe3)`, 008cf3d0).
- **IS_THING_ALIVE on PTarget + shield notification** -- ScorpionUnblockableLeft/RightTurnAttack Initialise 008c42b0/008c4330 and FinishAction
  008c43b0/008c4430: alive idiom then `if (GetInterface(&shield, 0x93))` with the creature argument fetched after the component.
- **Call-before-push** -- a creature fetched BEFORE constant argument pushes is a named local (008c4780 SetSuspendAnimation, 00854810 ApplyAcceleration).
- **Inline forwarder with by-value arguments** -- SummonerFlameSlice::FrameUpdate 008caf10: three GetPItem calls up front, then the float/position pushes =
  an inline `SetBetween(CThing*, CThing*, CTCPlacement*)` whose by-value arguments are evaluated first; the obstruction pointer read before them is a local.
- **Copy constructors** -- SendMessage 00982750, ShopkeeperGetOut 0084c170, ShowTrophy 006dfb90, TrollThrowProjectile 008c7d80: memberwise init lists;
  fixed-offset word copies need scalar members or a C3DVector with a MEMBERWISE USER COPY CONSTRUCTOR (an implicit/POD vector copy re-bases both sides with `lea`);
  CDefPointer copy = null test around the store + `inc [eax+4]`; CIntelligentPointer copy = base ctor + vtable + SetPItem(other.GetPItem()).
- **Constructors** -- ShowTrophy 006df730 (`#pragma pack(1)` class: second intelligent pointer at +0xb5), SitDown 008ab9d0 (by-value counted pointer:
  member add-ref and parameter release fold into `inc ecx ; mov eax,ecx ; dec eax` with both stores), StabbedToDeath 0087e7a0 and Die 008ed650 (two
  helper-returned CCharStrings built before any push; the creature's vtable slot 89 returns the CSoundPair through a hidden pointer), SummonerLightningOrbInto
  008cbd90 (0x008CBCF0 shape + IP member), SummonerLightningOrbOutOf 008cbe10 (raw-pointer alive test, stack IP temp assigned to PTarget, implicit dtor),
  TeleportOut x3 008bcea0/008bcf20/008bcfb0 (body struct assignment = `lea`, init-list copy through C3DVector user copy ctor = fixed offsets),
  TrollThrowProjectile 008c7390/008c7400 (TargetPos and flag are BODY assignments).
- **Locals** -- ShopkeeperGetOut::PerformAction 00842d40 (GoodsPlace local filled by two struct assignments), ShowToSheatheTrophy::FinishAction 006dd980
  (0xb0-byte stack CCreatureAction_SheatheTrophy; implicit destructors emit no vtable stores; the counted follow-up pointer released inline, object then block zeroed).

## Parked for Fable 5.1 (4)
- 00848fd0 CCreatureAction_SitDown::PerformAction DIFFER(107v105): retail joins the mode manager straight into ecx (`mov ecx,[eax+4]` / `mov ecx,[esp+8]`) and builds the by-value counted pointer through edx/eax; the lane joins into eax, copies through esi/ecx/edx and adds `mov ecx,eax` | tried: init-list copy ctor, converting template copy ctor, GetTC<T>() accessor (moves the lookup after the copy), body-assignment copy with a block local | next: lane gap: register allocation of a joined `this` around an inlined by-value copy (try the brief's auto_inline(off) callee-above trick only if a callee body is in the TU)
- 00854720 CCreatureAction_SpiritReturnToHero::PerformAction DIFFER(104v104): register swap only -- retail `push esi ; push edi ; mov edi,ecx` (this in edi, player in esi), lane this in esi, player in edi (push edi after the global load) | tried: ternary with player local, inline GetPhysics() accessor (fixed the vcall shape), inline GetHeroOrSelf helper, `self` local after the player | next: lane gap: callee-saved register colouring
- 0062d890 CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject DIFFER(129v129): scheduling only -- retail builds the "STANDARD_CAST" string, loads the owner into edi, then zeroes the sound pair and the two trailing zero arguments from eax; the lane hoists the zero (edi) above the string constructor call and loads the owner after the pushes | tried: CSoundPair() temporary, inline NoSound() helper | next: lane gap: zero-register hoisting across a call

## Skipped
- none (no row spans two functions).

## Label corrections
- 008ed650: chunk/manifest "CCreatureAction_StabbedToDeath::CCreatureAction_StabbedToDeath" (propagated from the byte-similar 0x0087E7A0) is
  **CCreatureAction_Die::CCreatureAction_Die(CThingCreatureBase*, bool)** -- the vtable stored is 0x01287FC4 = CCreatureAction_Die (rtti-vtables.tsv).
  The source carries `// Leaf: ??0CCreatureAction_Die`.
- Callee labels found propagated/wrong (named descriptively in the sources): 0x006925A0 = CCreatureActionBase::GetPerformedAction (`mov al,[ecx+62h]`, not
  CTCInventoryBase::GetCanItemBeQuickAssigned); 0x006925E0 = a CCreatureActionBase float getter (animation fraction), not CInputTypeMouseMovementEvent::operator==;
  0x00692940 = CCreatureActionBase::GetWorld-style getter (not _Dest_val<...>); 0x00693EF0 = ~CCreatureActionBase (not ~CPhysicsMeshInfo); 0x006924B0 =
  the __fastcall owner-animation lookup taking `const CCharString&` (not CMainGameComponent::BeginInputLoading); 0x0079E580 = a CTCPhysicalShieldSpell method
  (not "CCreatureAction_SetDirection"); 0x007FD3F0 = an obstruction-shape setter (start +0x0c, end +0x18, width +0x28; not CInputManager::UpdateChecksum);
  0x0058119B = std::set<CCreatureActionBase*>::erase(const key&) (not CBankFile::CompareDataOffset::operator()); 0x00800020 = a CTCHaste speed recompute
  (not CAnimation_RecalculateScale); 0x008574D0 = CCombatActionBase::SetName (not CTCInGameMenu::SetPDef).
