# Hand chunk 479 (CTCPhysicsRigidBody .. CApplyAdditionalExplosiveImpulseIfInsideRing): 24 of 40 byte-exact

All 24 src/ rows grade MATCH (2) or RELOCATION_MATCH (22) through gall.sh (grades.txt). Their 24 tests PASS under g++
(tall.sh) and VC7.1 (tall.py, probe/tall_vc.txt). calls.py shows no import mismatch (malloc 0xBFEA0E, free 0xBFEA14,
memmove 0xBFEAE6 where retail calls them). STLport code is written in namespace `std`.

## What matched (24)

- **STLport find / find_if** (007c2980, 007f7c10, 00809f80, 0080a910): the 4-way unrolled `__find` / `__find_if`
  verbatim (`(last - first) >> 2` trip count, then a fall-through switch over 3/2/1) as `__fastcall` templates.
  - 007c2980: `__find<CIntelligentPointer<const CThing>*>`. The engine `operator==` resolves the element before the value.
  - 007f7c10: `__find_if<CThing**, CIsTriggerActivated>` around the landed 0x007F7E70 predicate body.
    - CTCMap::LowerBound (0x004365B0) has its real inline body, using `len / 2` halving. VC7.1's per-function inline
      budget expands it at the first test only and calls it at the other six, which is exactly retail.
    - The component pick has to be the one-ternary `((it == last || type < it->kind) ? last : it)->tc`. The
      if/assign form copies the inlined result into eax.
  - 00809f80: the stealable-stock filter is plain inline, and the budget expands it once.
  - 0080a910: `Bind2<IsInFOV, unary_negate<IsTargetableBehind>>`.
- **STLport sorts over targeting candidates** (007fa450, 007faa20, 007fac80, 0080a240, 0080ac10, 0080b330, 0080b620,
  0080bab0, 0080bd30). These are `__insertion_sort`, `__partial_sort`, `__introsort_loop` and `__make_heap`, verbatim.
  - By-value comparators are copied memberwise.
  - 0080bab0 and 007fa450 only matched once the comparator's or scorer's out-of-line callees had same-TU noinline bodies:
    - In 0080bab0, VC then caches `Hero` in esi.
    - In 007fa450, VC then hoists the inlined `__linear_insert` comparator copy out of the loop.
  - 0080ac10 needed pure same-TU scorer bodies (the GetCompareValue stand-in clobbers ecx).
  - `__median` is inline in 0080bd30 but out of line (0x7F9F60) in 007fac80.
- **STLport vector members** (007d23b0, 007d24f0, 007dc3f0, 00800da0, 008092d0): `_M_insert_overflow`,
  `_M_fill_insert` and `operator=`, verbatim.
  - The frame layout depends on the helpers. Same-TU `auto_inline(off)` + noinline template bodies of the out-of-line
    helpers let the temporaries share the dead argument slots.
  - 00800da0's `__uninitialized_fill_n` needs `__forceinline`.
- **STLport `__copy` / `__copy_backward` over CShoveHaPennyRegionDef** (007d14b0, 007d17a0): the counted loop with the
  def's implicit assignment inline. VC7.1 unrolls it by four.
- **CTCWillResponse::SetThingIs{ForcePushable, Lightningable, EpicSpellable}** (007e3d50, 007e3f40, 007e4330): the landed
  sibling body with the polarity inverted (default-true flags): get-or-add on clear, and Cleanup on set.
- **CApplyAdditionalExplosiveImpulseIfInsideSmearedLine::operator()** (0080da50): the line-out vector lives in a named
  local. Z = 0 is folded into the normalise, and `direction *= 1.0f / distance` goes through an inline `operator*=` (the
  vector stays in memory). The impulse is `dir * (Radius - distance) * 0.25f`.

## Parked for Fable 5.1 (7)

- 007bebe0 NThingShot::RandomiseShotDirection DIFFER(274v288): retail stores each component of the inline cross product
  `dir x (Y, -X, 0)` to a temporary in turn (`fstp [t] ; mov reg,[t] ; mov [axis],reg`); ours forwards X/Y from the x87
  stack and sends only Z through the temporary | tried: `return C3DVector(...)`, named-local result, member stores,
  inline SetCrossProduct (all identical) | next: `(float)(double)(...)` around each component (the landed 0x007A1870
  CrossProduct) to force the in-order rounding stores
- 007d9d70 std::vector<KnickKnack>::_M_fill_insert DIFFER(316v316): position/elems_after in ebx/ebp swapped against
  retail (ebp=position, ebx=elems_after), and the first branch spills `old_finish - n` into the dead position slot
  instead of the dead x slot | tried: verbatim STLport, cursor-local vs in-place `__uninitialized_fill_n` loop,
  C3DVector user copy constructor | next: same-TU noinline bodies of the out-of-line helpers (as 007d24f0); else lane
  gap: register allocation
- 00807c30 std::__find_if<CTargetPoint*, CFilter_Bind3<...>> DIFFER(659v749): inline budget. Retail expands the
  FurtherFromPosThan range test (0x00805920) at 6 of the 7 predicate sites; ours expands it at 4 (tests 3 and 4 and
  switch case 3 call it), which also swaps ebx/ebp | tried: free `operator==` template for the locked-target compare
  (fixed the load order), `__forceinline` on the C3DVector helpers and on Bind3::operator() | next: shrink the inlined
  callees' cost (same-TU noinline GetLengthSquared or IsInFOV bodies) so the budget reaches the 6th site; else lane gap:
  inline budget
- 00807f20 std::__find_if<CTargetPoint*, CTargetPointIsNotEnemyOfThing> DIFFER(476v480): retail keeps the dead-flag mask
  in bl (`mov bl,1 ; test byte [eax+91h],bl` in all seven inlined tests) and spills `last` to a `push ecx` word | tried:
  `Dead:1` bitfield with `!Dead`, the landed IS_THING_ALIVE, an if-form, a negated `||` chain | next: the landed 0x807B70
  predicate spelling (GetPItem returning CThing*, an `&& !Dead &&` chain) or a `bool` bitfield
- 0080db70 CApplyAdditionalExplosiveImpulseIfInsideRing::operator() DIFFER(275v275): one x87 operand order, the impulse
  Y * (OuterRadius - distance) (`fld st(1) ; fmul [esp+0Ch]` vs retail `fld [esp+0Ch] ; fmul st(2)`) | tried: inline
  `*=` (dead registers kept alive), per-component in-body normalise (fixed liveness), two operand spellings | next:
  `direction = physics->Position - BasePosition; direction.Z = 0.0f; direction *= 1.0f / distance;`, the operator*= form
  that matched the sibling 0080da50
- 007fe990 CTCSingleUserActionPoints::OnCreate DIFFER(351v351): register swap only (retail this=esi with the
  memmove sizes, the find end and the destroy bound in edi; ours the reverse) | tried: plain body, the index in a nested
  block (fixed the frame: the PThing temporary and the index share a slot), an inline / `__forceinline` base
  ResetActionPoints() | next: reorder the clears or spell the loop bound differently to move VC7.1's register priority;
  else lane gap: register allocation
- 007ff2f0 CTCNamedUserActionPoints::OnCreate DIFFER(358v360): the same this=esi/edi swap, plus retail's begin copy
  through ecx (`mov ecx,[names] ; mov edi,ecx`) and one shared tag byte (both push_back overflow tags at +0x13; ours puts
  the `__false_type` at +0x12) | tried: 1 grade (the 007fe990 model) | next: whatever fixes 007fe990; the tag slot may
  follow

## Skipped

Not attempted (budget). STATUS.tsv `SKIP -v<retail>`. All are 400-940-byte bodies with heavy x87, def-pointer or nested
vector code:

- 007bc38a: CTCPhysicsRigidBody force row; vtable 0x0123CF20.
  - It starts at an unaligned address straight after the previous function's `leave ; ret`, and both use ebp frames.
  - That suggests a size-optimised or non-/Gy TU: try `#pragma optimize("s", on)` first.
- 007bf000: NThingShot::GetCreatureFirePosAndOrientation.
- 007c0ad0: NThingShot::CreateShot. The PDB has two overloads; pick by the 944-byte body's arguments.
- 007cb8b0: a persistence Transfer over vector<CCharString>.
- 007db2e0: CTCTavernGameCardBase::NewGame.
- 007ea580: a vector<pair<C3DVector,long>> push_back loop. The catalog says CTCTargeted::BuildTargetTrajectory.
- 007eff80: CTCCarriedActionUseFishingRod::IsThingAbleToFish, using the fast inverse sqrt table 0x013CE558.
- 008028d0: anonymous GetTargetingRangeForTargetType.
- 00802b60: anonymous GetTargetingRangeForTarget.

No row in the chunk covers two functions.

## Label corrections

- 007f7c10 "Entity_FindHidden": std::__find_if<CThing**, CIsTriggerActivated> (callers 0x007F78F6/0x007F7EDA,
  CTCActivationReceptorBase).
- 00809f80 "std_copy_if_IsSpecialItem", 0080a910 "std_copy_if_WithMultiplePredicates", 00807c30
  "std_remove_if_ByPredicate": STLport `std::__find_if` instantiations (the find half of remove_if); nothing is copied or
  removed.
- 007fe990 "CTCGraphicAppearance_InitDummyObjects" / 007ff2f0 "..WithNames": CTCSingleUserActionPoints::OnCreate and
  CTCNamedUserActionPoints::OnCreate (vtable 0x01241EAC / 0x01241F24 slot 4; Ego_r.pdb names).
- 007bebe0: NThingShot::RandomiseShotDirection (`__fastcall`, PDB `?RandomiseShotDirection@NThingShot@@YI...`).
- 007e3f40 / 007e4330, both catalogued "SetThingIsAbleToBeLifted": CTCWillResponse::SetThingIsLightningable /
  SetThingIsEpicSpellable.
- 007d14b0 "memcpy_Entity_48byte_Blocks" / 007d17a0 "memmove_Entity_48byte_Backward": std::__copy / std::__copy_backward
  over CShoveHaPennyRegionDef.
- 007d23b0 / 007d24f0 / 007d9d70 / 00800da0 / 008092d0 (the "vector_*" labels): STLport vector `_M_insert_overflow` /
  `_M_fill_insert`, element types as on each line 1. 008092d0 "Vector_Insert_Range" is `_M_insert_overflow`.
- 0080b330 "std_pop_heap_TargetPointCustom": std::__make_heap.
- 0080a240 "std_introsort_TargetPointHeapsort": std::__partial_sort<CIntelligentPointer<CThing>*, CDoNotUseBinaryPredicate>.
- 0080ac10 "std_sort_ContextItems": std::__partial_sort<pair<C3DVector,long>*, CCompareTargetedPositionsForFirstPersonTargeting>.
- 0080b620: std::__introsort_loop over the same pair type.
- 0x007FA610 (callee) is `__unguarded_insertion_sort`, not `_Insertion_sort`.
- 0080da50 / 0080db70 (the "CTCPhysicsBase_ApplyExplosiveImpulseFrom*" labels):
  CApplyAdditionalExplosiveImpulseIfInsideSmearedLine::operator() / CApplyAdditionalExplosiveImpulseIfInsideRing::operator().
- Several catalog names in the chunk are descriptive (no PDB name of that spelling). The real names above come from
  the PDB name sets, vtables and callers.
