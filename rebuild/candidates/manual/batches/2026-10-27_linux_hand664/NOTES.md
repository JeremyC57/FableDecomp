# Hand chunk 664 (CTCPhysicsRigidBody .. CGPUScratchBufferTextureManager): 10 of 10 byte-exact

All ten rows RELOCATION_MATCH (q.py and the read-only vland run: 10 WINS, behaviour PASS under g++ and VC7.1, no
RELOC_IDENTITY). Every catalog label was a propagated guess; each row's identity was settled from its vtable store / sole
caller (the slot-0 ??_G), RTTI, the TU neighbours and the Ego_r / FableWin PDB name sets.

## What matched (10)
- **CTCPhysicsRigidBody::~CTCPhysicsRigidBody** 0x0075A480 (2 probes): user virtual dtor `if (PPhysicsMeshInfo) delete
  PPhysicsMeshInfo;` (a bare `delete` of the trivially destructible info drops retail's null test, 158v162); STLport vector
  members; CTCPhysicsBase's dtor is implicit (no vtable store), so its CDefPointer<const CPhysicsDef> release is expanded
  before the tail jump to ~CTCBase.
- **CNavigatorAStar::~CNavigatorAStar** 0x00A1CB40 (3 probes): body = inline CNavigatorQueueManager::RemoveNavigatorFromQueue
  through GetNavigatorQueueManager(); real STLport _Rb_tree / _List_base / _Vector_base text; CNavBlockedQuadTreeNode has no
  dtor (only the CANavQuadTreeNode store); list::remove_if stays declared-only (the in-class `_S_remove_if` body is inlined
  by VC7.1: 168v164).
- **NScript::CQ_AwakeningTheOracleScript::~CQ_AwakeningTheOracleScript** 0x00D005E0 (3 probes): compiler-generated dtor
  emitted out of line by the class's retail ctor in the TU; CLevelScriptBase<CQ_SunnyvaleMasterData> must have a
  compiler-generated dtor too (header debt below), so the tail jump binds ~CScriptBase.
- **CThingManager::~CThingManager** 0x00523440 (3 probes): `DeleteAllThings();` then STLport members; the
  CEntityRuntimePersistence model needs its typed members (a byte array put it at +0x9D).
- **CResourceBank::~CResourceBank** 0x009FC690 (1 probe): user `{}` after the same-file ~CResource / ~CResourceList bodies
  (retail TU order); VC7.1 expands both.
- **CEnginePrimitiveRenderer::AddPrimitiveManager** 0x00B33AE0 (1 probe): STLport push_back (placement new + CCountedPointer
  copy ctor; __false_type _M_insert_overflow declared) then g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(manager.Get()).
- **NUISystem::CClickable::~CClickable** 0x0055B760 (2 probes): four `delete PActionsOn*` in retail order (LeftClicked,
  RightClicked, LeftHeld, LeftUnclicked); the _list.c clear() body is kept out of line by VC7.1 as retail.
- **CBaseVideoRenderer::OnDirectRender** 0x00CA5570 (1 probe): DirectShow strmbase renbase.cpp verbatim on the landed
  OnRenderEnd model plus the four CBaseVideoRenderer virtuals (slots 45..48).
- **CMiniMapDisplay::~CMiniMapDisplay** 0x00442610 (1 probe): body `RegionMinimapTexture = 0; PRegion = 0;`, STLport members,
  out-of-line ~CRegionMinimap / ~CMinimapData.
- **CGPUScratchBufferTextureManager::~CGPUScratchBufferTextureManager** 0x00F37570 (2 probes): `for (long i = 0; i <
  MemoryPages.size(); ++i) if (MemoryPages[i].ScratchPool > -1) FreeMemoryPage(i);` (`!= -1` gives je for jle).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
| address | catalog label | retail identity |
|---|---|---|
| 0x0075A480 | CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip | CTCPhysicsRigidBody::~CTCPhysicsRigidBody |
| 0x00A1CB40 | CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip | CNavigatorAStar::~CNavigatorAStar |
| 0x00D005E0 | CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip | NScript::CQ_AwakeningTheOracleScript::~CQ_AwakeningTheOracleScript |
| 0x00523440 | CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip | CThingManager::~CThingManager |
| 0x009FC690 | CDynamicList::~CDynamicList | CResourceBank::~CResourceBank |
| 0x00B33AE0 | _global::CEnginePrimitiveTypeInfo_Manager_List_Insert | CEnginePrimitiveRenderer::AddPrimitiveManager |
| 0x0055B760 | _global::CHoverable_destructor | NUISystem::CClickable::~CClickable |
| 0x00CA5570 | _global::Audio_SleepUntilFrameTime | CBaseVideoRenderer::OnDirectRender (strmbase, not libpk audio) |
| 0x00442610 | _global::RenderState_Destructor | CMiniMapDisplay::~CMiniMapDisplay |
| 0x00F37570 | _global::CGPUScratchBufferTextureManager_Destructor | CGPUScratchBufferTextureManager::~CGPUScratchBufferTextureManager |
| 0x00D005C0, 0x009FC750, 0x00A224D0, 0x0075C020, 0x0052C610, 0x0055B850, 0x004426E0, 0x00F388D0 | CLandscapeBackgroundPatch `vector_deleting_destructor' | the ??_G of each class above (NAME_FIXES.tsv) |
| 0x00A214C0 | CGuiControlTreePane::SortTreeRecursively | std::_List_base<CNavWayPoint>::~_List_base |
| 0x00440730 | CGuiControlTreePane::SortTreeRecursively | std::_List_base<CMiniMapDisplay::CMarkerInfo>::clear |
| 0x005356A0 | remove_if<NEntityEvents::CConvFilterToCP<..>> | std::_List_base<CCountedPointer<NUISystem::CAction> >::clear |
| 0x00A16EB0 | (target of 0x00A155C0's jmp) | STLport std::_S_remove_if<pair<long,const CNavigatorBase*>, .., CIsNavigator> (descriptive: STLport source name, no PDB) |
| 0x00CBD510 | CParticleEmitter::DeleteAllParticles | CScriptBase::~CScriptBase |
| 0x0052C4B0 | CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures | NThingManager::CEntityRuntimePersistence::~CEntityRuntimePersistence |
| 0x00642A60 | CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures | CMinimapData::~CMinimapData |
| 0x0066CCF0 | CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip | CRegionMinimap::~CRegionMinimap |
| 0x00A47180 | CNavigatorBase::~CNavigatorBase | (correct) |
| 0x00686830 | (stand-ins) | CTCBase::~CTCBase |

## Proven
- 0x0075A480 ??1CTCPhysicsRigidBody@@UAE@XZ -- first store vtable 0x0126996C (RTTI .?AVCTCPhysicsRigidBody@@), sole caller its slot-0 ??_G 0x0075C020; Ego_r.pdb ??1CTCPhysicsRigidBody@@UAE@XZ
- 0x00A1CB40 ??1CNavigatorAStar@@UAE@XZ -- vtable 0x0129CCCC (RTTI .?AVCNavigatorAStar@@), sole caller ??_G 0x00A224D0; Ego_r ??1CNavigatorAStar@@UAE@XZ (0x00B88B80)
- 0x00D005E0 ??1CQ_AwakeningTheOracleScript@NScript@@UAE@XZ -- sole caller ??_G 0x00D005C0 = slot 0 of vtable 0x012C78B0 stored by ??0CQ_AwakeningTheOracleScript 0x00CFAD30 (slots 1..4 = the class's RegisterMain/Main/Init/OnPersist); TU end before CQ_BanditCampScript; Ego_r 0x00D02380
- 0x00523440 ??1CThingManager@@UAE@XZ -- vtable 0x01245C44 (RTTI .?AVCThingManager@@), sole caller ??_G 0x0052C610; Ego_r 0x0052D7D0; body calls DeleteAllThings
- 0x009FC690 ??1CResourceBank@@UAE@XZ -- vtable 0x0129C814 (RTTI CResourceBank), slot 0 ??_G 0x009FC750 calls it; Ego_r ??1CResourceBank@@UAE@XZ; TU neighbours ??0CResourceBank 0x009FC5F0 / ~CResourceList 0x009FC5C0
- 0x00B33AE0 ?AddPrimitiveManager@CEnginePrimitiveRenderer@@AAEXABV?$CCountedPointer@VCEnginePrimitiveManager@@@@@Z -- Ego_r name directly before ??0CEnginePrimitiveRenderer (same pair in retail: all 21 callers inside the ctor 0x00B33B50, each passing a CCountedPointer manager member copy); PrimitiveManagers at Ego_r +0x274
- 0x0055B760 ??1CClickable@NUISystem@@UAE@XZ -- stores the three CClickable vtables 0x0124BD2C/+4 0x0124BD08/+0x18 0x0124BD00 (RTTI .?AVCClickable@NUISystem@@); ??_G 0x0055B850 calls it; Ego_r ??1CClickable@NUISystem@@UAE@XZ; tail jump = landed ??1CHoverable 0x0055BC30
- 0x00CA5570 ?OnDirectRender@CBaseVideoRenderer@@UAEXPAUIMediaSample@@@Z -- slot 46 of CTextureRenderer 0x0129D08C and of CBaseVideoRenderer's own vtable 0x012BF5D8; renbase.h order RecordFrameLateness (45, landed 0x00CA5400) / OnDirectRender (46) / ResetStreamingTimes (47, landed 0x00CA52A0) / SendQuality (48, landed 0x00CA56E0); body = renbase.cpp OnDirectRender
- 0x00442610 ??1CMiniMapDisplay@@UAE@XZ -- vtable 0x01231768 (RTTI .?AVCMiniMapDisplay@@), ??_G 0x004426E0 (Ego_r ??_GCMiniMapDisplay@@UAEPAXI@Z) calls it; member set = Ego_r CMiniMapDisplay
- 0x00F37570 ??1CGPUScratchBufferTextureManager@@UAE@XZ -- vtable 0x01301CE4 (RTTI), ??_G 0x00F388D0; Ego_r ??1CGPUScratchBufferTextureManager@@UAE@XZ; callee ?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z
- 0x00CBD510 ??1CScriptBase@@UAE@XZ -- engine/CScriptBase.h + tail target of 0x00D005E0 and of ~CLevelScriptBase<T> copies 0x00CDD100..0x00CDD150
- 0x00686830 ??1CTCBase@@UAE@XZ -- RTTI chain of CTCPhysicsRigidBody; tail target of 0x0075A480
- 0x00A47180 ??1CNavigatorBase@@UAE@XZ -- RTTI chain of CNavigatorAStar (Ego_r 0x00B9E620); tail target of 0x00A1CB40
- 0x00A37360 ??1CClassFactory@@MAE@XZ -- RTTI chain of CThingManager; tail target of 0x00523440 (bound)
- 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ -- engine/CBaseClassNonCopyable.h; RTTI chains of four rows
- 0x009FC020 ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z -- Ego_r spelling (reference parameter); called by the inline ~CResource
- 0x00A155C0 ??$remove_if@VCIsNavigator@@@?$list@U?$pair@JPBVCNavigatorBase@@@std@@V?$allocator@U?$pair@JPBVCNavigatorBase@@@std@@@2@@std@@QAEXVCIsNavigator@@@Z -- Ego_r + FableWin spelling; the dtor's queue removal
- 0x00A21640, 0x005278A0, 0x005286D0, 0x00CD344A, 0x009AA830 STLport _Rb_tree<..>::_M_erase (private, five-parameter) -- the map member types from Ego_r (States, UIDToThingMap, NPCNameMap, cs_chars, cs_args); full spellings in NAME_FIXES.tsv
- 0x00A214C0 ??1?$_List_base@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@std@@@std@@QAE@XZ -- Route (Ego_r list<CNavWayPoint>); 0x00A21A00 `jmp 0x00A214C0` is the implicit ~list COMDAT
- 0x00440730 ?clear@?$_List_base@VCMarkerInfo@CMiniMapDisplay@@V?$allocator@VCMarkerInfo@CMiniMapDisplay@@@std@@@std@@QAEXXZ -- Markers (Ego_r list<CMiniMapDisplay::CMarkerInfo>); REVIEW the 0x0082A3xx callers
- 0x005356A0 ?clear@?$_List_base@V?$CCountedPointer@VCAction@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCAction@NUISystem@@@@@std@@@std@@QAEXXZ -- PActionsOn* (Ego_r list<CCountedPointer<NUISystem::CAction> >*)
- 0x0052C4B0 ??1CEntityRuntimePersistence@NThingManager@@QAE@XZ -- Ego_r name; EntityRuntimePersistence member of CThingManager (+0xA0)
- 0x0066CCF0 ??1CRegionMinimap@@QAE@XZ -- Ego_r name; RegionMinimap member of CMiniMapDisplay (+0x54)
- 0x00642A60 ??1CMinimapData@@QAE@XZ -- bound; MinimapList element dtor (Ego_r vector<CMinimapData>)
- 0x0051FBA0 ?DeleteAllThings@CThingManager@@QAEXXZ -- Ego_r name, bound
- 0x00B8FAD0 ?RegisterPrimitiveManager@CEnginePrimitiveTypeInfo@@QAEXPAVCEnginePrimitiveManager@@@Z -- FableWin.pdb name
- 0x00B38BB0 ?_M_insert_overflow@?$vector@V?$CCountedPointer@VCEnginePrimitiveManager@@@@..ABU__false_type@2@I_N@Z -- STLport overload for a non-POD element; the landed row has the same leaf

## Descriptive (no PDB name)
- 0x01436E84 `g_pEnginePrimitiveTypeInfo` -- established-by-count landed spelling (19 rows; competing g_pPrimitiveManagers,
  g_pPrimitiveTypeInfo, g_pEngine_01436e84); no PDB data symbol on disk (owner rule: keep established spellings).
- Member names of local models are the Ego_r.pdb names; opaque members (`unsigned char X[N]`) carry the real type in a comment.

## Header debt
- engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` -- retail's is compiler-generated (0x00CDD100..0x00CDD150
  are bare `jmp ~CScriptBase`); with the declaration an implicit derived dtor tail-jumps to ??1CLevelScriptBase<T> (masked
  RELOCATION_MATCH, wrong relocation identity), a `{}` body adds a vtable store. 0x00D005E0 carries a local copy without it.
  Needs the owner's decision before the header changes (1,000+ includers; only TUs that emit a derived dtor are affected).
- Flattened headers modelled locally: engine/CTCBase.h, CTCPhysicsBase.h, CTCPhysicsRigidBody.h (0x0075A480);
  CNavigatorBase.h, CClass.h, CNavBlockedQuadTreeNode.h, CANavQuadTreeNode.h, CNavigatorManager.h (0x00A1CB40);
  CHoverable.h, CObserver.h (0x0055B760); CRegionMinimap.h (no dtor), CMarkerInfo.h (global struct, retail is the nested
  class CMiniMapDisplay::CMarkerInfo) (0x00442610); CClassFactory.h (0x00523440); CResourceList.h (0x009FC690);
  CEnginePrimitiveRenderer.h, CEnginePrimitiveTypeInfo.h (no member functions) (0x00B33AE0).
- No headers: CCharString, CWideString, C3DVector, C2DVector, CMatrix3x4, CRightHandedSet, CRigidBody(Config),
  CEnginePrimitiveHandle, CMinimapData, CBaseIntelligentPointer (local models, Ego_r layouts / PDB class keys).
