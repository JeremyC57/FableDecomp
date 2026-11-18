# NF67 header overlay (hdr/)

Each file is a copy of the CURRENT repo header (re-diffed at hand-back) with `#ifdef` opt-ins only. Default path check: each overlay
and its repo header were run through `cpp -P` with no opt-in macro defined (rebuild/include on the path); "default tokens" below says
whether the two preprocessed texts are identical after whitespace normalisation (the __LINE__-numbered FABLE_STATIC_ASSERT typedef
names are normalised: added lines shift them; nothing references them). Macro names are member-specific (merge with NF64 / NF66 / NF65).

- `stlport/fable_containers.h` (copy of rebuild/include/stlport/fable_containers.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_STLP_LIST_REMOVE_IF_NOINLINE`
- `stlport/fable_map.h` (copy of rebuild/include/stlport/fable_map.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_STLP_MULTIMAP_EMPTY`, `FABLE_RETAIL_STLP_TREE_MINSERT_NOINLINE`
- `engine/CAIStateGroup_BanditKingLunge.h` (copy of rebuild/include/engine/CAIStateGroup_BanditKingLunge.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE`
- `engine/CAIStateGroup_CarryCrateAndFollow.h` (copy of rebuild/include/engine/CAIStateGroup_CarryCrateAndFollow.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW_EXPRESSION`
- `engine/CAIStateGroup_IceTrollIcicleAssault.h` (copy of rebuild/include/engine/CAIStateGroup_IceTrollIcicleAssault.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT`
- `engine/CAIStateGroup_MinionFollowThing.h` (copy of rebuild/include/engine/CAIStateGroup_MinionFollowThing.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING`
- `engine/CAIStateGroup_MinionSitAtInn.h` (copy of rebuild/include/engine/CAIStateGroup_MinionSitAtInn.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_MINIONSITATINN`, `FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE`
- `engine/CAIStateGroup_MinionSitOnSeatBase.h` (copy of rebuild/include/engine/CAIStateGroup_MinionSitOnSeatBase.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE`, `FABLE_RETAIL_CAISTATEGROUP_USESSEAT`
- `engine/CAIStateGroup_SitInPub.h` (copy of rebuild/include/engine/CAIStateGroup_SitInPub.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_SITINPUB`, `FABLE_RETAIL_CAISTATEGROUP_USESSEAT`
- `engine/CAIStateGroup_Thunder.h` (copy of rebuild/include/engine/CAIStateGroup_Thunder.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CAISTATEGROUP_THUNDER`
- `engine/CAIStateGroup_UsesSeat.h` (NEW FILE: rebuild/include/engine/CAIStateGroup_UsesSeat.h does not exist; the whole content is under its opt-in, so the default path is empty): opt-ins 
- `engine/CArmourDef.h` (copy of rebuild/include/engine/CArmourDef.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE`
- `engine/CCreatureAction_BanditKingLunge.h` (copy of rebuild/include/engine/CCreatureAction_BanditKingLunge.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CCREATUREACTION_BANDITKINGLUNGE`
- `engine/CCreatureAction_IceTrollIcicleAssaultInto.h` (copy of rebuild/include/engine/CCreatureAction_IceTrollIcicleAssaultInto.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEASSAULTINTO`
- `engine/CCreatureAction_ThunderLightningStorm.h` (copy of rebuild/include/engine/CCreatureAction_ThunderLightningStorm.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CCREATUREACTION_THUNDERLIGHTNINGSTORM`
- `engine/CDefString.h` (copy of rebuild/include/engine/CDefString.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING`
- `engine/CGameScriptThing.h` (copy of rebuild/include/engine/CGameScriptThing.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CGAMESCRIPTTHING_MSGGETTHINGSKILLED`
- `engine/CHitParameters.h` (copy of rebuild/include/engine/CHitParameters.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CHITPARAMETERS_SETASMODIFIEDBYARMOUR`
- `engine/CQuestManager.h` (copy of rebuild/include/engine/CQuestManager.h; removed lines vs repo: 5; default tokens identical: yes): opt-ins `FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_SCRIPT`
- `engine/CScriptBase.h` (copy of rebuild/include/engine/CScriptBase.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CSCRIPTBASE_GETPACTIVETHREAD_INLINE`
- `engine/CSpawnedFuncBase.h` (copy of rebuild/include/engine/CSpawnedFuncBase.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CSPAWNEDFUNCBASE_SCRIPTFRAMES_INLINE`
- `engine/CTCEntityEvents.h` (copy of rebuild/include/engine/CTCEntityEvents.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY`, `FABLE_RETAIL_CTCENTITYEVENTS_REMOVEEVENTS`
- `engine/CThingAICreature.h` (copy of rebuild/include/engine/CThingAICreature.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CTHINGAICREATURE_GETMOTHERCREATURE`, `FABLE_RETAIL_CTHINGAICREATURE_ISIMMUNETOATTACK`
- `engine/LHNetworkTypes.h` (copy of rebuild/include/engine/LHNetworkTypes.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins 
- `engine/LSocket.h` (copy of rebuild/include/engine/LSocket.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_LSOCKET_LADDRESS_CTOR_OUTOFLINE`, `FABLE_RETAIL_LSOCKET_SENDBLOCK`
- `engine/NEntityEvents_CEventExpressionPerformedTo.h` (copy of rebuild/include/engine/NEntityEvents_CEventExpressionPerformedTo.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS`
- `engine/NThingShot_CTCShotDefinitionBase.h` (copy of rebuild/include/engine/NThingShot_CTCShotDefinitionBase.h; removed lines vs repo: 0; default tokens identical: yes): opt-ins `FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ISABLETOBEDEFLECTED`

Notes on the wrapped headers: each NF67 opt-in wraps the whole generated class in `#ifdef <macro> <retail class> #else <the repo text verbatim> #endif`,
so the default path is token-identical (checked above). The retail branches are declaration-only class definitions with the same size and
member offsets as the generated layout (FABLE_STATIC_ASSERT on sizeof).
- `engine/CAIStateGroup_IceTrollIcicleAssault.h` (`FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT`): CAIStateGroupBase-derived retail class,
  PTarget CIntelligentPointer<CThing> +0x14 (row 0x009627C0).
- `engine/CCreatureAction_IceTrollIcicleAssaultInto.h` (`FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEASSAULTINTO`): CCombatActionBase-derived,
  no own data (0x114), PDB reference ctor 0x00962990 (row 0x009627C0).
- `engine/CAIStateGroup_BanditKingLunge.h` (`FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE`): Player CIntelligentPointer<CThingPlayerCreature>
  +0x14, TimeOfLastLunge +0x1c, protected CanLungeAtPlayer(CThing*) 0x009613A0 (row 0x00961880).
- `engine/CCreatureAction_BanditKingLunge.h` (`FABLE_RETAIL_CCREATUREACTION_BANDITKINGLUNGE`): CCombatActionBase-derived, no own data, PDB
  reference ctor 0x009619C0 (row 0x00961880).
- `engine/CAIStateGroup_Thunder.h` (`FABLE_RETAIL_CAISTATEGROUP_THUNDER`): PArenaCentre +0x14, bIsRangeAttack +0x1c, PTargetThing
  CIntelligentPointer<CThingCreatureBase> +0x1d, HitCount +0x28; public Update (row 0x0096C4F0).
- `engine/CCreatureAction_ThunderLightningStorm.h` (`FABLE_RETAIL_CCREATUREACTION_THUNDERLIGHTNINGSTORM`): ctor (CThingCreatureBase&) 0x0096C6E0
  (row 0x0096C4F0).
- `engine/CAIStateGroup_UsesSeat.h` (`FABLE_RETAIL_CAISTATEGROUP_USESSEAT`): **NEW FILE, no repo counterpart** -- the whole content is under the
  macro, so a TU that does not define it sees an empty header. Ego_r layout: PSeat CIntelligentPointer<CThingObject> +0x14, GotSeatPos +0x1c,
  Sitting +0x1d, SeatPosIndex +0x20, SeatPos +0x24, SittingOnBench +0x30, sizeof 0x34 (base of MinionSitOnSeatBase / SitInPub). Coordinator:
  add it as a shared header (rows 0x0090FAE0 / 0x0094B8B0).
- `engine/CAIStateGroup_MinionSitOnSeatBase.h` (`FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE`), `engine/CAIStateGroup_MinionSitAtInn.h`
  (`FABLE_RETAIL_CAISTATEGROUP_MINIONSITATINN`), `engine/CAIStateGroup_SitInPub.h` (`FABLE_RETAIL_CAISTATEGROUP_SITINPUB`, PBuilding +0x34,
  private GetBenchWithOnePersonSittingThere 0x0094B840, IsPubTime): rows 0x0090FAE0 / 0x0094B8B0.
- `engine/CThingAICreature.h`: `FABLE_RETAIL_CTHINGAICREATURE_GETMOTHERCREATURE` (inline GetMotherCreature, needs HOMEBUILDING + SAVE; row
  0x00915190) and `FABLE_RETAIL_CTHINGAICREATURE_ISIMMUNETOATTACK` (inline IsImmuneToAttack, +0x1C8 bit 6; row 0x00869020). Body-only blocks.
- `engine/CTCEntityEvents.h` (`FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY`): the GetEvent<E,F> template body visible and noinline (needs
  STLP_LIST_REVERSE; row 0x00915190).
- `engine/CAIStateGroup_MinionFollowThing.h` (`FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING`, with `enum EMovementType;`): row 0x00921440.
- `engine/CDefString.h` (`FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING`): operator==(const CCharString&) const 0x004B7980 (row 0x00921440).
- `engine/NEntityEvents_CEventExpressionPerformedTo.h` (`FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS`): the four inline getters.
- `engine/CAIStateGroup_CarryCrateAndFollow.h` (`FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW_EXPRESSION`): private IsExpressionPerfomedTo
  (row 0x00966290).
- `engine/CGameScriptThing.h` (`FABLE_RETAIL_CGAMESCRIPTTHING_MSGGETTHINGSKILLED`), `engine/CQuestManager.h`
  (`FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_SCRIPT`), `engine/CScriptBase.h` (`FABLE_RETAIL_CSCRIPTBASE_GETPACTIVETHREAD_INLINE`, the inline body
  with the original declaration kept in `#else`), `engine/CSpawnedFuncBase.h` (`FABLE_RETAIL_CSPAWNEDFUNCBASE_SCRIPTFRAMES_INLINE`, likewise):
  row 0x008D3DD0.
- `engine/CHitParameters.h` (`FABLE_RETAIL_CHITPARAMETERS_SETASMODIFIEDBYARMOUR`): the member declaration plus `class CArmourDef;`, skipped when
  FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT already declares it (row 0x00869020).
- `engine/CArmourDef.h` (`FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE`): AugmentationResponse as std::map<EObjectAugmentationType,float>
  (same 12 bytes; `#ifdef <typed line> #else <repo line> #endif`), for the visible body of 0x00868A80 in row 0x00869020.
- `engine/NThingShot_CTCShotDefinitionBase.h` (`FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ISABLETOBEDEFLECTED`): IsAbleToBeDeflected 0x007BED60
  declaration (+ StatGetInterfaceType unless ONHIT / WEAPONTRAIL give it); row 0x00869020.
- `stlport/fable_map.h`: `FABLE_RETAIL_STLP_TREE_MINSERT_NOINLINE` (`__declspec(noinline)` on the private _M_insert declaration; pair with
  MAP_ORIGINAL_INSERT_BODY) and `FABLE_RETAIL_STLP_MULTIMAP_EMPTY` (multimap::empty; needs STLP_SET_EMPTY). Row 0x00869020. NF41 lesson: a row
  must include stlport/fable_map.h after its macros and before any engine header, so the overlay copy is the one read.
- `stlport/fable_containers.h` (`FABLE_RETAIL_STLP_LIST_REMOVE_IF_NOINLINE`): the STLport _list.c member template remove_if body, visible and
  `__declspec(noinline)` (skipped under the declaration-only REMOVE_IF_OUTOFLINE); needs STLP_LIST (erase). Row 0x008A7980.
- `engine/CTCEntityEvents.h` (`FABLE_RETAIL_CTCENTITYEVENTS_REMOVEEVENTS`): the inline RemoveEvents<F>(const F&) template (FableWin.pdb
  ??$RemoveEvents@VCIsEventOfType@..), plus a forward declaration of NEntityEvents::CConvFilterToCP<F>. Row 0x008A7980.
- `engine/LSocket.h` (`FABLE_RETAIL_LSOCKET_SENDBLOCK`: SendBlock declaration; `FABLE_RETAIL_LSOCKET_LADDRESS_CTOR_OUTOFLINE`: ??0LAddress
  declared only, skipped under FABLE_RETAIL_LSOCKET_LADDRESS): used only by the BLOCKED draft work/att/008f95d0_d1.cpp (kept so it can be
  regraded). `engine/LHNetworkTypes.h` is a VERBATIM copy (no change): LSocket.h includes it by a path relative to its own directory, so
  the overlay copy needs it beside it.
