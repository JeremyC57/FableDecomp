# NF310 notes (2026-11-23): T1 signature, NF307's continuation (ENTRIES 1-4 = NF307's held 10 / 22 / 29 / 33; 5-6 = its 38 / 39; 7.. = its 41..)

Base: the repo working tree of today (after landing nf307). No repo writes, no git.

## Reading
Read in full: my BRIEF.md prefix, the standing brief BRIEF_NF_BATCH.md (with the NF299-NF307 rulings), docs/pipeline/GOTCHAS.md (all 513
lines), NF307's hand-back (NOTES, LESSONS, STATUS, TABLE_CHANGES, NAME_FIXES, RENAMES, held/) and its landing README.

## Off-limits lists
work/offlimits.txt (brief's held rows, Codex p28 still-reserved rows, every T3_PORT_QUEUE address), work/other_agents_entries.txt and
work/other_agents_witnesses.txt (NF308 / NF309 ENTRIES targets and every witness in their spellings column), built by tools/mkoff.py
(retargeted to NF308 / NF309); per row tools/cstat.py / aflags.py, and lock.sh for every compiled row. No lock of another agent was hit.

## Result (entries 1-50, 52, 55-68, 70-78 reached: 74 of 140; 51, 53, 54, 69, 79-140 `open`, not started) + the 0x00713EE0 rider
- **74 entries classified:** 18 done, 4 done-partial (5, 6, 12, 28), 33 skipped-port, 19 skipped-held. Order: 1-4, the rider, 5-6, then
  closable in-place entries first (NF285 E2): 2, 3, rider | 15, 20, 24, 29, 33, 56 | 17 + 5 / 6 | 28, 32, 36 (probe), 37, 60 | 62, 70, 71,
  75, 76, 78 | 12, 48, 70's second binder; every other reached entry classified from its source (and compiled where noted).
- **38 rows changed** (rows.txt == the 38 locks I still hold; every control lock released). One test changed (rows/004022b0_test.cpp: its
  stub of CXMVPlayer::GlobalRegisterPlayer follows the row to __fastcall, else the test link fails); no header changed (hdr/ empty).
  All 43 orig/ copies (38 rows, 1 test, 4 held variants) equal the repo at hand-back (cmp).
- **3 self renames** (RENAMES section 1): 0x0051EC50 AddToUpdateList (CThing* -> CThing&, MATCH), 0x007F5A70 IsTargetedThingOfType
  (unsigned long -> ETargetingType, RELOCATION_MATCH), 0x006A7930 CThingFilter_InMap::operator() (CThing* -> const CThing&, MATCH); each
  with a full mangled Leaf; every landed binder of the old self spelling moves in this landing (NF304 Q6) or none existed.
- **RENAMES section 2:** 38 caller lines. **TABLE_CHANGES:** 24 lines (14 delete, 10 replace; one hand delete for a stale witness).
  **NAME_FIXES:** 4 lines.
- **Rider 0x00713EE0** (NF307 D2): CCreatureNavOutput nested into its CTCCreatureNavigation model and LookAtPos by reference, exactly the
  0x00714900 edits; RELOCATION_MATCH, ALL_EQ. NF307's two rewitness lines become deletes: every census binder of 0x006CBAC0 / 0x00753E90
  was compiled and binds the PDB spelling (evidence/bind_g1_*), except 0x0097EC00 (an NF308 / NF309 entry: source read, it carries NF307's
  landed nested form, rechecked at landing nf307) and 0x00787E12 (an NF308 / NF309 witness: source read, it defines a same-TU noinline
  LookAtPos in the PDB spelling, pre-existing). The self 0x006CBAC0 has no landed row (unresolved before and after on the PDB name).
- **Proof per changed row:** HEAD + new VC7.1 objects RELOCATION_MATCH / MATCH (evidence/grades.log); objsec ALL_EQ for all 38
  (evidence/proof_all.txt over rows.txt: 38 ALL_EQ; per-group proof_g1, g2, g34, g5 (its NOT_EQ is the reverted 0x00836A50 probe), g6, g7; .drectve included, drectve_eq clean, no table emitted by any changed row); `nf.py syms` HEAD v new
  per row (evidence/syms_g*.txt): only the respelled symbols change; DEF renames only in the three selves and one pick-any getter in
  0x006A7930 (`?GetPhysicsTC@CThing@@QAE...` -> the const overload `?GetPhysicsTC@CThing@@QBEPBVCTCPhysicsBase@@XZ`, a FableWin name, its
  copy byte- and relocation-equal to 0x0050A650's: NF304 Q1).
- **Links** (evidence/link_g1, g2, g34, g4b, g5, g6, g7; /FORCE, HEAD set v new set with every compiled control): no new duplicate except
  the D3 selves (below, not in rows/); nothing HEAD resolved is unresolved in the new set; the new set resolves what HEAD left unresolved
  for ApplyBonusItemEffects, OnSpecialAbilityUsed, IsTargetedThingOfType, AddToUpdateList, CreateMinion, HasEmptyJugSlot, the CTCUnholyPower
  ctor, ReadFromDataStream, IsTimeToGoToSchool, TriggerMe, GetMaxAntialiasing, AutoSaveQuestStart, GetMapNavigationAreaInit,
  GlobalRegisterPlayer, CThingFilter_InMap. Left unresolved on the PDB name (self not compiled or not landed): GetNextNavPosOnRoute
  (no self row), OnWallMountUseAction / RenderUpdate / AddThingNotSolidTo (selves are NF308 / NF309 witnesses), PopulateRegionWithVillagers
  (no self row), Unlock (the self defines a stand-in; 0x009FB750 defines the PDB name strongly, below).
- **vland --recheck-landed** (recheck_final.log, all 38 rows from rows/ + rows/004022b0_test.cpp): 36 RELOCATION_MATCH + 2 MATCH, every
  test PASS; no RELOC_CONFLICT / NOT_GENUINE / HEADER_POLICY; the RELOC_NAME warnings name other callees' labels.
- **Table lines v the full caller list:** every target touched was censused in the exe (nf.py census / census_addr.py: calls, jumps,
  absolute dwords in landed code; evidence/census_exe.tsv) and every census row compiled as a control and read with bind.py, except rows of
  other agents or off limits (named in STATUS). No touched spelling is virtual (no UAE / UBE / MAE / EAE letter in any line), so no vtable
  references apply. Entry 12's PAV line stays (witness 0x00845CE0 off limits).
- **Name census:** grep of every touched Class::Method definition in rebuild/src/compiled: each is defined only by its self, except the
  pre-existing 0x00787E12 LookAtPos copy (NF307) and 0x009FB750, which defines `CTexture::Unlock(unsigned long)` STRONGLY under the PDB
  name (compiled control: DEF ?Unlock@CTexture@@QAEXK@Z). 0x00A3B730 (entry 33) now binds that name like the other four binders already did
  (NF307 D5): queue 0x009FB750 for the nf86 / nf88 pick-any scaffolding.
- **Evidence index:** bind_g*_head/new (what every compiled census row binds), proof_all / proof_g*, syms_g*, link_g*, ppdiff_enum (NF300 Q1),
  self_rename_census (old self spellings by NAME: table, sources, include/), comdat_census (D1 copies, the GetPhysicsTC copy, 0x009FB750's DEF),
  name_census, census_exe, pdb_namesets, pre.log / pre_zs.log, grades.log, recheck_groups.log.
- **Path includers:** no landed source or test #includes any of the 38 changed files by path.

- STATUS.tsv / APPLIED.tsv end with an `R` line for the rider (appended after tools/assemble.py, which writes ENTRIES lines only:
  re-running assemble drops it).

## Decisions / questions
- **D1 (entries 5 / 6, ruling NF307 D3):** the full unit (selves 0x00766050 / 0x007660C0 retyped to EObjectAugmentationType with full
  Leaf lines, binder 0x00766480 passing `(EObjectAugmentationType)(FLAME | LIGHTNING)` / FLAME / LIGHTNING) is RELOCATION_MATCH and ALL_EQ
  row by row, but the link creates NEW duplicates: 0x00839100 and 0x007677E0 include engine/CTCObjectAugmentations.h's
  FABLE_RETAIL_AUGMENTATION_PARTICLES branch and emit pick-any COMDATs of both PDB names (bytes equal to the selves; relocations differ only
  in GetSubDef: the selves call their same-TU stand-in CThingBaseDef::GetSubDef, the includers the header's template). A strong self beside
  them is LNK2005 (GOTCHAS 427). I landed only the binder 0x00766480 (it resolves to the includers' COMDATs; link_g4b: no new duplicate,
  nothing unresolved; NF302 Q2) and parked the two self variants in held/. Accept the binder-only partial and queue the selves as a T3 pick-any
  port (nf86 / nf88), or hold 0x00766480 too until the selves are ported?
- **D2 (entry 37):** the self 0x004C9030 itself defines the public ??0CThing@@QAE@K@Z (PDB protected IAE) and 0x005296B0 (an NF308 /
  NF309 entry) binds it; the binder halves 0x008367B0 / 0x008385B0 (held/, RELOCATION_MATCH) would leave themselves unresolved where HEAD
  resolved them. One unit (self + 0x005296B0 + both binders) once NF308 / NF309 release 0x005296B0?
- **D3 (entry 36):** 0x00836A50's ??1CThing@@UAE@XZ binding is to the implicit ~CThing it emits itself (engine/CThing.h's default branch
  declares no destructor); the existing FABLE_RETAIL_CTHING_DTOR opt-in gives the PDB MAE call but drops 23 COMDATs (objsec NOT_EQ). I read
  this as a T3 port (the default branch of CThing.h lacks the protected virtual destructor), not an in-place fix. Agree?
- **D4 (entry 60):** `friend class CCreatureAction_UseSwitch;` in the binder's local CThingPhysicalSwitch model (TriggerMe is protected in
  both PDBs and its only retail caller is that real class): landed under NF305 Q5. Flagging it only because the friend is not visible in any
  PDB record; the access retail forces is the evidence.
- **T3 / queue suggestions:** the two D3 selves (nf86 / nf88 pick-any form); 0x009FB750's strong CTexture::Unlock (nf86 / nf88); the 0x009F9DE0
  self (stand-in CTexture_Methods); the entry 37 unit; NF307's T3 lines for 0x00766050 / 0x007660C0 stay (the binder half is done here).

## Method
- Groups: g1 = 2, 3, rider; g2 = 15, 20, 24, 29, 33, 56; g3 = 17; g4 = 5 / 6 (+ g4b binder-only link); g5 = 28, 32, 36 (probe), 37
  (reverted), 60; g6 = 62, 70, 71, 75, 76, 78; g7 = 12, 48, 70's untabled binder 0x00417001; g8 = 0x009FB750 (name-census control). Per
  group: changed list + controls (selves + the full exe census, minus other agents' rows), pre_tests.sh on every landed test BEFORE the
  locks (NOWCHAR reruns; D1 cl /Zs for 0x0057C4F1, 0x0064BFBA, 0x007EF200, 0x00766C00, 0x007677E0, 0x007BFA90, 0x008385B0, 0x009C8DE0,
  0x004A3740, 0x0050A650, 0x006A16A0, 0x0090E5D0, 0x00617A00, 0x00617A90 tests and the 0x00839100 / 0x007677E0 HEAD sources, whose clang
  failure is only engine/CTCObjectAugmentations.h line 107), locks, medit.py / ins.py specs (work/spec_*.json, ins_*.json), clang
  prechecks of every new and HEAD source, grades in up to 4 background lanes (`timeout 900` per grade line), then bind / proof / syms /
  link / gen_ren / assemble, then one vland recheck of all 38 rows.
- Edit classes: pointer -> reference (`*p` / dropped `&`), pointer -> const pointer, `unsigned long` for `long`, a shared enum header with
  literals -> enumerators (EHeroAbility member retype, ETargetingType, EObjectAugmentationType), a `long` member retyped to the PDB enum,
  return type `bool` / `unsigned long`, access (`protected:`, one friend), `__fastcall` on a no-argument static, a nested-class re-scope.

## Rule notes
- No sleeps; every VC7.1 grade, cl /Zs, link and vland ran in the background with `timeout 900` on its own launch line and was checked
  with kill -0 / done files in a later call; clang prechecks ran in the foreground under `timeout 120`. No rm: removals went through
  clean.py (four rows moved to held/ and the 0x00836A50 probe out of rows/, its orig/ copy too); nf.py's own scratch cleanup now calls clean.py too. Slip: the inherited nf.py removed its own grade / zs / link scratch with shutil.rmtree / os.unlink (inside my work/ folder) during group g1, before I patched it to call clean.py.
- Every heredoc quoted. No retail or donor bytes in hand-back files (the vland oracles stay in work/).
