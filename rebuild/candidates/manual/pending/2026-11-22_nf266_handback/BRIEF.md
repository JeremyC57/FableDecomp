# Name-fix agent NF266 (2026-11-22, round 26b): the four UBE AllowsCombinationAnims binders, CastAssassinRush, the map dtor mirror, 0x00418DCA

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF265 (`$S/w22/agentNF265/`: the rest of L129 GPLandscapeRenderer, then 0x009B0AC0) and you. No authoring agent
runs. Off limits: NF265's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not
`released` or `NF266` (write NF266 there first, before ANY compile of that row, controls included; a read-only clang census does not need
locks but must not leave files outside your folder), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF264's with overrides
Read `$S/w22/agentNF264/BRIEF.md` and the briefs it chains to (NF261, NF259, NF257, NF255, NF253, NF251, NF249, NF247, NF244, NF242, NF238 ...
NF215 "Lane", `$S/w22/agentNF266/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF266`, names -> `NF266`. On a
conflict: this file > NF264 > earlier. nf264 LANDED (`batches/2026-11-22_landing_nf264/README.md`, whose rulings you work under; hand-back
`pending/2026-11-22_nf264_handback/`: NOTES has the slot-66 census method; nf261 NOTES entries 4 / 5 scope q12743 / q12744). Start from the
repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool
call, never pgrep -f, NEVER any sleep; a grade runs only in a later call than its passing clang precheck, after the lock is yours, with
`timeout 900` on the launch line (equiv_check may take `timeout 2700`); never `rm -rf` a variable path (tools/clean.py). Keep every scratch
file inside `$S/w22/agentNF266`. Disk is tight (under 3 GB). Every edit to an existing header line goes under an opt-in macro; prove header
changes with equiv_check (wait for it) and recheck every differing row. No retail byte dumps in hand-back files.

## Entries (ENTRIES.tsv), in this order
1. q12760.  2. q12743.  3. q12744.  4. q12659 only with the full proof.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck / vtmap every emitted vtable, link-check (no
slot may resolve to a wrong body), TABLE_CHANGES.tsv. Hand back with the NF264 file set and the decisions you need; hand back early if
your context gets heavy.
