# Hand chunk 422 (_STL::__partial_sort .. _STL::iter_swap): 39 of 40 byte-exact

All 40 rows are `_global` in the catalog. Every one turned out to be an STLport 4.6 helper or a small member of the
navigation / animation-file TUs. 34 rows grade RELOCATION_MATCH and 5 grade MATCH. Every test prints PASS under both
g++ and VC7.1. Default flags throughout: no row needed a `// Flags:` line. Algorithm templates are `__fastcall` (/Gr TUs).

## What matched (39)
- **_STL algorithms over the C3DAnimFile chunk records** (pair<long | CCharString, {CCharString, CCountedPointer}>,
  16-byte records):
  - 0xA73330 and 0xA733B0 `__partial_sort`: make_heap, __pop_heap and sort_heap are out of line; the comparator is inline
    and pair's explicit copy ctor makes the value copy. The string version also needs the same-TU noinline
    CBasicString<char>::operator< body.
  - 0xA734D0 and 0xA73530 `vector::push_back`: needs a same-TU noinline `_M_insert_overflow` stand-in.
  - 0xA73910 and 0xA73A00 `__introsort_loop`: __median is inline for the long keys and out of line (0xA70020) for the
    string keys.
- **vector<C2DLineF> / CArray<C2DLineF>**:
  - 0xA74940 `reserve`: STLport verbatim, with `_M_allocate_and_copy` inline and a same-TU noinline
    `__uninitialized_copy` that receives the tag.
  - 0xA74A20 `__uninitialized_fill_n` and 0xA74CA0 `__uninitialized_copy` of vector<C2DLineF>: these need a memberwise
    user copy ctor on C2DVector to get the dword copies.
  - 0xA74EA0 `CArray<C2DLineF>::DeallocateUnusedMemory` is `temp.reserve(size()); temp.insert(temp.begin(), begin(),
    end()); DeallocateMemory(); swap(temp);`. It matched only once reserve, `_M_range_insert` and erase had same-TU
    noinline bodies. With those bodies the swap and the temporary's destructor fold into "load the old start, take the
    temporary's 3 words, free the old start".
- **0xA762A0 GFProcessBreath**: the time goes into a named double local, and the test is `lastBreathTime - now >= 0.005`
  followed by a virtual `SuspendableProcess()`.
- **CNavRegion / CNavQuadTree**:
  - 0xA78680 `CNavRegion::CNavRegion(long, long)`: the list's `_List_base` ctor builds the sentinel in a local and
    publishes it last.
  - 0xA79290 `CNavQuadTree::OnNavMapUnload`: the bounds are named locals and vector::erase is inline over same-TU
    noinline `__copy` and `__destroy_aux` bodies, so the tags are homed on live slots.
  - 0xA80E90 `list<CCountedPointer<CNavRegionConnection> >::_M_insert_dispatch` (CNavRegion::Connections).
- **for_each functors**:
  - 0xA7CB50 deletes the quad-tree nodes: `if (node && node != &g_BlockedNavNode) delete node;`. The functor has user
    ctors, so it comes back through the hidden pointer.
  - 0xA7E440 is CSaveNavigationPositionToFile: a relative CNavigationPosition local, then CAFile::Write.
- **Navigation 64-bit-keyed sorts** (pair<unsigned __int64, bool> = CNavSwitchableLeafNode::UniqueIDsOpen,
  pair<unsigned __int64, CANavQuadTreeNode*> = CNavQuadTree::SwitchableNodes):
  - `__unguarded_linear_insert` 0xA7D570 / 0xA7D610.
  - `__unguarded_insertion_sort_aux` 0xA7EFE0 / 0xA7F060 (comparator declaration-only).
  - `__unguarded_partition` 0xA7FBA0 / 0xA7FCB0 (iter_swap inline).
  - `__make_heap` 0xA80000 / 0xA80080.
  - `__linear_insert` 0xA81C50 / 0xA81CC0: copy_backward goes to `__copy_backward_ptrs`, which is inline, then to the
    out-of-line random-access `__copy_backward`. The tag is homed in the dead comparator slot.
  - `__partial_sort` 0xA828C0 / 0xA829E0: __pop_heap is inline here.

  Each pair differs only in the value-word copy (a bool byte against a pointer dword).
- **vector<CNavigationPosition>**:
  - 0xA7E9D0 and 0xA7F9F0 `_M_allocate_and_copy`: the non-const and const iterator instantiations of the member
    template.
  - 0xA80980 `reserve`.
- **CNavigationLayer line lists**. list<C2DLineF> is 4 bytes with 0x18-byte nodes. LineLists is vector<list<C2DLineF> >
  and SwitchableLineLists is CVectorMap<unsigned __int64, list<C2DLineF> >.
  - 0xA81E20 `__unguarded_linear_insert`: matched once the list's operator= 0x517720, destructor 0x5127A0 and range
    insert 0x5152C0 had same-TU noinline bodies. Retail then keeps the by-value key in ebx:edi across the in-loop
    operator= calls.
  - 0xA82000 `__copy`.
  - 0xA825D0 pair copy ctor: the list copy ctor is inline, `pos` is built in its slot, and the tag goes in the dead `o`
    slot.
  - 0xA826D0 `__uninitialized_fill_n`.
  - 0xA827D0 `__destroy_aux` with `~_List_base` and `clear()` inline.
  - 0xA82960 `__unguarded_insertion_sort_aux`: a same-TU noinline `_M_insert_dispatch` homes the tag on the comparator
    slot.
  - 0xA82AC0 `__copy_ptrs` with `__forceinline __copy`.
  - 0xA82B10 `iter_swap`: needs `__forceinline __iter_swap` plus the same-TU list bodies.

## Parked for Fable 5.1 (1)
- 0xA79240 CNavQuadTree::SetSwitchableNodesAsBlocked DIFFER(80v77): retail loads the 64-bit ID into ebx:ebp after the UpperBound/LowerBound calls and re-reads the bool from its slot every iteration; ours keeps the bool in ebx and re-reads both ID halves in the loop | tried: inline EqualRange pair, named ID copy (ID in registers but loaded at entry, halves swapped), by-value key in EqualRange, named bounds UpperBound-first (4 grades + 1 SRC_FAIL) | next: same-TU noinline bodies of LowerBound 0xA7DC60 / UpperBound 0xA7DCC0 / CNavSwitchableLeafNode::SetAsBlocked 0xA78800. That lever fixed the three "field of an address-taken local kept in registers across calls" rows of this chunk.

## Skipped
None. No row holds a second function.

## Label corrections
Catalog name → real function:
- 0xA73330 `Introsort_QuickPartition_LongUnsigned` → `_STL::__partial_sort<pair<long,SNamedChunk>*>`.
- 0xA733B0 `Introsort_QuickPartition_FloatStr` → `__partial_sort` (CCharString keys).
- 0xA734D0 `Vector_PushBack_Refcounted` → `vector<pair<long,SNamedChunk> >::push_back`.
- 0xA73530 `Vector_Insert_ElementAt` → `vector<pair<CCharString,SNamedChunk> >::push_back`.
- 0xA73910 `Introsort_QuickPartition_Recursive` → `__introsort_loop` (long keys).
- 0xA73A00 `Vector_IntrosortDepth_StringPair` → `__introsort_loop` (string keys).
- 0xA74A20 `Vector_ConstructRanges_16ByteElements` → `__uninitialized_fill_n<vector<C2DLineF>*>`.
- 0xA74CA0 `MultiArray_CopyElements` → `__uninitialized_copy<vector<C2DLineF>*>`.
- 0xA74EA0 `Vector_ShrinkToFit_C2DLineF` → `CArray<C2DLineF>::DeallocateUnusedMemory`.
- 0xA78680 `PackedKeyList_Construct` → `CNavRegion::CNavRegion(long,long)`.
- 0xA79240 `CNavSwitchableLeafNode_BlockRange` → `CNavQuadTree::SetSwitchableNodesAsBlocked`.
- 0xA79290 `CVectorMap_EraseRange_Keys` → `CNavQuadTree::OnNavMapUnload`.
- 0xA7CB50 `ForEach_CallDeleteFunctor` → `for_each` with the node-delete functor. The functor name is descriptive.
- 0xA7D570 `SortedArray_InsertWithBackwardShift` and 0xA7D610 `SortedArray_InsertNavigationPos` → `__unguarded_linear_insert`
  (pair<u64,bool> / pair<u64,CANavQuadTreeNode*>).
- 0xA7E9D0 `Vector_Allocate_12ByteElements_Copy` and 0xA7F9F0 `..._CopyRange` →
  `vector<CNavigationPosition>::_M_allocate_and_copy` (non-const / const iterators).
- 0xA7EFE0 `_Copy_impl<CComplexNumber*,...>` and 0xA7F060 `_Copy_impl<CNavigationPosition*,...>` →
  `__unguarded_insertion_sort_aux` (u64 pairs).
- 0xA7FBA0 / 0xA7FCB0 `Heap_AdjustDown_*ByteElements` → `__unguarded_partition` (u64 pairs).
- 0xA80000 / 0xA80080 `Heap_BuildHeap_TextureDecal(_Var2)` → `__make_heap` (u64 pairs).
- 0xA80980 `Vector_Reserve_SpecificSize_12Byte` → `vector<CNavigationPosition>::reserve`.
- 0xA80E90 `List_InsertCopy_Range` → `list<CCountedPointer<CNavRegionConnection> >::_M_insert_dispatch`.
- 0xA81C50 `InsertionSort_Callback_PairLong` and 0xA81CC0 `InsertionSort_Callback_MeshInfo` → `__linear_insert` (u64 pairs).
- 0xA81E20 `Std_Insert_Into_Sorted_List` → `__unguarded_linear_insert<pair<u64,list<C2DLineF> >*>`.
- 0xA82000 `_Move<CEngineTextureMorph*,...>` → `__copy<pair<u64,list<C2DLineF> >*>`.
- 0xA825D0 `list_pair_init` → the `pair<u64,list<C2DLineF> >` copy ctor.
- 0xA826D0 `_Fill_n<unsigned long*,...>` → `__uninitialized_fill_n<list<C2DLineF>*>`.
- 0xA827D0 `DestroyBinaryTreeRange` → `__destroy_aux<list<C2DLineF>*>`.
- 0xA828C0 / 0xA829E0 `Heap_Merge_*ByteElements` → `__partial_sort` (u64 pairs).
- 0xA82960 `_Copy_impl<CNavigationPosition*,...>` → `__unguarded_insertion_sort_aux<pair<u64,list<C2DLineF> >*>`.
- 0xA82AC0 `Vector_AssignUninit_DestroyElements` → `__copy_ptrs<const list<C2DLineF>*, list<C2DLineF>*>`.
- 0xA82B10 `GuiTree_SwapAndReorder` → `iter_swap<pair<u64,list<C2DLineF> >*>`.

Callees:
- 0xA71880 / 0xA71910 are `__unguarded_partition`.
- 0xA70020 is the string-pair `__median`.
- 0x517720 / 0x5127A0 / 0x5152C0 are `list<C2DLineF>` operator= / destructor / range insert.

Landed labels that look wrong (details in LESSONS.md, "Review at landing"):
- 0x5127A0 is landed as "CGuiControlTreePane::SortTreeRecursively".
- The landed 0xA820E0 / 0xA821C0 model an "8-byte list".
- 0xA84FB0 is a "CRetailVector" push_back.
