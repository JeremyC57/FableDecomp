# Retail class-model migration, wave 3 (one agent; the chunk 582 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig3/repo; cd $S/mig3/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1 and 2 landed as 08436554e and 96aef05db (613 + 711 rows). Records: `rebuild/candidates/manual/headers/
2026-10-29_retail_models_wave1/` and `2026-10-30_retail_models_wave2/` (HEADERS.md, LESSONS.md, scripts/, and NEXT.md = the
plan you execute). Full rules: `$S/mig1/BRIEF.md` (same rules; your worktree is `$S/mig3/repo`, detached HEAD 96aef05db;
scratch `$S/mig3/work/`; hand-back files in `$S/mig3/`). Copy wave 2's `scripts/` into `$S/mig3/scripts/` and extend them.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS. Stop at about 800 edited rows.

## Order of work
1. **Settle the def-chain copy-member question first** (wave 2 NEXT item 3; it also settles wave 1 open item 1, the
   CScriptThing copy that is inline at one site and out of line at another). Experiment exactly as NEXT.md describes: give
   `FABLE_PDCB_TYPED_MEMBERS` includers implicit (undeclared) copy members with shared CDefString / CVectorMap / CArray
   models or a CCountedPointer-style contract; rerun the 64 /Os callers (must still call 0x00431ED0) and the 5 /O2 expanders.
   Record the rule in LESSONS.md and encode it in the headers. If it cannot be settled in ~1.5 hours, record the residue and
   move on with the out-of-line declarations.
2. **BC:** `mig_bc.py --allow-v00` exact shapes (cbb21bf1 100, 3ed153c6 53, 9b25d360 29, bbb0d2cf 12), then the dtor-only
   b638a797 (466) with mcheck, then the inline-dtor shapes 77992de8 (96) / 62d3d4ae (82); `mig_bc_copy.py` (169); the 135
   inline-ctor rows checked each against its bytes (store overwritten by the derived vtable = implicit copy; call = the
   out-of-line ctor 0x0099A2D0). LESSONS wave 2 item 1 is the rule.
3. **CGSI interface-only:** `mig_cgsi.py` (462), with a rename map for invented slot names at a proven index
   (Update/YieldFrame at slot 7 are NewScriptFrame, 18 rows); then the 151 local-CBaseClass interface rows after BC.
4. **PDCB remainder (108)** including the 8 DIFFER reverts (drop the derived `_body` pads over the base), the 2 `typedef long
   CDefString` rows, `add_def_overrides.py` on abstract-derived SRC_FAILs.
5. **DCB clusters** 83101c63 (18), 46f7e374 (15), 662692ff (14), 012a455e (24) / fe4a0baa (7), the Destroy/RefCount stand-ins,
   the 35 local-CResource rows, the 16 SFB hand edits, as budget allows.
6. If budget remains: start the NScript CScriptBase + CLevelScriptBase family per wave 1 NEXT (a)-(d).

## Rules (unchanged)
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm.
- Regrade in batches of ~400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig3/work/lv<n>`, delete each
  lv after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert rows that stop matching; log residue.
- Bindings: one retail address = one name; update `rebuild/oracles/reloc_bindings.tsv` only when a rewrite changes a row's
  relocation symbols; `bindings_multi_check.py` must print 0. Leave 0x0143E8F8 and the NAME_FIXES respellings to the
  coordinator (add to NAME_FIXES.tsv instead).
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig3/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md, NEXT.md (wave 4 counts and plan), NAME_FIXES.tsv, LESSONS.md, logs/, scripts/. Final
message: the copy-member rule found, headers added/changed, rows per family, recheck totals, reverted rows, bindings changes,
open questions. Leave no background job running.
