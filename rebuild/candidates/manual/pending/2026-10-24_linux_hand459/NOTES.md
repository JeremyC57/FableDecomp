# Hand chunk 459 (std .. std): 40 of 40 byte-exact

All 40 rows are STLport 4.6 helpers (namespace `std`, algorithm templates `__fastcall` under /Gr, members thiscall).
gall.sh: 12 MATCH + 28 RELOCATION_MATCH; tall.sh (g++) and tall.py (VC7.1) 40/40 PASS; calls.py shows no import
mismatch (malloc 0xBFEA0E / free 0xBFEA14 / memmove 0xBFEAE6 / operator delete 0xBFE9BC as retail). Every
`// Leaf:` was checked against `objdump -t`: exactly one symbol per object, the intended one.

## What matched (40)

### Red-black tree / map
- 0x00534F20 `std::__find` (set<NUISystem::CComponent*> iterators): input-iterator find; operator++ is the static
  `_Rb_global<bool>::_M_increment(node)` walk; the iterator class has user ctors, so the result goes through the
  hidden pointer in ecx and &val takes edx.
- 0x0053CB9B `_Rb_tree::_Rb_tree(const _Rb_tree&)` (map<unsigned long, NUISystem::CUIState>): `optimize("s", on)` TU,
  `__forceinline` `_Rb_tree_base` ctor (header word zeroed before malloc(0x38)), out-of-line `_S_minimum/_S_maximum`.

### vector members
- 0x0053CB4F `vector<unsigned char>::push_back`: frame-pointer size TU (`optimize("y", off)` + `optimize("s", on)`).
- 0x00673E10 / 0x00708830 vector copy constructors (pair<unsigned long, CTextureMorph>, CTCHeroAttachableAppearanceModifiers::
  CModifierEntry): real `_Vector_base(n, a)` + `_STLP_alloc_proxy`, `__forceinline __uninitialized_copy` (STLport's
  default traits make every user class non-POD: the null-tested placement loop, never memmove).
- 0x006562E7 `vector<CIntelligentPointer<CThing> >::_M_fill_insert`: size TU + same-TU noinline bodies of the three
  tag-taking helpers (tags homed in the live `position` slot).
- 0x006860F0 `vector<bool>::_M_fill_assign` (a byte vector in retail): noinline stand-ins of `_Vector_base(n, a)`
  0x00450CCC and `vector(n, val, a)` 0x00451583 keep the swap temporary uncaptured.
- 0x006A3C70 `vector<CScreenFilterSThingByPass>::push_back` (9-byte packed {CIntelligentPointer<CThing>, bool}) and
  0x00723270 `vector<CTCSmokeGenerator::CGenerationPos>::push_back` ({C3DVector Pos; CIntelligentPointer<CThing> PEmitter}):
  inline `CIntelligentPointer(const&) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }`, the overflow
  call's __false_type in the dead `&x` slot; 0x00723270 needs a user C3DVector copy ctor (implicit: 109v105).
- 0x00721060 / 0x00728D10 `_M_insert_overflow` (pair<long, CTCHeroMorph::CTextureLayer>; CTCAppearanceMorphBase::
  CMeshMorphSet::CMeshMorphSetParts): STLport text as the landed 0x00BEF830; both __false_type temporaries live in the
  atend slot only when the out-of-line helpers (__uninitialized_copy / __uninitialized_fill_n, and __destroy_aux in
  0x00728D10) have same-TU noinline bodies (declarations only: `sub esp,8`, 230v226).
- 0x00841FC0 `vector<basic_string<char> >::erase(first, last)`: `__copy_ptrs` -> out-of-line `__copy` whose tag is
  homed on the LIVE `first` slot (needs the __copy stand-in), inline _Destroy loop + string dtor (`if (p) free(p)`).

### Uninitialized helpers / copies
- 0x005657C0 `__uninitialized_fill_n<CTCHeroOnlineScoreboard::CActivity*>`, 0x008656B0 `__uninitialized_fill_n
  <NCreatureHitNotification::CFlash*>` (inline STLport list copy ctor: `_List_base(a)` sentinel malloc(0x24) and
  `insert(begin(), x.begin(), x.end())` -> `_M_insert_dispatch` with the iterator built in its arg slot and the tag in
  the dead `x` slot; matched first grade with the split `_Nonconst_traits` / `_Const_traits` iterators).
- 0x00569140 `basic_string::_M_assign_dispatch<char*>`; 0x00673760 `__copy_ptrs` of a 44-byte record (`__forceinline
  __copy`, rep movsd).

### Sort / heap / search chains (CKeyPairCompareLess vector maps and others)
- ENavigatorType: 0x006721C0 `__linear_insert`, 0x00672240 `__introsort_loop` (`__forceinline __unguarded_partition`).
- CTestQuest: 0x006251C0 `__partial_sort` (same-TU noinline comparator keeps the functor word in esi).
- CDegradableInfo: 0x0067E9B0 `__unguarded_partition`, 0x0067EAD0 `__make_heap`.
- CSimVoice voice pairs: 0x00680400, 0x00680B40, 0x00680CB0, 0x006810E0 (`__forceinline __pop_heap`), 0x00681270.
- CRandomAppearanceMorph texture morphs: 0x0086A640 `__unguarded_linear_insert` (unsigned key).
- CRandomAppearanceMorph skeletal morphs (PDB {GroupID, CDefString Name, bool AllowVariation}): 0x0086A6E0, 0x0086B3B0,
  0x0086B5C0, 0x0086D160, 0x0086DB30 -- all STLport verbatim, first grade.
- CExpressionSet entries (pair<EExpressionType, CExpressionSet::CEntry>): 0x0086F090, 0x0086F820, 0x0086FEB0.
- 0x00711640 `__inplace_stable_sort` (list<CAnimSet>::const_iterator by priority): `< 15` insertion sort, halves,
  `__merge_without_buffer`.
- 0x00695E30 `__find_if` / 0x00696240 `remove_copy_if` (CCountedPointer<CActionEventBase>, CIsActionEventFinished).
- 0x00841640 `__find_if<const char*, _Eq_char_bound<char_traits<char> > >`: the by-value 1-byte predicate (`mov bl,[esp+8]`).
- 0x006984A0 `CGameCameraManager::RemoveCameraPoint(CThing&)`: find of a CIntelligentPointer temporary (virtual dtor
  slot 0), list erase with `_Destroy` + free; noinline `__find` stand-in.

## Parked for Fable 5.1 (0)

## Skipped
None (no row holds two functions).

## Label corrections
Catalog labels (all descriptive or propagated) -> STLport names:
- 0x00534F20 __find_in_tree_by_key -> `std::__find` (set<CComponent*> iterators)
- 0x0053CB4F deque_push_back_or_allocate -> `vector<unsigned char>::push_back`
- 0x0053CB9B list_construct_from_list -> `_Rb_tree` copy ctor of map<unsigned long, NUISystem::CUIState>
- 0x005657C0 __init_range_fixed_struct -> `__uninitialized_fill_n<CTCHeroOnlineScoreboard::CActivity*>`
- 0x00569140 string_InsertRange -> `basic_string::_M_assign_dispatch<char*>`
- 0x006251C0 introsort_partition_impl -> `__partial_sort<CTestQuest*>`
- 0x006562E7 vector_insert_animationentry -> `vector<CIntelligentPointer<CThing> >::_M_fill_insert`
- 0x006721C0 InsertSorted_Pair -> `__linear_insert`; 0x00672240 IntroSort -> `__introsort_loop` (ENavigatorType)
- 0x00673760 copy_range<44byte_element> -> `__copy_ptrs`; 0x00673E10 vector_CopyRange -> vector copy ctor
- 0x0067E9B0 partition_pivot -> `__unguarded_partition`; 0x0067EAD0 make_heap<CDebugBox> -> `__make_heap<CDegradableInfo*>`
- 0x00680400 lower_bound_insert, 0x00680B40 partition_pairs, 0x00680CB0 insertion_sort_pairs, 0x006810E0
  partial_sort_heap, 0x00681270 IntroSort -> `__unguarded_linear_insert / __unguarded_partition / __linear_insert /
  __partial_sort / __introsort_loop` (CSimVoice voice pairs)
- 0x006860F0 vector_insert_value -> `vector<bool>::_M_fill_assign`
- 0x00695E30 find_if_nonzero -> `__find_if`; 0x00696240 vector_copy_filtered -> `remove_copy_if`
- 0x006984A0 list_erase_value -> `CGameCameraManager::RemoveCameraPoint` (Ego_r.pdb name)
- 0x006A3C70 vector_push_back_CBaseIntelligentPointer -> `vector<CScreenFilterSThingByPass>::push_back`
- 0x00708830 vector_InitFromRange -> `vector<CModifierEntry>` copy ctor
- 0x00711640 introsort_CAnimSet -> `__inplace_stable_sort` (not an introsort)
- 0x00721060 vector_InsertRangeComplex, 0x00728D10 vector_InsertAndGrow_Pair -> `vector::_M_insert_overflow`
- 0x00723270 vector_push_back_or_grow -> `vector<CTCSmokeGenerator::CGenerationPos>::push_back`
- 0x00841640 find<char*,char> -> `__find_if<const char*, _Eq_char_bound<char_traits<char> > >`
- 0x00841FC0 vector_move_and_cleanup -> `vector<basic_string<char> >::erase(first, last)`
- 0x008656B0 vector_InitializePairAnimationNodes -> `__uninitialized_fill_n<NCreatureHitNotification::CFlash*>`
- 0x0086A640 / 0x0086F090 pair_insertion_sort_shift, 0x0086A6E0 insertion_sort_shift_16byte -> `__unguarded_linear_insert`
- 0x0086B3B0 introsort_4way_partition_int, 0x0086F820 introsort_partition_3way_12byte -> `__unguarded_partition`
- 0x0086B5C0 make_heap_CTextureMorphEntry_16byte -> `__make_heap` (skeletal-morph pairs, not texture morphs)
- 0x0086D160 partial_sort_top_k_CSkeletalMorph, 0x0086FEB0 introsort_3way_partial_sort -> `__partial_sort`
- 0x0086DB30 introsort_4way_partition -> `__introsort_loop`

Already-landed sources whose names look wrong (found while naming callers/callees):
- 0x0065572D landed as "_Destroy_range": it is `std::fill(first, last, const T&)` (its loop calls operator= 0x004A9760).
- 0x0086C620 landed as CCompressorZlib::SetupStream (its own header says propagated): `__linear_insert` of the
  skeletal-morph map.
- 0x00722130 DestroyAux_SNestedVectorRecord10, 0x00728A40 / 0x00728B10 / 0x00728B50 SPairLongCSecond_005bbdf1: the element
  is CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts; 0x005BBDF1 CRetailVector20_005bbdf1 is
  CArray<CEngineTextureMorph>'s copy ctor (as 0x00709E60 already says).
- 0x00841DE0 Copy_RetailString_00841de0 = `std::__copy<const basic_string*, basic_string*, int>`.
- 0x00865650 UninitCopy_ListLongPair_00865650 = `__uninitialized_copy<NCreatureHitNotification::CFlash*>`.
- 0x0086F3E0 / 0x0086FA00 / 0x0086F600 / 0x008700A0 / 0x0086EFF0 (pair<long, CLongMapValue8>, Element_008700a0): the
  map is CExpressionSet::Expressions = CVectorMap<EExpressionType, CExpressionSet::CEntry> (see 0x0086F090's header).
- 0x0086AF60 models CRandomAppearanceMorph::CSkeletalMorph as {GroupID, Words[2]}; Ego_r.pdb has {GroupID, CDefString
  Name, bool AllowVariation} (same bytes).
