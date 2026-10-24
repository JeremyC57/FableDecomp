# Hand chunk 425 (QuadricOptimiser STLport helpers .. CParticleSimulation): 36 of 40 byte-exact

All 40 rows are `_global` catalog rows. Their labels were propagated or descriptive, and every one was renamed from
Ego_r.pdb / FableWin.pdb names, callers and vtables (see Label corrections). 36 rows match: 4 MATCH and 32
RELOCATION_MATCH. 4 rows are parked and none are skipped. Every src/ row has a test in test/ that passes under g++ and
under VC7.1 (tall.py). test/00adbff0.cpp belongs to the parked row 00adbff0 and also passes; finish.py ignores it
because the row has no src/.

## What matched (36)

- **QuadricOptimiserInternals::COptimisedVertex / CUniqueVertex**
  - 00ad4c40: `CUniqueVertex()`, two inline `set<unsigned long>` constructors.
  - 00ad7a00: `COptimisedVertex() : Exists(true)`, three inline STLport tree constructors.
  - 00ad71b0: `Remove()`, written as `Exists = false; FaceList.clear(); PairContractions.clear();` with the erase
    callees out of line.
  - 00ad4960: `sort_heap`, and 00ad6f10: `__introsort_loop`. Both run over `COptimisedVertex::CGroup` with the static
    `__fastcall` CompareWeight as a /Gr function-pointer comparator.
- **multimap<float, CCountedPointer<CVertexPairContraction>>**
  - 00ad4ca0: `erase(iterator)`. It goes through `_Rb_global<bool>::_Rebalance_for_erase`; the CCountedPointer
    destructor releases the pointer and then zeroes both words.
  - 00ad54b0: `insert_equal`, with the float `less` key compare.
- **COptimisedSubMesh range helpers (/Gr)**
  - 00ad75f0: `__uninitialized_copy`.
  - 00ad76e0: `__uninitialized_fill_n`.
  - 00ad7dc0: `__destroy_aux`.
  - The key: an explicit specialisation declaration of the inner callee (the COptimisedBone copy, the counted-pointer
    destroy) keeps that callee out of line while the outer one is instantiated with its inline vector copy.
- **CVertexGroupBlock / CVertexGroupBlockOptimiser**
  - 00ad9030: the constructor.
  - 00ad8fe0 / 00ad9d10: `CalcCurrentBestTheoreticalResultNewBlock` / `NewBlock2`. Which is which follows the call
    order in AddBlocks 0x00AD9F10.
  - 00ad9120: `CountRemainingCurrentBlockSegments`, a local `set<long>`. The pair needs user constructors so that
    `pair<iterator,bool>` comes back through the hidden pointer.
  - 00ad91f0: `AddGroup`, push_back + `map::insert`. The empty `__true_type` tag is homed on the LIVE `group` argument
    slot only with a same-TU `__declspec(noinline)` body of `vector<unsigned long>::_M_insert_overflow` under
    `auto_inline(off)`. A declaration-only callee gives the tag its own 0x14 frame.
- **CVertexGroupBlockOptimiser::CBlockInfo sort family (/Gr, `less<CBlockInfo>`, `operator<` on Cost)**
  - 00adaa30 `__unguarded_linear_insert`, 00adaf90 `__unguarded_insertion_sort_aux`, 00adb320 `__unguarded_partition`
    (iter_swap's temporary reuses the dead pivot slots), 00adbc50 `__linear_insert` (same-TU noinline
    `__copy_backward`, so the tag lands on the comparator slot), 00adbf00 `__partial_sort` (make_heap and __pop_heap
    inline), and 00adc890 `sort(first, last)`.
  - In `sort`, `less<>` needs a user default constructor. Without it every `__less()` temporary gets a
    `mov byte ptr [esp+8],0`. With it, the uninitialised `push ecx` frame word is pushed as the comparator.
- **map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>**
  - 00adb670: `_Rb_tree::_M_insert`, the STLport 4.x body with `_M_create_node` inlined in both arms.
- **vector / list of CVertexGroupBlock (element = vector<unsigned long> + map<unsigned long, long>)**
  - 00adc190 `__destroy_aux` and 00adc250 `_List_base::clear`. In both, `~_Rb_tree() { clear(); }` and the recursive
    `_M_erase` body are visible in the TU, and VC7.1 inlines the top level, keeping the recursion out of line
    (0x004507A1).
  - 00adc330 / 00adc440 `__uninitialized_copy`. They are byte twins. 00adc330 is the <T*, T*> instance (callers
    reserve / _M_insert_overflow) and 00adc440 the <const T*, T*> instance (caller operator=).
  - 00adc3b0: `__uninitialized_fill_n`.
  - 00adcb90: `reserve`, in a /Gr TU. Both `__false_type` tags are homed on the live `n` slot, which needs same-TU
    noinline bodies of the copy and destroy helpers.
  - 00adc4e0 `list::erase(first, last)` (erase(iterator) inline, post-increment written back to the `first` slot),
    00adcc80 `_M_insert_dispatch`, and 00adce10 `push_back`. In push_back the vector<unsigned long> copy constructor is
    out of line (0x00535470).
  - 00add020: `list::operator=`. The STLport `_List_iterator(const iterator&)` constructor is the user copy constructor
    of `iterator`, which is built in its argument slot. `const_iterator` keeps an implicit copy and is pushed straight
    from its register.
- **LHA file layer**
  - 00add130: `CLHFileCompletionCallback::OnFileReadCompletion(bool)`, written as
    `client->OnReadComplete(success != false, user); delete this;`.
  - 00add240: `CLHFile::Read`, vtable slot 1 (the name is inferred). It builds a `CThreadedFileObjectDesc` with a
    `new CLHFileCompletionCallback`. The priority if/else is followed by ONE `AddObject` + `return true`, and VC7.1
    duplicates that tail into both arms. Writing the call in each arm schedules the second arm differently.
- **C3DBone2**
  - 00add650: `SetName`, i.e. `NameCRC = CCRC::CalcFromString(name)` with the CCharString accessors written null arm
    first (`PStringData == 0 ? 0 : ...`). The opposite order inverts both branches.
- **CParticleSimulation**
  - 00ae0600: the constructor, `: MemoryHandle(-1), MemoryPool(0) { Clear(); }`. The empty inline CMovableResource
    constructor's vtable store is dropped.

## Parked for Fable 5.1 (4)

- 00ad6420 map<unsigned long,float>::operator[] DIFFER(115v115): retail stores the 0.0f of `value_type(k, float())` after `push edx` (`mov [esp+1Ch],0`), the lane before the iterator copy (`mov eax,esp ; mov [esp+18h],0 ; mov [eax],edx`) | tried: `__default_constructed((float*)0)`, `_Tp()`, full map -> _Rb_tree layering with `_M_lower_bound`, base-iterator / `_Nonconst_traits` iterator | next: lane gap: late pair store scheduled after the hidden-result push
- 00ad64c0 map<unsigned long,CContractionReference>::operator[] DIFFER(163v158): (1) the 00ad6420 late Data=0 store; (2) the post-call pair release is `mov esi,[esp+14h] ; test esi,esi ; je ; dec [esi] ; jne` in retail but `mov eax ; mov esi,eax ; dec [eax] ; cmp [esi],0` in the lane | tried: no user default ctor on CContractionReference (fixes SubIndex), Release through a local `info` with `--info->RefCount == 0`, member re-read form | next: a `__forceinline` release that re-reads `Info` from the temporary's slot; the scheduling part is the same lane gap as 00ad6420
- 00ad8af0 CVertexGroupBlock::SharesGroupWith DIFFER(188v188): register assignment only: retail keeps this/other headers in ebp/ebx and the iterators in edx/ecx (loads `other` first), the lane keeps this in ebx/ecx and other in ebp/edx | tried: plain `return true/false` form, `bool shares = false ... shares = true; break;`, iterator declaration order, a non-forceinline SharesElement helper (not inlined, 33v188) | next: a `__forceinline` helper whose arguments evaluate `other` first, else lane gap: register allocation
- 00adbff0 map<unsigned long,CGroupInfo>::operator[] DIFFER(120v120): retail stores value.first (edi) and BlockIndex -1 (esi) after `push edx` (`mov [esp+18h],edi ; mov [esp+20h],esi`), the lane before the iterator copy | tried: `__default_constructed((CGroupInfo*)0)`, `_Tp()` | next: lane gap: late pair store scheduled after the hidden-result push (same as 00ad6420)

## Skipped

None. No row holds a second function body.

## Label corrections

- 00ad4960 `_Sort_heap<long*>`: is `_STL::sort_heap<COptimisedVertex::CGroup*, bool (__fastcall*)(const CGroup&, const CGroup&)>`.
- 0x00AD1DD0: its CMatchProbability label is wrong. It is `__adjust_heap` over CGroup.
- 00ad4c40 `Intrusive_List_Pair_Construct`: is `QuadricOptimiserInternals::CUniqueVertex::CUniqueVertex()`. The landed
  0x00AD4A70 "CTwoLongSets" destructor is `~CUniqueVertex`.
- 00ad4ca0 `CMemoryAllocator_Erase`: is `_Rb_tree::erase(iterator)` of `multimap<float, CCountedPointer<CVertexPairContraction>>`.
- 0x0042A34E: is `_Rb_global<bool>::_Rebalance_for_erase`, not CMemoryAllocatorVariableSize::Erase.
- 00ad54b0 `BTree_FindAndInsert`: is `insert_equal`. 0x00AD4540 "BTreeNode_InsertValue" is `_M_insert`.
- 00ad6420 `BTreeMap_FindOrInsert`: is `map<unsigned long,float>::operator[]`.
- 00ad64c0 `BinaryTree_FindOrInsert`: is `map<unsigned long,CContractionReference>::operator[]`.
- 00ad6f10 `Heap_Partition`: is `__introsort_loop<CGroup*>`.
- 00ad71b0 `COptimisedPrimitive_InitializeTrees`: is a COptimisedVertex member (Exists=false + two clears). It is named
  `Remove` descriptively.
- 00ad75f0 `Vector_CopyConstructor_6Way`: is `__uninitialized_copy<const COptimisedSubMesh*>`. 0x00AD1B20
  "_Uninit_copy<CFeatDef*,...>" is `__uninitialized_copy<const COptimisedBone*>`.
- 00ad76e0 `Vector_ReallocateElements_6Way`: is `__uninitialized_fill_n<COptimisedSubMesh*>`.
- 00ad7a00 `COptimisedPrimitive_ConstructorBody`: is `COptimisedVertex::COptimisedVertex()`.
- 00ad7dc0 `_Copy_impl<CNavigationPosition*,...>`: is `__destroy_aux<COptimisedSubMesh*>`.
- 00ad8af0 `TreeNodes_HaveIntersection`: is `CVertexGroupBlock::SharesGroupWith`.
- 00ad8fe0 `VertexGroup_CountComponents`: is `CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock`.
- 00ad9d10 `VertexGroupBlockOptimiser_CountComponents`: is `CalcCurrentBestTheoreticalResultNewBlock2`.
- 00ad9030 `Vector_Construct_Empty`: is `CVertexGroupBlock::CVertexGroupBlock()`.
- 00ad9120 `QuadricOptimiser_BuildGroupTree`: is `CVertexGroupBlockOptimiser::CountRemainingCurrentBlockSegments`.
- 00ad91f0 `AnimBlockGroup_AddBlock`: is `CVertexGroupBlock::AddGroup`.
- 00adaa30 `Insertion_ShiftAndInsert`: is `__unguarded_linear_insert<CVertexGroupBlockOptimiser::CBlockInfo*>`.
- 00adaf90 `_Copy_impl<CNavigationPosition*,...>`: is `__unguarded_insertion_sort_aux<CBlockInfo*>`.
- 00adb320 `Partition_ThreeElementSwap`: is `__unguarded_partition<CBlockInfo*>`.
- 00adbc50 `Vector_Pair_InsertOrShift`: is `__linear_insert<CBlockInfo*>`.
- 00adbf00 `Std_HeapifyAndSort_Pair`: is `__partial_sort<CBlockInfo*>`.
- 00adc890 `Std_Sort_Pairs`: is `sort<CBlockInfo*>`.
- Other CBlockInfo instances: 0x00ADC650 is `__introsort_loop<CBlockInfo*>`, 0x00ADB420 is `__make_heap`, 0x00ADBCB0 is
  `sort_heap`.
- 00adb670 `std_set_insert_uint_triple`: is `_Rb_tree<unsigned long, pair<const unsigned long, CGroupInfo>>::_M_insert`.
- 00adbff0 `BinTree_SearchOrInsert`: is `map<unsigned long,CGroupInfo>::operator[]`.
- 00adc190 `LTextTreeWalkThrough_DestroyRange`: is `__destroy_aux<CVertexGroupBlock*>`.
- 00adc250 `LTextTreeWalkThrough_Cleanup`: is `_List_base<CVertexGroupBlock>::clear`.
- 00adc4e0 `LTextTreeWalkThrough_DestroyRange`: is `list<CVertexGroupBlock>::erase(first, last)`.
- 00adc330 `Vector_CopyGroups`: is `__uninitialized_copy<CVertexGroupBlock*, CVertexGroupBlock*>`.
- 00adc440 `Vector_CopyConstructor_6Way_List`: is the `<const CVertexGroupBlock*, ...>` twin of 00adc330.
- 00adc3b0 `Vector_Copy_UninitRange`: is `__uninitialized_fill_n<CVertexGroupBlock*>`.
- 00adcb90 `Vector_Reallocate_Sized`: is `vector<CVertexGroupBlock>::reserve`.
- 00adcc80 `List_InsertCopy_Range`: is `list<CVertexGroupBlock>::_M_insert_dispatch`.
- 00adce10 `List_EmplaceBack_Pair`: is `push_back`.
- 00add020 `List_CopyAssign`: is `list<CVertexGroupBlock>::operator=`.
- Other STLport callees in this chunk:
  - 0x004507A1: `_Rb_tree<unsigned long, pair<const unsigned long,long>>::_M_erase`.
  - 0x00A96E40: the copy constructor of that tree.
  - 0x0045C79E: the operator= of that tree.
  - 0x0045BC09: `vector<unsigned long>::operator=`.
  - 0x00535470: the `vector<unsigned long>` copy constructor.
  - 0x00464ACE: `vector<unsigned long>::_M_insert_overflow`.
  - 0x0042971D: `_Rb_global<bool>::_Rebalance`.
- 00add130 `Callback_InvokePair_add130`: is `CLHFileCompletionCallback::OnFileReadCompletion(bool)` (vtable 0x0129F084
  slot 2).
- 00add240 `NHeroInformationScreens_CAnimationEntry_Construct`: is CLHFile vtable 0x0129F094 slot 1, an async block
  read. It is named `Read` descriptively.
- 00add650 `CCRC_CalcRangeChecksum`: is `C3DBone2::SetName(const CCharString&)`.
- 00ae0600 `GetDefaultVal<CPhysicalPrimitiveInit>`: is `CParticleSimulation::CParticleSimulation()` (vtable 0x0129F0C4).
- Landed 0x00ADF1E0 `CLandscapeBackgroundPatch_ClearTextures`: is `CParticleSimulation::Clear()`. Its stores are
  CParticleSimulation members.
