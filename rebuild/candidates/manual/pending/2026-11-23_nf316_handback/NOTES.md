# NF316 notes (2026-11-23): NF312's 29 held T1 signature entries

Base: the repo working tree of today (after landing nf312). No repo writes, no git.

## Reading
Read in full: my BRIEF.md prefix, the standing brief BRIEF_NF_BATCH.md (with the NF299-NF313 rulings), docs/pipeline/GOTCHAS.md (all
514 lines), NF312's hand-back (NOTES, LESSONS, STATUS, TABLE_CHANGES, RENAMES, NAME_FIXES) and its landing README; skimmed EXTRA_RULES.md
(naming rules) and COORDINATION.md.

## Stay-off lists
tools/mkoff.py retargeted to NF314 / NF315 (and widened: T3_PORT_QUEUE column 3 addresses per NF309 Q5; their rows/*.cpp as well as
rows.txt): work/offlimits.txt (2175), work/other_agents_entries.txt (119), work/other_agents_witnesses.txt (412). tools/whose.py
retargeted likewise. Re-run right before the lock claims; no lock held by anyone else was hit (all 53 rows I compiled were free).

## Result: all 29 entries reached
- **6 done / done-partial, 2 skipped-port, 21 skipped-held.** Order: every entry re-screened first (evidence/cstat_all.txt,
  evidence/whose_ties.txt); then g1 = entries 1, 2, 5, 13 (binder respells) + 10 (table only); g2 = entry 11 (self + 9 binders).
- **Why 21 stay held:** NF309 / NF311 have landed, but most of these binders are now named in NF314's or NF315's ENTRIES spellings
  column (NF314 carries NF309's work on; whose.py: entries 3, 4, 6, 8, 9, 15, 23-28), entry 7 still waits for 0x005296B0 (now NF314#14's
  target), and entries 12, 17-22, 29 have their only wrong binder in T3_PORT_QUEUE. Each STATUS line names the tie.
- **2 ports (no longer tied):** 14 (binder 0x00AD9D70 passes stand-in CRemainderArray / CBoolArray structs), 16 (binder 0x00750BE0 calls
  through the stand-in CThingDefClass and carries a stand-in CThingBaseDef::GetSubDef body). Both binders opened.
- **14 rows changed** (rows.txt == the 14 locks I still hold; all 39 control locks released). No test changed (all 14 landed tests are
  standalone models; none includes its row or links the respelled callee). No header changed (hdr/ empty). All 14 orig/ copies equal
  HEAD at hand-back.
- **1 self rename** (RENAMES section 1): 0x0051E210 CThingManager::RemapUIDForSerialisation QAE `long` -> the PDB QBE `unsigned long`
  (full mangled Leaf added; NF304 Q6: all 9 landed binders of the old form move in the same landing).
- **RENAMES section 2:** 13 caller lines. **TABLE_CHANGES:** 6 lines (2 delete for moved witnesses, 2 replace, 2 hand deletes of
  stale lines: entry 10's PAV line and entry 11's QAE line, whose witnesses already bind the PDB form). **NAME_FIXES:** 1 line (0x006B0210: the self defines the stand-in CTCPhysicsBase_Methods; 0x006A4DC0 binds `Sub::Run_006b0210`).

## Proof
- Grades (evidence/grades.log): HEAD + new VC7.1 objects of the 14 rows and HEAD objects of 39 controls (the FULL exe census of every
  target touched: 3 + 20 + 1 + 1 + 21 + 1 rows, minus the changed ones); all RELOCATION_MATCH except 0x00846100, which the scratch grader
  reports DIFFER(687v686) for HEAD and new alike; vland (catalog oracle) reports RELOCATION_MATCH for it (recheck_final.log).
- objsec HEAD v new (evidence/proof_all.txt): 14 ALL_EQ; `.drectve` byte-equal for all 14 (evidence/drectve_eq.txt); the only emitted
  table (0x0040D350's `??_7CDiskFileWin32@@6B@`) unchanged inside ALL_EQ. Symbol diffs (evidence/syms_diff.txt): only the respelled UND
  symbol per binder and the self's DEF; no pick-any COMDAT renamed.
- Bindings (evidence/bind_head.txt v bind_new.txt): every changed binder flips WRONG -> OK at the entry address; nothing else moves.
- Links (evidence/link_g1.txt, link_g2.txt; /FORCE, LINK= cleared, HEAD set v new set with every census control): no duplicate in either
  set; nothing HEAD resolved is unresolved in the new set; g2's new set resolves the 12 PDB-form RemapUIDForSerialisation binders HEAD
  left unresolved and leaves none. g1's new set leaves only ?ApplyAcceleration@CTCPhysicsBase@@QAEXABVC3DVector@@@Z unresolved (18 rows
  at HEAD, 19 new) because the self row 0x006B0210 still defines the stand-in CTCPhysicsBase_Methods::ApplyAcceleration (NF302 Q2;
  owner queued in NAME_FIXES).
- Table v callers (evidence/table_check.txt): every delete / replace line checked against the full exe caller list of its target (calls,
  jumps, absolute dwords; no target is virtual), each caller compiled.
- Name census (evidence/name_census.txt): each respelled name is defined (source and object) only by its own self row; no same-TU body
  of a respelled callee in any binder.
- Two sources per name (evidence/pdb_namesets.tsv): all six PDB spellings are in both the Ego_r and the FableWin name sets; second
  sources per entry: the self's DEF (1, 5, 10, 13), engine/CThingManager.h:157 + 12 compiled PDB-form binders (11), 18 compiled PDB-form
  binders (2).
- vland --recheck-landed (recheck_final.log, the 14 rows from rows/ + their landed tests): WINS 14 (14 RELOCATION_MATCH), every test
  PASS; no RELOC_CONFLICT / NOT_GENUINE / HEADER_POLICY; the RELOC_NAME warnings name other callees' catalog labels (pre-existing).

## Decisions / questions
- **Q1 (held on the neighbours' spellings columns):** 12 entries (3, 4, 6, 8, 9, 15, 23-28) are held only because a binder is a witness
  in NF314's / NF315's ENTRIES spellings column (whose.py), though their locks were free. I read "stay off their witnesses" literally.
  Several of those witnesses bind a DIFFERENT target of theirs (e.g. 0x00BFC770 for NF314#41 WriteUShort, 0x00733950 for NF315#15
  GetComponent): once NF314 / NF315 land, these 12 can be taken in one short pass (each is a one-binder pointer -> reference or
  const fix, except 4 / 26-28, which are ports anyway). Agree?
- **Q2 (entry 2, done-partial):** 0x006A4DC0 binds a made-up `?Run_006b0210@Sub@@QAEXXZ` through a placeholder forwarder (`struct T`,
  catalog `_global::Forward_ApplyAcceleration_via0x60_006a4dc0`). Its own identity looks like CThingPhysical::ApplyAcceleration(const
  C3DVector&) (both PDB name sets carry ?ApplyAcceleration@CThingPhysical@@QAEXABVC3DVector@@@Z; the retail body forwards its stack argument
  through the +0x60 member, which is `CTCPhysicsBase* PPhysics` in the landed 0x00846100 model; the address sits beside CThingPhysical's
  virtuals, e.g. slot 35 at 0x006A4E70): a self re-scope on shape evidence, so identity research / T3, NOT in NAME_FIXES and not applied. The self 0x006B0210 still defines CTCPhysicsBase_Methods::ApplyAcceleration (T3 self port). Is landing the binder alone
  under NF302 Q2 fine?
- **Q3 (entry 11, the shared declaration):** engine/CThingManager.h:157 already declares the PDB form; the 9 binders keep their local
  CThingManager models (now with the PDB signature), so the strict HEADER_POLICY debt of those local models is unchanged (vland reports
  SHARED_HEADER_AVAILABLE warnings only).
- **Q4 (entry 13):** `protected:` is written on the local model's declaration line (`protected: void LoadGameState(CAFile& file); public:`)
  to keep the line count; the caller is the class's own Load, so no friend is needed.

- **Q5 (T3 queue reason-column mentions):** a full-text grep of T3_PORT_QUEUE.tsv (evidence/t3_fulltext_screen.txt) names four of my
  binders only in its reason column, as binders in OTHER targets' port units (never in column 2 / 3): 0x0040D350 (VerifySignature free
  stand-in, unit 0x00409730), 0x00522880 (SaveThingList stand-in, unit 0x00521EB0), 0x00846100 (GetHeadPos, unit 0x00661B40), 0x00969470
  (units 0x0082D000 / 0x00773C00 / 0x004C7950). My edits touch other lines only and are ALL_EQ. If the rule covers reason mentions, drop
  per entry exactly: entry 1 = rows/00522880.cpp + its RENAMES section-2 line + the GetInventoryOwner QAE delete; entry 2 = rows/00846100.cpp
  + its RENAMES line + the ApplyAcceleration PAV delete; entry 5 = rows/00969470.cpp + its RENAMES line + the GetEmptyJugSlot replace;
  entry 13 = rows/0040d350.cpp + its RENAMES line + the LoadGameState replace. Entries 10 and 11 are independent of all four.

## Extra checks (after the first draft)
- Binders outside the exe census (evidence/extra_binders.txt): every landed source naming a respelled method was grepped and the extra
  files opened; none binds an old spelling (0x0057DE57's same-TU copy of the 0x005800D6 template declares the PDB ABV form). 0x006A7F30
  strongly DEFINES ??RCThingFilter_HasScriptData@@QBE_NPAVCThing@@@Z as its own (wrong) self; it is CThingFilter_HasTrackName, already
  queued (NAME_FIXES_QUEUE land1120nf110, T3). Path includers of the 14 changed files: none (evidence/path_includers.txt).
- Repo paths newer than my BRIEF.md (find, .git excluded): tools/decomp_pipeline/__pycache__/naming_status.cpython-313.pyc (06:34),
  tools/decomp_pipeline/naming_remainder.py (06:35), docs/journal/2026-11/2026-11-23_naming_remainder.md and docs/ROADMAP.md (06:36).
  None is mine (I never ran naming_status.py or wrote those files); they look like the coordinator's. Nothing deleted.
- An empty `D:\Documents\FableTLC\rebuild\build\landverify` tree appeared in my folder at 06:36:58 (a verify_and_land import with cwd =
  my folder, NF312's lesson); removed with clean.py, as was the spent vland work dir work/wd_rf.

## Method / rule notes
- Per group: lists (work/g*_changed.txt / g*_controls.txt), pre_tests.sh on every landed test BEFORE the locks (43 PASS; the changed
  row 0x00846100's test fails clang only in VC7.1 headers and passed a VC7.1 `cl /Zs` (D1) in a later call before the recheck; three
  controls' tests fail clang in their own text and were never compiled), mkoff / aflags re-run, locks, medit.py / ins.py specs
  (work/spec_g1.json, spec_g2.json, ins_g2.json), clang prechecks of every new + HEAD source and every control (67 PASS), grades in 4
  background lanes (`timeout 900` per grade line), then bind / proof / syms / drectve / link / gen_ren / assemble, one vland recheck.
- No sleeps; every VC7.1 compile, cl /Zs, link and vland ran in the background with `timeout 900` on its own launch line and was checked
  in a later call; clang prechecks ran under `timeout 120`. No rm (nothing deleted except through clean.py). Every heredoc quoted. No
  retail or donor bytes in hand-back files (the vland oracle stays in work/).
- Slip: my first recheck.sh edit (cwd -> my work dir) failed, so the one vland run started with cwd = the repo root; it had already
  finished (WINS 14) when I went to stop it. `find /home/user/FableDecomp -newer work/rf_payload.json` (excluding .git) lists nothing:
  it wrote nothing there. recheck.sh now cds to my work dir.
