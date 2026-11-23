# NF303 notes (2026-11-23): T1 "other" made-up names, continuation of NF292 / NF296 / NF299 (hand-back 1)

## Summary
- **53 rows changed**: 40 landing-ready (`rows.txt`) and 13 that wait on decision Q2 (`rows_conditional.txt`). All 53 graded
  RELOCATION_MATCH PASS in vland `--recheck-landed` (`recheck_final.log`, payload work/rc1: 53 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT,
  0 LINK_FAIL / TCC_FAIL). The warnings are pre-existing local models or callee labels. One is expected: 0x007A1870 RELOC_NAME DeleteData -> catalog
  `DeleteObject_007a4120` (that self row is a stand-in, see NAME_FIXES).
- No header, flag, catalog or test change. hdr/ is empty. Every test of a changed row is a standalone copy that does not link the row, so the tests are unchanged.
- **Entries classified: all 802** (STATUS.tsv has one line per ENTRIES address, plus the prefix item 0x008E9710; APPLIED.tsv repeats it
  with entry numbers). 683 are classified by the census only, 77 are untouched.
  - Worked by hand (43 lines):
    - done 4 (0x008574D0, 0x00D89AD0, 0x00A441E0, and the prefix item 0x008E9710);
    - done-partial 7;
    - done-conditional 9 plus done-partial-conditional 3 (decision Q2);
    - skipped-held 8 (stay-off: NF300 / NF302 entries and witnesses, see Q4);
    - skipped for decision Q1: 4 (VIRT entries 13 / 16 / 17 / 18);
    - skipped-port after reading the witness object: 7 (81 / 140 / 173 / 543, the three sort_heap entries; 265 read from source);
    - STL fold 1 (730).
  - The rest come from the Q4 census (tools/census303.py, evidence/census.tsv):
    - 682 skipped-port (census);
    - 1 with no stand-in table line;
    - **77 `open`**: in-place-shaped SAME-scope SIGEQ / NEAR lines plus entry 797, not reached; next batch.
  - Sample: the 1-in-10 census sample plus every HDRFLAG entry (95 entries in all) were read through the stand-in's declaring lines in the witness
    (tools/sample303.py, evidence/census_sample.txt), not whole files. Every one shows a made-up scope, a free stand-in or another class.
    Entries 84 and 306 are flagged as identity questions.
- Table: TABLE_CHANGES.tsv has 31 action lines (21 of them CONDITIONAL on Q2):
  - 18 deletes. 11 are CONDITIONAL: 10 GetSubDef lines and the GetPFirstThing<HasDefGlobalIndex> line. The 7 unconditional ones are SetName,
    DestroyObject, PlayCutScene, CCombatActionBase(long) QAE, CSmallVector Clear QAE, AngleDifference and Compare.
  - 11 adds, for targets left with no PDB line. 9 are CONDITIONAL (8 GetSubDef<T>, GetPFirstThing<HasDefGlobalIndex>); the unconditional
    ones are PlayCutscene and OnAnimationLoaded MAE.
  - 1 replace (ClearStrafingCombinationAnims QAE -> AAE).
  - 1 conditional rewitness (GetPFirstThing<HasScriptName> -> 0x00963640).
  - Checked by applying the changes in memory: every `done` target keeps only its PDB spelling, and each target touched has a PDB line.
- RENAMES.tsv: section 1 has 2 self leaf changes (0x008E9710, 0x009D2110); section 2 has 51 caller respellings.
- NAME_FIXES.tsv: 10 self rows, not changed: 0x007A4120, 0x008E9850, 0x00857E40, and the seven combat-action constructors of entry 1.

## Units (evidence/proof_<u>.txt, evidence/link_<u>.txt)
- **u1** (entries 1, 2, 4, 7):
  - SetName -> SetCombatAnimation (8 ctors).
  - DestroyObject -> DeleteData. This respells the member of 0x007A1870's own local `CCountedPointer` template model in place; it is not
    the NF282 hybrid (shared-header) form.
  - Init -> protected Initialise (3 rows). 0x00A6C170 spells the call `CInitBaseClass::Initialise()`, because CSystemFont declares its own
    Initialise and the unqualified name would be hidden.
  - PlayCutScene -> PlayCutscene.
- **u2** (entries 14, 15): access only. `protected:` on two public-virtual CCombatActionBase::FinishAction declarations and one
  OnAnimationLoaded declaration (MAE). No `virtual` was added. 0x008CF7D0's emitted ??_7CCreatureAction_TentacleStrikeOutOf table is byte- and
  relocation-equal under the rename.
- **u3** (prefix item): 0x008E9710 ClearStrafingCombinationAnims is private (AAE).
  - The owner declaration is wrapped and its bare Leaf is written in full (RENAMES section 1).
  - Its sole caller 0x008E9850 is wrapped the same way.
  - xsites finds one call site; absrefs finds none.
- **u6** (entries 629-667, conditional): GetSubDef non-template -> the PDB member template `template <class T> bool GetSubDef(CDefPointer<const T>*) const`
  (11 binders; const added where the binder had none). See Q2.
- **u7 / u7b / u7s** (entries 94, 364):
  - `protected:` on the CCombatActionBase(long) ctor (7 binders).
  - `protected:` on CSmallVector<CCharString,8>::Clear (2 binders + the owner self 0x009D2110: wrap and full Leaf).
- **u7 + u9** (entry 801): `__stdcall AngleDifference` -> `__fastcall GFGetSignedAngleDifference` in 11 binders. Two float arguments mean
  nothing goes in registers and the callee pops 8 either way, so objsec is ALL_EQ.
- **u10** (entries 82, 607, 608):
  - Compare -> operator!= in the stand-in row 0x006BBF20.
  - GetPFirstThing non-template -> member template (conditional, Q2) in 0x00921630 / 0x00910100.

## Proof
- **objsec** (tools/proof2.py, explicit old -> new pairs read from the HEAD / new objects; every new symbol is the ENTRIES pdb_spelling):
  - ALL_EQ for the 40 landing-ready rows. Table counts are unchanged and nothing is UNMAPPED.
  - For the 13 conditional rows, every section is equal except `.drectve`. See Q2.
- **Links:**
  - Every made-up spelling HEAD left unresolved is resolved by the owner's HEAD object, or stays unresolved for the same reason as at HEAD, which
    is explained in the line:
    - SetCombatAnimation: owner 0x008574D0 is an NF300 witness, not compiled; its source defines the name.
    - DeleteData: the self row is a stand-in.
    - OnAnimationLoaded MAE: the owner test needs D1.
    - CCombatActionBase(long): the owner defines another ctor (NAME_FIXES).
  - Nothing HEAD resolved becomes unresolved. 0x009D2110's self change keeps Clear resolved.
  - 0 duplicate lines in any link.
- **Copy grade** (tools/grc.sh): the trial `__sort_heap` -> `sort_heap` respell of entries 734-736 made the binder's same-TU body a pick-any copy of
  the owner's symbol. That copy grades DIFFER(166v179) against the owner, so the three entries are skipped-port and their rows were removed (clean.py).
- **Every table delete** was checked against the full list of exe callers:
  - work/xs_all.tsv for every rel32 call / jmp;
  - tools/absrefs.py for every address operand;
  - a source-wide declaration scan (tools/srcwide.py) for the two virtual targets.
  - That late check withdrew the two UAE deletes (entries 14 / 15): 11 + 4 other landed rows declare those virtuals public, and the exe has
    170 / 300+ vtable references to them.
  - An earlier census read had been cut short by `head`, which hid three CCombatActionBase(long) binders. They were found again and respelled (u7b).
- **Name census** (evidence/name_census.txt plus hand checks for template / operator names): no changed row defines any new name.
  The pre-existing same-TU copies are listed.

## Decisions / questions
- **Q1 (VIRT entries 13, 16, 17, 18).**
  - The QAE witnesses (0x005469E0, 0x00547C90, 0x00557C10, 0x0054A990, 0x0078B410, 0x008D87C0, 0x008E7A00, 0x0082D810, 0x00836260)
    declare the base method plain AND their own override plain.
  - Ruling Q3's `virtual` on the base would make each row's own function virtual, so its self leaf flips QAE -> UAE. UAE is the PDB spelling in
    both sets (e.g. ?ChangeState@CZoomingComponent@NUISystem@@UAEXK@Z, ?WorldUpdate@CThingBuilding@@UAE_NXZ).
  - That is a self rename, which Q3 does not cover. Allowed (with RENAMES section 1 per row), or T3?
  - Entry 14's 13 FinishAction binders are the same case (listed in STATUS).
- **Q2 (.drectve on declared-only member templates).**
  - Moving a binder from a non-template declaration to the PDB member template makes VC7.1 add
    `/alternatename:<template instance>=<old non-template name>` to `.drectve`. All other sections are equal.
  - Precedent: landed 0x00894490's HEAD object already carries the same directive for GetPFirstThing (evidence/drectve_precedent_00894490.txt: HEAD object of the landed row, compiled under its lock).
  - The directive is a link-time fallback only. The old name is defined nowhere, so the owner's instance is what resolves (link_u6 / link_u10:
    all resolved, 0 duplicates).
  - Do these 13 rows land with that `.drectve` difference, or must objsec be ALL_EQ here too (then they are T3: they need the shared
    CDefClassBase_GetSubDef.h / CThingSearchTools.h template route)?
- **Q4 (held again).** The prefix calls lines 1-12 open. Seven of them (entries 3, 5, 8, 9, 10, 11, 12) are held again: the sole stand-in
  binder (or the target) is an NF300 / NF302 ENTRIES target or spelling witness. Wait for those batches, or may NF303 take them when their locks are free?
- **Q3.** The 77 `open` entries are in-place-shaped:
  - SIGEQ: virtual dtors 155 / 395 / 569 (vtable risk); UpdateScan template 700, whose binder identity NF299 doubted.
  - NEAR: pointer <-> reference, builtin parameter types.
  - Most NEAR lines are signature work. Should they go to the T1-signature batch, not "other"?

## Rule notes
- **Locks and prechecks:**
  - Locks held now are exactly the 53 changed rows; owner / witness locks were released after use.
  - Tests were prechecked (clang) before each lock.
  - Every compiled source had its own precheck in an earlier call. The work/pre_*.log files hold the planted control (pre.sh now also
    reports a nonzero clang exit with no error line).
- **Background runs:**
  - Every grade, link and vland ran in the background with `timeout 900` on its own launch line.
  - Completion was read with kill -0 / log tails in later calls.
- **Never done:** sleep, rm, git, writes under the repo, __asm / naked / _emit.
- **Rule slips:**
  - Locks on 0x00859EF0 (u2 owner) and 0x00D89AD0 (u1 owner) were claimed after their landed tests had failed the clang precheck (both only
    on VC7.1 / shared-header asserts). 0x00859EF0 was released uncompiled; 0x00D89AD0's object was compiled (source precheck PASS; the test is
    not compiled by a grade).
  - 0x00EF7800 was locked after its test passed and released uncompiled when its source precheck failed.
- **Compiles skipped on precheck:**
  - 0x00EF7800's precheck failed in its own code (not toolchain), so it was not compiled.
  - 0x00859EF0's source fails clang only on a shared FABLE_STATIC_ASSERT typedef (D1 would allow cl /Zs). It was not compiled; its MAE Leaf
    was read from source.
- **Stay-off** (evidence/stayoff.txt): NF300 / NF302 ENTRIES targets and spelling witnesses, the standing held rows, Codex p28 still-reserved
  rows; the T3 queue was checked per row. No lock conflict was met.
