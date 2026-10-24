# Hand chunk 398 (_global .. _global): 33 of 40 byte-exact

All 40 catalog rows are `_global` with descriptive or propagated labels. The real owners are CTCDHeroSuit,
CTCGatherPointBase, CTCHaste, CTCIdleScheduler, CTCTargetingPlayer and NPlayerTargeting helpers, plus the STLport 4.x
algorithm instantiations that sort and filter player-targeting candidates: CTargetPoint, pair<C3DVector,long> and
CIntelligentPointer<CThing>. Every matched row passes its behaviour test under g++ and under VC7.1 (tall.sh and
tall.py, 33/33).

## What matched (33)

**Thing components**
- 007ff7d0 CTCDHeroSuit::Create creates a level-persistent hero-suit thing for player 4 on map 0, activates it, and
  returns 0 when the thing is killed or creation fails. It is the landed CTCDExperienceOrb::Create 0x00779E90 form
  without the map lookup: a `result` local preloaded with 0 serves the failure paths, and the success path returns the
  thing register.
- 007ff8a0 CTCDHeroSuit::OnCreate looks up GetSubDef<CHeroSuitDef> (0x5935C4) through the owner's def and push_backs
  each named suit part as (part, false). This needed STLport's explicit memberwise pair copy ctor plus a same-TU
  `auto_inline(off)` stand-in body of `_M_insert_overflow`, so that LTCG's callee knowledge stores the `false` byte
  once above the loop. Took 4 grades.
- 007ffdb0 CTCGatherPointBase::RemoveAsUser is `it = remove(begin, end, ptr); Users.erase(it);` written as **separate
  statements**. Both empty tags then share the dead `user` argument slot; nesting the calls as `erase(remove(..))`
  adds `sub esp,0Ch`.
- 00800020 CTCHaste::UpdateActionSpeedMultiplier computes 1.0 times every multiplier in the map. end() is re-read
  every pass because the float store may alias it, and STLport's `_M_increment` is inlined.
- 008018b0 CTCIdleScheduler::Clear zeroes both scalars first (they share one zero register), then runs STLport
  `clear()` inline on both maps, re-reading the header for every store.
- 008025d0 CTCTargetingPlayer::GetFOVToBeInToBeTargetable has two complete `return def->x * K;` arms. Each arm loads
  the def into a different register.
- 00802890 (anonymous) GetTargetingRange looks the type up in the def's TargetingRanges map and returns the range, or
  0.0f when the type is missing. The key is homed in a frame word because its address is passed to find (0x7F9830).
- 00809270 CTargetPointFilter_IsStealableStockItem::operator() returns true when the point's thing has a stock-item
  component in a shop that is stealable. Retail's `mov al,1` / `xor al,al` tail comes from `return true` nested in
  ifs with one shared `return false`; an early `return false` or `return a && b` does not give it.

**STLport vector<pair<C2DVector,unsigned char>> (route points, 9-byte pack(1))**
- 00800380 `_M_allocate_and_copy` is `allocate(n)` (malloc) followed by `__uninitialized_copy` with a null-tested
  placement new per element.
- 00800ca0 `reserve` is the STLport 4.x reserve. capacity() and size() divide by 9 with a reciprocal multiply, and
  `_M_clear` reduces to `if (p) free(p)`.

**STLport over NPlayerTargeting::CTargetPoint (0x1c) with CThingFilter_IsInFOV and CThingCompare_BestForTargeting2**
- 008060c0 `__find_if` is unrolled 4x, with a `switch` over 3/2/1 for the remainder.
- 00806640 `find_if` forwards to `__find_if` with a tag. The callee body must be compiled noinline in the same TU so
  the tag is homed on a live argument slot.
- 00806cf0 `remove_copy_if<IsInFOV>` and 008078a0 `remove_if<IsInFOV>` (find_if, then remove_copy_if) copy points
  through CTargetPoint's implicit operator=: SetPItem(GetPItem()), then a block copy.
- 0080a850 `remove_copy_if<CFilter_Bind2<IsInFOV, unary_negate<IsTargetableBehind>>>` evaluates `(*F1)(t) &&
  (*F2)(t)`. F2 is the unary_negate itself, used as `this` (0x8091A0).
- 00808470 is the vector<CTargetPoint> copy ctor through `_Vector_base(n, alloc)` with an alloc_proxy. The three zero
  stores share `mov eax,0` (the flags are still live), and the tag is homed in the `x` slot.
- 0080a1b0 `__unguarded_partition` uses the out-of-line swap<CTargetPoint> at 0x8073D0. 0080a340
  `__unguarded_linear_insert` is the plain STLport loop.
- 0080b050 `__insertion_sort` copies the comparator inline at its single use and calls `__linear_insert` (0x80AA60).
- 0080b430 `__pop_heap_aux` calls `__pop_heap` at 0x80B210. 0080b810 `make_heap` calls `__make_heap` at 0x80B330.
- 0080b5a0 `__final_insertion_sort` uses the comparator copy three times, so it goes through the out-of-line copy ctor
  0x7F9730. The copy common to both arms is hoisted above the branch.

**STLport over CIntelligentPointer<CThing> with CDoNotUseBinaryPredicate (coin flip)**
- 00807370 `swap` uses a temporary: base ctor, vtable store, SetPItem; a direct base dtor destroys it.
- 00808230 `__make_heap` and 00808950 `__unguarded_partition` are the plain STLport bodies.
- 00809700 `sort_heap` inlines pop_heap. The 8-byte value temporary inside the loop produces `and esp,-8` with no
  extra source needed.
- 0080af70 `__introsort_loop` calls the median at 0x805FE0, 0x808950, `__partial_sort` at 0x80A240, and itself.

**STLport over pair<C3DVector,long> with CCompareTargetedPositionsForFirstPersonTargeting (0x2c)**
- 00808120 `__unguarded_insertion_sort_aux`, 00808cf0 `__make_heap` and 008097d0 `make_heap` are front ends and
  loops. The loop rows software-pipeline their argument copies just as retail does.
- 00809e40 `__final_insertion_sort` (threshold 16) hoists the comparator copy common to both arms.
- 0080b1a0 `partial_sort` → `__partial_sort` 0x80AC10 copies the comparator memberwise into its slot first: ten
  dwords, then the byte through cl.
- 0080b900 `sort` → `__introsort_loop` 0x80B620 with `__lg` as a shift loop, then 0x809E40.

## Parked for Fable 5.1 (7)
- 00802110 map<EIdleStateGroup,CAIStateGroupBase*>::operator[] DIFFER(115v115): the pair's zero `_Tp()` word is stored before the hidden-pointer `push edx`; retail stores it after that push, just before the insert call | tried: key_comp() functor, pair<const K,T> value_type, same-TU insert_unique stand-in | next: lane gap: scheduling
- 00802d10 NPlayerTargeting::IsThingInTargetingRangeForTargetType DIFFER(231v231): the GetSubDef<CTargetingDef> holder loads into esi before the two GetPosition calls (retail: `mov esi,[esp+18h] ; mov edx,esi` after the delta X store), so the thing position goes to edi instead of esi | tried: named raw holder `targetingDef = def.Object` read before the delta (kept), the same read after the delta (loads late but moves the thing pointer to esi) | next: drop the named raw pointer and pass `*def` (CDefPointer operator*) straight to GetTargetingRangeForTargetType, with the release through `def`'s dtor; otherwise lane gap: register allocation
- 00806c80 _STL::__uninitialized_fill_n<CTargetPoint*> DIFFER(98v98): retail keeps cur in ebx and the value reference in edi; ours swaps them, with the instructions otherwise identical | tried: STLport `__cur` copy, iterating `__first` directly, CTargetPoint's implicit copy ctor | next: lane gap: register allocation
- 00807170 _STL::__median<pair<C3DVector,long>> DIFFER(216v216): in the two compares that first touch c, retail does `mov ebx,[esp+14h]` between the first GetCompareValueForPoint call and the `fstp` of its result; ours does it after the fstp | tried: nested CVectorCompare operator(), one named compare value, two named compare values | next: lane gap: scheduling (RTM/QFE/G7 same; family with 00807250)
- 00807250 _STL::__unguarded_partition<pair<C3DVector,long>> DIFFER(199v199): retail forms the pivot address `lea eax,[esp+24h]` between the first GetCompareValueForPoint call and the `fstp` of its result; ours does it after the fstp | tried: plain expression, two named float values, reversed `<` form, /G7, QFE toolset | next: lane gap: scheduling (same family as 00807170)
- 00807a90 _STL::remove_copy_if<CFilter_Bind3<IsInFOV,FurtherFromPosThan,unary_negate<IsLockedTarget>>> DIFFER(214v222): retail reads PLockedTarget before the GetPItem call and parks it in the dead `result` slot (`mov [esp+18h],ecx ; call ; cmp eax,[esp+18h]`); ours calls first and then does `cmp [ecx],eax` through F3 | tried: plain `PLockedTarget == t.PThing.GetPItem()`, a named local read first (217v222: colored into ebp, result spilled), a pointer operator== | next: put the read in IsLockedTarget::operator() as `CThing* locked = PLockedTarget;` with a short live range that ends at the compare, or add an LTCG callee-knowledge stand-in for GetPItem
- 00808aa0 _STL::__linear_insert<pair<C3DVector,long>> DIFFER(229v229): scheduling only, in the else arm. Retail software-pipelines the comparator and value copies into the argument area (two loads hoisted above `sub esp,2Ch`, each load one store ahead); ours does each load and store in order | tried: two named compare values (this fixed the FP compare form), the __unguarded_linear_insert body in the same TU | next: lane gap: scheduling (the loop rows 0x808120/0x808CF0 pipeline and match; it may need retail's exact callee inlining state)

## Skipped
None. No mis-cut bounds: every row ends at its `ret`, followed only by int3 padding. No int3-after-noreturn, no
jump tables, no fistp rows.

## Label corrections
Catalog label → actual. Names from Ego_r.pdb (by donor) or the STLport 4.x source. "Descriptive name" means there is
no PDB counterpart for the exact STLport instantiation, because the donor build is Dinkumware.
- 007ff7d0 `Create` → CTCDHeroSuit::Create (PDB)
- 007ff8a0 `TreeTraverse_AbilityDefPopulator` → CTCDHeroSuit::OnCreate (PDB). 0x5935C4 is GetSubDef<CHeroSuitDef>.
- 007ffdb0 `CAIState_RemoveEquippedCombatAbilities` → CTCGatherPointBase::RemoveAsUser (PDB)
- 00800020 `CAnimation_RecalculateScale` → CTCHaste::UpdateActionSpeedMultiplier (PDB). The landed caller 0x58148D
  calls it RecalculateMultiplier.
- 00800380 `std_vector_Allocate_9ByteElements` → vector<pair<C2DVector,uchar>>::_M_allocate_and_copy (STLport,
  descriptive name)
- 00800ca0 `std_vector_Reserve` → vector<pair<C2DVector,uchar>>::reserve
- 008018b0 `LTextBinTree_RebuildWalkerArray` → CTCIdleScheduler::Clear (PDB). 0x4DA468 and 0x4DA43B are
  `_Rb_tree::_M_erase`, not BuildTreeArray.
- 00802110 `BinaryTree_FindOrInsert` → map<EIdleStateGroup,CAIStateGroupBase*>::operator[]
- 008025d0 `GetModifierValue` → CTCTargetingPlayer::GetFOVToBeInToBeTargetable (PDB)
- 00802890 `GetTargetingRange` → `anonymous namespace'::GetTargetingRange. 0x7F9830 is map<ETargetingType,float>::find.
- 00802d10 → NPlayerTargeting::IsThingInTargetingRangeForTargetType (PDB). 0x7EA960 is GetSubDef<CTargetingDef>.
- 008060c0 `std_copy_with_Cons_val` → __find_if<CTargetPoint*,CThingFilter_IsInFOV>. 0x6A7380's propagated
  `_Cons_val` label is wrong: it is CThingFilter_IsInFOV::operator()(const CTargetPoint&).
- 00806640 `std_copy_Fill` → find_if<CTargetPoint*,CThingFilter_IsInFOV>
- 00806c80 `fill<C3DAnimationStateAlpha*,..>` → __uninitialized_fill_n<CTargetPoint*,unsigned,CTargetPoint>
- 00806cf0 `std_uninitialized_copy_n` → remove_copy_if<CTargetPoint*,CTargetPoint*,CThingFilter_IsInFOV>
- 00807170 `std_introsort_Med3_TargetPoint` → __median<pair<C3DVector,long>,CCompareTargetedPositions..>
- 00807250 `std_introsort_Partition_TargetPoint` → __unguarded_partition<pair<C3DVector,long>*,..>
- 00807370 `_Swap_adl<CIntelligentPointer<CThing>>` → STLport swap<CIntelligentPointer<CThing>>
- 008078a0 `CViewBase_InitializeRangeElements` → remove_if<CTargetPoint*,CThingFilter_IsInFOV>
- 00807a90 `std_copy_if_WithDistance` → remove_copy_if<.., CFilter_Bind3<..>>
- 00808120 `_Copy_impl<CNavigationPosition*,..>` → __unguarded_insertion_sort_aux<pair<C3DVector,long>*,..>
- 00808230 `std_make_heap_Generic` → __make_heap<CIntelligentPointer<CThing>*,CDoNotUseBinaryPredicate,..>
- 00808470 `NPlayerTargeting_AllocateCopyTargetPoints` → vector<CTargetPoint>::vector(const vector&)
- 00808950 `std_partition_Generic` → __unguarded_partition<CIntelligentPointer<CThing>*,..,CDoNotUseBinaryPredicate>
- 00808aa0 `std_insertion_sort_Generic` → __linear_insert<pair<C3DVector,long>*,..>
- 00808cf0 `std_pop_heap_WithPredicate` → __make_heap<pair<C3DVector,long>*,..>
- 00809270 `CTCStockItem_IsSpecialItem` → CTargetPointFilter_IsStealableStockItem::operator() (PDB)
- 00809700 `_Sort_heap<CIntelligentPointer<CThing const>*, fn-ptr>` → sort_heap<CIntelligentPointer<CThing>*,
  CDoNotUseBinaryPredicate>
- 008097d0 `std_pop_heap_FirstPersonTargeting` → make_heap<pair<C3DVector,long>*,..>
- 00809e40 `make_heap<CThing**,CThingCompare_BestForTargeting2>` → __final_insertion_sort<pair<C3DVector,long>*,..>
- 0080a1b0 `std_partition_WithComparison` → __unguarded_partition<CTargetPoint*,CTargetPoint,BT2>
- 0080a340 `NPlayerTargeting_InsertTargetPoint` → __unguarded_linear_insert<CTargetPoint*,CTargetPoint,BT2>
- 0080a850 `std_copy_if_WithOperator` → remove_copy_if<.., CFilter_Bind2<IsInFOV, unary_negate<IsTargetableBehind>>>.
  0x8091A0 is IsTargetableBehind::operator().
- 0080af70 `std_introsort_TargetPointRecursive` → __introsort_loop<CIntelligentPointer<CThing>*,..,CDoNotUse..>
- 0080b050 `NPlayerTargeting_InsertMultipleTargets` → __insertion_sort<CTargetPoint*,BT2>
- 0080b1a0 `std_introsort_GateToDeeperSort` → partial_sort<pair<C3DVector,long>*,..>
- 0080b430 `NPlayerTargeting_PopHeapTargetPoint` → __pop_heap_aux<CTargetPoint*,CTargetPoint,BT2>
- 0080b5a0 `IntroSort_TargetPoint_Setup` → __final_insertion_sort<CTargetPoint*,BT2>
- 0080b810 `std_sort_heap_TargetPointWrapper` → make_heap<CTargetPoint*,BT2>
- 0080b900 `std_introsort_setup` → sort<pair<C3DVector,long>*,CCompareTargetedPositionsForFirstPersonTargeting>
- Callees identified along the way: 0x8073D0 swap<CTargetPoint>, 0x80B210 __pop_heap<CTargetPoint>, 0x80B330
  __make_heap<CTargetPoint>, 0x80B130 __unguarded_insertion_sort<CTargetPoint>, 0x80AA60 __linear_insert<CTargetPoint>,
  0x80B620 __introsort_loop<pair>, 0x80AC10 __partial_sort<pair>, 0x80A240 __partial_sort<ThingPointer>, 0x805FE0
  __median<ThingPointer>, 0x7F9730 CThingCompare_BestForTargeting2 copy ctor, 0x8098B0 BT2::operator(), 0x7F9660
  CCompareTargetedPositionsForFirstPersonTargeting::GetCompareValueForPoint.
