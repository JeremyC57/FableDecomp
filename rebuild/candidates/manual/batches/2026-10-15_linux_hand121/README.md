# Hand chunk 121 (CAIStateGroup_MeleeCombatNearbyBase .. CAIStateGroup_SeekScriptTarget): 26 of 40 byte-exact

All 26 graded RELOCATION_MATCH (grades.txt); all 26 behaviour tests PASS under g++ and the lane's VC7.1 cl.exe.
Every source models `this` locally with the PDB member names: the engine headers of these classes flatten the base
and carry no vtable (the landed siblings do the same). The shared model block (probe/common.txt) is the landed
0x00930DC0 one: presence bitset at +0x20, CVectorMap::LowerBound @ 0x0040F020 / @ 0x004365B0 inlined through
`GetInterface(T**, type)`, `IS_THING_ALIVE` as two GetPItem calls, CAIStateGroupBase with PBrain at +4.

## What matched (26)
- CAIStateGroup_MinionGossip: 0090c800 InitStateGroup -- random 0..GetTimeToInitiate()*fps frames ago; the fps global read
  BEFORE GetPWorldSeed, the virtual call after it, the landed seed step (`seed*0x24A1+0x24DF; GFROR13; range ? % : 0`).
- CAIStateGroup_MinionGuardBribed: 00929aa0 IsRunnable -- a bool `&&` chain (`mov eax,1`/`xor eax,eax`): interface
  present, village exists, `GFGetCurrentFrame() < village->+0xA8`; the dropped `GetPWorld()` call is a bare statement
  inside an inline helper.
- CAIStateGroup_MinionGuardReactToCriminal: 009331e0 OnReplace -- nested ifs; the creature in a named local for the
  compare with the village's intelligent pointer at +0xDC; SetCriminalHandler(0) @ 0x00933280.
- CAIStateGroup_MinionGuardReactToPlayerWithWeapon: 00902233 Initialise -- `optimize("y",off)` + `("s",on)` (the landed
  0x00902045 shape); three plain zero stores then `PPlayer = 0` through the converting CIntelligentPointer temporary;
  00939d00 IsAnotherGuardAdmonishingPlayer -- `IsOfType(0x1b) && HasExtraData()` Bind2 by const reference, the
  originator test is a 64-bit UID compare (`event->OriginatorUID != creature->UID`).
- CAIStateGroup_MinionHasInformation: 0090f290 CreatureWantsPlayersAttention -- nested ifs with ONE `return false`
  (per-test early returns duplicate the `xor al,al` exit); the hero via PlayerManager->GetCurrentPlayer()->GetPHero();
  the perceive component through HasTC + the pointer-returning FindTC (the component lands straight in ecx).
- CAIStateGroup_MinionKidGroupTag: 00934120 OnActivate -- HasMode(1)/AddMode(1) with StandModeAddedToCreature, the
  group tag's slot-31 call, `if (InitiatedTag || GetTagger() == 0) SetTagger(creature)`.
- CAIStateGroup_MinionReactToApology: 0091d120 IsRunnable -- `const CEventHitBy* ev = 0; GetEvent(&ev, CEventFilter_None())`
  with the out pointer homed in the dead key slot and the empty filter in the high byte of the first slot.
- CAIStateGroup_MinionReactToCorpse: 00936ba0 OnActivate -- `if (GetLifeState() != 3) { GetInterface(&modes,0x31); AddMode(15); }`.
- CAIStateGroup_MinionReactToCrime: 009587d0 OnActivate -- the CCharString("OPINION_DEED_MASK_REPORTING_CRIME_TO_GUARD",-1)
  must be a NAMED local in an inner scope (`lea edx,[esp+8]`; a temporary reuses the constructor's eax), destroyed
  before `DeedMaskApplied = true`.
- CAIStateGroup_MinionReactToNoise: 007338a0 "Cleanup" (label propagated, see below) -- RemoveMode(12) + two bools cleared
  from one `xor al,al`; 0091cfa0 Cleanup -- RemoveMode(0x11), InvestigatingNoise = false.
- CAIStateGroup_MinionReactToPlayerInteraction: 0092ecd0 IsTooFarFromThePlayer -- a function-static float initialised from
  `const float k = 3.0f; k*k` (the front end treats it as dynamic init: guard byte + `mov [g],0x41100000`), the delta by
  a by-value `operator-`, `X*X + Y*Y + Z*Z` (comes out z,x,y), `fcomp [static]`; 0092ed40 IsTooNearToThePlayer -- the delta
  spilled to the frame across the distance call, `X*X + (Y*Y + Z*Z)` (comes out z,y,x), `fcompp` against `min*min`.
- CAIStateGroup_MinionReactToUnconsciousPerson: 00937d20 IsRunnable -- `person = (state == 3) ? A() : B()`, null + dead-bit
  test, `PPerson = person` through the converting temporary, `mov al,1`.
- CAIStateGroup_MinionSitAtInn: 0090fae0 GetPOwnerObject -- nested ifs, `*ppOwner = inn; return true;`.
- CAIStateGroup_MinionSitAtSchool: 0090fd20 IsRunnable -- `&&` chain with the creature taken as a named local inside an
  inline helper (it rides in esi across the world call; `this` moves to ebx).
- CAIStateGroup_MinionSitOnSeat: 0090f690 Initialise -- base Initialise first, then the random step with range fps*80
  (`lea esi,[esi+esi*4] ; shl esi,4`), `FrameToSitDown = frame + r`.
- CAIStateGroup_MinionSitOnSeatBase: 0090fa60 Cleanup -- `if (UsesGatherPoint()) if (IS_THING_ALIVE(POwner)) { POwner.GetPItem()
  ->GetInterface(&gp,0x7b) ... RemoveUser(creature) }` (the out-param form here, unlike IsRunnable).
- CAIStateGroup_MinionSitOutside: 00939060 IsStillValid -- `setle al` with no zero-extension = an `unsigned char` return
  type (`return GFGetCurrentFrame() <= FrameToStopSitting`), the curfew early return keeps its own byte epilogue.
- CAIStateGroup_OpinionReactionRoot: 0095f660 OnReplace -- the world in a NAMED local before the LogReaction argument
  loads (24a lesson); 00984a20 WatchUpdate -- the random range passed BY CONST REFERENCE reproduces `mov ecx,2Dh ; div ecx`
  (a by-value literal becomes a magic multiply); a `long` result; `if (r != 4) return 0; break;` shares the `mov eax,1`
  exit of the timed-out loop (an `if (r == 4) return 1; return 0;` collapses to `sete`).
- CAIStateGroup_PrepareMeal: 00946100 Cleanup -- `ActionPointIndex != -1`, IS_THING_ALIVE, out-param lookup,
  FreeActionPoint(index); 009462a0 IsTimeToPrepareMeal -- a CDefPointer local built from the RAW def pointer at +0x94
  (`CDefPointer(CVillageDef*)`: a direct `mov esi,[eax+94h]`, a copy from the member forms the address), destroyed after
  IsCookTime with the result in bl; `if (village) { ... } return false;` keeps the byte exit at the end.
- CAIStateGroup_RangedCombatUsePreferredFirePoint: 0095d310 CanUseRangedCombat -- HasTC + FindTC on the carried item.
- CAIStateGroup_SeekScriptTarget: 0093bfa0 IsStillRunnable -- `PEnemy.GetPItem() == tc->GetTarget()` (the virtual call is
  the RIGHT operand: evaluated first, kept in ebx, `cmp eax,ebx`).

## Parked for Fable 5.1 (14)
- 00c17260 CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction DIFFER(144v138): retail never loads esi (`mov ecx,[esp+40h] ; push esi ; ... ; mov ecx,esi ; call 0xC307C0`), the caller 0x00C17364 does `lea esi,[esp+14h] ; call` -- `this` arrives in esi; ours adds `mov esi,ecx` and shifts the frame by 4 | tried: __thiscall model with /EHsc | next: lane gap: custom (link-time) calling convention; also a label correction (engine function of the 0x00C1 module, not an AI body)
- 00933470 CAIStateGroup_MinionGuardReactToBribe::IsStillValid DIFFER(123v123): `xor ecx,ecx ; test edx,100000h ; ... mov ecx,[eax+4] ; mov al,[edi+20h]` vs ours `xor eax,eax ; ... mov eax,[eax+4] ; mov cl,[edi+20h]` | tried: out-param GetInterface with `p = 0`, HasTC + FindTC, inline accessor on the component, declaration after the creature load | next: lane gap: register allocation of a null-initialised component pointer (same family as 0090fb60)
- 0090ee20 CAIStateGroup_MinionKidCalledToMother::FindCallHomeEvent DIFFER(134v134): retail `lea ecx,[esp+4] ; mov [esp+14h],ecx` (Bind3 first pointer) BEFORE the originator's GetPItem + out-of-line IsOriginatedByThing ctor, ours after | tried: inline chain, named creature locals, an inline MotherOf() helper (fixed the `lea`), a member operator&& | next: lane gap: store scheduling across the out-of-line filter constructor (or a Bind2 that binds the left operand before the right is built)
- 0090f110 CAIStateGroup_MinionReactToAlert::FindAlertEvent DIFFER(146v154): retail `call GetPItem ; xor dl,dl ; mov [esp+10h],eax ; jne fail ; call GetPItem` between the null test and the dead-bit test (three GetPItem calls, a constant-false bool never folded, the item homed in a dead slot) | tried: plain IS_THING_ALIVE, a `bool bad = false` local, an inline type check taking `CThing* const&` returning false, an identity-cast compare | next: find the inline (a release-empty type check / checked cast on CIntelligentPointer) whose constant-false result VC7.1 cannot fold -- the same shape sits in 0x006E7820, 0x008941A0, 0x008D04E0, 0x00958B20
- 0090f1c0 CAIStateGroup_MinionReactToAlert::Update DIFFER(143v149): the same typed read on the event's pointer at +0x50 (`xor dl,dl ; mov [esp+0Ch],eax ; jne`) | tried: the typed-read inline, if/else Terminate layout | next: same as 0090f110
- 0090fc50 CAIStateGroup_MinionSitAtSchool::InitCreaturePosition DIFFER(129v127): `mov eax,[eax+4] ; mov ecx,eax ; call GFGetPVillageInterface` vs retail `mov ecx,[eax+4]` | tried: out-param GetInterface + early return, HasTC + FindTC (restructures the miss exit inline), without the creature local, `if (HasTC) { && chain } return false;` | next: a form that keeps the early `xor al,al` tail at the end while the pointer-returning lookup feeds ecx (an inline `GetVillage()` accessor on the creature?)
- 0090fb60 CAIStateGroup_MinionSitAtInn::GetSeat DIFFER(142v140): retail `mov eax,[esp+4] ; mov edx,[eax+34h]` before the prologue and `xor ecx,ecx` / `mov ecx,[eax+4]`, ours loads the argument into ecx after the prologue and keeps the tavern in eax (+`mov ecx,eax`) | tried: HasTC + FindTC with `tavern = 0`, out-param GetInterface on a named building local, the bitset word read first, a lookup-or-null inline | next: lane gap: register allocation (same family as 00933470)
- 0090f9c0 CAIStateGroup_MinionSitOnSeatBase::IsRunnable DIFFER(150v148): retail `mov edx,[edi+4] ; mov ecx,[eax+4] ; mov eax,[edx+18h] ; push eax ; call AddAsUser` (component first, argument after) and the AddAsUser failure jumps to the shared tail; ours loads the argument first, moves the component, and lays the exit inline | tried: out-param GetInterface, HasTC + FindTC, a named FindTC local with a single textual `return false` | next: an inline accessor that yields the component as `this` after the argument is pushed; lane gap otherwise
- 009104d0 CAIStateGroup_MinionSitOnSeatBase::GetSeat DIFFER(158v152): retail `xor esi,esi` zero / count in edi / `mov ebp,[esp+10h]`, ours `xor edi,edi` / count in esi / `mov eax,[esp+10h] ; mov ebp,eax` | tried: STLport vector with size()/operator[], named last/first locals (both orders), first local + member last | next: lane gap: register allocation; the filter-in-the-dead-argument-slot, seed step and `free` behind the null test already match
- 0090bbf0 CAIStateGroup_MinionWorkInInn::IsRunnable DIFFER(100v100): retail `mov ecx,[esi+4] ; mov ecx,[ecx+18h]` / `lea edx,[esp+4]` / `mov eax,[ecx] ; call [eax+4]`, ours edx/ecx swapped and `mov edx,[ecx]` | tried: inline home accessor (fixed the `lea`), hand release on a pointer local, the landed CDefinitionManager::GetDef CRefCounted model (fixed the release shape), a named creature local | next: lane gap: scratch-register choice
- 0093cab0 CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition DIFFER(104v104): `lea esi,[ecx+14h]` (member address) / edi successor vs ours edi / esi | tried: direct member use, a reference local, a pointer local, the successor declared first | next: lane gap: register allocation
- 0095d070 CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy DIFFER(108v108): retail `push esi ; mov esi,ecx ; ... jne body ; xor al,al ; pop esi ; ret` (the shared false exit inline after the first test), ours `this` in edi and the exit at the end | tried: two early returns, `if (Can) { if (!far) return ... } return false;`, an `||` of the two tests, early returns through an inline distance helper | next: lane gap: block placement (the FPU body and the `cmp eax,1 ; setne al` tail match; `== 1 ? false : true` gives the setne where `!= 1` gives dec/neg/sbb/neg)
- 0095d4c0 CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList DIFFER(130v130): retail keeps both empty filters in the frame dword at +4 (`lea ecx,[esp+6]` / `lea edx,[esp+7]`), ours homes the first one in the dead argument slot (`lea ecx,[esp+34h]`) | tried: temporaries in the && chain, named filter locals (own dwords), a named Bind2 (most-vexing-parse SRC_FAIL, then a copy), a member operator&& | next: a filter type whose temporary cannot be homed in the argument slot (one-byte member with a user copy constructor?), else lane gap: temporary homing
- 00939180 CAIStateGroup_MinionSitAtHome::IsRunnable DIFFER(161v155): retail `mov [edi],eax ; xor edx,edx ; mov ecx,96h ; div ecx ; mov edi,edx ; call GFGetCurrentFrame`, ours keeps the rotated seed in ebx, calls first and divides after (`mov edi,96h ; div edi`, +`push ebx`) | tried: range by const reference (gives the literal `div` as in 00984a20 but not the order), the random step as its own statement, the world as an unused helper argument | next: a form that forces the modulo before the frame call (store the remainder through a volatile? a non-inline helper compiled earlier in the TU?)

## Skipped
None (no row holds two functions).

## Label corrections
- 00c17260 is NOT CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction: an engine function of the 0x00C1 module (EH frame, a 0x30-byte "WaypointData"-labelled record, the 0x00C30F60 loader, the 0x00C307C0 flag getter, text append @ 0x00C0DEC0) whose `this` is passed in esi by its caller 0x00C17364. The manifest name is propagated from the AI body at 0x0093D140.
- 007338a0 is NOT CAIStateGroup_MinionReactToNoise::Cleanup: the pointer at +4 is tested for the interface bitset directly (it is the CThing, not an AI brain) and the flags cleared are at +0x62/+0x63 -- a creature-mode object of the 0x0073 module (12 callers, 0x00733B35 / 0x00733FB4 among them). Authored under the catalog name with a local `CCreatureModeWithNoiseFlags` model (leaf `Cleanup`).
- Callee labels used as hints only: 0x007EE540 is GFGetPVillageInterface (manifest "IS_THING_ALIVE<...>"), 0x008FCFC0/0x008FCFA0 are the state group's world accessors ("CPlayer::GetCurrentMode"), 0x0049D870 is GFGetCurrentFrame ("GFGetPreMainMemoryUsed"), 0x0049D810 is CWorld::GetPRandomSeed ("DoSizeof"), 0x004C73D0 is CThing::GetPosition ("DoSizeof"), 0x00662010 reads `[this+0E0h]+0F0h` (a creature life state; "CPixelShader::CountReferences"), 0x008300D0/0x00830100 are +0x1C8 flag getters bits 2/5 ("CanCollideWithLandscape"), 0x00658A35 is the village school-time test ("IsCookTime"), 0x00658E38 is FindNearestBuildingWithProperty, 0x007FEF40 frees an action point ("EditCyclePreviousKeyCamera"), 0x00A55A30 is C2DBoxF::ToIntBox ("GetPenArea"), 0x00661CC0 is a free float getter (`fld [0x0125AAB8]`, 0.8f; "GetCombatMinDistance"), 0x0095DB90's Bind3 label does not match the Bind2 of two empty filters that 0095d4c0 builds.
