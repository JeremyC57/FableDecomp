# Name-fix agent NF265 (2026-11-22, round 26): the rest of L129 (GPLandscapeRenderer), then 0x009B0AC0

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF264 (`$S/w22/agentNF264/`: the held AllowsCombinationAnims unit 0x00692890 / 0x006F5C10 / 0x0062EA10, the
CastAssassinRush models 0x0072CAA0 / 0x008CD470, 0x00AD9770 / 0x00AD9980, 0x00418DCA) and you. No authoring agent runs. Off limits: NF264's
entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not `released` or `NF265` (write
NF265 there first, before ANY compile of that row, controls included), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF263's with overrides
Read `$S/w22/agentNF263/BRIEF.md` and the briefs it chains to (NF262, NF260, NF258, NF256, NF254, NF252, NF250, NF248, NF246, NF245, NF243, NF241
... NF215 "Lane", `$S/w22/agentNF265/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF265`, names -> `NF265`. On a
conflict: this file > NF263 > earlier. nf263, nf262 and nf260 LANDED (`batches/2026-11-22_landing_nf263/README.md`, `.../2026-11-22_landing_nf262/README.md`,
whose rulings you work under; hand-backs `pending/2026-11-22_nf263_handback/` (NOTES: the plan for the 12 remaining spellings on 22 rows) and `.../2026-11-22_nf262_handback/`).
Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a
later tool call, never pgrep -f, NEVER any sleep in any call (not even `sleep 0`); a grade runs only in a later call than its passing clang
precheck (a clang failure a two-line VC7.1-header control reproduces is clang's: note and grade), after the lock is yours, under an outer
`timeout 900`; never `rm -rf` a variable path (tools/clean.py). Disk is tight (under 3 GB). Every edit to an existing header line goes
under an opt-in macro; prove header changes with equiv_check (wait for it) and recheck every differing row. No retail byte dumps.

## Entries, in this order
1. **L129 remaining** (nf263 NOTES order): first 0x00BE5C30 (`?g_obj_01436ea8`, untabled) + 0x00BE6090 with 0x00BDA110 onto
   `?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z` (Ego_r 009c26a0 + FableWin 02cb7490), via an add-only declaration
   opt-in in CEngineLandscapeMemoryManager.h proven with equiv_check; then LocalDetailEngine 0x00BFAAE0 / 0x00BFB590 and 0x00B684F0 after
   their ports; 0x00BEB4D0's `?DAT_01436ea8@@3PAEA`; the CEngineGlobal rows last. Hand back after any finished unit.
2. **0x009B0AC0** (nf260 NOTES plan).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Put `timeout 900` on the launch line of every grade. Hand back with the NF263 file set and the decisions you need; hand
back early if your context gets heavy.
