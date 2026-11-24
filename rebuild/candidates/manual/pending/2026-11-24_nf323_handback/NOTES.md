# NF323 (2026-11-24): the CCreatureAction Clone family, the FableWin-only declarations and the rest (53 ENTRIES + 5 prefix rows)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Scope note: the prefix names six FableWin-only rows,
but ENTRIES.tsv holds only 0x00B4B6B0. The other five (0x00A7D950, 0x00A7D960, 0x00A84E20, 0x00AB7910, 0x00AB7920) are nf320 lines of
T3_PORT_QUEUE (released to me by the prefix), so I worked them as well: STATUS.tsv has 58 lines (Q5).

## Result (STATUS.tsv, 58 lines)
- **27 done**, ready to land (rows.txt, rows/<addr>.cpp; no test changed: every landed test is a standalone stub and passed unchanged):
  - **21 Clone rows** (of 23): 0x0084C6F0 0x0084C7D0 0x009115A0 0x009115D0 0x00916760 (existing class models, no header change);
    0x0084C6C0 0x00941CC0 0x008683D0 (add-only `_RTTI_DECLS` overrides; 0x0084C6C0 is NF320's pilot); 0x0062F230 0x0062F260 0x0084C460
    0x0084C7A0 0x00855200 (new `_VIRTUAL_MODEL` opt-in in the flat GENERATED header, `#ifdef` new / `#else` old text); 0x007EBB30
    0x007ED140 0x0084C010 0x0084CA30 0x0084CA60 0x00933D50 (NEW header files: no class model existed); 0x008E6F50 (81-slot table on
    the retail CCombatActionBase, new `_VIRTUAL_MODEL` opt-in); 0x00860290 (81-slot table; opt-in gives Clone its PDB access, see below).
  - **6 add-only declaration rows**: 0x004DEA03 (CFireballSpellLevelDef::Transfer, both PDBs), 0x00AB7910 / 0x00AB7920 (CJoystickDX
    rumble getters), 0x00A7D950 / 0x00A7D960 (CNavBlockedQuadTreeNode, protected), 0x00B4B6B0 (CEnginePrimitiveManager2DParticleGroup::
    RenderPrimitive); the last five are RTTI + FableWin.pdb (NF320 Q3).
- **11 skipped-held**, **20 skipped-port**: one reason per line in STATUS.tsv. Summary:
  - neighbour classes (NF321 / NF322 active): 0x008800C0 (StabbedToDeath, NF322), 0x006CE650 (CGroundNavigatorExternalInterface, NF322),
    0x00A5F9D0 (CStreamingFontDataBank, NF322), 0x00A84E20 (CNavSwitchableLeafNode, NF321 + NF322), 0x00BCEA40 (also NF320 Q2);
  - settled by rulings: 0x005DBDA0 (NF318 Q5), 0x00452B14 (NF319 Q3), 0x0058509F (NF281 D2), 0x004DE916 / 0x006FEED0 / 0x00933300
    (NF318 Q2: the declaration exists only under a switch no landed row defines; the landed default branch is flat);
  - 0x009680D0 PickUpJug Clone: held, Q2;
  - base class-model ports: 0x0043368B, 0x0068CB70, 0x00690D20, 0x00905320 (the base model lacks the virtual);
  - body / signature ports: 0x00453ED2 (stand-in callee CMemberDef::Copy), 0x00904CD0, 0x00A24410, 0x00B98DE0, 0x00BC2460;
  - the 10 CScriptThing members (0x004AA8D0 ... 0x004AB150): one nf86 / nf88 pick-any unit with the landed in-TU NScript copies (Q4).
- REJECTED.tsv: none. TABLE_CHANGES.tsv: none (no landed row or table line binds an old Clone / alias spelling; the Clone addresses are
  referenced only by their vftables, exe census evidence/vt_emitters_*.tsv). NAME_FIXES.tsv: 62 lines (see "Queue").
- Order of work (NF285 E2 / NF319 Q1): every entry triaged by script first (tools/clonetri.py, tools/ovlist.py, queue reasons), then
  the feasible groups: Clone A (header complete) -> B (add-only overrides) -> C (new models) -> the add-only declaration rows ->
  the two CCombatActionBase Clones -> one final regrade of all 27 on frozen trees.

## Clone family: method and proof (tools/cloneproof.py, the NF320 Q1 rule as a script)
- Draft (tools/mkclone.py): the HEAD row's leading comment block kept, the local one-slot stand-in models dropped, the full PDB Leaf,
  a provenance note, the class switches (FABLE_RETAIL_BASE_OBJECT, _CCREATUREACTIONBASE, _CCREATUREACTIONBASE_COPYCTOR, + the class
  switch / new opt-in), the shared headers (base first), and `CCreatureActionBase* X::Clone() const` with HEAD's body. Every switch has a
  landed definer, or is one of my new opt-ins.
- Class models (tools/mkmodel.py, NF322 recipe): overrides = the retail vftable slots whose RTTI-port label is not a base-chain member
  (tools/ovlist.py reads Fable.exe; evidence/ovlist_*.txt), each declared with the base header's own declaration text, public (every
  label is U..). No own members except CastStrengthMultiStrikeProper's SpellLevel (+0x114, sizeof 0x118 asserted). Flat GENERATED
  headers: `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` new / `#else` the old text unchanged; moved asserts pinned (pin_asserts.py,
  12 + 3). Six classes had no header: new files, declarations only, like the landed CCreatureAction_AlertToPlayer.h.
- cloneproof.py per row, HEAD v new objects compiled by vland under ONE work path (tools/runpair.sh / runpair2.sh):
  1. the Clone COMDAT: bytes equal and relocations equal under old self -> new self;
  2. no HEAD definition lost; the only added definitions are `??_G<C>` / `??_E<C>` (allow-list; anything else would be NF320 Q2);
  3. the emitted `??_7<C>@@6B@`: slot count == retail's (70, or 81 for the CCombatActionBase pair) and every slot's symbol == the retail
     RTTI-port label of that slot's target (slot 0 compared by class: the KIND comes from check 4);
  4. the `??_G<C>` body == the retail slot-0 body (relocation fields zeroed) and its two relocations resolve to the table's symbols at
     retail's targets (~CCreatureActionBase 0x00693EF0 or ~CCombatActionBase 0x0062E900, operator delete 0x00BFE9BC);
  5. `.drectve` equal or HEAD + `/DEFAULTLIB:"libcp"` (NF318 Q1; all 21 Clone rows gain libcp from the shared headers);
  6. every other COMDAT equal.
  Result: evidence/cloneproof_fin.txt (final run), 21 OK. Control: NF320's pilot objects (header without the GetAnimExtraFlags
  override) give NOT_OK on slot 10 (evidence/cloneproof_control_nf320pilot.txt).
- Deleting destructors: per the deleting-destructor ruling the kind comes from the retail body. labels_rtti_port_dtors.tsv says `??_E`
  for 10 of these classes (AlertToPlayer, PutGoodsDown, StopSneaking, ...), but each retail slot-0 body is byte-equal to the scalar
  `??_G` VC7.1 emits, so the spelling is `??_G` (NAME_FIXES.tsv). The landed slot-0 rows define free stand-ins, so no landed strong
  `??_G` collides with the new pick-any one (evidence/namecensus.txt).
- 0x00860290 GetUpFromSitting: engine/CCreatureAction_GetUpFromSitting.h declared Clone `protected` (MBE), while Ego_r.pdb, FableWin.pdb and
  the RTTI label all spell UBE. The opt-in FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_RTTI_DECLS declares Clone public plus the five
  missing overrides; the `#else` keeps the old `protected:` line unchanged (NF309 Q2 form). This is the only edit that moves an existing
  header line into an `#else`.
- The CCombatActionBase retail branch is selected by FABLE_RETAIL_KRAKEN_ROAR (only CCombatActionBase.h reads it; landed definer
  0x00924B90). Its model gives exactly retail's 81 slots for both classes (cloneproof check 3).

## Add-only declaration rows (6)
- tools/hdredit.py (NF319's, retargeted): the block goes just before the class's closing `};`, declarations only, access restored.
  CJoystickDX's block sits after NF319's `_RTTI_DECLS` block (its pins kept); CFireballSpellLevelDef's after NF319's Copy block.
- Override probe (tools/probe.py, work/probe.log): the row's preamble WITHOUT the new opt-in + `struct P : X { <decl> override; }`
  passes clang for all six; the made-up-name control fails for all six. So each declaration overrides a base virtual (no new slot).
- Proof: vland HEAD and new under one work path, final run all MATCH / RELOCATION_MATCH PASS; tools/libcp.py (evidence/libcp_fin_addonly.txt):
  exactly one DEF rename old -> pdb_spelling, UND / DEF sets equal, no ??_7 / ??_R, every non-debug section equal, .drectve equal (3) or
  HEAD + libcp (3: the nav and particle-group rows). So the COMDAT sets are the same.

## Headers (hdr/engine/, hdr/<X>.diff, orig/hdr/engine/ = repo copies at the time of the edit)
- 20 headers: 14 edited (each checked identical to the CURRENT repo copy just before the final run) + 6 new files.
- Opt-in macros: FABLE_RETAIL_CCREATUREACTION_{DROPGENERICBOX,PLAYHOPSCOTCH,BEDAZED,GETUPFROMSITTING}_RTTI_DECLS;
  FABLE_RETAIL_CCREATUREACTION_{STARTSNEAKING,STOPSNEAKING,PLAYANIMATIONFROMINDEX,YELLOVERHERE,SPIRITRETURNTOHERO,CASTSTRENGTHMULTISTRIKEPROPER}_VIRTUAL_MODEL;
  FABLE_RETAIL_CFIREBALLSPELLLEVELDEF_TRANSFER_DECL, FABLE_RETAIL_CJOYSTICKDX_RUMBLE_DECLS, FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE_RTTI_DECLS,
  FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_RENDERPRIMITIVE_DECL. No landed row defines any of them.
- Asserts: pin_asserts.py pinned 3 (DropGenericBox) + 2 (PlayHopscotch) + 12 (the 5 flat-wrapped headers) + 3 (CastStrength) +
  1 (GetUpFromSitting); the only new asserts sit inside the new opt-ins.
- equiv_check (tools/equiv3.py, NF319's, S pointed into work/): frozen copy of the repo include tree (work/tree_old) v the new tree
  (work/tree = tree_old + hdr/engine) v a control tree (tree + `typedef int nf323_planted_<Header>;` appended to each of the 14 edited
  headers) over every landed row source and test: hdr/equiv_check.tsv (DIFF lines must be 0; CTL lines = the includers the planted
  control flags) - see "equiv_check result" below.
- The repo's include tree changed during this pass (a landing at 00:43 touched ~45 other headers). None of my 14 headers changed;
  the final grade, link and equiv_check all ran on trees rebuilt from the repo after that landing.

### equiv_check result (hdr/equiv_check.tsv, hdr/equiv_summary.txt)
- `#DONE rows 57146` (every catalog row with a source), **0 DIFF, 0 ERROR** between work/tree_old and work/tree.
- Planted control (same run): every edited header with landed includers is flagged in all of them: DropGenericBox 5, GetUpFromSitting 3
  (0x0085FF10 / 0x00860150 / 0x008A8C60: their text is unchanged, so they keep the MBE binding noted in Q3), SpiritReturnToHero 2,
  BeDazed 1, PlayHopscotch 1, CEnginePrimitiveManager2DParticleGroup 4, CFireballSpellLevelDef 4, CJoystickDX 12, CNavBlockedQuadTreeNode 19
  (transitive includers too); the counts match a grep of direct includers (CJoystickDX 12 of 13 grep hits: one hit is a comment).
- Five edited headers (CastStrengthMultiStrikeProper, PlayAnimationFromIndex, StartSneaking, StopSneaking, YellOverHere) and the six new
  files have NO landed includer (grep of rebuild/src, rebuild/tests, rebuild/include: 0), so the control cannot flag anything there and
  no landed row can change.
- After the run the repo moved again (NF321's landing: CBankFile, CEngineCamera, CWorldMap, CNavNavigableLeafNode, ... 6 headers). None is
  one of my 20, and none is in the include closure of any of my 27 rows (cpp -M over rows/, work/drift2.txt); my orig/ sources, tests
  and orig/hdr copies still equal the repo (checked after the run).

## Binders and link (tools/census_addr.py, tools/link_clone.sh + tools/linkdiff.py)
- Exe census of all 27 targets and all 21 vftables (evidence/census_all.tsv) + reloc_bindings column-2 grep (evidence/rb_targets.txt):
  no landed code references any of the 27 targets; the Clone addresses are reached only through their vftables. The vftables are
  stored by the landed constructor / caller rows listed in census_all.tsv. TABLE_CHANGES: none (every table line on these targets
  is a `??_7` line whose spelling does not change; the 0x01276C2C line naming ??_7CCreatureAction_StandUp is pre-existing, NAME_FIXES).
- Link: /FORCE link of the 27 HEAD objects + 21 control objects v the 27 new objects + the same controls (evidence/link_fin2.txt,
  evidence/linkdiff_fin2.txt). Controls: the 14 constructor rows storing the 0xA8 vftables (work/recheck_ctl.log, 14 RELOCATION_MATCH
  PASS) and 7 rows storing the GetUpFromSitting / CastStrengthMultiStrikeProper / StandUp vftables (0x0085FF10, 0x00860150, 0x008A8C60,
  0x00938A10, 0x0093AFC0, 0x008E6BB0, 0x008E6C90; evidence/recheck_ctl2.log, 7 RELOCATION_MATCH PASS). Every control lock is released.
  - duplicates: 1 in HEAD and the same 1 in new (?GetDef@CGameDefinitionManager@@... between controls 0x008E6BB0 / 0x008E6C90:
    pre-existing, not mine).
  - resolved by the new set: ?Clone@CCreatureAction_PutGoodsDown@@UBE.. (binder: the 0x00915810 constructor's vftable),
    ?Clone@CCreatureAction_GetUpFromSitting@@UBE.. (binder: the 0x00938A10 vftable) and the 21 stand-in `?v00@..` symbols.
  - still unresolved in both: ?Clone@CCreatureAction_GetUpFromSitting@@MBE.. (2 binders, 0x0085FF10 / 0x00860150: their default header
    declares Clone protected; NAME_FIXES + Q3). Pre-existing split, narrowed (HEAD: QBE self, MBE x2, UBE x1; new: UBE self, MBE x2, UBE x1).
  - newly unresolved: only vftable slot targets that the new tables reference for the first time (owner rows outside the link set);
    none of the 27 new self names is unresolved, and no HEAD definition was lost (cloneproof check 2, libcp DEF sets), so nothing HEAD
    resolved goes unresolved.
- Four further vftable storers (0x007ECFD0, 0x00842A80, 0x00842AF0, 0x00933CC0) are T3_PORT_QUEUE column-2 rows, so I did not lock or
  compile them as controls.
- New-name census (evidence/namecensus.txt): no other landed source defines any of the 27 new self names or a `??_G` of these classes;
  the slot-0 rows define free stand-ins or made-up members (0x00911200: `ScalarDeletingDestructor`), so the Clone TUs' pick-any `??_G`
  collides with nothing. New macro names: 0 hits in rebuild/src, rebuild/tests, rebuild/include (evidence/macro_census.txt).

## Queue (NAME_FIXES.tsv, evidence/)
- 21 slot-0 deleting destructors: the landed rows define free `..._scalar_deleting_destructor` stand-ins; the PDB-shaped name is `??_G<C>`
  (10 of them carry a donor `??_E` label in labels_rtti_port_dtors.tsv; the retail body is scalar). The Clone TUs now emit the real
  `??_G` pick-any; the stand-in rows can become nf86 / nf88 scaffolding or be dropped.
- 38 override owner rows the new vftables reference (evidence/ovowners_fin.tsv, the final 21-Clone link) (PerformAction, GetAnimExtraFlags, Initialise, IsPreventingNormalMovement ...):
  no `// Leaf:` line and a free stand-in / local-model definition Their spelling was not compiled here.
- Other emitters of the same `??_7` (the landed constructor rows, evidence/vt_emitters_*.tsv) still emit their own (older) tables; with
  the opt-ins landed they can move onto the same models.
- The six new headers may need engine/INDEX.tsv lines at landing.
- evidence/newly_flagged_rows.tsv: 20 landed rows keep a local model of a class that now has a shared header (the 6 new headers);
  re-port backlog (owner decision, GOTCHAS), not a blocker.
- 0x00463BDB defines ?Copy@CCombatAbilityFlourishCounterAttackDef@@UAEXPBVCDefClassBase@@@Z while RTTI names BlockUnarmedAttackDef there
  (already queued by NF320; re-listed).

## Locks
Kept on the 27 changed rows (work/mylocks.txt == rows.txt). The 21 control rows (locks claimed only to compile them) are released.

## Rule slips
- One recheck.py call ran from my folder instead of the repo (it needs the repo cwd); its vland launch failed at once and I removed its
  outputs with clean.py before relaunching.
- The equiv_check launch line has `timeout 3600` (it is not a grade; NF319's run took ~25 minutes).

## Questions
- **Q1 (new header files):** six Clone classes had no class model at all, so I added new engine/CCreatureAction_<X>.h files (declarations
  only, unguarded like the landed AlertToPlayer / OrderSecuritySweep headers). A new file changes no existing includer (equiv_check
  cannot see it at all). Is a new header acceptable under "classes with no class model need one", or should it be an opt-in-guarded file?
- **Q2 (PickUpJug 0x009680D0, held):** its base CCreatureAction_PickUpThingAtTable (engine/CCreatureAction_Tankard.h) has an implicit
  copy constructor that retail calls out of line (0x0084C9A0, FableWin `??0..@ABV0@@Z`). On that model the Clone TU would compile the
  implicit copy constructor itself (adding a COMDAT, NF320 Q2) or the model would need a copy-constructor declaration retail never
  wrote. Which is preferred: hold until the TankardsRow / PickUpThingAtTable owner port, or allow a declared copy constructor?
- **Q3 (GetUpFromSitting access):** the opt-in moves the existing `protected:` + Clone lines into `#else` and declares Clone public in the
  new branch (both PDBs say public). This is NF309 Q2's form; confirm it covers a virtual's access. Two landed rows (0x0085FF10,
  0x00860150) compile the default branch and bind the protected spelling `MBE` through their vftables (pre-existing; HEAD's self was QBE).
  The fix is to define the opt-in in those two rows too (their vftables then also gain the five overrides); I did not change them.
- **Q4 (CScriptThing, 10 rows):** the route is one nf86 / nf88 unit (the self rows become pick-any scaffolding next to the 19+ landed
  NScript in-TU copies, each copy's body checked equal). Hand it to a unit pass, or to me next round?
- **Q6 (libcp on the Clone rows):** all 21 Clone objects gain exactly `/DEFAULTLIB:"libcp"`, but their UND sets also grow by the
  vftable slot targets, so NF318 Q1's "UND sets equal" condition does not hold literally; this rests on the NF320 Q1 pilot shape (the
  pilot had the same libcp + UND growth). Please confirm.
- **Q7 (0x00B4B6B0):** the RenderPrimitive declaration went into the `_MANAGER` branch of engine/CEnginePrimitiveManager2DParticleGroup.h
  (the branch whose base declares RenderPrimitive in the same header); the `_RENDER` branch does not have it.
- **Q5 (scope):** five of the prefix's six FableWin-only rows were not in ENTRIES.tsv; I worked them from the released nf320 queue lines.
