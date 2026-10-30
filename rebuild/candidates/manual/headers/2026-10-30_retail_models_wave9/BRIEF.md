# Retail class-model migration, wave 9 (one agent; a naming agent / chunk agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig9/repo; cd $S/mig9/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1-8 landed (613 + 711 + 838 + 1,228 + 1,103 + 1,481 + 1,667 + 1,384 rows). Records in `rebuild/candidates/manual/headers/
2026-10-*_retail_models_wave{1..8}/`; wave 8's NEXT.md is the plan you execute and waves 3-8 LESSONS are binding (wave 8
LESSONS 2, 4, 6, 7, 9 especially: `__single_inheritance`, slot counting with a CBaseClass base, read every RELOC_RENAMED,
try the true model before forcing inlining, the includer-side CCountedPointer). Full rules: `$S/mig1/BRIEF.md` (your worktree
is `$S/mig9/repo`, detached HEAD 3e83ccc7f; scratch `$S/mig9/work/`; hand-back in `$S/mig9/`). Copy wave 8's `scripts/`
into `$S/mig9/scripts/` and extend them. Regrading runs ~50 rows/min when the lane is quiet, ~17 with another agent on it;
budget ~1,500 edited rows. A naming wave (wave 11) may run at the same time on other rows: it edits names only; if a row you
plan to touch is in `$S/naming/w11/` file lists, leave it for the next wave.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS.

## Order of work (wave 8 NEXT items 2-7)
1. **A shared `engine/CCountedPointer.h`** (4,290 local models): Data +0, Info (CCPPointerInfo*) +4; copy / assign AddRef,
   ShareData 0x00CE1040, the real release dtor as plain inline (not forced). Prove on 5 rows of each shape first (the
   forced-`__forceinline` rows, the out-of-line dtor rows, the DeleteFunc/ShareData old-body rows), then migrate by family
   with mcheck. Record exactly which landed shapes it cannot reproduce.
2. **MovieBase onto `engine/CScriptGameResourceObjectMovieBase.h`:** the rows VC7.1 reproduces with the implicit dtor and the
   shared pointer (mbexp.py SAME) first; then the 9 mbexp DIFF rows and the 32 single-site 0x006E7AB0 rows (try the COMDAT
   order idea in NEXT.md item 3.3; time-box 1 h; record the rule or the residue).
3. **CScriptThing residue (428)**, mechanical first: CCountedPointer placement (30), PImp_Data words (47), PImp arrays (27),
   local CBaseClass (11), the non-CBaseClass heads (64), InlineCopy tags (43, lesson 7), vptr words (33), pads (29), slot
   mismatches (13). After each batch run esbfollow.sh and a PRED pass (ESB 227 and the predicates 138 follow ST).
4. **CPlayerManager (104):** the 19 const-only rows (model the caller's `const CPlayerManager&` so the header picks QBE
   0x00449960), the 30 pointer stand-ins (respell to the reference accessors), the vptr rows.
5. **The 35 namespace CreateActiveEntityScript rows:** rewrite onto the template binding body of the 85 CAES rows; byte-check
   each. Then CTimer's last 16 and CScriptBase's 88 as budget allows.
6. **CGSI remainder (220 + 36):** per-slot overload evidence for the 77 slot-index rows; the wave 6 reverts.

## Rules (unchanged)
- Def-chain copy members stay out-of-line declarations; the /O2 expanders and CBaseClass.cpp TU rows stay local by design.
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm / asm_bake / toolchain_library rows are
  not yours to touch.
- Regrade in batches of ~300-400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig9/work/lv<n>`, delete
  each lv after). Baseline-regrade failures on an unedited copy. Revert per family edit from its pre-image; payload_check.py
  and mstat_fam.py at the end.
- Bindings: one retail address = one name; `reloc_bindings.tsv` changes only when a rewrite changes a row's relocation
  symbols (bind_add.tsv); `bindings_multi_check.py` must print 0. NAME_FIXES respellings go to NAME_FIXES.tsv.
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig9/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md, NEXT.md (wave 10 counts and plan), NAME_FIXES.tsv, LESSONS.md, bind_add.tsv, logs/,
scripts/. Final message: headers added/changed, rows per family, recheck totals, reverted rows, bindings changes, the
CCountedPointer and MovieBase findings, open questions. Leave no background job running.
