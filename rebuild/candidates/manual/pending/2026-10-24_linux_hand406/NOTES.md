# Hand chunk 406 (NBalverineCombat .. CIsCookingPot): 31 of 40 byte-exact

## What matched (31)
- **NBalverineCombat** -- 0x00905550 `CanLungeThisFrame`: the CBalverineBattleDef `SecondsBetweenLunges` (default 5 s) times the
  frame rate against the frames since the last lunge. A same-TU `auto_inline(off)` GetSubDef stand-in lets VC reuse the def
  pointer's frame slot. The fps global is read into a `long` before the GFGetCurrentFrame call.
- **anonymous namespace (balverine-combat TU)** -- 0x00905600 `GetPRangedWeaponFromCreature` (__fastcall): this is the twin of
  the landed 0x006A1620. It uses PeekTC<T> twice plus IsThingAlive and returns the item. A caller wrapper keeps the
  file-local function emitted.
- **LSocketServer** -- 0x0090AEC0 ctor: the STLport list and inline LAddress ctors come first, then the three scalar members
  are stored in the body, then `ListOfSocketContainers.push_back(this)`. 0x0090AC50 `GetAcceptedConnectionsOnSocket` uses the
  `iterator it = begin(); if (it == end())` form; `empty()` gives `cmp [eax],eax`.
- **STLport deque<CBezierCurve>** -- 0x009137F0 `_Deque_base::_M_initialize_map`, 0x00913930 `__destroy_aux` (/Gr, the
  empty tag in ecx), 0x00913A20 `clear` and 0x00913E70 `push_back` all match the STLport 4.6 source verbatim, with
  buffer = 128 / 48.
- **STLport sort of CThing\*\* under CThingCompare_Nearest** -- 0x0091FA00 `__median` (nested if/else returning `const&`),
  0x00920160 `sort_heap` and 0x009206A0 `sort` (the landed 0x007A3B60 / 0x007A4260 forms).
- **STLport vector<C3DVector>** -- 0x00920FF0 `reserve` and 0x00920F90 `_M_allocate_and_copy` match verbatim
  (_Vector_base/_STLP_alloc_proxy, allocate = `n != 0 ? malloc : 0`, the placement-new null test). The body is
  COMDAT-folded for every 12-byte element type.
- **CAIStateGroup_MinionFollowThing** -- 0x009215E0 `SetThingToNotAvoid`: the bool out-parameter GetInterface inside the `if`
  stores the component to a local (`mov eax,[eax+4] ... mov ecx,eax`). The FindEntry form folds it into ecx instead.
- **STLport sort of CIntelligentPointer<const CThing> under a `bool (__fastcall*)(ptr, ptr)` comparator** (9 matched; the
  __push_heap row is parked):
  - 0x009265C0 `__unguarded_linear_insert`
  - 0x00926760 `__copy_backward`
  - 0x009267F0 `swap`
  - 0x009274B0 `__make_heap`
  - 0x00927740 `__linear_insert`: copy_backward is out of line in the TU, so its tag temporary is homed over the live
    comparator slot.
  - 0x00927810 `sort_heap` (`and esp,-8`)
  - 0x009278C0 `__insertion_sort`
  - 0x00927AA0 `__introsort_loop`

  All of these are STLport 4.6 bodies on the landed CIntelligentPointer<CThing> twin model (the 0x0080xxxx shuffle). The
  comparator pointer is a /Gr register candidate (edx).
- **STLport sort of CThing\*\* under a `bool (__fastcall*)(CThing*, CThing*)` comparator** (the MinionReactToCriminal height
  sort):
  - 0x00932330 `__unguarded_linear_insert`
  - 0x00932420 `__adjust_heap`
  - 0x00932490 `__unguarded_partition`
  - 0x00932660 `__insertion_sort`: copy_backward is STLport's `__copy_trivial_backward` memmove.
  - 0x00932770 `__partial_sort`
  - 0x009327F0 `__introsort_loop`: the inline `__median` IS inlined here and selects the element address in esi.

  All six matched on the first grade.
- **CAIStateGroup_StealthSeekEnemy** -- 0x0093C7B0 `MoveTowardsEnemy`:
  - `do { if (!IsStillRunnable(*p)) { Terminate(); return; } ... } while (result == 4); if (result == 1) Terminate();`. The
    two Terminate blocks tail-merge, whereas a `while` loop is rotated.
  - `bool arrived;` is homed in the dead argument slot.
  - The C3DVector goes through a named local copy.
- **GetDummyObjectRecursive** -- 0x00942B30 (__fastcall): the appearance's GetDummyObject is tried first, then the live
  objects of CTCThingOwner are walked recursively. A same-TU `const LowerBound` stand-in makes VC7.1 pack the lookup key and
  the owner into the dead `set` argument slot.
- **anonymous CIsCookingPot** -- 0x00945DE0 `operator()(const CIntelligentPointer<CThing>&) const`: an && chain, then the
  out-parameter GetInterface, then `if (m_onlyWithFreePoint) return points->IsActionPointFree(); return true;`.

## Parked for Fable 5.1 (9)
- 0090bc60 CAIStateGroup_MinionWorkInInn::GetFreeBench DIFFER(211v211): ours stores the bench vector's 3 zero words before the ObjectList end() load (`mov eax,[eax+0Ch]`) and the `push &benches`; retail stores them after | tried: plain vector ctor, iterator without a copy ctor, STLport _Vector_base/_STLP_alloc_proxy model, end() hoisted to a named local | next: lane gap: scheduling (compiler build)
- 0090df60 CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>::operator() DIFFER(69v69): retail jumps forward (`test eax,eax ; jne`) to the wrapped-filter call placed after the shared `xor al,al` exit; ours falls through into the call | tried: nested ifs, ternary return, early-return guards, `thing && filter(thing)`, /Os /Oa /G7 /Og | next: model it as the member template defined inside the class (implicit inline) with the call through a by-reference filter member; else lane gap: block layout
- 0090dfb0 CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>::operator() DIFFER(69v69): same block layout as 0090df60 (same template) | tried: the 0090df60 forms (1 grade) | next: whatever fixes 0090df60 fixes this one
- 0091a630 CAIStateGroup_WorkBase::HasSeller DIFFER(50v51): retail `mov eax,[eax+18h] ; add eax,12Ch ; mov eax,[eax]`, ours `mov ecx,[eax+18h] ; mov eax,[ecx+12Ch]` | tried: getter chain, `T* const&` GetPObject, a named const-ref local, __forceinline getters, /Os /Oa /Og /G7 | next: an address-returning accessor (`&m_member`) dereferenced at the call site, or an array-index member access; else lane gap: address folding
- 0091fb30 std::__adjust_heap<CThing**, int, CThing*, CThingCompare_Nearest> DIFFER(135v135): retail loads comp.Pos.X and .Y (`mov ecx,[esp+20h] ; mov edx,[esp+24h]`) before `sub esp,0Ch`, ours loads Y after the first store | tried: init-list and body-assignment C3DVector copy ctors, a same-TU auto_inline(off) __push_heap body, /Os /Oa /Og /G6 /G7 | next: lane gap: scheduling
- 009204f0 std::__partial_sort<CThing**, CThing*, CThingCompare_Nearest> DIFFER(174v172): ours copies the comparator's Y/Z through the FPU (`fld [comp.Y] ; fld [comp.Z] ... fstp`), retail moves all three words through ecx/edx | tried: init-list and body-assignment C3DVector copy ctors, no pack(1), a user copy ctor on CThingCompare_Nearest, /Os /Oa /Og /G6 /G7 | next: a CThingCompare_Nearest whose Pos is copied by an implicit (bitwise) copy ctor of a C3DVector that has no user copy ctor, inside a compare with a user ctor; else lane gap: float copy
- 00926690 std::__push_heap<CIntelligentPointer<const CThing>*, int, CIntelligentPointer<const CThing>, bool (__fastcall*)(..)> DIFFER(192v198): register assignment -- retail keeps first in ebp and spills holeIndex to the `push ecx` slot (reloaded into esi at the exit); ours keeps holeIndex in ebp and spills first | tried: iterator-arithmetic form, the landed 0x00806990 `first[parent]` index form | next: lane gap: register allocation
- 00939dd0 CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded DIFFER(155v150): retail hoists the Bind2 `m_pFirst = &IsOfType` store (`lea eax,[esp+8] ; mov [esp+10h],eax`) above an unaligned loop top and stores 0x1B after the creature push; ours stores both in the (aligned) loop | tried: for(;;)+break (216 bytes, rotated), do/while (best), tail recursion, backward goto | next: build the Bind2 once before the loop (a named `CFilter_Bind2` whose second pointer is re-pointed per pass), or a same-TU FindMostRecentMessage body
- 0093dfc0 CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity DIFFER(208v203): the no-perception early exit gets its own `mov al,1 ; pop edi ; pop ecx ; ret`; retail jumps into the main epilogue's shared tail | tried: nested ifs, one && chain, #pragma optimize("s"), a bool result local | next: the same-TU const LowerBound stand-in (it changed slot packing on 0x00942B30), or the GetInterface miss written as `if (!HasTC) return true;` with the lookup outside the if

## Skipped
- None. No row holds a second function body.

## Label corrections
- 0x00905550 CanLungeThisFrame -> `NBalverineCombat::CanLungeThisFrame` (module).
- 0x00905600 -> the file-local `GetPRangedWeaponFromCreature` (anonymous namespace, __fastcall).
- 0x0090AC50 "LinkedList_PopFront" -> `LSocketServer::GetAcceptedConnectionsOnSocket`.
- 0x0090AEC0 "Manager_ConstructAndRegister_0090aec0" -> `LSocketServer::LSocketServer`.
- 0x0090BC60 "CleanBench_PickRandomMarker" -> `CAIStateGroup_MinionWorkInInn::GetFreeBench`.
- 0x0090C100 "_Find_if<..CMiniMapDisplay..>" -> `for_each<list<CIntelligentPointer<CThing>>::const_iterator, CAddFreeBenchToList>`.
- 0x0090DF60 "CThingFilter_FilterThingWithMask" and 0x0090DFB0 "CThingFilter_IsValidGossipee" -> `CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>` and `<CThingFilter_IsNavLineOfSightToThing>::operator()`.
- 0x009137F0 "CAIStateGroup_FlyingThingBase_AllocateCurveBuffer", 0x00913930 "DequeIterator_AdvanceWithWrap", 0x00913A20 "PathBuffer_ClearAndReset" and 0x00913E70 "CGPUScratchBufferTextureManager_AddResourceEntry" (propagated) -> STLport `deque<CBezierCurve>` `_M_initialize_map`, `__destroy_aux`, `clear` and `push_back`.
- Landed 0x00913590 `_Construct` is for CBezierCurve, not CResourceEntry.
- 0x0091A630 "Inventory_CountItems" -> `CAIStateGroup_WorkBase::HasSeller` (descriptive name: no PDB symbol).
- 0x0091FA00 "Sort_Partition_MedianOfThree" -> `__median<CThing*, CThingCompare_Nearest>`.
- 0x0091FB30 and 0x00932420 "_Adjust_heap<..>" (Dinkumware spelling) -> STLport `__adjust_heap`.
- 0x00920160 "_Sort_heap<long*>" -> `sort_heap<CThing**, CThingCompare_Nearest>`.
- 0x009204F0 "Sort_IntroSort" -> `__partial_sort`.
- 0x009206A0 "Sort_Init" -> `sort`.
- 0x00920480 "make_heap<..BestForTargeting2>" -> `__final_insertion_sort`.
- 0x0091F870 "_Push_heap<float*..>" -> `__push_heap<CThing**>`.
- 0x00920F90 "Vector_Reserve_3ints" -> `vector<C3DVector>::_M_allocate_and_copy`.
- 0x00920FF0 "vector_reserve" -> `vector<C3DVector>::reserve`.
- 0x009215E0 "CAIStateGroup_MinionFollowThing_AssignTask" -> `CAIStateGroup_MinionFollowThing::SetThingToNotAvoid`.
- 0x009265C0 "_Push_heap<CIntelligentPointer<CThing const>*..>" -> `__unguarded_linear_insert`.
- 0x00926690 "_Push_heap<..>" -> `__push_heap`.
- 0x00926760 "Median_GetByIndex" -> `__copy_backward`.
- 0x009267F0 "_Swap_adl<CIntelligentPointer<CThing>>" -> `swap<CIntelligentPointer<const CThing>>`.
- 0x009274B0 "Sort_MakeHeap_Init" -> `__make_heap`.
- 0x00927740 "_Med3<..>" -> `__linear_insert`.
- 0x00927810 "_Sort_heap<..>" -> `sort_heap`.
- 0x009278C0 "std_sort_Median" -> `__insertion_sort`.
- 0x00927AA0 "std_sort_QuickSort" -> `__introsort_loop`.
- Other functions in the same sort:
  - 0x00927440 = the iter_swap tail-jump
  - 0x009263F0 = `__median`
  - 0x00926850 = `__adjust_heap`
  - 0x00927590 = `__unguarded_partition`
  - 0x00927910 = `__partial_sort`
- 0x00932330 "Sort_Sift_Comparator" -> `__unguarded_linear_insert<CThing**>`.
- 0x00932490 "Sort_Partition_Predicate_Forward" -> `__unguarded_partition`.
- 0x00932660 "std_sort_DownHeap" -> `__insertion_sort`.
- 0x00932770 "Sort_IntroSort_Buildings" -> `__partial_sort`.
- 0x009327F0 "std_sort_QuickSort" -> `__introsort_loop`.
- The CThingBuilding / CIsHouseLargerThan template arguments on the 0x00932xxx rows are propagated from an Ego_r Dinkumware
  instantiation. Here the element is CThing\* and the comparator is the file-local `bool (__fastcall*)(CThing*, CThing*)`
  0x00930D90, which computes `a->GetPosition().Z < b->GetPosition().Z` (a descriptive reading).
- 0x00932540 = `__make_heap`, 0x009326D0 = `sort_heap`, 0x009323D0 = `__unguarded_insertion_sort_aux`.
- 0x00939DD0 "..._CheckAndRemove" -> `CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded`.
- 0x0093C7B0 "CAIStateGroup_StealthSeekEnemy_MoveOrQuit" -> `CAIStateGroup_StealthSeekEnemy::MoveTowardsEnemy(const CTCPerceiveThing*)`.
- 0x0093DFC0 "CAIStateGroup_MeleeCombatNearby2_AddCombatMessage" -> `bool CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity()`.
- 0x00945DE0 "CVillageTaskTorch_FindInvolvedTask" -> anonymous `CIsCookingPot::operator()` (SitForMeal TU). The member
  `m_onlyWithFreePoint` is a descriptive name.
- Callee labels:
  - 0x0049D870 = GFGetCurrentFrame
  - 0x008FCF40 "CXMVPlaybackInfo" = CAIStateGroupBase::Terminate
  - 0x007446A0 "_Cons_val<..>" = CTCPerceiveThing::GetPerceivedThing
  - 0x004C73D0 / 0x006A4D00 "DoSizeof" = CThing::GetPosition
  - 0x006CBE30 "CVillageTaskCarryCrate::AssignCreatureToTask" = CTCCreatureNavigation::SetThingToIgnore
