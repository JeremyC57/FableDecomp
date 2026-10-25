# Hand chunk 423 (CNavRegion .. CSoundEmitterLH): 36 of 40 byte-exact

All 40 rows are `_global` in the catalog. They are mostly STLport 4.x helpers instantiated for the navigation quad tree,
3D mesh files and chunked files, plus a few class members that the catalog had not named. Grades were re-run with
gall.sh at the end: 36 rows are MATCH/RELOCATION_MATCH. Every matched row has a behaviour test that passes under g++
(tall.sh) and VC7.1 (tall.py).

## What matched (36)
- **CNavRegion** 00a83110: copy constructor. The member-wise copy inlines STLport `list(const list&)` (fresh sentinel,
  then `insert(begin(), x.begin(), x.end())`). It needs a split iterator / const_iterator pair with a user copy
  constructor, and `_M_insert_dispatch` as a same-TU noinline body.
- **STLport heap/sort over `pair<unsigned __int64, list<C2DLineF> >` (CKeyPairCompareLess)**:
  - 00a83680 `__make_heap`
  - 00a83570 `__pop_heap`: the by-value `value` is built in its argument slot with an inline list copy.
  - 00a83cc0 `__unguarded_partition`: the by-value pivot's ~list runs at exit.
  - 00a843f0 `__linear_insert`: same-TU noinline pair copy constructor and `list::operator=` stand-ins stop the key
    words being re-read.
  - 00a848c0 `__insertion_sort`
  - 00a84a60 `__partial_sort`
  - 00a84c20 `vector<pair<..> >::_M_insert_overflow`
- **CVectorMap**:
  - 00a837e0 `Insert` (u64 to CANavQuadTreeNode*): `push_back(pair(key, value)); Dirty = true;`. It needs a same-TU
    noinline `_M_insert_overflow` stand-in, or the pair temporary counts as exposed.
  - 00a83fd0 `DeallocateMemory` (long to CCountedPointer<CNavConnectorNode>): `clear(); vector empty; swap(empty);
    Dirty = false;`.
- **vector<list<C2DLineF> >**:
  - 00a84090 `erase`: `__copy` must be `__forceinline` because its loop calls an out-of-line `list::operator=`;
    `__destroy_aux` is a same-TU noinline body.
  - 00a85240 `resize(n)`: it is `resize(n, list())`, with the ~list and `_M_fill_insert` bodies in the same TU.
- **vector<CNavQuadTree::CNavigationLayer>** 00a85e40 `push_back`: the 40-byte layer copy is implicit and inline.
- **vector<unsigned short> / vector<unsigned long>**:
  - 00a87090 copy constructor (`__copy_trivial`, memmove).
  - 00a8d750 and 00a96100 `vector(n)`: the `fill_n` zero loop compiles to `rep stos`.
- **list<CWall>** 00a88b10 `push_back`: CWall is 24 bytes.
- **CScopedMemoryHandle**:
  - 00a8d190 `Clear`.
  - 00a8d7c0 `CCountedPointer<CScopedMemoryHandle>::DeleteData`: a static `__fastcall` deleter with inline
    destructor and Clear.
- **C3DPrimitive2** 00a910e0 `CalculateVertexSize`: a static `__fastcall` function with two derived vertex-shader-input
  temporaries. Their Initialise and destructors are called directly.
- **STLport fills**, all `__fastcall` templates:
  - 00a945d0 `fill<C3DTriangle2*>`
  - 00aaaf50 `fill<C3DClothQuad*>`
  - 00a96350 `fill<CCountedPointer<C3DGroup2>*>`: inline CCountedPointer::operator=.
  - 00a95970 `uninitialized_fill_n<C3DTriangle2*>`
  - 00a94950 `__uninitialized_fill_n<SKeyRecord32*>`
- **vector<C3DVertex2>** 00a96a90 `erase`: a 32-byte POD, so it compiles to `rep movsd`.
- **vector<SKeyRecord32>** 00a96b30 `_M_insert_overflow`.
- **C3DMeshFileLoader**:
  - 00a9ad00 `AddChunkType(prototype, parent)`: no NRVO, so the local is copied into the hidden result.
  - 00a9b930 `RegisterClothSubChunkType`: SubMeshCloth comes first, then PrimitiveCloth. AddChunkType is a same-TU
    noinline body, otherwise the count block is re-read after the decrement.
- **WriteToFile(CVertexCompressionParams, CDataOutputStream&)** 00aa4d20: a `__fastcall` free function with eight
  WriteFloat calls.
- **C3DMeshFileXUVTangentChunk** 00aa64e0 `AddTangent`: two inline push_backs.
- **C3DMeshFileXAnimatedPrimitiveBlockChunk** 00aa65a0 `AddVertex`: it needs a same-TU noinline `_M_insert_overflow`
  stand-in, or the tag gets its own frame byte.
- **CChunkedFileRawDataChunk** 00aa7760 `SetData`: `delete[]`, then `new[]` (0x00BFEB22), then memcpy as an intrinsic.
  It uses the shared header `engine/CChunkedFileRawDataChunk.h`.
- **CChunkedFileSuperChunk**:
  - 00aa84f0 `AddSubChunk`: SetParent, then push_back.
  - 00aa87e0 `RegisterSubChunkType`: push_back. This body is ICF-shared.
- **CSoundEmitterLH** 00aabbb0, vtable 0x0129E6EC slot 15 (SetPosition): the guards are system and active, then
  playing && !stopping. C3DVector is pack(1) and the per-sound copy is built component-wise, which gives retail's
  `lea ebp,[esi+8]` shape.

## Parked for Fable 5.1 (4)
- 00a834b0 std::__linear_insert DIFFER(111v113): retail keeps `first` in esi and loads value's count block per branch; ours hoists that load to the entry and puts `first` in edi (extra push/pop edi) | tried: declaration-only pair operator=, same-TU noinline operator= (before and after the caller), non-const operator= parameter | next: same-TU noinline copy_backward / __unguarded_linear_insert bodies (the 00a843f0 recipe) with a user CCountedPointer copy constructor
- 00a83770 CVectorMap::Insert DIFFER(111v109): retail keeps the key in ecx/edx and the bool in bl and does `mov eax,[esi+4]; add eax,10h; mov [esi+4],eax`; ours re-reads the high key word and the bool from the frame and does `add dword ptr [esi+4],10h` | tried: plain vector, make_pair, _Vector_base + _STLP_alloc_proxy, implicit pair copy | next: same-TU noinline _M_insert_overflow 0x00A82380 stand-in (the exposure fix that matched its twin 00a837e0)
- 00a852b0 CVectorMap::Insert DIFFER(153v143): in the inline list copy of the pair temporary, retail drops the proxy zero store before malloc, folds begin() of the fresh sentinel and sinks `temp.second._M_node = n` to the _M_insert_dispatch call; ours keeps `mov [esp+14h],0` and reloads `mov eax,[eax]` | tried: same-TU / declaration-only _M_insert_dispatch, no proxy zero-init, a named local pair | next: same-TU noinline bodies for the pair copy constructor 0x00A825D0, _M_insert_overflow 0x00A84C20 and ~list (the temporary's address currently escapes to declaration-only callees)
- 00a85930 std::__destroy_aux DIFFER(108v108): the SwitchableLineLists __destroy_aux tag is homed at [esp+0Ah] where retail shares [esp+0Bh] with the LineLists tag; every other byte is identical | tried: base form, no tag on the pointer _Destroy, user ~CVectorMap, STLport __destroy(value_type*) dispatch | next: lane gap: frame-slot packing of empty tag temporaries

## Skipped
None.

## Label corrections
Catalog labels (propagated `_global` names) and the names these rows actually have, with evidence in each source's
header:

| Row | Catalog label | Actual name |
|---|---|---|
| 00a83110 | List_Construct | CNavRegion::CNavRegion(const CNavRegion&) |
| 00a834b0 | Vector_InsertOrErase_CountedPointer | std::__linear_insert over (long, CCountedPointer<CNavConnectorNode>) pairs |
| 00a83570 | Std_Heap_Construct_And_Adjust | std::__pop_heap |
| 00a83680 | HeapSort_BuildHeap_PairTextureDecal | std::__make_heap over (u64, list<C2DLineF>) pairs; not texture decals |
| 00a83770 | Vector_PushBack_9ByteElement | CVectorMap<u64, bool>::Insert |
| 00a837e0 | Vector_PushBack_12ByteElement | CVectorMap<u64, CANavQuadTreeNode*>::Insert (16-byte pair) |
| 00a83cc0 | Partition_GuiTree_16ByteElements | std::__unguarded_partition (navigation, not GUI) |
| 00a83fd0 | Vector_DestroyAndClear_ParticleEmitter | CVectorMap<long, CCountedPointer<CNavConnectorNode>>::DeallocateMemory |
| 00a84090 | Vector_DestroyBackwardIterate | vector<list<C2DLineF>>::erase |
| 00a843f0 | InsertionSort_Callback_TreeNode | std::__linear_insert |
| 00a848c0 | Vector_ForEach_ConstructList | std::__insertion_sort |
| 00a84a60 | Heap_Merge_PairULong_BinaryTree | std::__partial_sort |
| 00a84c20 | Vector_InsertN_MusicSetEntry | vector<pair<u64, list<C2DLineF>>>::_M_insert_overflow (not music) |
| 00a85240 | Vector_EnsureCapacity_TreeLines | vector<list<C2DLineF>>::resize(n) |
| 00a852b0 | Vector_PushBack_ControlTreeNode_With_Sort | CVectorMap<u64, list<C2DLineF>>::Insert (no sort) |
| 00a85930 | _Copy_impl<CNavigationPosition*,...> | std::__destroy_aux<CNavQuadTree::CNavigationLayer*> |
| 00a85e40 | Vector_AssignCopy_NavQuadLayer | vector<CNavigationLayer>::push_back |
| 00a87090 | Vector_Copy | vector<unsigned short>::vector(const vector&) |
| 00a88b10 | LinkedList_InsertNodeLarge | list<CWall>::push_back |
| 00a8d190 | DiscardAsyncData | CScopedMemoryHandle::Clear |
| 00a8d750 | Vector_Construct_uint16 | vector<unsigned short>::vector(n) |
| 00a8d7c0 | DeleteData | CCountedPointer<CScopedMemoryHandle>::DeleteData |
| 00a910e0 | CalculateVertexSize | C3DPrimitive2::CalculateVertexSize (static __fastcall) |
| 00a94950 | fill<C3DAnimationStateAlpha*,...> | std::__uninitialized_fill_n<SKeyRecord32*> (32-byte record) |
| 00a95970 | _Fill_n<unsigned_long*,...> | std::uninitialized_fill_n<C3DTriangle2*> |
| 00a96100 | Vector_ZeroInit | vector<unsigned long>::vector(n) |
| 00a96350 | _Copy_backward<CCountedPointer<C3DGroup2>*,...> | std::fill<CCountedPointer<C3DGroup2>*> |
| 00a96a90 | Mesh_CopyVertexBuffer_32ByteElements | vector<C3DVertex2>::erase |
| 00a96b30 | Vector_AssignCopy_AnimationKeyFrames | vector<SKeyRecord32>::_M_insert_overflow |
| 00a9ad00 | CCountedPointer_Construct | C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk&, CChunkedFileSuperChunk*) |
| 00a9b930 | PhysicsFacesSegment_InitCountedPointers | C3DMeshFileLoader::RegisterClothSubChunkType |
| 00aa64e0 | Vector_PushBack_3Floats_Pair | C3DMeshFileXUVTangentChunk::AddTangent |
| 00aa65a0 | ParticleEffectParameter_InsertData | C3DMeshFileXAnimatedPrimitiveBlockChunk::AddVertex |
| 00aa7760 | Buffer_Reallocate_And_Copy | CChunkedFileRawDataChunk::SetData |
| 00aa84f0 | vector_push_back | CChunkedFileSuperChunk::AddSubChunk |
| 00aa87e0 | C3DMeshFileXLoader_AddMeshToQueue | CChunkedFileSuperChunk::RegisterSubChunkType |
| 00aabbb0 | Particle_Or_Emitter_SetPosition | CSoundEmitterLH vtable 0x0129E6EC slot 15 (SetPosition). Kept as `vfunc_15` because no PDB name was confirmed (descriptive name) |

**Already-landed sources whose names look wrong:**
- 0x00A9BA10 and 0x00A9BA40 landed as CreatePhysicsFacesSegment / ...FromPhysicsChunks ("MakeSegment"). They are
  `C3DMeshFileLoader::RegisterMapSubChunkType` (+0x88 MapChunkType) and `RegisterHelpersSubChunkType`
  (+0x98 HelpersChunkType), the siblings of 00a9b930. Their callee 0x00A9AD00 is AddChunkType.
