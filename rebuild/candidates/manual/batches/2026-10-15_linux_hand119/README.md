# Hand chunk 119 (CAIFilter_ShouldStillBeWorkingInWarehouse .. CAIStateGroup_Gaze): 30 of 40 byte-exact

All 30 sources grade RELOCATION_MATCH (grades.txt) and every behaviour test prints PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). No pragma or flag was needed anywhere.

## What matched (30)
- **CAIFilter_ShouldStillBeWorkingInWarehouse** -- 0x00944470 constructor (village in the init list, two inline
  CIntelligentPointer copy constructors, the 8-byte by-value arguments destroyed through the base destructor);
  0x009444E0 operator() (nested ifs with ONE `return true` reproduce the early `push ebx` and the shared
  `xor al,al` tail -- sequential early returns duplicate epilogues; needs `// Leaf: ??R<Class>`).
- **CAIStateGroupBase** -- 0x008FD1B0 DestroyItem (engine headers + the landed GetInterface out-parameter
  template, `Get().GetDefGlobalIndexFromName(name)`); 0x0097E200 AIAction_DoNothing (GetInterface must be
  `__forceinline` next to two other calls; `IsStopping() ? 1 : 4` gives `neg ; sbb ; and -3 ; add 4`);
  0x009812C0 / 0x00981EA0 AIAction_SpeakAISpeechToTarget (six-to-seven argument forwarders: the by-value
  CIntelligentPointers are copy-constructed in their argument slots and released after the call).
- **CAIStateGroup_BanditPlayDice / BanditPlot** -- 0x009284B0 InitStateGroup (two inline world-random steps,
  `frame + r * 0.5f` with the draw in a named local before the frame call); 0x00929220 / 0x009292B0 GetDiceGame
  (two `__forceinline` GetInterface lookups around GFGetPVillageInterface).
- **CAIStateGroup_BanditSitRoundCampfire** -- 0x0093A540 Idle (`GetPWorld()->Static()` keeps the discarded
  GetPWorld call; frame rate read into a local before the seed call; named float delay; `do/while`).
- **CAIStateGroup_BirdGroundWander** -- 0x009143E0 IsRunnable (untested GetInterface: the miss path reads the dead
  out slot; frame rate local doubled with `lea`).
- **CAIStateGroup_BoastingCrowdMember** -- 0x0095E870 IsStillValid (nested ifs, int-width result).
- **CAIStateGroup_CleanWork** -- 0x00917DB0 GetBuilding (an inline accessor RETURNING `member.GetPItem()` gives
  `lea ecx,[eax+170h]`; direct member access or a pointer-returning accessor gives `mov ecx ; add ecx,170h`).
- **CAIStateGroup_CreatureAdvanceInCover / CreatureTakeCover** -- 0x00979260 / 0x0097AAE0 ProcessCoverThingList
  (`C3DVector d = a - b;` through an inline operator- keeps a 12-byte frame slot with the Z subtraction dead; the
  sort comparator is constructed in place in the argument area only with a user copy constructor; its members
  come from C3DVectors through `C2DVector(const C3DVector&)` = dword moves).
- **CAIStateGroup_CreatureIdleUnderground** -- 0x009208E0 OnActivate (action temporary inside the
  SetCurrentAction call, creature in a named local).
- **CAIStateGroup_CreatureRangedCombat2** -- 0x0091E990 Cleanup (if/else with the trailing flag store
  tail-duplicated; implicit CCharString conversion temporary for AddTC; an inline `GetCreature()` accessor lands
  the creature chain in ecx).
- **CAIStateGroup_CreatureRangedCombatUseWillShield** -- 0x0091F0D0 Update (a NAMED CCharString in an inner scope
  built before any push and destroyed before Terminate; the creature a named local -> the `eax` chain; the action
  temporary inside the call).
- **CAIStateGroup_CreatureSeekEnemyUnderground** -- 0x00920990 OnActivate (CIntelligentPointer reset through a
  converting-constructor temporary; the C3DVector member assigned with the IMPLICIT POD assignment gives
  `lea ecx,[esi+1Ch]` + stores through ecx).
- **CAIStateGroup_DeedReactionRoot** -- 0x0095FE60 InitStateGroup (one zero register; `class` forward declarations
  so the mangled name matches the oracle exactly).
- **CAIStateGroup_EatAtTable / EatAtTavern** -- 0x0096D190 IsStillValid (untested GetInterface, slot-15 result
  returned); 0x0096D260 GetBuilding (village nearest-building query, arguments right to left).
- **CAIStateGroup_EnemyCreatureBase** -- 0x0091D850 Cleanup (creature AND world in named locals so the world call
  precedes the push); 0x0091DD30 IsRunnable.
- **CAIStateGroup_FollowPlayer** -- 0x0094EF90 IsRunnable (chained player getters); 0x0094F000 Update
  (`if (alive) { while (Move(..) == 2) DoNothing(); }` -- the alive test is not repeated; a `while (alive)` loop
  is rotated).
- **CAIStateGroup_GameMaster** -- 0x0094C7E0 InitStateGroup (base GoToPub call then resets; `protected` virtual for
  the `MAE` symbol).
- **CAIStateGroup_Gaze** -- 0x00918190 constructor (inline CAIStateGroupBase constructor over the out-of-line CBase,
  implicit-conversion name temporaries for the two def indices; `// Leaf: ??0CAIStateGroup_Gaze` because the TU
  also emits `??_G`); 0x00918880 FindFreeGazePos (bool frame byte reloaded as a dword for the pushes; game time in
  a named local before the sleep-time call); 0x00918AC0 InitCreaturePosition (world-random `< 30.0f`, marker and
  creature named locals).

## Parked for Fable 5.1 (9)
- 009578a0 CAIStateGroupBehaviourBase::UpdateFocusAttentionOnThingActive DIFFER(148v148): retail `cmp eax,2 ; je turn ; xor al,al ; ret ; turn: call TurnToFace`, lane `jne false ; call TurnToFace ; ret ; false: xor al,al` | tried: for(;;) with continue + `== 2 return TurnToFace()`, inverted `!= 2 return false`, do/while, switch | next: lane gap: block placement of the final two-way exit
- 0095ebf0 CAIStateGroup_BoastingCrowdMember::OnReplace DIFFER(109v107): retail `mov ecx,[eax+4] ; mov eax,[edi+4] ; mov eax,[eax+18h] ; mov edx,[ecx] ; push eax`, lane loads the creature into ecx, the component into eax, then `mov ecx,eax` | tried: out-parameter GetInterface, forceinline pointer-returning find, an inline wrapper taking PBrain | next: lane gap: register allocation of a virtual call's `this` (same as 0x0091F050)
- 00919a70 CAIStateGroup_BuyFromSeller::GetSellersReply DIFFER(105v105): retail homes the reply pointer in the dead `seller` argument slot (`sub esp,20h`) and lowers `status == 2 ? reply : 0` as `sub eax,2 ; neg ; sbb ; not ; and`; the best source has its own slot (`sub esp,24h`) and `xor ecx,ecx ; cmp ; setne ; dec ; and` | tried: `&&` filter chain (builds the seller filter first), by-value Bind2 operand, three-argument Bind3 constructor (fixes the build order), named filter local | next: an `&&` chain whose operator evaluates the right operand first (the chain form DID home the reply in the seller slot), else lane gap: frame-slot sharing
- 00965f30 CAIStateGroup_CarryCrateAndFollow::FindPCrate DIFFER(115v115): retail loads the three position words (ecx, edx, eax) before storing them into the comparator, the lane interleaves the first two stores | tried: implicit POD copy, by-value constructor parameter, body assignment, three scalar float members | next: lane gap: copy scheduling
- 00965fb0 CAIStateGroup_CarryCrateAndFollow::IsRunnable DIFFER(126v126): retail `call GetPItem ; add esi,14h ; push eax`, lane `add esi,14h ; call GetPItem` | tried: operator= returning a reference / void, a named temporary in a scope, a member pointer local | next: lane gap: scheduling of the member address around a call
- 0091f050 CAIStateGroup_CreatureRangedCombatUseWillShield::IsRunnable DIFFER(128v126): retail `mov ecx,[eax+4] ; mov eax,[ecx] ; call [eax+74h]`, lane `mov eax,[eax+4] ; mov edx,[eax] ; mov ecx,eax ; call [edx+74h]` | tried: out-parameter GetInterface, pointer-returning find, the lookup written inline, an uncast CTCBase virtual | next: lane gap: register allocation of a virtual call's `this` (same as 0x0095EBF0)
- 009708a0 CAIStateGroup_CreatureUseCoverBase::RemoveThingFromCoverList DIFFER(143v143): `this` in ebp and the copy count in ebx in retail, swapped in the lane | tried: STLport erase + __copy loop (both `__forceinline` needed for the shape), a list reference local | next: lane gap: callee-saved register colouring
- 00960f00 CAIStateGroup_DeedReactionRootFromSleep::IsRunnable DIFFER(118v118): retail pushes esi in the prologue (`push ecx ; push esi ; push edi`), the lane shrink-wraps `push esi` after the interface-bit test | tried: nested ifs, a result variable, optimize("s"), early `return false` | next: lane gap: shrink-wrapping
- 009226b0 CAIStateGroup_Drunk::IsStillValid DIFFER(156v156): retail stores the Bind2 first pointer before the IsForThing constructor call and the 0x14 type after the creature push, with the creature in eax; the lane stores the type first, the pointer after the call and uses ecx/edx | tried: creature named locals, a member operator&& | next: lane gap: store scheduling around an out-of-line filter constructor

## Skipped
- 009576c0 CAIStateGroupBehaviourBase::OnActivate: the 144-byte row holds two functions -- the 13-byte OnActivate
  (`xor eax,eax ; mov [ecx+28h/30h/2Ch],eax ; mov eax,[ecx] ; jmp [eax+40h]`) and a 123-byte predicate at
  0x009576D0 (attention level != 0, perceiving, focused, distance^2 < 25) with no direct references.

## Label corrections
- 0x004C73D0 "CVertexBufferWin32::DoSizeof" -> CThing::GetPosition (const C3DVector*; four rows).
- 0x0049D870 "GFGetPreMainMemoryUsed" -> GFGetCurrentFrame (as the landed 0x00934090).
- 0x007EE540 "IS_THING_ALIVE<..>" -> GFGetPVillageInterface(CTCVillager*) (as the landed 0x0093EDF0).
- 0x008FCFA0 / 0x008FCFC0 "CPlayer::GetCurrentMode" -> CAIStateGroupBase world accessors (PBrain->+0x1c->+0x24);
  0x008FCF90 "CPixelShader::CountReferences" -> CAIStateGroupBase player-manager accessor (PBrain->+0x20).
- 0x0073A6E0 "CPixelShader::CountReferences" -> CTCCreatureModeManager current-mode getter; 0x0073A8A0
  "CPlayer::HasMode" -> CTCCreatureModeManager::HasMode; 0x006CB6A0 "CIsHitEventBlocked::operator()" -> the mode
  manager's current mode-type getter.
- 0x0062E900 "CTCLightningOrb::~CTCLightningOrb" -> the creature-action base destructor (0x114/0x124-byte actions).
- 0x00662410 "C3DAnimationBlendState::AddCombinationBlend" -> a creature predicate (registered combatant).
- 0x0091A670 "CCOE_AltarEvil::Main" -> the CAIStateGroupBase templated wait-for-message action.
- 0x00744430 "CTCInventoryBase::GetCanItemBeQuickAssigned" -> a CTCPerceiveThing predicate.
- 0x00957750 "CAIStateGroup_OpinionReactionRoot::TurnToFace" -> CAIStateGroupBehaviourBase::TurnToFace (propagated).
- 0x00449970 "Catch@0040d959" -> CPlayerManager::GetMainPlayer; 0x00487DC0 "_Cons_val<..>" -> CPlayer::GetPlayerCreature;
  0x008FCF40 "CXMVPlaybackInfo" -> CAIStateGroupBase::Terminate (all as in landed sources).
- 0x006BB390 "Game_IsTimeOfDayInRange" (_global, unknown convention) -> a thiscall member of the world's +0x1c object.
- 0x009812C0 keeps its catalog name AIAction_SpeakAISpeechToTarget, but its target 0x00980BD0 is labelled
  AIAction_SpeakToTarget: the row may be the six-argument AIAction_SpeakToTarget overload.
