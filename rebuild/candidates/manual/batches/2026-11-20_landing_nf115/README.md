# 2026-11-20 landing nf115: PRIORITY table slice 5, first hand-back (24 rows)

Records: `naming/2026-11-20_agentNF115/` (selfcheck 0 problems). 31 entries: 12 done (q4423, q4461..q4466, q4488, q5218, q5219, q6411, q6540;
q5218 on an amended spelling: retail's `ret 0Ch` shows the fifth parameter `const forward_iterator_tag&` the posed symbol leaves out, REJECTED.tsv),
2 blocked: port (q4495 0x0090C1A0, q4497 0x0065F6A6), 17 open (14 not started, 3 partly done): handed to NF119.

## Rows (24): 2 MATCH + 22 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
13 self renames, 20 caller respellings; 0x006724B0, 0x00661D20, 0x006627F0 only got their `// Leaf:` lines completed. Folds checked: 0x00662010
GetProperty is not a fold (the same body as the virtual CThingAICreature::GetProperties sits apart at 0x00830140).
- Headers: CThingSearchTools.h `FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST` (retail's null test before the filter; rows 0x0065F744,
  0x00659800), CThingCreatureBase.h `FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETPROPERTY` (declaration only). Default path token-identical, pin_asserts 0,
  assert_names_check 0.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 1159 good, 1 pre-existing (0x007E2A60).
No-new-split gate 0; table spelling sets: 0x00662010 4 -> 1, 0x00685F80 2 -> 1, 0x00662930 2 -> 1, 0x006627F0 5 -> 4, and **0x005ECF50 2 -> 3**:
0x0069C3D0 moved off the invented `?Destroy_005ecf50@@YIXPAX@Z` to the FableWin.pdb `?DeleteData@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@SIXPAX@Z`
while 0x005ED270 keeps the stand-in and five rows bind `DeleteObject`. No address went from one spelling to two (the rule holds); the third spelling
is the PDB one, and moving the other six binders to it is queued (NAME_FIXES 0x005ECF50).
