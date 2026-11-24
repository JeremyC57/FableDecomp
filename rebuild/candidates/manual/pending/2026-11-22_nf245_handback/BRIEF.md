# Name-fix agent NF245 (2026-11-22, round 15b): the 0x004046B0 list family as one unit, the wrong DTOR_OUTOFLINE families, NF241's open lines

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF244 (`$S/w22/agentNF244/`: the CEngine unit, CModeBase follow-ups, the 0x006A1620 binders, the held p27 lines,
0x00418DCA) and you. No authoring agent runs. Off limits: NF244's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is
not `released` or `NF245` (write NF245 there first; the 65 0x004046B0 rows are already NF245), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF243's with overrides
Read `$S/w22/agentNF243/BRIEF.md` and the briefs it chains to (NF241 ... NF215 "Lane", `$S/w22/agentNF245/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF245`, names -> `NF245`. On a conflict: this file > NF243 > earlier. nf243 LANDED (`batches/2026-11-22_landing_nf243/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf243_handback/`; its NOTES sections 1-2 are your starting point, and its work
dir `$S/w22/agentNF243/work/` (census, drafts t_*.cpp) and tools (copied to `agentNF245/tools/`: xl.py, xl2.py, dtk.py) are yours to read).
Start from the repo tree now. Never write into /home/user/FableDecomp: vland / equiv_check need it as cwd, so list the root before and
after and leave nothing there; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f, and never as a foreground
sleep loop. Every grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under an opt-in macro; prove header
changes with equiv_check and recheck every differing row.

## Entries, in this order
1. **0x004046B0, ruling 2 (a):** write the add-only `FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY` partial mode in engine/CStringParser.h
   (NF243 NOTES section 1: the line-182 `#else` -> `#elif !defined(..._ONLY)` is an edit to an existing line, so prove with equiv_check that
   every landed row preprocesses identically), port each movable row per the recipe (dtor-only rows: BASE_OUTOFLINE + fable_containers.h;
   inline-parser rows: BASE_DTOR_VISIBLE_NOINLINE + the partial mode; the 3 DTOR_OUTOFLINE rows: NF241's swap), grade all 39 ungraded ones,
   then land the self row (`~list` -> `~_List_base`) with every binder that grades as ONE unit. Target split 3 + 61; 0x00501D30 / 0x00756D00
   / 0x007AE3D0 stay on `~list` (their vector / map ports are out of scope unless cheap). Also the catalog rows: 0x00404710 = `~list` (the jmp
   thunk) and the 0x00404690 / 0x004046F0 spellings, if their catalog names are wrong. Hand back after this entry if it is large.
2. **The wrong DTOR_OUTOFLINE families** (NF243 NOTES section 2): 0x004AB4F0 (thunk 0x004AC080; 7 referencers incl. 0x005EB590),
   0x005127A0 (thunks 0x00514280 / 0x00A81C40; 13 incl. 0x00A82BE0, 0x00A83570), 0x00685BC0 (thunk 0x006860E0; 0x00682810 + 0x00986510,
   which needs its global-namespace `list` model ported first). Each as a self + binders unit, census first; as many as fit.
   0x004B8940 stays undecided (one source).
3. `QUEUE_LINES.tsv` (39 lines, NF241's open ones, untouched by NF243), then 0x0049D800 (CWorld.h GetLocalSeed) and 0x01296F14 (the
   ScreamerIntoAggressive vtable with its slot rows 0x0096DBA0 / 0x0096DBB0 / 0x0096E3F0).
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF243 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if your context gets heavy.
