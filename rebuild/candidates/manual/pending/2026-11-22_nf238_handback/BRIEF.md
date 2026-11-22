# Name-fix agent NF238 (2026-11-22, round 12): the nf236 follow-ups, the CEngine ctor / dtor unit, CMainGameComponent

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF237 (`$S/w22/agentNF237/`: the land1122p27 / land1122auth24 NAME_FIXES in its QUEUE_LINES.tsv) and you. No
authoring agent runs. Off limits: NF237's queue lines and every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is not `released`
or `NF238` (write NF238 there before you touch a row), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF236's with overrides
Read `$S/w22/agentNF236/BRIEF.md` and the briefs it chains to (NF235 ... NF215 "Lane", `$S/w22/agentNF238/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF238`, names -> `NF238`. On a conflict: this file > NF236 > earlier. nf236 LANDED (`batches/2026-11-22_landing_nf236/README.md`,
whose rulings you work under; its hand-back `pending/2026-11-22_nf236_handback/`, NOTES entry 5 = the held-line blockers). Start from the
repo tree now. Never write into /home/user/FableDecomp (run tools with a scratch cwd: a tool once left a log in the repo root); never run
git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade under `timeout 900`. Disk is tight (about 3 GB free).
Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check (two include-tree copies) and
recheck every differing row.

## Entries (ENTRIES.tsv), in this order
1. q12668: the 29 held p27 lines, as many as fit.  2. q12669.  3. q12670.  4. q12671.
5. q12672: CEngine ctor + dtor as one unit (header opt-in, full proof).  6. q12659: only with the full proof, else leave it.
For every entry: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF236 file set and the decisions you need.
