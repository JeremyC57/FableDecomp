# NF68 header overlay (hdr/)

Each file is a copy of the CURRENT repo header (re-diffed at hand-back) with `#ifdef` opt-ins only. Default path check: each overlay
and its repo header were run through `cpp -P` with no opt-in macro defined (rebuild/include on the path); "default tokens" below says
whether the two preprocessed texts are identical after whitespace normalisation (the __LINE__-numbered FABLE_STATIC_ASSERT typedef
names are normalised: added lines shift them; nothing references them). Macro names are member-specific (merge with NF66 / NF67).

- `stlport/fable_containers.h` (copy of rebuild/include/stlport/fable_containers.h; removed lines vs repo: 1; default tokens identical: yes): opt-ins `FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE`, `FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE`, `FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE`, `FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE`, `FABLE_RETAIL_STLP_VECTOR_C`, `FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL`
- `engine/CAIStateGroup_CreatureAdvanceInCover.h` (copy of rebuild/include/engine/CAIStateGroup_CreatureAdvanceInCover.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_UPDATE`
- `engine/CAIStateGroup_CreatureTakeCover.h` (copy of rebuild/include/engine/CAIStateGroup_CreatureTakeCover.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_UPDATE`
- `engine/CAIStateGroup_CreatureUseCoverBase.h` (copy of rebuild/include/engine/CAIStateGroup_CreatureUseCoverBase.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE`, `FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TAKECOVER`, `FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST`
- `engine/CBankFile.h` (copy of rebuild/include/engine/CBankFile.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CBANKFILE_ALLOCATEBUFFERS`, `FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR`, `FABLE_RETAIL_CBANKFILE_SYMBOLINDICES`
- `engine/CBankFileAsync.h` (copy of rebuild/include/engine/CBankFileAsync.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY`
- `engine/CCreatureAction_ProjectileCrouch.h` (copy of rebuild/include/engine/CCreatureAction_ProjectileCrouch.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS`
- `engine/CEngineLightingManager.h` (copy of rebuild/include/engine/CEngineLightingManager.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINELIGHTINGMANAGER_ENABLEFOG`
- `engine/CEnginePrimitiveRenderer.h` (copy of rebuild/include/engine/CEnginePrimitiveRenderer.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINEPRIMITIVERENDERER_PRERENDERLAYER`
- `engine/CEngineWaterRenderer.h` (copy of rebuild/include/engine/CEngineWaterRenderer.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CENGINEWATERRENDERER_SETUPSEANONREFLECTIVEFOREGROUND`
- `engine/CLipSyncDataBank.h` (copy of rebuild/include/engine/CLipSyncDataBank.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CLIPSYNCDATABANK_CLASS`, `FABLE_RETAIL_CLIPSYNCDATABANK_LIPSYNCDATA`
- `engine/CPhysicsMesh.h` (copy of rebuild/include/engine/CPhysicsMesh.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CPHYSICSMESH_SIZEOF`
- `engine/CShaderRenderManager.h` (copy of rebuild/include/engine/CShaderRenderManager.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins 
- `engine/CTCPerceiveThing.h` (copy of rebuild/include/engine/CTCPerceiveThing.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE`
- `engine/CV_TravellingHeroesScript.h` (copy of rebuild/include/engine/CV_TravellingHeroesScript.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_THUNDER`
- `engine/NScript_CQ_WaspBossScript.h` (copy of rebuild/include/engine/NScript_CQ_WaspBossScript.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CQ_WASPBOSSSCRIPT_DOMISSION`

## Opt-ins by header (all `#ifdef`, default path token-identical; see the table above)
- engine/CBankFile.h: FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR (CRuntimeData as a class with FableWin.pdb's ??0CRuntimeData@CBankFile@@QAE@XZ
  default ctor, member-wise zero), FABLE_RETAIL_CBANKFILE_SYMBOLINDICES (SymbolIndices typed std::map<CCharString,unsigned long>),
  FABLE_RETAIL_CBANKFILE_ALLOCATEBUFFERS (private AllocateBuffers(unsigned long) declaration). Row 0x009CEAE0.
- engine/CEngineWaterRenderer.h: FABLE_RETAIL_CENGINEWATERRENDERER_SETUPSEANONREFLECTIVEFOREGROUND (private declaration). Row 0x00B7FA20.
- stlport/fable_containers.h: FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE, FABLE_RETAIL_STLP_FILL_NOINLINE,
  FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE (row 0x00AD85D0), FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE, FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE,
  FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL, FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL (row 0x009F6890): the same STLport bodies kept out of line
  as same-TU noinline __fastcall COMDATs (or declaration-only protected members), as those retail TUs call them. The row includes
  stlport/fable_containers.h first, so the overlay copy is the one the TU reads (NF41 lesson).
- engine/CEnginePrimitiveRenderer.h: FABLE_RETAIL_CENGINEPRIMITIVERENDERER_PRERENDERLAYER (PreRenderLayer / private InitialiseGlobalLights
  declarations). engine/CShaderRenderManager.h: FABLE_RETAIL_CSHADERRENDERMANAGER_GETSHADERSENABLED_INLINE (FableWin inline getters).
  engine/CEngineLightingManager.h: FABLE_RETAIL_CENGINELIGHTINGMANAGER_ENABLEFOG (declaration). Row 0x00B32AD0.
- Note on stlport/fable_containers.h ("removed lines vs repo: 1"): the existing line `#ifndef FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22`
  (VECTOR_C's in-class _M_allocate_and_copy body) now reads `#if !defined(..._ORIGINAL_SCOPE_PASS22) && !defined(FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL)`;
  without the new macro it selects exactly the same text (default and every existing opt-in path token-identical; cpp check above).
  The generated macro list above misses opt-ins introduced by `#elif defined(...)` (FABLE_RETAIL_STLP_FILL_NOINLINE,
  FABLE_RETAIL_CSHADERRENDERMANAGER_GETSHADERSENABLED_INLINE) and FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL; the list in this section is complete.
- Extra path check at hand-back (NF68): every overlay was also preprocessed against its repo copy with `-DFABLE_RETAIL_STLP_VECTOR_C -DFABLE_RETAIL_STLP_VECTOR`,
  with `-DFABLE_RETAIL_CBANKFILE_FILENAMEINDICES -DFABLE_RETAIL_CARRAY -DFABLE_RETAIL_CCHARSTRING`, and with
  `-DFABLE_RETAIL_CENGINEPRIMITIVERENDERER -DFABLE_RETAIL_CENGINELIGHTINGMANAGER`: identical in each case (static-assert names normalised).
- engine/CLipSyncDataBank.h: FABLE_RETAIL_CLIPSYNCDATABANK_CLASS (the retail class `CLipSyncDataBank : public CBankFileAsync` with Entries opaque
  +0x164, ResourceBank CCountedPointer<CResourceBank> +0x170, SkipUpdate +0x178, PhonemeBank +0x17c; sizeof 0x180 and offsets asserted; the
  generated flattened class is the #else branch, unchanged). Needs the retail engine/CBankFileAsync.h and FABLE_RETAIL_DEF_CLASS_BASE. Row 0x00A42980.
- engine/CPhysicsMesh.h: FABLE_RETAIL_CPHYSICSMESH_SIZEOF (declaration of ?Sizeof@CPhysicsMesh@@QBEJXZ, long). Row 0x00A25BD0.
- engine/CCreatureAction_ProjectileCrouch.h: FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS (the retail class `: public CCombatActionBase`, Ego_r.pdb
  ctor (CThingCreatureBase&, CThingCreatureBase&, const C3DVector&) 0x008B8310, TurnToFaceTarget +0x114, FacePos +0x115 (PDB +300 / +301), sizeof 0x124, offsets asserted; as
  engine/CCreatureAction_DuckAndCover.h; members byte-packed so FacePos sits at +0x115 as the landed ctor 0x008B8310 stores it, class aligned 4). Rows 0x009790F0 / 0x0097A630.
- engine/CAIStateGroup_CreatureUseCoverBase.h: FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE (GetHiddenCoverPosAndSetUpCoverThing / SetAtFailed /
  RemoveThingFromCoverList(const CThing*)), _TAKECOVER (IsLowCover(const CThing&) const / ClearCoverThing / GetHiddenCoverPos / CanSeeTarget),
  _TYPED_COVERLIST (CoverList as std::vector<CIntelligentPointer<CThing> >, same 12 bytes at +0x28). Rows 0x009790F0 / 0x0097A630 / 0x00979F70.
- engine/CAIStateGroup_CreatureAdvanceInCover.h: FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_UPDATE (virtual Update, protected IsStillRunnable),
  inside the existing ISRUNNABLELEAF retail class. Row 0x009790F0.
- engine/CAIStateGroup_CreatureTakeCover.h: FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_UPDATE (virtual Update; private IsStillRunnable,
  AttackTargetThing, MoveFromCoverToFiringPosition), inside the existing ISRUNNABLELEAF retail class. Rows 0x0097A630 / 0x00979F70.
- engine/CTCPerceiveThing.h: FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE (declaration of ?SetAsDirectlyAwareOfPerceivedThing@CTCPerceiveThing@@QAEXXZ,
  out of line 0x007449B0) in the FABLE_RETAIL_CTCPERCEIVETHING class. Row 0x0097A630.
- engine/CBankFileAsync.h: FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY (declaration of the PDB ?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z,
  retail 0x009D58D0; neither PDB has the CreateEntry the existing CREATEENTRY opt-in declares: NAME_FIXES.tsv). Rows 0x00A42980 / 0x00A25BD0.
- engine/NScript_CQ_WaspBossScript.h: FABLE_RETAIL_CQ_WASPBOSSSCRIPT_DOMISSION (DoMission / GuildmasterHelp / WaspIntro declarations). Used only by the
  blocked 0x00E12580 port attempt (work/att/); no ported row needs it.
- engine/CV_TravellingHeroesScript.h: FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_THUNDER (PDB ThunderChatter / PlayCutscene(CCharString) declarations). Used only by
  the blocked 0x00EE2490 port attempt; no ported row needs it.
