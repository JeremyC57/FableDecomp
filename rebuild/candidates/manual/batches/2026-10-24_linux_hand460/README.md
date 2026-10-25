# Hand chunk 460 (std .. std::list): 39 of 40 byte-exact

All 39 matched sources grade MATCH / RELOCATION_MATCH through gall.sh (grades.txt), every test prints
`<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and calls.py shows no import mismatch (STLport nodes use
malloc 0xBFEA0E / free 0xBFEA14, the map release calls operator delete 0xBFE9BC as retail). Most rows are STLport 4.6
algorithm or container instantiations, written as templates in namespace `_STL` under their real STLport names; the
catalog labels were mostly descriptive or propagated (see Label corrections).

## What matched (39)

- **STLport sort family, CTurningAnimCompare** (anonymous-namespace comparator of the
  CCreatureAction_InterruptableMidAttackAutoTurn TU, `vector<const CAnimationEntry*>`): 0x0087DFA0
  `__unguarded_partition`, 0x0087E370 `__insertion_sort`, 0x0087E400 `__final_insertion_sort`, 0x0087E450
  `__partial_sort`, 0x0087E610 `sort`. The comparator class lives inside `namespace { }`, so the leaves are prefixes
  (`??$__partial_sort@` ...). The comparator is inlined, and retail reads its left operand twice. In
  `__final_insertion_sort` the unguarded insertion sort and its aux are inline. `__lg` is inline in sort.
- **CAIBrain::StateGroupList sort** (`pair<long, CAIStateGroupBase*>`, CKeyPairCompareGreater `a.first > b.first`):
  0x0088C970 `__unguarded_partition`, 0x0088CD30 `__linear_insert`, 0x0088CFD0 `__partial_sort`, 0x0088D060
  `__introsort_loop`. The pair has STLport's explicit copy constructor, so by-value pivots and values are rebuilt in
  their argument slots (`sub esp,8 ; mov eax,esp`). `copy_backward` is the counted random-access `__copy_backward`
  loop, inlined.
- **CRC-keyed CVectorMap sorts**: 0x009B7ED0 / 0x009B7FA0 `__introsort_loop` over `pair<unsigned long, long>` /
  `pair<unsigned long, unsigned long>`, CKeyPairCompareLess. These are byte twins of 0x0088D060 with unsigned
  `jae/jb`.
- **Definition-manager templates TU**:
  - 0x009B8440 two-argument `sort<CWideString*>`. STLport passes `__less(__value_type(first))`, and `less<>` must
    derive from `binary_function`, otherwise VC zero-initialises the temporary.
  - 0x009B8B50 `__introsort_loop` over `pair<CCharString, unsigned long>`. Here `__median` is declared out of line
    (0x9B5730).
  - 0x009B8F70 `__insertion_sort` over `pair<unsigned long, CDefClassInfo>`.
  - 0x009B9170 `vector<pair<unsigned long, CDefClassInfo> >::push_back`. `_M_insert_overflow` is declaration-only, and
    the tag sits in x's dead slot.
- **NGameText::CDataBank entry vectors**:
  - 0x009CB4E0 `vector<CDataBankTextEntry>::_M_insert_overflow`, a copy of the landed twin 0x00A43B20 with this TU's
    callees.
  - 0x009CB740 / 0x009CB790 `push_back` of TextEntryBuffer (+0x1EC) and GroupEntryBuffer (+0x1F8). The
    CIVCountedPointer copy add-refs at +4.
- **CBankFile CompareDataOffset sort**: 0x009D4620 `__partial_sort`, 0x009D48C0 `__introsort_loop`. Both use a local
  CBankFile_Retail model because the engine header disagrees with the bytes. `__median` is out of line.
- **CLoadingMemoryFailureHandler::RegisterBank** 0x009D6420: the critical-section lock is a local, then
  `Banks.push_back(bank)`. It matches only with a same-TU `__declspec(noinline)` STLport POD `_M_insert_overflow` body
  under `auto_inline(off)`. Declared-only, the tag gets its own frame byte (99v97).
- **CSymbolMap rows**:
  - 0x009E66F0 `__unguarded_linear_insert` over `pair<CCharString, CSymbolMap::CSymbolInfo>`. The CCompareString
    operator() is `a.first.FastCompare(b.first)`.
  - 0x009E79E0 `CVectorMap<CCharString, CSymbolInfo, CCompareString>::DeallocateMemory`, written as
    `Map.clear(); CVectorMap empty; swap(empty);`. The swap exchanges the vector words through `_STL::swap` and then
    Dirty.
- **CConsoleCommandParameters constructors** 0x009EBB80 (two types) / 0x009EBC00 (three types): one
  `AddParameterType` (`ParamTypes.push_back`) per argument. These are adapted from the landed one-type constructor
  0x009EBCB0.
- **vector<CVertexShaderInput::CStreamInfo>::push_back** 0x00AB4640: the implicit CStreamInfo copy inlines
  CVertexStreamFormat's copy constructor. The derived vptr store (0x0129D578) comes before the out-of-line
  `CArray<CDataElement>` copy (0x00A5E0A0).
- **QuadricOptimiser CGroup sort** (/Gr, `bool (__fastcall*)(const CGroup&, const CGroup&)` = CGroup::CompareWeight):
  0x00AD2CE0 `__make_heap`, 0x00AD4880 `__linear_insert`, 0x00AD5BC0 `__partial_sort`. These are plain STLport 4.6
  bodies with make_heap / `__pop_heap` / copy_backward inline. The CMatchProbability labels are propagated.
- **map<pair<long,long>, CCountedPointer<CVertexPairContraction> >::operator[]** 0x00AD6740: STLport operator[]
  verbatim. It needs two things:
  - The landed 0x00A2D810 release form `info = Info; if (info && --info->RefCount == 0)`.
  - A same-TU noinline `insert_unique(hint, v)` stand-in that reads v and may write its counted pointer. Declared-only,
    VC re-reads the escaped temporary (141/142v137).
- **CBlockInfo heap** 0x00ADB420 `__make_heap<CBlockInfo*, less<CBlockInfo> >` (/Gr): 12-byte stride by reciprocal.
  It matched first try with the PDB class (no user copy constructor needed).
- **CTCSoundPlayer::PlayAttachedDialogue(const CCharString&, bool, bool, bool, bool)** 0x0066BED0 does
  `bank = g_pMainGameComponent->GetGameTextDataBank(); tag = bank->GetTagFromCRC(name.GetCRC());` and then calls the
  tag overload 0x0066BA80. The bank and the tag must be separate statements: the one-expression form pushes the bools
  before the CRC (94v93).
- **CDefString::operator==(const CCharString&)** 0x006AEB10: `_stricmp(table.GetStringAsCharArray(TablePos),
  text.GetChars()) == 0`. 0xBFEBA8 was verified as `_stricmp` against IAT slot 0x014400EC.
- **CWideString::Contains** 0x0099AF20 (`wcsstr(...) != 0`; VC tail-duplicates the call into both null arms) and
  **CWideString::operator<** 0x0099AF70 (`wcscmp(...) < 0`). They sit in the CWideString TU after the landed accessors
  0x0099AF00 / 0x0099AF10.
- **bitset<274>::test** 0x00410DE0 (CThing::UsingComponents): STLport no-exceptions `puts("bitset"); abort();` range
  check, then `(_M_getword(pos) & _S_maskbit(pos)) != 0`. It is the same model as the landed set/reset 0x004CC090 /
  0x004CC0D0.
- **STLport list<T>::operator=** 0x00517720 (C2DLineF) and 0x00535800 (`NUISystem::CComponent*`): the 4.6 body
  verbatim, with the member-template `insert(pos, first, last)` inline over the out-of-line
  `_M_insert_dispatch(..., const __false_type&)`. The tag and the erase result are homed in the dead `x` slot. The
  iterators use the `_Nonconst_traits` / `_Const_traits` split with user copy constructors.
- **NUISystem::CAction::AddParam(CActionParam*, CCountedPointer<CAction>&)** 0x0053FDE0: for a parameter of type 1,
  `static_cast<CComponent&>(*param).AddActionIAmParamOf(self)` (slot 96). No null test is emitted, and the pointer
  cast grades the same because param was already dereferenced. Then
  `UIActionParams.push_back(param)`. The STLport list insert is inline and the node comes from malloc.

## Parked for Fable 5.1 (1)

- 0x009C2900 _STL::vector::reserve DIFFER(171v171): frame-slot swap only. Retail spills the old `_M_finish` to the `push ecx` word and homes the new block (and the __false_type tag) in n's dead argument slot; ours homes them the other way round (same instructions, swapped [esp+N] slots) | tried: STLport verbatim, uncast pointers, uninitialized_copy front end, non-template _M_allocate_and_copy | next: a same-TU noinline `__uninitialized_copy(first, last, result, const __false_type&)` stand-in (0x0040DDC0) that never touches the tag (the chunk-422 recipe), else lane gap: frame-slot assignment

## Skipped

None. No row holds two functions; every row is one body ending in `ret`.

## Label corrections

- 0x0066BED0 "std::_Iterator_base12::_Getcont" -> CTCSoundPlayer::PlayAttachedDialogue(const CCharString&, bool, bool, bool, bool) (Ego_r/FableWin `?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z`); its callee 0x0066BA80 "Sound_InitializeFromTextEntry" = PlayAttachedDialogue(unsigned long, bool, bool, bool, bool) (`...QAEKK_N000@Z`).
- 0x006AEB10 "std::_Iterator_base12::_Getcont" -> CDefString::operator==(const CCharString&) const (_stricmp form; caller IsHeroPerformingExpression 0x008953A0).
- 0x0099AF20 "std::_Iterator_base12::_Getcont" -> CWideString::Contains(const CWideString&) const.
- 0x0099AF70 "std::_Iterator_base12::_Getcont" -> CWideString::operator<(const CWideString&) const.
- 0x0053FDE0 "std::list::push_back" -> NUISystem::CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) (PDB name; some landed callers call it "SetTarget").
- 0x00535800 "std::list::splice" -> _STL::list<NUISystem::CComponent*>::operator= ; its callee 0x00534D30 "std::list_node::splice" = list<CComponent*>::erase(iterator, iterator).
- 0x00517720 "std::list::operator=" is right; element C2DLineF.
- 0x009EBB80 "std::vector_construct_pair" / 0x009EBC00 "std::deque::push_back_expand" -> CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType, EGenericVarType[, EGenericVarType]).
- 0x009E79E0 "std::vector_swap_move" -> CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolInfo> >::DeallocateMemory.
- 0x009D6420 "std::vector_push_back_4byte_list" -> CLoadingMemoryFailureHandler::RegisterBank(CBankFileAsync*); 0x009D5A30 (manifest "CBankFile::PopLoadErrorQueue") = CLoadingMemoryFailureHandler::UnregisterBank.
- 0x009B8440 "std::introsort_build_heapify" -> _STL::sort<CWideString*>(first, last); 0x009B2AC0 (manifest "_Destroy") = __unguarded_insertion_sort_aux<CWideString*>.
- 0x009CB4E0 "vector_reserve_8byte_fill" -> vector<NGameText::CDataBankTextEntry>::_M_insert_overflow; 0x009CB740 / 0x009CB790 -> vector<CDataBankTextEntry> / vector<CDataBankGroupEntry>::push_back.
- 0x00AD2CE0 "make_heap<CMatchProbability>" -> __make_heap<COptimisedVertex::CGroup*, bool(__fastcall*)(const CGroup&, const CGroup&)>; 0x00AD4880 "_InsertionSort_PartitionHelper" -> __linear_insert (same family); 0x00AD5BC0 "_Heap_PopSort" -> __partial_sort (same family); the callee 0x00AD1DD0's "_Adjust_heap<CSpeechAnalyser::CMatchProbability*...>" manifest label is propagated (it is the CGroup __adjust_heap, as its landed source says).
- 0x00ADB420 "make_heap_Pair_EExpressionType" -> __make_heap<CVertexGroupBlockOptimiser::CBlockInfo*, less<CBlockInfo> >; 0x00ADB000's pair<EExpressionType, CExpressionSet::CEntry> manifest label is propagated (CBlockInfo).
- 0x00AD6740 "std::map_InsertOrFind" -> map<pair<long,long>, CCountedPointer<CVertexPairContraction> >::operator[]; 0x00AD5630 "std::map_FindOrInsert" = its _Rb_tree::insert_unique(iterator, const value_type&).
- 0x00AB4640 "vector_InsertPair" -> vector<CVertexShaderInput::CStreamInfo>::push_back.
- The remaining sort rows' catalog names (introsort_*, for_each_range*, insertion_sort_8byte, partial_sort_8byte, heapsort_and_extract_min, make_heap_pair_fixup, vector_push_back_impl, insertion_sort_16byte) are descriptive; the real STLport names are on line 1 of each source.
- 0x009CC410 "CVectorMap::LookupValue" = NGameText::CDataBank::GetTagFromCRC (as landed callers already declare); 0x004014A0 "Calc" is the CRC.
