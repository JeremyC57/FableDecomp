# Landing nf401 b1: 16 rows now grade on their own function (one `// Leaf:` line each)

NF388's q8 triage found that 17 landed rows grading DIFFER were grading artifacts:
- class B (14 rows) had no `// Leaf:` line, so vland graded the largest function in the object;
- class C (3 rows) had a leaf naming the wrong function.

NF401 gave each row the full mangled symbol its object defines for the function at the retail address. Each source differs from HEAD by that one comment line: 15 rows gain one, and 00c5eca0's `// Leaf: Kill` is replaced. No code bytes change.

Grades:
- 16 rows now pass: 3 MATCH and 13 RELOCATION_MATCH, tests PASS. This holds both normally and under LANDVERIFY_STRICT_LEAF=1 (NF401). The coordinator regraded from the repo after applying the changes; that pass also covers the 8 EH funclets that include 00c5eca0 and 00cac300. All 24 pass, with 0 LITERAL_DIFF (logs/recheck_from_repo.log).
- 009e15e0 is held: with the correct leaf it is a real DIFFER(99v99), now T3 nf401 0x009E15E0.

self_symbols: 16 lines are corrected to the function the object defines at the address (SELF_SYMBOLS_CORRECTIONS.tsv).
- Column 4 was stale on all 16.
- Column 2 changed on 9 rows. Before, the six IsThingAlive rows all named the inline helper `?IsThingAlive@@...`, a different function. 00c5eca0 named `?NotifyKilled`, 00aa9190 named `??1CChunkedFile`, and 00cac300 named `?Initialize`.
- Names that are still stand-ins, or that conflict with a reloc_bindings name, are queued as T3 nf401 identity lines (007fb120 vs `?IsTargetAlive@CTCKraken`, the five IsThingAlive rows, the CWound stand-in, the 00cac300 PAV/PAD spelling, and the ??1CCharString RELOC_CONFLICT in 004af220 / 0056c840). None of the chosen symbols is another address's self, apart from the EH funclets, which follow the existing convention.
