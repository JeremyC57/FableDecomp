# Hand chunk 429 (std::__introsort_loop<CSortElement> .. CEngineStaticMapFileBlockManager): 35 of 40 byte-exact

All 40 rows are module `_global`. 35 graded MATCH (5) or RELOCATION_MATCH (30) on a final gall.sh re-grade; all 35 tests
print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). 5 rows are parked in differ/. No skips.

## What matched (35)

- **CEngineScene** (4): UpdatePosition 0x00B91840 (static __fastcall: RemoveFromList, then the primitive-type
  manager's AddPrimitive unless flag 0x40), EnumerateScene 0x00B93290, InvalidateBoundingBoxesAndDrawDistances
  0x00B93500 and CheckVisibility 0x00B93CC0. All three walks are a double STLport map walk (MapList, then each row's
  inner map) with `_Rb_global::_M_increment` inlined. Each reuses the landed RenderLayerCells/ForEachUpdate model, with
  retail offsets 4 bytes below Ego_r after the 12-byte MapList.
- **CEngineSceneGrid::MovePrimitivesUp** 0x00B91F00: two unsigned loops over Size x Size. Size is re-read at every test
  and each cell is null-tested before MovePrimitivesTo(cell->Parent).
- **CEngineOcclusionManager**:
  - GetPrevFrameFlareTestResultFromRetrivalID 0x00B8D3C0: CVectorMap-style bound inlined. On a hit it tail-calls
    GetPrevFrameFlareTestResult with the handle written over the argument slot; on a miss it returns `0.0f`.
  - `std::__lower_bound<..., MapperPred, int>` 0x00B8F1C0: `__fastcall` template with an anonymous-namespace functor;
    the `/12` distance comes out as `imul 0x2AAAAAAB`.
  - `std::vector<pair<ulong,ulong>>::insert` 0x00B8F650: needs `__forceinline` on `__copy_backward_ptrs` and
    `__copy_backward`, otherwise both are calls.
- **CShaderRenderManager::DrawBuffersIndexed**, two overloads:
  - 0x00B8EB00: inline SelectVertexBuffer + a `__forceinline` 6-arg overload.
  - 0x00BA7DB0: texture loop through a `__forceinline` `CRenderManagerCore::SetTexture(long, const CTexture*)` overload,
    then the 6-arg overload.
  - Retail offsets: RenderManager at +0x1D0 and D3DDevice at +0x1D4 (Ego_r less 0x10 for four 12-byte CArrays).
- **CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks** 0x00BA1110: static __fastcall. The first bool
  is dead; the second picks the DegenerateTriangles pass after IndexBuffer.
- **CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade** 0x00BAA460:
  - a local CEngineFadeDistance with a user constructor, returned through the hidden pointer (no NRVO);
  - its end clamped by a by-value `Clamp(value, lo, hi)` template (`value < lo ? lo : (value > hi ? hi : value)`);
  - its start moved by the same amount.
- **CEngineClothRenderer::GetPositionBuffer / GetNormalBuffer** 0x00BB1E30 / 0x00BB1E90: regrow from the frame pool
  using the landed pool array-new nested conditional. The pools sit at +0xA8 / +0xCC (Ego_r less 0x18).
- **CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const CMatrix3x4&)** 0x00BB6450:
  - the C3DVector overload is inlined, written as `(position - camera).GetLength()`;
  - GetPos() must build its result member by member, because a block copy gives integer moves;
  - the single expression `X*X + Y*Y + Z*Z` squares in the order Z, Y, X, matching retail.
- **CEngineInternalPrimitiveMorphInfo** (texture decals):
  - `CVectorMap<pair<long,long>,CEngineTextureDecal>::LowerBound` 0x00BD8480: a hand-written `count/2` (cdq) bound, with
    the `std::pair` operator<.
  - GetTextureDecal 0x00BD8800 (descriptive name):
    - the find is `(it == end() || Compare(key, *it)) ? end() : it`;
    - the out-of-line LowerBound has to be a same-TU `auto_inline(off)` + `noinline` stand-in, or the key is reloaded
      from the frame.
- **CEngineLandscapeMemoryManager::CEngineLandscapeMemoryManager(const CEngineMemoryInit&)** 0x00BD9FD0:
  - bases: CFailedAllocationHandler with an implicit constructor (no vtable store of its own), then CMovableResource with
    a user constructor;
  - three `std::list<T*>[3]` arrays, each built by the inline `_List_base` (malloc(12), self-link);
  - the five scalar member stores.
- **CEngineStaticMapFileBlockManager::DeleteUnusedBlocks** 0x00BE04F0: map walk with erase. It discards the async load
  when that load has not finished, then releases the counted pointer.
- **STLport `_Rb_tree`** (5):
  - `_M_insert` 0x00B96D70 (map<EEngineRenderLayer, CEngineSceneRenderStats>);
  - `_M_copy` 0x00B97980 (inner MapList map) and 0x00BD8B30 (map<ulong, CArray<C3DVector>>);
  - `_M_erase` 0x00B98350 (outer MapList: the inner map's dtor is inlined, its `_M_erase` stays out of line);
  - `map::operator[]` 0x00BD9110 (`__default_constructed` plus insert over insert_unique; a same-TU noinline copy-ctor
    stand-in was needed).
  - All of these reuse the landed STLport 4.6 shapes.
- **STLport `list`** (5):
  - `list<CEnginePrimitiveHandle>`: erase(first, last) 0x00BBD6B0, `_M_fill_insert` 0x00BBDAD0 and resize 0x00BBDC60
    (the by-value iterators are built in their argument slots);
  - `list<long>`: `_M_fill_insert` 0x00BBD770 and resize 0x00BBDA70.
- **STLport `vector`** (3):
  - `vector(size_type n)` for 32-byte CTVertexNormTex1 0x00BB9FD0 and 48-byte CMatrix3x4 0x00BD8730 (element types
    descriptive), both with the real `_Vector_base`;
  - `vector<CEnginePrimitiveHandle>::reserve` 0x00BC5E90: retail homes the `__false_type` tag in the live `n` slot, so
    the `__uninitialized_copy` callee needs a same-TU noinline stand-in.
- **STLport algorithms** (3):
  - `__introsort_loop<CSortElement*, CSortElement, int, less<CSortElement>>` 0x00B867A0: `__fastcall` templates, an inline
    `CSortElement::operator<`, and `less() { }`;
  - `uninitialized_fill_n` 0x00BBD580 (pair<bool,long>, descriptive element);
  - `__uninitialized_copy<C2DLineF*>` 0x00BDB750.

## Parked for Fable 5.1 (5)

- 00b96350 CShadowClipVolume::ClipSphere DIFFER(208v208): only the x87 term order of the plane dot product differs. Retail does `fld Z ; fmul`, then X with `faddp`, then Y, with the induction pointer at the plane's Y; ours does Y, X, Z with the pointer at Z | tried: `X*v.X + Y*v.Y + Z*v.Z` as centre.Dot(normal) and as normal.Dot(centre), forwarding to an inline ClipSphere(const C3DVector&, float) (left a call), centre-before-radius locals | next: a Dot spelled `X*v.X + Z*v.Z + Y*v.Y`, or the plane loop over a `const CPlane&` local
- 00b9d670 CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures DIFFER(178v179): scheduling only. Retail loads g_pRenderManagerCore at entry, before the first stage test, and keeps block/stages in ebx/ebp; ours loads it after the texture select | tried: a GetD3DTexture helper, a `__forceinline` SetTexture(long, const CTexture*) with the -1 test inside (got the entry load, 177v179, but hoisted the texture load and folded stage+1 into `inc esi`) | next: the same `__forceinline` overload with the -1 tests OUTSIDE it, as 0x00BA7DB0 matched
- 00ba7b30 CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade DIFFER(153v172): retail converts the fade fraction with a bare `fld [f] ; mov edx,[p] ; fistp dword ptr [edx]` (an inline-asm float-to-int helper, with the int homed in the dead distance slot); ours calls `_ftol2`, and everything before the conversion matches | tried: the C++ `(int)` cast (1 grade, as the rules say) | next: lane gap: retail_asm. Land as grade retail_asm with a minimal inline-asm helper and a `// Retail-asm:` line
- 00bba1e0 CEngineInternalPrimitiveParticleGroup::`scalar deleting destructor' DIFFER(69v82): retail inlines the compiler-generated destructor (FreePrimitiveIDs, PrimitiveIDToIterator, Primitives, then the CIVCountedPointeeBase vtable); ours calls the implicit `??1` | tried: a user `__forceinline ~Group() {}` (adds the group vtable store at entry and leaves list<CEnginePrimitiveHandle>::~list as a call), no declared destructor (calls ??1), `__declspec(novtable)` (drops ??_G) | next: the wave-24m "polymorphic base as first member" model with each list member's destructor `__forceinline`
- 00bc9b80 CEngineTreeFilerBase::FindID DIFFER(116v115): the recursive FindNode inlines one level, but `inline_depth(1)` leaves the Children() helper as a call; the key-compare loop indexes key[i] and strength-reduces, where retail walks a key pointer with a fixed leaf-minus-key difference | tried: `inline_recursion(on)` + `inline_depth(1)` + `__forceinline`, an indexed compare | next: read the child table and leaf words through direct casts (no helper member) under the same pragmas, then try the compare as `node->Key()[i] != key[i]` or a pointer walk

## Skipped

None.

## Label corrections

Every row's catalog name was propagated or descriptive. These are corrected from Ego_r/Ego_d/FableWin name sets, RTTI and
callers:

- 00b867a0 Std_Introsort_8BytePartition -> `std::__introsort_loop<CSortElement*, CSortElement, int, less<CSortElement> >`
- 00b8d3c0 SunFlare_BinarySearch -> CEngineOcclusionManager::GetPrevFrameFlareTestResultFromRetrivalID
- 00b8eb00 CEngineScreen_RenderFrame -> CShaderRenderManager::DrawBuffersIndexed (vertex buffer, index buffer, primitive type) -- catalog wrong
- 00b8f1c0 BinarySearch_VertexBuffer -> `std::__lower_bound<pair<ulong, CCountedPointer<CVertexBufferWin32> >*, ulong, MapperPred, int>`
- 00b8f650 Vector_InsertElement_WithReallocation -> `std::vector<pair<ulong, ulong> >::insert(iterator, const value_type&)`
- 00b91840 UpdatePosition -> CEngineScene::UpdatePosition (static)
- 00b91f00 CEngineScene_IterateGrid_MovePrimitives -> CEngineSceneGrid::MovePrimitivesUp
- 00b93290 engine_enumerate_scene_grid_primitives -> CEngineScene::EnumerateScene
- 00b93500 InvalidateAllGridBoundingBoxes -> CEngineScene::InvalidateBoundingBoxesAndDrawDistances
- 00b93cc0 CEngineScene_CheckVisibilityAllCells -> CEngineScene::CheckVisibility
- 00b96350 CFrustum_CheckSphereVisibility -> CShadowClipVolume::ClipSphere(const C3DBoundingSphere&) -- catalog wrong
- 00b96d70 std_set_insert_int_array -> `_Rb_tree<EEngineRenderLayer, ...CEngineSceneRenderStats...>::_M_insert`
- 00b97980 NThingMap_CloneRedBlackTree -> `_Rb_tree<long, CCountedPointer<CEngineSceneMap>...>::_M_copy`
- 00b98350 Utility_DestroyRefcountedTreeNode -> `_Rb_tree<long, map<long, CCountedPointer<CEngineSceneMap> >...>::_M_erase`
- 00ba1110 RenderMeshPrimitiveBlocks -> CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks
- 00ba7b30 CFrustum_ComputeAlphaBlend -> CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float) -- catalog wrong
- 00ba7db0 CEngineScreen_RenderFrameAdvanced -> CShaderRenderManager::DrawBuffersIndexed (textures, count, vertex buffer, index buffer, primitive type) -- catalog wrong
- 00baa460 engine_get_clamped_z_coordinate -> CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade
- 00bb1e30 CSimpleMemoryPool_ReallocateBuffer -> CEngineClothRenderer::GetPositionBuffer
- 00bb1e90 VertexBuffer_Reallocate -> CEngineClothRenderer::GetNormalBuffer
- 00bb6450 ComputeDistanceToCamera -> CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const CMatrix3x4&)
- 00bb9fd0 std_vector_initialize_with_fill -> `std::vector<CTVertexNormTex1>::vector(size_type)` (element descriptive)
- 00bba1e0 CGuiControlTreePane_OnFileLoadComplete -> CEngineInternalPrimitiveParticleGroup::`scalar deleting destructor' -- catalog wrong
- 00bbd580 _Fill_n<unsigned_long*,unsigned_int,unsigned_long> -> `std::uninitialized_fill_n<pair<bool,long>*, unsigned int, pair<bool,long> >` (element descriptive)
- 00bbd6b0 DestroyLinkedListBetween -> `std::list<CEnginePrimitiveHandle>::erase(iterator, iterator)`
- 00bbd770 AllocateLinkedListNodes -> `std::list<long>::_M_fill_insert`
- 00bbda70 LinkedList_RemoveN -> `std::list<long>::resize(size_type, const long&)`
- 00bbdad0 AllocateLinkedListNodesWithRefcount -> `std::list<CEnginePrimitiveHandle>::_M_fill_insert`
- 00bbdc60 LinkedList_AddN -> `std::list<CEnginePrimitiveHandle>::resize(size_type, const T&)`
- 00bc5e90 std_vector_reserve_with_copy -> `std::vector<CEnginePrimitiveHandle>::reserve`
- 00bc9b80 CEngineTreeFilerBase_FindKeyNode -> CEngineTreeFilerBase::FindID(unsigned long*)
- 00bd8480 std_lower_bound_TextureKey -> `CVectorMap<pair<long,long>, CEngineTextureDecal, CKeyPairCompareLess<...> >::LowerBound`
- 00bd8730 MatrixBuffer_Allocate -> `std::vector<CMatrix3x4>::vector(size_type)` (element descriptive)
- 00bd8800 Texture_FindOrCreateCacheEntry -> CEngineInternalPrimitiveMorphInfo::GetTextureDecal(long, long) (descriptive name; nothing is created) -- catalog wrong
- 00bd8b30 NThingMap_CloneMapInfoTree -> `_Rb_tree<unsigned long, CArray<C3DVector>...>::_M_copy`
- 00bd9110 std_map_find_or_insert_uint -> `std::map<unsigned long, CArray<C3DVector> >::operator[]`
- 00bd9fd0 ParticleSpriteGroup_Construct -> CEngineLandscapeMemoryManager::CEngineLandscapeMemoryManager(const CEngineMemoryInit&) -- catalog wrong
- 00bdb750 _Copy_impl<C2DLineF*,C2DLineF*> -> `std::__uninitialized_copy<C2DLineF*, C2DLineF*>(..., const __false_type&)`
- 00be04f0 BankFileList_RemoveFinished -> CEngineStaticMapFileBlockManager::DeleteUnusedBlocks
- Only 00b9d670 SetupFirstTextures agrees with the catalog.

Neighbours and callees mislabelled outside this chunk (see LESSONS.md "Review at landing"):

- manifest 0x009B6300 "vector_insert" is `vector<pair<ulong,ulong> >::_M_insert_overflow`;
- 0x0042971D / 0x0042A34E are STLport `_Rb_global` rebalance helpers, not CMemoryAllocatorVariableSize;
- 0x0042AB73 is list<long>::~list, not CActiveFile::OnReadFinished;
- 0x00B8F220 is labelled a CFaction pair deleting dtor, which is suspect.
