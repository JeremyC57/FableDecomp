
## B24A9 (2026-11-24) baseline on today's headers
SRC_FAIL	NEEDS_PORT (B24A9): the draft's opt-in FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1 no longer exists; engine/CQ_BanditCampBossBattleScript.h only forward-declares class CAssassin1 (line 42; GATE2GUARD1 / BANDITKING / CROWDBANDITS / HOSTAGE / GAMEMASTER / FORGER / CAMPHOSTAGEGUARD opt-ins show the pattern for an add-only ASSASSIN1 section); best on record DIFFER(3847v3623) (MovieBase dtor inline budget)

## B24A10 (2026-11-24): untouched (context spent on 0x00931390 / 0x00ED8680 / 0x00DE9690). Levers found today for the next agent:
- Debug build on the lane ($S/dl/debug_owner/): Ego_d locals + scopes via `python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/
  tools/egod_locals.py $S/dl/debug_owner/Ego_d.pdb '<Qualified::Name>'` (types: `llvm-pdbutil dump -types -type-index=0x.. Ego_d.pdb`);
  FableWin /Od copy via fwdis.py (address from ghidra_out/fablewin_pdb_names.tsv). Read both FIRST.
- Quest scripts: locals as Ego_d names them, `TRetVoid _Fail_Val;` + `return _Fail_Val.Return();` at every terminate exit; FableWin
  `xor reg,reg ; je ; jmp` before a loop = `if (false) {} else for (...)` (GOTCHAS 442), e.g. `for (;; Game->NewScriptFrame())` main loops --
  this loop text alone fixed 0x00ED8680's inline split at stock flags.
- Nested CScriptThing copy CALLED inside an expanded predicate / thread ctor: `#pragma inline_depth(0)` round the row-local inline
  CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (H40), and round an explicit CSpawnedFunc1<..> ctor specialization (0x00ED5F50).

## B24A11 (2026-11-24): RELOCATION_MATCH with /d2inlS15 (family flag), vt PASS (mutations fail), strict vland PASS (no WARN / RELOC_), payload.json written (flags ["/d2inlS15"]).
LESSON: CQ_BanditCampBossBattleScript rows whose only residue is ~MovieBase called at cold exits (call sequence: retail 0x6E7AB0 + 0x99A430
expanded, ours 0x6E7B80) match with the family's /d2inlS15 once ported; S10..S14 do not (~MovieBase c1 19).
- Port: hdr/engine/CQ_BanditCampBossBattleScript.h = today's header + FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1 (hdr/README.md;
  re-applied from chunk 845, independent of the staged ASSASSINMARKER opt-in). Stock grade after the port: DIFFER(3884v3623) 41 hunks
  (call-sequence diff a11/cseq.py: only the MovieBase dtor sites).
- Debug build: Ego_d locals seh_me, TalkedTo (was `asked`), _Fail_Val; blocks {speech_movie, PauseMyEntitiesPlease} (+ long answer);
  FableWin 0x03691C40: TRetVoid::Return at every exit, TalkedTo set BEFORE ParentClass.TalkedToAssassin (draft had the reverse), tail is
  if (hired || attacked) { T; ClearThingHasInformation; Release; while (true) {NSF; T;} } else { T; while (true) {NSF; T;} }; plain while
  loops (no for-scope openers), so no loop respelling. Macros A1_T / A1_SAY / A1_ANSWER expanded; local enums -> headers;
  pGlobalScriptDef via fable_script_def_global.h.
- Measured needs under S15: inline_depth(0) round the CIsThingAlive ctor (without 3655 / 24 hunks) and NOVTABLE (without 3623 / 7 hunks).
  ~CIsThingAlive under inline_depth(0) for the family ??_G copy. The stock-flag sibling 0x00D0A830 uses goto + an in-body pragma device, so
  no stock form was borrowed from it. S15 is a fitted family device that object mode must settle (EXTRA_RULES 2026-11-24, line 516).
- Pick-any (lv object): reloc_bindings checker 8 EQ, NEQ ??1CIsThingAlive 0x00E241B0 and ??1MovieBase 0x006E7B80 (not leaf-referenced);
  by hand (a11/../../00de9690/a11/retcmp.py): EQ ??_GCBaseClass, ??_GCScriptGameResourceObjectBase, ??_G / ??1 ScriptedThingBase; NEQ
  ??_GCScriptThing 0x004AB170, ??_GCIsThingAlive 0x00CDF160, ??_GCScriptGameResourceObjectMovieBase 0x006E7BD0 -- every NEQ copy is
  byte-identical to the staged 0x00ED8680 / 0x00DE9690 objects' copies (MovieBase ones also to staged 0x00D11930's). Family HEAD debt.
- Files: 00d04ae0.cpp = differ/; 00d04ae0_test.cpp (15 scenarios).
