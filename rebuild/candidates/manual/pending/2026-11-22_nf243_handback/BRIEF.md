# Name-fix agent NF243 (2026-11-22, round 14b): the remaining CODEXP27 lines, the 0x004046B0 list family, the DTOR_OUTOFLINE census

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF242 (`$S/w22/agentNF242/`: the held CModeSitBench unit with the vtable slot swap, the CEngine unit, the
0x006A1620 binders, the held p27 lines, 0x00418DCA) and you. No authoring agent runs. Off limits: NF242's entries, every row whose lock
owner (`$S/w22/nflocks/<addr>/owner`) is not `released` or `NF243` (write NF243 there first), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF241's with overrides
Read `$S/w22/agentNF241/BRIEF.md` and the briefs it chains to (NF239 ... NF215 "Lane", `$S/w22/agentNF243/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF243`, names -> `NF243`. On a conflict: this file > NF241 > earlier. nf241 LANDED (`batches/2026-11-22_landing_nf241/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf241_handback/`: NOTES has the per-instantiation census recipe and the
relocation-diff check that proves an opt-in moves only its target). Start from the repo tree now. Never write into /home/user/FableDecomp
(vland / equiv_check need it as cwd: list the root before and after, leave nothing); never run git, not even read-only; wait on jobs by
PID (kill -0), never pgrep -f, never a foreground sleep loop. Every grade under `timeout 900`. Disk is tight. Every edit to an existing
header line goes under an opt-in macro (comment-only edits: token-identical after stripping comments, as NF241 proved); prove header
changes with equiv_check and recheck every differing row.

## Entries, in this order
1. **0x004046B0** (about 70 rows; the list<CComponent*> shape: `~list` spelled where the function is `~_List_base`): census from the
   compiled objects, then the self row and every binder as one unit, so no binder names a function nobody defines.
2. **The DTOR_OUTOFLINE census** (`dtoroutofline_all.txt`: the 19 other landed rows that use `FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE` for
   other list types): per instantiation, is the address `~list` (a 5-byte jmp) or `~_List_base` (the body)? Report each; fix the ones
   that are wrong as family units, as many as fit.
3. **L108-L110:** the three CBankFile members inline in CBankFile.h, as one unit behind a new add-only declaration-only opt-in.
4. `QUEUE_LINES.tsv` (39 lines, NF241's open ones in its order), then 0x0049D800 (CWorld.h GetLocalSeed) and 0x01296F14 (the
   ScreamerIntoAggressive vtable with its slot rows 0x0096DBA0 / 0x0096DBB0 / 0x0096E3F0).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF241 file set and the decisions you need; hand back early if your context gets heavy.
