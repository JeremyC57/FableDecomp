# Hand chunk 389 (std::_Rb_tree .. CThingObject): 35 of 40 byte-exact

All 40 catalog rows are `_global` and propagated labels; nearly every one names a different function (see Label
corrections). 35 RELOCATION_MATCH (gall.sh), 35 tests PASS under g++ (tall.sh) and VC7.1 (tall.py). 5 parked, 0 skipped.

## What matched (35)

**Combatant maps -- `std::_Rb_tree<unsigned __int64, ...>` (STLport, CCombatManager's UID maps)**
- 006f2760, 006f27b0 `_Rb_tree::equal_range(const u64&)` (UID -> CThingCreatureBase* and UID -> CCombatantInfo trees):
  `pair<iterator,iterator>(lower_bound(k), upper_bound(k))`. VC evaluates the upper bound first, inlining its walk,
  and calls the lower bound.
- 006f2870 `_Rb_tree::_M_copy`: STLport verbatim. `_M_clone_node` = malloc + placement copy + colour; recurse right,
  walk left.
- 006f34b0 `std::for_each(map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator, ..., CGetBestAttackerForProng)`:
  the iterator's ++ must be STLport's static `_Rb_global<bool>::_M_increment(node)` returning the node.
- 006f4af0 `pair<const u64, CCombatantInfo>` scalar deleting dtor: an explicit `p->~T()` goes through `??_G` with
  flags 0, and members die in reverse.
- 006f4bc0 `_Rb_tree::erase(iterator)`, 006f4c20 `_Rb_tree::_M_erase`: STLport verbatim; the value dtor inlines.
- 006f5000 `_Rb_tree()`: `_M_header(0)` in the init list puts the zero store before the malloc of the 0x88-byte header.

**Combat rings and prongs -- `std::vector<CCombatRing>`, `CProng`**
- 006f2b30 `std::__partition<CProng*, CIsProngUsable>`: STLport's two-loop form with
  `while (true) if (first == last) return first; else if (pred(*first)) ++first; else break;`. The predicate inlines
  twice and shares `bl = 1`.
- 006f3260 `vector<pair<C3DVector,bool>>::_M_insert_overflow`: STLport verbatim. The pair keeps STLport's explicit
  copy ctor. `__uninitialized_copy` and `__uninitialized_fill_n` get same-TU noinline bodies so the empty tags share
  argument slots. Uses the landed pack(1) C3DVector precedent.
- 006f40a0, 006f4140 `__uninitialized_copy<(const) CCombatRing*>` and 006f40f0 `__uninitialized_fill_n<CCombatRing*>`:
  an STLport loop with a cursor copy, where `_Construct` is placement new of the implicit copy ctor. The unsigned
  count gives `test edx,edx ; jbe`.
- 006f43c0 `vector<CCombatRing>::vector(const vector&)`: `_Vector_base(n, a)` zero-initialises the three pointers in
  its init list, then allocates in the body.
- 006f4930 `vector<CCombatRing>::push_back`: construct at finish, else `_M_insert_overflow`. That callee is a same-TU
  noinline STAND-IN body, so the `__false_type` temporary shares x's slot.

**Animation events -- `NAnimationEvents::CManager` and its SKeyedEvents vector (SGI/STLport algorithms)**
- 006f9ac0 `CManager::GetExtraEventNames`, 006f9b50 `CManager::GetExtraEvents`: the inlined GetPExtraAnimEvents
  (`result = 0; it = Find(anim); if (it != End()) result = &it->second;`). Find copies the key into a local passed by
  reference, and VC homes that local in the dead argument slot.
- 006f9ee0 `NAnimationEvents::SaveAnimationEvents()`: one statement per manager,
  `GetXManager().Save(GetMiscDir() + CWideString(L"..."))`. The literal ctor takes `const unsigned short*` because the
  lane's VC7.1 has no built-in wchar_t.
- 006fafe0 `CSmallVector<CEvent,32>::Allocate`: Clear, new[], then a fill loop over an end-pointer local computed once.
- 006fb8a0 `__destroy_aux<SKeyedEvents*>` and 006fbb30 `__uninitialized_fill_n<SKeyedEvents*>`: the inlined
  CSmallVector dtor/copy per element.
- 006fbee0 `__unguarded_partition`, 006fbfd0 `__pop_heap`, 006fc070 `__make_heap`, 006fcb30 `__insertion_sort`,
  006fcea0 `__partial_sort`: the SGI forms verbatim. The by-value `T(*i)` / value copy is built in the call's argument
  slot.
- 006fc8e0 `for_each<const CEvent*, CFillEventNames>`: the functor inlined as
  `EventNames->push_back(map->FindFirstSymbolWithVal(event.Index)->EntryName)`. The 30-bit Index bitfield gives
  `and eax,3FFFFFFFh`. Stand-in `_M_insert_overflow`.
- 006fcc80 `CFillEvents::operator()`: a NAMED pair local `entry(name, event.Time)`, pushed back and then destroyed.
  Stand-in `_M_insert_overflow`.
- 006fcd90 `vector<SKeyedEvents>::push_back`: as 006f4930, with a stand-in `_M_insert_overflow`.
- 006fd330 `CManager` scalar deleting dtor: the class declares no dtor (compiler-generated, so no vtable reset), and
  members die in reverse. `__destroy_aux` is a same-TU body so its tag shares the flags slot.

**Camera -- `CGameCameraBase`**
- 006fd4f0 `GetRotationAngle(const C3DVector&, const C3DVector&)`: a signed rotation in turns. The `float result = 0.0f`
  local declared first preloads the 0.0. The XY-flattened target is a named C3DVector, Dot/GetLength are inline with
  `#pragma intrinsic(sqrt, acos)`, and `sign` defaults to 1.0f.

**Reaction frequency traits**
- 006fed30 `CReactionFrequencyTraits_PeriodicFrequency` ctor (vtable 0x1264B7C): the init list holds the two rates and
  FramesPerMinute (`imul ecx,ecx,3Ch`). The body converts the times to frames and zeroes LastCutoffFrame.
- 006ff930 `CReactionFrequencyTraits::CreateFromType`: a switch on 1/2/3 (a `dec ecx` chain), each case returning `new`
  of a default-constructed traits class; null otherwise.

**Quest card -- `CTCQuestCard`**
- 00702810 `IsActivatable() const`:
  - a const_iterator for loop that re-reads `PDef->Prerequisites.end()`;
  - the static `_M_increment`;
  - the hero fetched per renown prerequisite;
  - `const CTCHeroStats* stats = 0;` followed by the inline GetInterface, which is jump-threaded.

  Interface type numbers come from each class's GetInterfaceType (vtable slot 21, `mov eax,imm ; ret`). Matched on the
  first grade.

**Thing object -- `CThingObject`**
- 007034d0 `WorldUpdateWeaponTrails()`: updates the weapon trails and refreshes the graphic appearance, unless the thing
  is a melee weapon nobody carries.
  - HasTC is written `if (bits & mask) return true; return false;`, which gives `test edi,mask`.
  - `CInterfaceMap::LowerBound` 0x40F020 is a same-TU noinline stand-in, so the CThing bitset word stays in edi across
    the call.

## Parked for Fable 5.1 (5)
- 006fc3e0 std::__linear_insert DIFFER(139v137): retail `mov ebx,[esp+8] ; cmp ebx,[esi]` loads the key straight into ebx; ours does `mov ecx,[esp+0Ch] ; cmp ecx,[esi] ; mov ebx,ecx`, and ecx/edx are swapped around the delete[] of the else-branch temporary | tried: STLport original form, + same-TU __copy_backward body (fixed the tag slot), key local with memberwise assign, `pair(key, val.second)` ctor | next: compare `val.first` directly with no key local and reorder the copy -- else lane gap: register choice
- 006f9640 NAnimationEvents::CManager::AnimHasExtraEvent DIFFER(128v126): ours has an extra `push ecx` frame word for Find's key copy; retail homes the key copy, the Lookup out index and the find_if tag all in the dead `anim` argument slot (every esp offset shifts by 4) | tried: Find(const K&) with no copy, lookup in an inline helper, __forceinline helper | next: reorder/scope the index and tag declarations so all three share the arg slot -- else lane gap: frame-slot sharing
- 006f96c0 NAnimationEvents::CManager::GetExtraEventTime DIFFER(94v94): `xor ebx,ebx` is scheduled before `mov [esp+18h],ebp` in ours, after it in retail | tried: result init after Find, ternary, if/else | next: lane gap: scheduling
- 00701750 COpinionReactionManager::GetThingOpinionOfPlayer DIFFER(229v227): retail `xor edx,edx ; mov [eax],edx ...` stores the five zeros through eax; ours `xor ecx,ecx ; mov edx,eax ; mov [edx],ecx ...` (FillN's cursor copy) | tried: STLport vector(n) over a zero-init _Vector_base (254), allocation in the init list (230), landed alloc-proxy CArrayBase + FillN (254), local data pointer + FillN then end/finish/start stores (229) | next: FillN that advances its `first` parameter itself (STLport fill_n), with no cursor copy
- 00703210 CThingObject::Create DIFFER(217v217): register swap only -- retail `mov edi,edx ; mov ebx,ecx` (position in edi, defIndex/physics in ebx); ours `mov ebx,edx ; mov edi,ecx` | tried: result=0 before Alloc (registers right, but xor ebp before the call and physics reloaded), physics local + result after Alloc, physics local at top, result declared at top and zeroed after Alloc | next: the inline Alloc/new form of the landed CThingNoise::Create 0x8385B0 -- else lane gap: register allocation

## Skipped
None.

## Label corrections

| Address | Catalog label | Real function |
|---|---|---|
| 006f2760 | BinaryTree_LowerBoundPair | `std::_Rb_tree<u64, pair<const u64, CThingCreatureBase*>>::equal_range` |
| 006f27b0 | BinaryTree_LowerBoundPair_Helper | `_Rb_tree<u64, pair<const u64, CCombatantInfo>>::equal_range` |
| 006f2870 | BinaryTree_CloneNodeRecursive | `_Rb_tree::_M_copy` |
| 006f2b30 | CDefendingCombatantInfo_PartitionProngs | `std::__partition<CProng*, CIsProngUsable>` |
| 006f3260 | Vector_InsertRepeatElements | `vector<pair<C3DVector,bool>>::_M_insert_overflow` |
| 006f34b0 | CDefendingCombatantInfo_ApplyAttackerFunctor | `std::for_each<map iterator, CGetBestAttackerForProng>` |
| 006f40a0 | `_Move<CMusicEntry*>` | `__uninitialized_copy<CCombatRing*>` |
| 006f40f0 | `fill<CTextureManagerPoolInfo*>` | `__uninitialized_fill_n<CCombatRing*>` |
| 006f4140 | `_Move<CMusicEntry*>` | `__uninitialized_copy<const CCombatRing*>` |
| 006f43c0 | Vector_InitFromRange_Prongs | `vector<CCombatRing>::vector(const vector&)` |
| 006f4930 | CDefendingCombatantInfo_PushBackProng | `vector<CCombatRing>::push_back` |
| 006f4af0 | std_pair_EAnimComponent_CIVCountedPointer_dtor | `pair<const u64, CCombatantInfo>` scalar deleting dtor |
| 006f4bc0 | CMemoryAllocatorVariableSize_EraseAndDestroy | `_Rb_tree::erase(iterator)` |
| 006f4c20 | ClearPrimitiveList | `_Rb_tree::_M_erase` |
| 006f5000 | LTextBinTree_..._ctor | `_Rb_tree()` |
| 006f9640 | AnimHasEvent_Check | `NAnimationEvents::CManager::AnimHasExtraEvent` |
| 006f96c0 | GetEventTimeInSeconds_Impl | `CManager::GetExtraEventTime` |
| 006f9ac0 | Game_ApplyAdditionalImpulseIfInCombat | `CManager::GetExtraEventNames` |
| 006f9b50 | Game_GetBestTargetInDirection | `CManager::GetExtraEvents` |
| 006fafe0 | AnimEventArray_Reallocate | `CSmallVector<CEvent,32>::Allocate` |
| 006fb8a0 | `_Copy_backward<CPixelFormat*>` | `__destroy_aux<SKeyedEvents*>` |
| 006fbb30 | `_Fill_n<unsigned long*>` | `__uninitialized_fill_n<SKeyedEvents*>` |
| 006fbee0 | std_quicksort_Partition | `__unguarded_partition` |
| 006fbfd0 | std_heapify_PopElement | `__pop_heap` |
| 006fc070 | std_heapify_BuildHeap | `__make_heap` |
| 006fc3e0 | Vector_Erase_WithMove | `__linear_insert` |
| 006fc8e0 | `_For_each<CThingCreatureBase**, CApplyAdditionalImpulse...>` | `for_each<const CEvent*, CFillEventNames>` |
| 006fcb30 | AnimEvent_DeleteAll | `__insertion_sort` |
| 006fcc80 | ParticleMorphs_BuildEntry | `NAnimationEvents::CFillEvents::operator()` |
| 006fcd90 | AnimEventVector_PushBack | `vector<SKeyedEvents>::push_back` |
| 006fcea0 | AnimEventMap_MakeHeapSort | `__partial_sort` |
| 006fd330 | CPatchTesselationEdgeStrip_dtor | `NAnimationEvents::CManager` scalar deleting dtor |
| 006fd4f0 | Vector3_AngleBetween | `CGameCameraBase::GetRotationAngle` |
| 006fed30 | ConsoleSetViewPort | `CReactionFrequencyTraits_PeriodicFrequency` ctor (vtable 0x1264B7C) |
| 00701750 | COpinionReactionManager_GetOpinionValue | `COpinionReactionManager::GetThingOpinionOfPlayer` |
| 00702810 | CViewBase_FindParticleMorph | `CTCQuestCard::IsActivatable` |
| 00703210 | Create | `CThingObject::Create` |
| 007034d0 | CRenderTarget_UpdateWeaponTrails | `CThingObject::WorldUpdateWeaponTrails` |

Two related findings:
- Callee 0x00686D50 carries two labels: `CTCBase::GetPlayerManager` and `_global::GetGlobal_013b8a1c_deref0x30`.
- Callee 0x004C9CA0 is `CThing::DoActivationSequence`.
