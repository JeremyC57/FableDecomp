
## B24A9 (2026-11-24) baseline on today's headers
TIMEOUT	NEEDS_PORT (B24A9): includes engine/CQ_GuildTrainingWoodsWillScript.h, not in the repo (checked: no GuildTrainingWoodsWill header under rebuild/include, any prefix); best on record DIFFER(3472v3426) (resize(n) expansion)

## B24A10 (2026-11-24): untouched (context spent on 0x00931390 / 0x00ED8680 / 0x00DE9690). Levers found today for the next agent:
- Debug build on the lane ($S/dl/debug_owner/): Ego_d locals + scopes via `python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/
  tools/egod_locals.py $S/dl/debug_owner/Ego_d.pdb '<Qualified::Name>'` (types: `llvm-pdbutil dump -types -type-index=0x.. Ego_d.pdb`);
  FableWin /Od copy via fwdis.py (address from ghidra_out/fablewin_pdb_names.tsv). Read both FIRST.
- Quest scripts: locals as Ego_d names them, `TRetVoid _Fail_Val;` + `return _Fail_Val.Return();` at every terminate exit; FableWin
  `xor reg,reg ; je ; jmp` before a loop = `if (false) {} else for (...)` (GOTCHAS 442), e.g. `for (;; Game->NewScriptFrame())` main loops --
  this loop text alone fixed 0x00ED8680's inline split at stock flags.
- Nested CScriptThing copy CALLED inside an expanded predicate / thread ctor: `#pragma inline_depth(0)` round the row-local inline
  CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (H40), and round an explicit CSpawnedFunc1<..> ctor specialization (0x00ED5F50).
