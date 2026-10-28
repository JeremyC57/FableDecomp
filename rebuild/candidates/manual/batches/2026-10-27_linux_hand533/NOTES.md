# Chunk 533 (wave 25 re-queue) -- notes

10 rows, 0x009F73C0..0x00A35B70. Landed-ready: 3 RELOCATION_MATCH in src/ and 1 RELOCATION_MATCH retail_asm row in
retail_asm/. Parked: 6 rows in differ/. Every row has a PDB identity now. The catalog labels were wrong for 9 of the 10.

## Rows

| address | status | name |
|---|---|---|
| 0x009F73C0 | DIFFER 724v732 | `?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ` |
| 0x00A15140 | RELOCATION_MATCH | `?GetNextNeighbour@CNavigatorManager@@QAEPBVCANavQuadTreeNode@@ABV2@PBV2@ABVC2DVector@@PA_N@Z` |
| 0x00A18B50 | DIFFER 783v783 | `?HasCollidedWithTriangleAt@CPhysicsMesh@@ABE_NABVC3DTriangle2@@MABVC3DVector@@1MAAV3@2@Z` |
| 0x00A1A7E0 | DIFFER 895v862 | `?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z` |
| 0x00A1C700 | RELOCATION_MATCH | `??0CSteeringAgent@@QAE@PBVCNavigatorExternalInterface@@_N@Z` |
| 0x00A1CEB0 | DIFFER 992v992 | `?Initialise@CNavigatorAStar@@AAEXPBVCANavQuadTreeNode@@0ABVC2DVector@@10ABVC3DVector@@_N3M@Z` |
| 0x00A22590 | RELOCATION_MATCH (retail_asm) | `?SetSlerp@CQuaternion@@QAEXABV1@0M@Z` |
| 0x00A229E0 | DIFFER 691v689 | `??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z` |
| 0x00A27350 | RELOCATION_MATCH | `??1CMeshDataBank@@UAE@XZ` |
| 0x00A35B70 | DIFFER 1012v1022 | `?GetRotationMatrixForTransformFromTo@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@0@Z` |

## Names

### Proven (two sources each)
- 0x009F73C0 OnPostSwapScreens: Ego_r donor order (ClearAllQueries, ~CVsyncCallbackTimer, OnPreDeviceReset,
  OnPostSwapScreens; landed OnPreDeviceReset 0x009F73A0 sits right before) + the D3D query body.
- 0x00A15140 GetNextNeighbour: Ego_r donor 0xB82770 right after GetQuadTreeMemoryUsage (retail 0x00A15100) + the signature
  (`ret 10h`, a node reference, a nullable previous node, a position, a bool*).
- 0x00A18B50 HasCollidedWithTriangleAt: FableWin name set (after GetPointIntersectionFromTo; the point overload is
  0x00A18910, `ret 18h`) + 7 arguments (`ret 1Ch`) + this+0x3c = TotalPoints. The row has no callers (dead code kept).
- 0x00A1A7E0 GetIntersectionPointsBetweenNodes: Ego_r 0xB85D40 + FableWin 0x031A8110; the hidden pair<C2DVector,C2DVector>
  result, two node references and a float (`ret 10h`).
- 0x00A1C700 CSteeringAgent ctor: Ego_r + FableWin; callers CNavigatorBase ctor 0x00A47158 and CTCObstacle 0x004DF3A6.
- 0x00A1CEB0 CNavigatorAStar::Initialise: Ego_r 0xB87CD0 + FableWin; 9 dwords (`ret 24h`); reset block is the landed
  InitialiseFromPreCalculatedRoute 0x00A1E370 twin.
- 0x00A22590 CQuaternion::SetSlerp: FableWin 0x031E2170 sits inside the CRightHandedSet TU (with CQuaternion::Dot). Retail
  0x00A22590 is in the same TU, between GFFastArcCos 0x00A22530 and the CRightHandedSet(C3DAngles) ctor. Thiscall, two
  quaternion references and a float (`ret 0Ch`). The body is the landed CalculateSlerpWeights / SetSlerpFromIdentity
  slerp-weight code, storing scale0*a + scale1*b.
- 0x00A229E0 CRightHandedSet(const C3DAngles&): Ego_r 0xB8B300 + TU order + reads three angles.
- 0x00A27350 ~CMeshDataBank: Ego_r + vtable 0x0129CE94 store (RTTI CMeshDataBank : CBankFileAsync : CBankFile).
- 0x00A35B70 GetRotationMatrixForTransformFromTo: Ego_r, after GetPositionMatrixFromRHSet 0x00A358F0; __fastcall static.
- Callees, each proven:
  - 0x00A14F70 `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z`
  - 0x00A76320 `?GetSmallestNodeSize@CNavQuadTree@@SIMXZ`
  - 0x00A76340 `?SetBlockedNode@CNavQuadTree@@SIXMMJJ@Z`
  - 0x013D2D2C `?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A`
  - 0x00A75B30 `?GetSize@CANavQuadTreeNode@@QBEMXZ`
  - 0x00A26900 CloseMeshes
  - 0x00A9D730 / 0x00A9D740 C3DAnimationManager::Initialise / Uninitialise
  - 0x009D6490 ~CBankFileAsync
  - 0x009FC690 ~CResourceBank
  - 0x00A2DC20 STLport `_M_erase` of the lip-sync map
  - 0x00A2BD20 `__destroy_aux`
  - 0x00A55F90 CMatrix3x4::SetFromVectorAndAngle
  - 0x00A22F80 CRightHandedSet::Rotate
  - 0x00A22CA0 GetAngleXZ
  - 0x00A155C0 CNavigatorQueueManager::RemoveNavigatorFromQueue: the body is `jmp 0x00A16EB0`, the list remove_if, on the
    manager's +0x2c QueueManager.
  - 0x00A154D0 AddNavigatorToQueue
  - 0x00A76240 `??4CNavBlockedQuadTreeNode@@QAEABV0@ABV0@@Z` (Ego_r 0xBBBF90 + FableWin)
  - 0x00A4B230 `?GetSquaredDistanceBetweenSphereTriangle@NBBBMaths@@YIMABVC3DVector@@M000@Z`: Ego_r donor 0xB9F590, just
    before IsPosInFOVWithWeight = retail 0x00A4B670, + fastcall shape. It has no binding yet; it is used only by parked
    0x00A18B50.

### Probable
- 0x00A2D710 `??1CMeshBankStateBlock@@UAE@XZ` (member dtor order in ~CMeshDataBank; no second name source).
- 0x00A1CEB0 parameter names `useAStar`, `urgent`, `priority` are descriptive. The PDB gives types only;
  AddNavigatorToQueue receives the last two.

### Descriptive
- 0x013D27B0 `g_AStarSearchesInProgress`, 0x013D27B8/BC/C0 `CNavigatorAStar::CAStarState::NoOfStates` /
  `NoOfStatesOnOpenList` / `NoOfStatesOnClosedList` (not in any PDB name set; the spellings already landed are kept).
- `ENavQuadTreeNodeType` value 2 = the blocked-node type returned by CANavQuadTreeNode virtual slot 6 (+0x18,
  GetNodeType per the FableWin CNavBlockedQuadTreeNode name set). The enumerator name is unknown.

## Label corrections (catalog -> retail)
- 0x00A15140 CNavigatorManager_FindValidNavNode -> CNavigatorManager::GetNextNeighbour
- 0x00A18B50 Collision_TestTriangleSphere -> CPhysicsMesh::HasCollidedWithTriangleAt
- 0x00A1A7E0 CANavQuadTreeNode_GetConnectingPoint -> CNavigatorAStar::GetIntersectionPointsBetweenNodes
- 0x00A1C700 CNavigatorBase_Constructor -> CSteeringAgent::CSteeringAgent (the label is the caller's)
- 0x00A1CEB0 AStar_initialize_search -> CNavigatorAStar::Initialise
- 0x00A22590 ApplyPartialAnimationAlpha -> CQuaternion::SetSlerp
- 0x00A229E0 BuildEulerRotationMatrix -> CRightHandedSet::CRightHandedSet(const C3DAngles&)
- 0x00A27350 CGameWorld_Destructor -> CMeshDataBank::~CMeshDataBank
- 0x00A76320 GetConstFloat_0_5_00a76320 -> CNavQuadTree::GetSmallestNodeSize
- 0x00A9D740 SetMemberConst_00a9d740 -> C3DAnimationManager::Uninitialise
- 0x00A2BD20 DestroyRange_CountedPointer_00a2bd20 -> std::__destroy_aux<CCountedPointer<CMeshDataBankEntry>*>
- 0x00A155C0 (bound as CNavigatorList::remove_if / SubObject::Notify) -> CNavigatorQueueManager::RemoveNavigatorFromQueue

## Header debt
- CNavigatorManager, CANavQuadTreeNode, CNavQuadTree, CPhysicsMesh, CNavigatorAStar and CNavigatorBase are modelled
  locally. The generated engine headers carry no member functions and use Ego_r.pdb offsets, while STLport containers
  are 4 bytes smaller in retail. vland warns SHARED_HEADER_AVAILABLE for CANavQuadTreeNode, CNavQuadTree,
  CNavigatorManager, CSB, CBankFile, C3DMeshInfo and C3DAnimationInfo.
- CMeshDataBank: the generated header uses PDB offsets. Retail offsets come from the landed ctor 0x00A27030.
- The landed CNavBlockedQuadTreeNode::operator= (0x00A76240) names its members BlockedCount/ChildMask/Blocked/Partial.
  Ego_r gives the CANavQuadTreeNode base as Position (+4, a C2DVector), QuadTreeLevel +0xc and NavLayer +0xd.
- PDB CNavWayPoint members are Position / NavLayer / Removable. The landed InitialiseFromPreCalculatedRoute 0x00A1E370
  calls them Type / Valid.

## Review at landing
- STLport `_Rb_tree` template-argument count:
  - The landed `_M_erase` leaves of 0x00A21640 (and the settled 0x0043020A spelling) mangle `_Rb_tree` with 2 template
    arguments (or 4).
  - STLport 4.6 `map` instantiates `_Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc>`, which has 5.
    The landed `_M_insert` 0x00A21D90 and the map ctor 0x00A22360 already use 5.
  - NAME_FIXES gives the 5-argument `_M_erase` for 0xA21640 and `insert_unique` for 0xA221B0.
- `_Rb_tree_iterator` argument count:
  - The landed `_M_insert` 0x00A21D90 leaf mangles `_Rb_tree_iterator<value>` with ONE argument.
  - STLport 4.6 and the 0x00A22360 model both use `<_Value, _Nonconst_traits<_Value> >`.
  - The 0xA221B0 spelling in NAME_FIXES uses two arguments. The coordinator should pick one spelling for both.
- Three statics have two bound names each: 0x013D27B8/BC/C0 (`NoOfStates*` vs `g_instances*_013d27b8`).
- 0x00A1CEB0 is parked one instruction away. A retry could try other inline forms of the CAStarState copy into make_pair
  (the TotalCost fstp is scheduled after the NoOfStatesOnOpenList load in ours).
