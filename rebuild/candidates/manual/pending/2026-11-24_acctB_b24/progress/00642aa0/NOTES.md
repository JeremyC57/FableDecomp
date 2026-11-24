# 0x00642AA0 CMinimapData::PopulateMinimapThemeList -- B24A8 extent check only (row not worked)

ORACLE LENGTH IS WRONG (tell coordinator; not re-cut here): the registered oracle is 3376 bytes and spans several functions.
Retail: code 0x00642AA0..0x00642FBF (`ret 8` at 0x00642FBD, 1312 bytes), then the function's own 7-entry switch jump table at
0x00642FC0..0x00642FDB (entries 0x00642BEC, ..BF3, ..BFA, ..C01, ..C08, ..C0F, ..C16; referenced by the `jmp [*4+0x00642FC0]`
at 0x00642BE8 per xref.py), then int3 pad 0x00642FDC..DF; the next function starts at 0x00642FE0.
Correct length: 1340 bytes (0x53C) = code 1312 + table 28. This agrees with the record DIFFER(1308v1340).

## B24A9 (2026-11-24) baseline on today's headers
TIMEOUT	NEEDS_PORT (B24A9): includes engine/CEngineThemeDef.h, which now lives only in rebuild/include/engine/_quarantine/ (read why it was quarantined before reusing it); oracle re-cut to 1340 by the coordinator; best on record DIFFER(1308v1340) frame layout only

## B24A10 (2026-11-24): untouched (context spent on 0x00931390 / 0x00ED8680 / 0x00DE9690). Levers found today for the next agent:
- Debug build on the lane ($S/dl/debug_owner/): Ego_d locals + scopes via `python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/
  tools/egod_locals.py $S/dl/debug_owner/Ego_d.pdb '<Qualified::Name>'` (types: `llvm-pdbutil dump -types -type-index=0x.. Ego_d.pdb`);
  FableWin /Od copy via fwdis.py (address from ghidra_out/fablewin_pdb_names.tsv). Read both FIRST.
- Quest scripts: locals as Ego_d names them, `TRetVoid _Fail_Val;` + `return _Fail_Val.Return();` at every terminate exit; FableWin
  `xor reg,reg ; je ; jmp` before a loop = `if (false) {} else for (...)` (GOTCHAS 442), e.g. `for (;; Game->NewScriptFrame())` main loops --
  this loop text alone fixed 0x00ED8680's inline split at stock flags.
- Nested CScriptThing copy CALLED inside an expanded predicate / thread ctor: `#pragma inline_depth(0)` round the row-local inline
  CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (H40), and round an explicit CSpawnedFunc1<..> ctor specialization (0x00ED5F50).
B24A11: untouched (context spent on 0x00DE9690 / 0x00D04AE0 / 0x00D67890)
