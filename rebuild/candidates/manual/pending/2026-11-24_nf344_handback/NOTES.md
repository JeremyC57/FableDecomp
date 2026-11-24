# NF344 notes (Account A naming queue, 2026-11-24)

Scope: item 1 = the 29 /d2inl rows of the NF340 audit (`naming/audits/2026-11-24_pk_all_nf340/SUMMARY.md`, "/d2inl control");
item 2 = leaf-vs-table census (no compile).

## Ownership / reservation notes (decisions, not silent)
- 0x00D45060, 0x00CE7670: lock held by NF343 -> skipped, not compiled (STATUS `skipped-held`).
- 0x00DE8370, 0x00EFA4F0: the newest COORDINATION_CLAIMS line is Account B `claimed` (2026-11-20, author). Measured read-only; no
  setting helped, so nothing is proposed for them.
- 0x0064B1AC, 0x0091F140, 0x00E4FA20: still listed `reserved (Codex pass 27)` in pending/2026-11-22_codex_pass27/RESERVED.tsv, although
  landing p27 landed all 100 rows. 0x0064B1AC and 0x00E4FA20 carry a proposed change: **D1 below**.
- 0x009C4440 (T3 nf300 port) and 0x00BD9CE0 (T3 nf303 census port) are column-2 T3 addresses; my BRIEF prefix sends me to them for the
  flag only. Neither gets a change. 0x00AFCC30 / 0x00B03CA0 / 0x00B1F200 are column-2 T3 `pickany_d2inlS15` debt lines (prefix sends me):
  0x00AFCC30 / 0x00B03CA0 get a change, but the debt is NOT fixed (resize 0x00AFE910 stays NEQ), so the T3 line should stay open:
  land_scripted will hold them as T3 (see "How to land"). 0x00B2C8E0 is named in T3 nf283's column 3 (as a binder), which land_scripted
  also treats as reserved.

## Method (item 1)
- Precheck: clang `-fsyntax-only` (tools/pre.sh) of all 29 sources and tests (work/pre_all.log); every clang failure was in toolchain
  headers / `_assert`, MSVC-only syntax in an unchanged test, or (0x007677E0's source) in shared header engine/CTCObjectAugmentations.h:107
  (`explicit instantiation cannot be 'inline'`); those passed VC7.1 `cl /Zs` in an earlier call (D1 / NF300 D1 ext., work/zs_pre1.log).
  The changed 0x007677E0 source fails clang with the same error as its HEAD source, then passed cl /Zs (NF305 Q2, zs_b1.log).
- Lock, then compile. tools/flagrun.py compiles one landed row exactly as recheck.py + vland --recheck-landed (catalog flags, /Oy- rule,
  Leaf, 180 s tool limit, no sweep) under a flag variant, at ONE path per row (work/obj/<addr>.cpp, so `?A0x` hashes agree), runs
  pk_all.check (copy addresses from source a / a2 plus, always, source o = NF340's flagged-run addresses) and pickany_vs_retail.py, keeps the
  object. Every compile had its own `timeout 900` line (runA.sh / runB.sh / runC.sh). 27 rows x (catalog, no-/d2inl) + 77 + 12 sweep variants.
- tools/gatecmp.py applies the NF340 gate per variant against the catalog build: BROKE (leaf-called = leafref or reach; NEQ that was EQ or
  absent at HEAD), REPAIR (leaf-called NEQ -> EQ), DEBT (NEQ both). Keys are symbols with `?A0x` stripped, from BOTH sides.
- tools/objfn.py diffs two objects function by function (bytes + relocation lists, non-code sections), to tell a no-op flag from one that
  changes code. tools/findcopy.py searches Fable.exe (relocation fields wildcarded) for a NOADDR copy's body.
- Sweep sets (evidence for "other settings"): the documented match windows in each row's own Inline-budget line, a few neighbours, each flag
  alone for two-flag rows, and the TU-twin's flags (0x00B1F200 v 0x00AFCC30's /d2inlT1537 /d2inlS15). No open-ended fitting: 3-8 values a row.
- Final check of every change: vland --recheck-landed on HEAD and on the new payload (flags edited, new source) - all RELOCATION_MATCH
  PASS, no sweep tag, identical RELOC warnings; and the new source's object is function-for-function identical to the measured variant.

## Results (item 1): FLAG_CHANGES.tsv (top level; subsets in each run dir)
Run dirs: **batches/b1** (no decision: 0x00B47BC0, 0x00E4AA00, 0x00B2C8E0, 0x00AFCC30, 0x00B03CA0), **batches/b2** (D1: 0x0064B1AC,
0x00E4FA20), **batches/b3** (0x007677E0, recommended, see R1). Each has rows/, orig/ (rows + tests), head/payload.json + HEAD_FROM_REPO.txt,
RESULTS.tsv, rows.txt, an empty TABLE_CHANGES.tsv and its FLAG_CHANGES.tsv subset. No tests or headers change.
| row | old flags | new flags | why |
|---|---|---|---|
| 0x007677E0 | /d2inlT59 | /d2inlT60 | leaf still RELOCATION_MATCH; repairs leaf-reached copies 0x007696E0 / 0x007698C0 (__uninitialized_copy<AttackParticleAndEndPoints>); no copy broken. T58 and below DIFFER, T61 also changes `??E` _Rb_tree_iterator, T62 / T64 DIFFER. Side effect: NOADDR __uninitialized_fill_n<AttackParticleAndEndPoints> 45 B -> 113 B; neither form is in Fable.exe (masked search). |
| 0x00B47BC0 | /d2inlT29 | /d2inlT39 | repairs leaf-called 0x00B49DF0 (_M_insert_overflow<CManagerLocalLight*>); T29..T38 keep it NEQ, T39..T45 repair. Side effect: NOADDR `??2CManagerLocalLight` 12 B -> 51 B; the 51-B body occurs in Fable.exe at 0x00B45310 / 0x00B45350 / 0x00B45390 (beside the lighting manager; no catalog row yet) and as the CParticle-pool `operator new` siblings 0x00AF8680 / 0x00AF86C0 / 0x00AF8B00, the 12-B one only as generic forwarders. |
| 0x00AFCC30 | /d2inlT1537 /d2inlS15 | /d2inlT1537 | S15 is a no-op: the object is identical without it. resize 0x00AFE910 stays NEQ debt (only dropping T1537 repairs it, and that breaks the leaf). |
| 0x00B03CA0 | /d2inlT1537 /d2inlS15 | /d2inlT1537 | same as 0x00AFCC30. |
| 0x00E4AA00 | /d2inlT200 | (none) | the object is identical without the flag. |
| 0x00B2C8E0 | /d2inlS100 /d2inlT200 | (none) | leaf the same; the only change is the NOADDR vector<unsigned char>::resize(n) COMDAT, 94 B -> 25 B, and the 25-B body is retail's (0x00412880 vector<char>::resize, 0x007B9E50; the 94-B body occurs nowhere). |
| 0x0064B1AC | /Os /Oy- /d2inlT59 | /Os /Oy- | object identical without T59. **D1** (Codex p27 list). |
| 0x00E4FA20 | /d2inlT69 | (none) | object identical without the flag. **D1** (Codex p27 list). |

Kept (flag needed or no gain), see STATUS.tsv:
- the flag repairs a leaf-called copy: 0x005414A0 / 0x006116E0 / 0x007E4B60 (_M_insert_overflow 0x00485DBC), 0x0090E5D0
  (FindMostRecentMessage 0x0090EBE0), 0x00B3FC40 (PeekMapNormal 0x00B3F5E0), 0x0091F140 (GetAllThingsInRoughArea 0x00920200: the NF340
  control's '-' was only a missing address; without the flag the copy is NEQ);
- the flag repairs a non-leaf copy: 0x00BD9CE0 / 0x00BDA150 (FreeVirtual 0x00BD9B90; T114 is that owner's own flag);
- no gain: 0x006C6040 (copies' verdicts equal, but the drop turns the NOADDR ~list<CThingAICreature*> from retail's 62-B folded body
  0x004046B0 into a 5-B jmp), 0x00803D60 (both leaf-called NEQ copies stay NEQ).
- of the 15 flag-caused rows, no setting found keeps the leaf and repairs a copy for 0x00465C54 (S44..S60 repair ~_Rb_tree 0x00451B07 but
  break clear 0x004512DA), 0x0083F650, 0x008DDAD0, 0x009C4440, 0x00A71910, 0x00B1F200, 0x00B21E90, 0x00DE8370, 0x00EFA4F0: their copies
  stay per-copy header debt (T3 nf340 lines); 0x00CE7670 / 0x00D45060 not measured (NF343).

### Debt still NEQ in the changed rows (unchanged by the change, listed per the NF340 gate)
- 0x007677E0: 0x0042B248 GetSubDef<CAugmentationDef>, 0x0076BDD0 __destroy_aux, 0x0076B760 / 0x0076B850 / 0x0076C470 / 0x0076C590
  _M_insert_overflow (all NEQ at HEAD too).
- 0x00AFCC30 / 0x00B03CA0: 0x00440F00, 0x00AFE7C0 _M_fill_insert; 0x00AFE6F0 _M_insert_overflow; 0x00AFE910 resize.

## How to land (coordinator)
- The real build and the recheck read `CompilerFlags` from rebuild/build_candidates.ps1, not the `// Flags:` line (GOTCHAS 449 / 499).
  So: (1) edit each row's CompilerFlags to FLAG_CHANGES.tsv `catalog_flags_new` (drop the line where it says "remove"); (2) copy
  batches/b1/rows/<addr>.cpp over the landed source (comment lines only: `// Flags:` and the Inline-budget text, line count kept);
  (3) recheck. land_scripted.py with this run dir does (2)+(3) but NOT (1), so do (1) first or its recheck grades the old flags
  (harmless for the no-op rows, but the source would then disagree with the catalog).
- land_scripted holds 0x00AFCC30 / 0x00B03CA0 (T3 `pickany_d2inlS15` col 2) and 0x00B2C8E0 (T3 nf283 col 3); keep those T3 lines
  open (debt unchanged) and land the three by hand: CompilerFlags edit + copy rows/<addr>.cpp + recheck.py / vland (my vland logs:
  evidence/vland_b1*_new.log).
- Locks: I keep NF344 on the 8 changed rows until the landing (NF309 Q5); the other 19 are released.
- Wording: the two reworded Inline-budget lines list only the measured points (0x007677E0: T55..T58 DIFFER, T59 / T60 / T61 match,
  T62 / T64 DIFFER; 0x00B47BC0: T29..T32, T35..T40, T45 match, T39 the smallest that repairs); re-prechecked and vland PASS after the rewording.

## Decisions needed
- **D1** 0x0064B1AC / 0x00E4FA20: Codex pass 27's RESERVED.tsv still says `reserved`, but landing p27 landed all 100 rows. Is the list
  stale (land the no-op flag drops), or are the rows still Codex's (hold)?
- Flag drops that change no byte (0x0064B1AC, 0x00E4AA00, 0x00E4FA20, S15 on 0x00AFCC30 / 0x00B03CA0) follow the brief ("or the flag
  turns out to be unneeded"); not a decision.
- **R1 (recommendation, passes the gate as written)** 0x007677E0 T60 (batches/b3): no leaf-called copy breaks; the one NOADDR copy it
  changes (__uninitialized_fill_n<AttackParticleAndEndPoints>, 45 B -> 113 B, neither form in Fable.exe) is called only from
  _M_insert_overflow<AttackParticleAndEndPoints> 0x0076B760, itself an existing NEQ debt copy. Land it, or hold until that debt is fixed.
  (0x00B47BC0 T39's NOADDR side effect gives a retail-shaped body, so it is in b1.)
- Stale comment (not changed here): 0x00B1F200's Inline-budget line says "the twin 0x00AFCC30 carries /d2inlT1537 /d2inlS15".

## Item 2: leaf-vs-table census (LEAF_TABLE_CONFLICTS.tsv, tools/census_leaf.py, evidence/census_summary.txt)
- Run on the table and catalog as of 10:13 (after the nf341_b2x landing, which renamed 0x006EA430 to SetAttackerAsBeenStruck: that
  control is now gone, as expected; it reproduced on the earlier state).
- Leaf per row: the first `// Leaf:` line (what recheck.py uses) when it is an exact table symbol; otherwise the row's graded symbol in
  rebuild/oracles/self_symbols.tsv, if the row has no Leaf line or the graded symbol agrees with the Leaf text (same symbol, mangled
  prefix of it, or a bare name inside it). Exact after stripping `?A0x<hash>` and folding `??_E` onto `??_G`. No prefix guessing.
  Unresolved (no self_symbols entry, or inconsistent with the Leaf): 4,889 rows (evidence/census_summary.txt); 18,220 full mangled Leaves
  with no table line and no self symbol are simply unbound.
- Controls: 0x0090310E, 0x00CBD310 (no Leaf line: self_symbols), 0x00643D8C reproduce; 0x00464536 (fixed nf337 b3) and 0x006EA430 (fixed
  nf341 b2x) are gone.
- Result: **624 rows** whose own leaf is bound only at another address (ELSEWHERE): 121 by an exact Leaf line, 77 by self_symbols for a
  prefix / bare Leaf, 426 by self_symbols for a row with no Leaf line. No non-anonymous SPLIT (bound here and elsewhere). 26 lines are
  anonymous-namespace twins (ANON_*: another TU's instance; not conflicts).
- Cheap side check per line (no bytes written): catalog bodies compared raw and relocation-masked (capstone; absolute addresses and
  out-of-body branch targets masked), plus the bound address's own Leaf / graded symbol:
  - 140 folded / twin bodies (IDENTICAL or EQUAL_MASKED): one name may legitimately cover both; research which owns the name;
  - 335 "two landed rows define the same Leaf": both addresses grade the same symbol (stand-in names shared by several rows, e.g.
    `?BuildTreeArray@Tree@@`): identity claims, hold both as one unit (coordinator ruling 2026-11-24);
  - 143 "bound address is a landed row with another Leaf": the table line is likely stale or the binder wrong (e.g. 0x00643D8C
    AddHitByMelee -> 0x00437990, a landed row graded as a different function);
  - 6 plain research.
  None is settled by two sources here; every line is "research" with a hint. Not fixed (per brief).
