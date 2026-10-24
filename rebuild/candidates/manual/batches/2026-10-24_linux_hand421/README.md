# Hand chunk 421 (_STL::__copy_backward<CIVCountedPointer<CLocalDetailPlacementGrid>*> .. _STL::__insertion_sort<pair<CCharString,C3DMeshVolume>*>): 39 of 40 byte-exact

All 39 matched rows grade MATCH (5) or RELOCATION_MATCH (34) through gall.sh. Their tests print `<addr>_TEST PASS` under
g++ (tall.sh) and the lane's VC7.1 (tall.py).

Apart from 0x00A6B0E0 and the parked 0x00A6BFC0, the chunk is one translation unit: the CPhysicsMesh TU
(0x00A6B1A0..0x00A73330). It holds the STLport sort/heap/copy helpers instantiated for the mesh's sorted CVectorMaps
(retail CVectorMap is 16 bytes: a 12-byte STLport vector, then the comparator, then Dirty):

| Map | Offset | Entry | Comparator |
|---|---|---|---|
| HelperPoints | +0x10 | pair<CCharString, C3DHelperPoint>, 20 bytes | CCompareString (FastCompare 0x009E6530) |
| DummyObjects | +0x20 | pair<CCharString, C3DDummyObject>, 56 bytes | CCompareString |
| NavigationContainmentVolumes | +0x54 | pair<long, C3DMeshVolume>, 16 bytes | signed key |
| ContainmentVolumes | +0x64 | pair<CCharString, C3DMeshVolume>, 16 bytes | CKeyPairCompareLess: inline CCharString::operator< over CBasicString<char>::operator< 0x00429950 |

The TU also holds a pair<unsigned long, unsigned long> sort.

## What matched (39)

### _STL algorithms over pair<unsigned long, unsigned long> with less<>

- 0x00A6B1A0 `__unguarded_linear_insert`, 0x00A6B420 `__unguarded_partition`, 0x00A6B480
  `__unguarded_insertion_sort_aux`, 0x00A6B820 `__linear_insert`, 0x00A6BBB0 `__partial_sort`.
- These are plain STLport 4.6 bodies with the lexicographic `operator<` of pair and `less<>` with a user ctor.
- In the partition, iter_swap is the inline `__iter_swap(a, b, __value_type(a))`.
- In `__linear_insert`, the copy_backward chain is fully inline and `__unguarded_linear_insert` is declared only.
- In `__partial_sort`:
  - `make_heap` stays inline and reaches the declared `__make_heap(first, last, comp, T*, int*)`.
  - `__pop_heap` stays inline and reaches the declared `__adjust_heap`.
  - `sort_heap` is declared.

### _STL::__copy_backward<CIVCountedPointer<CLocalDetailPlacementGrid>*> (0x00A6B0E0)

- This is the SGI `__copy_backward` with the CIVCountedPointer assignment inlined. It is a byte twin of the landed
  0x00769210.
- `RemoveRef` must be a member of the pointee, not of the pointer.

### CPhysicsMesh methods (real names from egor_pdb_names.tsv)

- 0x00A6C9C0 `GetHelperPointPositionStatic(name, index)`: `return HelperPoints.LowerBound(name)[index].second.Position;`
  - C3DVector must be pack(1) and needs the user memberwise copy ctor. With the implicit copy the result is 51v49
    (`push esi ; mov ecx,eax`).
  - This row took 4 grades.
- 0x00A6CA20 `IsInContainmentVolume`: `last = UpperBound(name)` before `for (it = LowerBound(name); ...)`.
- 0x00A6CA80 `PeekRoughProportionOfSphereContained`: the same loop sums the contributions, then clamps the result
  (`< 0 -> 0`, `> 1 -> 1`).
- 0x00A6CBC0 `HasContainmentVolume`: inline `CVectorMap::Find` (LowerBound / end / Compare(key, *it)) with a named
  iterator, then `!= end()`.

### vector<C3DTriangle2> (6-byte VertexIndex[3])

- 0x00A6E470 `_M_allocate_and_copy<const C3DTriangle2*>`: the member template needs `__forceinline` on
  `__uninitialized_copy` (otherwise a cdecl call, 59v83). Placement new is `throw()`. This row took 2 grades.
- 0x00A70BC0 `reserve`: STLport 4.6 body. `_M_allocate_and_copy` is a declared member template.

### _STL helpers over pair<CCharString, C3DHelperPoint> / pair<CCharString, C3DDummyObject> (CCompareString)

- 0x00A6E5E0 `swap<pair<CCharString,C3DHelperPoint>>`.
- 0x00A6E840 `__copy_backward` over the dummy-object pair.
- 0x00A6EDE0 `__uninitialized_fill_n` and 0x00A6EE30 `__uninitialized_copy`.
- 0x00A6EFD0 / 0x00A6F070 `__unguarded_linear_insert`.
- 0x00A6F610 / 0x00A6F6F0 `__unguarded_partition`:
  - The 20-byte pair's swap is a call, so it is declared (0x00A6E5E0).
  - VC inlines the 56-byte pair's swap from the plain body.
- 0x00A70630 / 0x00A706B0 `__make_heap` (`__adjust_heap` declared).
- 0x00A70F40 `__linear_insert` (`__copy_backward` 0x00A6E7C0 and the unguarded insert declared).
- 0x00A72300 / 0x00A72380 `__partial_sort`: here `__pop_heap` and `sort_heap` are out of line, so they are declared.
- 0x00A72700 `vector<pair<CCharString,C3DDummyObject>>::push_back`:
  - STLport form: `_Construct` with the null test from the nothrow placement new.
  - `_M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true)` is declared. The tag is homed in x's dead slot
    without needing a same-TU body.
- The C3DDummyObject copy is the implicit 13-dword `rep movsd`.

### _STL helpers over pair<long, C3DMeshVolume> / pair<CCharString, C3DMeshVolume> (CKeyPairCompareLess)

- The family's model:
  - `C3DMeshVolume::operator=` is defined in-class (Name, then CCountedPointer ShareData). VC inlines it at some
    sites and calls the 0x00A32960 COMDAT at others, matching retail per site.
  - The implicit pair copy constructor and assignment.
  - CCountedPointer: copy ctor plus a dtor that releases the count and then zeroes Data and Info.
- 0x00A703C0 / 0x00A70540 `__unguarded_linear_insert`.
- 0x00A71090 `__unguarded_insertion_sort_aux`.
- 0x00A71880 `__unguarded_partition` for the long key (swap declared).
- 0x00A71910 `__unguarded_partition` for the string key. Two changes were needed (3 grades):
  - Declare `~pair();  // 0x00A6F390`, because retail calls the pivot's destructor out of line and VC inlines it
    (182v110).
  - Add the same-TU `CBasicString<char>::operator<` body (landed 0x00429950) under `auto_inline(off)`, which fixes the
    register allocation (119v110 to match).
- 0x00A71BE0 `__pop_heap`: `*result = *first; __adjust_heap(first, 0, last - first, val, comp)`. The callee
  destroys val at its end.
- 0x00A71CA0 / 0x00A71D40 `__make_heap`.
- 0x00A728F0 / 0x00A729B0 `__linear_insert` (`__copy_backward` 0x00A70A10 / 0x00A70AB0 declared).
- 0x00A73240 / 0x00A732B0 `__insertion_sort(first, last, comp)` (STLport 4.6 form without the `_Tp*` tag, `ret 4`).

### _STL::for_each<C3DVector*, CAddElements<C3DVector>> (0x00A6ECE0)

CAddElements has a user constructor, so the functor comes back through the hidden result pointer in ecx.

## Parked for Fable 5.1 (1)

- 00a6bfc0 CPreallocTexturePool::`scalar deleting destructor' DIFFER(30v64): ours calls the compiler-generated ~CPreallocTexturePool out of line (`call ; test flags ; delete`) where retail inlines it into ??_G (__destroy_aux 0x00A6B700 call with the tag homed in the flags slot, null-tested free of the vector block, ~CResourceList 0x009FC5C0 call) | tried: plain-inline vector destructors (40v64: implicit dtor inlined but ~vector stays a call), __forceinline ~vector/~_Vector_base/_Destroy (30v64: implicit dtor out of line), `novtable` + `__forceinline` user dtor (no ??_G emitted at all), flattened _Destroy + free in ~vector (30v64) | next: __forceinline only on _Destroy and ~_Vector_base with ~vector plain inline, or `#pragma inline_depth(255)` around the TU; retail has no vtable re-store, so a user-defined dtor is unlikely

## Skipped

None. No row covers two functions, and none is int3-after-noreturn.

## Label corrections

Each line gives the manifest label, then the correct name.

- 0x00A6BFC0: `CDataBankGroupEntry_Destructor` is CPreallocTexturePool::`scalar deleting destructor'. It is slot 0 of
  vtable 0x0129DC50, its only reference. 0x009FC5C0 is CResourceList::~CResourceList; the catalog's
  `Stack<unsigned long,64>::~Stack` there is propagated.
- 0x00A6B0E0: `_Copy_backward<CDefPointer<CThingBaseDef const>*,...>` is propagated from the byte twin 0x00769210. This
  instance is __copy_backward<CIVCountedPointer<CLocalDetailPlacementGrid>*>: its callers 0x00A6B39C and 0x00A6BD49
  are the detail-placement-grid vector insert paths.
- CPhysicsMesh methods:
  - 0x00A6C9C0 `CVectorMap_GetValueByIndex` is CPhysicsMesh::GetHelperPointPositionStatic.
  - 0x00A6CA20 `C3DMeshVolume_IsPointInContainment` is CPhysicsMesh::IsInContainmentVolume.
  - 0x00A6CA80 `C3DMeshVolume_GetProportionInContainment` is CPhysicsMesh::PeekRoughProportionOfSphereContained.
  - 0x00A6CBC0 `C3DMeshVolume_HasVolume` is CPhysicsMesh::HasContainmentVolume.
- C3DTriangle2 vector:
  - 0x00A6E470 `AllocAndCopyRange_6byte` is vector<C3DTriangle2>::_M_allocate_and_copy<const C3DTriangle2*>.
  - 0x00A70BC0 `StdVector_Reserve_Tri` is vector<C3DTriangle2>::reserve.
- Helper-point and dummy-object pairs:
  - 0x00A6E5E0 `swap<C3DSkeletalBoneMorph>` (propagated) is swap<pair<CCharString,C3DHelperPoint>>.
  - 0x00A6E840 `_Copy_backward<C3DGeneratorObject*,...>` (propagated) is __copy_backward<pair<CCharString,C3DDummyObject>*>.
  - 0x00A6EDE0 `fill<C3DAnimationStateAlpha*,...>` (propagated) is __uninitialized_fill_n<pair<CCharString,C3DHelperPoint>*, unsigned int>.
  - 0x00A6EE30 `std_Copy_CCharString_Array` is __uninitialized_copy<pair<CCharString,C3DDummyObject>*>.
  - 0x00A6EFD0 `std_fill_Pair_WithComparison` / 0x00A6F070 `std_fill_StringDummyObjectPair_Radial` are
    __unguarded_linear_insert (helper / dummy pair).
  - 0x00A6F610 `QuickSort_Partition_Skeleton_Small` / 0x00A6F6F0 `QuickSort_Partition_CCharString` are
    __unguarded_partition (helper / dummy pair).
  - 0x00A70630 / 0x00A706B0 `HeapSort_MakeHeap_*` are __make_heap.
  - 0x00A70F40 `Vector_ConstructElement_Pair` is __linear_insert (helper pair).
  - 0x00A72300 `QuickPartition_Generic` / 0x00A72380 `Introsort_QuickPartition_BlendedEnv` are __partial_sort
    (helper / dummy pair).
  - 0x00A72700 `Vector_PushBack_CCharString` is vector<pair<CCharString,C3DDummyObject>>::push_back.
- Mesh-volume pairs:
  - 0x00A703C0 `std_copy_ComplexNumber_WithCleanup` / 0x00A70540 `std_partition_PairWithOrdering` are
    __unguarded_linear_insert (long / CCharString key).
  - 0x00A71090 `_Copy_impl<CComplexNumber*,...>` is __unguarded_insertion_sort_aux<pair<long,C3DMeshVolume>*>.
  - 0x00A71880 `QuickPartition_WithHeapPhase` / 0x00A71910 `Introsort_QuickPartition_Str` are __unguarded_partition
    (long / CCharString key).
  - 0x00A71BE0 `std_Adjust_Heap_Pair` is __pop_heap<pair<CCharString,C3DMeshVolume>*>.
  - 0x00A71CA0 / 0x00A71D40 `Heap_BuildHeap_*Key` are __make_heap.
  - 0x00A728F0 `Vector_InsertElement_ComplexValue` / 0x00A729B0 `Vector_InsertElement_StringPair` are __linear_insert.
  - 0x00A73240 / 0x00A732B0 `Vector_ReserveLog2_*_Impl` are __insertion_sort.
- 0x00A6B1A0 / 0x00A6B420 / 0x00A6B480 / 0x00A6B820 / 0x00A6BBB0 (`*_u64Pair`) are the STLport sort helpers named above
  over pair<unsigned long, unsigned long>.
- 0x00A6ECE0 `_For_each<...>` is STLport `for_each`.
- Labels of already-landed functions:
  - 0x00A6F290 (landed as `CVectorMap<CCharString,CNavLineMapValue,...>::LowerBound`) is also the HelperPoints
    LowerBound (C3DHelperPoint, 20-byte entries) for caller 0x00A6C9C0. The body is shared (folded) by the two
    20-byte maps.
  - 0x00A70A10 (landed as `CopyBackward_PairULongMeshVolume`) is reached from 0x00A728F0, whose key compare is signed
    (`jge`). The element is pair<long, C3DMeshVolume> (NavigationContainmentVolumes), not unsigned long.
