# Hand chunk 424 (CSoundEmitterLH .. std::__copy): 38 of 40 byte-exact

All 40 rows are `_global` catalog rows from the sound-emitter, mesh/animation-file, vertex-stream and QuadricOptimiser
TUs. 38 match: 6 plain MATCH and 32 RELOCATION_MATCH, graded by gall.sh into `grades.txt`. Each has a behaviour test that
passes under both g++ (tall.sh) and VC7.1 (tall.py). The other 2 are parked in `differ/`.

## What matched (38)

**CSoundEmitterLH TU (sound records, 16-byte polymorphic CActiveSoundInfo)**
- 00aac530 `CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing*)`: CBaseClass base, the thing at +4, the vtable, an
  empty retail list at +0x14 and three false flag bytes. One zero register (ebx) feeds the list's pre-allocation
  zero store and the three byte stores. The position members have no initialiser.
- 00aacb10 `std::__uninitialized_copy<CActiveSoundInfo*>` and 00aacb50 `std::__uninitialized_fill_n<CActiveSoundInfo*>`:
  STLport 4.x `_Construct` loops. The placement-new null test survives. The implicit copy of the polymorphic record
  stores the vtable and then three words. fill_n uses an unsigned count (`jbe`, `dec edx ; jne`).
- 00aacef0 `vector<CActiveSoundInfo>::push_back`: STLport 4.6 push_back. The overflow arm passes `__false_type()` homed
  on the value argument slot.

**STLport vectors of PODs (C3DAnim / mesh-file TUs)**
- 00aadd80 `vector<unsigned short>::vector(size_type n)`: `_Vector_base(n, allocator_type())`, then the POD
  `uninitialized_fill_n`. That routes to the generic `fill_n` loop, which VC7.1 emits as `rep stosd ; adc ecx,ecx ;
  rep stosw`.
- 00aade80 `vector<unsigned short>::operator=`: STLport 4.6 verbatim. `__copy_trivial` uses the memmove ternary. The
  new block is homed in x's dead argument slot, and the final `_M_finish = _M_start + xlen` is tail-duplicated.
- 00aafca0 `vector<CMatrix3x4>::_M_allocate_and_copy<const CMatrix3x4*>` and 00ad04e0
  `vector<COptimisedVertex::CGroup>::_M_allocate_and_copy<const CGroup*>`: `allocate(n)` plus an inlined
  `__uninitialized_copy(..., __false_type())`. The matrix is copied with `rep movsd` x12; the 8-byte group moves through
  an integer register.
- 00aafd90 `vector<CMatrix3x4>::reserve`: STLport 4.6 reserve. capacity() and size() are signed /48 divides
  (0x2AAAAAAB) compared unsigned.
- 00ab1660 `C3DAnimationBuilder::C3DAnimationBuilder()`: `AddedBonePrefixes.push_back("x"); push_back("sub");`. Each
  literal is an implicit CCharString conversion temporary. The vtable store comes before the `_Vector_base` zero stores.
- 00abcec0 `C3DMeshFileTrianglesChunk::AddTriangle` and 00ac4110 `C3DGroup2::AddVertexBlend`: push_back inlined on the
  member. The 6-byte triangle is copied as a dword plus a word, and the 8-byte blend as two dwords. The tag is homed on
  the value argument slot.
- 00ab9dd0 `C3DMeshFileBoneChunk::GetBone() const`: a named local C3DBone2 (VC7.1 has no NRVO), so both `rep movsd`
  blocks appear: x12 for the matrix and x15 for the 60-byte result.

**Vertex stream format / shader input**
- 00ab3740 `CVertexStreamFormat::GetDataTypeSize` (static `__fastcall`): one switch with a return per case. Only the
  three 8-byte types share a case group, placed second. This gives the direct 14-entry jump table and retail's block
  order. Every case-to-size pair was checked against the retail table. The enumerator names are descriptive, since the
  PDB has none.
- 00ab3b40 `CVertexShaderInput::AddStream`: `DataStreams.push_back(CStreamInfo(streamIndex, format))`, then the flag
  test. The tag is homed on the live flag slot only when `_M_insert_overflow` has a same-TU noinline body that never
  reads its tag.
- 00ab4490 `std::__uninitialized_fill_n<CVertexShaderInput::CStreamInfo*>`: a user copy constructor chain. The
  implicit CBaseClass copy's vtable store is dead and dropped.

**C3DMeshFile / partition**
- 00ad1e80 `C3DMeshFile::GetRoot()` (non-const): `root.SetStaticCast(GetChunkedFileRoot()); return root;` with an
  inline `GetChunkedFileRoot() { return RootChunk; }`. This is the model of the landed const twin 0x00A32620.
- 00ac3810 `CCountedPointer<C3DMeshPartitionElement>::DeleteData` (static `__fastcall`): `delete (T*)data`. The
  destructor deletes both children (null test plus `??_G`), then two zero stores.

**QuadricOptimiser: arrays and sorts**
- 00acffe0 `std::__uninitialized_fill_n<COptimisedEdge*>` and 00ad04a0 `std::fill<COptimisedFace*>`: /Gr
  `__fastcall` templates.
- 00ad2730 `vector<COptimisedBone>::vector(const vector&)`: `_Vector_base(x.size(), x.get_allocator())` plus
  `__uninitialized_copy`. One register zeroes the three words and is also the null allocation.
- 00ad08b0 `__unguarded_linear_insert`, 00ad1ca0 `__unguarded_insertion_sort_aux` and 00ad2af0
  `__unguarded_partition` (CGroup with a function-pointer predicate): /Gr `__fastcall`. When an 8-byte by-value argument
  comes second, the next 4-byte argument (comp) takes edx. 00ad1ca0 inlines the insert, keeping the copy in an
  `and esp,-8` frame slot.

**QuadricOptimiser: STLport trees**
- 00ad3230 / 00ad3500 / 00ad43a0 `_Rb_tree::_M_copy` for set<unsigned long>, map<unsigned long,float> and
  map<unsigned long,CContractionReference>: STLport 4.6 with `_M_clone_node` inlined twice. 00ad3500 needs STLport
  pair's user copy constructor and its template converting constructor. Without them, x is loaded after the malloc.
- 00ad42c0 / 00ad4540 `_Rb_tree::_M_insert` for map<unsigned long,CContractionReference> and
  multimap<float,CCountedPointer<CVertexPairContraction>>: STLport 4.6 verbatim, `_Rebalance` as a `__cdecl` static.
- 00ad3c30 `_Rb_tree::erase(iterator)` for the cost multimap: `_Rebalance_for_erase(node, root, leftmost, rightmost)`,
  then the value's destructor. The counted pointer is CopyTo-style: decrement, Info re-read, release, then Data and
  Info zeroed.
- 00ad24f0 `_Rb_tree::_M_lower_bound` for map<pair<long,long>,CCountedPointer>: STLport pair `operator<`; k.first is
  hoisted out of the loop.
- 00ad4720 `_Rb_tree::insert_unique(const value_type&)` for the same map. The iterator `operator--` needs a same-TU
  `__declspec(noinline)` body (the retail `_M_decrement` walk). Then VC7.1 packs `_M_insert`'s temporary into j's slot;
  declaration-only, it adds a `push ecx` slot (177v175).
- 00ad4810 `insert_iterator<set<unsigned long> >::operator=`: `iter = container->insert(iter, val); ++iter;`, with
  `_Rb_global<bool>::_M_increment` as an inline static, walked on a register copy and stored once.
- 00ad48f0 `std::__copy(set const_iterator, ..., insert_iterator, const input_iterator_tag&, int*)`: /Gr `__fastcall`.
  The hidden return pointer goes in ecx and the tag reference in edx; the iterators go on the stack (`ret 14h`). The
  inserter's operator= stays out of line, as a same-TU noinline body.
- 00ac8960 / 00aca2b0 / 00aca380 `CIndexList::GetIntersection` / `GetUnion` / `GetSymmetricDifference`: `CIndexList result;
  set_X(..., inserter(result.Indices, result.Indices.begin())); return result;`, with set_X inlined down to `__set_X(...,
  __less(value_type*))`. less<> must derive from `binary_function`; otherwise its value-initialised temporary gets a
  zero store (204v196).
- 00ac96e0 `COptimisedPrimitive::UpdateContractions(const CSubContraction&)`: walks Vertices[sub.Vertex1]'s
  PairContractions. For each entry it copies the CContractionReference and a counted-pointer local, then calls
  ComputeContraction(*c) and UpdateCost(c). It matches only with same-TU noinline bodies for UpdateCost (its retail
  code) and ComputeContraction (a stand-in). Declaration-only, VC7.1 treats the local as escaped and re-reads
  Data/Info (252v236).

## Parked for Fable 5.1 (2)
- 00ab3810 CVertexStreamFormat::AddDataElement DIFFER(83v83): register allocation only. Retail loads finish through the owner ([ecx+8]) first, keeps usage/type in edx/esi for the placement copy, and bumps finish with load/add/store. The lane loads both arguments first, frees esi early and re-reads type | tried: brace temporary, named local, user copy ctor on CDataElement, CArray wrapper vs raw vector (all identical output) | next: same-TU noinline `_M_insert_overflow` body (the 00ab3b40 fix), otherwise lane gap: register allocation
- 00ac6d50 CQuadric::EvaluateError DIFFER(96v104): retail keeps every product separate in the order CC*z*z, 2BC*y*z, BB*y*y, 2AC*x*z, 2AB*x*y, 2CD*z, 2BD*y, 2AD*x, AA*x*x, +DD, each doubled with its own `fadd st,st`. VC7.1 factors the shared x/y/z terms and reorders the sum | tried: coefficient-first natural form, coefficient-last form, retail-grouped `x*y*2*AB` form under /Op (124 bytes) | next: lane gap: x87 sum canonicalisation (as GOTCHAS chunk 293)

## Skipped
None. No row holds two functions, and no row looked cut wrong. Every retail span ends in int3 padding or at the
next function, with no noreturn tail.

## Label corrections
Catalog/manifest labels → the names used here (PDB name sets plus callers and layouts):
- 00aac530 "NHeroInformationScreens_CBase_Constructor" → `CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing*)` (vtable 0x0129E6EC).
- 00aacb10 "_Copy_backward<C3DVector*,C3DVector*>" → `std::__uninitialized_copy<CActiveSoundInfo*>`.
- 00aacb50 and 00ab4490 "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" → `std::__uninitialized_fill_n` of
  CActiveSoundInfo and of CVertexShaderInput::CStreamInfo.
- 00acffe0 "fill<C3DClothQuad*,C3DClothQuad>" → `std::__uninitialized_fill_n<COptimisedEdge*>`.
- 00ad04a0 "_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>" → `std::fill<COptimisedFace*>`.
- 00ab9dd0 "swap<CDebugSphere>" → `C3DMeshFileBoneChunk::GetBone() const`.
- 00ab1660 "CParticleMorphs_InitializeEntry" → `C3DAnimationBuilder::C3DAnimationBuilder()`.
- 00ad2730 "Vector_Construct_CFeatDef" → `vector<COptimisedBone>::vector(const vector&)`. Its callee 0x00AD1B20 is
  catalogued as `_Uninit_copy<CFeatDef*>` but is `__uninitialized_copy<const COptimisedBone*>`.
- 00ad1e80 "CCountedPointer_MoveAssign" → `C3DMeshFile::GetRoot()`.
- 00ad3230 / 00ad3500 / 00ad43a0 "TreeNode_DeepCopySmall/Medium/Large" → `_Rb_tree::_M_copy` (three instantiations).
- 00ad42c0 "std_set_insert_uint_with_refcount" and 00ad4540 "BTreeNode_InsertValue" → `_Rb_tree::_M_insert`.
- 00ad3c30 "BTreeNode_Erase" → `_Rb_tree::erase(iterator)`.
- 00ad24f0 "RBTree_LowerBound_int64" → `_Rb_tree::_M_lower_bound` (key pair<long,long>).
- 00ad4720 "std_map_insert_pair" → `_Rb_tree::insert_unique(const value_type&)`.
- 00ad4810 "LTextTreeWalkThrough_Erase" (propagated, wrong) → `insert_iterator<set<unsigned long> >::operator=`.
- 00ad48f0 "TreeWalk_Destroy" → `std::__copy` (input-iterator form).
- 00ac8960 "BinaryTree_Copy_Simple", 00aca2b0 "BinaryTree_Copy_Transformed" and 00aca380
  "QuadricOptimiser_DetectFoldOver_BuildTree" → `CIndexList::GetIntersection` / `GetUnion` / `GetSymmetricDifference`.
- 00ac96e0 "QuadricOptimiser_ProcessVertexContraction" → `COptimisedPrimitive::UpdateContractions(const CSubContraction&)`.
- 00ac6d50 "QuadraticForm_Evaluate" → `CQuadric::EvaluateError(const C3DVector&) const`.
- 00ad08b0 "Insertion_Sort_Shift_Single", 00ad1ca0 "Insertion_Sort_Elements" and 00ad2af0 "Partition_TwoWaySwap" →
  `__unguarded_linear_insert` / `__unguarded_insertion_sort_aux` / `__unguarded_partition`.
- Descriptive labels replaced by PDB/STLport names: Vector_PushBack_SingleElement (00aacef0), Vector_Construct_uint16_v2
  (00aadd80), Vector_Assign_uint16 (00aade80), Array_Allocate_0x30Bytes (00aafca0), C3DAnimSequence_Reserve (00aafd90),
  VectorPushBack (00ab3810 AddDataElement), CVertexShaderInput_InsertElementAt (00ab3b40 AddStream),
  VertexData_AppendInterleaved (00abcec0 AddTriangle), C3DGroup2_InsertVertexInline (00ac4110 AddVertexBlend),
  Vector_Reserve_8bytes_Variant2 (00ad04e0). The labels GetDataTypeSize (00ab3740) and DeleteData (00ac3810) are right.
- Callees named along the way: 0x00AC8820 PopFrontContraction, 0x00AC92C0 ComputeContraction, 0x00AC8A60 UpdateCost,
  0x00AC7EB0 ComputeSelfContraction, 0x00AD3A10 the set<unsigned long> tree copy constructor, 0x00AD59D0 / 0x00AD6BC0 /
  0x00AD6D70 `__set_intersection` / `__set_union` / `__set_symmetric_difference`.
- Already-landed rows with wrong callee names:
  - CopyTo 0x00ACD5A0 calls 0x00AD1E80 "GetMeshChunk"; it is GetRoot.
  - PerformContraction 0x00ACD840 declares 0x00AC96E0 as "UpdateSubPair_00ac96e0"; it is UpdateContractions.
  - 0x00AD52A0 and 0x00AD5030 use non-template "RecutTree" models.
  - The tree operator= rows 0x00AD3720 / 0x00AD3E30 carry generic `CRetailTree_<addr>::operator=` names.
