# Hand chunk 416 (CPhysicsMeshObject .. CMeshBankStateBlock): 35 of 40 byte-exact

All 40 catalog rows are `_global`; the real owners (PDB names from Ego_r.pdb / FableWin.pdb unless marked
"descriptive name") are the navigation (CNavigatorAStar / CNavigatorQueueManager / CSteeringAgent), physics-mesh,
2D-vector maths, async-loading and mesh-data-bank TUs, plus the STLport 4.6 templates instantiated in them.
Every matched row has a behaviour test passing under g++ (tall.sh) and VC7.1 /Od (tall.py).

## What matched (35)

CPhysicsMeshObject / CPhysicsMesh
- 00a11760 CPhysicsMeshObject::GetHelperPointPositionStatic -- mesh helper point pushed through the object's
  PositionMatrix. Shape: C3DVector with a user copy ctor (hidden-pointer return from the callee into a named local),
  x/y cached in locals before the three row sums.
- 00a17600 CPhysicsMesh::ResetStartInformationCount -- zero the 48-byte static CPhysicsInfomationOutputData. Shape:
  intrinsic memset (`rep stosd`, 12 dwords); the catalog "$E1" is wrong (only caller CWorld::UpdateFrame).

C2DVector
- 00a141d0 C2DVector::Rotate -- rotate by a turn fraction keeping the length. Shape: GetAngle and WrapAngle both
  `__forceinline` (plain inline GetAngle stays a call), `#pragma intrinsic(atan2, sqrt, fmod, sin, cos)`, 2pi as the
  double 6.2831854820251465.
- 00a13440 for_each<C3DVector*, CRescaleBy> -- scale every vector. Shape: /Gr fastcall free template, functor with a
  user ctor returned through memory, scale re-read from the functor's argument slot.

CAsyncLoadingMemoryPool (static vector LoadingCallbacks at 0x013D2770)
- 00a14900 ProcessLoading -- call slot 0 on every callback, end() re-read after each virtual call.
- 00a14930 RemoveLoadingCallback -- out-of-line STLport `__find` + inline single-element erase; `__find`'s body must
  be in the TU so _M_finish stays cached across the call.
- 00a14990 AddLoadingCallback -- inline push_back with `_M_insert_overflow(finish, x, tag, 1, true)`.

CNavigatorQueueManager / CNavigatorFlyer / CNavigatorAStar / CSteeringAgent
- 00a154d0 CNavigatorQueueManager::AddNavigatorToQueue -- front with rank -1, else rank = (long)(35/max(p,1)) placed
  after the first two entries past every rank >= it (or -1). Shape: STLport list with distance()-based size(),
  advance(), inline insert, pair/iterator user copy ctors.
- 00a18f70 CNavigatorFlyer::Create / 00a1cbf0 CNavigatorAStar::Create -- static fastcall factories: operator new,
  ctor, `PostConstructionInit` on the new-expression result (null `this` tail when the allocation fails).
- 00a1ca70 CNavigatorAStar::CNavigatorAStar -- retail 0xC4 STLport layout (map, three CNavBlockedQuadTreeNode,
  priority queue, route list, flags).
- 00a1b470 CSteeringAgent::CreateLineList -- three list loops calling vtable slot 3, end() re-read per call.
- 00a16550 __find_if<vector<const CANavQuadTreeNode*>::iterator, IsSameNavQuadTreeNode> -- STLport 4-way unrolled
  random-access find with the 0..3 remainder switch, fastcall.
- 00a21730 __find_if<multimap<float, const CANavQuadTreeNode*>::iterator, value_equals> -- the predicate takes the
  pair BY VALUE (the dead `second` store into the frame temporary survives).
- 00a21640 _Rb_tree<long, pair<const long, CAStarState>>::_M_erase -- recursive erase, inline ~CAStarState counting
  the three live-state statics down.
- 00a21d90 / 00a21e80 _Rb_tree::_M_insert for map<long, CAStarState> and multimap<float, const CANavQuadTreeNode*> --
  STLport 4.6 verbatim; the pair<const long, CAStarState> copy is kept out of line by an explicit-specialisation
  declaration; the x87 key compare makes VC7.1 store the null child links as immediates.
- 00a21fc0 / 00a22010 list<CNavWayPoint>::push_front / push_back -- inline insert; the list iterator needs a user
  copy ctor (else edi is pushed inside the construct branch).
- 00a22460 CPriorityQueue<CAStarState*>::Push<CAStarStateTotalGreaterThan> -- inline push_back then push_heap with
  the empty comparator copied byte-wise into the dead argument slot.

C3DAnimationInfo / CMeshDataBank / CMeshBankStateBlock
- 00a25970 C3DAnimationInfo::~C3DAnimationInfo -- counted-pointer release through a cached local, empty map walk
  kept, base vtable store last.
- 00a25b20 C3DAnimationInfo::CAnimationData::Sizeof -- STLport 4.6 static `_Rb_global::_M_increment(node)` (walk on
  a copy in eax; a member increment is 2 bytes shorter).
- 00a26410 CMeshBankStateBlock::CLODStateBlock::GetBoneLODs -- inline set::clear, then insert of every bone at the
  LOD; pair<iterator,bool> returns through a hidden pointer because STLport's pair has user ctors.
- 00a26530 CMeshDataBank::FindLipSyncAnimation -- the real CCharString Compare body above under auto_inline(off) so
  VC7.1 keeps the count and string block in registers across the call.
- 00a26900 CMeshDataBank::CloseMeshes -- every clear() as erase(begin(), end()); counted-pointer erase inline.
- 00a2afa0 __uninitialized_fill_n<C3DAnimationInfo*, unsigned, C3DAnimationInfo> -- fastcall, placement-new null
  test, MovementVector's user copy ctor keeps the copy memberwise.
- 00a2bc00 __copy<CPhysicsMeshInfo*, CPhysicsMeshInfo*, int> -- random-access copy with CIVCountedPointer operator=.
- 00a13650 / 00a2c2b0 / 00a2c360 vector<C2DLineF / CPhysicsMeshInfo / C3DAnimationInfo>::reserve -- STLport 4.6
  reserve with `_M_allocate_and_copy` inlined; the out-of-line `__uninitialized_copy` needs a same-TU noinline body
  (template under auto_inline(off) + __declspec(noinline)) to home the new block and the tag on the `n` slot.
- 00a2c210 vector<C3DMeshInfo>::reserve -- here `_M_allocate_and_copy` and `_M_clear` are both out of line
  (declarations only).
- 00a2c4e0 _Rb_tree<unsigned long, pair<const unsigned long, CCountedPointer<C3DAnimation2>>>::_M_insert -- STLport
  4.6 verbatim, CCountedPointer copy addrefs CCPPointerInfo::RefCount.
- 00a2d190 vector<CPhysicsMeshInfo>::push_back -- inline copy ctor, `_M_insert_overflow` declared only.
- 00a2cfa0 CMeshBankStateBlock::CLODStateBlock::CLODStateBlock / 00a2d650 CMeshBankStateBlock::CMeshBankStateBlock --
  inline STLport tree constructors; the inline base ctor's dead vtable store is dropped by VC7.1 (no novtable needed).

## Parked for Fable 5.1 (5)

- 00a1e030 CNavigatorAStar::UpdateGoal DIFFER(216v217): retail copies the cached steering agent (`mov eax,edi`) and loads NavigatorInterface through eax before PeekNavigatorRadius; ours loads it from edi and the saved 2 bytes shift the nop padding of the Route-size loop | tried: ~40 od variants (named/const limit, flipped compares, inline helpers, accessor members, agent-first ordering -- gives the copy but moves the vtable load to edx --, /Oa, QFE toolset); float& bestLeft/bestRight reference locals and the ternary SetGoal tail took it from 34 diffs to 1 | next: an inline accessor returning the agent by reference (or a `GetSteeringAgent()` getter) so the agent copy appears without disturbing the register allocation
- 00a13eb0 C2DVector::GetRandomUnitVector DIFFER(165v191): both inlined table lookups round with a bare `fld [mem] ; fistp dword [mem]` through their own by-value parameter copies; ours calls __ftol2 twice | tried: the plain C++ form (GFFloatRandom then inline GFFastCos/GFFastSin, one grade) | next: retail_asm with the landed FloatToLong __forceinline helper (rebuild/src/compiled/00/af/NParticleEngineCPSCLight_SmoothFadeValue_00af9000.cpp) inside the two inlines
- 00a147e0 GFFastSin DIFFER(70v86): retail `fst [esp+0Ch] ; fsub 0.5 ; fstp [esp+4] ; fld [esp+4] ; fistp dword [esp]`; ours `call __ftol2` and keeps the scaled angle in st(0) | tried: `(long)(angle - 0.5f)` (one grade) | next: retail_asm with the landed FloatToLong helper (00af9000)
- 00a14840 GFFastCos DIFFER(65v81): the same fistp sequence as GFFastSin without the -256 quarter-turn shift; ours calls __ftol2 | tried: `(long)(angle - 0.5f)` (one grade) | next: retail_asm with the landed FloatToLong helper (00af9000)
- 00a22530 GFFastArcCos DIFFER(72v88): `(c + 1) * 512.5` rounded by the bare fld/fistp pair into the acos table index; ours calls __ftol2 | tried: `(long)(c - 0.5f)` (one grade) | next: retail_asm with the landed FloatToLong helper (00af9000)

## Skipped

None.

## Label corrections

Chunk rows (catalog -> real):
- 00a11760 Matrix_ApplyToPoint -> CPhysicsMeshObject::GetHelperPointPositionStatic (Ego_r.pdb)
- 00a13440 _For_each<C3DVector*,CAddElements<C3DVector>> -> for_each<C3DVector*, CRescaleBy> (the functor multiplies;
  CAddElements adds)
- 00a13650 CArray_Reserve -> vector<C2DLineF>::reserve
- 00a13eb0 GetRandomUnitVector -> C2DVector::GetRandomUnitVector (class missing; static __fastcall)
- 00a141d0 Vector2_RotateAndScale -> C2DVector::Rotate
- 00a147e0 Math_InterpolateSineLookup -> GFFastSin (FableWin.pdb `?GFFastSin@@YIMM@Z`)
- 00a14840 Math_InterpolateSineLookup_Alt -> GFFastCos (FableWin.pdb `?GFFastCos@@YIMM@Z`; table 0x013CD550 holds
  cosines, filled with fcos at 0x00A0DBB0)
- 00a22530 Math_InterpolateTableLookup -> GFFastArcCos (FableWin.pdb `?GFFastArcCos@@YIMM@Z`; table 0x013CB530 =
  acos(k*2/1025 - 1))
- 00a14900 ProcessLoading -> CAsyncLoadingMemoryPool::ProcessLoading
- 00a14930 StackAllocator_Pop -> CAsyncLoadingMemoryPool::RemoveLoadingCallback
- 00a14990 StackAllocator_Push -> CAsyncLoadingMemoryPool::AddLoadingCallback
- 00a154d0 SortedIntList_Insert -> CNavigatorQueueManager::AddNavigatorToQueue
- 00a16550 Vector_Find_NAVNode -> STLport __find_if<..., IsSameNavQuadTreeNode>
- 00a17600 $E1 -> CPhysicsMesh::ResetStartInformationCount (not a dynamic initializer)
- 00a18f70 Create -> CNavigatorFlyer::Create; 00a1cbf0 Create -> CNavigatorAStar::Create
- 00a1b470 linked_list_for_each_call_virtual -> CSteeringAgent::CreateLineList
- 00a1ca70 CNavigatorBase_ctor -> CNavigatorAStar::CNavigatorAStar (names the base)
- 00a1e030 waypoint_update_with_width_calc -> CNavigatorAStar::UpdateGoal
- 00a21640 TreeNode_RecursiveDelete -> _Rb_tree<long, pair<const long, CAStarState>>::_M_erase
- 00a21730 BinarySearchTree_FindNextWithValue_int -> STLport __find_if<multimap<float, const CANavQuadTreeNode*>::
  iterator, value_equals<float, const CANavQuadTreeNode*>>
- 00a21d90 RBTree_InsertNodeAStar / 00a21e80 RBTree_InsertNodeFloat2 -> _Rb_tree::_M_insert (map<long, CAStarState> /
  multimap<float, const CANavQuadTreeNode*>)
- 00a21fc0 CNavigatorPathNode_Create -> list<CNavWayPoint>::push_front; 00a22010 LinkedList_InsertNodeSmall ->
  list<CNavWayPoint>::push_back
- 00a22460 PriorityQueue_Push -> CPriorityQueue<CAStarState*>::Push<CAStarStateTotalGreaterThan>
- 00a25970 CMeshDataBank_Entry_Destructor -> C3DAnimationInfo::~C3DAnimationInfo
- 00a25b20 C3DAnimationInfo_CalcSize -> C3DAnimationInfo::CAnimationData::Sizeof
- 00a26410 LTextTree_Insert -> CMeshBankStateBlock::CLODStateBlock::GetBoneLODs
- 00a26530 animation_entry_search -> CMeshDataBank::FindLipSyncAnimation
- 00a26900 CMeshDataBank_Close -> CMeshDataBank::CloseMeshes (private; Close is its caller 0x00A26C50)
- 00a2afa0 fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha> -> STLport __uninitialized_fill_n<C3DAnimationInfo*,
  unsigned, C3DAnimationInfo> (mis-propagated identical-body name)
- 00a2bc00 _Move<CPhysicsMeshInfo*,CPhysicsMeshInfo*> -> STLport __copy<CPhysicsMeshInfo*, CPhysicsMeshInfo*, int>
- 00a2c210 / 00a2c2b0 / 00a2c360 vector_reserve_generic(_0x10/_0x28) -> vector<C3DMeshInfo / CPhysicsMeshInfo /
  C3DAnimationInfo>::reserve
- 00a2c4e0 std_set_insert -> _Rb_tree<unsigned long, pair<const unsigned long, CCountedPointer<C3DAnimation2>>>::
  _M_insert (CAnimationData::Animations map)
- 00a2cfa0 StdTree_DefaultCtor_ExtraFields_a2cfa0 -> CMeshBankStateBlock::CLODStateBlock::CLODStateBlock (FableWin.pdb)
- 00a2d190 vector_push_back_generic -> vector<CPhysicsMeshInfo>::push_back
- 00a2d650 CMeshDataBank_Initialize -> CMeshBankStateBlock::CMeshBankStateBlock

Callee labels found propagated/wrong while authoring:
- 0x00A36F80 "CThingFilter_IsValid::operator()" -> CClass::PostConstructionInit(CClassFactory&) (identical COMDAT fold)
- 0x009D5620 "CVertexBufferWin32::DoSizeof" -> CBankFileAsyncData::Sizeof (identical COMDAT fold)
- 0x004ABA70 "LTextTreeWalkThrough::BuildTreeArray" -> _Rb_tree<CCharString,...>::_M_erase (set<CCharString>)
- 0x00A206F0 "_Push_heap<float*,int,float,less<float>>" -> STLport __push_heap for CAStarState* (identical body)
- 0x00A21CB0 "PriorityQueue_Reallocate" -> vector<CAStarState*>::_M_insert_overflow
- 0x00A2C960 "Vector_ReallocInsert_CScriptThing" -> vector<CPhysicsMeshInfo>::_M_insert_overflow
- 0x00A11210 "_Copy_impl<C2DLineF*>", 0x00A2AE80 "_Uninit_copy<...CScriptThing...>", 0x00A2AF20
  "_Move<CTCCameraPointScriptedSpline::CKeyCamera*>" -> STLport __uninitialized_copy(first, last, result,
  const __false_type&) for C2DLineF / CPhysicsMeshInfo / C3DAnimationInfo
- 0x004978A0 is GFFloatRandom (`?GFFloatRandom@@YIMMPAK@Z`, seed in ecx)
