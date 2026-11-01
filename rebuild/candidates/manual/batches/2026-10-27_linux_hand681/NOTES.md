# Hand chunk 681 (CFishCompOrganiser .. CV_BordelloScript): 9 of 10 byte-exact

All nine matches are RELOCATION_MATCH. vland (read-only) shows 9 WINS, every behaviour test PASS, and no RELOC warning after
the CTextureManager / CPixelFormat header fix. The parked row is 0x00AE1A90 (21 probes). Destructor labels were right for 5 of
9 rows (2 of the 5 also needed the NScript nesting added). The other four were wrong: two CParticleEmitter placeholders,
CV_FisticuffsClubScript and CSystemManager.

## What matched (9)
- **Quest / script implicit destructors (6)**, built from the chunk 665/672/673 template: the PDB ctor is defined in the TU, a
  local CLevelScriptBase<T> declares no dtor, and CScriptThing, CTimer and the seh dtors are inline. Each matched on the first
  probe.
  - NScript::CV_ExposeMayorScript 0x00E7B2E0: four seh_* members and Evidence.
  - NScript::CLadyGreyWifeManagerScript 0x00CF9210: five CScriptThing members and two CCharString members.
  - NScript::CQ_RansomVictimScript 0x00DEBCB0: five seh_* members, a CTimer and a CCharString.
  - NScript::CV_BordelloScript 0x00E46A50: the STLport map with an out-of-line _M_erase, four seh_* members and a CScriptThing.
  - NScript::CQR_EscortTraderScript 0x00E24C80: two `vector<CCharString>[3]` arrays become retail's stack-counter loops (no
    EH vector iterator), CCharString[10]/[5] become countdown loops, plus four CTimer members.
  - NScript::CQ_MinionCampScript 0x00DA2F90: two STLport `list<CScriptThing>`. The first probe matched with
    `_List_base::clear` only declared. The second probe defines clear with its STLport _list.c body, and VC7.1 still calls
    0x00CD4DF0 out of line, so the faithful form is kept.
- **Entity-script implicit destructor (1)**: NScript::CV_FishingCompetitionScript::CFishCompOrganiser 0x00E7F210. It is the
  chunk 672 CBriarRose template with the STLport vector + map text and the engine CEntityScriptBase / CLevelScriptBase
  headers. First probe.
- **Engine user destructors (2)**:
  - CEnginePrimitiveManagerRepeatedStaticMeshes 0x00B999F0: the body is `CMemoryAllocator::DeallocateAligned(SIMDVariableBlock)`.
    Twelve CPixelShader / CVertexShader members follow, using the ZSprites 0x00BABE60 shader models with their inline user dtors.
    First probe.
  - CEngineSurfaceCompositionManager 0x00B2D1B0: the body loops over CachedSurfaces, calling `cache->GraphicFrame.UnInitialise();
    cache->ManagedTexture.Release(); RemoveResource(*cache);`. Members (shader, texture manager, map, three CWideString, three
    CGraphicDataBank, list) and the two bases follow. Probe 1 was 383v374. Shape lesson: CGraphicFrame::UnInitialise must go
    through a local `CGraphicFrameData* data = FrameData;`, as the landed 0x00B2EEC0 does. Writing `FrameData` directly
    re-reads it after the decrement and keeps the delete null test.

## Parked for Fable 5.1 (1)
- 00ae1a90 CParticleProgram::ExecuteProgram DIFFER(296v292): `sub esp,14h` v `10h`. VC7.1 gives the empty `__false_type` tag of
  `_M_insert_overflow` its own frame slot (`lea edx,[esp+1Bh]`), but retail homes it on the live `simulation` argument slot
  (`lea ecx,[esp+2Ch]`). push_back's fast path reloads LoopCount (`mov ecx,[esp+20h]`) instead of keeping ecx. Case 1 uses
  edx/eax where retail uses eax/ecx, and the arguments in case 2 are scheduled in another order. The jump table, the register
  assignment and the rest match (37 differing lines).
  | tried (21 probes, probe/00ae1a90.log): STLport clear/push_back/back/pop_back text with a CArray::push_back wrapper; empty
  case labels; the FableWin-named inline empty SatisfyMinMaxDistance / Unbend / SphereCollision bodies, which give the 0..5
  jump table; __true_type / __false_type tags; CLoopBegin placement and aggregate initialisation; long count; header read
  order; raw-offset reads; a named constraint pointer; distance case first; a liveness test (simulation unused: the tag still
  gets a slot); CArray::push_back out of class; a plain vector; out-of-class Satisfy* definitions; empty-loop and dead-load
  inline bodies; const_cast in place of mutable; /Ob1 /Ob2 /Og- /Oa
  | next: find what makes VC7.1 home an empty temporary on a LIVE argument slot. The precedent 0x00B1B380 homes it on a DEAD
  one, the params slot after `mov esi,[esp+28h]`. Possibly simulation was register-allocated and then spilled to its home.
  The LoopCount reload should follow once the frame matches.

## Skipped
- None. No row runs past its function end: all ten end at `jmp`/`ret` and the oracle lengths, and 0x00AE1A90's 292 bytes
  include its own 6-entry case table at 0x00AE1B9C. No manifest split is needed.

## Label corrections
- 0x00E7F210 "CParticleEmitter::~CParticleEmitter_ScreenFree" -> NScript::CV_FishingCompetitionScript::CFishCompOrganiser::~CFishCompOrganiser.
- 0x00E24C80 "CParticleEmitter::~CParticleEmitter_0x24c80" -> NScript::CQR_EscortTraderScript::~CQR_EscortTraderScript.
- 0x00DA2F90 "CV_FisticuffsClubScript::~CV_FisticuffsClubScript" -> NScript::CQ_MinionCampScript::~CQ_MinionCampScript (the
  real CV_FisticuffsClubScript dtor is 0x00E8B750, chunk 673; the landed list clear 0x00CD4DF0 already names this row).
- 0x00E46A50 "CSystemManager::~CSystemManager" -> NScript::CV_BordelloScript::~CV_BordelloScript (vtable 0x012E38B0 has no
  RTTI COL; the body walks a script layout).
- 0x00E7B2E0 / 0x00CF9210 / 0x00DEBCB0: class right, NScript nesting missing in the catalog.
- Manifest ctor labels: 0x00E398F0 "CQ_HangingTreeGoodScript" is CV_BordelloScript's ctor (landed under the right class).
  0x00E7EF70 "CGameScriptThing" is CFishCompOrganiser's ctor. The landed ??_G stand-ins at 0x00E7F1F0, 0x00E24C60, 0x00E7B2C0,
  0x00DA2F70, 0x00CF91F0, 0x00DEBC90 and 0x00E46A30 are `CLandscapeBackgroundPatch vector deleting destructor` stand-ins.
- 0x00994B40, landed as "CCreatureActionBase::GetFramesRemaining", is CMemoryDataOutputStream::GetLength (NAME_FIXES.tsv).

## Proven
- 0x00E7F210 ??1CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@UAE@XZ -- ??_G 0x00E7F1F0 = slot 0 of vtable 0x012EA620; ctor 0x00E7EF70 -> factory 0x00E7EEF0 (new 78h) stored at 0x00E7E513 by CV_FishingCompetitionScript::Main 0x00E7E370 beside "FishCompOrganiser" (0x012EA554); Ego_r ??1/??0 names; Ego_r size 0x84 = 0x78 + 4 + 2x4
- 0x00E24C80 ??1CQR_EscortTraderScript@NScript@@UAE@XZ -- vtable 0x012E12E0 -> ctor 0x00E1DB00 -> Alloc 0x00E24C30 (push 120h) -> RegisterScripts 0x00CD7B7B "S_QRET"; conversion_queue QR_EscortTrader; Ego_r ??1
- 0x00E7B2E0 ??1CV_ExposeMayorScript@NScript@@UAE@XZ -- vtable 0x012E96CC -> ctor 0x00E755F0 -> Alloc 0x00E7B290 (push 0A0h) -> 0x00CD8667 "S_VEM"; conversion_queue V_ExposeMayor; Ego_r ??0
- 0x00DA2F90 ??1CQ_MinionCampScript@NScript@@UAE@XZ -- vtable 0x012D6C60 -> ctor 0x00D9E210 -> Alloc 0x00DA2F40 (push 0ACh) -> 0x00CD6D23 "S_QMC"; conversion_queue Q_MinionCamp; Ego_r ??1
- 0x00CF9210 ??1CLadyGreyWifeManagerScript@NScript@@UAE@XZ -- vtable 0x012C6C10 -> ctor 0x00CF88C0 -> Alloc 0x00CF91C0 (push 94h) -> 0x00CD5770 "LadyGreyWifeManager" / "S_LGWM"; conversion_queue; Ego_r ??1 (0x00CE5400)
- 0x00DEBCB0 ??1CQ_RansomVictimScript@NScript@@UAE@XZ -- vtable 0x012DCE70 -> ctor 0x00DE0320 -> Alloc 0x00DEBC60 (push 0BCh) -> 0x00CD73B6 "S_QRV"; conversion_queue Q_RansomVictim; Ego_r ??1
- 0x00E46A50 ??1CV_BordelloScript@NScript@@UAE@XZ -- vtable 0x012E38B0 (no RTTI) -> ctor 0x00E398F0 -> Alloc 0x00E46A00 (push 0C0h) -> 0x00CD8535 "S_VB"; conversion_queue V_Bordello; Ego_r ??1 / ??_G / ??_E; Ego_r size 0xD8 = 0xC0 + 0x14 + 4
- 0x00B999F0 ??1CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE@XZ -- own vtable store 0x012A3E9C (RTTI COL), slot 0 ??_G 0x00B9C6D0 sole caller; Ego_r ??1 0x00A51BB0
- 0x00B2D1B0 ??1CEngineSurfaceCompositionManager@@UAE@XZ -- own vtable stores 0x012A1238 / 0x012A1200, RTTI : CResourceBank, CEngineComponent@0x7C; slot 0 ??_G 0x00B2F6E0 sole caller; Ego_r ??1 0x009CE560
- 0x00AE1A90 ?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z -- sole caller 0x00AE2510 (`ret 0Ch` loop resetting and testing simulation+0x30) = ?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z; Ego_r / FableWin names; `ret 4`
- 0x00E7F1F0 / 0x00E24C60 / 0x00E7B2C0 / 0x00DA2F70 / 0x00CF91F0 / 0x00DEBC90 / 0x00E46A30 / 0x00B9C6D0 / 0x00B2F6E0 ??_G<Class>...@@UAEPAXI@Z -- slot 0 of the vtables above, each calling the dtor proven here
- 0x00E7EF70 / 0x00E1DB00 / 0x00E755F0 / 0x00D9E210 / 0x00CF88C0 / 0x00DE0320 / 0x00E398F0 ??0<Class>@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (CFishCompOrganiser: QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z) -- the vtable stores and Alloc chains above, Ego_r spellings
- 0x00CF91C0 ?Alloc@CLadyGreyWifeManagerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- fastcall factory stored at 0x00CD5770; Ego_r 0x00CE55E0
- 0x009FC3C0 ?RemoveResource@CResourceBank@@QAEXAAVCResource@@@Z -- `add ecx,4 ; jmp CResourceList::RemoveResource 0x009FC020` on ResourceList +4; Ego_r name set (AddResource / RemoveResource / AddToLoadingQueue); called with the cache (CResource base at +0) (not landed)
- 0x009F2B20 ?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z -- Ego_r; landed 0x00B47B50 binding
- 0x00995E70 ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ / 0x00994B40 ?GetLength@CMemoryDataOutputStream@@QBEKXZ -- bodies over Data (CArray<char>) at +8; called on CParticleProgram::Constraints; the only PDB spellings
- 0x00B2EA90 ?clear@?$_List_base@V?$CCountedPointer@VCCompositeSurfaceCache@@@@...@std@@QAEXXZ -- _list.c clear body; ~CEngineSurfaceCompositionManager calls it on CachedSurfaces
- 0x00B2EF20 ?_M_erase@?$_Rb_tree@U?$pair@JJ@std@@...VCEngineTextureMorph...@@AAEXPAU?$_Rb_tree_node@...@@Z -- recursive _tree.c body freeing CEngineTextureMorph::Blends; called on GeneratedMorphs
- 0x00ADF960 ?SatisfyDistanceConstraints@CParticleSimulation@@AAEXPBVCParticleDistanceConstraint@@K@Z -- Ego_r; the case-2 call of ExecuteProgram (not landed)

## Descriptive (no PDB spelling found)
- ExecuteProgram: the TConstraintType enumerator names (CT_REPEAT .. CT_SPHERE_COLLISION) and SConstraintHeader. The enum is
  global (W4TConstraintType@@); the record order follows the AddConstraint overloads.
- The nested enum placement of CQ_MinionCampScript::MissionStage / BriarRoseState and CQR_EscortTraderScript::ETradeRoutes (no
  mangled use fixes their scope). PATH_ID, STAGE_IN_PATH and EscortTraderQuestStage are nested per the PDB Transfer<>
  manglings. Enumerators are not in the dump.

## Header debt
- engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();`. Retail's is implicit, so the six quest rows restate it
  locally without a dtor, as chunks 664-677 did. The entity row uses the header for the enclosing class only.
- engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h, engine/CEngineSurfaceCompositionManager.h, engine/CTextureManager.h,
  engine/CResourceList.h and engine/CEngineComponent.h are flattened pack(1) records. The polymorphic classes and bases are
  restated locally with PDB member names. The shader classes follow the landed 0x00BABE60 models; CResourceBank,
  CResourceList and CFailedAllocationHandler follow the landed 0x009FC690 ones. CGraphicDataBank (0x30C) and CTextureManager
  (0x5D4) are opaque bodies with their virtual dtors.
- No engine/CCharString.h: the one-pointer local model the landed quest rows use. CCompositeSurfaceCache, CEngineTextureMorph
  and CManagedTexture follow the landed ctor 0x00B2C400.

## Callee names
All callees use PDB-confirmed spellings (objdump checked):
- engine and script: ~CScriptBase, ~CEntityScriptBase, ~CCharString, ~CBaseClass, ~CBaseObject, DeallocateAligned,
  ~CEnginePrimitiveManagerOcclusionBase, ~CResourceBank, ~CEngineComponent, ~CGraphicDataBank, ~CTextureManager,
  ~CWideString, CManagedTexture::Release, CGraphicFrameData::Uninitialise, CResourceBank::RemoveResource;
- STLport: the clear / _M_erase instances.
