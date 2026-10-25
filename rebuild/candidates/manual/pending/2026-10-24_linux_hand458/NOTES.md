# Hand chunk 458 (std .. std): 40 of 40 byte-exact

All 40 rows are STLport 4.x instantiations (namespace `std`): sort/heap algorithms over five keyed vectors, plus a
handful of vector/bitset/uninitialized helpers. Every source is a real template with an explicit instantiation;
the /Gr algorithm TUs get an explicit `__fastcall` on the template definition, so no row needs a `// Flags:` line.
One row (004dd80b) uses `#pragma optimize("s", on)`. Grades: 6 MATCH, 34 RELOCATION_MATCH. All 40 tests PASS
under g++ and under VC7.1. calls.py confirms every call target: malloc 0xBFEA0E, free 0xBFEA14, memmove 0xBFEAE6,
puts 0xBFEAF2, abort 0xBFEAEC, and the CCharString thunks 0x99EC30/0x99EAE0/0x99EFB0. Each leaf names exactly one
defined function in its object, and each object's function size equals the retail row's size.

## What matched (40)
- **NGraphicAppearance::CMeshInfo, sorted by CSortPrimaryMeshesFirst / CSortPrimaryMeshesLast (8 rows):**
  - Rows: 004c3860 / 004c38f0 `__unguarded_partition`, 004c3ab0 / 004c3b60 `__make_heap`, 004c5220 / 004c52d0
    `__partial_sort`, 004c59b0 / 004c5aa0 `__introsort_loop`.
  - What it does: puts a graphic appearance's primary meshes first, or last.
  - Shape: the PDB comparators are empty functors that read only the left operand (`return a.PrimaryMesh;` and
    `return !a.PrimaryMesh;`). That makes `comp(pivot, *last)` loop-invariant, so retail's right-hand scan is a
    `jmp $` (`eb fe`) self-loop and the pivot's bit byte stays in a dead argument slot. make_heap and `__pop_heap`
    are inline in `__partial_sort` for the 12-byte POD element.
  - Behaviour hazard: retail never terminates when a partition's median-of-three pivot is primary (First) or
    non-primary (Last). An introsort over more than 16 meshes can therefore hang; shorter lists use only the
    insertion sort. The tests use pivots that terminate, and an independent Python port of the algorithms
    supplies the expected orders.
- **CThing::Components, a `CVectorMap<ETCInterfaceType, CTCBase*>` (7 rows):**
  - Rows: 004cb5e0 `__unguarded_linear_insert`, 004cd0c0 `__linear_insert`, 004cc690 `__unguarded_partition`,
    004cd370 `__partial_sort`, 004cd500 `__introsort_loop`, 004cbbf0 `vector::_M_allocate_and_copy<const pair*>`,
    004cc9b0 `vector::reserve`.
  - Shape: the interface-type key is a signed enum, so the compares are `jl`/`jge`.
  - In `__linear_insert`, copy_backward is inline through `__copy_backward(random_access_iterator_tag)`, and the
    tag has a user constructor.
  - reserve inlines allocate and free with their null tests and calls `_M_allocate_and_copy` out of line.
  - The pair carries STLport's converting template constructor next to its copy constructor.
- **`_Vector_base<unsigned long>::_Vector_base(n, a)`, 004cd9d0:** the real `_Vector_base` over an
  `_STLP_alloc_proxy`, with allocate guarded by `n != 0`.
- **`bitset<274>::set` and `bitset<274>::reset`, 004cc090 / 004cc0d0:** the STLport bitset with exceptions off.
  The range error is `puts("bitset"); abort();`, with `abort` declared `__declspec(noreturn)`. The two bodies come
  from the same text and differ only in scheduling.
- **NQuestManager::CRuntimeQuestPersistence::PersistScripts, a `CVectorMap<CCharString, CPersistInfo>` (4 rows):**
  - Rows: 004bafb0 `__insertion_sort`, 004bb810 `__partial_sort`, 004bb6b0 `CVectorMap::DeallocateMemory`,
    004bb780 `vector::push_back`.
  - Shape: CCharString::operator< is inline with its null tests, and the real CBasicString<char>::operator<
    (0x429950) sits in the same TU under `auto_inline(off)`.
  - `__pop_heap` stays out of line (0x4BABF0) for this 12-byte non-POD pair.
  - DeallocateMemory is `clear()` followed by a swap with an empty vector. `__copy` (0x4B84F0) and `__destroy_aux`
    (0x4B93E0) stay out of line; free is inline.
  - push_back is the STLport fast path, with `_M_insert_overflow` (0x4BB400) out of line.
- **CSubtitledMessage tag maps (13 rows):**
  - AttitudeTags `pair<float, EConversationAttitude>` (4): 004d0170, 004d0bf0, 004d1b40, 004d2160.
  - CameraTags `pair<float, CCameraTag>`, 28 bytes (3): 004d0330, 004d0c90, 004d1c80.
  - AnimationTags `pair<float, CCharString>` (6): 004cfcf0, 004d0220, 004d0c40, 004d14e0, 004d1bc0, 004d2240.
  - Shape: float keys compare with `fld ; fcomp ; fnstsw ; test ah,5`.
  - The pair copy constructor moves the float through the FPU in `__partial_sort`'s by-value `T(*i)` but as a dword
    in `__make_heap`, from the same source.
  - `__pop_heap` is inline for the 8-byte pairs. For the CCharString pair, the `T(*i)` local gets an `and esp,-8`
    frame. For the 28-byte camera pair `__pop_heap` is out of line (0x4D0B50).
  - For the camera pair, the implicit operator= is `mov ecx,7 ; rep movsd` while the copy constructor unrolls seven
    dword moves.
- **CTCBase's registry, a `CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>`, 004dd80b `__unguarded_linear_insert`:**
  - The row is unaligned because its TU is size-optimised. It needs `#pragma optimize("s", on)` plus
    `__forceinline` on both CCharString::operator< and the comparator's operator(). Without the `__forceinline`
    VC7.1 builds an ebp frame and calls the comparator (DIFFER 69v85).
  - The implicit pair operator= stays out of line (0x4DA56A).
- **`vector<set<unsigned long> >::resize(n)`, 0051d240:** STLport's `resize(n, _Tp())`. The whole temporary set is
  inline: the `_Rb_tree_base` header malloc(0x14), `_M_empty_initialize`, `clear()` through the out-of-line
  `_M_erase` (0x4A9CB0), and free. The landed `_Rb_tree` model of 0x00AD9120 matched as is.
- **`vector<unsigned char>::operator=`, 005295b0:** STLport verbatim with the `__true_type` paths (memmove through
  `__copy_trivial`). It is adapted from the landed `vector<unsigned short>::operator=` 0x00AADE80.
- **NThingManager::CEntityRuntimePersistence::CEntityScriptInfo raw-storage builders (2 rows):**
  - Rows: 0052b510 `__uninitialized_fill_n` and 0052bb40 `__uninitialized_copy<const T*, T*>`.
  - Shape: the implicit copy constructor inlines the CCharString copy and the SaveData vector's copy constructor
    (`_Vector_base(n, a)` plus memmove).
  - 0052bb40 is a byte twin of the landed 0x0052BBE0.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections
- **Catalog row names (all descriptive or wrong).** The real STLport names are on line 1 of each source.
  - "Introsort_Recursive" 004bb810 is `__partial_sort`.
  - "vector_ProcessElements" 004bafb0 and "vector_ForEach_Apply_Callback" 004d14e0 are `__insertion_sort`.
  - "heapsort_SiftDown" 004cc690 is `__unguarded_partition`.
  - "heapsort_Recursive" 004cd500 is `__introsort_loop`.
  - "_Insertion_sort_BinaryInsert" 004cb5e0 is `__unguarded_linear_insert`.
  - "introsort_PopHeap_OnPartition_Float" 004cd370 is `__partial_sort`; its key is a signed enum, not a float.
  - "make_heap_CDebugBox" 004d0c90 and "introsort_Heapsort_Top_CDebugCone" 004d1c80 are the CSubtitledMessage
    camera-tag instantiations.
  - "set_Insert_Sorted" 004dd80b is `__unguarded_linear_insert` over the CRegisteredTCInfo map.
  - "vector_Construct_FromTreeTraversal" 0051d240 is `vector<set<unsigned long> >::resize(n)`.
  - "__init_range_from_CCharString" 0052b510 is `__uninitialized_fill_n<CEntityScriptInfo>`.
  - "__init_range_CCharString_reverse" 0052bb40 is `__uninitialized_copy<const CEntityScriptInfo*>`.
- **Manifest callee labels propagated across byte twins:**

  | Address | Current label | Real function |
  |---|---|---|
  | 0x0099EAE0 | `_Cons_val<allocator<pair<EHeroMorphType,...>>>` | `CCharString::~CCharString` |
  | 0x004A9CB0 | `LTextTreeWalkThrough::BuildTreeArray` | `_Rb_tree<unsigned long, ...>::_M_erase` |
  | 0x0051D030 | `vector<CUIMiniMapMarker>::resize` | `vector<set<unsigned long> >::resize(n, const set&)` |
  | 0x004DA56A | `pair<CCharString,CCharString>` | implicit `pair<CCharString, CTCBase::CRegisteredTCInfo>::operator=` |
  | 0x004D78C5 | `CRegisteredTCInfo` (a constructor name) | implicit `CTCBase::CRegisteredTCInfo::operator=` (two dwords, then CCharString::operator=) |
  | 0x004D0B50 | `_Pair_base<CDateAndTime const,CWideString>` | `__pop_heap<pair<float, CCameraTag>*>` |
  | 0x004D0660 | `_Adjust_heap<CDebugBox*>` | `__adjust_heap<pair<float, CCameraTag>*>` |
  | 0x004D16A0 | `_Sort_heap<CDebugCone*>` | `sort_heap<pair<float, CCameraTag>*>` |
  | 0x004D15A0 | `_Sort_heap<pair<long,unsigned long>*>` | `sort_heap<pair<float, EConversationAttitude>*>` |
  | 0x004CBED0 | `_Adjust_heap<pair<float,EConversationAttitude>*>` | `__adjust_heap<pair<ETCInterfaceType, CTCBase*>*>` |
  | 0x004CD130 | `_Sort_heap<pair<long,unsigned long>*>` | `sort_heap<pair<ETCInterfaceType, CTCBase*>*>` |
  | 0x004C2C70 / 0x004C2D10 | `_Adjust_heap<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*>` | `__adjust_heap<CMeshInfo*>` with CSortPrimaryMeshesFirst / Last |
  | 0x004C4B30 / 0x004C4BD0 | `_Sort_heap<pair<long, CMusicManager::CRegisteredMusicEntry>*>` | `sort_heap<CMeshInfo*>` with First / Last |
  | 0x004BB570 | `_Sort_heap<pair<unsigned long, CSmallVector<...>>*>` | `sort_heap<pair<CCharString, CPersistInfo>*>` |
  | 0x004B84F0 | `_Move<...>` | STLport random-access `__copy` |
  | 0x004B93E0 | `_Destroy` | `__destroy_aux` |
  | 0x004BAB30 | unnamed | `__linear_insert<pair<CCharString, CPersistInfo>*>` |
  | 0x004BAC90 | unnamed | `__make_heap<pair<CCharString, CPersistInfo>*>` |
  | 0x004CC740 | unnamed | `__make_heap<pair<ETCInterfaceType, CTCBase*>*>` |
  | 0x004D0F80 | unnamed | `__linear_insert<pair<float, CCharString>*>` |

- **Already landed, 0x0052BBE0:** it is named `__uninitialized_copy<const CEntityScriptInfo*, CEntityScriptInfo*>`,
  but its callers make it the `<T*, T*>` instantiation: `_M_insert_overflow` 0x0052C150 and `_M_fill_insert`
  0x0052C300. The `<const T*, T*>` one is 0x0052BB40, whose callers are `_M_allocate_and_copy` 0x0052BE50 and
  operator= 0x0052BFF0.
