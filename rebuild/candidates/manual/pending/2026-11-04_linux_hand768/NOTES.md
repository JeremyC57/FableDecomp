# Chunk 768 -- 14 under-budget parked rows (273-336 bytes)

## Progress log (incremental)
- 005ebfa0 CTCTavernGame::CameraUseCameraPoint: RELOCATION_MATCH (grade 2 of this chunk). Replaced the fabricated
  FindThingsWithScriptName stand-in with the real callee chain on the retail headers: `PeekWorld().PeekThingSearchTools()
  .GetAllThings<CThing>(CThingFilter_HasScriptName(name), things)` (0x00686A80 / [world+0x20] / 0x005ED860), `thing->GetTC(&point)`,
  `point->GetPointType(&scripted)` (result unused, as the landed 0x0089E190), `GetWorld().GetGameCamera()->GetCameraManager(&manager)`,
  `new CScriptModuleTavernGame(scripted->GetStartPos(0, 0), scripted->GetLookDirection(0))` in the called CCountedPointer(T*) ctor.
  The block-layout residue (null test on the success path's free) was escape analysis: a same-TU noinline GetAllThings body
  (new nested header opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY, the landed 0x005ED860 shape over PeekClassList 0x0049C770)
  tells VC7.1 the vector is not captured.
- 0096dbc0 CCreatureAction_ScreamerIntoAggressive::Initialise: RELOCATION_MATCH (grade 3). The two mode constructions written as two
  full `modeManager->AddMode(CCountedPointer<CModeBase>(new ...))` calls in an if/else (VC7.1 cross-jumps the identical tail). The
  ternary (chunk 764) and a `mode` local both keep the esi/ebp/ebx rotation and the swapped frame words.
- 00969e70 CAIStateGroup_SleepOnFloor::IsRunnable: RELOCATION_MATCH (7 grades). Ported to retail headers (CAIStateGroupBase PEEK/GETCREATURE,
  GFRandom inline, PDB filter classes, local STLport unary_negate/not1 as landed 0x00958770). Two fixes: (1) the 2D centre is a named
  `C2DVector centre(GetCreature().GetPos())` and the C2DBoxF temporary goes inside the call (GetPos first, filter after, box among the
  pushes); (2) the same-TU noinline GetAllThingsInRoughArea body (new nested overlay FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY +
  NThingMap_CThingMap.h + CTCMapwho _SEARCHID + CThing _STATTYPEFLAGS) keeps the vector start in ebx and zero in edi (dl 56 -> 0).
- 00753380 CTCLook::FrameUpdateHeadInterested: RELOCATION_MATCH (4 grades). Restructure: `if (target == 0 && !targeting->IsLookingAtCamera())
  { CentreHead(); return; }` then `if (IsLookingAtCamera()) camera else thing` (VC threads the second test; block order = retail).
  Then the retail-header port fixed the registers: CThing::GetTC, FableWin inline CTCTargetingBase::IsLookingAtCamera (new overlay
  FABLE_RETAIL_CTCTARGETINGBASE_LOOKATCAMERA, + GetPNearestInterestingThing const), CWorld::GetGameCamera()->PeekCurrentPos()
  (slot 11 returns const C3DVector), GetFocalPosIfLoaded(C3DVector&, float) const (overlay _CTCGRAPHICAPPEARANCE_FOCALPOS),
  GetPhysicsTC()->GetPosition(). FableWin says CentreHead / FrameUpdateHeadLookAtPos / GetHeadPos are private (AAE/ABE).
- 0070d780 CAnimateThing::AddCombinationAnim: RELOCATION_MATCH (5 grades). STLport std::vector / std::list members (no pragma, no hand
  vectors), the implicit CAnimSet copy constructor, a same-TU noinline body of the private CAnimSet::Init(CAnimateThing&, const
  CAnimInit&, float) (retail compiles 0x0070D1A0 just before; dl 69 -> 7: ebp keeps InternalAlphas' start), then `set.LastAlpha =
  init.LastAlpha;` before `set.OwnedByMainAnim = ownedByMainAnim;` (ecx/edx swap, dl 7 -> 0).
  Final source uses engine/CWorld.h (FABLE_RETAIL_CWORLD) for the static CWorld::GetFrame (vland SHARED_HEADER_AVAILABLE fix; still RELOCATION_MATCH).

## Results (5 matched / 9 parked)
| address | name | result | grades |
|---|---|---|---|
| 005ebfa0 | CTCTavernGame::CameraUseCameraPoint | RELOCATION_MATCH | 2 |
| 0096dbc0 | CCreatureAction_ScreamerIntoAggressive::Initialise | RELOCATION_MATCH | 5 |
| 00969e70 | CAIStateGroup_SleepOnFloor::IsRunnable | RELOCATION_MATCH | 12 |
| 00753380 | CTCLook::FrameUpdateHeadInterested | RELOCATION_MATCH | 5 |
| 0070d780 | CAnimateThing::AddCombinationAnim | RELOCATION_MATCH | 7 |
| 00c3a7d0 | CRampedParam::ApplyGainToFrame (PROPOSAL name) | DIFFER(272v273) dl 7 | 23 |
| 0083dc80 | CThingPhysicalSwitch::DrawPreparePrimitivesForRendering | DIFFER(304v309) dl 34 | 15 |
| 00837b30 | CThingTrackNode::DrawPreparePrimitivesForRendering | DIFFER(309v314) dl 34 | 10 |
| 0089da70 | CGameScriptInterface::EntitySetAlpha | DIFFER(314v314) dl 43 | 13 |
| 009d5bf0 | CBankFileAsync::HandleFailedAllocation | DIFFER(305v314) dl 47 | 11 |
| 00a76770 | CNavQuadTreeNode::GetLeafNodeAt | DIFFER(318v321) dl 58 | 14 |
| 00ed9bb0 | NScript::CV_TalentlessBardScript::ProcessSongsClothes | DIFFER(298v306) dl 1 | 10 |
| 00f35c60 | NFractal::CNoise::PerlinNoise | DIFFER(312v316) dl 13 | 10 |
| 00bcfb00 | CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer | DIFFER(329v336) dl 35 | 11 |

vland (read-only): 5/5 RELOCATION_MATCH + behaviour PASS. Warnings: 00969e70 RELOC_NAME x2 (0x00A55A30 / 0x00969FB0 labels are
wrong; NAME_FIXES rows), 00753380 RELOC_CONFLICT (CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound has per-TU copies 0x004365B0 /
0x0040F020; this TU's retail call is 0x004365B0).

## Parked: residue and next idea
- 00c3a7d0: retail `fst [gain]; fcomp 0` then reload for the > 1 test; VC7.1 emits `fcom; fst` and keeps st(0). Next: a volatile-free
  form that forces the store first (e.g. the gain as a member written then re-read) -- ~22 forms tried.
- 0083dc80 / 00837b30 (twins of 0x0083B6B0): retail adds OldPosition X, Y, Z with a store after each, VC7.1 adds Z, Y, X (the
  out-of-line GetPosition 0x0049D150 itself has Z, Y, X, so the inline context differs); early channel-desc zero stores; PhysicsTC
  CSE. Best: `C3DVector d = Position - OldPosition; d *= t; return d + OldPosition;`. Next: port to engine/CThingPhysical.h /
  CTCPhysicsBase.h (PDB OldPosition, virtual GetRHSet(float)) with a CEnginePrimitiveStaticMesh header.
- 0089da70: loop register swap (start eax / element ecx in retail). Next: same-TU body idea does not apply (GetAllCarriedThings is
  another TU); try iterator vs index loop forms on the retail header port.
- 009d5bf0: retail spills `this` into a frame word at entry and keeps count/data in ebp/esi. Next: std::set port with a same-TU
  insert_unique body (needs an upper_bound overlay in stlport/fable_map.h).
- 00a76770: the QuadTreeLevel byte read by GetSize() is CSEd into dl, so pos goes to esi and the child calls are not tail jumps.
  Retail-header port done (CANavQuadTreeNode friend overlay, CNavQuadTree::BlockedNode, C2DVector::Initialise). Next: find what
  breaks the CSE in retail (an aliasing store or a differently typed level read).
- 00ed9bb0: retail's entry `mov [esp+4],0` (conditional-temporary flag) has no source yet; the five push_back branches match.
- 00f35c60: b11 load scheduled late with `fmul [esi+eax*8+804h]` folded; ours hoists it.
- 00bcfb00: around the expanded CShaderRenderManager::DrawBuffersIndexed, retail keeps the manager in ebx / index buffer in ebp and
  homes the counts; ours the reverse. Next: port to engine/CShaderRenderManager.h with a DrawBuffersIndexed body overlay.

## Header overlays ($D/hdr, all default=same in hdrcheck2.txt)
- CThingSearchTools.h: FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY (noinline GetAllThings + PeekClassList 0x0049C770),
  FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (noinline GetAllThingsInRoughArea body + GetAllThingMapsInArea 0x0049C6F0; needs
  NThingMap_CThingMap.h, CTCMapwho _SEARCHID, CThing _STATTYPEFLAGS).
- NEW NThingMap_CThingMap.h (NThingMap::CThingMap 0x34 with vfptr, CMapwhoLevelInfo, CThingMapCell; landed global spellings).
- CThing.h _STATTYPEFLAGS (StatGetTypeFlags, 0); CTCMapwho.h _SEARCHID (Get/SetSearchID); C2DVector.h _FROM3D, _INITIALISE;
  C2DBoxF.h _CENTRE (ctor (centre, radius) + ToC2DBoxI 0x00A55A30); CTCTavernGame.h _CAMERAPOINT; CTCTargetingBase.h
  _LOOKATCAMERA; NGraphicAppearance_CTCGraphicAppearance.h _SETALPHA / _FOCALPOS; CTCCarrying.h _ALLCARRIED;
  CANavQuadTreeNode.h _FRIENDNODE.

## GOTCHAS one-liners
- A same-TU noinline body of the callee that fills a local container (GetAllThings / GetAllThingsInRoughArea / CAnimSet::Init) tells
  VC7.1 the local does not escape: start kept in a callee-saved register, no second null test (0x005EBFA0, 0x00969E70, 0x0070D780).
- `C2DVector centre(pos)` named, then the float box built inside the call, gives retail's GetPos-first / box-among-pushes order.
- An early `if (!x && !y) { Leave(); return; }` before `if (y) A else B` lets VC thread the repeated test into retail's layout (0x00753380).
- Two full arms beat a ternary for a three-register rotation (0x0096DBC0); member store order decides ecx/edx for a bitfield + float pair (0x0070D780).
