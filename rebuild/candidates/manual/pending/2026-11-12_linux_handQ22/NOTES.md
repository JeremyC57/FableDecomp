# Q22 notes (2026-11-12)

Matched 3 of 6 (all RELOCATION_MATCH, no flags); tests pass under VC7.1 (behaviour_of) and g++ for all three.

## Matched
- 0x0096CD70 CAIStateGroup_EatAtTable::Update -- ported (overlay: CAIStateGroupBase PERFORMACTIONONCE, CTCTavernTable SEATS,
  NCreatureMode_CModeSitFloor CMODESITTAVERNSTOOL). CAIStateGroup_EatAtTable itself has no shared header (local derived class on
  the shared CAIStateGroupBase, Ego_r members). Needs the repo's FABLE_RETAIL_CTHING_GETTC_EXPANDED + FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
  (the `const bool&` HasTC that stores the presence bit into a stack byte) and the visible called CCountedPointer ctor/dtor.
- 0x006C1250 CTCBuyableHouse::FrameUpdate -- the real name (CTCBase slot 9 = FrameUpdate; Ego_r/FableWin
  ?FrameUpdate@CTCBuyableHouse@@UAEXXZ); the batch said `Update`, the catalog `vfunc_9`. STILL A LOCAL MODEL: it would raise 8
  SHARED_HEADER_AVAILABLE warnings (rejected under LANDVERIFY_STRICT_HEADERS=1). Correct identities for the port: world +0x38 is
  CWorld::GetGameScriptInterface() (FableWin inline, CWorld.h) and the +0x628 virtual is slot 394
  CGameScriptInterface::IsHeroControlledByPlayer() const 0x0088E980 (engine/CGameScriptInterface.h); the font is
  CThing::PeekComponent() 0x004C7A70 -> CMainGameComponent::PeekCutSceneMainFont() (+0x1614C). The same-TU helpers must be real
  out-of-line definitions in the TU (IsHouseBeingUsed(bool) const with the CountIfWork 0x006C1C70 / CountIfHomeOrWork 0x006600EC
  branches, GetRentsDue() const, GetPriceForNextDressLevel() const returning -1, NeedsDressingSignPost()).
- 0x00A78B20 CNavQuadTree::SaveToFile -- ported (overlay: CNavQuadTree SAVETOFILE, CANavQuadTreeNode SAVETOFILE slots 16..22,
  CAFile WRITEVAR). Statement order from FableWin 0x03289B50 (decomp_navmesh.c): WriteVar<long/float/unsigned int>, Regions.size(),
  ActionPoints.size(), for_each with CSaveNavigationPositionToFile(file, MapPos), NavigationLayers.size(), map insert
  make_pair(&BlockedNode, 0), two layer/node loops (AddToIndexTable, SaveToFile(file, table, true, MapPos)), end marker, patch size.
  The draft's `AddToIndexMap` is Ego_r/FableWin `AddToIndexTable`. Descriptive (no PDB/strings symbol found): the TU-static consts
  NAV_QUAD_TREE_FILE_VERSION (8, 0x0129DCC8) and NAV_QUAD_TREE_FILE_END_MARKER (0xACBDEF12, 0x0129DCCC). CSaveNavigationPositionToFile
  is a TU-local class (FableWin ctor ??0CSaveNavigationPositionToFile@@QAE@AAVCAFile@@ABVC2DVector@@@Z, operator() QBE); no shared header.

## DIFFER
- 0x00649E8B NPlayerGui::CDrawFishingGame::Draw -- ported (4 grades). Residue is frame packing only: retail gives the two implicit
  CCharString temporaries (the GetTextBySymbol symbol names) separate slots (-0xC, -0x10), VC7.1 here shares one, so the frame is 0x14
  not 0x18; plus one `mov ecx,[eax+1C8h]` scheduled before the colour byte stores. 96 toy variants never split the temps.
  CTCFishing / CFishingRodDef have no shared header (local, Ego_r members, FableWin inline getters).
- 0x0063F7C0 ForEachInRoughArea<CThingPhysical, CFilter_Bind3<IsNot, HasNavigationInformation, OverlappingBoundingBox>,
  CThingFindNavigationLayerAt> -- not ported. Findings for the port: the PDB signature takes `const Proc&` (ABV) and is const; the
  shared FOREACHINROUGHAREA declaration says `Proc&` (the landed caller 0x0063E590 relocates to it, masked). The draft header comment
  describes the Bind2 twin 0x0063F4B0; this row is the Bind3 instance. The inline OverlappingBoundingBox test passes &F3->Pos to
  CPhysicsMeshObject::IsPointContainedInBoundingBox 0x00A12890 (the catalog's `CMainGameComponent::BeginInputLoading` there is a
  propagated label); the draft's CAABox member is wrong (Ego_r: Pos +0, Radius +0xC).
- 0x00489D40 CPlayer::CreateCharacter -- not ported (no grades); needs the CPlayer retail opt-ins listed in its draft.
