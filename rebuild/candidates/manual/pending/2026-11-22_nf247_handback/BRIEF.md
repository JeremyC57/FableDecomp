# Name-fix agent NF247 (2026-11-22, round 16b): CModeBase slot bindings, the 0x006A1620 binders, the held p27 lines, 0x00418DCA

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF246 (`$S/w22/agentNF246/`: the wrong DTOR_OUTOFLINE list families 0x005127A0 / 0x004AB4F0 / 0x00685BC0, NF241's
open CODEXP27 lines, 0x0049D800, 0x01296F14) and you. No authoring agent runs. Off limits: NF246's entries, every row whose lock owner
(`$S/w22/nflocks/<addr>/owner`) is not `released` or `NF247` (write NF247 there first), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00B260B0 / 0x00B261D0 (queued header claim),
`$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF244's with overrides
Read `$S/w22/agentNF244/BRIEF.md` and the briefs it chains to (NF242, NF238 ... NF215 "Lane", `$S/w22/agentNF247/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF247`, names -> `NF247`. On a conflict: this file > NF244 > earlier. nf244 LANDED (`batches/2026-11-22_landing_nf244/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf244_handback/`). CIEngine.h slots 11-13 are now the PDB declarations by
default; CEngine.h has the add-only FABLE_RETAIL_CENGINE_OVERRIDES. NF244's entries 1-2 are DONE; yours are its entries 3-6 (ENTRIES.tsv).
Start from the repo tree now. Never write into /home/user/FableDecomp: vland / equiv_check need it as cwd, so list the root before and after
and leave nothing there; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f, and NEVER a foreground sleep
(not even one: NF244 slipped once; run the job in the background and check it with kill -0 in a later call). Never `rm -rf` a variable
path: use the guarded tools/clean.py. Every grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under an
opt-in macro; prove header changes with equiv_check and recheck every differing row.

## Entries (ENTRIES.tsv), in this order
1. q12682 (the 16 off-address CModeBase slot bindings).  2. q12679 (0x006A1620 blocked binders).  3. q12676 as blockers fall.
4. q12659 only with the full proof.
For every entry: recheck every changed row, pk.py every new copy, census spellings (NF242's `tools/vtmap.py` maps vtable slots onto retail
by RTTI), link-check, TABLE_CHANGES.tsv. Hand back with the NF244 file set and the decisions you need; hand back early if your context
gets heavy.
