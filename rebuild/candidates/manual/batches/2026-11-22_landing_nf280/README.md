# Landing nf280 (Account A, 2026-11-22): 0x00416E78 GetInputs onto the shared headers with its callee family (4 rows, 1 test, 3 headers)

Agent NF280 (entry 1 done; entries 2 / 3, the `_M_erase` split and 0x00ADCE60, not started). Hand-back: `pending/2026-11-22_nf280_handback/`.
- **0x00416E78** `?GetInputs@CMainGameComponent@@QAEXXZ` -> `MAEXXZ` (both PDBs; slot 6 of both emitters 0x00418DCA / 0x00418B7A now resolves): every
  local model gone, on the headers for CMainGameComponent, CWorld, CAIGameCameraBase, CCamera, CGameEvent, CChecksum, CInputManager, CSystemManager and
  CGamePlayerInterface. RELOCATION_MATCH on the first form; the relocation diff against HEAD is exactly four callee targets (0x00415FE0 / 0x00415FF2
  QAE -> IAE; 0x004457F0 `?BeginEventPoll@CGamePlayerInterface@@QAEXXZ` -> `?Update@CGamePlayerInterface@@QAEXXZ`; 0x009F4A90 PAV -> ABV
  `?UpdateChecksum@CInputManager@@QAEXJABVCChecksum@@@Z`). New copies are header inlines with no owner; `??0CGameEvent` EQ to 0x004159A0 as at HEAD.
- **0x004457F0** `?CXMVPlaybackInfo@@YIXPAUCXMVPlaybackInfoObj@@@Z` -> `?Update@CGamePlayerInterface@@QAEXXZ` (both PDBs; Ego_r order Update ->
  GetButtonPressureForEvent -> GetMouseMovement as retail; the body clears HasCalledInputUpdateThisFrame +0x894; the other `void()` members ruled
  out); catalog `_global::SetMemberConst_004457f0` -> `CGamePlayerInterface::Update` (file renamed). MATCH. New test links the row (plain C++,
  replaces the HEAD standalone test and its `__asm` block); PassPattern unchanged.
- **0x00415FE0** `?ProcessEvent@T@@...` -> `?ProcessEvent@CMainGameComponent@@IAEXPBVCGameEvent@@@Z`; **0x00415FF2** `?IsSystemEvent@T@@...` ->
  `?EventIsSystemEvent@CMainGameComponent@@IAE_NPBVCGameEvent@@@Z`. MATCH each.
- **Headers** (add-only): `FABLE_RETAIL_CMAINGAMECOMPONENT_PROCESSEVENT`, `FABLE_RETAIL_CINPUTMANAGER_UPDATECHECKSUM`, `FABLE_RETAIL_CCHECKSUM_CTOR_THREE`
  (FableWin `??0CChecksum@@QAE@JJJ@Z`; 5 asserts pinned). Whole-tree equiv_check 57,125 rows, 0 differ.
- **Census:** 0x00416E78 2 -> 1, 0x00415FE0 3 -> 1, 0x00415FF2 3 -> 1, 0x004457F0 2 -> 1, 0x009F4A90 2 -> 1; no new split; each new name one body; link 0 duplicates.
- **Table:** four stand-in lines removed (the QAE ProcessEvent / EventIsSystemEvent lines whose only witness was 0x00416E78, BeginEventPoll, the PAV
  UpdateChecksum); two PDB lines added (`?Update@CGamePlayerInterface@@QAEXXZ`, the ABV UpdateChecksum). funclet_includes_check 0 broken.
- **Recheck** (`logs/recheck.log`, landed tree): the 4 rows + 0x009F4A90, both emitters 0x00418DCA / 0x00418B7A, the IAE witnesses 0x00416670 / 0x0041649C
  and 0x00417747 (10 rows): 10 PASS (5 RELOCATION_MATCH, 5 MATCH), 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF280's questions
1. Lands as one set (rows, opt-ins, RENAMES, TABLE_CHANGES, the 0x004457F0 file rename and new test).
2. NAME_FIXES queued: 0x013B8388 `?GPInputManager@@3PAVCInputManager@@A` (both PDBs) v CInputManager.h's `PInputManager` (existing data-name split).
3. / 4. Test debt (0x00416E78's HEAD test is a copy of the old source) and header debt (CInputProcessManager.h uses CEnginePrimitivePrepareInfo
   undeclared) queued.
5. Entries 2 / 3 (0x00ADC190 / 0x00ADC250 / 0x00ADC4E0 / 0x00ADCB90 `_M_erase` port; 0x00ADCE60) stay queued.
Slips (one scratch file under /tmp; a background `tail --pid` watcher; clang prechecks during the equiv_check; `FABLE_RETAIL_CNETWORKCLIENT_UPDATE`
carried with 0x00417747's define block): noted.
