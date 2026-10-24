# Hand chunk 373 (_STL::__unguarded_insertion_sort_aux .. _STL::__copy_backward_ptrs<CPrizeScoreDef>): 37 of 40 byte-exact

All 40 rows are catalogued `_global`. They turned out to be STLport 4.x template bodies (algorithms, vector/tree members)
and GFSerialise* persist templates, spread over three TUs:
- 0x004D0xxx is the CSubtitledMessage vector-map TU, built /O2.
- 0x004DFxxx-0x004E8xxx is the thing-component registry / morph / weapon-def TUs, size-optimised.
- 0x004E9xxx-0x004EDxxx is the tavern-game / shop / augmentation def TUs, size-optimised, mostly with a frame pointer.

Status: 37 matched (4 MATCH, 33 RELOCATION_MATCH). All 37 tests PASS under g++ (tall.sh) and VC7.1 (tall.py). 3 rows are
parked, 0 skipped.

## What matched (37)

### STLport sort and heap algorithms (/Gr: free templates are `__fastcall`, first in ecx, second in edx)
- **0x004D0460 `__unguarded_insertion_sort_aux` and 0x004D0660 `__adjust_heap`** over pair<float, CSubtitledMessage::CCameraTag>
  (28 bytes).
  - The pair needs STLport's memberwise copy constructor. Element moves are the implicit operator= (`rep movsd`).
  - A by-value pair argument is built in the argument area.
  - `fld ; fcomp ; test ah,5 ; jp` is the less-than comparator.
- **0x004D0F10 / 0x004D0F80 `__linear_insert`**, for pair<float, EConversationAttitude> and pair<float, CCharString>.
  - The `__copy_backward(..., random_access_iterator_tag, Distance*)` call is inline in the first and out of line in the
    second.
- **The registry pair<CCharString, CRegisteredTCInfo> (16 bytes) family**, in a size TU: 0x004DFD60
  `__unguarded_insertion_sort_aux`, 0x004E1C8A `__linear_insert`, 0x004E1D68 `__make_heap`, 0x004E415D
  `__final_insertion_sort`, 0x004E6E9E `__partial_sort` and 0x004E823B `__introsort_loop`.
  - Use `#pragma optimize("s", on)`.
  - The comparator, CCharString::operator< and the pair copy ctor are `__forceinline`; the size TU out-lines a plain
    `inline`.
  - Recursion and the out-of-line helpers are declared with their retail addresses.
- **0x004EA57F `uninitialized_fill_n<CEngineGraphic*>`** (MATCH). It is the 3-argument STLport entry with
  `__uninitialized_fill_n(..., __false_type)` inlined. The leaf is `??$uninitialized_fill_n`.

### STLport vector members
- **Copy constructors: 0x004D0830, 0x004D08A0, 0x004EBE1A (vector<C2DVector>) and 0x004EC4B0 (vector<CDefString>).**
  - The source text is `_Vector_base(x.size(), x.get_allocator())`, then `_M_finish = __uninitialized_copy(...)`.
  - The allocator temporary has a user ctor, so it gets no store and is homed at `[ebp+0Bh]`.
  - In the def TUs the `_Vector_base(n, a)` ctor is out of line, with `y` off and `s` on.
- **`_M_allocate_and_copy`: 0x004E6088 (CEngineGraphic, pack 2), 0x004E70CF / 0x004E7279 / 0x004E741D (the hero-morph pairs)
  and 0x004EC10F (CCardDef).**
  - The body is `allocate(n)` plus a `__false_type` copy loop.
  - In the size TU, `allocate`, `_Construct` and `__uninitialized_copy` are `__forceinline`; when `_Construct` is out of line,
    only allocate and the loop are.
- **operator=: 0x004E9AD3 (CShopItemDef), 0x004EBCA9 (CCardPositionsDef) and 0x004EBF32 (CCardDef).**
  - This is the STLport text verbatim.
  - `_TrivialAss()` is a POD empty `__false_type`, so `xor eax,eax ; stosb` writes it into the dead x slot.
  - The `_Destroy` tag must be a named uninitialised local.
  - `_Destroy`, `__destroy` and `__uninitialized_copy` are `__forceinline`.
  - The 0x44 stride changes the register choice (`sub esp,0Ch`) with no source change.
- **0x004EA40B vector<CEngineGraphic>::erase** (MATCH): `__copy_ptrs` and `__copy` are inline and the trivial destroy loop vanishes.
- **0x004ED353 vector<CShopItemDef>::_M_insert_overflow**: the STLport text, with every helper `__forceinline` except
  `_Construct` and `_M_clear`.
- **`__copy_ptrs`: 0x004EBDB0 (CCardPositionsDef), 0x004EAF33 (CShoveHaPennyRegionDef) and 0x004EDCC2
  (`__copy_backward_ptrs`, CPrizeScoreDef).**
  - The inner `__copy` / `__copy_backward` is `__forceinline`.
  - The element's implicit operator= is spelled out and `__forceinline` (base operator= out of line).
  - VC7.1 copies the first float member with `mov` and the second with `fld/fstp` by itself.
- **0x004D2060 CVectorMap<float, CCharString>::Insert**: `Vector.push_back(pair(key, value)); Dirty = true;` with STLport
  push_back inline.
- **0x004E4BEE `_Rb_tree<EObjectAugmentationType, pair<const ., CWeaponTrailGraphicSet>>::_M_create_node`**:
  `allocate(1)` and `_Construct` are `__forceinline` under "s".

### GFSerialise persist templates
- **0x004E910F and 0x004EC85A GFSerialiseMapBinaryOut, and 0x004E5A76 GFSerialiseMapBinaryIn<long, CDefString,
  CCompareDefString<long>>**, on the landed 0x00469309 / 0x004EC66C models.
  - In 0x004E910F the value traits' two-call TransferBinaryOut must be `__forceinline`.
  - In 0x004EC85A the value traits hold an inline-built DefaultVal, which is destroyed out of line.
  - 0x004E5A76 uses the same-TU noinline stand-in for the out-of-line long traits (0x0040FE60), so the empty traits are
    homed on the map spill word.
- **0x004E9CE8 / 0x004EB1A8 GFSerialiseVectorBinaryOut and 0x004E9D71 / 0x004EB219 GFSerialiseVectorBinaryIn**
  (CMusicEntry, CExplosionRing).
  - The CMusicEntry traits carry an inline DefaultVal.
  - The CExplosionRing traits are an empty specialization forwarding to the element's member.
  - The one-argument `resize(n)` is out of line in 0x004E9D71 and `__forceinline` in 0x004EB219.

## Parked for Fable 5.1 (3)
- 004d1e20 CVectorMap<float,EConversationAttitude>::Insert DIFFER(101v95): retail loads `_M_finish` into eax before the value word (`mov ecx,[edx]` reuses ecx) and tests `cmp eax,[esi+8]`; ours loads the value into eax first, finish into ecx (`cmp eax,ecx`) and re-reads the second word from the frame for the in-place copy | tried: pair temporary, make_pair, trivial `~pair()` (the same model matches the CCharString twin 0x004D2060) | next: lane gap: register allocation (scheduling only)
- 004dfcea _STL::__unguarded_partition (registry pair) DIFFER(110v113): ours (size TU + forceinline comparator) builds an ebp frame and keeps the pivot string pointer in its argument slot (`cmp [ebp+8],ecx`, re-read every pass); retail has no frame, keeps pivot.p in esi (`mov ecx,esi` before the right-scan call) and re-reads it only after iter_swap | tried: plain /O2 (128v113), "s" (84v113, comparator out of line), "s" + __forceinline comparator / CCharString::operator< (110v113), + "y" on (110v113) | next: define the landed CBasicString<char>::operator< 0x00429950 body above under `#pragma auto_inline(off)` (+ "s", forceinline comparator) so VC7.1 knows the call writes no memory and keeps the pivot in a register
- 004ea21e CPersistContext::Transfer<NGraphicAppearance::CReplaceableMeshes> DIFFER(80v77): retail hoists `lea ecx,[ebp-0Ch]` (the traits object) above the mode switch, so the default path calls the destructor with it and both arms reuse it; ours re-materialises it in each arm and again before the destructor | tried: switch (the landed 0x0045C8D3 model), if/else-if, a traits reference local, implicit traits ctor/dtor over the vector member's out-of-line ctor/dtor | next: lane gap: code hoisting (scheduling only); maybe the QFE-4035 compiler

## Skipped
None. No row holds two functions; every row's extent matches its retail body (the trailing int3s are padding).

## Label corrections
- 0x004D0460 `_Fill<CHeroMarriageInfo*,...>` and 0x004D0660 `_Adjust_heap<CDebugBox*,...>`: both are propagated. They are
  STLport `__unguarded_insertion_sort_aux` / `__adjust_heap` over pair<float, CSubtitledMessage::CCameraTag>.
- The callee 0x004CFEB0 `_Push_heap<CDegradableInfo*,...,CIsHealth1GTHealth2>` is propagated: its comparator is less-than,
  not GT.
- 0x004D0830 `Vector_Copy_Uint` and 0x004D08A0 `Vector_Copy_FixedStruct_0x1c`: these are vector<pair<float,CCharString>> and
  vector<pair<float,CCameraTag>> copy constructors.
- 0x004D0F10 / 0x004D0F80: these are STLport `__linear_insert` (the catalog names are descriptive).
- 0x004D1E20 / 0x004D2060: these are CVectorMap<float, EConversationAttitude / CCharString>::Insert (Ego_r.pdb names the
  member `Insert`). 0x004D2060's `std_vector_push_back_EHeroMorphType_CParticleMorphs_CEntry` is propagated.
- 0x004DFD60 `_Fill<CIntelligentPointer<CThing>*,...>` and 0x004E415D `make_heap<CThing**,...>`: both are propagated. The
  element is the registry pair<CCharString, CRegisteredTCInfo>.
- 0x004E6E9E `_Find<CCharString*,CCharString>` is propagated: the body is `__partial_sort`.
- 0x004E1C8A, 0x004E1D68 and 0x004E823B (`Set_Insert_Element`, `Heap_SiftDown`, `Partition_WithMedianOfThree`): these are
  `__linear_insert`, `__make_heap` and `__introsort_loop`.
- 0x004E4BEE `AllocAndCopyThreeFields`: this is the CWeaponDef WeaponTrails `_Rb_tree::_M_create_node`.
- 0x004E5A76: this is GFSerialiseMapBinaryIn<long, CDefString, CCompareDefString<long>> (FableWin.pdb). Its writer twin is
  0x004E5A39.
- 0x004E6088 / 0x004E70CF / 0x004E7279 / 0x004E741D (`AllocateAndCopyActiveFileStride14/24/40/12`): these are
  `_M_allocate_and_copy` for CEngineGraphic and for the hero-morph pairs.
- 0x004E910F `WriteAllocatedAreasDataWithThreeFields`: this is GFSerialiseMapBinaryOut<CWeaponTrailGraphicSet,
  EObjectAugmentationType>.
- 0x004E9AD3 `CActiveFile_ReallocateAndCopy`: this is vector<CShopItemDef>::operator=. Callees with propagated labels:
  - 0x004E9BB7 `CActiveFile::OnReadFinished` is `_M_clear`.
  - 0x004E9BDA `_Copy_impl<pair<ulong,C3DDummyObject>*>` is `__copy_ptrs<CShopItemDef>`.
  - 0x004E9C39 `_Cons_val<...CDateAndTime...>` is `_Construct<CShopItemDef>`.
- 0x004E9D71 and 0x004EB219 `GFSerialiseVectorBinaryIn<CBrainUpdateZone>`: both are propagated. They are <CMusicEntry> and
  <CExplosionRing> (16-byte strides).
- 0x004EA21E `CReplaceableMeshes_Transfer`: this is CPersistContext::Transfer<NGraphicAppearance::CReplaceableMeshes>.
  Callees with propagated labels:
  - 0x004EA2AE `CIndexBuffer::CIndexBuffer` is the traits ctor.
  - 0x004EA2BB `CActiveFile::OnReadFinished` is the traits dtor.
- 0x004EA40B `Vector_InsertMoveRange_0xe`: this is vector<CEngineGraphic>::erase.
- 0x004EA57F `_Fill<CEngineGraphic*,CEngineGraphic>` uses the Dinkumware spelling: this is STLport `uninitialized_fill_n`.
- 0x004EAF33 `VectorReallocateCopyConstruct`: this is `__copy_ptrs<CShoveHaPennyRegionDef>` (vtable 0x0124383C).
- 0x004EBCA9 `Pair_CCharString_CArray_CopyAssign` is propagated: this is vector<CCardPositionsDef>::operator=.
- 0x004EBDB0 `VectorReallocateCopyConstruct`: this is `__copy_ptrs<CCardPositionsDef>`. Its callee 0x0046EABD
  `CActiveFile_Copy` is vector<C2DVector>::operator=.
- 0x004EBE1A / 0x004EC4B0 `VectorAppendRange`: these are the vector<C2DVector> / vector<CDefString> copy constructors.
  Their callees 0x004EBE62 / 0x004EC4F2 are `_Vector_base(n, a)`.
- 0x004EBF32 `CShoveHaPennyRegionDef_CopyAssign_0x44` and 0x004EC10F `CShoveHaPennyRegionDef_AllocateCopyConstruct_0x44`: the
  class is wrong. The element is CCardDef (vtable 0x01243A44): these are vector<CCardDef>::operator= and
  `_M_allocate_and_copy`.
- 0x004EC85A `Shadow_WriteAugmentationParticles`: this is GFSerialiseMapBinaryOut<CObjectAugmentationParticleSet,
  EObjectAugmentationType>. Its callee 0x004E25F1 (`FinishWithDynamicTextures`) is ~CObjectAugmentationParticleSet.
- 0x004ED353 `Vector_ReallocateInsert_0x48`: this is vector<CShopItemDef>::_M_insert_overflow.
- 0x004EDCC2 `Vector_InsertCopyRange_Backward_0x30`: this is `__copy_backward_ptrs<CPrizeScoreDef>` (vtable 0x012437D4).
