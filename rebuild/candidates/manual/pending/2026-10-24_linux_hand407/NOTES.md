# Hand chunk 407 (CAIStateGroup_LightHomeCandle .. CAIStateGroup_ScorpionKing): 38 of 40 byte-exact

All 40 rows are `_global` catalogue rows. In practice they are four AI-state TUs' STLport sort instantiations plus eight
AI helpers. Every matched source is RELOCATION_MATCH under the lane defaults (/O2 /Oy /W3) with no pragma. Every
behaviour test passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (38)

**STLport sort of `CThing*` under the anonymous `CThingCompare_InDirectionOfAndNearestToHeuristicBitch`: 18 rows**

There are two byte-identical copies, one per TU:
- The CAIStateGroup_CreatureAdvanceInCover TU (`?A0xffab493a`; operator() at 0x00978790): 00978a80, 00978b00,
  00978c50, 00978d00, 00978da0, 009792f0, 00979370, 00979650, 00979870.
- The CAIStateGroup_CreatureTakeCover TU (`?A0xeaff0e08`; operator() at 0x0097ABF0): 0097ae60, 0097aee0, 0097b030,
  0097b0e0, 0097b180, 0097b240, 0097b2c0, 0097b320, 0097b660.

Each copy holds these steps: `__unguarded_partition`, `__unguarded_insertion_sort_aux`, `__linear_insert`,
`__make_heap`, `__insertion_sort`, `__final_insertion_sort`, `sort_heap`, `__partial_sort` and `sort`.

Shape lessons:
- The bodies are STLport 4.x and `__fastcall` (/Gr).
- C2DVector has the engine's user copy constructor, so the 16-byte comparator is built in each callee's argument
  area.
- The comparator's operator() is compiled as a same-TU stand-in under `auto_inline(off)` + `__declspec(noinline)`.
  Without it, VC7.1 loads each comparator copy one call later instead of two words ahead, and `__partial_sort` gets
  x87 copies.
- The exception is `__unguarded_insertion_sort_aux` (00978b00 / 0097aee0). There the stand-in lets VC hoist the
  loop-invariant comparator copy out of the loop, so that row keeps operator() declaration-only.

**STLport sort of `CThing*` under a raw `CThingCompare_Nearest` (the CAIStateGroup_RangedCombatUsePreferredFirePoint
TU, fire-point sort): 4 rows**

Rows: 0095cfe0 `__adjust_heap`, 0095db30 `sort_heap`, 0095de60 `__partial_sort`, 0095e010 `sort`.

- The functor's `T*` result (the nearer thing) is the predicate itself (`test eax,eax`). It is true for any non-null
  pair.
- That is a retail quirk. For ranges of 16 or fewer, `sort` only runs the insertion sort, which reverses the range;
  the test checks this.
- The template operator() (0x004A8D80) is a same-TU stand-in body, as in the family above.

**STLport sort of `CThing*` under `CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>` (the
CAIStateGroup_ScreamerPursueTarget TU): 8 rows**

Rows: 0096d590 `__unguarded_linear_insert`, 0096d5e0 `__push_heap`, 0096d660 `__median`, 0096d6f0
`__unguarded_partition`, 0096d780 `__unguarded_insertion_sort_aux`, 0096d7e0 `__adjust_heap`, 0096d980
`__insertion_sort`, 0096daf0 `__partial_sort`.

- The adapter (Ego_r.pdb: a 4-byte `Functor` pointer) is inline: `(*Functor)(a, b) == a`.
- `__insertion_sort` inlines `__linear_insert` and the memmove `copy_backward`.
- The adapter is reflexive: comp(x, x) is true for the same pointer. The tests skip self-comparisons.

**AI helpers: 8 rows**
- 00946ae0 `CAIStateGroup_LightHomeCandle::GetCandleInBuildingNeedingChanging(CThingBuilding&) const`:
  - GetInterface(0x21 = CTCThingOwner), then `find_if` (0x00946D80) over the owned-object list with
    `CIsCandleNeedingChanging(this)`.
  - Same-TU stand-ins for LowerBound (0x0040F020) and find_if let the key and find_if's hidden result share one frame
    word.
- 0094b840 `CAIStateGroup_SitInPub::GetBenchWithOnePersonSittingThere(const CThingBuilding&) const`:
  - The same shape, with the const LowerBound (0x004365B0) stand-in and the empty `CIsBenchWithOnePerson` predicate.
- 0094bf60 `CAIStateGroup_SellToBuyer::RefuseCustom()`:
  - AddMessage(0x10, 1000, zero position, creature, buyer).
  - The creature, the zero C3DVector and the message manager are named locals. The manager is fetched before the
    arguments are evaluated.
- 0095f830 `CAIStateGroup_OpinionReactionRoot::SetGreetedThingReactingTo()`:
  - Sets InteractedFlag and GreetedFlag on the 0x4F component when ReactingToPlayer is set. GetInterface is unchecked,
    so both flag stores are tail-duplicated.
  - The PDB's packed members are byte arrays, so no `#pragma pack` is needed.
- 00961ab0 anonymous `GetPredictedTargetPosForThing(const CThing&, const CThing&)`:
  - Extrapolates the target's position by its last-frame movement times the snow-troll def's prediction time (default
    2.0).
  - `position += velocity * time; return position;` is what makes the result reuse the position's frame slot.
- 00961b90 anonymous `IsThingInRangeOfPound(const CThing&, const CThing&)`:
  - The XY distance to the predicted position is compared with DistancePoundGroundRange (default 6.0).
  - The predicted position is a named local. The callee is only declared, so the call stays out of line.
- 00979910 anonymous `IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase&)`:
  - Returns true for an attacker. Otherwise it walks the attacked creature's attacker map and returns true if any
    attacker's combatant info has `UsingCombatWheel` set.
  - The retail CCombatantInfo layout is used (Attackers header at +0x34). The map increment (0x006EF790) is out of
    line.
- 0097c580 `CAIStateGroup_ScorpionKing::SpawnBabies()`:
  - `AIAction_PerformActionAndWait(&CCreatureAction_ScorpionSummon(*creature, *PEnemy.GetPItem()), false)`, where the
    0x114-byte temporary is destroyed straight after the call. Then `if (!IsStillRunnable()) Terminate();`.

The three anonymous-namespace rows keep VC7.1 emitting the internal function through a `_Use` caller placed before the
definition. Their leaves match by prefix (`?Name@`).

## Parked for Fable 5.1 (2)
- 00954220 CAIStateGroup_MazeRangedCombat::IsStillRunnable DIFFER(140v142): retail keeps `this` in edi, &PTarget in esi and the target in ebx (`push ebx` shrink-wrapped around the GetPosition pair, edi reused for the creature); ours keeps `this` in esi and &PTarget in edi, never uses ebx, and is 2 bytes shorter | tried: base [kept], self/target declaration order swapped, the GetPosition calls swapped, IS_THING_ALIVE spelt out as a direct condition | next: an inline XY-distance helper taking (target position, self position), or `CThing* target` read once before IsStopping; else lane gap: register allocation
- 0096eb40 CAIStateGroup_MeleeCombatNearby::AlertAlliesInVicinity DIFFER(208v203): retail's early exit (no 0x34 CTCPerceiveThing) jumps into the shared `mov al,1 ; pop edi ; pop ecx ; ret` tail after the shrink-wrapped `pop ebp ; pop ebx ; pop esi`; ours emits a separate 5-byte copy of that return at the end, and every other byte matches | tried: nested ifs with a named filter, managers as named locals with a by-reference filter temporary, + a same-TU const LowerBound stand-in [kept: the key and the filter now share retail's frame word], HasTC/GetTC instead of GetInterface | next: one `bool result = true` with a single return, or the perceive test in a __forceinline helper; else lane gap: tail merge into a shrink-wrapped epilogue (its byte twin 0x0093DFC0 will behave the same)

## Skipped
None. No catalogue row holds two functions. 0097b320's 208-byte row is the 194-byte `__partial_sort` plus int3
padding, and every other row ends at its own `ret` (plus padding).

## Label corrections
Catalogue rows (catalogue name -> real name):
- 00946ae0 `CAIStateGroup_LightHomeCandle_FindAndValidateTask` -> CAIStateGroup_LightHomeCandle::GetCandleInBuildingNeedingChanging (Ego_r.pdb, const, private).
- 0094b840 `CAIStateGroup_SitInPub_FindAndValidateTask` -> CAIStateGroup_SitInPub::GetBenchWithOnePersonSittingThere (Ego_r.pdb).
- 0094bf60 `make_pair<C3DVector_const_,int>` -> CAIStateGroup_SellToBuyer::RefuseCustom.
- 00954220 `AIState_CheckCanReach` -> CAIStateGroup_MazeRangedCombat::IsStillRunnable.
- 0095f830 `CThing_UpdateSkillState` -> CAIStateGroup_OpinionReactionRoot::SetGreetedThingReactingTo.
- 0096eb40 `Combat_ProcessMeleeAttackMessage` -> CAIStateGroup_MeleeCombatNearby::AlertAlliesInVicinity (bool, private).
- 0097c580 `CAIStateGroup_ScorpionKing_ExecuteBriarRoseAbility` -> CAIStateGroup_ScorpionKing::SpawnBabies.
- STLport sort rows. Every name below means `_STL::<name>` of that row's comparator family:
  - `__unguarded_partition`: 00978a80 / 0097ae60 `std_sort_Partition_HeuristicBitch[_Variant]`, 0096d6f0 `std_sort_Partition_WithPivot`.
  - `__unguarded_insertion_sort_aux`: 00978b00 / 0097aee0 `std_sort_InsertionSort_HeuristicBitch[_Variant]`, 0096d780 `std_sort_InsertionSort_Alternate`.
  - `__linear_insert`: 00978c50 / 0097b030 `std_sort_InsertionPoint_HeuristicBitch[_Variant]`.
  - `__make_heap`: 00978d00 / 0097b0e0 `std_make_heap_helper_HeuristicBitch[_Variant]`.
  - `__insertion_sort`: 00978da0 / 0097b180 `std_for_each`, 0096d980 `std_sort_DownHeap`.
  - `__final_insertion_sort`: 009792f0 / 0097b240 `make_heap<CThing**,CThingCompare_BestForTargeting2>`.
  - `sort_heap` (with the real element type and comparator): 00979370 / 0097b2c0 / 0095db30 `_Sort_heap<long*>`.
  - `__partial_sort`: 00979650 / 0097b320 `std_sort_HeapSortFinalize_HeuristicBitch[_Variant]`, 0095de60 `std_sort_HeapSortFinalize`, 0096daf0 `std_sort_HeapSortFinalize_Adapted`.
  - `sort`: 00979870 / 0097b660 `std_sort_Introsort_HeuristicBitch[_Variant]`, 0095e010 `std_sort_Introsort`.
  - `__unguarded_linear_insert`: 0096d590 `std_sort_SiftUp`.
  - `__median`: 0096d660 `std_sort_Median3`.
- Correct apart from the STLport spelling: 0095cfe0, 0096d5e0, 0096d7e0, 00961ab0, 00961b90, 00979910.

Callee labels seen while authoring:
- 0x008FCFC0 `CPlayer::GetCurrentMode` -> CAIStateGroupBase::GetPWorld.
- 0x008FCFA0 `GetCurrentMode` -> its byte-identical twin GetPThingWorld.
- 0x004C73D0 `CVertexBufferWin32::DoSizeof` -> CThing::GetPosition.
- 0x006A4D00 `DoSizeof` -> the creature's GetPosition copy (`m_pPhysics + 0xc`).
- 0x007446A0 `_Cons_val<...>` -> CTCPerceiveThing::GetPerceivedThing.
- 0x00662410 / 0x00662430 `C3DAnimationBlendState::AddCombinationBlend` -> CThingCreatureBase::IsInMeleeAsAttacker /
  IsInMeleeAsAttacked. They call the combat manager's 0x006E92C0 (combatant Attacked != 0) and 0x006E9270 (attacker
  count != 0).
- 0x006E9360 `CSurface::GetHeight` -> CCombatManager::GetCombatantInfo.
- 0x008E46D0 `CCreatureAction_BriarRoseShotFarInto` -> the CCreatureAction_ScorpionSummon constructor.
- 0x0062E900 `~CTCLightningOrb` -> ~CCombatActionBase.
- 0x0097B870 `CAIStateGroup_BriarRose::IsStillRunnable` -> CAIStateGroup_ScorpionKing::IsStillRunnable.
- 0x00979760 / 0x0097B4E0 / 0x0095DF40 `std_sort_QuickSort` -> `__introsort_loop`.
- 0x009789B0 / 0x0097ADA0 `std_sort_SiftUp_HeuristicBitch[_Variant]` -> `__unguarded_linear_insert`.
- 0x00978920 `std_sort_Median3_HeuristicBitch` -> `__median`.
- 0x0096D900 / 0x0095D880 `std_make_heap_helper[_variant]` -> `__make_heap`.
- 0x0096DAA0 `_Sort_heap<long*>` -> sort_heap.
- 0x0095DDF0 `make_heap<...BestForTargeting2>` -> `__final_insertion_sort`.
- 0x0095CDF0 `_Push_heap<float*,...>` -> `__push_heap<CThing**, ..., CThingCompare_Nearest>`.
- 0x00946D80 / 0x0094BA90 are the two find_if instantiations. 0x00946C60 / 0x0094B9B0 are
  CIsCandleNeedingChanging::operator() and CIsBenchWithOnePerson::operator() (catalogued as `_Find_if<...>`).
- 0x0093DFC0 `CAIStateGroup_MeleeCombatNearby2_AddCombatMessage` -> CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity
  (the byte twin of 0x0096EB40; only its delay global differs).
