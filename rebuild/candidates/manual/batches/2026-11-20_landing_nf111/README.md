# 2026-11-20 landing nf111: PRIORITY table slice 3 (34 rows)

Records: `naming/2026-11-20_agentNF111/` (selfcheck 0 problems). 30 entries: 28 done, 2 open.
- q4186 (0x00567400) open: a 9-row `basic_string::append<char*>` family bound under 5 spellings; all 9 rows must move onto stlport/fable_string.h
  together (its own pass).
- q4333 (0x00528760) open: the self row lands on the claim; 0x0051FD80 / 0x00767200 already bind it; 0x007F1B70 (CTCDRumble::Create, an all-local
  model) still binds `?GetDef@CGameDefinitionManager@NFactory007f1b70@@...`, so the address stays the existing two-spelling split (gate: existing,
  not created). Porting 0x007F1B70 is an authoring-size job.

## Rows (34): 9 MATCH + 25 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
33 self renames (RENAMES_applied.tsv). 0x0052CC50 / 0x0055B760 were not binders (their rows already emit the claim). 0x0054E1C0 / 0x0053E8E0
(the CFrontEndButton / CMenuEntry scalar deleting destructors) moved with the two QAE -> UAE destructor fixes so no split is created; self rows
define `??_G` and the thunk rows bind its `??_E` alias (precedent 0x00546F00).
- **Flags carried into CompilerFlags:** 0x0054B360 ~CSlider, 0x0054DED0 ~CFrontEndButton, 0x0054E1C0 `/Gr` (STLport `__destroy_aux` at
  0x0054A1C0 is __fastcall in retail).
- Headers: CThingManager.h `FABLE_RETAIL_CTHINGMANAGER_REMOVEFROMPHYSICALLIST`, CThingPhysical.h `FABLE_RETAIL_CTHINGPHYSICAL_HEALTHOVERTIMEHELPER`;
  add-only, pin_asserts 0, assert_names_check 0.
- Needs a header claim (not applied): engine/NUISystem_CText.h declares `GetValue() const`, FableWin.pdb names it non-const (UAE); engine/CText.h
  already has it right.

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 295 good / 0 bad. No-new-split
gate (before set compiled from HEAD at HEAD flags): 0 created. Known data split outside the slice: 0x0129BA3C bound as `?DELTA_NOTIONAL_ZERO@@3MB` and
labelled `?g_NormaliseEpsilon@@3MB` (pre-existing).
