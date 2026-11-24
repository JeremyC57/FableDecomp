# Name-fix agent NF262 (2026-11-22, round 24): the rest of the L129 GPLandscapeRenderer family, then 0x009B0AC0

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF261 (`$S/w22/agentNF261/`: seven CCombatActionBase slot rows 0x0062EA40-0x0062EAB0, 0x00692890, the pooled 0.5f
0x0122F59C, the CastAssassinRush models 0x0072CAA0 / 0x008CD470, 0x00AD9770 / 0x00AD9980, 0x00418DCA) and you. No authoring agent runs.
Off limits: NF261's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not
`released` or `NF262` (write NF262 there first, before ANY compile of that row, controls included), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF260's with overrides
Read `$S/w22/agentNF260/BRIEF.md` and the briefs it chains to (NF258, NF256, NF254, NF252, NF250, NF248, NF246, NF245, NF243, NF241 ...
NF215 "Lane", `$S/w22/agentNF262/NF214_BRIEF.md` and its read-first list, in full, GOTCHAS end to end). Paths -> `$S/w22/agentNF262`, names
-> `NF262`. On a conflict: this file > NF260 > earlier. nf260 LANDED (`batches/2026-11-22_landing_nf260/README.md`, whose rulings you work
under; hand-back `pending/2026-11-22_nf260_handback/`: NOTES has the 19-spelling census and the order of the remaining units, NAME_FIXES
lists every remaining spelling with its rows). The header opt-in `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER` is landed.
Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a
later tool call, never pgrep -f, NEVER any foreground sleep; a grade (HEAD tests included) runs only in a later call than its passing clang
precheck (a clang failure that a two-line VC7.1-header control reproduces is clang's: note it and grade), after the lock is yours, under an
outer `timeout 900`; never `rm -rf` a variable path (tools/clean.py). Disk is tight (under 3 GB). Every edit to an existing header line goes
under an opt-in macro; prove header changes with equiv_check (wait for it) and recheck every differing row. No retail byte dumps.

## Entries, in this order
1. **L129 remaining:** the 16 stand-in spellings on 29 rows, ported onto engine/CEngineLandscapeRenderer.h's members (MemoryManager +0x6B0,
   GetSettings, ...) in the nf260 NOTES order, each unit leaving every address it touches on fewer spellings, never a split; also
   `?g_obj_01436ea8@@3PAUCGlobal@@A` (0x00BE5C30, untabled). Hand back after any finished unit.
2. **0x009B0AC0** (nf260 NOTES plan): prove the 0x009B9490 sort spelling first; then a CDefinitionManager.h opt-in typing `DefClassInfo`
   (the existing line `unsigned char NameCRCToIndexMap[16],DefClassInfo[16];` is split only under the opt-in, equiv_check proof), then
   the port; 0x00535470 2 -> 1 and the counts at 0x009B9170 / 0x009B9490 / 0x009AD2E0 fall.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF260 file set and the decisions you need; hand back early if your context gets heavy.
