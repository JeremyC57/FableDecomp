# Hand chunk 258 (CAIStateGroup_GoToPub .. CAIStateGroup_MinionWander): 24 of 40 byte-exact

All 24 grade RELOCATION_MATCH (grades.txt). Every one has a test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (24)
- **CAIStateGroup_GoToPub**
  - 00948d40 PlayAnims(..., long): into, loop xN and out-of animations, stopping when the tavern game starts. The key shares the out-param slot through a same-TU LowerBound stand-in, and the source keeps an explicit `if (failed) return`.
  - 00948ef0 PlayAnims(..., float): the timed variant. Same stand-in LowerBound; keeps retail's `frame > endFrame` loop test.
  - 009498b0 OnReplace: frees the seat and bar point, gets up from the table and clears the pointers. The `gotUp` bool is block-scoped and folds after the name dtor.
- **CAIStateGroup_Guildmaster** 00977c40 OnActivate: collects the GuildmasterTeleportMarker track things within 55 units. Uses a named filter, and a same-TU collector stand-in keeps the vector start in esi.
- **CAIStateGroup_HoverHornetQueen** 0095b3e0 SetupArena: circle points, not-solid radius, and an arena centre from the track thing. The creature is named before the vector; the centre out-pointer sits in its own block.
- **CAIStateGroup_HoverMovementBase** 0091d320 AIAction_MoveToAndFaceThingAtDistance: an inline bool alive helper keeps the early return inline.
- **CAIStateGroup_IceTrollIcicleAssault** 00961fb0 IsRunnable: same-TU noinline GetSubDef stand-in plus a by-value def-class temporary.
- **CAIStateGroup_JackOfBladesStage3** 00972670 OnActivate: the will lookup is checked; the combat and hit-location accessors are unchecked.
- **CAIStateGroup_KidReactToBlockage** 00941e00 Update: look, then a three-part tantrum. An inline brain-creature getter orders the PlayAnimation args.
- **CAIStateGroup_KidWatchHopscotch** 00941210 IsRunnable: fatigue hysteresis, then the nearest board. Needs an inner-block board out-pointer, a per-arm rate expression, and a `U* const&` pointer ctor.
- **CAIStateGroup_LookAtInterestingThings** 009195d0 Update:
  - walk to within 6 units of the thing, honouring the navigation def's AvoidDynamicObstacles, then look;
  - the same-TU GetSubDef stand-in keeps the def in edi;
  - RefCount is a direct member at +4 (`dec [reg+4]; jne`);
  - the look loop is written `if (r == 4) continue; break;`.
- **CAIStateGroup_MazeRangedCombat** 00954470 IsRunnable: a plain inline operator= (out of line once, inlined once).
- **CAIStateGroup_MeleeCombatNearby2**
  - 0093dde0 AttackEnemy: inline CBrainOutput ctor and SetAsLookAtThing, with float NavigationInput zeros.
  - 0093e090 MoveToEnemyCreatureCombatPosition: a named creature local before the UID read.
- **CAIStateGroup_MeleeCombatNearbyBase**
  - 0093d3a0 IsEnemyRunningAway: an inline CThing::GetFacing forwarder fixes the vptr register.
  - 0093d580 RequestMoney: out-param GetInterface for interfaces fine, 4 and 8.
- **CAIStateGroup_MinionFollowThing** 00921440 IsExpressionPerfomedTo: a by-value def getter plus a const out-param GetInterface.
- **CAIStateGroup_MinionGuardReactToCriminal** 00932990 IsRunnable: an uninitialised out-param villager and a named item in the pointer operator=.
- **CAIStateGroup_MinionGuardReactToPlayerWithWeapon** 00939ed0 IsRunnable: named world and fps locals before the static frame read.
- **CAIStateGroup_MinionHasInformation** 0090f470 Update:
  - look at the hero; wave every 5 s (ATTRACT_HERO_ATTENTION, only every fifth wave after 20 s); wander every 10 s;
  - an inline `HasTimePassed(const long&, long)` loads fps into edi before the frame call;
  - the loop is a do/while on DoNothing, with explicit `Terminate(); return;` exits.
- **CAIStateGroup_MinionReactToCriminal** 00931010 IsRunnable: named creature locals for the building reads, and an fps long local.
- **CAIStateGroup_MinionReactToEnemy** 00937da0 Update:
  - back off 2.4 units when the enemy is within 5 units in XY, then look at it;
  - an XY distance helper takes `const C3DVector&` positions (second argument first) and stores into a float local;
  - `away *= 2.4f` after the out-of-line Normalise;
  - a user copy ctor builds `pos + away` in the argument slot.
- **CAIStateGroup_MinionSleep** 0092c5a0 IsStillValid: FindMostRecentMessage plus both LowerBound stand-ins.
- **CAIStateGroup_MinionWander** 0092c070 Update: a def holder and a function-scope `reached` bool.

## Parked for Fable 5.1 (16)
- 0096ec10 CAIStateGroup_MeleeCombatNearby::MoveToEnemyCreatureCombatPosition DIFFER(408v407): inlined LowerBound `mov eax,[+44h]..mov edi,eax` copy, proximity fmul/fadd scheduled before pushes, eax/edx for slot-14 vtable | tried: inline LowerBound (3 shapes), ternary select, GetPCombatManager accessor, named diameter float/double/extern scale | next: lane gap: register coalescing of an inlined binary search
- 00929650 CAIStateGroup_MinionGuardGreetPlayer::IsRunnable DIFFER(448v452): region def holder lives in edi (retail ebp), next-greeting/hero in ebp (retail edi); otherwise instruction-identical | tried: raw-pointer vs copy ctor, pointee AddRef/Release, named region ref, nested ifs, early-return flow, inline next-greeting helper, frame/hero/self locals, named dist | next: lane gap: callee-saved register choice
- 009490d0 CAIStateGroup_GoToPub::GetBar DIFFER(458v459): ebp (loop iterator) is pushed after the contents lookup and popped after the loop in ours; retail saves it at entry | tried: LowerBound stand-in, iterator at function scope, named found thing, cached end (fixes the push, breaks registers), if(!empty) layout, forceinline/inline collect helper, forceinline list members | next: lane gap: callee-saved push placement (shrink-wrap)
- 00949380 CAIStateGroup_GoToPub::ThankBarmaid DIFFER(497v498): creature in edi / talk in ebp (retail esi / edi) through the two inlined HasTC-reference GetInterface lookups | tried: declaration order (talk/scratchpad/tankard first), block-scoped creature, forceinline HasTC, LowerBound stand-in, unnamed map (fixes creature reg, loses lea/add map: 494v498), map reference, named end | next: lane gap: register coalescing of the map pointer with the out component
- 0095ab50 CAIStateGroup_HoverHornetQueen::SetUpCirclePoints DIFFER(456v456): the found marker is loaded into ecx (alive test) / edx (loop-bottom test) and copied to esi; retail loads esi directly both times | tried: do-while vs for+break, named found copy (block/loop scope), forceinline IS_THING_ALIVE, marker at function scope, found-based loop test (frame grows), index/zero before clear, marker zeroed before sprintf | next: lane gap: register targeting of a reloaded escaped local
- 0095b820 CAIStateGroup_HoverHornetQueen::Spawn DIFFER(445v445): the filter index store is scheduled after the PCreature load, and the 0xad lookup keeps the creature in eax with lea edi,[eax+0x44] (retail edi + add edi,0x44) | tried: same-TU collector stand-in (fixed the vector caching), index local in the box block (fixed the shared name/filter/boss slot), filter ctor/aggregate/assignment, creature before filter, filter after GetPosition, unnamed map, map before the test, HasTC+GetTC, map-member Find | next: lane gap: same as 00949380 (map pointer coalescing with the creature register)
- 0094fa00 CAIStateGroup_JackDragon::FindFlightPathCurvePoint DIFFER(421v421): retail hoists `mov edx,[out]` / `pop esi` above the Normalise deferred Z*r fmulp; ours schedules them 3 instructions later | tried: separate scale statement (reorders lengths), commuted final add, out alias reference, Normalise sum parenthesisations, Normalise writing from the locals, swapped scale operands, half distance before Normalise (sunk) | next: lane gap: x87 scheduler placement of an independent integer load
- 0094fc00 CAIStateGroup_JackDragon::CreateIBC DIFFER(476v482): retail copies the GetFacing return temp through eax into the sum operand at the same slot (x,y to x87, z stored); ours elides the copy, which also moves the init zero stores | tried: block-scoped facing (member-wise / ctor sum), facing assigned after declaration, user copy ctor, member/non-member/by-value operator+ (5 forms), += based sums, named and const-ref facing, inline CThing::GetFacing wrapper | next: find the retail physics GetFacing return shape (maybe a wrapper type) or an inlined helper whose by-value parameter shares the temp slot
- 009718f0 CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay DIFFER(510v510): retail squares path.Y before path.X (CSE order into the XY sum and the Normalise length) and pops esi earlier | tried: 4 Normalise sum parenthesisations x 2 XY member forms, explicit XY sums (worse), swapped XY statements (worse), forceinline XY helper with locals (worse), QFE-4035 compiler (same) | next: lane gap: x87 CSE materialisation order
- 00971cd0 CAIStateGroup_JackOfBladesStage1::GetRushToPos DIFFER(506v510): retail keeps Jack in ebp across the target GetPItem (target in ebx, pushes shrink-wrapped) and squares dy before dx in the length | tried: ternary of thing vs of positions (positions fixed it), named left/right with if/else copy (fixed the select), target/creature declaration order (fixed the entry push), XY-offset inline helper, 3D difference, Y*Y+X*X (VC canonicalises) | next: lane gap: register allocation of a value loaded before a call
- 00919240 CAIStateGroup_LookAtInterestingThings::FindInterestingThing DIFFER(508v508): eax/ecx permutation in the first weighting loop (thing vs component) and ecx/edx vs edx/eax temps later; filter store scheduled one load later | tried: collector stand-in (fixed vector caching), FrameToLook forms x6 (random then fps local fixed it), named component locals / out-param form / shared loop index, total before count, thiscall float random, filter inside the block or as a temporary | next: lane gap: register tie-breaking in a loop body
- 0092ea90 CAIStateGroup_MinionFindPersonalSpace::Update DIFFER(444v440): retail evaluates the direction difference Z,Y,X (X on the stack top, spilled only after scaling) and spills the navigation-present bool into the arrived byte | tried: operator- via ctor, direct-init from position references, GetNormalised by value (worse), in-place *=, member-read Normalise sum | next: lane gap: x87 stack order of an inlined vector temporary; try a Normalise model that writes back through locals in Z,Y,X order
- 00910df0 CAIStateGroup_MinionGuardHeardYellForHelp::Update DIFFER(500v500): retail tests the originator type as `xor eax,0xf ; test al,0xf` and loads the brain into eax before the slot-92 turn-speed call | tried: (f & 0xf) == 0xf, ~f & 0xf, (f ^ 0xf) & 0xf, ((f & 0xf) ^ 0xf), a 4-bit bitfield == 15, IsThingOfType<T> mask template with a typed-item ternary (worse), four bit tests (worse) | next: find the engine's thing-type predicate that VC lowers to xor/test (maybe an enum compare through a by-value helper); everything else (filter chain, temporary action via &T(...), template copy ctor of the pointers) matches
- 0091e750 CAIStateGroup_MinionMotherConsoleChild::Update DIFFER(432v432): retail keeps `this` in edi and the predicate constant / child in ebp; ours swaps them (and shrink-wraps the esi/edi pushes) | tried: GetEvent stand-in (fixed the hoisted predicate store), plain-inline GetInterface returning its present local (fixed the present-byte store), 6 sum parenthesisations (X*X + (Y*Y + Z*Z) fixed the x87), result/event/child declaration order, creature local, early-return form, QFE-4035 compiler | next: lane gap: register tie-breaking for `this`
- 00935c40 CAIStateGroup_MinionPutOutFires::GoToWell DIFFER(494v502): frame slot layout (retail filter 0x14/well 0x20/compare 0x24, arrived 0x13, present byte 0x1f), mov eax,[eax+0x44];mov esi,eax vs mov esi,[eax+0x44], missing lea ebx,[ebx] loop pad | tried: plain-inline vs __forceinline GetInterface (forceinline needed), nested-if/while loop (522), arrived declared first (494) | next: same-TU noinline stand-in for GetPBestLeafThing 0x936100, reorder filter/compare temporaries
- 00935e40 CAIStateGroup_MinionPutOutFires::PutOutFire DIFFER(476v468): inline lower_bound loads first straight into esi (retail mov eax,[eax+0x44] ... mov esi,eax); PlayAnimation zero args push 0 (retail xor ecx,ecx; push ecx x4 + CSoundPair stores via ecx) | tried: std-style _Lower_bound helper (plain: not inlined; forceinline: same), separate begin copy, inline member LowerBound, tail inside the loop | next: the e750 action call does get the zero CSE, so look for what frees ecx/edx there; the eax-then-esi copy also remains in 00935c40

## Skipped
None. All 40 rows were attempted; 24 matched and 16 were parked with their best source in differ/.

## Label corrections
- 0x0094B400 is CTCTavernTankard::SetPBarmaid; the propagated label is wrong.
- 0x00978250, 0x0095C7C0, 0x008E3EB0 and 0x00919780 are 3-argument rough-area thing collectors, not the labels propagated onto them.
- 0x0094CA80: its base OnReplace is 0x009498B0 (CAIStateGroup_GoToPub::OnReplace).
- 0x004C9840 is CThing::RemoveTC.
- 0x00449960 ("Catch@...") is CPlayerManager::GetMainPlayer, and 0x008FCF80 ("CPixelShader::CountReferences") is the state group's player-manager accessor; both labels are propagated.
- 0x006AF1C0 (labelled HasExtraData) is CMessageEventFilter_IsOfType::operator().
- 0x00661CC0 (labelled GetCombatMinDistance) is a static float-constant getter.
- 0x006A4D00 (labelled DoSizeof) is CThingCreatureBase::GetPosition.
- 0x00910B20 (labelled _Dest_val) is ~CCreatureAction_DeclarePlayerACriminal, and 0x00693EF0 (labelled CPhysicsMeshInfo) is ~CCreatureActionBase.
- 0x00843840 (labelled AddRealObjectToInventory) is the CCreatureAction_FillBucket ctor.
- 0x00936100 carries a propagated filter label (CIsFreeHopscotchBoard). It is CThingSearchTools::GetPBestLeafThing<CThingFilter_HasThingTypeFlags, CThingCompare_Nearest, CThingObject>, instantiated in the PutOutFires TU.
