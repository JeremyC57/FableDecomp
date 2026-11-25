# NF443 hand-back (2026-11-25): T1 list B caller respells, continued from NF439

## Result
| batch | rows | renames | targets (closed / partly) | TABLE_CHANGES | CROSSBATCH | bodiless class (r3) | friend (q1) | free -> static | held |
|---|---:|---:|---|---|---:|---:|---:|---:|---:|
| a1 | 48 | 51 | 5 (0 / 5) | 6 delete | 2 rewitness | 10 | 0 | 0 | 2 |
| a2 | 49 | 53 | 14 (6 / 8) | 2 add, 21 delete | 1 rewitness | 41 | 10 | 27 | 1 |
| a3 | 49 | 51 | 29 (21 / 8) | 9 add, 39 delete | 1 rewitness | 39 | 9 | 10 | 1 |
| a4 | 46 | 51 | 51 (39 / 12) | 22 add, 49 delete | 0 | 39 | 7 | 6 | 4 |
| a5 | 45 | 48 | 48 (41 / 7) | 18 add, 46 delete | 2 rewitness | 31 | 11 | 5 | 4 |
| c1 | 50 | 50 | 1 (0 / 1) | none | 0 | 0 (model edit) | 0 | 0 | 0 |

- 287 ACCEPT caller rows, 304 callee respells, 148 distinct B targets; 107 have no wrong table spelling left once the six batches
  and their CROSSBATCH lines are applied. No header changes (HEADERS.tsv empty in every batch). Each row's own self is unchanged.
- No target or table line is shared between batches (checked over every renames.tsv), so each batch is its own unit (UNITS.tsv = the batch).
  The batches can land in any order. CROSSBATCH.tsv holds every rewitness: each one names a still-binding row outside the batch.
- Ruling tags: a1-a5 `ruling.txt` = r3 (bodiless local class; the new SHARED_HEADER_AVAILABLE warn is header debt, one line in T3.txt).
  c1 has none: it edits the row's own model, not a bodiless class.
- Locks: NF443 on exactly the 287 ACCEPT rows; every other NF443 lock released (holds and rows not batched).

## Shapes
- **a1-a5: tools/rebind443.py.** This is NF438's rebind438.py run over the B targets (tracker = T1_B ∩ NF439 RESIDUE, minus T1_A and
  0x0070E8B0; the 12 + 1 big-family targets NF439 set aside are excluded), with three extensions:
  1. **Protected / private PDB members (q1).** The bodiless declaration carries the member under its PDB access label, so it binds
     IAE / AAE. The caller gets access by one of two lines: `friend class ::X;` or `friend struct ::X;` when the call is in a member
     of a namespace-scope class X (X is forward-declared with its own key), or `friend <head>;` for a plain global free caller.
     Anything else is residue.
  2. **Free stand-in -> static member, same calling convention** (e.g. `?Create@@YI...` -> `?Create@CTCDExplosion@@SI...`). The call
     becomes `Q::M(...)` on a bodiless `class Q { public: static ... };`. The ABI is unchanged, and objgate proves it.
  3. **Const-away rows.** Clang flagged 10 rows; they use `const_cast` for the object and a parenthesised C-style cast for pointer and
     reference returns and arguments.
- **c1: CCombatSequenceBase ctor 0x008B0AB0.** 50 of 63 identical factory rows. The row's own model ctor goes from
  `(CThingCreatureBase*, CThingCreatureBase*)` to the PDB `(CThingCreatureBase&, CThingCreatureBase&)` (both PDBs:
  `??0CCombatSequenceBase@@QAE@AAVCThingCreatureBase@@0@Z`), and the derived inline ctor passes `*first, *second`. The pushes are
  unchanged and objgate is ALL_EQ. The other 13 rows (work/c1_left.txt) are generated, clang-clean and ready (work/cs_rows) for the next
  batch. They stay out because the component is 63 rows, over 50. They keep the old table line: its witness 0x008B5550 is not in c1.
- **Tests (q2).** HEAD tests that stub the callee under the stand-in name were changed in two forms:
  - **a1 (0046c2d2, 0047931e):** the stand-in stub is REPLACED by a stub under the PDB name (bodiless class plus an out-of-line
    definition with the old effect).
  - **a3 (00468ee2, 007e8140, 007f81c0, 008d69f0, 008d6a30):** the stand-in stub is KEPT and a PDB-named stub forwards to it.
    q2 says "renamed": please rule whether the forwarding form is acceptable. Converting these to the a1 form is mechanical.
  - Tests PASS and name_gate --test is NO_WORSE in both forms. The RESULTS reasons of these 7 rows say so.

## Gates (every ACCEPT row; evidence in batches/<b>/evidence)
- **vland:** HEAD and new were run at one LANDVERIFY_WORK path (runpair.sh). Grades are equal and PASS. Tests PASS. LITERAL 0. The only
  new warn kind is SHARED_HEADER_AVAILABLE, under r3. RELOC_NAME warns removed: 50 / 24 / 22 / 23 / 20 / 0.
- **objgate (gate438):** ALL_EQ. Every non-debug section is equal under the rename, the DEF set is equal, the UND delta is exactly
  old -> new, and .drectve is equal.
- **name_gate HEAD v new with --test (namegate438):** NO_WORSE on every ACCEPT row. Two rows were WORSE and are held.
- **pk (pkgate438):** 0 new NEQ and copy counts unchanged.
- **/FORCE link (linkgate438, definers = HEAD objects of the target self rows):** duplicates do not go up (2->2, 6->6, 5->5, 4->4, 0->0,
  0->0), NEW_UNRESOLVED is 0, and every new name is DEFINED.
- **xrcheck438 + xr2_438:** every renamed callee resolves to its target. 0x00BC5AE0 was re-proven by xr2, because nf.py picked the wrong
  leaf COMDAT. The a5 xrcheck evidence was run on the pre-split 49 rows, a superset of the ACCEPT rows.
- **Table (tools/tablefix443.py):** every delete and rewitness was decided from COMPILED HEAD objects (census439 resolves each
  candidate's leaf relocations against retail). The candidates are every landed row that names the old identifier and whose retail bytes
  reference the target. A delete is witnessed by a same-batch ACCEPT row; a rewitness goes to CROSSBATCH.tsv.
  apply_table --dry on TABLE_CHANGES + CROSSBATCH of each batch: 0 misses, multi-target 1 -> 1, no new multi-target symbol.
- **reserved_check (RESERVED_REFS=""):** 0 over rows and targets. Its MENTIONS lines name reserved addresses already present in the
  HEAD text; the edits add none. a5's first run hit target 0x009CFBC0, which is in codex_pass26 RESERVED. Its row 0x009D0380 was moved to
  a5_hold before the final finalize.
- **screen438:** no row or target whose newest claim is Account B. T3 col 2/3 rows were skipped entirely.
- **SELF_SYMBOLS.tsv:** five columns, `address, symbol, status, leaf_given, current_line`.
  - The status is the new vland grade, RELOCATION_MATCH everywhere, and equal to the current line wherever one exists.
  - 50 rows have no self_symbols line yet. For those, symbol = leaf_given = the row's `// Leaf:` (a full mangled name) and current_line is empty.
- **orig/:** rows and tests re-diffed against the repo at hand-back: 0 drift.
- **Re-screened at hand-back (HEAD moved mid-run):**
  - apply_table --dry per batch and once over all six batches' TABLE_CHANGES + CROSSBATCH: delete 161, rewitness 6, add 51; 0 misses;
    multi-target 1 -> 1. No new symbol maps to two targets across the batches.
  - reserved_check 0 for every batch; no newest Account B claim; no registry hit (184 lines).
- **Evidence scope:** the objgate / pk / link / vland evidence of a2, a4 and a5 was run on the pre-split superset (the HOLD rows
  included). a5's xrcheck.txt holds OK for all 48 ACCEPT renames.
- **New self lines with a pre-existing LEAF_MISMATCH.** Four rows already show the mismatch at HEAD (name_gate's catalog expectation v the
  row's Leaf): 0089d970, 0089bcd0, 0089bd90 (QBE v the Leaf's UBE) and 009d1320 (CBankFile_Retail v the Leaf's CBankFile).
  - Their SELF_SYMBOLS line carries the row's Leaf, the symbol vland graded RELOCATION_MATCH.
  - The mismatch is identical at HEAD (self debt, not touched).
- **Mutation control:** none. The leaf bytes equal HEAD, so NF419 r1 applies.

## Names
- Every new callee name is in both ghidra_out PDB name sets, and it equals the target's tracker `self_leaf` (the landed self Leaf). The
  pre-screen dropped 1 row: 0x00A86B30 -> 0x00A868C0 (the PDB spelling is not in both sets or differs from the self Leaf). It is residue.
- No target is in rebuild/oracles/descriptive_names.tsv. No new symbol is registered there at another address, so REGISTRY_CORRECTIONS.tsv is empty.
- None of the GameCODA render ctors (nf441 hold list) or the NF444 rows appear: every row and target is below 0x00C00000.
- No BEST_GUESS names. NAME_FIXES.tsv is empty.
- **Identity note for the catalog:** `?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ` is the self of 0x00789CC0 (a T1 target).
  The table also binds it at 0x007CBF50, witnessed by 0x007BDCFC, while 0x007CBF50's self is `?GetDamageMultiplier@@YIMPAUS@@@Z` (T3).
  vland flags RELOC_CONFLICT, so row 0x00698D10 is held. Which address owns the PDB name needs an identity check.

## Held (batches/*_hold, RESULTS.tsv reasons)
- **a1:** 00689df0 (objgate NOT_EQ: a ProcessInput COMDAT relocation) and 008c6b20 (SetValue(long) -> SetLifeTimeSeconds(float)
  converts the argument; DIFFER).
- **a2:** 00b2c4e0 (objgate NOT_EQ: CCountedPointer<CBankStateBlock> struct v class key).
- **a3:** 004fe510 (ApplyScriptedMapBrush long -> bool converts the argument; DIFFER).
- **a4:** 00698d10 (RELOC_CONFLICT, above), 00891220 (DIFFER), and 00747660 / 007ebbc0 (name_gate WORSE: the bodiless class adds a
  SHARED_HEADER_AVAILABLE FAIL and the stand-in removed none).
- **a5:** 00985420 (DIFFER), 008a1cc0 / 00aeefe0 (name_gate WORSE), and 009d0380 (reserved target).

## Residue (RESIDUE.tsv: target, wrong_symbol, row, reason; 1,136 lines)
- **Targets with no actionable caller.**
  - 34 targets: no caller found.
  - 65 targets: census rows exist but none binds a wrong spelling under libclang.
  - 13 big-family targets and 3 locator-crash rows: not examined.
- **0x00691680, one of the five r1 rows.** a1 edits it for a different callee (AddData). Its r1 respell is still open and must build on
  a1's text.
- **281 row screens.** 248 T3 col 2/3 lines (skipped; only a callee-only respell where the T3 line is about the row's own self would be
  allowed, NF423 r1, not done), 31 libclang parse errors at HEAD, 2 rows in a pending RESERVED.tsv.
- **218 special member / operator.** Mostly ctor forms. CCombatSequenceBase is done in c1; the FindMostRecentMessage<...> member template is
  26 lines; the CEnginePrimitiveManager ctor is 8.
- **115 call-site shapes.** A pointer->reference or template-argument parameter, or a callee that is not `B->M`. The largest group is 28
  lines of `CCountedPointer const& -> CCombatAction&`.
- **46 generated rows fail the clang precheck.** Incomplete type, unknown type, or conversion.
- **38 parameter count differs.** SIG.
- **Others:** model-scope issues (`class X defined in the row more than once`), virtual on the row's own model (12, the VIRTUAL group),
  and stand-in bodies.
- **25 generated rows not gated.** work/g4_bin_5.txt: 12 rows, clang-clean, ready for a batch; plus 1 row locked elsewhere (00960760).
- **13 c1 leftover rows.**
- **Not attempted:** the 5 NF439 r1 rows through the header; the 12 big-family targets.

## Tools (tools/)
- **Copied from NF438:** rebind443.py (rebind438 + friend / static / const-away), census438, cfilt438, ast438, mkb438 (BNAME, lock
  claim), finalize438 (five-column SELF_SYMBOLS), gate438, pkgate438, linkgate438, namegate438, split438, payfilter438, xrcheck438,
  xr2_438, screen438, release438.sh.
- **New:** pack443.py (bins whole target components, at most 50 rows per batch), tablefix443.py (compiled-census table decisions,
  CROSSBATCH), run443.sh (definers + runpair + gates), fin443.sh (finalize chain), gsum.sh (gate summary).
- **tools439/:** census439.py, which writes a SEEN line per compiled row.

## Resume note
1. Batch work/g4_bin_5.txt (12 rows in work/g4_rows) and work/c1_left.txt (13 rows in work/cs_rows) as is:
   `BNAME=x python3 tools/mkb438.py <plan> <rows dir> x <list>`, then run443.sh, gsum.sh and fin443.sh.
2. Next generator steps:
   - the 46 clang failures (forward declarations for nested and template types);
   - `CCountedPointer const& -> X&` arguments;
   - FindMostRecentMessage<> member templates;
   - other ctor families in the same model-edit form as c1.
3. HEAD moved during this run (another landing deleted a file that tablefix443 read; the tool now skips missing files). Re-diff orig/
   before landing.
