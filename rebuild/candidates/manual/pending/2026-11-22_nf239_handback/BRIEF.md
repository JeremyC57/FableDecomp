# Name-fix agent NF239 (2026-11-22, round 12b): the rest of the CODEXP27 name fixes (NF237's open + deferred lines)

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF238 (`$S/w22/agentNF238/`: the held p27 bindings, 0x004C7E30, 0x007D7C10, the CEngine ctor / dtor, 0x00418DCA)
and you. No authoring agent runs. Off limits: NF238's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is not
`released` or `NF239` (write NF239 there before you touch a row), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF237's with overrides
Read `$S/w22/agentNF237/BRIEF.md` and the briefs it chains to (NF235 ... NF215 "Lane", `$S/w22/agentNF239/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF239`, names -> `NF239`. On a conflict: this file > NF237 > earlier. nf237 LANDED (`batches/2026-11-22_landing_nf237/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf237_handback/`: NOTES section 3 orders the open families, APPLIED.tsv says
why each deferred line waited). nf236 also landed, so the deferred lines are free unless a lock says otherwise. Start from the repo tree
now. Never write into /home/user/FableDecomp (run tools from a scratch cwd); never run git, not even read-only; wait on jobs by PID
(kill -0), never pgrep -f. Every grade under `timeout 900`. Disk is tight (about 3 GB free). Every edit to an existing header line goes
under an opt-in macro; prove header changes with equiv_check and recheck every differing row.

## Entries
1. `QUEUE_LINES.tsv`: 63 lines (41 open, 22 deferred), each with NF237's line number L<n>. Families in NOTES section 3 order: the NUISystem
   list family (L13-L18), the CPlayerGui family (L20-L24, L38, L39), then the deferred one-line access fixes (e.g. L45 0x0065C781 private,
   L88 0x0090FDB0 / 0x009387C0 protected), then the rest.
2. 0x0122DEE0 (coordinator ruling 4, nf237): `?g_fMinusOne@@3MA` is in no PDB; respell its binders onto the `-1.0f` literal
   (`__real@bf800000`), count-reducing, as many as fit.
3. NF237's follow-ups: 0x0049D800 (0x008EE720's CEngineRandomOwner stand-in v GetLocalSeed@CWorld: needs CWorld from its header);
   0x004046B0 / 0x004AB490 binders spelling `~list` where the function is `~_List_base` (0x004AB490 has three spellings incl. CThingList).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF237 file set and the decisions you need; hand back early if your context gets heavy.
