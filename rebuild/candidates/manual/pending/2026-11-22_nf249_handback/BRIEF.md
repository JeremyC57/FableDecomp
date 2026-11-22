# Name-fix agent NF249 (2026-11-22, round 17b): UnsheatheWeapon to protected, 0x00890D80, the CEngine ctor / dtor (step 3), 0x00418DCA

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF248 (`$S/w22/agentNF248/`: NF241's 39 open CODEXP27 lines, 0x0049D800, 0x01296F14) and you. No authoring agent
runs. Off limits: NF248's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex directory names, no
0x) is not `released` or `NF249` (write NF249 there first), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, 0x00890250 (authoring), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF247's with overrides
Read `$S/w22/agentNF247/BRIEF.md` and the briefs it chains to (NF244, NF242, NF238 ... NF215 "Lane", `$S/w22/agentNF249/NF214_BRIEF.md`,
INCLUDING NF214's read-first list: NF247 skipped it). Paths -> `$S/w22/agentNF249`, names -> `NF249`. On a conflict: this file > NF247 >
earlier. nf247 and nf244 LANDED (`batches/2026-11-22_landing_nf247/README.md`, `.../2026-11-22_landing_nf244/README.md`, whose rulings you
work under; hand-backs `pending/2026-11-22_nf247_handback/`, `pending/2026-11-22_nf244_handback/`). Start from the repo tree now. Never
write into /home/user/FableDecomp: vland / equiv_check need it as cwd, so list the root before and after and leave nothing there; never run
git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, and NEVER any foreground sleep. Never
`rm -rf` a variable path: use tools/clean.py. Every grade under `timeout 900`. Disk is tight (about 3 GB). Every edit to an existing
header line goes under an opt-in macro; prove header changes with equiv_check and recheck every differing row. Nothing meant for the
repo carries retail byte dumps: one verdict line per DIFFER.

## Entries (ENTRIES.tsv), in this order
1. q12690.  2. q12691.  3. q12692 (hand back after the header if large).  4. q12659 only with the full proof.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF247 file set and the decisions you need; hand back early if your context gets heavy.
