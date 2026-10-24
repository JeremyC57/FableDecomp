# Hand chunk 378 (std::vector<CHeroMarriageInfo> .. std::vector<CDefPointer<const CTattooDef> >): 38 of 40 byte-exact

All 40 rows are catalog `_global` rows. They fall into five families: the STLport containers of the CTCHeroStats TU,
CTCInventory, the STLport sort/heap/vector/tree instantiations of the CTCInventory TU, the CFrontEndManager screen
stack, and CTCInventoryTrade. Every matched source is a frame-pointer size TU (`optimize("y", off)` +
`optimize("s", on)`) or a frameless size TU (`optimize("s", on)`). All 38 tests pass under g++ and in the VC lane.

## What matched (38)

- **CTCHeroStats TU STL** (CHeroMarriageInfo 24-byte records, CHousePosition, MirroredCriminalStatus):
  - 005806df `vector<CHeroMarriageInfo>::reserve`. `&_M_end_of_storage` is formed once and kept in a frame word; the
    old size reuses the dead `n` slot.
  - 00580928 `vector<CHeroMarriageInfo>::_M_fill_insert`, twin of the landed 0x0048608C. Same-TU helper bodies, plus
    the copy ctor's real memberwise body, keep `_M_finish` in ebx across the `__x_copy` construction.
  - 00580a7b `vector<pair<const CRegion*, C3DVector> >::_M_insert_overflow`: the STLport form, with every empty tag
    in the `atend` high byte (`[ebp+1Bh]`).
  - 005810da `__copy<const CHeroMarriageInfo*, back_insert_iterator>`: `/Gr` fastcall, hidden result pointer in ecx,
    count in the dead `last` slot.
  - 0058123a `__copy_aux<_List_iterator, back_insert_iterator>`: the tag temporary is homed in the pushed `result`
    slot (`[ebp+13h]`).
  - 005812df `map<CCharString, MirroredCriminalStatus>::operator[]`. `_Tp()` matches only as a named local of the
    insert block, and needs the noinline `less` body.
  - 00581404 `GFSerialiseVectorBinaryIn<CHeroMarriageInfo>`: the landed 0x005C2AD8 template with the inline stream
    Read and the 64-bit position guard.
- **CTCInventory:**
  - 0058ef79 `ContructItemLists` and 00590198 `ContructItemListsPC` (sic). The guild-seal name is a named CCharString
    in its own block, and end() is cached in a local.
  - 00592495 `pair<const CCharString, CCharString>`: the converting ctor from `pair<const char*, const char*>`
    (size TU, unaligned entry).
- **CTCInventory TU STL instantiations:**
  - CContextSensitiveItem (12 bytes):
    - 005924f4 `__uninitialized_fill_n` and 0059266b `__copy` (both frameless size TU).
    - 00593f39 `_M_insert_overflow`.
    - 00594782 `push_back`: `_Construct` must be `__forceinline`.
  - CFadeInFadeOutBase (28 bytes): 005926ce `__copy`, 00594880 `_M_insert_overflow` and 00594a73 `_M_fill_insert`.
    A local model is used because the generated header cannot declare the out-of-line copy ctor.
  - CPriorityDefPair (`pair<long, CDefPointer<const CContextSensitiveItemDef> >`), with
    `CContextSensitivePriorityCompare`:
    - 00592ffb `__median`, 00593924 `__unguarded_linear_insert` and 00593e10 `__unguarded_insertion_sort_aux`.
    - 00594637 `__unguarded_partition`, 00594706 `__make_heap` (frameless, ebp a register) and 00594946
      `__linear_insert`.
    - 00594c77 `__partial_sort`, 00594d36 `_M_insert_overflow` and 00594e4d `__introsort_loop`.
    - The shape keys: same-TU `auto_inline(off)` helper bodies so empty tags are homed in dead slots, and a
      comparator passed by value from its own slot.
  - `_Rb_tree<CDateAndTime, pair<const CDateAndTime, CWideString> >`: 0059339a `_M_lower_bound` (frameless),
    00593ce4 `_M_insert` (STLport `parent, left, right` store order; VC7.1 reorders it) and 00593d76 `insert_unique`.
  - 005943a2 `CThing::GetTC<T>`: an ICF-folded member template; CTCBase stands for T. The miss test compares the
    parameter, not the homed key copy.
- **CFrontEndManager screen stack** (STLport deque<CComponent*>, 32 pointers per 128-byte node):
  - 0059525d `ClearComponents` (frameless) and 005959ab `IsScreenOnStack`. The latter needs same-TU bodies of
    `back()` and `_M_decrement` so that `_M_finish._M_cur` stays cached in esi.
  - 0059ac41 `_Deque_iterator::_M_decrement` and 0059ac9f `_Deque_base::_M_initialize_map`, which uses
    `(max)` through const references.
  - CFrontEndManager is modelled locally because engine/CFrontEndManager.h has the Dinkumware Ego_r layout.
- **CTCInventoryTrade:**
  - 0059de04 `UpdateBuyActionsList` / 0059de76 `UpdateSellActionsList`. push_back binds to the node key itself
    only when the list's element type is the key type, so the model uses `list<CComponent*>`; `list<CActionParam*>`
    adds a conversion temporary (120 v 114). Sell adds the inline shopkeeper lookup (kind 0x53, LowerBound 0x0040F020)
    and the three shop-type tests in retail order 0x00772590, 0x00772490, 0x00772510. It matched on the first grade.
  - 005b3100 `map<CComponent*, pair<CWideString, long> >::operator[]` (NumBuyable/Sellable/WantedThings). This is the
    005812df named `_Tp def;` form, with pair's inline default ctor and an inline `less`.
  - 005b3275 `vector<CDefPointer<const CTattooDef> >::_M_insert_overflow`, the 00594d36 template. The inline add-ref
    copy ctor must be `__forceinline`; without it the call stays out of line and the registers flip (172 v 176).

## Parked for Fable 5.1 (2)

- 00594a08 std::map<CDateAndTime, CWideString>::operator[] DIFFER(104v107): retail re-addresses the `_Tp()` default after its out-of-line CWideString() ctor (`lea eax,[ebp-4] ; push eax`) where ours pushes the ctor's returned eax; all else matches | tried: bare `value_type(k, _Tp())`; named `_Tp __def;` (adds the lea but moves the insert result into the dead `k` slot, frame 0x24 v 0x28); named value_type (110v107); same-TU bodies of the pair ctor / _M_lower_bound / map::insert | next: a named `_Tp` declared before `lower_bound` (function scope) or `T def = T();` copy-init, keeping the result in its own word as 005b3100's inline-ctor version does
- 0059afd7 std::__copy<_Deque_iterator<CComponent*>> DIFFER(73v73): scheduling only -- in `*__result = *__first` retail loads the source element (`mov eax,[eax]`) before the destination cursor (`mov ecx,[ebp+28h]`); ours loads the cursor first | tried: POD iterator (hidden result pointer pushed, not in ecx); user memberwise copy ctor (called out of line); forced-inline copy ctor (four mov pairs instead of the movsd copy); STLport cursor/node + default ctors with an implicit copy (this file) | next: spell the store through iterator `operator*` returning `_Tp&` on both sides (`*__result = *__first` through `reference operator*() const { return *_M_cur; }` on a const iterator copy), or `_Tp __tmp = *__first; *__result = __tmp;`

## Skipped

None. Every row is a single whole function, and no BOUNDS_FIX problems were found: each row's length ends on its
`ret` and the next row's prologue.

## Label corrections

- 005806df "Vector_Reallocate" -> `vector<CHeroMarriageInfo>::reserve`
- 00580928 "Vector_InsertRangeInit" -> `vector<CHeroMarriageInfo>::_M_fill_insert`
- 00580a7b "Vector_ReallocateForCArray" -> `vector<pair<const CRegion*, C3DVector> >::_M_insert_overflow`
- 005810da "LoopConstructClosures" -> `std::__copy<const CHeroMarriageInfo*, back_insert_iterator<vector<CHeroMarriageInfo> >, int>`
- 0058123a "ConstructString" -> `std::__copy_aux<_List_iterator<CHeroMarriageInfo>, back_insert_iterator<...> >`
- 005812df "DeserializeStdMap_WithPairs" -> `map<CCharString, CTCHeroStats::MirroredCriminalStatus>::operator[]`
- 00581404 "GFSerialiseVectorBinaryIn<CBrainUpdateZone>" -> `GFSerialiseVectorBinaryIn<CHeroMarriageInfo>`
- 0058ef79 "ConstructItemCategoryList" -> `CTCInventory::ContructItemLists` (FableWin.pdb, sic)
- 00590198 "ConstructPCExpressionCategoryList" -> `CTCInventory::ContructItemListsPC`
- 00592495 "CCharString_Construct_Pair" -> `pair<const CCharString, CCharString>::pair<const char*, const char*>`
- 005924f4 "_Fill_n<unsigned_long*,...>" -> `std::__uninitialized_fill_n<CContextSensitiveItem*, unsigned int, CContextSensitiveItem>`
- 0059266b "_Move<CMusicSetEntry*,...>" -> `std::__copy<CContextSensitiveItem*, CContextSensitiveItem*, int>`
- 005926ce "_Copy_impl<CDegradableInfo*,...>" -> `std::__copy<CFadeInFadeOutBase*, CFadeInFadeOutBase*, int>`
- 00592ffb "Min3_CContextSensitiveItem" -> `std::__median<CPriorityDefPair, CContextSensitivePriorityCompare>`
- 0059339a "BinarySearchTree_FindNode_DateAndTime" -> `_Rb_tree<CDateAndTime, ...>::_M_lower_bound`
- 00593924 "PriorityQueue_SortedInsert" -> `std::__unguarded_linear_insert<CPriorityDefPair*, ...>`
- 00593ce4 "CMemoryAllocatorVariableSize_InsertNode" -> `_Rb_tree<CDateAndTime, ...>::_M_insert`
- 00593d76 "BinarySearchTree_FindOrInsert_DateAndTime" -> `_Rb_tree<CDateAndTime, ...>::insert_unique`
- 00593e10 "_Fill<CIntelligentPointer<CThing>*,...>" -> `std::__unguarded_insertion_sort_aux<CPriorityDefPair*, ...>`
- 00593f39 "Vector_ReallocateForUint" -> `vector<CContextSensitiveItem>::_M_insert_overflow`
- 005943a2 "CTCInterfaceBase_TryGetInterfaceElementByType" -> `CThing::GetTC<T>` (ICF-folded member template)
- 00594637 "Quicksort_Partition_CContextSensitive" -> `std::__unguarded_partition<CPriorityDefPair*, ...>`
- 00594706 "Heap_BuildFromRange" -> `std::__make_heap<CPriorityDefPair*, ...>`
- 00594782 "Vector_PushBack_Uint" -> `vector<CContextSensitiveItem>::push_back`
- 00594880 "Vector_ReallocateForFadeInFadeOut" -> `vector<CFadeInFadeOutBase>::_M_insert_overflow`
- 00594946 "PriorityQueue_InsertSorted" -> `std::__linear_insert<CPriorityDefPair*, ...>`
- 00594a08 "BinarySearchTree_FindAndConstruct_DateAndTime" -> `map<CDateAndTime, CWideString>::operator[]`
- 00594a73 "Vector_InsertRange_CFadeInFadeOut" -> `vector<CFadeInFadeOutBase>::_M_fill_insert`
- 00594c77 "Heap_Sort_CContextSensitive" -> `std::__partial_sort<CPriorityDefPair*, ...>` (the only caller is partial_sort 0x00594DE2)
- 00594d36 "Vector_Reallocate_PairAbilityDef" -> `vector<CPriorityDefPair>::_M_insert_overflow`
- 00594e4d "GameString_ConstructRecursive" -> `std::__introsort_loop<CPriorityDefPair*, ...>` (callers 0x00594EFF / 0x00594EAF)
- 0059525d "CCreatureAction_FireMissileWeapon_Finalize" -> `NUISystem::CFrontEndManager::ClearComponents`
- 005959ab "CShaderRenderManager_IterateElements" -> `NUISystem::CFrontEndManager::IsScreenOnStack`
- 0059ac41 "lua_getglobals" -> `_Deque_iterator<CComponent*>::_M_decrement`
- 0059ac9f "String_InitializeBuffer" -> `_Deque_base<CComponent*>::_M_initialize_map`
- 0059afd7 "ProcessItemStats" -> `std::__copy<_Deque_iterator<CComponent*>, ...>`
- 0059de04 "CTCCarryable_Cleanup" -> `CTCInventoryTrade::UpdateBuyActionsList`
- 0059de76 "Shop_CleanupCarryables" -> `CTCInventoryTrade::UpdateSellActionsList`
- 005b3100 "StdMap_LowerBound" -> `map<NUISystem::CComponent*, pair<CWideString, long> >::operator[]`
- 005b3275 "Tattoo_ReallocateAndCopy" -> `vector<CDefPointer<const CTattooDef> >::_M_insert_overflow`
- Callees and neighbours:
  - 0x0042B6B8 "Std_List_PushFront" is `list::push_back`, since it inserts before end().
  - 0x0042ADE5 "CTCCarryable::OnKill" is `map<CComponent*, CComponent*>::clear`.
  - 0x0059AF2A, the landed "CTCCarryable::OnKill", is `map<FRONTEND_SCREEN, CComponent*>::clear`.
  - 0x0058056F "ListPushFrontWrapper" is `list<CHeroMarriageInfo>::push_back`.
  - 0x0057F092 "GFToScreen" is the `pair<const CRegion*, C3DVector>` copy ctor.
  - 0x005B1DFE "RBTree_FindNode_Uint3" is `map<CComponent*, pair<CWideString, long> >::_M_lower_bound`.
  - 0x00592DFB carries a CHeroAbilityDef pair label, but that comes from an ICF twin; here it is the CPriorityDefPair
    copy ctor.
  - 0x005B26E9 is `_Destroy` over a `CDefPointer` range; the landed file still names it `_Destroy_range<CFlash>`.
