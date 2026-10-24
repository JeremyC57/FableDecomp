# Hand chunk 363 (_Sort_heap<pair<unsigned long,CParticleUpdateParamDesc>> .. _Uninit_copy<vector<unsigned int>>): 38 of 40 byte-exact

All 38 sources grade MATCH (9) or RELOCATION_MATCH (29) in `grades.txt`. All 38 tests pass under g++ (`tall.sh`) and under VC7.1 in Wine (`tall_vc.txt`).

## What matched (38)

- **STLport `__uninitialized_copy` (24 rows, all /Gr `__fastcall`: first in ecx, last in edx, result and the `__false_type` tag on the stack, `ret 8`).** Each one placement-copies [first, last) into raw storage and returns the advanced result.
  - pair<u64, CCountedPointer<CActiveEntityScriptBase>> 00cba3e0 / 00cba460: the direct `new (result) T(*first)` loop.
  - 4-byte CIVCountedPointer 0071ea10 / 00a6afd0.
  - pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>> 005e0c60.
  - pair<float, CSubtitledMessage::CCameraTag> 004cfa10 / 004cfac0: needs STLport's `T* cur = result; ... _Construct(cur, *first); return cur;` form. The direct `new` loop strength-reduces a second destination pointer (96v83).
  - CTestQuest 004aa060 (six CCharString copy-ctor calls plus a long).
  - pair<CCharString, CRuntimeQuestPersistence::CPersistInfo> 004b8210: a nested pair/record model. The flat model differs in scheduling.
  - pair<CCharString, CCountedPointer> 00cba4a0.
  - {CCharString, long, CCountedPointer} 00a49bc0.
  - pair<unsigned long, C3DMeshVolume> 00a32f40 / 00a6fe00.
  - pair<CCharString, C3DMeshVolume> 00a6fed0.
  - pair<long, CCompositeSurfaceInfo> 00bedb60.
  - Polymorphic elements whose implicit copy ctor stores the vtable immediate: CScriptThing 008ac140, CVertexShader 00b6bab0 (CVertexShaderData count at +0x10), CPhysicsMeshInfo : CMeshDataBankEntry 00a2ae80 (two vtable stores).
  - Elements with an out-of-line copy ctor: CVectorMap (16 bytes) 00674ce0, the 0x1F4-byte record 00889880, COptimisedVertex 0xB8 00ad7b50.
  - Shape lessons:
    - STLport's explicit `pair(const pair&) : first(o.first), second(o.second)` gives the nested-copy register shapes.
    - `_Construct` and `cur` give the single advancing destination.
- **VC7 Dinkumware `vector<4-byte>::_Ucopy` (00c13ec0, 00c13ef0, 00c29480).** Thiscall, `ret 0Ch`: `memmove(ptr, first, n*4)` through the dllimport (IAT 0x01440144), then `+ n`.
- **STLport `_Rb_tree` with a CCharString key.**
  - `_M_upper_bound` 007acd30 / 008abd80 (`ret 4`, node in eax): the key's data pointer is hoisted out of the loop only when the CBasicString<char>::operator< template body (0x00429950, `__declspec(noinline)`) is in the TU. Otherwise it is re-read every iteration (65v67).
  - `insert_equal` 009d34e0 (multimap<CCharString,long>, `_M_insert` 0x0042BCB5) and 00a54b50 (multimap<CCharString,float>, `_M_insert` 0x00A546D0): the iterator result goes through the hidden pointer into `_M_insert(x, y, v, 0)`.
- **STLport `_Rb_tree` with a CThreadedFileManager::CPendingObjectSortKey key.** `_M_upper_bound` 0098fca0 and `_M_lower_bound` 009901d0. The comparator is inline: PFile->PhysicalSortKey, then Desc.FilePos, then the object address.
- **`std::find_if` 0080e2a0 (CIsFirstElementEqualTo over pair<CIntelligentPointer<CThing>,long>).**
  - VC homes the empty `random_access_iterator_tag` temporary in the predicate's argument slot only when the 4x-unrolled `__find_if` body is in the TU (`__declspec(noinline)`). With a bare declaration the tag gets its own `push ecx` byte (77v75).
  - Leaf `??$find_if@`: `find_if` also matches the larger `__find_if`.
- **`std::_Transform` (3 rows).**
  - 007896af / 007896e6 (pair<CIntelligentPointer,long>* or CIntelligentPointer* into back_inserter(vector<const CThing*>)): a size TU (`#pragma optimize("y", off)` + `optimize("s", on)`). back_insert_iterator needs a user ctor so the result goes through the hidden pointer (50v55 without it).
  - 008099b0 (CThing** into back_inserter(vector<CIntelligentPointer<CThing>>)): STLport's inline push_back with `_M_insert_overflow` 0x004AC940. The 8-byte temporary gives `and esp,-8`.
- **STLport `sort_heap` 0076b650 (pair<unsigned long, CParticleUpdateParamDesc>, 16 bytes).** `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap, `__pop_heap_aux` and `__pop_heap` all inline, reusing the element model of the landed `__pop_heap_aux` 0x0076AC70. Matched on the first try.
- **CPersistContext::Transfer(const char*, COpinionTransientOffset&) 006861c0.** Forwards to the landed three-argument Transfer 0x00685F80 with a zeroed default.
  - With POD value-initialisation `T()`, the value-slot load is scheduled after the zero stores.
  - A user default ctor that zeroes the six fields gives retail's order.
- **libpk red-black InsertFixup (".\PKCTRRedblackTree.cpp").** Both are textbook (Niemann) fixups.
  - 00c18b20 (tree: vtable, allocators, nil sentinel +0xC, root +0x20): the rotations RotateLeft 0x00C18A80 / RotateRight 0x00C18AD0 are defined above under `#pragma auto_inline(off)`. VC then knows they keep ecx and never reloads `this`.
  - 00c1b470 (map: allocator +0, sentinel +4, root +0x1C): the rotations 0x00C1B200 / 0x00C1B250 are declared only. `this` then lives in edi and is moved to ecx before each call.

## Parked for Fable 5.1 (2)

- 0098fe70 CPendingObjectTree_0098fe70::_M_find DIFFER(150v150): the post-loop `k < key(y)` compare loads its operands into swapped registers (retail `mov ecx,[edx] ... mov edx,[eax+10h]`, ours `mov ecx,[eax+10h] ; mov edx,[edx]`); the loop and frame match | tried: if/assign, a returned ternary, `if (y != header && !comp) return y;`, inline _M_lower_bound + ternary (73 bytes, not inlined) | next: STLport's literal `iterator __j = iterator(__y); return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end() : __j;` through a real iterator class with a user copy ctor, or swap the operand order of the inline operator< (`o < *this` form)
- 006b8550 CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo DIFFER(123v123): only the first two scalar copies differ (retail `mov eax,[esp+48h] ; mov edx,[esp+40h]`, Fade before Length; ours loads Length into eax first); the vector copies, pushes and deferred 1.0f alpha stores match | tried: in-place CRadialBlurParams ctor init list (123v123, colours first), assignment from CRGBFloatColour temporaries (185v123), by-value colour ctor (187v123), member-wise assignment in the order scalars, intensity, threshold (123v123, the best form) | next: assign `params.Fade` before `params.Length` (one statement swap; the attempt budget ran out)

## Skipped

None. Every row is one function, and the oracle extents match the retail `ret`, followed by int3 padding.

## Label corrections

- 005e0c60 is `__uninitialized_copy<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>>`, not pair<u64, CCountedPointer<CActiveEntityScriptBase>> (propagated).
- 0071ea10 / 00a6afd0 are CIVCountedPointer (4-byte) copies, not the pair<u64,...> label.
- 00a32f40 / 00a6fe00 = pair<unsigned long, C3DMeshVolume>; 00a49bc0 = {CCharString, long, CCountedPointer}; 00bedb60 = pair<long, CCompositeSurfaceInfo> (all carried the pair<u64,...> label).
- 008ac140 = CScriptThing (correct family). The same CScriptThing label on the following rows is wrong:
  - 00a2ae80 is CPhysicsMeshInfo.
  - 00b6bab0 is CVertexShader.
  - 00a6fed0 is pair<CCharString, C3DMeshVolume>.
  - 00cba4a0 is pair<CCharString, CCountedPointer>.
- 004aa060 = CTestQuest; 004b8210 = pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>; 004cfa10 / 004cfac0 = pair<float, CSubtitledMessage::CCameraTag> (all labelled `_Uninit_copy<vector<unsigned int>>`).
- 00674ce0 = CVectorMap (16 bytes), 00889880 = a 0x1F4-byte record, 00ad7b50 = QuadricOptimiserInternals::COptimisedVertex (all labelled CFeatDef).
- 00c13ec0 / 00c13ef0 / 00c29480 = VC7 vector::_Ucopy (`_Umove` is the VC8 name; the NGameText::CDataBankEntry** element is propagated).
- 007acd30 / 008abd80 = STLport `_M_upper_bound` (the label `_Ubound` is the Dinkumware name). 009d34e0 / 00a54b50 = `insert_equal`, not `_Ubound`.
- 0098fca0 = `_M_upper_bound`; 0098fe70 = `_M_find`; 009901d0 = `_M_lower_bound` (all labelled `_Lbound`).
- 0080e2a0 = `std::find_if<..., CIsFirstElementEqualTo>` (labelled `_Tree<long,long,...>`).
- 006861c0 = CPersistContext::Transfer(const char*, COpinionTransientOffset&); 006b8550 = CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float, ScreenEffectPriority, const C3DVector&, float, const C3DVector&, float, float, float) (both labelled `vector<pair<long,CCountedPointer<CContainmentVolume>>>::_Ucopy`).
- 00c18b20 / 00c1b470 = libpk red-black InsertFixup (labelled `_Tree<_Tmap_traits<long,CBlendedParticleEffectSet>>::_Insert`). Their callees are also mislabelled: 0x00C18A80 / 0x00C18AD0 (`List_RemoveNode` / `List_RemoveIntNode`) and 0x00C1B200 / 0x00C1B250 (`..._Variant2`) are RotateLeft / RotateRight.
- 0076b650 is STLport `sort_heap` (labelled with the Dinkumware name `_Sort_heap`); the comparator name CParticleUpdateCCompareParamDesc<unsigned long> is right.
