# Hand chunk 380 (CAnimationSet .. CTCInventoryAbilities): 37 of 40 byte-exact

All 40 rows sit in the `_global` catalog module under propagated labels. They resolve to the animation-set TU
(CAnimationSet / CAnimationEntry), three STLport 4.x sort instantiations it uses (CCharString, the 8-byte
anim-component pair, the 20-byte CRC/entry pair), the anim-component map's tree erase, a quest-boast list, and four
CTCInventoryAbilities button-map members. Every matched row passes its behaviour test under g++ (tall.sh) and VC7.1
(tall.py). No row needed a flag; the four CTCInventoryAbilities rows use the TU pragmas noted below.

## What matched (37)

- **CAnimationSet** (1): 005dc2e0 AnimExists(name, filter). It walks the CRC run from FindFirstAnimWithName, comparing
  `i->first == it->first` through both pointers, and returns true on the first entry the virtual filter accepts. The
  plain `for` with an early `return true` reproduces the rotated loop and the two `al` exits.
- **STLport sort<CCharString*>** (11): 005e0650 __unguarded_partition, 005e0710 __unguarded_linear_insert, 005e08c0
  __push_heap, 005e0fe0 copy_backward, 005e1170 __adjust_heap, 005e1a40 __linear_insert, 005e1be0 __make_heap, 005e20c0
  __insertion_sort, 005e2eb0 __partial_sort, 005e3740 __introsort_loop, 005e3a50 sort. These are /Gr `__fastcall`
  bodies. The key shape: CBasicString<char>::operator< (0x00429950) gets its real body *above* the callers under
  `#pragma auto_inline(off)`. VC7.1 then knows the call writes no memory and keeps the by-value pivot/val payload
  pointer in esi across the compares, where a bare declaration re-reads it after every call. Other fixes:
  - copy_backward needs `__forceinline` on its `__copy_backward(..., random_access_iterator_tag, int*)` helper, and the
    leaf `?copy_backward@@` (the plain leaf picks the larger helper symbol).
  - sort's `__less(T*)` returns `less<T>()`, which stores a zero byte unless `less` has a user `less() { }`.
  - __partial_sort inlines make_heap and __pop_heap.
  - __introsort_loop calls __median (0x005E0580) and __unguarded_partition out of line, and inlines partial_sort.
- **STLport, 8-byte pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >** (9): 005e0760 swap, 005e0d80 __copy,
  005e0f00 __destroy_aux, 005e0fa0 __unguarded_insertion_sort_aux, 005e19d0 __unguarded_partition, 005e1c30
  __make_heap, 005e2f40 __partial_sort, 005e37d0 __introsort_loop, 005e3110 vector::push_back.
  - The STLport pair has an explicit memberwise copy constructor.
  - CIVCountedPointer has an add-ref copy ctor, a `__forceinline Release()` with the zero store, and a self-testing
    `operator=`. The zero store survives in __destroy_aux and is dropped for dying temporaries.
  - __introsort_loop's __median is inline here (nested `if`s), unlike the CCharString instance.
  - push_back's `__false_type` tag needs a user ctor and lands in x's argument slot.
- **STLport, 20-byte pair<unsigned long, CAnimationEntry>** (8): 005e1020 swap, 005e10b0 __unguarded_linear_insert,
  005e2100 __unguarded_partition, 005e2320 __make_heap, 005e2770 __linear_insert, 005e2e50 __insertion_sort, 005e3550
  __partial_sort, 005e36b0 vector::push_back. These use the same models. VC7.1 itself reproduces retail's choice of
  out-of-line versus inline implicit pair `operator=`:
  - swap calls it out of line for `a = b` and inlines it for `b = tmp`.
  - __linear_insert's copy_backward is an inline wrapper over the out-of-line `__copy_backward` (0x005E0E70).
- **STLport _Rb_tree** (1): 005e1f30 `_M_erase` of map<CCharString, CIVCountedPointer<CAnimComponentBase> >. It is the
  textbook `while (x) { _M_erase(right); y = left; _Destroy(&value); deallocate(x); x = y; }`. The pair's implicit
  destructor releases the pointer (keeping the zero store), then calls ~CCharString. The allocator's `if (p) free(p)`
  test folds away.
- **STLport list<CQuestManager::CBoast>** (3): 005e90b0 erase(first, last), 005e9280 _M_insert_dispatch, 005e9630
  operator=. The list is CTCQuestCompletionUI::Boasts. The element is the PDB's 0x20-byte CBoast (CCharString +
  2 CWideString, implicit copy/assign/destroy).
  - `_M_insert_dispatch` needs the STLport 4.6 insert link order, with `p = n->_M_prev` read into a named local first;
    the 4.0 re-reads store tmp->next before tmp->prev.
  - `operator=` needs the real STLport iterator: `_List_iterator(const iterator&)` is iterator's *user* copy ctor
    (built in place at esp), but only a converting ctor for const_iterator, whose implicit copy is a plain `push`.
- **CTCInventoryAbilities** (4): 005ee365 GetSpecialAbilityPDefSetToButton, 005eed4c OnSpecialAbilityButtonPressed,
  005f6423 SetAbilityToButton, 005f66d9 SetExpressionToButton.
  - These use a retail-layout local model: Abilities is at +0x160, AbilityButtons at +0x16C and ExpressionButtons at
    +0x1C8. engine/CTCInventoryAbilities.h has the PDB offsets.
  - The CVectorMap members carry FableWin.pdb names: Find, Insert(const K&, const V&), Update() `if (Dirty) Sort();`
    and Sort().
  - In the size TU, `Find` and `Insert` must be `__forceinline`.
  - The const and non-const LowerBound copies (0x005F7F89 and 0x005F7F4C) are separate overloads.
  - 005ee365 and the two setters are `optimize("y", off)` + `("s", on)`; 005eed4c is `("s", on)` only.
  - The value search is the landed 0x005EDF33 early-return helper.

## Parked for Fable 5.1 (3)

- 005dc340 CAnimationSet::GetAnim DIFFER(56v66): retail `push ebx ; push ebp ; push esi ; push edi ; push eax ; mov edi,ecx` keeps the never-read `this` in edi (filter ebx, countdown ebp, plus the 6-byte `lea ebx,[ebx]` loop alignment); ours saves three registers (filter edi, countdown ebx) | tried: `for(;;)` with `break`, an empty-if end test, an `|| true` end condition, an unreachable trailing return with `return it->second` inside the loop | next: a source shape that keeps `this` live into register allocation (an ASSERT-style `it != Anims.end()` test compiled out, an inline helper member), else lane gap: dead `this` held in a callee-saved register
- 005e0380 CAnimationEntry::GetComponent<CAnimComponentFlags> DIFFER(120v125): retail zeroes one key word before the null test and compares both LowerBound results with the immediate (`cmp dword ptr [eax],0 ; jle/jg`) with three separate `*pp` tails; ours (a const local key) shares the word but re-reads it per compare (`mov ecx,[esp+8] ; cmp ecx,[eax] ; jge`) and cross-jumps the found tail | tried: the StaticType() temporary bound to `find(const K&)` (128v125), a by-value `find(K)` over `LowerBound(const long&)` (107v125: compares fold but each branch zeroes its own key word), the const local (120v125) | next: lane gap: the two-level CVectorMap lookup family F88 (FINDINGS_LOG, 10 bodies, best 120v125 on the DDK lane too); re-grade on the Windows RTM box
- 005e2b30 vector<pair<unsigned long, CAnimationEntry> >::reserve DIFFER(230v226): retail homes both empty `__false_type` tag temporaries in n's live argument slot (`lea edx,[esp+18h]`, one `push ecx` word for tmp); ours opens `sub esp,8` with the tags at `[esp+13h]`, every other instruction identical | tried: the plain STLport form, the `_IsPODType` typedef + `__destroy(first, last, T*)` chain | next: lane gap: empty tag temporary over a live argument slot (GOTCHAS wave 24n, 0x0053848F)

## Skipped

None. No row holds two bodies, and every row's bounds match its `ret`.

## Label corrections

| Address | Catalog label | Correct name |
|---|---|---|
| 005dc2e0 | CAnimationDef_FindAnimationByName | CAnimationSet::AnimExists |
| 005dc340 | Animation_FindNthValid | CAnimationSet::GetAnim |
| 005e0380 | AnimationDef_GetAnimationFlags | CAnimationEntry::GetComponent<CAnimComponentFlags> |
| 005e0650 | QuickSort_Partition_StringKey | STLport __unguarded_partition<CCharString*> |
| 005e0710 | SortedList_Insert | STLport __unguarded_linear_insert<CCharString*> |
| 005e0760 | swap<CAnimationEntry> | swap<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > > |
| 005e08c0 | _Push_heap<...> | STLport __push_heap<CCharString*> |
| 005e0d80 | _Copy_backward<CDefPointer...> | STLport __copy of the 8-byte anim-component pairs |
| 005e0f00 | _Copy_backward<CDefPointer...> | STLport __destroy_aux of the 8-byte anim-component pairs |
| 005e0fa0 | _Copy_impl<CComplexNumber*> | STLport __unguarded_insertion_sort_aux of the 8-byte pairs |
| 005e0fe0 | STL_CopyBackward_CCharString | STLport copy_backward<CCharString*> |
| 005e1020 | swap<CAnimationEntry> | swap<pair<unsigned long, CAnimationEntry> > |
| 005e10b0 | _Move<CPhysicsMeshInfo*> | STLport __unguarded_linear_insert of the 20-byte entry pairs |
| 005e1170 | _Adjust_heap<...> | STLport __adjust_heap<CCharString*> |
| 005e19d0 | QuickSort_Partition_8Byte | STLport __unguarded_partition of the 8-byte pairs |
| 005e2100 | QuickSort_Partition_20Byte | STLport __unguarded_partition of the 20-byte pairs |
| 005e1a40 | SortedList_Insert_CCharString | STLport __linear_insert<CCharString*> |
| 005e2770 | std_MoveElement_Pair | STLport __linear_insert of the 20-byte pairs |
| 005e1be0 | STL_MakeHeap_CCharString | STLport __make_heap<CCharString*> |
| 005e1c30 | STL_BuildHeap_PairRefCounted | STLport __make_heap of the 8-byte pairs |
| 005e2320 | STL_BuildHeap_Pair20Byte | STLport __make_heap of the 20-byte pairs |
| 005e1f30 | BinaryTree_DestroyRecursive_WithRefcount | STLport `_Rb_tree::_M_erase` (map<CCharString, CIVCountedPointer<CAnimComponentBase> >) |
| 005e20c0 | std_CopyStringRange | STLport __insertion_sort<CCharString*> |
| 005e2e50 | std_HeapInitCopyPairs | STLport __insertion_sort of the 20-byte pairs |
| 005e2b30 | std_Vector_Reserve | vector<pair<unsigned long, CAnimationEntry> >::reserve |
| 005e2eb0 | std_sort_AdjustHeap | STLport __partial_sort<CCharString*> |
| 005e2f40 | STL_HeapSortPhase | STLport __partial_sort of the 8-byte pairs |
| 005e3550 | std_sort_IntrosortPartition20Byte_Callback | STLport __partial_sort of the 20-byte pairs |
| 005e3110 | Vector_PushBack | vector<8-byte pair>::push_back |
| 005e36b0 | std_Vector_PushBack_Inline | vector<20-byte pair>::push_back |
| 005e3740 | std_sort_QuicksortIntrosort | STLport __introsort_loop<CCharString*> |
| 005e37d0 | std_sort_IntrosortPartition8Byte | STLport __introsort_loop of the 8-byte pairs |
| 005e3a50 | CCharStringPair_SortedRange | STLport sort<CCharString*> |
| 005e90b0 | std_List_DestroyNodeRange | list<CQuestManager::CBoast>::erase(first, last) |
| 005e9280 | std_List_CloneNodeRange | list<CBoast>::_M_insert_dispatch<const_iterator> |
| 005e9630 | std_List_Swap | list<CBoast>::operator= |
| 005ee365 | CTCInventoryAbilities_FindAbility | CTCInventoryAbilities::GetSpecialAbilityPDefSetToButton |
| 005eed4c | CTCInventoryAbilities_TriggerSpecialAbility | CTCInventoryAbilities::OnSpecialAbilityButtonPressed |
| 005f6423 | AbilityMap_SetData | CTCInventoryAbilities::SetAbilityToButton |
| 005f66d9 | CRandomAppearanceMorph_SetSkeletalMorphValue | CTCInventoryAbilities::SetExpressionToButton |

Callee labels found wrong on the way:

| Address | Catalog label | Correct name |
|---|---|---|
| 0x005E1B60 | `pair<long,CDefPointer>::operator=` (propagated) | __pop_heap of the 8-byte pairs |
| 0x005DC290 | – | CAnimationSet::GetNoAnims |
| 0x005E2460 | – | `_Rb_tree::clear` of the anim-component name map |
| 0x005F87C0 | MedianCut::GetNewTreeNode | vector<pair<long, EHeroAbility> >::erase(iterator) |
| 0x00593139 | MedianCut::GetNewTreeNode | vector<pair<long, long> >::erase(iterator) (ICF-shared) |
| 0x005F9C4F | Vector_PushElement_8byte | vector<pair<long, ...> >::push_back(const pair&) |
| 0x005947B6 | Vector_PushBack_Pair | vector<pair<long, ...> >::push_back(const pair&) |
| 0x005FA47D | CGuiVarTransferBool::TransferToGui | CVectorMap<long, EHeroAbility>::Sort() |
| 0x005FA496 | CGuiVarTransferBool::TransferToGui | CVectorMap<long, long>::Sort() |
| 0x005FA4AF | – | CVectorMap::Update() (`if (Dirty) jmp Sort`) |
