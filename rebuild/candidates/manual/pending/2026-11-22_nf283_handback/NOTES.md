# NF283 notes (2026-11-22, round 35): continuation of NF281's T1-access + T2 batch, first hand-back

Base: the repo working tree as of this session (never written; no git). NF284 ran beside me: none of its 359 entries, none of the Codex
pass 28 RESERVED rows, none of 0x00B4D370 / 0x00BD3070 / 0x00890250 / 0x0091C080 or the NF279 / NF280 entries touched
(work/offlimits.txt, 408 addresses, intersected with every row I locked: empty). Every row I compiled was claimed with tools/lock.sh first
(work/locks.log). Rows I claimed and then dropped before any compile (failing landed tests) were written back to `released` at once;
the 46 control / rejected rows I compiled were written back to `released` at the end. rows.txt = the 47 changed rows, all NF283.

Reading list read in full: BRIEF.md (prefix and body), agentNF278/BRIEF.md, NF214_BRIEF.md, docs/pipeline/ASM_RULES.md, EXTRA_RULES.md,
docs/COORDINATION.md, docs/pipeline/GOTCHAS.md (all 504 lines), the nf266 / nf268 / nf281_282 landing READMEs, the tracker SUMMARY.md and
OPEN.tsv header, NF281's NOTES / LESSONS / STATUS.

## Method (NF281's, unchanged except where noted)
1. **Census before any edit.** Exe scan of every reference to each entry address (tools/census.py -> work/census0.txt); clang MS-ABI -O0
   objects of all 969 self + referencing rows (tools/csym.sh, now with `-Wno-address-of-temporary` per ruling D3; 61 still fail clang) ->
   work/csum.txt (tools/csum.py: per entry, which row DEFINES / binds which spelling); both PDB name sets for every entry (work/pdbchk.txt:
   all 191 pdb_spellings are in Ego_r AND FableWin). Every wrong binder the clang census named was confirmed by its VC7.1 HEAD object.
2. **Edit in place, zero line delta** (tools/fix.py, tools/rep.py), as nf266 / nf268 / NF281; tools/ctx.py checked the access in effect
   (public everywhere; three "default" hits were nested-struct false alarms, checked by hand).
3. **Precheck** (clang, in an earlier call, after the lock): new source, HEAD source and the landed test of every changed row (prep.sh,
   pret.sh), the HEAD source of every control (new tools/preh.sh). New in this pass: a row whose LANDED TEST fails the clang precheck was
   dropped before any compile (the nf281_282 landing counted g++-only test prechecks as slips). Controls are compiled source-only (q.py),
   so a control's failing test does not block it. Plus tools/csymf.sh: the clang symbol diff of every edited row against its HEAD census.
4. **Grade + proof** (tools/cls.sh, background, polled with kill -0): HEAD and new VC7.1 objects (each sc.sh under its own timeout 900),
   HEAD controls, proof.sh (objsec HEAD v new under the renames), lk.sh (HEAD set v new set). evidence/proof_c1..c4, link_c1..c4.
5. **vtmap** of every table-emitting changed row, HEAD v new (evidence/vtmap_*): 0x00A26D40, 0x006DD8F0, 0x009ADE90, 0x009BFB30 (none),
   0x00B52250: identical.
6. **Recheck**: work/rf_b1..b3.log, then recheck_final.log (one vland --recheck-landed run over all 47 rows, `timeout 900` on its launch line).

## Result (entries 1-60 of 191 reached, ENTRIES order; 61-191 open, not started)
- **35 done + 1 done-partial (11), 3 done-already (7, 15, 48: table-only, VC7.1 HEAD objects of the stale witnesses), 6 rejected
  (5, 8, 20, 29, 39, 49), 2 decisions (21, 22), 12 skipped-precheck, 1 held (10, D4 by the prefix).** STATUS.tsv has the reason per entry.
- 47 rows changed (rows/, orig/ = HEAD copies, all 47 still equal to the current repo file at its current path; none #included by another
  row, test or header; all zero line delta). objsec HEAD v new ALL_EQ under the renames for all 47; no table appears or changes.
- Links: every HEAD set leaves the moved wrong spellings unresolved and the new set resolves them, with the same duplicate count,
  measured on exactly the handed-back rows: link_c1b 4 -> 4, link_c2 1 -> 1, link_c3b 1 -> 1, link_c4 2 -> 2 (c1b / c3b re-linked with
  the set-aside rows' HEAD objects on both sides; the first link_c1 / link_c3 new sets still held them and are marked superseded). Left unresolved in both sets: spellings whose definer is a stand-in self
  (NotifyEntryAdded MAE, ConvertPathToFilename ABE, UpdateLargestFreeArea AAE) and symbols outside the batch.
- RENAMES: 10 self-leaf renames (section 1; each new name defined only by its self row: evidence/name_census.txt), 43 caller
  respellings (section 2). TABLE_CHANGES: 37 lines (tools/tabrec.py from section 2, plus the four stale done-already witnesses and the
  self-witness line of 0x00B91D20's recursive call; entry 11 is `add` IAE + `rewitness` the QAE line to 0x00694020, which still binds
  it, so the split stays visible). Every other replace / delete was swept: no unmoved row (clang census or, for clang failures, source)
  still binds an old spelling. NAME_FIXES: 9 findings outside my rows. No header touched (hdr/ empty), no flags.

## Decisions I need, per row (land or drop): edits beyond an access / virtual / const letter on the declaration
- **Entry 12 (0x00692EC0, T2 const):** the const body calls slot 36 of the local CCreatureActionBase model (`IsPlayingPrecedingAnimation`,
  a descriptive name with no PDB entry, called through the vtable so it has no symbol). It was made `const` on its own line so the body
  compiles. No byte or symbol effect (ALL_EQ); the slot's real name / const-ness is still unproven.
- **Entry 44 (0x00B4B830, T2 virtual):** the local model was not polymorphic, so `virtual` also gets the leading pad shrunk 0x2af -> 0x2ab
  on its own line (the vptr takes +0, every member keeps its offset). No ctor in the TU, no table emitted; ALL_EQ.
- **Entry 57 (0x009FE380):** besides `const`, the one-line local model's return type goes `long` -> `unsigned long` (both PDBs K): a
  signature respell on the same line (QAEJXZ -> QBEKXZ). ALL_EQ.
- **Entry 60 (0x007E5230):** `friend class CTCBetting;` on the access-fixed line, naming the real class (CTCBetting::ProcessInput calls
  the protected PDB IBE member through a CInputProcessGameBase*): the form the D1 ruling accepts.

## Points to look at when landing
- **Entry 14 side effect:** `virtual` on CCreatureAction_UnsheatheItemFromInventory::FinishAction makes 0x006DD8F0's own override virtual
  (QAE -> UAE, both PDBs UAE; no landed row names 0x006DD8F0; no Leaf line at HEAD, not added).
- **Entries 2 / 11 / 53:** a binder that fails the clang precheck was checked by source: 0x00753630 already binds QBE through
  engine/CAnimationEntry.h (entry 2 complete); 0x00694020 still binds QAE (entry 11 partial, NAME_FIXES); 0x00599E3F binds IAE by source (its `#define FABLE_RETAIL_CFRONTENDMANAGER_VIRTUALKEYBOARD`, line 34, selects the
  `protected: bool CreateNewProfile(CWideString&);` of engine/CFrontEndManager.h lines 36-37).

## Decisions I need (general)
- **D1 (precheck of landed tests):** 12 entries are skipped because a changed row's landed test fails the clang precheck: VC7.1 <cwchar> /
  iostream (0x00B82100, 0x00B619B0, 0x0069A990) or `_assert` overload resolution (0x00BCFDE0 / 0x00BCFCB0 / 0x00B9DE90, the three cloth
  binders), or the row itself fails clang (0x00A5B710 / 0x00A5B760 `short wchar_t`, 0x00859EC0 / 0x00859EF0 CTCFlourishTarget.h asserts,
  0x005955AB family, 0x0059A238, 0x004962C0). May a test that only includes VC7.1 system headers clang cannot parse be prechecked another
  way (NF281's g++ host-header form, or clang with `-D` / `-fno-...` flags for <cwchar>)? With that, entries 27, 28, 30, 31, 41, 42, 56 are
  in reach (drafts for 27 / 28 / 31 / 56 in work/unused/).
- **D2 (entries 21, 22, CDiskFileWin32 Read / Write):** the only fixable binders 0x0055D070 / 0x00B2C8E0 keep local CDiskFileWin32 models
  that emit `??_7CDiskFileWin32@@6B@` (inline ctor / dtor); making Read / Write virtual adds slots and moves SetPosition (retail order:
  Open, Close, Read, Write, SetPosition). A table change, so not compiled. Port these two onto engine/CDiskFileWin32.h (which has the
  retail slot order) as T3, or allow the in-place table change if vtmap shows it moves toward retail?
- **D3 (D2-pattern rejections, entries 8 and 29):** a same-TU strong copy elsewhere already carries the PDB spelling (0x0099F9B0 for
  AllocStringData IAE; 0x00B80F90 for ConvertVectorToLocal IBE), so moving the self row adds a duplicate. Same ruling as NF281's D2
  (T3), or make those copies pick-any first? Both drafts graded ALL_EQ (work/unused/).

## Rule slips
- Claimed 14 locks that I then released without compiling (the rule is to claim only locks I will compile): 0x00BCFDE0, 0x00BCFCB0,
  0x00B9DE90, 0x00B82100, 0x00B81050, 0x00B619B0, 0x0069A990 (their landed tests failed the clang precheck after the claim) and the
  controls that went with them, 0x00B81F00, 0x00B81ED0, 0x00B81EA0, 0x00B81F30, 0x00B5FAB0, 0x00B60F70, 0x00699130. Each was written back
  to `released` in the same call (work/locks.log `released-uncompiled`). From now on: precheck a row's landed test before claiming it.
- Otherwise none that I know of in the compile rules: every compiled source and every compiled changed-row test had a passing clang precheck in an
  earlier call after its lock; every grade / recheck had `timeout 900` on its own launch line; no rm on any path (dropped drafts were moved
  to work/unused/ with mv); no git; nothing written under /home/user/FableDecomp.
- The clang census (csym.sh, 969 rows) ran without locks, as NF281's did: it is a census, not a grade (no VC7.1, no lane work dir).
- 0x0099EA60 (entry 8) and 0x00B80AD0 / 0x00B80FE0 (entry 29) were compiled and only then found blocked by a same-TU strong copy (the
  c1 / c3 links). Not handed back. From then on I grepped for other definers of a self rename's new name BEFORE compiling.
