# Name-fix agent NF264 (2026-11-22, round 25b): the held AllowsCombinationAnims unit, CastAssassinRush, the map dtor mirror, 0x00418DCA

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF263 (`$S/w22/agentNF263/`: the rest of L129 GPLandscapeRenderer, then 0x009B0AC0) and you. No authoring agent
runs. Off limits: NF263's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not
`released` or `NF264` (write NF264 there first, before ANY compile of that row, controls included), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF261's with overrides
Read `$S/w22/agentNF261/BRIEF.md` and the briefs it chains to (NF259, NF257, NF255, NF253, NF251, NF249, NF247, NF244, NF242, NF238 ... NF215
"Lane", `$S/w22/agentNF264/NF214_BRIEF.md` and its read-first list, IN FULL: GOTCHAS and EXTRA_RULES without cutting long lines). Paths ->
`$S/w22/agentNF264`, names -> `NF264`. On a conflict: this file > NF261 > earlier. nf261 LANDED (`batches/2026-11-22_landing_nf261/README.md`,
whose rulings you work under, ruling 1 especially; hand-back `pending/2026-11-22_nf261_handback/`: NOTES entry 4 scopes q12743). Start from
the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool
call, never pgrep -f, NEVER any sleep; a grade (controls' HEAD tests included) runs only in a later call than its passing clang precheck,
after the lock is yours, under an outer `timeout 900`, with the lane env sourced; never `rm -rf` a variable path (tools/clean.py). Disk is
tight (under 3 GB). Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check (wait for it)
and recheck every differing row. No retail byte dumps in hand-back files.

## Entries (ENTRIES.tsv), in this order
1. q12750.  2. q12743.  3. q12744.  4. q12659 only with the full proof.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck / vtmap every emitted vtable, link-check
(a full link of the unit, so no slot resolves to a wrong body), TABLE_CHANGES.tsv. Hand back with the NF261 file set and the decisions you
need; hand back early if your context gets heavy.
