# Name-fix agent NF246 (2026-11-22, round 16): the wrong DTOR_OUTOFLINE families, NF241's open lines, 0x0049D800, 0x01296F14

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF244 (`$S/w22/agentNF244/`: the CEngine unit, CModeBase follow-ups, the 0x006A1620 binders, the held p27 lines,
0x00418DCA) and you. No authoring agent runs. Off limits: NF244's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is
not `released` or `NF246` (write NF246 there first), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`),
0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF245's with overrides
Read `$S/w22/agentNF245/BRIEF.md` and the briefs it chains to (NF243, NF241 ... NF215 "Lane", `$S/w22/agentNF246/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF246`, names -> `NF246`. On a conflict: this file > NF245 > earlier. nf245 LANDED (`batches/2026-11-22_landing_nf245/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf245_handback/`: its NOTES and LESSONS have the per-row recipes for moving a
list destructor binder, and `owners.tsv` is the pk.py owner map you need so a new `~list` thunk copy is compared against its 5-byte
thunk, not the body). Entry 1 of NF245's brief is DONE; yours are its entries 2 and 3. Start from the repo tree now. Never write into
/home/user/FableDecomp: vland / equiv_check need it as cwd, so list the root before and after and leave nothing there; never run git, not
even read-only; wait on jobs by PID (kill -0), never pgrep -f, never a foreground sleep loop. Every grade under `timeout 900`. Disk is
tight. Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check, recheck every differing row.

## Entries, in this order
1. **The wrong DTOR_OUTOFLINE families** (nf243 NOTES section 2; `refs_e2_census.tsv` = NF245's caller census today), each as a self +
   binders unit, census first, as many as fit: 0x005127A0 (thunks 0x00514280 / 0x00A81C40; 13 heap / sort / vector template rows incl.
   0x00A82BE0, 0x00A83570), 0x004AB4F0 (thunk 0x004AC080; 7 rows incl. 0x005EB590; the tree / map rows 0x00519560 / 0x0051A2F0 /
   0x0051BF90 may be blocked as 0x00756D00 was: leave blocked binders on `~list` and land the rest only if the self row can move without
   leaving a binder naming an undefined function, i.e. the `~list` reloc_bindings line stays with a blocked witness, as nf245 did),
   0x00685BC0 (thunk 0x006860E0; 0x00682810 + 0x00986510, whose global-namespace `list` model must be ported first). For each: the self
   row's catalog name, the thunk address's catalog name (queue if catalog-only), owners.tsv lines for pk.py. 0x004B8940 stays undecided.
2. `QUEUE_LINES.tsv` (39 lines, NF241's open ones), then 0x0049D800 (CWorld.h GetLocalSeed) and 0x01296F14 (the ScreamerIntoAggressive
   vtable with its slot rows 0x0096DBA0 / 0x0096DBB0 / 0x0096E3F0).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF245 file set (+ hdr/ and orig/ for any header, owners.tsv) and the decisions you need; hand back early if your
context gets heavy.
