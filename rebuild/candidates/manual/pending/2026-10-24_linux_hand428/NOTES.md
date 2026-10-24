# Hand chunk 428 (CEnginePrimitiveRenderer .. CEngineSubPrimitiveRenderer): 36 of 40 byte-exact

12 MATCH and 24 RELOCATION_MATCH by gall.sh. All 36 tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
Every source has a `// Leaf:` line, and each one names exactly one symbol in its object (checked with `objdump -t`).
crawl/purity.py classifies all src/, test/ and differ/ files as `genuine`. No source has a `// Flags:` line.
STLport is modelled in namespace `_STL`. Retail's RTTI strings spell it `std`, but the bytes come out the same; see LESSONS.md.

## What matched (36)
- **CEnginePrimitiveRenderer**
  - 0x00B31FC0 `ClearAllChildPrimitives(CEnginePrimitiveHandle&)` forwards to the handle's persistent primitive (vtable slot 11) when the handle holds one.
  - Shape: `if (handle.Data) handle.Data->ClearAllChildPrimitives();` through the CIVCountedPointer `operator bool`.
- **CEnginePrimitiveAttachmentManager**
  - 0x00B3ADC0 is the scalar deleting destructor. Bases CBase and CEngineComponent are modelled with real virtual destructors, and the class has an empty out-of-line destructor.
  - The 4-byte STLport list's `~_List_base` does `clear()` (0xB3A760), then `_M_node.deallocate` (`if (p) free(p)`).
- **vector<CTextureManagerPoolInfo>**
  - 0x00B3E920 `vector(unsigned n)`: the `_Vector_base(n, a)` model of the landed 0x0098CCC0. `CPixelFormat()` sets FormatIndex to -1, and `__uninitialized_fill_n` 0xB3E390 is declared only.
  - 0x00B3EA70 `_M_insert_overflow`: the full STLport 4.6 body, with same-TU `auto_inline(off)` + `__declspec(noinline) __fastcall` bodies of `__uninitialized_copy` 0xB3E340 and `__uninitialized_fill_n` 0xB3E390.
  - Supporting pieces: a `throw()` placement new, a reference-returning `(max)`, and `_M_clear` / `_M_set`.
- **CEngineStaticMapFileBlockManager**
  - 0x00B44370 is the constructor: `CCountedPointer<CBankFileAsyncEntry>` (Data(0), Info(0)) plus `_STL::map<long, CFileBlock>`.
  - It uses the header-node model of the landed CVertexGroupBlock 0x00AD9030.
- **CArray<CEngineLightingManager::CManager*Light*>**
  - 0x00B49FE0 (local lights) and 0x00B4A060 (shadowed spot lights) are `DeleteAndClear()`. This is a descriptive name: the rows are unreferenced COMDATs with no PDB name.
  - Shape: `for (i < size()) delete (*this)[i]; clear();`. The element's static `__fastcall operator delete` is `CDynamicObjectHeap::Free` on that class's heap (0x01436FB0 / 0x01437450). `clear()` goes through `__copy_trivial`, which is memmove.
- **CEnginePrimitiveRenderer2D**
  - 0x00B4A6C0 `Layer2DDistanceToZ(float)` computes the view depth `d*E33 + E43 + E23 + E13`, then `oneOverZ * ZBufferFactor + ZBufferOffset`.
  - The reciprocal has to be its own statement: `1.0f / z * F` folds to `F / z`.
- **CMatrix4x4**
  - 0x00B507E0 `PostMultiply(const CMatrix4x4&)`: a nested i/j loop into a local result, then `*this = result`.
  - VC7.1 fully unrolls the inner loop. Hand-unrolled column sums schedule differently.
- **CEngineLandscapeRenderer**
  - 0x00B670E0 `PatchWithinFadeStartSphere(long, long)` returns `|(x, y, 0) - cameraPos| < m_foregroundFadeStart`.
  - It must be written `if (...) return true; return false;` (`mov al,1` / `xor al,al`). `return a < b` widens to eax.
- **_STL algorithms over CVertexShader** (8 bytes: vptr and a CVertexShaderData* with an intrusive count)
  - 0x00B6B540 `fill`, 0x00B6B610 `__copy` (random-access, count loop), 0x00B6B790 `__copy_backward`.
  - The element's `operator=` is inline, in the landed release/share form.
- **CEngineWaterRenderer**
  - 0x00B7AD20 `SetRenderTargetToSurface(CRenderTarget&, const CSurface&)` does AttachTarget, `g_pDisplayManager->SetRenderTarget`, then `g_pRenderManager->SetViewport(C2DBoxF(0, 0, GFAXToVX(w), GFAYToVY(h)))`.
  - It needs a single full-expression, so that the GetDimensions temporaries keep separate frame slots (frame 0x20), and inline `GetWidth()` / `GetHeight()`.
  - C2DExtentsI needs a user ctor, so that it returns through a hidden pointer.
- **vector erase of background vertices**
  - 0x00B7FD70 (CTVertexWaterBackground, 56 bytes) and 0x00B80190 (CTVertexSeaBackground, 12 bytes) are `erase(first, last)`.
  - Both use the 2-level STLport chain `copy -> __copy_ptrs(__false_type) -> __copy` with all three `__forceinline`, written out by `probe/mkerase.py` from `probe/erase_body5.txt`.
  - With a non-power-of-two element size, VC7.1 will not inline the plain-inline chain (see LESSONS.md).
- **CEngineClothSimulationMeshBase**
  - 0x00B81450 `ClearStaticParticleSims()` (static `__fastcall`): `for (i < size()) if (!sim->IsRenderDataBusy()) { erase(begin()+i); --i; }`.
  - The busy test is an inline bool helper. Written inline in the condition it gives a register-only diff.
- **CEngineSimulationManager**
  - 0x00B817D0 `AddSimulationInfo(info, paused, fullUpdateOnly)`: if/else-if/else over three STLport 4-byte-list `push_back`s, which VC7.1 tail-merges.
  - Choosing the list by reference instead gives 82v79.
- **CCountedPointer<CParticleSimulation> algorithms**
  - 0x00B832B0 `_STL::fill` uses the landed 0x00AD1FC0 `operator=` form: read source Data/Info into locals, and release only when the blocks differ.
  - 0x00B83D30 `CArray::DeallocateMemory()` is `if (capacity()) { clear(); CArray<T>().swap(*this); }`. It keeps the out-of-line `__copy` 0xB83330 and `__destroy_aux` 0xB833F0, with user-ctor tags homed in the `push ecx` slot.
- **CEngineSubPrimitiveRenderer** (sort chain of CSortElement {SortValue, SubPrimitive}, `less<>`)
  - 0x00B842B0 `GetCount(ESubPrimitiveType)` uses a 24-byte retail CSubPrimitiveList at +0x18 (`lea eax,[eax+eax*2+3]`).
  - The other 15 rows are STLport sort / stable_sort: 0x00B84E20 `_Temporary_buffer::_M_allocate_buffer`, 0x00B851D0 `__unguarded_insertion_sort_aux`, 0x00B85240 `__lower_bound`, 0x00B85280 `__upper_bound`, 0x00B85390 `swap_ranges`, 0x00B855F0 `__make_heap`, 0x00B85870 `__linear_insert`, 0x00B858D0 `__merge_sort_loop`, 0x00B859A0 `__merge_backward`, 0x00B85E10 `sort_heap`, 0x00B86260 `__merge_sort_with_buffer`, 0x00B86450 `__partial_sort`, 0x00B86570 `__inplace_stable_sort`, 0x00B865E0 `__stable_sort_adaptive`, 0x00B86710 `__stable_sort_aux`.
  - The `__fastcall` templates are explicitly instantiated. The callees are same-TU noinline stand-ins, except that `operator()` stays declaration-only in `__unguarded_insertion_sort_aux`.

## Parked for Fable 5.1 (4)
- 00b6cd80 vector<CTVertexLandscapeBackground>::erase DIFFER(41v113): the copy loop is not inlined; ours calls `__copy_ptrs` out of line, retail inlines the whole 24-byte-element copy | tried: 4-level copy->__copy_aux->__copy_ptrs->__copy chain; `__forceinline __copy`; erase-only explicit instantiation; `__forceinline` copy + __copy | next: the 2-level chain (copy -> __copy_ptrs(__false_type) -> __copy) with all three `__forceinline`. That form is ungraded and is the file in differ/; the same form MATCHED the sibling rows 0x00B7FD70 and 0x00B80190.
- 00b806e0 CEngineClothSimulationMeshBase::CalculateFadeOut DIFFER(151v151): x87 only. Retail stores the distance (`fst [esp]`) after the three stack pops, ours right after `fsqrt`. Retail multiplies `SimFadeBegin*Zoom*SimRadius` / `Zoom*SimRadius`, ours `SimFadeBegin*SimRadius*Zoom` / `SimRadius*Zoom` whatever the source order | tried: both product orders; direct global accesses; a named `radius = SimRadius*Zoom` (that put Zoom first in the second product but changed the frame) | next: the named radius combined with retail's first-product shape, plus an inline distance helper.
- 00b84720 CEngineSubPrimitiveRenderer::AddSubPrimitive DIFFER(142v142): retail `fstp [esp+14h]; fld [esp+14h]; fistp dword [esp]` against ours `call __ftol` for the float-to-long sort key. The clamp to [0, 0x3FFFFFFF], the push_back through `_M_insert_overflow` 0xB85EC0 and the Manager store all match | tried: the plain C++ cast (1 grade) | next: lane gap: retail_asm (a bare `fistp dword`, an inline-asm float-to-long helper). A `// Retail-asm:` body is the candidate under the 2026-10-24 owner exception.
- 00b84b20 CEngineSubPrimitiveRenderer::SortSingleRadix DIFFER(221v232): register allocation. Retail has source in edx, count in edi and the loop-1 index in esi; ours has edi, esi and eax. The 11 extra bytes are retail's loop-1 alignment `jmp` plus a `lea esp,[esp]` pad. Loop 3 and the `and esp,-8` frame match | tried: a named shift local; an inline `radix * 10` shift; pre- and post-increment of counts; a named post-increment scatter index | next: loop 1 as a pointer walk (`const CSortElement* p = source; p != source + count`) to free esi, or zero the 1024 counts with a separate loop or memset to shift register pressure.

## Skipped
None.

## Label corrections
Propagated manifest label first, then the real name. "Descriptive" marks a name taken from what the body does because no PDB name exists.

| Address | Manifest label | Real name |
|---|---|---|
| 0x00B31FC0 | ChildPrimitive_ClearAllViaVtable | CEnginePrimitiveRenderer::ClearAllChildPrimitives(CEnginePrimitiveHandle&) |
| 0x00B3ADC0 | CGuiControlTreePane_Destructor | CEnginePrimitiveAttachmentManager::`scalar deleting destructor' |
| 0x00B3E920 | CTextureManagerPool_Initialize | vector<CTextureManagerPoolInfo>::vector(unsigned) |
| 0x00B3EA70 | CEngineResourceManager_ReallocateTextures | vector<CTextureManagerPoolInfo>::_M_insert_overflow |
| 0x00B44370 | List_InitializeNode_36Bytes_Complex | CEngineStaticMapFileBlockManager::CEngineStaticMapFileBlockManager() |
| 0x00B49FE0 | RefCountedObjectVector_Clear | CArray<CEngineLightingManager::CManagerLocalLight*>::DeleteAndClear (descriptive) |
| 0x00B4A060 | RefCountedObjectVector_Clear_Alt | CArray<CEngineLightingManager::CManagerShadowedSpotLight*>::DeleteAndClear (descriptive) |
| 0x00B4A6C0 | Perspective_TransformDepth | CEnginePrimitiveRenderer2D::Layer2DDistanceToZ(float) |
| 0x00B507E0 | Matrix_Multiply4x4 | CMatrix4x4::PostMultiply(const CMatrix4x4&) |
| 0x00B670E0 | Entity_IsWithinRangeCheck | CEngineLandscapeRenderer::PatchWithinFadeStartSphere(long, long) |
| 0x00B6B540 | _Copy_backward<CVertexShader*,CVertexShader*> | _STL::fill<CVertexShader*, CVertexShader> |
| 0x00B6B610 | _Copy_backward<CVertexShader*,CVertexShader*> | _STL::__copy<CVertexShader*, CVertexShader*, int> |
| 0x00B6B790 | _Copy_backward<CVertexShader*,CVertexShader*> | _STL::__copy_backward<CVertexShader*, CVertexShader*, int> (the label is the Dinkumware twin) |
| 0x00B6CD80 | Vector_Copy_Backward_6Word | vector<CTVertexLandscapeBackground>::erase(first, last) |
| 0x00B7AD20 | CTCActionUseReadable_SetupRenderTarget | CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget&, const CSurface&) |
| 0x00B7FD70 | Vector_Erase_56Byte | vector<CTVertexWaterBackground>::erase(first, last) |
| 0x00B80190 | Vector_Erase_12Byte | vector<CTVertexSeaBackground>::erase(first, last) |
| 0x00B806E0 | Render_CalculateClothVertexDistance | CEngineClothSimulationMeshBase::CalculateFadeOut(const C3DVector&) const |
| 0x00B817D0 | CEngineInternalPrimitiveAnimatedMesh_LinkNodeToList | CEngineSimulationManager::AddSimulationInfo (the label names the caller's class) |
| 0x00B832B0 | _Copy_backward<CCountedPointer<C3DGroup2>*,...> | _STL::fill<CCountedPointer<CParticleSimulation>*, ...> |
| 0x00B83D30 | Vector_Clear | CArray<CCountedPointer<CParticleSimulation> >::DeallocateMemory() |
| 0x00B842B0 | CEnginePrimitiveRenderer_GetIndexCount | CEngineSubPrimitiveRenderer::GetCount(ESubPrimitiveType) const |
| 0x00B84720 | Engine_AddClockVertex | CEngineSubPrimitiveRenderer::AddSubPrimitive(ESubPrimitiveType, CEngineSubPrimitiveBase*, float, CEnginePrimitiveManager*) |
| 0x00B84B20 | RadixSort_CountingPass_b84b20 | CEngineSubPrimitiveRenderer::SortSingleRadix |
| 0x00B84E20 | Vector_AllocateWithFallback | _STL::_Temporary_buffer<CSortElement*, CSortElement>::_M_allocate_buffer |
| 0x00B851D0 | _Copy_impl<CNavigationPosition*,...> | _STL::__unguarded_insertion_sort_aux |
| 0x00B85240 | BinarySearch_Lower | _STL::__lower_bound |
| 0x00B85280 | BinarySearch_LowerOrEqual | _STL::__upper_bound |
| 0x00B85390 | _Copy_impl<CNavigationPosition*,...> | _STL::swap_ranges |
| 0x00B855F0 | MakeHeap | _STL::__make_heap |
| 0x00B85870 | Insertion_HeapShift | _STL::__linear_insert |
| 0x00B858D0 | std_introsort_merge | _STL::__merge_sort_loop |
| 0x00B859A0 | Merge_TwoSortedRanges | _STL::__merge_backward |
| 0x00B85E10 | _Sort_heap<long*> | _STL::sort_heap |
| 0x00B86260 | std_introsort_heapsort_phase | _STL::__merge_sort_with_buffer |
| 0x00B86450 | PartialSort | _STL::__partial_sort |
| 0x00B86570 | std_introsort_recursive | _STL::__inplace_stable_sort |
| 0x00B865E0 | std_introsort_heapsort_phase | _STL::__stable_sort_adaptive |
| 0x00B86710 | IntroSort_Driver | _STL::__stable_sort_aux |

The manifest label for 0x00B81450 (ClearStaticParticleSims) is correct.

Neighbours found mislabelled while tracing callees (not chunk rows):

| Address | Manifest label | Real name |
|---|---|---|
| 0x00B443B0 | ~CBankFileEntryUpdateData | ~CEngineStaticMapFileBlockManager |
| 0x00B42A90 | ~CBankFileEntryUpdateData | CEngineMap's destructor |
| 0x00B3A760 | CGuiControlTreePane::SortTreeRecursively | list<CEngineInternalAttachedPrimitiveInfo>::clear |
| 0x00B3E340 | _Move<NThingMap::CMapwhoLevelInfo*> | __uninitialized_copy<CTextureManagerPoolInfo> |
| 0x00B3E390 | fill<...> | __uninitialized_fill_n<CTextureManagerPoolInfo> |
| 0x00B3EA40 | (no manifest label found) | CArray<CTextureManagerPoolInfo>::CArray(unsigned) |
| 0x00B835D0 | CScriptBase::RemovePredicate | vector<CCountedPointer<CParticleSimulation> >::erase(iterator) |
