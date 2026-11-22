# Name-fix agent NF256 (2026-11-22, round 21): L28, L57, the L37 family, the CEngineSkyRenderer builders, L129 plan

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF255 (`$S/w22/agentNF255/`: the four blocked CCreatureAction ctor rows L72 / L74 / L92 / L100 with their Clone rows,
0x0096B600, 0x00418DCA) and you. No authoring agent runs. Off limits: NF255's entries, every row whose lock owner
(`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not `released` or `NF256` (write NF256 there first, before ANY compile
of that row, controls included), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070,
0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF254's with overrides
Read `$S/w22/agentNF254/BRIEF.md` and the briefs it chains to (NF252, NF250, NF248, NF246, NF245, NF243, NF241 ... NF215 "Lane",
`$S/w22/agentNF256/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF256`, names -> `NF256`. On a conflict: this
file > NF254 > earlier. nf254 LANDED (`batches/2026-11-22_landing_nf254/README.md`, whose rulings you work under; hand-back
`pending/2026-11-22_nf254_handback/`: NOTES has the L28 / L57 plans and the L37 / L129 censuses). Start from the repo tree now. Never write
into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, NEVER
any foreground sleep; NEVER compile a row (HEAD controls included) before its lock is yours and its clang precheck passed; never `rm -rf` a
variable path (tools/clean.py). Every grade under `timeout 900` with the row's CompilerFlags from build_candidates.ps1 (list any new flag a
row needs). Disk is tight (under 3 GB). Every edit to an existing header line goes under an opt-in macro; prove header changes with
equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries, in this order
1. L28, then L57 (nf254 NOTES plans; prove the L57 flag offsets and the 0x0040F020 GetTC path before relying on them).
2. The L37 family: the 8 stand-in binders of 0x00535470 (CULongVector, CRetailVector, CGroupArray x2, one-parameter `vector<K>` x2,
   `?ctor@CCharString`, `?copy_00535470@CArrayUL`) onto the STLport spelling the self row and 18 binders already use, as one family.
3. 0x00B61CD0 / 0x00B61FF0 (BuildBaseBandMesh / BuildStarFieldVB): ports onto CEngineSkyRenderer.h (nf254 L120 follow-on), 2 -> 1 each.
4. L129 (91 referencers, 18 table spellings): only the address proof for `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`
   and a family plan; no partial landings.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF254 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if
your context gets heavy.
