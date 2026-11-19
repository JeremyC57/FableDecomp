# Catalog CompilerFlags repair (2026-11-19)

Found while repairing stale pass patterns: 0x004BA180 rechecked DIFFER(279v282) with its test passing, and graded RELOCATION_MATCH with `/Gr`. The real build
(rebuild/build_candidates.ps1) takes flags from the catalog entry's CompilerFlags, not from the row source's `// Flags:` line, and 489 landed entries lacked
flags their source declares. A recheck of all 489 with the catalog as it stood: 102 still match (the flag is not byte-relevant for them), 387 DIFFER.
Fix: the source's `// Flags:` tokens appended to those 387 entries' CompilerFlags (base `/O2 /Oy /W3` kept): 341 x /Gr, 38 x /d2inlS20, 2 x /d2inlS22, and single
/d2inlT59 / T65 / T66 / T76 / S60 / /Oa entries (`fixed_entries.tsv`: address, source Flags line, new CompilerFlags). Recheck after the fix: 387 / 387
MATCH / RELOCATION_MATCH + PASS (`recheck_good.txt`). This also explains the CResponseTimes fill / uninitialized_copy / uninitialized_fill_n DIFFERs seen
in the hand4b b6 recheck and the 7 heap / sort DIFFERs in the pass-pattern repair.
