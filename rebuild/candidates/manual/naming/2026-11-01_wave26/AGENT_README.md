# Naming pass wave 26 (2026-11-01): 62 landed rows respelled, all byte-identical (62/62 RELOCATION_MATCH + PASS)

## Inputs and index
- Inputs:
  - the brief naming/briefs/BRIEF_W26.md;
  - QUEUE_W26_input.tsv: 44 rows, including the c693 rows and the c694 rows the coordinator appended mid-wave;
  - DEFERRED_W25_INPUT.tsv.
- Index:
  - idx/full.jsonl is HEAD: the wave 25 after.jsonl, plus chunk 693 (idx/delta_c693.jsonl, 9 rows), plus chunk 694 (idx/delta_c694.jsonl, 10 rows).
  - Both chunks landed during the wave. I found them by modification time, with no git.
  - Before hand-back, nothing in the repo outside .git is newer than idx/delta_c694.jsonl.
  - None of the 62 rows was touched by either landing.
  - idx/after.jsonl is HEAD plus ov_final (idx/ovfinal.jsonl).

## Commands
Run from $S/naming/w26 with S exported:
1. `python3 scripts/compose26.py ov_final`
   - Runs scripts/hand26/h26_<fam>.py on fresh repo copies.
   - Writes runs/<fam>.rows.txt and ov_<fam>/.
2. `bash scripts/chain26.sh` does all of the following:
   - the full recheck: logs/recheck_final.log and runs/rk_final.pairs.jsonl;
   - mkindex and after.jsonl;
   - qscan26, mkq26 (with mkq26_notes.py), mkdef26, mkbind26 (and runs/reloc_bindings.after.tsv);
   - bindings_multi_check, which reports 0;
   - mkren26 then fixren26, and mklabels26 then fixlab26.

New helpers:
- scripts/osyms.sh `<ov> <regex> <addr...>` lists the defined and undefined symbols of a row's object.
- scripts/hand26/vecify.py holds the STLport vector re-model (vecify / stdvec).

## Families
Listed in compose order.

- **nul (3): 00c0b910 00c0b9f0 00c0ba40.**
  - `_afxDataNil` becomes `?_NullString@@3PAUCStringData@@A` (D1 of DECISIONS_W25_RESOLVED).
- **tbvm (1): 00744580.**
  - The row is `?GetTimeBasedVisibilityMultiplier@CTCPerceiveThing@@ABEMXZ` (D5; Ego_r + FableWin; Day/NightTimeVisibilityMultiplier through CGameTimeManager::IsDay / IsNightTime).
  - Renamed from GetExtendedSightRadius.
- **wcol (1): 009f31c0.**
  - The row is `?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z` (D7: the colour is passed by reference).
- **pred (11): 00d3a1a0 00d408b0 00d9f270 00e0e2e0 00e11bb0 00e4a060 00e8b020 00ec46b0 00ee76f0 00ef29e0 00ef3000.**
  - SetPredicate 0x00F35B10 and IsActiveThreadTerminating 0x00F35B30 move onto the global CEntityScriptBase; the filters derive from NScript::CPredicateBase.
  - This resolves the wave 25 DEFERRED entries for both targets.
  - In 0x00D9F270, CMC_Minion is now nested in CQ_MinionCampScript. Its Leaf line had named the PDB nesting but matched no symbol, so the verifier graded the largest function.
- **gossip (2): 008a78e0 008a7930.**
  - The rows become CGameScriptInterface::ClearActiveGossipCategories / ClearIsGossipForPlayer.
  - Each is map<CCharString,bool>::clear() on the STLport _Rb_tree, which calls the private _M_erase 0x007ACC50.
- **rbt (1): 007ad180.**
  - The row is _Rb_tree<...bool...>::erase(iterator, iterator), on the own-row STLport text of 0x007ACC50.
- **vlong (16): vector<long> grow-insert callers.**
  - Each row now uses std::vector<long, allocator<long> > with the protected `_M_insert_overflow` (IAE, size_type I).
  - Out-of-class template bodies are `__declspec(noinline)`.
  - 0x0098A9A0 uses CArray<T> : std::vector<T>, so its CPixelShader ctor keeps the CArray<long> parameter that the own row 0x00A5ECB0 defines.
- **vws (4): 005fdad0 0071ce80 009aef60 009af480.**
  - Each row now uses std::vector<CWideString, allocator<CWideString> > with the __false_type grow-insert 0x00412330.
- **gst (2): 00bb1640 00bc7920.**
  - The rows are GetSupportedTypes of CEnginePrimitiveManager2DSprites / CEnginePrimitiveManagerText (RTTI vtable slot 2), on the J027 0x00BD28A0 model.
- **c693 (4): own rows 00e241b0 009cbfe0 00994400 009cda70.**
  - 00e241b0 is ~CIsThingAlive.
  - 009cbfe0 is the private GetRecoveryFilePath.
  - 00994400 is CFileDataOutputStream(CAFile&).
  - 009cda70's local class is CBankFile, not CBankFile_Retail.
- **fdos (3): 009a7720 009d4ce0 00a60aa0.**
  - CDiskFileWin32 : CAFile (RTTI 0x0122D06C), and the stream takes the file by reference.
- **c694 (5): 00854500 00854540 00854580 00718810 00854450.**
  - The script-module ??_G stand-ins now call `??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ` (0x00854310) and `??1CScriptModuleBase@NCameraModes@@UAE@XZ` (0x005ED120).
  - Each call stays qualified. Leaf lines were added.
- **cti (9): CClassTypeInfo models.**
  - AllocFunc is `CClass* (__fastcall*)()` (FableWin ??0CClassTypeInit mangling, chunk 694 decision 2); it was __cdecl.

## Outputs
- QUEUE_W26.tsv:
  - all 44 queue rows;
  - outcomes: 7 respelled, 7 respelled-partial, 10 done, 10 deferred, 9 hold, 1 other-lane.
- DEFERRED.tsv:
  - 17 queue deferrals;
  - 12 wave-26 findings: own-row Leafs on touched rows that differ from the PDB;
  - 452 carried rows;
  - 3 wave-25 entries resolved (runs/w25_deferred_resolved.tsv).
- BINDINGS.tsv: 51 RETIRE, 3 ADD, 11 KEEP, 0 CONFLICT.
  - runs/reloc_bindings.after.tsv = the repo table − 51 + 3.
- RENAMES.tsv has 3 entries: 00744580, 009cda70, 00e241b0.
- LABELS.tsv has 10 entries.
- DECISIONS_W26.tsv has 7 items.
