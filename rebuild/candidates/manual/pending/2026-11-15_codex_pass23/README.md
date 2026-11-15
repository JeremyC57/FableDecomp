# Codex pass 23 reservation (2026-11-15, owner): 100 rows handed to Codex

Owner: "can i get a new set of functions / rows for codex to work on along with updated headers and documentation".
`RESERVED.tsv` lists them; they are off-limits to every other lane until it is renamed `RELEASED.tsv`. None overlaps the acctC
reservation (`../2026-11-13_acctC/RESERVED.tsv`) or the three pass-22 rows still being worked in house (0x00F3E120, 0x0064BFBA, 0x00675E20).

| kind | rows | what they are |
|---|---:|---|
| test_port | 11 | Codex's own pass-22 RELOCATION_MATCH rows whose tests needed a per-test `test_include/` directory (`../2026-11-14_codex_pass22/return/matched_test_port/`) |
| parked | 39 | the largest same-size parked near misses (808-1,085 bytes) that no Codex pass has had and nobody else holds |
| never_worked | 50 | every remaining never-worked row: `Unwind@0121xxxx` EH funclets (8-32 bytes); the package's `FUNCLET_PARENTS.tsv` names each parent (11 landed, 35 parked, 3 with no unwind entry) |

One funclet whose parent is in the acctC reservation (0x012133C0, parent 0x00C271F0) was swapped for the next parked row (0x007BCF7F).

## The package
`build.py` builds `fabledecomp_codex_pass23_2026-11-15.zip` in the scratch area (not committed; about 25 MB). It reads `rows.tsv`
(the fixed selection) and needs `FABLE_EXE` only to measure extents, find callers and resolve funclet parents (`tools/eh_parent.py`:
unwind-map entry -> FuncInfo -> handler thunk -> the prologue that pushes it). It writes no retail bytes. Same layout as pass 22
(`../2026-11-14_codex_pass22/README.md`) plus `FUNCLET_PARENTS.tsv`, each test_port row's returned pass-22 folder (`codex_pass22/`),
the pass-22 return and its in-house follow-ups in `lessons/codex/`, and the landings through 2026-11-15 in `lessons/landings/`.
`README_CODEX.md` "Since pass 22" turns the pass-22 landing problems into hard rules: self-contained tests (no test_include/), every
needed header change in hdr/, checked assert pins, `// Retail-asm:` lines with a `0x` address and an instruction, mangled Leaf lines.

## 2026-11-15: worked in house
The owner could not run Codex on this package ("i dont have that exe so you will have to continue forward. try hand authoring
as well as your usual tools"). The reservation stays in force, now for in-house agents: w23T (the 11 test ports), w23F (the 49
EH funclets) and w23P1 / w23P2 (20 parked rows each, hand-authored). Matches land as ordinary batches; rows still open at the
end are released.
