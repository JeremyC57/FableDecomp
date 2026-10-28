# Chunk 512 -- notes (agent 512)

17 RELOCATION_MATCH (0 MATCH, 0 retail_asm), 3 DIFFER parked in differ/, 0 SKIP. Tests PASS under g++ and VC7.1 for all 17.

## Proven
- 00a730e0 `std::__introsort_loop<pair<CCharString,C3DDummyObject>*, pair<...>, int, CCompareString<C3DDummyObject>>` -- the landed sort 0x00A73600 binds this exact symbol
- 00a74180 `CPhysicsMesh::CalculateLayeredNavigationLineInfo()` (DIFFER) -- Ego_r.pdb donor order of the CPhysicsMesh navigation functions
- 00a74ac0 `vector<C2DLineF>::_M_range_insert<const C2DLineF*>` -- bound by the landed DeallocateUnusedMemory 0x00A74EA0
- 00a750e0 `vector<CArray<C2DLineF>>::_M_fill_insert` -- bound by the landed resize 0x00A75230
- 00a77780 `CNavRegion::IsConnectedToRegion(const CNavRegion&) const` (DIFFER) -- the only such signature in Ego_r.pdb
- 00a83d70 `vector<list<C2DLineF>>::operator=` -- bound by the landed 0x00A85700
- 00a84940 `vector<pair<unsigned __int64,list<C2DLineF>>>::operator=` -- bound by the landed CNavigationLayer::operator=
- 00a84270 `vector<list<C2DLineF>>::_M_insert_overflow` -- called by this vector's _M_fill_insert 0x00A84690
- 00a84690 `vector<list<C2DLineF>>::_M_fill_insert` -- called by resize 0x00A85240
- 00a85560 `std::__uninitialized_fill_n<CNavQuadTree::CNavigationLayer*, unsigned, CNavigationLayer>` -- callers 0x00A85B90 / 0x00A85CC0; member order of the landed CNavigationLayer::operator= 0x00A85700
- 00a85b90 `vector<CNavQuadTree::CNavigationLayer>::_M_insert_overflow` -- called by the landed push_back 0x00A85E40
- 00a85cc0 `vector<CNavQuadTree::CNavigationLayer>::_M_fill_insert` -- called by the landed resize 0x00A85EC0
- 00a8b6c0 `C3DMesh2::PackBoneNames(const CArray<CCharString>&)` -- Ego_r.pdb donor order GetUsedBones / PackBoneNames / RemapBoneIndices = landed 0x00A8B5F0 / this / 0x00A8B820

## Probable
- 00a72dd0 `vector<pair<CCharString,C3DMeshVolume>>::_M_insert_overflow` -- CPhysicsMesh::ContainmentVolumes element (Ego_r.pdb), the type the landed callees 0x00A6FED0 / 0x00A70EE0 model
- 00a72fa0 `std::__introsort_loop<pair<CCharString,C3DHelperPoint>*, ..., CCompareString<C3DHelperPoint>>` -- CPhysicsMesh::HelperPoints CVectorMap sort (0x00A735A0)
- 00a7eb00 `std::__find<const CNavigationPosition*, C2DVector>` (random_access_iterator_tag) -- caller 0x00A77540 is CNavQuadTree::IsActionPoint by donor order (ActionPoints at +0x50)
- 00a85050 `std::__introsort_loop<pair<unsigned __int64,list<C2DLineF>>*, ..., CKeyPairCompareLess<...>>` -- CNavigationLayer::SwitchableLineLists CVectorMap sort (0x00A853A0)
- 00a96f00 `vector<C3DTriangle2>::_M_fill_insert` -- element size / callers 0x00A97A50, 0x00F3F3A0
- 00a97040 `vector<C3DVertex2>::_M_fill_insert` -- C3DMeshFileVerticesChunk::ReadFromFile 0x00ABD150 sizes its CArray<C3DVertex2>; 32-byte C3DVertex2 (Ego_r.pdb)
- 00aa3af0 `C3DKeyframe::ToMatrix(CMatrix4x4&, bool) const` (DIFFER) -- FableWin.pdb names ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z; body = 4x4 twin of the landed 3x4 0x00A52450 (transposed rotation, translation column, bool unread); no callers in retail

## Descriptive
- 013d2d28 `NavRegionSearchID_013d2d28` -- the global BFS search stamp read by 0x00A77780 (differ only)

## Label corrections (catalog names that are wrong)
- 00a72dd0 Vector_InsertSingle_ElementAt -> vector<pair<CCharString,C3DMeshVolume>>::_M_insert_overflow
- 00a72fa0 QuickSort_Partition_Small / 00a730e0 QuickSort_Partition_Large -> std::__introsort_loop instances
- 00a74180 NavMesh_ProcessLine_Indices -> CPhysicsMesh::CalculateLayeredNavigationLineInfo
- 00a750e0 Vector_Resize_ComplexElements -> vector<CArray<C2DLineF>>::_M_fill_insert
- 00a77780 NavigationGraph_BreadthFirstSearch -> CNavRegion::IsConnectedToRegion
- 00a7eb00 Vector_FindElement_3DVector -> std::__find<const CNavigationPosition*, C2DVector>
- 00a83d70 Vector_AssignUninit / 00a84940 Vector_AssignMove -> vector operator= instances
- 00a84270 Vector_Assign_FillOrCopy_BinaryTree -> vector<list<C2DLineF>>::_M_insert_overflow
- 00a84690 Vector_Assign_FillOrCopy_TreeNodes -> vector<list<C2DLineF>>::_M_fill_insert
- 00a85050 HeapSort_BuildHeap_16ByteElements_v2 -> std::__introsort_loop instance
- 00a85560 Std_Vector_Assign_Multi -> std::__uninitialized_fill_n<CNavQuadTree::CNavigationLayer*>
- 00a85b90 Vector_AssignCopy_NavQuadTreeLayer -> vector<CNavigationLayer>::_M_insert_overflow
- 00a85cc0 Vector_Assign_QuestPersistInfo -> vector<CNavigationLayer>::_M_fill_insert
- 00a8b6c0 Resource_CompressAndSave_AnimationData -> C3DMesh2::PackBoneNames
- 00a96f00 Vector_Assign_C3DTriangle2_Vertices -> vector<C3DTriangle2>::_M_fill_insert
- 00a97040 Vector_Assign_AnimationKeyFrames -> vector<C3DVertex2>::_M_fill_insert
- 00aa3af0 Quaternion_ToMatrix4x4 -> C3DKeyframe::ToMatrix(CMatrix4x4&, bool) const
- 00a77540 CTCBuyableHouse::MovedInNotification -> CNavQuadTree::IsActionPoint (probable)
- 00994b40 CCreatureActionBase::GetFramesRemaining -> CMemoryDataOutputStream::GetLength
- 00996b30 ~CCompressedMemoryDataInputStream -> CMemoryDataOutputStream::~CMemoryDataOutputStream
- 00a73e50 Mesh_FindIntersectionPoints -> CPhysicsMesh::GenerateLineListFromBaseline
- 00a70ee0 _Destroy_range -> std::__destroy_aux
- 00a95a10 UninitCopy_Record32 / 00a96b30 CRetailVector_SRecord32::InsertGrow -> C3DVertex2 __uninitialized_copy / vector<C3DVertex2>::_M_insert_overflow

## Header debt
- engine/CMemoryDataOutputStream.h and engine/CDataOutputStream.h are flat (`__vftable` data member): no virtual dtor / GetPosition, so 00a8b6c0 models both locally (vland warns SHARED_HEADER_AVAILABLE CDataOutputStream).
- engine/CNavQuadTree.h has no nested CNavigationLayer: 00a85560 / 00a85b90 / 00a85cc0 use the donor include (`#define CNavQuadTree CNavQuadTreeLayout`) and re-derive.
- engine/CHandleBasedMemoryPool.h lacks GetPointerFromHandle and the CMovableResourceMemoryPool subclass (00a8b6c0 donor include).
- No headers for CPhysicsMesh (00a74180 local model), C3DKeyframe / CQuaternion / CMatrix4x4 (00aa3af0), C3DVertex2 / C3DTriangle2.

## Review at landing
Real STLport 4.6 spellings were used (2-arg vector, struct pair `U`, protected `_M_insert_overflow` = IAE, public `_M_fill_insert` / `_M_range_insert` / `operator=` = QAE). Existing landed bindings that spell the same address differently:
- 00a72dd0: landed push_back 0x00A73530 binds it as an invented 1-arg `vector<pair<CCharString,SNamedChunk>>` QAE member.
- 00a72fa0: landed sort 0x00A735A0 binds the invented `IntrosortLoop_00a72fa0`.
- 00a74180: landed 0x00A74360 binds the invented `ProcessNavigationLines`.
- 00a74ac0 RELOC_CONFLICT (vland): `??$__uninitialized_copy@PBVC2DLineF@@PAV1@@std@@YIPAVC2DLineF@@PBV1@0PAV1@ABU__false_type@0@@Z` resolves to 0x00A13490 here (retail 0x00A74AC0 calls 0x00A13490 from all six copy sites, const-source ones included) but to 0x00A11210 in landed reserve 0x00A13650. The two 56-byte bodies are byte-identical (both landed as UninitCopy_SElement16); bytes cannot say which spelling owns which address. Retail 0x00A74AC0 calling one address from both the PAV and PBV call sites means the two instances were folded there. Not respelled.
- 00a77780: landed 0x00A1BCA0 binds the invented `IsConnectedTo(const CNavRegion*)`.
- 00a7eb00: landed 0x00A77540 binds the invented `FindPosition`.
- 00a84690: landed resize 0x00A85240 uses the invented `vector_allocator` spelling.
- 00a85050: landed 0x00A853A0 binds the invented `IntrosortLoop_00a85050`.
- 00a85b90: landed push_back 0x00A85E40 binds it QAE (public); STLport is IAE.
- 00a85cc0: landed resize 0x00A85EC0 binds it IAE; STLport _M_fill_insert is public (QAE).
- 00a851e0: `??1CNavigationLayer@CNavQuadTree@@QAE@XZ` here vs the landed invented `CVectorTriple_00a851e0::~CVectorTriple_00a851e0`.
- 00a96f00: landed 0x00A97A50 / 0x00F3F3A0 bind an invented 1-arg vector + `CTriangleArray`; its _M_insert_overflow 0x00A71550 is labelled VertexData_BuildInterleaved; __uninitialized_copy 0x00A6E350 labelled UninitCopy_C3DTriangle2_00a6e350.
- 00a97040: landed 0x00ABD150 binds an invented `CArray<C3DVertex2>::Insert`; 0x00A96B30 bound as `CRetailVector_SRecord32::InsertGrow`; 0x00A95A10 labelled UninitCopy_Record32.
- 00a8d750: vector<unsigned short>(n) is bound as `CIndexArray` by landed code (00a8b6c0 uses the STLport ctor spelling).
- 00994b40: `?GetLength@CMemoryDataOutputStream@@QBEKXZ` here vs catalog CCreatureActionBase::GetFramesRemaining.
