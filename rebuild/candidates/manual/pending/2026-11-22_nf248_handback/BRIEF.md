# Name-fix agent NF248 (2026-11-22, round 17): NF241's 39 open CODEXP27 lines, 0x0049D800, 0x01296F14

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF247 (`$S/w22/agentNF247/`: the 16 CModeBase slot bindings, the 0x006A1620 binders, the held p27 lines,
0x00418DCA) and you. No authoring agent runs. Off limits: NF247's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is
not `released` or `NF248` (write NF248 there first), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, 0x00B260B0 / 0x00B261D0, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF246's with overrides
Read `$S/w22/agentNF246/BRIEF.md` and the briefs it chains to (NF245, NF243, NF241 ... NF215 "Lane", `$S/w22/agentNF248/NF214_BRIEF.md`).
Paths -> `$S/w22/agentNF248`, names -> `NF248`. On a conflict: this file > NF246 > earlier. nf246 LANDED
(`batches/2026-11-22_landing_nf246/README.md`, whose rulings you work under). NF246's entry 1 is DONE; yours is its entry 2. Start from
the repo tree now. Never write into /home/user/FableDecomp: vland / equiv_check need it as cwd, so list the root before and after and leave
nothing there; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, and NEVER any
foreground sleep (NF244 and NF246 each slipped once). Never `rm -rf` a variable path: use tools/clean.py. Every grade under `timeout 900`.
Disk is tight. Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check and recheck every
differing row. Nothing meant for the repo carries retail byte dumps: one verdict line per DIFFER.

## Entries, in this order
1. `QUEUE_LINES.tsv` (39 lines, NF241's open ones, in its order; nf241 hand-back `pending/2026-11-22_nf241_handback/` has their context).
2. 0x0049D800 (CWorld.h GetLocalSeed).  3. 0x01296F14 (the ScreamerIntoAggressive vtable with its slot rows 0x0096DBA0 / 0x0096DBB0 /
   0x0096E3F0).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF246 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if your context gets heavy.
