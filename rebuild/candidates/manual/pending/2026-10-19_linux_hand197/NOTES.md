# Hand chunk 197 (CMurderTwistSheriff .. CPSCDecalRenderer): 23 of 40 byte-exact

All 23 sources in src/ grade RELOCATION_MATCH (gall.sh) and their tests print PASS under g++ (tall.sh) and VC7.1/Wine (tall.py).
Pragmas/flags: only 0x0042EBB6 needs `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (in the source). No /EHsc rows.
Non-default leaves (all written as `// Leaf:` lines): ??1CPSCDecalRenderer, ??0COpinionTransientOffset, ??4COptimisedVertex,
??0COptimisedVertex, ??0CEngineWorldMap, RestartReads, ?Update@CPSCDecalRenderer, ?Init@COakValeBanditRaidBandit@@,
ConvertIndexesToPointers, and the plain names elsewhere.

## What matched (23)
- CMurderTwistSheriff (0x00EB1EC0, 0x00EB53A0, 0x00EB6200): the previous agent's three Init sources re-graded unchanged apart
  from renaming the placeholder `CBase` to `CBaseClass` (vtable 0x01231710 is CBaseClass in RTTI). Tests written.
- CBankFileAsync::RestartReads (0x009D5A90, catalog "CMusicManager::UpdateRegistrations"): lock + walk ReadsToRestart re-queuing
  each read with its bank's threaded file, then clear(). Lesson: STLport's iterator ++ is `_M_node = _Rb_global::_M_increment(node)`
  (a static helper taking the node, so the walk runs in its own register), and CCountedPointer access via `operator->`.
- CNOVI_BarrelMan / CNOVI_Bully / COakValeBanditRaidBandit Init: script-interface call lists; the parent quest is byte-packed
  (the PDB itself puts C3DVectors at odd offsets), the gate name is a scoped CCharString with the nested lookup (landed
  CM_HiddenBooty shape).
- CNavQuadTreeNode::ConvertIndexesToPointers: the index-table lookup must be an inline `map::find` wrapping an out-of-line
  `_Rb_tree::find` (retail re-reads the returned iterator from its frame slot instead of using eax).
- CEngineWorldMap ctor (0x00B423F0, catalog "CNavigationLayer::Initialise"): members in PDB order with STLport sizes; the embedded
  CStaticMapBankFile's inline ctor (CBankFileAsync base + SetIsStaticMap(true)); clamp written as an if / else-if.
- CNavigatorAStar: GetSpeedReducedForStartAndEnd (2-float C2DVector ctor + operator-, squared radius compare), SmoothCorners (four
  node pointers, legs normalised, results unused), UpdateLastPosition (outer call's argument pushed before the inner
  GetAreaCovering / GetSize call is VC's normal order).
- CNavigatorManager: IsInSameMap (C2DCoordI conversion through __ftol2; VC hoists the loop-invariant ftol pair itself),
  ActivateNavigationMap (C2DVector(C2DCoordI) temporary + C2DVector(half, half), the 0.5f helper is static).
- CNewFrontendGameComponent::Shutdown: /Os + frame pointer; `CDefPointer = NULL` is Clear() then the store; the def's release must
  be a member `if (--RefCount == 0) Destroy();` (dec flags, no re-read); CRGBColour stores in R,G,B,A order need body assignments.
- CNoise::InitPerlinNoise: GFFloatRandom/GFROR13 __fastcall helpers (landed names), `while (--i)` Fisher-Yates.
- COpinionTransientOffset ctor: sign tests are `>= 0.0f ? Pos : Neg` (test ah,1), frame counts through `n ? n : 1` long locals.
- COptimisedPrimitive::PerformContraction: the existence scan takes size() once into a local.
- COptimisedVertex operator= / copy ctor (0x00AD7050 / 0x00AD77F0): compiler-generated; the engine C3DVector has a user copy ctor
  (flat dword copies in the copy ctor) but implicit assignment (shared-destination form in operator=). Emitted via a one-line
  assignment / placement copy in the TU.
- COverheadDisplayDef::Transfer: three inline CPersistContext::Transfer<bool> (landed CActivateQuestDef template).
- CPSCDecalRenderer: dtor = novtable chain, each CEnginePrimitiveHandle's dtor is Clear() (uniqueness test, `Data = 0`) followed by
  the counted pointer's own dtor; Update's real signature is (system, float, bool&) - the temporary homed in the bool& slot gave
  the parameter order away.

## Parked for Fable 5.1 (17)
- 00eb3ba0 CMurderTwistSheriff::Init DIFFER(186v187): `mov edx,[esi+30h] ; mov eax,[ecx+7Ch] ; shl edx,4 ; lea esi,[eax+edx]` vs ours `mov esi,[esi+30h] ... add esi,edx` | tried: begin()+n, _M_start[n], signed index, no named pointer | next: lane gap: register allocation (likely really CMurderTwistSheriffGuard::Init, ID +0x30)
- 00db80c0 CNOVI_CreatedBeetle::Main DIFFER(217v232): retail duplicates `DeregisterTimer + epilogue` for the in-loop early return, lane cross-jumps into the final call | tried: explicit DeregisterTimer calls, CTimer object with inline dtor, nested if(!IsDone()), pragma t | next: lane gap: tail merging
- 00a780b0 CNavConnectorNode::~CNavConnectorNode DIFFER(236v238): retail keeps Neighbour1 in ebx / data in ebp (`mov edx,ebp ; lea edx,[edx+ecx*4]`) and pushes the shared tag first | tried: begin()/end() members, cached data local, uninitialised shared tag (by const reference, which fixed the push), end += size | next: lane gap: register allocation
- 00a77a10 CNavNavigableLeafNode::SaveToFile DIFFER(226v226): regionID homed in the origin slot (+0x10) and the count/index in the bool slot (+0xc) in retail, swapped in ours | tried: separate locals, cached key, one shared `value` local, declaration order | next: lane gap: dead-argument slot assignment
- 00a792e0 CNavQuadTree::ClearNodes DIFFER(238v238): `xor ecx,ecx ; cmp eax,ecx` + ecx zero stores vs ours `xor edx,edx ; test eax,eax` | tried: explicit pointer zeroing (DSE drops the m_last store), empty-temporary swap, memset | next: explicit zero stores through a second pointer to the array, or lane gap
- 00a77b80 CNavSwitchableLeafNode::MakeRegion DIFFER(166v169): retail pushes ebx/ebp/edi at entry with its own refusal epilogue, the lane shrink-wraps | tried: flat early return, nested success path, cached GetNeighbours local, pragma s | next: lane gap: shrink-wrapping
- 00a1ab40 CNavigatorAStar::GetNearestNavigablePosTo DIFFER(166v166): retail `fld Y ; fld X ; fstp X ; fstp Y` for the nearest centre, ours copies X with mov | tried: member stores, inline SetXY(float,float), float temporaries | next: lane gap: x87 float copy
- 00a1af00 CNavigatorAStar::IsNavLineOfSightClear DIFFER(223v227): way points built through the FPU (fld X ; fld Y ; fxch) in a 0x28 frame, lane copies with mov in 0x20 | tried: C2DVector temporaries, float-pair CNavWayPoint ctor, flat early returns | next: lane gap: x87 float copy
- 00a1df60 CNavigatorAStar::CalculateNavigableWidthOfLine DIFFER(206v206): same FPU way-point copy and a conversion temporary that overlaps a way point (0x1c frame vs 0x24) | tried: temporaries from C2DVector(x,y), named way points from a float-pair ctor | next: lane gap: x87 float copy
- 00a156d0 CNavigatorManager::AddNavigationMap DIFFER(236v237): retail reloads the list header before building insert()'s by-value iterator (hidden pointer in edx) and orders the area copy differently | tried: direct (float) members, long locals, C2DCoordI top-left temporary (fixed the frame) | next: lane gap: scheduling
- 00a157c0 CNavigatorManager::AddNavigationMap DIFFER(194v196): the same `mov ecx,[edi]` header reload before insert | tried: raw node scan, iterator-class scan | next: lane gap: CSE of the header load
- 0065f056 CNearestBuildingWithProperty::operator() DIFFER(164v164): squares summed z,x,y in retail, z,y,x in the lane whatever the source order | tried: X*X+Y*Y+Z*Z, Y*Y+X*X+Z*Z (plus the /Os pragmas and __forceinline helpers that fixed the rest) | next: lane gap: x87 sum reordering (GOTCHAS chunk 154)
- 006a2220 CTCCarrying::GetCarriedThings DIFFER(162v161): retail homes both the pushed pointer and InsertGrow's tag in the dead argument slot, the lane opens a push-ecx slot | tried: long cast into CRetailVector_dword, typed CArray<CThing*>, named local, by-value push_back | next: lane gap: dead-slot homing
- 00ac7eb0 COptimisedPrimitive::ComputeSelfContraction DIFFER(215v215): retail squares the normal distance before scaling and loads the dot's x term from the other vertex | tried: squared locals, both Dot directions, mixed-order Dot | next: lane gap: x87 operand commutation
- 00ac82a0 COptimisedPrimitive::ComputeSubContractionCost DIFFER(192v192): retail pushes ebx/ebp before spilling this ([esp+8]), lane spills between the pushes ([esp+4]) | tried: dot operand order fix (rest identical) | next: lane gap: prologue scheduling
- 00ac85d0 COptimisedPrimitive::CollapseSegments DIFFER(183v165): the empty allocator temporary overlays the live threshold slot and the byte array's data stays in ebp for the whole body | tried: CByteArray ctor with default allocator | next: lane gap: empty temporary on a live parameter slot (GOTCHAS wave 24l)
- 00b18060 CPSCAttractor::DatabaseCopy DIFFER(170v170): bitfield merge loads the destination byte once (bl) in retail, twice in the lane | tried: bool:1, unsigned char:1, inline setters, explicit mask merge | next: lane gap: bitfield RMW codegen

## Skipped
None (no row holds two functions).

## Label corrections
- 0x009D5A90 "CMusicManager::UpdateRegistrations" (forge auto-name) is CBankFileAsync::RestartReads (critical section +0x14c, ReadsToRestart +0x134).
- 0x00B423F0 "CNavigationLayer::Initialise" is CEngineWorldMap::CEngineWorldMap (vtable 0x012A1F50); 0x00B59710 is CEngineComponent::CEngineComponent.
- 0x006A2220 "COptimisedPrimitive::RemapGroups" is a CTCCarrying collector (the real RemapGroups is 0x00AC99F0, landed).
- 0x00AD7050 "COptimisedVertex::COptimisedVertex" is COptimisedVertex::operator= (calls the trees' operator=); 0x00AD77F0 is the copy ctor.
- 0x00EB3BA0 (catalog CMurderTwistSheriff::Init) indexes seh_Guards by the +0x30 ID: most likely CMurderTwistSheriffGuard::Init.
- 0x00A7A760 "CPSCOrbit::CPSCOrbit" is CNavQuadTree::CNavQuadTree; 0x00A17480 "CCombatSequence_WillForcePush::OnCreate" is
  list<CNavQuadTreeMap>::insert(iterator); 0x00A16640 is CCountedPointer<CNavQuadTree>::Reset; 0x00A762E0 "CXMVPlaybackInfo" is
  CNavQuadTree::Activate; 0x00A76320 "CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance" is a static 0.5f half-cell getter.
- 0x00A1D6E0 is the CNavWayPoint overload of CalculateNavigableWidthOfLine; 0x00A1A260 is IsRouteClear.
- 0x00AC7FF0 "BinaryTree_ComputeSubContractionCost" is a COptimisedVertex group-difference walk; 0x00AC6D50 is CQuadric::Evaluate.
- 0x00AEA9F0 "GFGetPreMainMemoryUsed" returns the particle update rate (a long) used to turn seconds into frames.
- 0x00F35B10 / 0x00F35B30 are the entity script's SetPredicate / IsActiveThreadTerminating (landed names vary: RegisterFilter,
  RegisterFunctor, IsDone, IsAborted).
