# Name-fix agent NF258 (2026-11-22, round 22): the L37 vector<unsigned long> family, then the L129 GPLandscapeRenderer family

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF257 (`$S/w22/agentNF257/`: CCreatureAction L72 SummonCreatures / L100 WillHealInto, 0x0096B600, 0x00418DCA) and
you. No authoring agent runs. Off limits: NF257's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex
names, no 0x) is not `released` or `NF258` (write NF258 there first, before ANY compile of that row, HEAD controls included), the 25 Codex
pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`,
`$S/w22/c2t*/`.

## Your brief is NF256's with overrides
Read `$S/w22/agentNF256/BRIEF.md` and the briefs it chains to (NF254, NF252, NF250, NF248, NF246, NF245, NF243, NF241 ... NF215 "Lane",
`$S/w22/agentNF258/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF258`, names -> `NF258`. On a conflict: this
file > NF256 > earlier. nf256 LANDED (`batches/2026-11-22_landing_nf256/README.md`, whose rulings you work under; hand-back
`pending/2026-11-22_nf256_handback/`: NOTES has the L37 order and the L129 family plan; nf254 NOTES has the L37 census). Start from the repo
tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call,
never pgrep -f, NEVER any foreground sleep; never compile a row (HEAD controls included) before its lock is yours and its clang precheck
passed, and always under an outer `timeout 900` (NF256 ran controls under q.py's inner timeout only); never `rm -rf` a variable path
(tools/clean.py). Every grade with the row's CompilerFlags. Disk is tight (under 3 GB). Every edit to an existing header line goes under
an opt-in macro; prove header changes with equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries: `OPEN_LINES.tsv`, in this order
1. **L37 (0x00535470):** the 8 stand-in binders (CULongVector, CRetailVector, CGroupArray x2, one-parameter `vector<K>` x2,
   `?ctor@CCharString`, `?copy_00535470@CArrayUL`) onto the STLport spelling the self row and 18 binders already use, as one family; also
   fix 0x00ADC310's catalog name (its members are vector<unsigned long> + map<unsigned long,long>, nf256 NOTES). Hand back after it if large.
2. **L129 (0x01436EA8, GPLandscapeRenderer, proven by nf256):** the 91-referencer family per the nf256 plan, in units that each leave every
   address they touch on fewer spellings, never a partial split; hand back after any finished unit.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF256 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if
your context gets heavy.
