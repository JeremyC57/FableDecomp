# Hand chunk 374 (vector<CPrizeScoreDef>::_M_insert_overflow .. CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp): 36 of 40 byte-exact

All 40 rows are `_global` in the catalog, and most labels are descriptive or propagated from Ego_r's Dinkumware build.
The bodies are retail STLport 4.x instances: vector, list, rb-tree, the copy, fill, heap and sort helpers, and
random_shuffle. The rest are the CWorldMap/region-population helpers and the old map-header serialise chain. Every
matched source is in `src/` with a behaviour test in `test/`, and each test prints PASS under both g++ and the lane's
VC7.1 cl.exe. `grades.txt` holds the final `gall.sh` run: 3 MATCH and 33 RELOCATION_MATCH.

## What matched (36)

### STLport vector growth (`_M_insert_overflow`, `_M_allocate_and_copy`, copy ctor) (6)
- 004edd13 vector<CPrizeScoreDef>, 004f9fa3 vector<CCardPositionsDef>, 004fa434 vector<CCardDef> `_M_insert_overflow(pos, x, __false_type, n, atEnd)`:
  - Frame-pointer size TU: `optimize("y", off)` then `("s", on)`.
  - Allocate `size + max(size, n)` elements.
  - `__uninitialized_copy` / `__uninitialized_fill_n` are `__forceinline`.
  - The per-element placement copy is the out-of-line `__fastcall _Construct` instance (0x4EAC30 / 0x4EBDF1 / 0x4EC08A).
  - The old storage is released through the out-of-line `_M_clear` (0x4EABC2 / 0x4EBD8D / 0x4EC01C).
  - CPrizeScoreDef uses `engine/CDefClassBase.h` + `_pad_0x25[3]`, Score +0x28, Mult +0x2C (0x30 bytes).
  - The card defs use a local 0x39-byte CParentDefClassBase; CCardDef is 0x44 and CCardPositionsDef 0x48.
- 005135d0 vector<unsigned short> `_M_insert_overflow(.., __true_type, ..)`:
  - Speed TU.
  - `__copy_trivial` is written as STLport's `(last == first) ? result : (char*)memmove(result, first, n) + n` (the pointer compare comes before the subtraction), then `fill_n`, then an inline `_M_clear` (null-tested free).
- 004f9408 vector<pair<EAttackHistoryType,EDamageAttribute>> copy ctor: frame-pointer size TU, and the allocator temporary sits in the top byte of the dead `x` slot.
- 004f9484 vector<CAttackHistoryCombo>::_M_allocate_and_copy: size TU with `__forceinline` helpers, and the destination walks as `edi + esi`.

### STLport copy helpers (`__copy_ptrs` / `__copy_backward_ptrs`, `__fastcall`, `ret 8`) (4)
- 004f94d2 `__copy_ptrs<CAttackHistoryCombo*>`, 004f9db7 `__copy_ptrs<CCardPositionsDef*>`, 004f9f62 `__copy_backward_ptrs<CCardPositionsDef*>`, 004fa3e3 `__copy_backward_ptrs<CCardDef*>`:
  - Counted loops in size TUs.
  - The element's assignment must be a `__forceinline` memberwise `operator=`, or the size TU calls the implicit one out of line.
    - CAttackHistoryCombo: the pair vector's operator= 0x4F9508, then the float through the FPU.
    - CCardPositionsDef: base `operator=` 0x431ED0, then `Offset = other.Offset` 0x46EABD.
  - 004fa3e3 also needs the frame pointer (`("y", off)`), which keeps `result` in its argument slot.

### STLport vector(n) / fill / base (8)
- 0050e500 `fill<short*,short>`: speed TU, with the value re-read through its reference every pass.
- 0050fa40 `_Vector_base<void*>(n, a)`: end-of-storage in an `_STLP_alloc_proxy`, and `allocate` = `n ? malloc(n*4) : 0`.
- 00512630 vector<unsigned long>(n, value): the POD `__uninitialized_fill_n` is `fill_n`, a plain store loop.
- 005136b0 vector<unsigned char>(n): the one-byte fill is a `memset`, expanded inline (`rep stosd` + `rep stosb`).
- 00513b00 `__uninitialized_fill_n<CRGBColour*>`: placement null test, plus CRGBColour's permuted byte copy through the inline Set.
- 00515720 vector<CWorldMap::CPlayerPositionInfo>(n), 005158b0 vector<CLevelLoader::CObject::CLevelFiles>(n), 00515ad0 vector<CMapInfo>(n):
  - STLport `explicit vector(n) : _Vector_base(n, allocator_type()) { _M_finish = uninitialized_fill_n(_M_start, n, _Tp()); }`.
  - The default record is built at the top of the frame.
  - The empty tag is homed in the dead `n` slot.
  - The out-of-line fill comes through the 0x513A00 / 0x513570 instances; the CMapInfo fill is inline.
  - The record is destroyed after the call.

### STLport list and rb-tree (8)
- 00512120 list<C2DLineF>::erase(first, last): `while (first != last) erase(first++);` with the iterator returned through the hidden pointer.
- 00513b60 list<long>, 005152c0 list<C2DLineF> `_M_insert_dispatch`: the range insert inlines `insert` and `_M_create_node`, and reads `prev = pos->_M_prev` before `node->_M_next = pos`.
- 005151d0 list<CIntelligentPointer<CThingAICreature>>::insert: placement copy through the pointer's inline copy ctor (base ctor, vtable, `SetPItem(x.GetPItem())`).
- 00515420 CLineList::AddFront: `push_front` = `insert(begin(), x)` followed by `return *this`.
- 005155f0 CNavigationAreaUpdate::CSwitchableBlockage ctor: the STLport `_List_base` self-linked sentinel, built by a bare `malloc(0x18)`.
- 005114f0 `_Rb_tree<long>::_M_copy`, 00513300 `_Rb_tree<CCharString>::_M_copy`: STLport 4.5 `_M_copy` with `_M_clone_node` inlined at both sites. Node {color, parent, left, right, value +0x10}.

### Region population / world map (9)
- 004faff0 CWorldMap::GetThingFilenameFromLevelFilename (L"tng" 0x012442C4) and 004fb0a0 GetThingFilenameFromWorldFilename (L"gtng" 0x012442CC):
  - Static `__fastcall`, returning a CWideString through the hidden pointer.
  - Body: three default CWideStrings, then the extension string, `CAFile::SplitPath`, and `CAFile::MakePath` into a named local that is returned.
- 004fb370 BuildingHasVolume: nested null tests (building, PhysicsTC +0x60, `GetPhysicsMesh()` slot 93), then `GetPhysicsMeshObject().HasContainmentVolume("BUILDING_BOUNDRY")`.
- 004fe550 CWorldMap::ApplyScriptedMapBrush and 004fe5b0 CWorldMap::DeregisterScriptedMapBrush:
  - An inline `map::find` that wraps `_M_find` 0x42B467.
  - Apply: `Maps[it->second].PMap->ApplyScriptedMapBrush(name, apply)`.
  - Deregister: the inline STLport `erase(iterator)`, i.e. `_Rebalance_for_erase` 0x42A34E (`__cdecl`, `add esp,10h`), the CCharString dtor, `if (p) free(p)`, `--_M_node_count`.
  - Uses a local CWorldMap model: the retail map at +0x14 is 12 bytes, and Maps is at +0x20.
- 005003e0 AddToWorkBuildingList (attempt 2):
  - Body: `find`, then `CDefPointer<const CSimBuildingDef> def = it->first->PeekPSimDefinition()`, then `if (def.Object) { if (size() + 1 < WorkSpaces) push_back(creature); else erase(it); }`.
  - Needs same-TU `auto_inline(off)` bodies for `map::find` 0x513420 and `map::erase(iterator)` 0x51A2F0. Without them VC gives a 3-word frame and spills `creature`.
  - Needs the STLport list shape: an iterator with a user copy ctor, `push_back` = `insert(end(), x)`, and `size()` as a distance walk.
- 00513210 / 00513270 `random_shuffle<CThingBuilding**/CThingAICreature**, RandFunc>`:
  - `__fastcall`.
  - `iter_swap` is inline.
  - `RandFunc` is `{ unsigned long* Seed; long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); } }`, with GFFloatRandom 0x4978A0 declared `float __fastcall (float, unsigned long*)`.
- 00515570 `__adjust_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>` (first grade):
  - STLport loop (`secondChild = 2 * (secondChild + 1)`, then the `secondChild == len` tail).
  - The comparator call is on the by-value argument itself.
  - Ends with the out-of-line `__fastcall __push_heap` 0x5140A0; topIndex lives in the spare `push ecx` word.

### Old map-header serialise chain (1)
- 00515fa0 `CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::GetMaxSize` (full MATCH, attempt 4):
  - Form: `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T)); return MAX(TOld::GetMaxSize(version, size), sizeof(T));`, with `MAX` the macro ternary and the base `CBaseSerialise::GetMaxSize` returning `MAX(size, sizeof(T))`.
  - Other forms fail. A named local folds the v2 test (53 bytes). MAX over a ternary stops the inlining (72). `__forceinline` gives 158.

## Parked for Fable 5.1 (4)
- 004f932c std::vector<CAttackHistoryCombo>::operator= DIFFER(214v220): tail placement copy calls the implicit CAttackHistoryCombo copy ctor out of line where retail inlines it (call 0x4F9408 + dword float move) | tried: plain STLport form (220v220, extra destroy-tag stosb), __type_traits local typedef tags, visible __copy_ptrs body (copy tag store dropped, regs shift), visible __destroy_aux body + ~vector() (destroy matches, copy ctor out of line) | next: visible __destroy_aux body WITHOUT declaring ~vector() (or a __forceinline implicit-style copy ctor that copies the float as a dword) -- attempt 2 + 4 combined
- 005148d0 CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp DIFFER(214v209): register allocation only -- retail spills the hidden result pointer ([esp+10h]) and keeps result.Object/Info/held.Info in ebx/esi/ebp with `test`; ours keeps the hidden pointer in ebp, spills result.Object and adds a zero register (`xor ebx,ebx ; cmp esi,ebx`, `cmp byte ptr [edi+4],1` for retail's `mov al,[edi+4] ... cmp al,1`) | tried: landed 0x0051B940 model, version local + default-constructed held, same-TU previous-step body, named raw header local | next: lane gap: spill choice (hidden result vs SRA'd counted-pointer field); retail's own v7->v8 step 0x0051B940 shows the ours-shape, so try the TU's real neighbours (GetMaxSize/other steps compiled first)
- 00516010 CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp DIFFER(214v208): same register-allocation gap as 0x005148D0 (retail result.Object in ebp, held.Info in ebx) | tried: the 0x005148D0 model, explicit `return CCountedPointer<..>(result)` temporary | next: lane gap: as 0x005148D0
- 005154c0 std::__unguarded_insertion_sort_aux<CThingBuilding**,CThingBuilding*,CIsHouseLargerThan> DIFFER(98v96): retail homes the empty comparator copy over the spilled `last` word (`push ecx ... pop ecx`, `lea ecx,[esp+18h]` = last's slot) where the lane reserves its own word (`sub esp,8 ... add esp,8`), shifting the loop-head padding | tried: plain STLport form (100v96, byte copy `mov al,[esp+20h] ; mov [esp+18h],al` + ebx/ebp swap), binary_function base (PDB lists it beside the class; no change), empty user default+copy ctors (copy gone, registers match, 98v96) | next: lane gap: empty temporary over a live slot (the 0x00A76C70 / 0x007703B0 family)

## Skipped
- None. 00513210 is one function followed by int3 padding inside its 96-byte row, not two functions. All row lengths agreed with the scanner extents.

## Label corrections
- 004edd13 "Vector_ReallocateInsert_0x30" = `vector<CPrizeScoreDef>::_M_insert_overflow`.
- 004f9fa3 "CActiveFile_OnReadFinished" = `vector<CCardPositionsDef>::_M_insert_overflow`.
- 004fa434 "ShoveHaPennyRegionDef_OnReadFinished" = `vector<CCardDef>::_M_insert_overflow`.
- 004f932c "Vector_ReassignAndRelocate" = `vector<CAttackHistoryCombo>::operator=`.
- 004f9408 "std_vector_copy_pairs" = the `vector<pair<EAttackHistoryType,EDamageAttribute>>` copy ctor.
- 004f9484 "Vector_Reallocate_CopyPairs" = `vector<CAttackHistoryCombo>::_M_allocate_and_copy`.
- 004f94d2 "VectorReallocateCopyElements" = `__copy_ptrs<CAttackHistoryCombo*>`.
- 004f9db7 "CActiveFile_CopyAssign_Loop" = `__copy_ptrs<CCardPositionsDef*>`.
- 004f9f62 "CActiveFile_Copy_Element" = `__copy_backward_ptrs<CCardPositionsDef*>`.
- 004fa3e3 "ShoveHaPennyRegionDef_Copy" = `__copy_backward_ptrs<CCardDef*>`.
- 0x0046EABD "CActiveFile_Copy" = `vector<C2DVector>::operator=`.
- 0x004EABC2 / 0x004EBD8D / 0x004EC01C / 0x004F963D = the vectors' `_M_clear`.
- 0x004EAC30 / 0x004EBDF1 / 0x004EC08A = `_Construct` instances (`__fastcall`).
- 004fb0a0 "GetThingFilenameFromLevelFilename" (propagated from its twin) = `CWorldMap::GetThingFilenameFromWorldFilename`. Evidence: the L"gtng" literal and the PDB pair order.
- 0x0099AED0 "CCharString" = `CWideString::CWideString()`.
- 004fe550 "CreatureMode_ApplyAnimatedBrush" = `CWorldMap::ApplyScriptedMapBrush`.
- 004fe5b0 "ParticleMorphs_Entry_Erase" = `CWorldMap::DeregisterScriptedMapBrush`.
- 0x0082E250 "CModeBlock::GetAnimFilter" = a folded `lea eax,[ecx+48h]` getter, used here as `CScriptedMapBrush::GetName`.
- 0x0042B467 "StdMap_FindLE" = rb-tree `_M_find`.
- 0x0042A34E "CMemoryAllocatorVariableSize::Erase" = `_Rb_global<bool>::_Rebalance_for_erase`.
- 005003e0 "ListNode_InsertPointer" = `AddToWorkBuildingList`. Its twin 0x00500490 is AddToHomeBuildingList (LiveSpaces).
- 0x00513420 "RBTree::FindPredecessorNode" = `map<CThingBuilding*, list<CThingAICreature*>>::find`.
- 0x0051A2F0 "ListNode_RemoveAndSort" = that map's `erase(iterator)`.
- 0x0082D2D0 "CThingBuilding::PeekPMaterial" = `CThingBuilding::PeekPSimDefinition`.
- 0050e500 `fill<short*,short>`: the STLport spelling is the same (no change).
- 0050fa40 "std_vector_allocate_ptrs" = `_Vector_base<void*>(n, a)`.
- 005114f0 "TreeNode_DeepClone_Generic" = `_Rb_tree<long,...>::_M_copy`.
- 00513300 "CCharStringTreeNode_DeepClone" = `_Rb_tree<CCharString,...>::_M_copy`.
- 00512120 "List_Assign_Destroy" = `list<C2DLineF>::erase(first, last)`.
- 00512630 "Vector_InitFromValue" = `vector<unsigned long>(n, value, a)`.
- 00513210 "PopulateRegionWithVillagers_FloatRandom" = `random_shuffle<CThingBuilding**, RandFunc>`.
- 00513270 "std_random_shuffle_Fisher_Yates" = `random_shuffle<CThingAICreature**, RandFunc>`.
- 005135d0 "Vector_InsertN_uint16" = `vector<unsigned short>::_M_insert_overflow` (`__true_type`).
- 005136b0 "LoadDataStream_AllocateSurface" = `vector<unsigned char>(n)`.
- 00513b00 "_Fill_n<unsigned long*,unsigned int,unsigned long>" (Dinkumware, propagated) = `__uninitialized_fill_n<CRGBColour*,unsigned int,CRGBColour>`.
- 00513b60 "std_list_link_nodes" = `list<long>::_M_insert_dispatch` (range insert).
- 005148d0 "CEngineWorldMap_MoveConstructor" = `CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp`.
- 00516010 "CEngineWorldMap_MoveConstructor_Type2" = `CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp`.
- 0x00512C70 "CEngineWorldMap::CreateNewBankMap" = `CBaseSerialise<COldFileFormatMapHeader1,1>::ConvertPreviousUnknownVersionUp`.
- 0x0050D1F0 / 0x0050D180 / 0x0050D230 "operator_new" = `CCountedPointer<T>::DeleteData`.
- 00515fa0 "GetMaxSize" = `CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::GetMaxSize`.
- 005151d0 "ListWrapper_InsertElement" = `list<CIntelligentPointer<CThingAICreature>>::insert`.
- 005152c0 "Std_List_Construct_From_Iterator_Range" = `list<C2DLineF>::_M_insert_dispatch`.
- 00515420 "StdList_InsertNode" = a list<C2DLineF> `push_front` returning the list. The owner name CLineList::AddFront is descriptive, because no caller survives.
- 005154c0 "std_insertion_sort_CThingBuilding" = `__unguarded_insertion_sort_aux<CThingBuilding**,CThingBuilding*,CIsHouseLargerThan>`.
- 00515570 "_Adjust_heap<...>" (Dinkumware, propagated) = STLport `__adjust_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>`.
- 0x005140A0 "_Push_heap<float*...>" = `__push_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>`.
- 005155f0 "StdList_ConstructMember" = the `CNavigationAreaUpdate::CSwitchableBlockage` default ctor.
- 00515720 "StdVector_AllocateConstructCAnimatedBlock" = `vector<CWorldMap::CPlayerPositionInfo>(n)`.
- 0x00513A00 "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" = `__uninitialized_fill_n<CPlayerPositionInfo*>`.
- 005158b0 "StdVector_AllocateConstructPairScriptBase" = `vector<CLevelLoader::CObject::CLevelFiles>(n)`.
- 0x00513570 "_Uninit_fill_n<pair<..>>" = `__uninitialized_fill_n<CLevelFiles*>`.
- 00515ad0 "CWorldMap_Constructor" = `vector<CMapInfo>(n)`.
