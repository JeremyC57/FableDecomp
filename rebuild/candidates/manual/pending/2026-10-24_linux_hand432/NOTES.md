# Hand chunk 432 (CEngineStaticMapFileBlockManager .. __aullrem): 31 of 40 byte-exact

## What matched (31)

- **CEngineStaticMapFileBlockManager / its STLport map<long, CFileBlock>** (0xBE05D0 Reset, 0xBE0A50 `_Rb_tree::erase(iterator)`,
  0xBE0B20 `_Rb_tree::_M_insert`): the map clear loop is `next = it; ++next; erase(it); it = next;` (the current node stays in
  ecx and the copy advances in esi), then `AsyncBankEntry.Reset()`. The tree is a full STLport 4.x model in `namespace _STL`:
  node value at +0x10, `_M_header` pointer, `_Rebalance` @ 0x0042971D and `_Rebalance_for_erase` @ 0x0042A34E as `__cdecl`
  statics, nodes freed with `if (p) free(p)`. Explicit `template class` instantiation lets VC inline `_M_create_node` in the
  left branch of `_M_insert` and call it out of line in the right branch, which is what retail does.
- **CArray<T>::push_back** (0xBE4F90 water vertex, 0xBE5950 sea vertex): STLport `if (finish != eos) { _Construct; ++finish; }
  else _M_insert_overflow(finish, x, __false_type(), 1, true)`. The tag has no user constructor here, so VC zero-stores it into
  the `push ecx` frame byte.
- **CLandscapeBackgroundPatch / CLandscapeLayerMesh::SetTargetVertexNormal** (0xBE66F0 static __fastcall, 0xBFDEC0 member):
  11:11:10 unpack with sign extension. The three products build a NAMED C3DVector local through its 3-float ctor and are then
  copied member-wise. This gives z through the frame word plus the `fxch st(2)` / 3-`fstp` shape.
- **CEngineInternalPrimitiveMeshBase::GetMaxDrawDistance** (0xBEB660): the distance is read into a local before the flag test,
  which hoists the `fld`. Writing `member * global` inline loads the global first.
- **CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap** (0xBEBBE0): an if/else max of the frame sides into an
  `unsigned long` (one movzx per arm), then a separate level counter, so the counter is not the parameter register.
- **CEngineLandscapePatch ctor** (0xBF3A10), **__uninitialized_fill_n of a 28-byte record** (0xBF5BB0, descriptive name, same
  model as the landed 0xBF5B50), **GFDataIsInsideBlock<C4DVector*>** (0xBFB750): straight bodies.
- **STLport sort / heap / vector helpers of CVectorMap<long, CCompositeSurfaceInfo>** (element A: 16-byte
  `pair<long, CCompositeSurfaceInfo>`; vptr 0x012A8154 plus a CCountedPointer). These were all verbatim STLport 4.6 bodies as
  `/Gr` templates in `namespace _STL` with explicit instantiation:
  - 0xBED790 `__unguarded_linear_insert`, 0xBEDD20 `__unguarded_insertion_sort_aux` (comparator declaration-only),
    0xBEDFE0 `__unguarded_partition` (out-of-line `iter_swap`), 0xBEE140 `__make_heap`, 0xBEE8F0 `__linear_insert` (VC
    calls the pair's implicit operator= 0xBED640 out of line by itself), 0xBEECD0 `__insertion_sort`, 0xBEED30
    `__partial_sort`, 0xBEF1D0 `__introsort_loop` (inline `__median` and `partial_sort`, threshold 16 shown as
    `and 0xfffffff0 ; cmp 0x100`).
  - 0xBEEF90 `vector::_M_insert_overflow`: same-TU noinline `__uninitialized_copy` / `__uninitialized_fill_n` bodies.
  - 0xBEEB80 `vector::reserve`: needs `/Gr`, plus a same-TU noinline `__uninitialized_copy` so the new block lands in the
    dead n slot.
  - 0xBEF440 `CVectorMap::Add`: `push_back(pair(key, value)); Dirty = true;`, with the STLport pair (a, b) ctor.
- **The same family for CVectorMap<pair<long, long>, CEngineTextureDecal>** (element B: 28 bytes; the decal's
  vector<CCompositeBlendDesc> is 12 bytes, with operator= at 0x006CADE0 and copy ctor at 0x005BB346 out of line):
  - 0xBEEC60 `__unguarded_partition`, 0xBEE840 `__unguarded_insertion_sort_aux`, 0xBEEE40 `__make_heap` and 0xBEF640
    `__insertion_sort`: verbatim.
  - 0xBEE280 `iter_swap`: needs `__forceinline __iter_swap`; otherwise VC keeps it out of line.
  - 0xBEE660 `__uninitialized_copy`: needs STLport's explicit memberwise pair copy ctor and a same-TU body for the blend
    copy 0x005BAF28.
  - 0xBEF830 `_M_insert_overflow`: `_M_clear` out of line (0xBEF580).
  - 0xBEE320 `__unguarded_linear_insert`: matched only with the key read into a local, a (key, entry) comparator overload
    and `last->first = key; last->second = val.second` (retail keeps the key words in ebx/ebp).

## Parked for Fable 5.1 (9)

- 00be6800 CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex DIFFER(82v113): each component is rounded by the bare `fstp [esp] ; fld [esp] ; fistp dword [esp+8]` ROUND helper; C++ `(long)x` calls __ftol2 | tried: C++ reference with (long) conversions | next: retail_asm grade (same helper as the landed 0x00BE6790 PackVertexNormal)
- 00bfdfd0 CLandscapeLayerMesh::GetPackedNormalFromTargetVertex DIFFER(88v123): the same bare dword fistp ROUND helper three times | tried: C++ reference with (long) conversions | next: retail_asm grade (as 0x00BFDF60)
- 00bec8d0 CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials DIFFER(126v127): retail keeps the cached MaterialCount in ebp and spills &HiddenMaterialNames to a frame word (reloaded for the _M_find `this` and the end() compare); ours keeps &set in ebp and spills the count | tried: node-pointer find, iterator compare, explicit iterator local, nested set/_Rb_tree inline find/end | next: an inline `IsMaterialHidden(name)` helper or the count as `HiddenMaterials.size()`; otherwise lane gap: register allocation
- 00bef2b0 _STL::__linear_insert (decal entries) DIFFER(158v156): retail holds key.first in ebx and key.second in ebp from entry; the key-local model puts key.second in ebx and splits key.first across ecx/ebp, so the else-branch copy loads OriginalTextureIndex/DecalID after the key stores | tried: key local + T(key, val.second) (best), T(val) rebuild, verbatim with a user CEngineTextureDecal::operator=, implicit key-pair copy | next: find the source form that makes VC7.1 enregister a by-value pair's key across the vector operator= call (it does so in 0xBEE320/0xBEE420 too); lane gap: register allocation
- 00bfe9c8 report_failure DIFFER(0v48): MSVC CRT seccook.c (`__SEH_prolog`, `__security_error_handler`, noreturn `_exit`), hand-written CRT object | tried: none (CRT tail) | next: lane gap: CRT library object
- 00bfea30 __chkstk DIFFER(0v61): MSVC CRT chkstk.asm | tried: none | next: lane gap: hand asm CRT
- 00bfea70 __ftol2 DIFFER(0v117): MSVC CRT ftol2.asm | tried: none | next: lane gap: hand asm CRT
- 00bfeb50 __allmul DIFFER(0v52): MSVC CRT llmul.asm | tried: none | next: lane gap: hand asm CRT
- 00bfebb0 __aullrem DIFFER(0v117): MSVC CRT ullrem.asm | tried: none | next: lane gap: hand asm CRT

## Skipped

None. Every row holds one function. The CRT tail (0xBFE9C8 .. 0xBFEBB0) is parked above as library objects, not skipped.

## Label corrections

These manifest labels were propagated or descriptive. The retail identity is given on the right.

- 0x00BE05D0 BankFileEntry_Destroy -> CEngineStaticMapFileBlockManager::Reset (its caller 0x00B42A90 "~CBankFileEntryUpdateData" is propagated too)
- 0x00BE0A50 CMemoryAllocatorVariableSize_Erase_Destructor -> _STL::_Rb_tree<long, pair<const long, CEngineStaticMapFileBlockManager::CFileBlock>>::erase(iterator) (0x00BE0B00 is the map::erase forwarder)
- 0x00BE0B20 predicate_tree_insert_node -> the same tree's _M_insert
- 0x00BE4F90 Vector_PushBack_14Elements / 0x00BE5950 Vector_PushBack_3Floats -> CArray<CTVertexWaterBackground>::push_back / CArray<CTVertexSeaBackground>::push_back
- 0x00BEB660 CEnginePrimitive_ComputeFadeDistance -> CEngineInternalPrimitiveMeshBase::GetMaxDrawDistance
- 0x00BEC8D0 CEngineInternalPrimitive_UpdateVisibilityVector -> CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials(const C3DMesh2&)
- 0x00BED790 CNavigationPosition_CopyRange -> _STL::__unguarded_linear_insert<pair<long, CCompositeSurfaceInfo>*>
- 0x00BEDD20 _Copy_impl<CNavigationPosition*,...> -> _STL::__unguarded_insertion_sort_aux (same element)
- 0x00BEDFE0 Std_HeapPartition -> _STL::__unguarded_partition (same element)
- 0x00BEE140 Std_Make_Heap_SiftDown -> _STL::__make_heap (same element)
- 0x00BEE280 swap<CEngineTextureDecal> -> _STL::iter_swap<pair<pair<long,long>, CEngineTextureDecal>*>
- 0x00BEE320 std_vector_insert_sorted -> _STL::__unguarded_linear_insert (decal entries)
- 0x00BEE660 CArray_CopyConstruct -> _STL::__uninitialized_copy (decal entries)
- 0x00BEE840 Std_Vector_PreallocAndCopy -> _STL::__unguarded_insertion_sort_aux (decal entries)
- 0x00BEE8F0 Std_Vector_InsertWithMove -> _STL::__linear_insert (surface entries)
- 0x00BEEB80 Std_Vector_ReallocateForComposite -> _STL::vector<pair<long, CCompositeSurfaceInfo>>::reserve
- 0x00BEEC60 Std_HeapPartitionForDecals -> _STL::__unguarded_partition (decal entries)
- 0x00BEECD0 std_vector_destroy_with_callback -> _STL::__insertion_sort (surface entries)
- 0x00BEED30 Std_Heap_InsertAndSort -> _STL::__partial_sort (surface entries)
- 0x00BEEE40 Std_Make_Heap_SiftDownDecals -> _STL::__make_heap (decal entries)
- 0x00BEEF90 CCompositeSurfaceMap_Reallocate -> _STL::vector<pair<long, CCompositeSurfaceInfo>>::_M_insert_overflow
- 0x00BEF1D0 std_make_heap_build -> _STL::__introsort_loop (surface entries)
- 0x00BEF2B0 Std_Vector_InsertWithCompare -> _STL::__linear_insert (decal entries)
- 0x00BEF440 CCompositeSurfaceMap_PushBack -> CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<...>>::Add
- 0x00BEF640 std_make_heap_build_array -> _STL::__insertion_sort (decal entries)
- 0x00BEF830 CRandomAppearanceMorph_Reallocate -> _STL::vector<pair<pair<long,long>, CEngineTextureDecal>>::_M_insert_overflow
- 0x00BF3A10 NHeroInformationScreens_Initialize -> CEngineLandscapePatch::CEngineLandscapePatch() (vtable 0x012A8200)
- 0x00BF5BB0 fill<C3DAnimationStateAlpha*,...> -> __uninitialized_fill_n of a 28-byte record (caller 0x00BF5EE0 _M_insert_overflow; descriptive name)
- Already landed, wrong identity:
  - 0x00BEE710 "CRandomAppearanceMorph::UninitializedFillN" (CMorphRecord) is _STL::__uninitialized_fill_n over the decal entries (called by 0x00BEF830).
  - 0x00BED9A0 "CopyBackward_PairLongPointerHolder" is __copy_backward over pair<long, CCompositeSurfaceInfo>.
  - 0x00BEDD70 (file named for pair<long, CCountedPointer<CNavConnectorNode>>) is __adjust_heap over pair<long, CCompositeSurfaceInfo>.
  - 0x00BEF5C0 "Sort_SElement16" is _STL::sort over the CVectorMap<long, CCompositeSurfaceInfo> entries.
