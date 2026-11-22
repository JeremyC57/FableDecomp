# Name-fix agent NF251 (2026-11-22, round 18b): land the held CEngine pair, its CIEngine stand-ins, the 0.0 constant, 0x00890C80

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF250 (`$S/w22/agentNF250/`: NF241's remaining CODEXP27 lines, 0x0049D800, the 0x01296F14 unit) and you. No
authoring agent runs. Off limits: NF250's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex
names, no 0x) is not `released` or `NF251` (write NF251 there first, BEFORE grading), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF249's with overrides
Read `$S/w22/agentNF249/BRIEF.md` and the briefs it chains to (NF247, NF244, NF242, NF238 ... NF215 "Lane", `$S/w22/agentNF251/NF214_BRIEF.md`
and its read-first list, in full). Paths -> `$S/w22/agentNF251`, names -> `NF251`. On a conflict: this file > NF249 > earlier. nf249 LANDED
(`batches/2026-11-22_landing_nf249/README.md`, whose rulings you work under; hand-back `pending/2026-11-22_nf249_handback/`, with the held
CEngine pair drafts in rows/ and the clash link in lk3.txt). For the nf159 TU-emitter rule, grep the repo docs and batches for "TU-emitter"
/ "nf159" first. Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by
PID (kill -0) in a later tool call, never pgrep -f, NEVER any foreground sleep; never launch a grade whose clang precheck failed; never
`rm -rf` a variable path (tools/clean.py). Every grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under
an opt-in macro; prove header changes with equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries (ENTRIES.tsv), in this order
1. q12700 (hand back after it if it is large).  2. q12701.  3. q12702.  4. q12703.  5. q12704.  6. q12659 only with the full proof.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF249 file set and the decisions you need; hand back early if your context gets heavy.
