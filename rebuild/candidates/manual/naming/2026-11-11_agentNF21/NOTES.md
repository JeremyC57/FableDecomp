# NF21 notes (Codex pass 20 name corrections, tag land1011bu)

## Result
- 176 queue entries: 29 done, 147 partial, 0 rejected (APPLIED.tsv gives the reason for each partial).
- 87 rows changed (rows/*.cpp; new tests rows/006622a0_test.cpp, rows/006a76a0_test.cpp, rows/006af1c0_test.cpp).
  Final pass: every changed row compiled again (80 RELOCATION_MATCH, 7 MATCH) and its test PASSes with the catalog
  PassPattern (work/final_pairs.log, work/final_tests.log). Most grades on one row: 5 (00681a70).
- 21 self-row renames (RENAMES.tsv section 1) and 93 old->new bound-symbol respellings (section 2, from comparing the
  repo object with the new object for each row: work/pairdiff.txt).
- Header opt-ins: hdr/README.md (CTCBuyableHouse WIFE, CDataOutputStream WRITEUBYTE, N3DPrimitive CBaseBlock SAVEBINARY).

## How claims were checked
- Functions: the claimed mangled name appears in ghidra_out/egor_pdb_names.tsv and/or fablewin_pdb_names.tsv, plus a
  second source: the callee body (filters: 0x006AF1C0 compares Type +0x08 = IsOfType; 0x006AF200 OriginatorUID +0x20 =
  IsOriginatedByThing; 0x006AF1E0 TargetUID +0x28 = IsTargettedAtThing), the RTTI base lists in ghidra_out/pdb_class_keys.tsv
  (CCreatureAction_FinishFireballSpell -> CCombatActionBase; CThingPlayerCreature -> CThingCreatureBase), the Ego_r layout
  (struct_layouts_egor.tsv), or the shared header that already names the address.
- Data globals: the local PDB name sets list functions only. Crime*Text / BUILDING_BOUNDRY / GDefStringTable were checked
  through the PDB dynamic-initializer names; NEngineInternal::GPPrimitiveRenderer2D and GPCamera could not be checked here.
- GameCODA / SAL entries (00c3*, 00c4*, 012b5ab0, 0139c988, 0143bb48/58, 0143bc28): no GameCODA SDK CodeView on this
  machine, so they could not be checked; nothing applied.

## Things the coordinator should know
- 0091a5b0 (CMessageEventManager::FindMostRecentMessage<Bind3<IsOfType,IsOriginatedByThing,IsTargettedAtThing>>): self
  rename to the const PDB instance (QBEPBVCMessageEvent, Ego_r 0x0093E080). Its 2nd and 3rd filters were HasExtraData /
  OccuredBetween before: the bodies at 0x006AF200 / 0x006AF1E0 prove IsOriginatedByThing / IsTargettedAtThing.
- 00753ba0: the two head-turn globals now bind the landed definition spelling g_CCharString_013bace4 / _013bace8 (was
  g_lookHeadAnimHorizontal/Vertical typed long). The PDB names are ST_HEAD_TURN_H / ST_HEAD_TURN_V; a NAME_FIXES respell of
  the definition row(s) should follow (const-ness not proven).
- 00682140 / 00681610: ApplyOpinionTransientOffset is AdjustOpinionTransient(EOpinion, ...); the COpinionTransientOffset
  ctor takes EOpinion first (both PDBs). 006cfda0 / 006d0180 call it with (EOpinion)n.
- 006345c0: CPlayer::InitActiveMode / UnInitActiveMode (0x00633CB0 / 0x00633BF0) are private (AAE) in Ego_r.pdb; the six
  other AddMode/RemoveMode binders listed in APPLIED.tsv still pass long.
- 0072c790: 0x0062E900 is ~CCombatActionBase; the enflame action now has an implicit destructor so the temporary calls it.
- 0089a390: type 0x46 is MESSAGE_EVENT_HERO_PICKED_LOCK (shared EMessageEventType.h); the door goes in the long data
  dword as (long)door. 007b0950 likewise passes the fish as (long)fish.
- 0064499c / 00645292: the C2DCoordF stand-in temp is now an explicit C2DVector(pos.X, pos.Y) temporary (retail copies
  the position for each sprite).

## Rule slips / debt left in changed rows
- 0072c790 and 007c9180 now carry a small local `CCreatureActionBase` model (vptr, pad, the +0x2C intelligent pointer)
  under a local `CCombatActionBase`. Both rows already modelled these classes locally (007c9180 as the stand-in
  namespace N_0062e900); the shared engine/CCreatureActionBase.h + CCombatActionBase.h clash with their local
  CBaseIntelligentPointer / CCharString models. 007e5b90 does use the shared headers.
- Pre-existing local models kept as they were (CEnginePrimitive2DSprite, CEnginePrimitiveSprite, CThing views, ...);
  only the declarations that carry the bindings were respelled.
- 00681a70: one grade hung on a header #error and was killed by `timeout 240`.

## Header debt seen (not changed)
- engine/CMessageEventManager.h uses FABLE_STATIC_ASSERT without including rebuild_abi.h.
- engine/CThingFilter_IsNot.h needs CThing but does not forward-declare it.
- 0x0072D360 is ?SetPItemRemoved@CCreatureActionBase@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z (shared header); row
  007c9180 still binds it as CCreatureAction_FinishFireballSpell::SetLookAtThing(CIntelligentPointer<const CThing>). Not
  in this queue.
