# Name-fix agent NF241 (2026-11-22, round 13b): the rest of the CODEXP27 name fixes (NF239's open lines) and the list<CComponent*> family

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF240 (`$S/w22/agentNF240/`: 0x0143B8D8 / 0x006A1620 with NF237's L38 / L39, the CModeSitBench ctor 0x00740040,
the CEngine unit 0x00B260B0 / 0x00B261D0, the other held p27 lines, 0x00418DCA) and you. No authoring agent runs. Off limits: NF240's
entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is not `released` or `NF241` (write NF241 there first), the 25 Codex
pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF239's with overrides
Read `$S/w22/agentNF239/BRIEF.md` and the briefs it chains to (NF237 ... NF215 "Lane", `$S/w22/agentNF241/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF241`, names -> `NF241`. On a conflict: this file > NF239 > earlier. nf239 LANDED (`batches/2026-11-22_landing_nf239/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf239_handback/`: NOTES section 3 orders the open work, APPLIED.tsv gives each
line's blocker). Use NF239's fixed census.py (one-line catalog entries). Start from the repo tree now. Never write into
/home/user/FableDecomp (vland / equiv_check need it as cwd: list the root before and after, leave nothing); never run git, not even
read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade under `timeout 900`. Disk is tight (about 3 GB free). Every edit
to an existing header line goes under an opt-in macro; prove header changes with equiv_check and recheck every differing row.

## Entries, in this order (NF239's own order)
1. **The list<CComponent*> family pass:** the held self rows 0x0042ABAF / 0x0042ABCA (`_List_base<NUISystem::CComponent*>` ctor / dtor,
   `pending/2026-11-22_nf239_handback/rows/`) land only together with their 31 + 10 `list` binders, so that no binder is left with a name
   nobody defines (0x005AE6AF, 0x005AE9DE, 0x005AEAEE, 0x005B43A2, 0x0042BED4 first). In the same pass correct the
   `FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST` comment in stlport/fable_containers.h (comment only: its evidence is STLport 4.6's shape, not the
   Dinkumware PDB). Then 0x004046B0 (about 70 rows, the same shape) if it fits.
2. `QUEUE_LINES.tsv` (42 lines: NF239's open / partial / deferred; L38 / L39 are NF240's): the one-row self ports whose binder is already
   right, the virtual-slot rows (run vtcheck), L57 (0x00899DB0 onto engine/CTCCombat.h), L111, L36, L28, L114, L120 (the sky builder),
   L23 / L24, L27, L4 / L5, L129.
3. 0x0049D800 (engine/CWorld.h needs GetLocalSeed declared) and 0x01296F14 (the ScreamerIntoAggressive vtable: one unit with the slot rows
   0x0096DBA0 / 0x0096DBB0 / 0x0096E3F0).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF239 file set and the decisions you need; hand back early if your context gets heavy.
