# Chunk 757 notes (retry of 40 parked same-length rows, 302-393 bytes)

Final: 2 matched (RELOCATION_MATCH, vland PASS, behaviour tests PASS), 38 parked (differ/ + passing g++ model tests). No helper, no asm, no pragma for a residue. Every row got 10+ grades (LOG.tsv).

## Results

| address | name | status | grades | what fixed it / residue | next idea |
|---|---|---|---|---|---|
| 0047ed76 | CTCInGameMenu::SetToSave | parked | 10 | DIFFER(366v366) dl=2: x87/push scheduling: retail `fsubp st(1); push ecx` for the SetPosition float slot, ours `push ecx; fsubp st(1)` | a named y fixes the order but merges the two GetUIScale temporaries; try a named second scale temporary with a lifetime that ends after the subtraction |
| 004fec80 | CWorldMap::UpdateDestroyedThemeAreas | parked | 11 | DIFFER(337v337) dl=27: callee-saved pair swapped: retail count in ebx and index in ebp, ours the reverse (with the matching push/pop order) | retail headers (C2DBoxI / CArray) for the box and vector; then the shared-loop-counter forms again |
| 0058301a | CTCInventory::CreateEditBoxActions | parked | 10 | DIFFER(321v321) dl=2: one store scheduled early: the edit box spill to the dead argument slot (`mov [ebp+10h],esi`) follows `mov ecx,esi` in retail, `mov esi,eax` in ours | none found; the store floats freely (all binding / cast / const forms give one of two placements) |
| 005cb128 | CTCInventoryWeapons::RemoveItemFromInventory | parked | 10 | DIFFER(392v392) dl=37: callee-saved pair swapped: retail keeps this in esi and the weapon-pointer / owner temporaries in edi, ours the reverse | rewrite on the retail CThing / CTCInventoryBase headers (GetTC<CTCCarrying>) per the chunk 751 lever |
| 006061b0 | CTCInventoryMap::FrameUpdate | parked | 11 | DIFFER(322v322) dl=13: loop register swap: retail keeps the list node in edi and the card pointer (node+8) in esi, ours the reverse | the landed CountMoneyBags 0x006C04D0 list form is already used; try the retail CQuestManager header list type |
| 0063e590 | CWorldMap::GetNavigationLayerAt | parked | 10 | DIFFER(340v340) dl=4: one store scheduled early: retail stores the Bind2 P1 (&hasNavigation) after pushing &finder and loading World, ours straight after the lea | none found; P1 placement is independent of filter construction form |
| 00641ae0 | CTCLight::DrawFlare | parked | 11 | DIFFER(331v331) dl=23: scratch registers rotated in the flare block: retail halo graphic edx, thing eax, unique id ecx/eax; ours ecx, edx, eax/edx (same instructions and sizes) | try the retail CTCLight / CEnginePrimitiveFlareSprite headers (inline ctor from the PDB) for the rotation |
| 0065b142 | CTCVillage::EnableVillagerDefTypes | parked | 10 | DIFFER(386v386) dl=4: one scheduling difference: retail loads ParentThing (`mov eax,[ebp-4]; mov ecx,[eax+4]`) before `push 1`, ours after `push eax` | pure scheduling of the GetPosition this-load; a same-TU GetPosition body was tried by the earlier chunk |
| 0065be6c | CTCVillage::UpdateHornTasks | parked | 12 | DIFFER(336v336) dl=27: register swap: retail keeps this in ebx and the village def in edi, ours the reverse (an early-return form with the find_if result unnamed fixes the registers but grows to 344 bytes) | the early-return / unnamed find_if form (try e, 344 bytes) fixes the registers; shrink it back (the extra 8 bytes are the duplicated predicate dtor) |
| 006d4e60 | CTCFollowed::IsCreatureValidFollower | parked | 10 | DIFFER(363v363) dl=10: register choice: the component-key address temporary is eax/ecx/edx at the three lookups in retail (ours ecx/eax/ecx) and RenownLevel is pushed through eax (ours edx) | retail CThing GetTC<T> / PeekTC<T> header (owner-decided __forceinline) instead of the local GetInterface |
| 0071ad20 | CTCEmoteIcon::DrawEmoteTypeIcon | parked | 10 | DIFFER(365v365) dl=30: scratch registers rotated throughout (draw distance, camera vtable, sprite arguments); every instruction otherwise equal | retail CTCEmoteIcon / CEnginePrimitiveSprite headers; the rotation is whole-function |
| 0073acc0 | CTCCreatureModeManager::AddMode | parked | 10 | DIFFER(331v331) dl=2: the early `return false` sets al (`xor al,al`) before the register pops in retail, after `pop ebp` in ours | none found (the join block scheduling); try the retail CCountedPointer header dtor |
| 00744fb0 | CTCPerceiveThing::UpdateHearing | parked | 10 | DIFFER(355v355) dl=32: frame layout: retail passes the empty comparator at the Bind2 filter's own address (frame 24h), ours gives it its own slot (frame 28h), shifting the key and radius slots | the comparator address sharing needs the real CThingSearchTools::GetPBestLeafThing template signature (empty-class argument passing) |
| 007471f0 | CTCStealth::DrawPreparePrimitivesForRendering | parked | 11 | DIFFER(308v308) dl=21: frame packing: ours homes the ToWideString argument temporary in the dead viewport slot (frame 40h), retail gives it its own word (frame 44h) and schedules the position copy accordingly | retail CEnginePrimitiveText / CWideString headers for the temporary layout |
| 007534b0 | CTCLook::IsPosInViewRange | parked | 10 | DIFFER(384v384) dl=3: copy scheduling of the `from` position (three dword moves ordered differently) | retail C3DVector header ternary copy forms (copy ctor vs assignment) |
| 00760a70 | NShape::CSubdivisionTriangle::GetUpDirection | parked | 10 | DIFFER(332v332) dl=6: `pop esi` scheduled after the last faddp; retail pops it two x87 instructions earlier | none found (epilogue pop scheduling) |
| 007729a0 | CTCShop::CalcIsStallLike | parked | 10 | DIFFER(319v319) dl=8: loop register swap in the 0x21 list walk: retail node in edi and &node->item in esi, ours the reverse | retail CThing list / CIntelligentPointer headers (same residue family as 006061b0) |
| 00773950 | CTCShop::IsOpeningHours | parked | 11 | DIFFER(324v324) dl=3: scheduling: retail forms the villager lookup key address in ecx before `lea esi,[eax+44h]`, ours forms the map pointer first and takes eax for the key | retail CThing HasTC/GetTC header; a same-TU LowerBound body (try k) does not move the key lea |
| 00783a30 | CTCBoastingCrowdControl::GetCrowd | parked | 10 | DIFFER(393v393) dl=10: register choice in the insert tail: retail loads the hero UID into eax/esi and pushes the insert arguments via ecx/edx, ours ecx/edx then eax/ecx | retail std::map / pair header model for the insert argument |
| 007bc8fc | NThingShot::CTCShotDefinitionTearInGround::InitMovement | parked | 10 | DIFFER(308v308) dl=2: scheduling: the AddTC call loads ParentThing before the lea of the name temporary in ours, after it in retail | none found (implicit/named/getter/default-argument forms all keep the order) |
| 0085c7d0 | NCameraModes::CMoveOnCylinder::OnInit | parked | 10 | DIFFER(332v332) dl=8: three spots: the facing-vector virtual call fetches the vptr into edx (retail eax), one cross-product multiply loads Z.x before X.z (VC canonicalises the operands), one `mov edx,[esp+0Ch]` scheduled a step later | the vptr register gap of the 0x0053D000 family; no known lever |
| 00860ad0 | CVillageTaskPatrol::IsCreatureMoreAppropriateForTask | parked | 10 | DIFFER(302v302) dl=14: copy scheduling: retail loads all three dwords of the first two position copies before storing, ours interleaves | struct-copy forms interleave too; try the retail CThing::GetPosition by-value / PeekPos header accessor |
| 0086b9d0 | CRandomAppearanceMorph::GetRandomSkeletalMorph | parked | 10 | DIFFER(389v389) dl=63: register allocation: retail keeps this in ebp and the morph map in ebx (spilling `end`), ours this in ebx and the map in ebp | same-TU CVectorMap UpperBound / LowerBound bodies (try f, 387v389, dl=40) let VC keep the seed in ebp as retail does (LTCG knew the bounds write no memory) but reorder the two pure calls; combine with the template CVectorMap model (real mangled names) and an explicit upper-first count |
| 008bd3f0 | CCreatureAction_TeleportOut::FinishAction | parked | 10 | DIFFER(312v312) dl=17: register swap: retail keeps this in edi and the target in ebx, ours the reverse | retail CCreatureAction_TeleportOut header (landed TeleportOut::Initialise model) for the member types |
| 0090c900 | CThingFilter_IsValidGossipee::operator() | parked | 10 | DIFFER(345v345) dl=39: callee-saved pair swapped: the gossipee and the map/home-building temporaries take esi/edi the other way round | retail CThing GetTC header in place of the local LowerBound model |
| 00911a90 | CAIStateGroup_WanderAroundInitialPos::Update | RELOCATION_MATCH | 17 | RELOCATION_MATCH: creature lookups + wander query in one inner scope (found bool) (shared engine/C3DVector.h; vland RELOCATION_MATCH PASS) | - |
| 00956e70 | CAIStateGroup_BriarRose::Update | parked | 10 | DIFFER(363v363) dl=28: register allocation: retail keeps the creature def in edi and shrink-wraps ebx for the shield/creature block, ours pushes ebx at entry | retail CAIStateGroupBase header with the PDB AIAction_PerformActionAndWait(const CCreatureActionBase&, bool) (overlay drafted and withdrawn, see below) |
| 00961640 | CAIStateGroup_BanditKingThrowWeapons::Update | parked | 10 | DIFFER(323v323) dl=13: scratch register in the rotated loop test: retail reloads PBrain into ecx (`mov ecx,[esi+4]`), ours into eax | as 00956e70: the retail CAIStateGroupBase / CAIBrain headers for GetCreature |
| 00986ed0 | CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby | parked | 10 | DIFFER(353v353) dl=7: x87 operand order of three of the six cross-product multiplies (VC7.1 canonicalises commutative fmul operands) | VC7.1 commutative fmul canonicalisation; no source lever known |
| 009e5c90 | CSymbolMap::LoadFromFile | parked | 10 | DIFFER(373v373) dl=1: SIB operand order of the terminator store: retail `mov byte ptr [eax+esi],0`, ours `[esi+eax]` | none found (SIB base/index choice) |
| 00a1abf0 | CNavigatorAStar::IsNewDestinationGoingToCauseRestart | RELOCATION_MATCH | 10 | RELOCATION_MATCH: reach radius as a named local, then radius += reach; radius *= radius (shared engine/C3DVector.h + C2DVector.h; vland RELOCATION_MATCH PASS) | - |
| 00a22e10 | CRightHandedSet::MakePerpendicular | parked | 11 | DIFFER(358v358) dl=4: scheduling only: retail puts the second `mov esi,17Ch` between `fld st(3)` and `fmul st(4)`, ours after the fmul (the table operand line is a masking artefact of the same relocation) | shared C3DVector::Normalise loads Z first (header body is for 0x00B0CAD0); x87 scheduling only |
| 00a26ad0 | C3DAnimationInfo::BuildAnimationInstance | parked | 10 | DIFFER(383v383) dl=21: register allocation: `sete cl` vs al for the compressed flag, the bank pointer in edx vs ecx for the MemoryPool loads, and the first InitialiseAnimationInstance argument order | none found; same-TU GetEntryDataType body (LTCG view) untried |
| 00ab5030 | CMouseDX::ResetNonExclusiveMouse | parked | 10 | DIFFER(342v342) dl=36: frame slot order: retail puts the C2DCoordI below the origin POINT (pos, origin, cursor, extents, client), ours above the cursor | frame slot order of nested-scope locals; none found |
| 00acd960 | QuadricOptimiserInternals::COptimisedPrimitive::SimplifyByVertexPairs | parked | 10 | DIFFER(383v383) dl=39: register naming: retail keeps the copied contraction's Data in edi and Info in esi, ours the reverse | retail CCountedPointer header copy ctor |
| 00b92300 | CEngineSceneGridCell::CreateOcclusionGroup | parked | 11 | DIFFER(335v335) dl=4: scheduling: `mov ebp,ecx` and the this spill land in other slots of the prologue | prologue scheduling; none found |
| 00ba0fc0 | CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh | parked | 10 | DIFFER(333v333) dl=10: the flags load/store scheduling around the mesh stores, and the local handle at frame +8 (retail +4, this spill at +0) | the inline handle ctor of the retail CEnginePrimitiveHandle header |
| 00c1d1c0 | InvertMatrix3x4 | parked | 10 | DIFFER(347v347) dl=18: x87 operand order of seven commutative fmul pairs (VC7.1 canonicalises them; source order has no effect) | VC7.1 commutative fmul canonicalisation; no source lever known |
| 00c71f80 | CAudioSource::ComputePitchScale | parked | 12 | DIFFER(311v311) dl=4: scheduling after the pow call: retail reloads the flags and state before `fstp [esi+3Ch]`, ours stores the pitch scale first | none found (pow result store scheduling) |
| 00f36a30 | CHeteroTerrainFractal::GetFractalHeight | parked | 10 | DIFFER(361v361) dl=4: loop-counter scheduling: retail reloads the counter (`mov ecx,[esp+0Ch]`) before the fmul, ours after | none found (loop-counter reload scheduling) |

## What fixed the matches (GOTCHAS one-liners)
- 0x00911A90 CAIStateGroup_WanderAroundInitialPos::Update: put the creature's two GetInterface lookups and the GetNearbyWanderPosition
  query (bool result) in one inner `{}` scope; the scope ends the creature/navigation/scratchpad lifetimes before the MoveToPos call
  and VC7.1 then gives retail's esi/edi assignment. Plus PDB spellings: GetNearbyWanderPosition(const C3DVector&, float, C3DVector&,
  bool) const, protected AIAction_MoveToPos(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*, bool, float),
  the CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..>>::LowerBound template model (as landed RemoveMode 0x0073ABA0).
- 0x00A1ABF0 CNavigatorAStar::IsNewDestinationGoingToCauseRestart: bind the virtual GetReachRadius(false) result to a named float,
  then `radius += reach; radius *= radius;` (the inline `radius + GetReachRadius()` squared schedules the fld differently). PDB
  spellings: GetNodeAt(const C2DVector&, unsigned char) const returning const node, GetLayerAt returns unsigned char,
  FindNearestNavigableNode protected const with const pointers, IsEqual private const, C3DVector / C2DVector are `class`.
- Both now use the shared engine/C3DVector.h (and C2DVector.h): same bytes; the C2DVector argument is written `C2DVector(v.X, v.Y)`
  because the shared C2DVector has no C3DVector conversion. vland still warns SHARED_HEADER_AVAILABLE for the local CThing /
  CTCCreatureNavigation / CTCTargetingBase / CTCPerceiveThing / CAIBrain (00911a90) and CANavQuadTreeNode / CNavigatorManager /
  CNavigatorBase (00a1abf0) models (PASS, not strict).

## Observations for GOTCHAS
- Same-TU noinline bodies of out-of-line callees model what retail's LTCG knew: with CVectorMap UpperBound/LowerBound bodies in the
  TU VC7.1 stops reloading a by-reference seed across the calls (0x0086B9D0 `imul ebp,ebp,24A1h` straight after the bounds), but it
  may then reorder the now-pure calls (UpperBound/LowerBound order flipped).
- The 128-entry fast inverse-sqrt table 0x013CE558 is ?GFastOneOverSqrtTable@@3PAKA (draft 0x00A22E10 used g_InvSqrtMantissaTable);
  its `[eax*4+13CE558h]` line stays in the masked diff (q.py masking artefact, same relocation).
- The shared C3DVector::Normalise (GFFastOneOverSqrt of the one-line GetSquaredMagnitude) loads Z first; 0x00A22E10's two expanded
  normalises load X, Y, Z (local x/y/z copies), so that row keeps its local model.
- Commutative fmul operand order (0x00C1D1C0, 0x00986ED0, 0x0085C7D0): float locals copied from the matrix get register-allocated
  (471 bytes), an indexed `const float*` view, pointer parameters and /Op /O1 /Oy- all fail to move it.

## Helper / asm tables
None. No row uses __asm, naked, _emit, volatile, a helper or a pragma for a residue (the frame-pointer TU pragmas
`optimize("y", off)` / `optimize("s", on)` are the TU settings of the landed siblings).

## Header overlays
- Drafted `FABLE_RETAIL_CAISTATEGROUPBASE_ACTIONWAIT` (nested under FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS) in engine/CAIStateGroupBase.h
  declaring the Ego_r.pdb protected AIAction_PerformActionAndWait(const CCreatureActionBase&, bool) 0x0097DB70 and
  AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase&, CIntelligentPointer<const CThing>, bool) 0x0097DCB0
  (hdrcheck2 default=same for every macro). No graded row used it (00956e70 / 00961640 still sit on the `_pad_0x04` Methods
  proxies), so it was withdrawn: hdr/ is empty and the mirror equals base_include. No SIBLINGS.tsv (no header change).

## Name fixes
NAME_FIXES.tsv: 0x00A1A170 CNavigatorAStar::IsEqual (private const), 0x00A475C0 CNavigatorBase::FindNearestNavigableNode
(protected const, const pointers), 0x006CB6E0 CTCCreatureNavigation::GetNearbyWanderPosition (const, C3DVector& result; the landed
0x006CB6E0 / 0x006CB620 / 0x006CB840 rows spell it non-const with a C3DVector*).

