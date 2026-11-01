# Hand chunk 667 (CCreatureAction_BlockRespond .. CEngineInternalAttachedPrimitiveInfo): 8 of 10 byte-exact (+2 int3-after-noreturn)

All ten rows were settled from their own callers / vtable slots / RTTI before authoring. 8 rows RELOCATION_MATCH in src/ with
passing tests (g++ and VC7.1); the two CCachedFile throw stubs are exact retail bytes plus the 0xCC VC7.1 emits after the final
`call _CxxThrowException` (int3-after-noreturn, differ/ with `.test.cpp`, land against the N+1 span). Probe logs in probe/<addr>.log.
Call targets checked with `objdump -dr` of each object (every relocation names the retail callee's PDB spelling).

## Proven
- 0x008553C0 ?FrameUpdate@CCreatureAction_BlockRespond@@UAEXXZ -- only reference vtable 0x01276084 (RTTI CCreatureAction_BlockRespond) slot 56 = the family's FrameUpdate slot (CCombatActionBase::FrameUpdate 0x00859D80 is slot 56 of 0x01259BD4); Ego_r.pdb + FableWin.pdb; tail-calls the base FrameUpdate
- 0x008C3F20 ?FrameUpdate@CCreatureAction_WaspQueenDie@@UAEXXZ -- only reference vtable 0x01280D84 (RTTI CCreatureAction_WaspQueenDie) slot 56; Ego_r.pdb + FableWin.pdb; tail-calls CCombatActionBase::FrameUpdate
- 0x0091D8C0 ?OnActivate@CAIStateGroup_EnemyCreatureBase@@UAEXXZ -- slot 2 of the class's own vtable 0x0128B8BC (RTTI) and of 11 derived vtables that inherit it; CAIStateGroupBase's slot 2 is the empty 0x008FD26F; Ego_r.pdb + FableWin.pdb
- 0x00A5AFB0 ?Write@CCachedFile@@UAEXPBXJPAVCASuspendableProcess@@@Z -- vtable 0x012724D4 (RTTI CCachedFile) slot 4 = CMemoryFile's Write slot (0x009E95F0); Ego_r.pdb donor order Close/Write/SetPosition/MovePosition/SetLength/GetLength = retail 0x00A5AFA0/0x00A5AFB0/0x00A5B020/0x00A5B0A0/0x00A5B190/0x00A5B200
- 0x00A5B190 ?SetLength@CCachedFile@@UAEXJ@Z -- vtable 0x012724D4 slot 8 = CMemoryFile's SetLength slot (0x009E9730); same donor run
- 0x00A75B70 ?SetUpNeighbours@CNavNavigableLeafNode@@MAEXW4ENavQuadTreeNeighbours@@PAVCANavQuadTreeNode@@@Z -- vtable 0x0129DDA4 (RTTI CNavNavigableLeafNode) slot 17 and CNavSwitchableLeafNode 0x0129DE04 slot 17 (inherited); Ego_r.pdb + FableWin.pdb; calls slot 16 SetUpNeighboursWithLeaf (landed 0x00A75F10 family)
- 0x00B0A060 ??1CPSCSplineBase@NParticleEngine@@UAE@XZ -- called by ??_G 0x00B230C0 (slot 0 of 0x012A0B14, RTTI CPSCSplineBase) and ??_G 0x00B1B490 (CPSCSpline), tail-jumped to by ~CPSCSingleSprite 0x00B0A160; RTTI base arrays put CPSCSplineBase under both; members = Ego_r CPSCSplineBase; Ego_r.pdb
- 0x00A6B5B0 ??1CManagedTextureData@@UAE@XZ -- sole caller ??_G 0x00A6B590 = slot 0 of 0x0129DC24 (RTTI CManagedTextureData : CResource); members = Ego_r CManagedTextureData; Ego_r.pdb + FableWin.pdb
- 0x009AC7C0 ??1CRetailBank@NBankFileManager@@UAE@XZ -- sole caller ??_G 0x009AC7A0 = slot 0 of 0x0129B010 (RTTI CRetailBank : CBaseClassNonCopyable; the landed ctor 0x009AC4D0 stores it); members = Ego_r CRetailBank at STLport sizes; Ego_r.pdb + FableWin.pdb
- 0x00B3AB10 ??1CEngineInternalAttachedPrimitiveInfo@@UAE@XZ -- ??_G 0x00B3AB80 = slot 0 of 0x012A1AB8 (RTTI CEngineInternalAttachedPrimitiveInfo : CBaseClass) and CEnginePrimitiveAttachmentManager::SetAttachedPrimitive 0x00B3A180 (local destroyed at 0x00B3A2B7); Ego_r.pdb + FableWin.pdb
- Callees proven on the way (spellings used; see NAME_FIXES.tsv for the landed spellings they replace): 0x006925B0 ?GetTotalTimeWorldFrames@CCreatureActionBase@@QBEJXZ (Ego_r donor run), 0x009AA780 _M_erase of _Rb_tree<CCharString, pair<const CCharString, NBankFileManager::CContainedBank>, ...>, 0x008F7860 / 0x008F7950 NCombatActionHelpers, 0x006EDC50 RegisterCombatant(CThingCreatureBase&, CThingCreatureBase&, bool), 0x008FCFC0 CAIStateGroupBase::GetWorld, 0x00692910 PeekDefinitionManager, 0x00692C10 GetWorldMap (IAE), 0x004BFAC0 NGraphicAppearance::CTCGraphicAppearance::SetAlpha, 0x009FC550 ~CResource, 0x0099A300 ~CBaseClassNonCopyable, 0x0099A2E0 ~CBaseClass.
- Slot 20 (+0x50) of CAIStateGroup_EnemyCreatureBase = IsCombatStateGroup() const (pure, _purecall in 0x0128B8BC): every leaf's slot-20 stub returns true for the attack groups (HoverAttack 0x00900C4D, HoverAttackArea 0x00900D3A, HoverAttackUnlessTargetIsFleeing 0x00900C88, HoverKeepInArea 0x00900D03) and false for wander/whisper groups, the landed IsCombatStateGroup rows sit at exactly those addresses, and FableWin.pdb names IsCombatStateGroup for each of those leaves.

## What matched (8)
- CCreatureAction_BlockRespond::FrameUpdate (0x008553C0): first-frame dust effect; the FableWin-spelled NCombatActionHelpers fastcalls and the 0x0087CA30 shared model; creature in a named local (fetched before the target). First probe.
- CCreatureAction_WaspQueenDie::FrameUpdate (0x008C3F20): alpha fade. Lever: two named float locals (`startAlpha`, then `alpha = startAlpha - frames/total*51`) put the frames temporary / total / alpha in retail's frame words; one `alpha` with `-=` shuffles them. 13 probes.
- CAIStateGroup_EnemyCreatureBase::OnActivate (0x0091D8C0): IS_THING_ALIVE over the intelligent pointer, IsCombatStateGroup + RegisterCombatant, OnActivateLeaf tail. First probe (see Review: the dead-target path's discarded IsCombatStateGroup() call).
- CNavNavigableLeafNode::SetUpNeighbours (0x00A75B70): jump table without range check = `default: __assume(0)`; the `mov eax,K ; push eax` arms are one `opposite` local + one tail-duplicated call; cases written in arm order 3,2,1,0. Jump-table pairing verified by hand (side 0->1, 1->0, 2->3, 3->2).
- Destructors (all compiler-generated: no own vtable store; emitted out of line by defining the class's PDB/retail constructor in the TU, as chunk 664):
  ~CEngineInternalAttachedPrimitiveInfo (0x00B3AB10, CEnginePrimitiveHandle inline Clear + CIVCountedPointer release model of the landed 0x007CA8E0),
  ~CManagedTextureData (0x00A6B5B0, boost::scoped_ptr checked_delete of a COM interface = bare operator delete, engine/CCountedPointer.h),
  ~NBankFileManager::CRetailBank (0x009AC7C0, real STLport 4.6 map text from chunk 663, shared-zero clear),
  ~NParticleEngine::CPSCSplineBase (0x00B0A060, RTTI single chain CPSCBase : CParticleUpdateParamCallback<unsigned long, unsigned long> : CBaseClassNonCopyable, STLport vector text from chunk 662). All first or second probe.

## Parked for Fable 5.1 (2)
- 00a5afb0 CCachedFile::Write DIFFER(97v96): retail bytes + trailing 0xCC after the final `call _CxxThrowException` (int3-after-noreturn; retail 0x00A5B010 is that 0xCC) | tried: 1 probe (exact) | next: lane gap: int3-after-noreturn -- land against the N+1 span (GOTCHAS 2026-10-22 procedure); differ/00a5afb0.cpp + .test.cpp
- 00a5b190 CCachedFile::SetLength DIFFER(97v96): same, retail 0x00A5B1F0 is the 0xCC | tried: 1 probe (exact) | next: lane gap: int3-after-noreturn -- land against the N+1 span; differ/00a5b190.cpp + .test.cpp

## Skipped
- none.

## Label corrections
- 0x00B0A060 "C3DMeshStripifier::~C3DMeshStripifier" -> NParticleEngine::CPSCSplineBase::~CPSCSplineBase.
- 0x00A6B5B0 "C3DMeshStripifier::~C3DMeshStripifier" -> CManagedTextureData::~CManagedTextureData.
- 0x009AC7C0 "C3DMeshStripifier::~C3DMeshStripifier" -> NBankFileManager::CRetailBank::~CRetailBank.
- 0x0091D8C0: catalog vtable note "CAIStateGroup_HoverMovementBase#2" -> owner is CAIStateGroup_EnemyCreatureBase (vtable 0x0128B8BC slot 2); the PDB-spelled label was right.
- The other six labels (BlockRespond / WaspQueenDie FrameUpdate, OnActivate, CCachedFile Write / SetLength, SetUpNeighbours, ~CEngineInternalAttachedPrimitiveInfo) were confirmed right.
- Manifest callee labels found wrong: 0x00692910 "_Dest_val<...CMusicManager...>" (is PeekDefinitionManager), 0x006925B0 / 0x006925C0 "CWorld::DrawGetEnvironment" (GetTotalTimeWorldFrames / GetFramesRunning), 0x00B0A160 "CTCInventoryItem::OnDie" (~CPSCSingleSprite), 0x00B1B490 / 0x00B230C0 / 0x00B3AB80 "CLandscapeBackgroundPatch vector_deleting_destructor" (??_G of CPSCSpline / CPSCSplineBase / CEngineInternalAttachedPrimitiveInfo).

## Descriptive names (not PDB-attested)
- CEnginePersistentPrimitiveBase slot 3 `NotifyReleased` and slot 2 `CUnprovenSlot2_012a3d60` (landed spellings of 0x007CA8E0; no PDB name for the slot found).
- ENavQuadTreeNeighbours enumerators `NAV_NEIGHBOUR_0..3` (only the enum type is in the PDB), CBBBFileException enumerator `NotOpenForWrite` (=8, as landed CDiskFileWin32::Write 0x0099AA90), CANavQuadTreeNode placeholder slots `VFuncNN` (as landed 0x00A75F10).
- CAIStateGroup_EnemyCreatureBase slots 14-21: names are FableWin.pdb's; the slot order 14 Init, 15 IsRunnableLeaf, 16 OnActivateLeaf, 17 CleanupLeaf, 18 GetCombatProximity, 19 GetCombatMinDistance, 20 IsCombatStateGroup, 21 IsRequireDirectAwarenessOfThing is inferred from the stub bodies of vtable 0x0128B8BC (ret / mov al,1 / fld / _purecall) and the landed 0x008FE0F2 (OnActivateLeaf, slot 16).
- CAFile slots 1-3 / 5-7 in the CCachedFile sources (Open/Close/Read/SetPosition/MovePosition/GetPosition) follow CMemoryFile's vtable; parameter lists of the unused ones are simplified.

## Header debt
- engine/CNavNavigableLeafNode.h, engine/CCreatureAction_BlockRespond.h, engine/CAIStateGroup_EnemyCreatureBase.h, engine/CManagedTextureData.h:
  flattened layouts with the vptr as a data member and no base chain: the rows need virtual dispatch / the base dtor chain, so
  local models with the header's (or Ego_r's retail-adjusted) offsets are used.
- rebuild/include/fable_retail_banks.h: flat non-polymorphic CRetailBank: local model in 0x009AC7C0.
- No engine header for CCachedFile, CAFile, CBBBFileException, CPSCSplineBase / CPSCBase / CCardinalSpline, CEngineInternalAttachedPrimitiveInfo,
  CEnginePrimitiveHandle, CCreatureAction_WaspQueenDie, CCreatureAction_BlockBase, CCombatActionBase, CAIStateGroupBase (retail-usable).
- engine/CCountedPointer.h, engine/CBaseClass.h, engine/CBaseClassNonCopyable.h, engine/CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE) are used as-is.
- 0x0091D8C0 uses engine/CWorld.h and engine/CAIBrain.h (flattened, offsets right); their inline members CWorld::GetCombatManager()
  (FableWin ?GetCombatManager@CWorld@@QAEAAVCCombatManager@@XZ) and CAIBrain::GetCreature() cannot be declared on the generated
  classes, so they appear as a free inline `GetCombatManager(CWorld&)` and an inline CAIStateGroupBase::GetCreature() over
  PBrain->Creature: re-host them as members when the headers gain the real class models.
- engine/CANavQuadTreeNode.h (0x00A75B70) models the vptr as a data member: local CANavQuadTreeNode (vland SHARED_HEADER_AVAILABLE warn).
