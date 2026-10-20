# Hand chunk 219 (CAIStateGroup_KidHurtSelfRunToMother .. CAIStateGroup_VersusRockTroll): 15 of 40 byte-exact

All 15 grade RELOCATION_MATCH under the lane default (RTM cl 13.10.3077, no flags, no pragmas); every test prints
PASS under g++ and the lane's VC7.1. Sources are self-contained local models (the engine headers flatten the
CAIStateGroupBase base), built from the landed CAIStateGroup_* shared model (GetInterface over the +0x20 bitset /
+0x44 vector map, CIntelligentPointer temporaries, IS_THING_ALIVE).

## What matched (15)
- CAIStateGroup_LightHomeCandle::IsRunnable (0x00946B50) -- refresh the candle from the alive home unless its material
  is flagged; the CDefPointer temporary needs `__forceinline` on its dtor and release (a plain inline dtor stays an
  out-of-line call on the early-return path).
- CAIStateGroup_MinionBehaviourSad::Update (0x0096F050) -- approach, look, kneel once (DROP_TO_KNEES), emote; separate
  `if (r != 2) { Terminate(); return; }` exits tail-merge with the last one inline (an `||` condition moves the
  Terminate block to the end); look speed is 0.0027777f, not 1/360.
- CAIStateGroup_MinionFollowThing::Cleanup (0x00921870) -- the bool out-parameter GetInterface in an `if` for 0xAA, but
  `if (HasTC(n)) { T* p; GetInterfaceV(&p, n); p->f(); }` (followee) and `if (HasTC(7)) FindEntry(7)->component->f(0)`
  (navigation): using the bool form for those flips `this` to esi; the bool argument widens `xor eax,eax ; mov al`.
- CAIStateGroup_OpenAndCloseShop::InitStateGroup / InitCreaturePosition (0x00926A10 / 0x00926B30) -- one
  `__forceinline` UpdateShopState() body; the keeper-present `|= 1` must go through a NAMED building local (a member
  of a call result compiles to `mov dl ; add eax ; or dl ; mov`), `!open` passed straight to LockAllDoors and the sign.
- CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo (0x0095EE70) -- first try: packed +0x6b pointer, the two
  AIAction_SpeakAISpeechToTarget overloads (7 / 6 args).
- CAIStateGroup_RangedCombatUsePreferredFirePoint::Update (0x0095D200) -- first try: tail-merged Terminate exits around
  a do/while strafe loop and a `while (AttackEnemy())` loop.
- CAIStateGroup_TrollBase::HasEnemyBeenKnockedDown (0x0094D9C0) -- first try: two filter-bind chains
  (`CIsEventCreatedAfter(f) && CIsHitEventHitAndKnockedDownBy(c)` / `&& CIsHitEventHitByExplosion()`), each event local
  in its own block.
- CAIStateGroup_TrollBase::ThrowRocks (0x0094DC40) -- the landed CBrainOutput model; the throw action is a TEMPORARY
  passed by pointer to an inline SetPerformAction(output, action) (the named-local form devirtualises Clone and
  re-loads Flags; retail calls Clone through the ctor's returned this and ORs Flags in memory).
- CAIStateGroup_SitInPub::Update (0x0094B740) -- the fidget-interval global is read BEFORE `*seed = rotated` (the
  natural store-then-read order also matches under `--flags /Oa`); the pub component via `if (HasTC) { GetInterfaceV }`
  on a named building local.
- CAIStateGroup_SitInPub::IsRunnable (0x0094B8B0) -- `PBuilding = pub;` (IMPLICIT conversion through the out-of-line
  CIntelligentPointer<const CThing>(CThing*) ctor: operator= then gets `lea eax,[temp]`; an explicit temporary reuses
  the ctor's eax), the AddAsUser argument through an inline GetCreature() accessor, and `if (seat) SetSeat(seat); else
  SetSeat(Random());` (this set in both predecessors).
- CAIStateGroup_StayNearSchool::IsRunnable (0x009167F0) -- CDefPointer material test; the 20-unit XY range test is a
  `__forceinline` bool helper (`mov eax,1 / xor eax,eax`) negated into bl (`sete bl`) across the release.
- CAIStateGroup_StayNearSchool::Update (0x00916910) -- `do { if (!valid) { Terminate(); return; } r = Move(...); }
  while (r == 4);` with the GetInterfaceT (materialised bitset-test byte) lookup in the condition; the move target a
  named pointer read before the pushes.
- CAIStateGroup_TavernServer::PutDownJug (0x00967A20) -- CCreatureAction_PutDownJug is a derived action whose inline
  ctor passes "PUT_EMPTY_JUG_ON_BAR" as an IMPLICIT CCharString conversion in its base mem-initializer (built before any
  push and destroyed before the derived vtable store); the jug component via `if (HasTC) { GetInterfaceV }`.
- CAIStateGroup_MinionHasInformation::Wander (0x0090F320) -- the squared distance and squared radius are NAMED float
  locals (`fcomp st(1) ... fstp st(0)`; inline operands give fcompp), tested `!(d2 < r2)`; the two failure exits are
  separate tail-merged `{ Terminate(); return false; }` blocks; GetInterfaceT for the 0xAA component.

## Parked for Fable 5.1 (25)
- 00915080 CAIStateGroup_KidHurtSelfRunToMother::IsRunnable DIFFER(266v266): x of the C3DVector sum `fld [eax] ; fadd [esi]` v retail `fld [esi] ; fadd [eax]` (y/z match) | tried: member/free operator+, swapped operand order, const-ref temp | next: an operator+ written another way (or a different vector ctor) for the x lane
- 0093d270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable DIFFER(297v295): `lea esi,[ebx+14h] ; mov ecx,esi` hoisted before the temporary's GetPItem v retail `lea ecx,[ebx+14h]` after | tried: void/ternary/FindEntry lookups (ternary fixed the tracker register), named block temporary, /Oa | next: lane gap: member-address CSE across the vcall
- 00957db0 CAIStateGroup_MinionBehaviourCrowdBase::Update DIFFER(266v266): this/component esi<->edi swap (retail pushes edi in the prologue) + Terminate tail before the look loop | tried: TestTC bool as `reached`, GetInterfaceT, do/while, FindEntry/GetInterfaceV | next: lane gap: register colouring
- 0092e950 CAIStateGroup_MinionFindPersonalSpace::IsRunnable DIFFER(323v308): retail keeps 0 in ebp for the vector zeroing and the after-loop free test; box built without lea; filter/temporary slots differ by 4 | tried: 8-byte filter | next: STLport vector base model + loop form
- 0090ca60 CAIStateGroup_MinionGossip::OnReplace DIFFER(287v287): bound-filter first pointer stored before the UID-filter ctor call (scheduling) + eax/ecx/edx permutation | tried: message-manager accessor, member operator&& | next: lane gap: scheduling
- 0090c1a0 CAIStateGroup_MinionGuardFollowPlayer::IsRunnable DIFFER(293v293): the hero-position copy for CThingCompare_Nearest stores y after the first push in retail (loads all three first) | tried: user copy ctor (init-list/body), by-value compare ctor, /Oa, QFE compiler | next: lane gap: struct-copy scheduling (same as 00937B10/00937C20/00936A60)
- 0090c540 CAIStateGroup_MinionGuardFollowPlayer::Update DIFFER(299v299): esi/edi colouring swapped; materialised test byte at [esp+0eh] in retail, ours [esp+0fh] (reached flag the other one) | tried: reached declared first/last, global-plus-frame form | next: lane gap: register colouring
- 0090c670 CAIStateGroup_MinionGuardFollowPlayer::OnReplace DIFFER(251v249): CTCVillager result merges into eax then ecx for the fastcall (retail straight into ecx) | tried: FindEntry, accessor, ternary, thiscall GetPVillage | next: lane gap: phi register
- 00946190 CAIStateGroup_PrepareMeal::Update DIFFER(259v259): LookAtPos C3DVector slot pointer ecx (ours) v eax (retail) | tried: named position ref (fixed order), free operator+, POD vector | next: lane gap: register choice
- 00946400 CAIStateGroup_PrepareMeal::IsRunnable DIFFER(283v284): ebx/ebp roles swapped, &PCookingPot computed before the clear temporary's GetPItem, alive-test tail folds to `mov al,1 ; test al,dl` | tried: implicit conversion (fixed the operator= argument), GetCreature accessor, explicit alive test | next: the tail return form
- 0095d3a0 CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace DIFFER(287v285): 0xAC component merges into eax then ecx for SetUser(0) | tried: void/ternary/accessor/KidCleanup lookups, inline wrapper | next: lane gap: phi register
- 0095d0e0 CAIStateGroup_RangedCombatUsePreferredFirePoint::IsStillRunnable DIFFER(283v285): loop filter temporary stored before the two pushes (retail after, from an eax copy) | tried: three loop-body forms; elapsed local fixed the tail | next: lane gap: scheduling
- 0094dac0 CAIStateGroup_RockTroll::IsRunnable DIFFER(243v243): this/enemy/position registers permuted (esi/ebx/edi v ebx/edi/esi) | tried: tracker accessor v local, re-read position | next: lane gap: register colouring
- 0093c830 CAIStateGroup_StealthSeekEnemy::Update DIFFER(322v318): the two inlined lookup keys get separate frame slots (sub esp,8 v push ecx) | tried: blocks, inline helper, non-template lookup core, FindEntry | next: lane gap: inline-local slot packing
- 0093c2f0 CAIStateGroup_StealthSeekEnemy::IsRunnable DIFFER(303v303): the same key-slot packing + eax/edx roles in two brain loads (GetCreature() fixes them but recolours the rest) | tried: implicit conversion (fixed the operator= argument), GetCreature | next: lane gap: slot packing
- 00937c20 CAIStateGroup_MinionReactToUnconsciousPerson::FindUnconsciousPerson DIFFER(253v253): compare-position copy loads all three dwords first in retail | tried: UID/fps local order (fixed), copy ctors, /Oa, QFE | next: lane gap: struct-copy scheduling
- 00937b10 CAIStateGroup_MinionReactToUnconsciousPerson::FindUndiscoveredUnconsciousPerson DIFFER(261v261): same copy scheduling as 00937C20 | tried: same model + Bind4/unary_negate | next: lane gap: struct-copy scheduling
- 00936a60 CAIStateGroup_MinionReactToCorpse::IsRunnable DIFFER(307v307): compare-copy y store after the first push, corpse kept in esi (retail reloads ecx), eax/ecx/edx roles around AreThingsAllies | tried: named dead-thing local (fixed size), block-scoped corpse | next: lane gap: scheduling
- 00930ee0 CAIStateGroup_MinionReactToCriminal::IsValid DIFFER(294v297): first lookup keeps the map in esi and last in ecx (retail overwrites esi, re-takes the lea) | tried: nested ifs (fixed the exits), ternary/FindEntry lookups, named self local (fixed the tail) | next: lane gap: lookup register
- 0092bf50 CAIStateGroup_MinionWander::FaceAwayFromBuildings DIFFER(317v287): filter/compare temporaries laid out in the opposite order with no spill gap, and the away-vector spill differs | tried: typed building pointer | next: the argument temporaries as named locals; the away expression form
- 00964ca0 CAIStateGroup_StealStealableItem::FindPEscapePoint DIFFER(244v245): ours materialises 0 in ebx (vector, compare, result); retail keeps the result in edi and zeroes the vector with immediates | tried: else-block vector (fixed the frame), STLport vector base, per-branch results | next: lane gap: zero register
- 00938280 CAIStateGroup_UsesSeat::AllocChairSittingMode DIFFER(248v248): mode/seed esi<->ebx and the ctor argument register | tried: supervised branch after the switch (fixed the layout), per-case returns | next: lane gap: register colouring
- 00962c10 CAIStateGroup_VersusRockTroll::IsRunnable DIFFER(323v313): frame packing (key, def holder and temporary share one 8-byte region in retail) + ebx/edi roles | tried: hand-released def local (fixed the release), scoped holder, tracker accessor | next: lane gap: slot packing
- 0094d680 CAIStateGroup_TrollBase::GetEnemyPredictedPos DIFFER(249v251): final sum computed z,y,x (retail x,y,z + fxch st(2)), def copy in esi (retail ebx), lead-time store scheduling | tried: member-wise/aggregate/ctor operators, named result, CDefReleaser guard | next: the operator+ form for the return value
- 00969e70 CAIStateGroup_SleepOnFloor::IsRunnable DIFFER(324v311): float box / filter construction order and the zero register | tried: inline box, named x/y | next: author the box as a named local and the filter chain like 0x0094D9C0

## Skipped
None (no row covers two functions; 00938280's row ends with its own 24-byte jump table).

## Label corrections
- 0x00661CC0 is a static float getter (`fld [g] ; ret`, no `this`), called through an object expression whose result is
  discarded; manifest "CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance" is propagated.
- 0x006A4D00 (`mov eax,[ecx+60h] ; add eax,0Ch`) is the creature's physics-position accessor, not "DoSizeof".
- 0x00658D07 is a CTCVillage state test used by the guard follower (manifest "CTCVillage::IsCurfewTime").
- 0x0090C780 is CVillage::SetGuardFollower-like (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated);
  0x0087C390 / 0x006CBE30 likewise ("AssignCreatureToTask" propagated onto a fire-point SetUser / navigation
  SetThingToIgnore).
- 0x007FFDB0 is the pub component's RemoveCustomer(CThing*) (manifest "CAIState_RemoveEquippedCombatAbilities").
- 0x00662410 is a creature combatant test (manifest "C3DAnimationBlendState::AddCombinationBlend"); 0x00665260 a
  look/visibility test (manifest "CVertexBufferWin32::ComputeAngleBetweenPositions").
- 0x00744430 (crowd component test), 0x007446A0 (perceived thing getter), 0x00487DC0 / 0x0049D9D0 (player / hero)
  carry propagated labels.
- 0x00936950 / 0x00969FB0 / 0x0092C260 / 0x009642D0 / 0x00964EF0 search-tool instantiations carry labels from other
  template arguments; 0x00A55A30 is a float-box to C2DBoxI conversion (manifest "CEditWorldMap::GetPenArea").
