
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

## B24A11 (2026-11-24): RELOCATION_MATCH, stock flags. vt PASS (mutations fail), strict vland PASS (no WARN / RELOC_ lines), payload.json written.
LESSON (top): a frame-slot residue where a FUNCTION-SCOPE object with an out-of-line ctor is live in the WHOLE function (packer trace:
it interferes with every node, a10/a11 pk) is callee-body visibility: with only the declaration c2 assumes `this` escapes; give the TU
the real inline bodies (even when retail still CALLS them) and its live range starts at construction.
- Diagnosis (a11/b0.pk.json via $S/w22/b24d/00931390/tools/pk; a11/inter.py): ReturnToCaveTimer interfered with all 45 packer nodes,
  so the first hit test's temp2 could not share its slot (retail: Timer + temp2 at esp+0x14, slot weight 25 > the temp1 slot's 24).
  Probe e1 (Timer..end in a block) matched but contradicts Ego_d/FableWin (Timer is function scope: Ego_d record, FableWin frame -0x24 next
  to seh_me) -- not used.
- What made it match: `inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}` + `inline CTimer::~CTimer() {
  pDSTGame->DeregisterTimer(TimerIndex); }` (B24B SHARED_FORM text, 119-row landed spelling) in the TU; retail still calls 0x00CD4450 /
  0x00CD4470 and so do we. operator= / GetTime / operator== must stay FABLE_FORCEINLINE (plain inline: 3396 / 4 hunks; == alone: 4 hunks).
- Still needed (measured): inline_depth(0) round the CIsThingAlive ctor + NOVTABLE (without: 3428 / 9 hunks); row-local empty
  ~CIsThingAlive (without: 3390 / 52 hunks). Cleanups: local enums -> EScriptAIPriority.h (FABLE_RETAIL_PHASE1_ARCHER_PASS26) /
  ETextGroupSelectionMethod.h / EHeroAbility.h; dropped CQ_SunnyvaleMasterData.h include and the SCRIPTEDTHINGBASE_OUTOFLINE_DTOR opt-in
  (its ??_G copy then equals retail 0x007E7520); ~CIsThingAlive under inline_depth(0) (landed 0x00D973D0 form) for the family ??_G copy.
  FableWin 0x0388CEA0 loops are plain while loops (no `if (false) {} else for` openers), so no loop respelling.
- Pick-any: pickany_vs_retail 8 EQ + NEQ ??1CIsThingAlive 0x00E241B0 (not leaf-referenced); every other copy by hand (a11/retcmp.py):
  EQ ??_GCBaseClass, ??_GCScriptGameResourceObjectBase, ??_G/??1 ScriptedThingBase, CTimer ctor/dtor; NEQ ??_GCScriptThing 0x004AB170
  and ??_GCIsThingAlive 0x00CDF160 (vtable-only) -- all copies byte-identical to staged 0x00ED8680's object and landed 0x00D973D0's
  (a11/cmpobjs.py). Retail's ??_GCScriptThing expands ~CScriptThing; an inline ~CScriptThing here breaks Main (retail calls 0x004AA840
  six times; n1 3460). Family HEAD debt, same as 0x00ED8680.
- Files: 00de9690.cpp = differ/ = a11/n3.cpp + comments; 00de9690_test.cpp (11 scenarios).
- Pick-any rerun on the FINAL lv object (lv/00/de/00de9690/00de9690.obj): same result (9 EQ + NEQ ??1CIsThingAlive; by hand NEQ
  ??_GCScriptThing / ??_GCIsThingAlive only). The ??_GCScriptThing NEQ copy is also in staged 0x00ED8680's object (its reloc_bindings
  checker missed it: 0x004AB170 is not in reloc_bindings) -- report, row not touched.
