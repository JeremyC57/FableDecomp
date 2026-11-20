# NF135 notes (round 10, PRIORITY slice 7 fifth pass)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log; updated at hand-back): see the "Counts" section at the end.

## Gate checks run for the q6453 rows (advisor list)
- `grep -rnE '(class|struct)[[:space:]]+(CModeBriarRoseTeleportIntangible|CModeJackOfBladesFlash|CModeDragonFlyingBase|CModeDragonFlying|
  CModeDragonStrafeBurninate|CModeIceTrollGroundSmashIcicleAssault)\b' rebuild/include`: no hit (no shared header for any row-local mode class).
- `tools/xr.sh 008e3bb0`: the only landed referencer of 0x008E3BB0 is 0x008DCB60 (so the GetAllThings respell leaves two spellings: owner stand-in
  self leaf + the PDB one).
- Funclets: no file under rebuild/src/compiled/01/21 names any of the six changed files.
- orig/engine/CSnowTrollDef.h is byte-equal to the tree header (cmp).
- IS_THING_ALIVE COMDATs: 0x008D7630 / 0x008DCB60 emit IS_THING_ALIVE<CIntelligentPointer<CThing>>, <CIntelligentPointer<CThingCreatureBase>>,
  <const CThing*>, <CThingAICreature*> from engine/ThingAlive.h (inline). The landed owners 0x004CC340 / 0x006961F0 use explicit instantiations of
  the same header template, and other landed includers (0x007751A0, 0x00859660, 0x008547D0 ...) already emit the same COMDATs: no new pattern.
- 0x008DC900: the mode class key restored to `class` (Ego_r / FableWin V, as HEAD) after the RELOCATION_MATCH grade; the key does not enter the
  own leaf; regraded in recheck_final.log.

## q6453 / q6455 (0x0073A680 CTCCreatureModeManager::CModeBase::GetWorld): the last five REFS rows ported
- Claim: ?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ (Ego_r 0x00751030 / FableWin 0x024C31E0, NF126); the self row 0x0073A680
  already defines it (NF128). No self rename, so every port is independent.
- Ported (each RELOCATION_MATCH + test PASS; per-row form counts in STATUS.tsv):
  - 0x008E7CF0 CModeBriarRoseTeleportIntangible::OnDeactivate (1 form) and 0x008E7B90 ::OnActivate (1 form): off the Native / quarantine-CThing
    stand-ins onto CModeStandBase.h (DEACTIVATE / ACTIVATE) and the shared headers. 0x008E7790 is GetSubDef<CBriarRoseDef> (its landed self leaf
    and the "CBriarRoseDef" literal), so HEAD's CBriarRoseTeleportDef fields Duration / ThemeChangeCount / ThemeScale are CBriarRoseDef
    SecondsToBeIntangible +0x2c / EnvironmentThemeForIntangibleMode +0x3c / SecondsForIntangibleEnvironmentThemeTransitionIn +0x40, and
    HEAD's slot-28 "StopMoving" is CThing::SetAsDrawable (slot 28, CThing.h ORIGINAL_VTABLE); the OnDeactivate twin passes true.
  - 0x008D7630 CModeIceTrollGroundSmashIcicleAssault::OnActivate (2 forms): needs the new NF135 opt-in FABLE_RETAIL_CSNOWTROLLDEF
    (hdr/README.md); NCreatureHiding::GetValidPositions now binds its PDB spelling (already bound by 0x008BC1A0).
  - 0x008DCB60 CModeJackOfBladesFlash::ExtinguishFlames (1 form) + 0x008DF310 CModeJackOfBladesFlash::OnDeactivate (in-place access respell):
    a 2-row family. The self symbol becomes the PDB ?ExtinguishFlames@CModeJackOfBladesFlash@NCreatureMode@@AAEXXZ (namespace + private, both
    PDBs); the only exe referencer 0x008DF310 bound ...@NCreatureMode@@QAEXXZ and now binds the AAE spelling (moved its row-local declaration
    under `private:`). Before: two spellings for 0x008DCB60; after: one. Hand back both or neither.
  - 0x008DC900 CModeDragonStrafeBurninate::UpdateBeam (the "dozen local models" row; 2 source forms, 4 compiles): matched with the landed
    sibling 0x008DC6C0's mode shapes. HEAD's GetDisplayEngine()->GetWorldA/B stand-ins are CWorld slot 4 PeekThingManager / slot 3
    GetThingManager followed by the inline CThingManager PeekWorldMap / GetWorldMap (+0x28; FableWin names, CThingManager.h WORLDMAP);
    the def is read in place (CThing::PeekBaseDef, no AddRef) and VC7.1 drops the null test of the CDefPointer release after the
    FramesBetweenBurninateUpdate read, as retail.
- Census (tools/bindchk.sh on the other 13 REFS rows from the tree, work/bindchk_q6453.txt): see APPLIED.
- Self symbol changes (RENAMES s1, 5 rows; every new leaf in both PDBs; exe scan via tools/xr.sh): 0x008E7CF0, 0x008E7B90, 0x008D7630 and
  0x008DC900 have no landed referencer (virtuals / private); 0x008DCB60's one referencer 0x008DF310 is in this hand-back. 0x008E7CF0 /
  0x008E7B90 / 0x008D7630 move from stand-in classes (ModeBriar / ModeIceTroll) or QAE to the PDB class + UAE; the catalog names are unchanged
  except 0x008DCB60 (module gains NCreatureMode::; build entry Module / file name may follow).

## Callee spellings (coll.py on every changed pair; RENAMES section 2)
- Every new pair is a spelling already bound to that address or the callee's self leaf, except one: 0x008E3BB0 (0x008DCB60's GetAllThings)
  now binds the PDB instance ??$GetAllThings@VCThingAICreature@@V?$vector@PAVCThingAICreature@@...@VCThingFilter_IsNot@@@CThingSearchTools@@QBEX...
  (Ego_r 0x00825CE0 / FableWin 0x0286CD30), replacing HEAD's row-only stand-in; the address keeps two spellings (the owner row's own stand-in
  self leaf + this), so the count does not grow (accepted rule: a PDB spelling replacing a stand-in). NAME_FIXES has the owner fix.
- Collateral PDB respellings already bound elsewhere: SuspendAI (ThingNative -> CThingAICreature), CModeStandBase::OnActivate / OnDeactivate
  (stand-ins -> UAE self leaves), the three GetSubDef<T> instances, GetPerceivedThing (CTCPerception -> CTCPerceiveThing QBE),
  TransitionToDefaultThemeAllChannels (EnvironmentMethods -> CEnvironment), GetValidPositions, GetDummyObject, GetHeightZBelowPos, CTCDExplosion::
  Create / SetThingAsImmune.

## One definition (for the coordinator)
- 0x008E7CF0 / 0x008E7B90 / 0x008D7630 / 0x008DC900 include engine/CDefClassBase_GetSubDef.h, so their objects carry the GetSubDef<T> header body
  as a pick-any COMDAT, while the owner rows 0x008E7790 / 0x008C7A70 / 0x008E3300 define those instances as strong explicit specializations
  (`template <>`). This is the pattern the landed 0x00956E70 (GetSubDef<CBriarRoseDef>) and 0x008DB810 (GetSubDef<CJackDragonDef>) already
  have; not linked here. Declaring the explicit specialization in the row instead of including the body (0x008E7CF0 form 2) hit C1001 (ICE).
  If the coordinator wants one definition, the owners take the nf86 / nf88 scaffolding.
- No other new external definition: objdump of each object lists only header inlines / COMDATs and the row's own leaf (vtables are not emitted:
  no row-local class has a constructor in its row).

## Local class models (rule check)
- Row-local mode classes, each with no shared header anywhere under rebuild/include (grep `class|struct <name>`), each over a shared base, each
  matching a landed sibling's shape: NCreatureMode::CModeBriarRoseTeleportIntangible (0x008E7CF0, 0x008E7B90; siblings 0x008E8940 / 0x008C3700),
  NCreatureMode::CModeIceTrollGroundSmashIcicleAssault (0x008D7630; ctor rows 0x00743BE0 / 0x00743FE0 / 0x008C7C00) and its nested CIcicleData
  (declared only), NCreatureMode::CModeJackOfBladesFlash (0x008DCB60; HEAD already defined the class token, moved into NCreatureMode as the PDB /
  RTTI; sibling 0x008DF310), NCreatureMode::CModeDragonFlyingBase / CModeDragonFlying / CModeDragonStrafeBurninate (0x008DC900; HEAD defined
  CModeDragonStrafeBurninate; the three shapes are the landed sibling 0x008DC6C0's).
- Every class with a shared header that these rows modelled locally at HEAD now comes from its header (selfcheck: 0 NEWLOCAL).

## Descriptive names
None new. HEAD's descriptive CThingFilter_NearCreature / CreateExplosionObject / GetDisplayEngine / GetWorldA / GetWorldB / CTCDependents /
AddDependentThing / CDragonDef / GetDragonDef / GetBriar / GetSnowTroll / StopMoving stand-ins are gone (PDB names).

## Flags lines
None added; every changed row grades with its build entry's CompilerFlags (none).

## Funclets
No row under rebuild/src/compiled/01/21 includes any changed file (grep of each file name).

## Tests
All landed tests unchanged (standalone models); vland PASS.

## Rule slips
- One early Bash call exited 1 (a trailing `grep -c` that counted 0 in the source-location loop).
- docs/pipeline/GOTCHAS.md (134 KB) and ASM_RULES.md were not read in full; EXTRA_RULES.md was read in full (first pass with long lines cut at
  300 characters). GOTCHAS was grepped for the one-definition lines (nf80 / nf86 / nf88 / nf96a).
- The land1119 batch READMEs were not read: none of my five entries carries a land1119 tag.
- One owner-leaf probe of 0x008E3BB0 ran without sourcing env.sh (q.py could not find cl.exe; no compile happened) and was rerun once.
- 0x008DC900: three failed compiles before the match (two SRC_FAIL of the same text for missing prerequisite defines, one DIFFER), i.e. the
  match came on the fourth compile; counted in STATUS.

## q6408 (0x0073ACC0 CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>)): census only
- tools/bindchk.sh compiled all 34 REFS rows from the tree (work/bindchk_q6408.txt; wrong list work/q6408_wrong.txt): 9 bind the claim
  ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z, 25 do not (16 `CCountedPointer@UCModeBase@@`,
  5 `UCCountedModePointer@@`, one each `UCCountedPointer_CModeBase`, `CCountedPointer@UCModeBase@NCreatureMode`, `CTCCreatureModeManager_Retail`,
  `CreatureModeManagerMethods`). The self row still defines ?AddMode@CTCCreatureModeManager@@QAE_NUCModePointer@@@Z.
- No cheap group: the wrong rows model CTCCreatureModeManager / CModeBase / CCountedPointer / CCombatActionBase locally, and the mode they add is
  built through a ctor whose spelling also changes on a port (e.g. 0x008C1E80 binds ??0CModeBriarRoseTeleportInvulnerable@@QAE@PAVCThing@@PAV...
  while both PDBs have ??0CModeBriarRoseTeleportInvulnerable@NCreatureMode@@QAE@AAVCThing@@AAVCThingCreatureBase@@@Z, and the self row 0x008C35D0
  carries only a partial Leaf; 0x008C0C00 also has a same-TU CCountedPointer<CModeJackOfBladesFlash> ctor stand-in for 0x008C3260). An in-place
  nesting of the local `struct CModeBase {}` would emit local CCountedPointer<CTCCreatureModeManager::CModeBase> COMDATs that clash with the shared
  header's, so it is not used. Templates for real ports: 0x0096DBC0 / 0x008C4990 (FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER + the
  shared CCountedPointer.h and CCombatActionBase.h). Not attempted (context).

## q4747 / q4802: not started (read-only notes)
- q4747 is a 2-row family (hand back both or neither): the self row 0x00733480 is the stand-in CTCCombat_Retail::GetPMeleeFlourishAbility()
  (CDefPointer_MeleeCombatAbilityDef by value); engine/CTCCombat.h (FABLE_RETAIL_CTCCOMBAT) has UsableAbilityList / CurrentAbilityIndex and
  needs a new opt-in for `CDefPointer<const CMeleeCombatAbilityDef> GetPMeleeFlourishAbility() const` (Ego_r 0x0074B600 / FableWin 0x025005A0).
  The binder 0x0064BE45 (373 bytes, `#pragma optimize("y", off)` + `("s", on)`) models CTCCombat / CDefPointer / CAbilityDef / CThing locally;
  besides 0x00733480 it calls 0x007334B0, which is Ego_r ?GetPMeleeBlockCounterAbility@CTCCombat@@QBE?AV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@XZ
  (Ego_r order GetPMeleeFlourishAbility 0x0074B600 / GetPMeleeBlockCounterAbility 0x0074B630 / GetPMeleeFlourish360Ability 0x0074B650; the body
  returns PBlockCounterAbility +0x48 with AddRef), bound today only as the binder's stand-in GetPBaseAbility (catalog: CThing::GetPBaseDef), and
  0x00733290 (`xor al,al ; ret`), bound as IsShowingBaseAbility QAE by the binder and IsBreakBlockValid QBE by 0x0068ECA0 (an existing split;
  unresolved, one body that returns false).
- q4802: 0x00764E90 and 0x00BCD950 are byte-identical SGI __copy bodies over 12-byte elements (the element operator= is 0x0075B8D0 =
  vector<C3DVector>::operator=). Their own leaves are free stand-ins with CElement12 parameters, so binding the claim changes each self leaf;
  the two must get distinct STLport template arguments proven by their callers (e.g. const vs non-const source iterator), or stay blocked.

## Decisions for the coordinator
- New overlay opt-in FABLE_RETAIL_CSNOWTROLLDEF: a class-level retail branch (CSnowTrollDef : CSubDefClassBase) in engine/CSnowTrollDef.h,
  modelled on CBriarRoseDef.h's FABLE_RETAIL_BRIAR_ROSE branch (hdr/README.md). Needed by 0x008D7630.
- GetSubDef<T> one-definition pattern (rows 0x008E7CF0 / 0x008E7B90 / 0x008D7630 / 0x008DC900, as the landed 0x00956E70 / 0x008DB810): header body
  COMDATs vs the owners' strong explicit specializations 0x008E7790 / 0x008C7A70 / 0x008E3300.
- 0x008DCB60's catalog name gains NCreatureMode:: (RENAMES s1); its build entry Module / file name may follow. Land 0x008DCB60 with 0x008DF310.
- q4747 is a closable 2-row family; it may be worth taking ahead of q6408 / q4802 next round.
- q6408 needs ~25 full ports (each with a mode-ctor callee respell); consider splitting it by the ctor callee families.

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries (5): 2 done (q6453, q6455), 0 done-already, 0 rejected, 0 blocked, 0 deferred, 3 open (q6408: census only; q4802 and q4747: not started).
- Rows: 6 changed (5 ported: 0x008E7CF0, 0x008E7B90, 0x008D7630, 0x008DCB60, 0x008DC900; 1 respelled in place: 0x008DF310, the 0x008DCB60
  family), 3 not-a-binder (0x0073A680, 0x008EACB0, 0x008C1900), 0 blocked, the rest of rows.txt not reached (STATUS `none`; the q6408 ones carry
  their census spelling). recheck_final.log: 6 RELOCATION_MATCH, 6 PASS, 0 HARD / RELOC_CONFLICT (3 WARN SHARED_HEADER_AVAILABLE on 0x008DF310's
  HEAD local models, unchanged). selfcheck.log: 0 problems.
- Header opt-ins: 1 (FABLE_RETAIL_CSNOWTROLLDEF, engine/CSnowTrollDef.h). Locks: 0x008DF310 claimed (free); every other changed row is in rows.txt.
