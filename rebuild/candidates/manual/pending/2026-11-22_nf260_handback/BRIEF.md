# Name-fix agent NF260 (2026-11-22, round 23): the L129 GPLandscapeRenderer family, then 0x009B0AC0

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF259 (`$S/w22/agentNF259/`: CCombatActionBase-family slot self rows, the CCreatureAction port debt, 0x00418DCA)
and you. No authoring agent runs. Off limits: NF259's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase
8-hex names, no 0x) is not `released` or `NF260` (write NF260 there first, before ANY compile of that row, controls included), the 25 Codex
pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`,
`$S/w22/c2t*/`.

## Your brief is NF258's with overrides
Read `$S/w22/agentNF258/BRIEF.md` and the briefs it chains to (NF256, NF254, NF252, NF250, NF248, NF246, NF245, NF243, NF241 ... NF215
"Lane", `$S/w22/agentNF260/NF214_BRIEF.md` and its read-first list, in full, GOTCHAS / EXTRA_RULES included). Paths -> `$S/w22/agentNF260`,
names -> `NF260`. On a conflict: this file > NF258 > earlier. nf258 and nf256 LANDED (`batches/2026-11-22_landing_nf258/README.md`,
`.../2026-11-22_landing_nf256/README.md`, whose rulings you work under; nf256 NOTES has the GPLandscapeRenderer address proof and the
family plan). Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID
(kill -0) in a later tool call, never pgrep -f, NEVER any foreground sleep; a grade runs only in a later call than its passing clang
precheck (a clang failure that a two-line VC7.1-header control reproduces is clang's, and may be noted and graded), after the lock is
yours, under an outer `timeout 900`; never `rm -rf` a variable path (tools/clean.py). Every grade with the row's CompilerFlags. Disk is
tight (under 3 GB). Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check and recheck
every differing row. No retail byte dumps in hand-back files.

## Entries, in this order
1. **L129 (0x01436EA8, `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`, proven by nf256):** today 18 table
   spellings, 91 referencers. Census first (group the referencers by the spelling they use and the header that would carry the right one),
   then land in units, each leaving every address it touches on fewer spellings and never splitting one; hand back after any finished unit.
2. 0x009B0AC0 (the last L37 binder): port onto CDefinitionManager.h's CDefClassInfo as its landed siblings do (nf258 NOTES), then 0x00535470
   2 -> 1.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF258 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if
your context gets heavy.
