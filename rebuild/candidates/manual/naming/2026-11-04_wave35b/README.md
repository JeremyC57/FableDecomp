# Naming pass wave 35b (2026-11-04): 812 landed Main / Init / Alloc / constructor rows respelled to their PDB symbols

Input: `$S/wave35b/ROWS.tsv`, the 812 landed level- and entity-script rows (Main, Init, RegisterMain, Alloc, constructors) whose
catalog names changed in `naming/2026-11-04_anniversary/`. One agent built the overlay in scratch (`scripts/`, wave 33
generators: entity classes follow the wave 33 D1 model from the landed Alloc row's TU). The coordinator applied it with
`scripts/apply_w35c.py` and re-graded the result from the working tree.

## Result
- **812 respelled**, each now defining its exact PDB symbol. That is 737 clean rows, plus 75 rows (`DEFERRED.tsv` in the agent
  layout, `BINDINGS_inherited.tsv`) whose only warnings are RELOC_NAME on callees with invented names that the landed source
  already used: CMovieImpPointer, CScriptThingRuntime/Stored, CBaseObjectDtor, `_global::CCountedPointer_Set_00704580`, and others.
  Coordinator decision: take them, because the row itself is correct and byte-identical and every warning was in the row's
  grade before the wave.
- **Re-grade from the working tree** (`logs/log0..3.txt`, `vland.py --recheck-landed`, four splits of 203):
  **812 / 812 MATCH or RELOCATION_MATCH + PASS** (338 MATCH, 474 RELOCATION_MATCH).
  - Warnings:
    - 1,066 SHARED_HEADER_AVAILABLE. The level-script headers use the native Ego_r layout, not retail.
    - 151 RELOC_NAME, all on callee names that were there before this wave.
    - 19 RELOC_ALIAS for the `_rand` / `__CIfmod` import thunks.
  - The agent's per-batch logs are in `logs/agent/`.
- **One address, one name:** six Mains that were spelled RegisterMain (00d5ab40 00d5dd40 00e90830 00eed360 00efca00 00efe4b0)
  now define `?Main@...`.
- **Globals:** global 0x01375748 is respelled `NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations` in 28 rows.
- **Classes:** in 00e299f0 the stand-in `CRecordSet_00cbd510` is now `CScriptBase`. In 00dcde20 the class is `CCrateTeamMember`.
- **Callees:** three are respelled to the PDB name with the identical signature: GetHenchmanIndex -> GetHenchmanTypeFromString,
  OnRoseQuestCutscene_00ee2db0 -> PlayCutscene, MakePrisonerComment -> MakeRacerComment.
- **Tests:** 12 tests link the row's object and call it under its old symbol; they gain a
  `#pragma comment(linker, "/alternatename:OLD=NEW")` line (same thiscall ABI). The members are private virtuals, so a test cannot
  call them by name.
  This is test-harness only. Row sources carry no pragma.
- **Callers:** none needed (`CALLERS.tsv`). Six already spell the PDB name. Two are rows of this wave. The other ten references lie
  past the landed caller's body end, so they belong to neighbouring functions that are not landed.
- **Bindings:** +93 ADD lines (`BINDINGS.tsv` and `BINDINGS_inherited.tsv`, de-duplicated). `bindings_multi_check.py` prints 0.
  - Not applied: the 12 CONFLICT lines. They are vtable symbols such as `??_7CBase@@6B@` that the landed objects from before
    the wave already emit.
  - Not applied: the RETIRE candidates.
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Left for later (`NAME_FIXES.tsv`)
- 00dcded0 (a wave 35a row) still declares CGuardTeamMember.
- Landed sources outside this wave still use the old spellings:
  - 0x01375748: 00e02950 00cfee30 00e569d0 00ea0b10.
  - The CScriptBase ctor 0x00CB8110: 00ed0e30 00e0e7b0 00cef9a0 00cbd510 00e9c300 00cdd2f0.
- Three callee names in the inherited rows are in no PDB (00ed43d0 00e7fd50 00e83230).
