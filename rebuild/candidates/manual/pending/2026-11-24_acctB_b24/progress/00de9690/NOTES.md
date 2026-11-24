
## B24A9 (2026-11-24) baseline on today's headers
DIFFER(3428v3412)	compiles on today's headers; 55 hunks; CIsThingAlive CScriptThing-copy split (INLINE_DEPTH_REVISIT), untouched by B24A9 beyond the baseline

## B24A10 (2026-11-24), 5 grades: DIFFER(3412v3412) 46 hunks (from 3428 / 55). differ/00de9690.cpp = a10/h4.cpp.
- Fix that moved it (same as row 0x00ED8680): `#pragma inline_depth(0)` round the row-local inline CIsThingAlive ctor +
  FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -> predicate copy 0x004ABE90 CALLED, length now equal, call sequence identical to retail
  (a10/ocalls equivalent: 206 v 206 calls, no diff). Predicate written `SetPredicate(CIsThingAlive(Me));` (FableWin 0x0388CEA0 builds
  the temporary, calls SetPredicate, destroys it; the named-local block form gives the same bytes).
- Debug build (FableWin 0x0388CEA0 /Od, Ego_d.pdb locals via pending/2026-11-21_codex_pass26/tools/egod_locals.py on
  $S/dl/debug_owner/Ego_d.pdb): function-scope locals ReturnToCaveTimer (was HitTimer), seh_me, _Fail_Val (TRetVoid), BeenHit (was
  GoingHome); four blocks {speech_movie, PauseMyEntitiesPlease}. Every terminate exit is `return _Fail_Val.Return();` (FableWin calls
  TRetVoid::Return 0x017EA518 at each). Applied (h4): names + TRetVoid -- byte-identical to h2 (no codegen effect), kept as the real text.
- Remaining residue (46 hunks, all esp-slot operands): frame packing. /FAs maps (a10/base.asm, a10/h4.asm) are the same in the B24A9
  base and now: ours $T(-160) temp1, ReturnToCaveTimer -156, first-loop temp2 -128; retail temp1 0x18 (-156), temp2 0x14 (-160, shared
  with ReturnToCaveTimer). FableWin shows the same condition shapes (one condition, three CCharString temps, conditional dtor flags).
  Next idea: the frame-packer trace ($S/w22/b24d/00931390/tools/pk, pkshow.py; $S/w22/b24b/00839940/pk/qs3.py) on h4 to read the
  slot weights of temp1/temp2/ReturnToCaveTimer, then find which block-frequency change (guard v nested block) flips them.
