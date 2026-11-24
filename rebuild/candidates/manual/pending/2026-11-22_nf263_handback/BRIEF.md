# Name-fix agent NF263 (2026-11-22, round 25): the rest of L129 (GPLandscapeRenderer), then 0x009B0AC0

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF261 (`$S/w22/agentNF261/`: seven CCombatActionBase slot rows 0x0062EA40-0x0062EAB0, 0x00692890, the pooled 0.5f
0x0122F59C, the CastAssassinRush models, 0x00AD9770 / 0x00AD9980, 0x00418DCA) and you. No authoring agent runs. Off limits: NF261's
entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not `released` or `NF263` (write
NF263 there first, before ANY compile of that row, controls included), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF262's with overrides
Read `$S/w22/agentNF262/BRIEF.md` and the briefs it chains to (NF260, NF258, NF256, NF254, NF252, NF250, NF248, NF246, NF245, NF243, NF241
... NF215 "Lane", `$S/w22/agentNF263/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF263`, names -> `NF263`. On a
conflict: this file > NF262 > earlier. nf262 and nf260 LANDED (`batches/2026-11-22_landing_nf262/README.md`, `.../2026-11-22_landing_nf260/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf262_handback/`: NOTES has the plan for the 14 remaining spellings on 25 rows).
Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a
later tool call, never pgrep -f, NEVER any sleep in any call (not even `sleep 0`); a grade runs only in a later call than its passing clang
precheck (a clang failure a two-line VC7.1-header control reproduces is clang's: note and grade), after the lock is yours, under an outer
`timeout 900`; never `rm -rf` a variable path (tools/clean.py). Disk is tight (under 3 GB). Every edit to an existing header line goes
under an opt-in macro; prove header changes with equiv_check (wait for it) and recheck every differing row. No retail byte dumps.

## Entries, in this order
1. **L129 remaining**, in the nf262 NOTES order: the GB rows 0x00BF36A0 / 0x00BF36E0 / 0x00BF9920 as one unit with their self-renames (class
   operator deletes: FableWin `??3CEngineLandscapePatch@@SIXPAX@Z` / `??_V...SIXPAX@Z`, Ego_r the placement forms, same bytes; two sources
   per name); 0x00BE5C30 (`?g_obj_01436ea8`, untabled) + 0x00BE6090 with 0x00BDA110 only once a second source for
   `?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z` is found; LocalDetailEngine 0x00BFAAE0 / 0x00BFB590 and 0x00B684F0
   after their ports; the CEngineGlobal rows last; also 0x00BEB4D0's `?DAT_01436ea8@@3PAEA`. Hand back after any finished unit.
2. **0x009B0AC0** (nf260 NOTES plan).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF262 file set and the decisions you need; hand back early if your context gets heavy.
