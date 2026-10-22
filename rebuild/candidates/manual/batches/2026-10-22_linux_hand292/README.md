# Hand chunk 292 (CNavQuadTree .. CPSCSingleSprite): 21 of 40 byte-exact

All 21 are RELOCATION_MATCH. They use default flags, PASS under g++ (tall.sh) and VC7.1 (tall.py), and are listed in grades.txt.

## What matched (21)
- **CNavigatorAStar**
  - 0x00A1E370 InitialiseFromPreCalculatedRoute. Clears the search (state map, open list) and rebuilds the way-point route from the vector.
    - STLport `erase` goes through `__copy_ptrs`/`__copy_trivial`, which gives retail's `cmp eax,eax`.
    - `C2DVector::operator=(const C3DVector&)` gives the reload.
- **CNavigatorBase**
  - 0x00A472F0 GetNextNeighbour. Walks clockwise round a node's border.
    - In an x87 compare, VC computes the left operand first, so the right operand ends up in st0. Write the operands in retail's order.
- **CNecropolisGate**
  - 0x00D00050 Main (Awakening the Oracle). The gate guard warns the hero with a timed conversation, then plays the side or front intro cutscene and teleports him.
    - Uses the landed CHobbeLarder::Main 0x00D8F6D0 model as is: GameCalls, an inline CTimer (its dtor is expanded at the first-loop exits and called through the out-of-line copy 0x00CD4470 elsewhere), and `__fastcall` IsDistanceBetweenThingsUnder / GetSquaredDistanceBetweenThings.
    - The `if (IsActiveThreadTerminating()) return;` call shared by both cutscene arms is hoisted by VC. Matched on the first grade.
- **COVI_DeadFather**
  - 0x00DB8300 Main. Built on the CHTG_Audience::Main skeleton.
    - By-value `const char*` names build their CCharString temporaries before the pushes.
    - The facing angle is a single argument expression: `GetThingWithScriptName(...).GetAngleXY()`.
- **CObjectCacheGroupCollection**
  - 0x00BDE860 LoadContents. Matched by advisor C1.
    - `written = uncompressedSize + 3` is assigned after the CTemporaryBuffer ctor, so ebp holds the sum across the call.
    - A `__forceinline GetData()` accessor loads the buffer pointer into ebx before the inlined GetDataAndSkip.
    - Together these keep the callee-saved registers busy, so the zero register is created only at the loop, as in retail.
- **COpinion\***
  - 0x00478EE2 COpinionDeedEffectsDef::Transfer. Each flag is in its own `{ bool flag = X; ... }` block.
  - 0x00EF7800 COpinionMan::Main. Uses a `__forceinline ~CScriptThing`, with `#pragma inline_depth(0)` around the two exits that retail calls out of line.
  - 0x0045E8ED COpinionPersonalityTraits::TransferBinaryIn. Nine inlined persist reads into member-typed locals.
  - 0x007004B0 COpinionReactionManager ctor. Each lookup is an inline STLport find.
- **COptimisedMesh**
  - 0x00ACD5A0 QuadricOptimiserInternals::COptimisedMesh::CopyTo.
    - The counted-pointer dtor zeroes both Data and Info.
    - Caching the face count in a local gives retail's countdown loop.
    - `operator=` loads Info and Data before the identity test.
    - Writing `if (p.Data) { do {...} while }` instead of an early return gives retail's early `pop ebp`.
- **COracleTablet**
  - 0x00CFEE30 RemoveCursedTablets. VC7.1 cross-jumps the else-if chain into one shared tail.
- **COrchardFarmWhisper**
  - 0x00DCF0B0 Main. Built on the CHTG_Audience skeleton.
- **CPSC\***
  - WriteBinary rows: one stream call per member, in member order or in the original's write order.
    - 0x00AF9620 CPSCRenderSprite
    - 0x00AFEE00 CPSCRenderMesh
    - 0x00B06060 CPSCSingleSprite
    - 0x00B0B5C0 CPSCUpdateNormal
    - 0x00B1BA10 CPSCDecalRenderer
    - 0x00B12EF0 CPSCEmitterGeneric, which also needs a same-TU copy-ctor stand-in.
  - 0x00B18110 CPSCAttractor::DatabaseSerialise. Flags in their own blocks, and one shared name string.
  - 0x004550AC CInventoryDef ctor and 0x00606D20 CTCInventoryMap ctor. Both rows are catalogued as CPSCOrbit (wrong label).

## Parked for Fable 5.1 (18)
- 0x00A793D0 CNavQuadTree::ConnectHorizantalMapEdge DIFFER(593v678): `push ebp ; mov ebp,esp ; and esp,-8` aligned frame (retail plain `sub esp,44h`) | tried: early-continue vs if/else; named locals; same-TU GetNodeAt/_M_insert_overflow stand-ins (588) | next: lane gap: 8-byte C2DVector argument temporary inside nested loops forces the aligned frame (GOTCHAS chunk 232)
- 0x00A79680 CNavQuadTree::ConnectVerticalMapEdge DIFFER(593v678): same aligned frame as its horizontal twin | tried: the twin's best form with the y walk | next: lane gap: aligned frame (as 0x00A793D0)
- 0x00A1BA10 CNavigatorAStar::GetRandomNavigablePos DIFFER(642v642): the three fadds of `ahead + g_NavHeightTestOffset` take their operands in the opposite order | tried: member / free / by-value operator+, explicit component sums (all orders), static / defined / block-extern / const globals, `r += o` | next: lane gap: fadd operand order of a global + local sum
- 0x00A1CC30 CNavigatorAStar::ConstructFinalRoute DIFFER(632v632): only the radius `fstp [esp+10h]` (retail sinks it below the statistics `mov eax,[13D27B8h] ; cmp eax,[13D27C4h]`) | tried: separate declaration, const local, statistics in an inline helper | next: scheduling only; maybe a float-returning accessor around GetRadius
- 0x00A1D290 CNavigatorAStar::CalculateRoute DIFFER(817v817): frame-slot packing (retail shares the find key's word with the relaxed cost, lane with the neighbour state; 14h/18h/1Ch shifted); budget-exit `mov byte [ebx+0C1h],1` placed before the shared pops; `mov ecx,[goal] ; lea edx,[ecx+4]` vs `mov edx,[goal] ; add edx,4` | tried: if/else ordering and inline vs helper lookup, named key, wider neighbourState scope, break / continue / do-while exits, goal test as else-branch, distance wrappers, find/insert stand-ins | next: lane gap candidate (slot packing); try the state lookup as an out-of-line-looking member with a by-value key
- 0x00A1E110 CNavigatorAStar::MoveTowardNextWayPoint DIFFER(596v601): esi/edi swap (pos/this), delta-block scheduling, way-point temporary loads Y before X, SetGoal(front, front) CSE'd | tried (with advisor C1, 12 grades): delta copy + `-=`, `*= 0.5f` in place, `s = Y*Y; s += X*X`, free operators, stand-ins, /Os | next: two non-CSE-able front() accessors; out-param SteeringAgent accessor
- 0x00874790 NCameraModes::CPCMouseLook::OnInit DIFFER(749v752): the three scaled components are loaded from frame copies in retail and straight from the vector in ours; cross-product operand order | tried: operand orders, named component locals | next: lane gap: x87 operand order
- 0x00B18390 NParticleEngine::CPSCAttractor::ReadBinary DIFFER(780v785): register rotation (this ebp, size ecx/edi in retail; ebx/edi/ebp in ours) through the inlined ReadLong checks | tried: 8 read-helper shapes | next: the advisor's ReadLong-local trick (0x00BDE860) on the user-point loop
- 0x00B16AA0 NParticleEngine::CPSCOrbit::GetUpdateParam DIFFER(673v681): tangent-branch temp slots and Normalise squaring order (z,y,x vs y,z,x) | tried: Normalise forms, temp naming | next: lane gap: x87 scheduling
- 0x00B17030 NParticleEngine::CPSCOrbit::DatabaseSerialise DIFFER(991v991): loading-branch name/default CCharString slots, GetCRC pointer via edx vs eax, zero-store order, and loop-head padding (`jmp ; lea ecx,[ecx]` vs fall-through) | tried: same-TU Transfer stand-ins (unsigned long/long: 998 to 991; all four reorder the pushes) | next: stand-in shapes that only read the name
- 0x00AF90C0 CPSCRenderSprite::GetComponentCost DIFFER(-v588): 3 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere DIFFER(-v977): 1 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B0FBF0 CPSCEmitterGeneric::GetNumberOfParticles DIFFER(-v973): 5 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B0FFC0 CPSCEmitterGeneric::GetNumberOfDecals DIFFER(-v605): 3 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B1D550 CPSCDecalRenderer::DecalCollides DIFFER(-v576): 2 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B1D790 CPSCDecalRenderer::PoolDecal DIFFER(-v876): 2 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B1DC70 CPSCDecalRenderer::AddToBoundingSphere DIFFER(-v860): 4 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper
- 0x00B1DFD0 CPSCDecalRenderer::CPSCDecalRenderer DIFFER(-v931): 14 bare dword `fistp` | tried: not authored | next: lane gap: hand-written assembly fistp helper

## Skipped
- 0x0042EC7C CNewFrontendGameComponent::Run. The catalog length (965) cuts the function short. Run is one contiguous body from 0x0042EC7C to 0x0042F50A (2191 bytes), ending at `ret 4` just before the dtor at 0x0042F50B. The row's last instruction is `jmp 0x42F20F` into the rest of the body, so no source can match the truncated row. It needs a corrected catalog row.

## Label corrections
- 0x00A793D0 is CNavQuadTree::ConnectHorizantalMapEdge, not ConnectVerticalMapEdge. 0x00A79680 is ConnectVerticalMapEdge.
- 0x00ACD5A0 is QuadricOptimiserInternals::COptimisedMesh::CopyTo. The CPSCOrbit-family label is wrong.
- 0x004550AC is CInventoryDef::CInventoryDef (vtable 0x01233404), not CPSCOrbit::CPSCOrbit.
- 0x00606D20 is CTCInventoryMap::CTCInventoryMap, not CPSCOrbit::CPSCOrbit.
- 0x00B0B5C0 is CPSCUpdateNormal::WriteBinary, not CPSCDecalRenderer::WriteBinary.
- 0x00AFEE00 is CPSCRenderMesh::WriteBinary, not CPSCRenderSprite::WriteBinary.
- 0x00CFF9A0 "Main_InitializeHeroMorphComponents" is CQ_AwakeningTheOracleScript::PlayCutscene(CCharString, bool).
- 0x00BDE680 "SetMovableResource" is CLocalDetailCacheMap::CObjectTypeCollection::Load.
- 0x00BE04C0 "_Dest_val" is CEngineStaticMapFileBlockManager::GetBlockData.
- 0x00AD1E80 "CCountedPointer_MoveAssign" is C3DMeshFile::GetMeshChunk.
- 0x00CD4470 "~CTimer" is correct. 0x004AA840 "~C3DClothPrimitive" is ~CScriptThing.
- 0x00A216A0 "RBTree_FindNearestLE" is map<long, CAStarState>::find.
- 0x00A221B0 "RBTree_InsertIfNewFloat" is the map's insert.
- 0x00A22460 "PriorityQueue_Push" is CPriorityQueue<CAStarState*>::Push.
- 0x00A21C30 is CPriorityQueue::UpdateElement.
- 0x00A763F0 "operator==" is CANavQuadTreeNode::operator==.
- 0x00F35B30 "CCreatureAction_TrollWhackGroundBase::Initialise" is CEntityScriptBase::IsActiveThreadTerminating.
- 0x004859AD is the C3DVector Transfer.
- 0x0084F1B0 is GetObservedThing.
- 0x00A76320 is the smallest-node-size helper.
- 0x006CD240 is list<CNavWayPoint>::size.
- 0x00A21FC0 is push_front.
- 0x00A1A7E0 is GetConnectingPoints.
- 0x00A1AD40 is GetNavigablePosInDirection.
- 0x013D27B0..0x013D27CC are the A* search statistics: state/open/closed counts at 0x013D27B8/BC/C0, and their snapshot at peak at C4/C8/CC.
- 0x0139713C is a 50-frame countdown.
