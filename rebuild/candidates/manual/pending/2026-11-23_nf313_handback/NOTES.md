# NF313 notes (2026-11-23): NF311's tail (4 held + 48 unopened signature entries) + 17 uncovered tracker T1 entries, hand-back 1

Base: the repo working tree as of today; all 22 orig/ copies are cmp-equal to HEAD at hand-back (no staleness). Nothing was written
under the repo; no git command of any kind was run. "Line n" = the n-th data line of ENTRIES.tsv. STATUS.tsv has all 69 entries.

## Summary
- **22 rows changed, landing-ready** (rows.txt; vland --recheck-landed 22/22 RELOCATION_MATCH + test PASS, recheck_final.log):
  - **Entry 18 unit (line 3, NF302 Q5 / NF304 Q6):** owner 0x0097E330 self QAE -> IAE (`protected:` + full Leaf) with all four landed
    QAE binders: NF308's three (0x00967930 / 0x00967B40 / 0x00968BC0, re-derived on today's rows: identical to NF311's held_18 files but
    for the comment tag) and 0x009689C0 line 73 (re-derived after NF310's landing; line 74 is `protected:` already). LAND TOGETHER, with
    the QAE delete line.
  - **Batch A (pointer -> reference / const / unsigned; the owner HEAD object already defines the PDB form, evidence/owner_defs_A.txt):**
    every landed binder of each stand-in, not only the table witness: CalculateMatrix 0x00B242A0 + 0x00BACBD0 + 0x00BC5FC0;
    SetMainAnim 0x00861A80; CEngineInternalPrimitiveBase ctor 0x00B3A910; SetText (6-arg) 0x006CEF20 + 0x00729B20 + 0x007800B0 +
    0x008139C0 + 0x00849380; SetText (9-arg) 0x007DF4B0; AddObject 0x009D5A90; GetRegionNumberFromName 0x0088E0B0 (+ `const`);
    PerformSequenceReset 0x006EA450 + 0x006EABB0 (+ `private:`, PDB AAE; both are CCombatManager members).
  - **Batch M (NF300 Q1 + R1):** AIAction_MinionWander binders 0x0092A8A0 + 0x0092B3E0 -> the PDB IAE signature: shared
    engine/EMovementType.h included, literal 3 -> AI_MOVEMENT_WALKING, return EAIActionReturn, `class CThingBuilding;`
    (evidence/ppdiff_M.txt: the preprocessed diff is only the enum, the forward class, the declaration and the literal).
- **Proof:** HEAD and new RELOCATION_MATCH for every row (evidence/gr_*.log); objsec ALL_EQ under exactly one rename per row, .drectve
  included, tables unchanged (evidence/proof_e18 / proof_A / proof_M); COMDAT sets SAME (cmpfn_*); links (evidence/link_e18 / link_A /
  link_M, owners' HEAD objects included): HEAD leaves each stand-in unresolved, the new set resolves every moved reference, nothing HEAD
  resolved becomes unresolved, 0 duplicates. Each row's only defined-symbol change is 0x0097E330's own self (work/newdefs v headdefs).
- **Table (TABLE_CHANGES.tsv, 10 deletes):** one per fixed stand-in spelling, each LAND WITH its rows. Every delete was checked against
  all exe call sites of its target (tools/delchk.py -> evidence/xsites_<target>.tsv: each landed caller's declaration lines, the
  shared headers' declarations for header users) and absolute references (0 for every target; none is virtual). For 0x0097E330 also a
  source grep of SetPushableByHero / 0097E330 over src, tests and include (no #include-folded caller).
- `apply_table.py --dry` on TABLE_CHANGES.tsv: 10 delete, 0 misses, no new multi-target symbol (evidence/apply_table_dry.txt).
- Counts: 10 done-partial (line 3 = the entry-18 unit: 0x00944DB0's made-up CAIStateGroupBaseI line remains; lines 12, 15, 17, 24, 30,
  31, 34, 35, 53), 31 skipped-held (7 on off-limits / T3-queue ties: lines 1, 2, 4, 14, 25, 40, 57; 24 in place but not done or awaiting
  a ruling, listed in unreached.txt: Q3 / Q4 / Q5 / Q6 / Q7), 28 skipped-port (local STL models, borrowed element types, made-up
  classes, same-TU stand-in bodies, other declarations), 0 rejected. done-partial = every in-place binder of the entry moved; the
  remaining stand-ins bind made-up classes / free stubs / T3-queue rows.
- evidence/held_18/ is NF311's prepared copy, superseded by rows/ (same edits, re-derived on today's rows; only the comment tags differ).

## How the verdicts were reached
- Census first (tools/census311.py over all 69, full spellings from reloc_bindings.tsv), then tools/sig313.py: for each SIG / OPEN
  stand-in the demangled PDB v stand-in signature and the witness's declaration lines; tools/owner313.py: the owner's Leaf and
  definition line (the advisor's rule: a binder may move alone only when the owner defines the PDB form or has no row; an owner that
  defines the binder's stand-in makes an owner unit).
- **Table witnesses undercount binders:** CalculateMatrix had 3 pointer binders, SetText 5, PerformSequenceReset 2, MinionWander 2,
  GiveTutorialForCategory 8, CTCBase(CThing*) 58, SetPItem(CThing*) 10+ (one witness each in the table). delchk.py lists them.
- Stale suspects compiled (evidence/und_stale.txt): 0x0090F0D0 / 0x0095A890 / 0x007BB150 still bind the stand-ins through a local
  `typedef long E...`; 0x0089E540 is stale (binds the PDB SetPItem form; Q4).
- Held: lines 1 / 2 / 4 (NF311's entries 3 / 17 / 23: 0x004BFFC0 still in T3_PORT_QUEUE, 0x009A6610 an NF309 witness, 0x007BDB66 an
  NF312 witness), 14 (owner unit needs NF309 witness 0x0096FCD0), 25 (target 0x00820B80 is an NF309 witness), 40 (0x00733950 NF312
  witness), 57 (0x00845CE0 T3 queue).
- Order (E2): line 3 (entry 18 unit) first, the held lines 1 / 2 / 4 re-read, then the stale suspects, then batch A, then M, then the
  rest by census + source read.

## LAND TOGETHER
- Entry 18: 0x0097E330 + 0x00967930 + 0x00967B40 + 0x00968BC0 + 0x009689C0 + the SetPushableByHero QAE delete.
- Each batch-A / M delete line with all of its rows (named in its note): 0x00B242A0 + 0x00BACBD0 + 0x00BC5FC0; 0x006CEF20 + 0x00729B20
  + 0x007800B0 + 0x008139C0 + 0x00849380; 0x006EA450 + 0x006EABB0; 0x0092A8A0 + 0x0092B3E0; the single-row ones with their line.
- Includers by path: none of the 22 changed sources is named by any file under rebuild/src, rebuild/tests or rebuild/include other than
  itself (basename grep, evidence/includers.txt: no unwind funclet or folded TU includes them).

## Rule notes
- Locks: lock.sh NF313 before any compile; every landed test prechecked BEFORE its lock (pre_tests18 / testsA / testsOA / testsS /
  testsW / testsM); every compiled source (HEAD and new) prechecked after its lock in an earlier call (pre_srcH18, srcHA, rowsA, srcOA,
  srcS, srcW, srcHM, rowsM). D1 `cl /Zs` (earlier call, background, timeout 900): tests 0x007DF4B0 / 0x0070D580 / 0x0089E540 (clang
  fails only in VC7.1 cwchar / ostream / functional), test 0x0092B3E0 and source 0x0070D580 (clang-only errors on MSVC-accepted syntax in
  unchanged landed files: address-of-temporary, a for_each over an anonymous-namespace functor; NF300 D1 extension): all ZS_OK
  (evidence/zs_*.log). pre_rows18.log carries 5 FAIL lines from a first run with relative paths (clang could not find the files), then
  the 5 PASS lines of the rerun with absolute paths, before any compile.
- Held now: the 22 changed rows. Released: every owner / witness lock used only for HEAD compiles (evidence/locks.log).
- Background: every grade / link / /Zs / vland ran under nohup with `timeout 900` on its own launch line (tools/gr.sh, lk.sh, zsrun.sh,
  rf.sh) and was checked with kill -0 / its DONE line in a later call. One slip: a first recheck payload was built through `| head`, which
  broke the pipe before the payload was written; I killed my own rf.sh PID at once (no child was running: ps check), rebuilt the payload
  and relaunched. No sleep, no rm, no header / test / flag / catalog change, no __asm / naked / _emit.
- Off-limits (tools/offl.py v2): NF309 / NF312 targets plus EVERY reloc_bindings witness of their targets (their ENTRIES witness lists
  are cut with `...`), live locks, codex-p28 reservations, held rows, T3_PORT_QUEUE; refreshed before each claim.
- No retail or donor bytes in hand-back files (hex-run grep clean); oracles stay in work/.
- Reading: the prefix, the standing brief, all of GOTCHAS.md and NF311's hand-back + landing README in full; CLAUDE.md as given.

## Questions
1. **Entry 18 unit** (5 rows + the delete): ready as one unit now that NF310 has landed. Confirm.
2. **Stretched rulings, with drop units.** (a) Batch A edits call-site expressions as well as the local model (`&x` -> `x`, `p` -> `*p`,
   bytes equal): R1 "wider fix, byte-identical, evidence-backed". If only model-line edits are allowed, drop each batch-A delete line with
   its rows (units in LAND TOGETHER; nothing else depends on them). (b) Batch M goes past NF300 Q1 (enum header + literal) with the
   access label (IAE), the return type EAIActionReturn, `CThingBuilding*` for `CThing*` and a forward `class CThingBuilding;` (both PDBs
   give the signature; the argument is a literal 0). If that is too wide, drop 0x0092A8A0 + 0x0092B3E0 + the MinionWander delete together.
3. **`typedef long E...` rows** (0x0090F0D0, 0x0095A890, 0x007BB150; 0x0044C47B passes its own `long` ctor parameter): the binder fix is
   to include the shared enum header, but the typedef also spells the row's OWN self (virtual Initialise overrides, PDB UAE ...W4E...;
   CTCTrespasser::SeeIfAnyOccupantsKnowAboutDeed AAE ...W4EOpinionDeedType). Does NF304 Q6 cover a self rename of a virtual override
   (its vftable slot binder renames too), or are these ports?
4. **0x00A01B90 SetPItem (line 19):** the `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCThing@@@Z` witness 0x0089E540 is stale (its HEAD
   object binds the PDB PAVCBaseObject form). At least 10 landed rows still declare `SetPItem(CThing*)` (e.g. 0x008C8180, 0x008C0900,
   0x00930EE0), but the first one compiled (0x007CC060) never references it (inline not emitted). Rewitness after a landing-side scan of
   the declarers' objects, or delete if none binds it?
5. **Next-batch in-place candidates not done here (budget; STATUS skipped-held, unreached.txt):** 0x0091E050 alone under NF302 Q2
   (line 14, the rest of that entry is held on an NF309 witness); owner units 0x006EBDD0 (line 21: + 0x0092B3E0 + 0x006EBEE0),
   0x00843F50 (line 22: + 0x007F03D0 + 0x007EF600), 0x007008C0 (line 27: + 0x00784BD0); GiveTutorialForCategory's 8 `long` binders
   (line 33); 0x00919A70 (26), 0x00B2C460 (28, owner self unread), 0x00738F90 (40, also held on an NF312 witness), 0x006B8640 (54 / 55
   re-scope + its two GetCurrent* callees).
6. **CTCBase(CThing*) (line 32):** 58 landed sources declare the pointer form (one table witness). One in-place unit in a dedicated
   batch, or T3?
7. **LSocket (lines 59-68):** one unit (every row naming LSocket / LSocketServer / LAddress moves into LHNetworkLib, private members
   with friends where the retail access forces them, 0x00840070's deliberate global model). A dedicated batch?
