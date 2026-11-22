# Name-fix agent NF255 (2026-11-22, round 20b): the four blocked CCreatureAction ctor rows (one class per pass), 0x0096B600, 0x00418DCA

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF254 (`$S/w22/agentNF254/`: L4 / L5 + 0x0074FCE0 family, L28, L57, L111, L114, L120, the L37 / L129 plans) and
you. No authoring agent runs. Off limits: NF254's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase
8-hex names, no 0x) is not `released` or `NF255` (write NF255 there first, before ANY compile of that row, controls included), the 25
Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held),
`$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF253's with overrides
Read `$S/w22/agentNF253/BRIEF.md` and the briefs it chains to (NF251, NF249, NF247, NF244, NF242, NF238 ... NF215 "Lane",
`$S/w22/agentNF255/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF255`, names -> `NF255`. On a conflict: this
file > NF253 > earlier. nf253 LANDED (`batches/2026-11-22_landing_nf253/README.md`, whose rulings you work under; hand-back
`pending/2026-11-22_nf253_handback/`: NOTES entry 4 and evidence/e4 are the slot map you start from; the drafts are in
`pending/2026-11-22_nf250_handback/blocked/`). Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even
read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, NEVER any foreground sleep; NEVER compile a row before its
lock is yours and its clang precheck passed (NF253 slipped on both); never `rm -rf` a variable path (tools/clean.py). Every grade under
`timeout 900`, with the row's CompilerFlags from build_candidates.ps1. Disk is tight. Every edit to an existing header line goes under an
opt-in macro; prove header changes with equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries (ENTRIES.tsv), in this order, one class per pass
1. q12720 (L92 first).  2. q12721.  3. q12722.  4. q12723.  5. q12724.  6. q12659 only with the full proof.
Each class is a unit: its ctor, every Clone / stand-in row whose spelling would split, the vtable vtcheck'd slot for slot against retail.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv. Hand back with the
NF253 file set and the decisions you need; hand back early (after any finished class) if your context gets heavy.
