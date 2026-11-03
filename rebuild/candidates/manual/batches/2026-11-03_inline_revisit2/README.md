# Inline revisit 2 (2026-11-03): 20 more `Revisit: inline-depth` rows match pragma-free (NOT yet landed)

Read-only study over the 90 rows left after landing round d and the 7 rows chunks 728-730 parked on inline splits. Grades are
`vland.py --recheck-landed` on copied payloads against a full header mirror at tree 01a7b4a1a (vland/r1: WINS 14, vland/r2: WINS 6).
Sources: `out/<addr>.cpp` (finalized: marker dropped, or the line-2 note for per-exit wrapping). Per-row lever: `matches*.tsv`;
all 92 rows with best grade and attempts: `summary.tsv`; best parked drafts: `parked/`.

## To land (next landing round)
- 10 rows with the marker dropped: 0x00715110, 0x00D79D50, 0x00D7A360, 0x00EE7A20, 0x00D86C30, 0x009F7890, 0x00EF6FE0,
  0x0066BD00, 0x0066BDF0, 0x00BC9B80.
- 10 rows with per-exit `do { if (C) return; } while (0);` masks, line 2 `// Inline-split: exits wrapped individually; retail's construct is unproven`:
  0x00CE0920, 0x00CE0B00, 0x00CE0CE0, 0x00CF02A0, 0x00D151D0, 0x00DDE230, 0x00E02E50, 0x00E6B750, 0x00EC0BA0, 0x00DD6440.
- 0x009F7890 lands as `// Flags: /Od` (unoptimised STL TU; precedent 0x00D1EB80 `/Ob1`): its build_candidates.ps1 entry needs
  `CompilerFlags = '/O2 /Oy /W3 /Od'`. Alternative without a flag: `out/009f7890_pragma_only_alt.cpp` (`#pragma optimize("", off)`).
  Prefer the flag (a whole-TU property retail's build had), as for 0x00D1EB80.
- 0x0066BD00 / 0x0066BDF0 keep the existing `__declspec(noinline)` + `auto_inline(off)` stand-in for 0x0066BC10 the landed rows already had.
- Re-land each with repair_remove + vland --land; recheck tests; regrade on the then-current tree first (the mirror predates later landings).

## Remaining 70 (why)
Outer-expanded / nested-called splits that plain inline cannot reproduce (CSpawnedFunc1 member copy 0x004ABE90 in 0x00D16460,
0x00E0DAA0, 0x00F09420, 0x00F09530, 0x00E7BD60; UI Pointer ctor in 0x005518E0 / 0x00551BC0; predicate dtors); one cold-site call
(0x00CEF3B0 ~CTimer, maybe a per-function growth budget); layout residues (0x00D8F040, 0x00D73F40, 0x00EFAB40, 0x00DA68E0; the
dead-branch double terminate check rows 0x00D97170, 0x00E53D90, 0x00E36390, 0x00F09950, 3 bytes short); large rows with greedy masks only.
