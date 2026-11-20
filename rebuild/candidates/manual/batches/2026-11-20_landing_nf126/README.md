# 2026-11-20 landing nf126: PRIORITY slice 7, second remainder (19 rows)

Records: `naming/2026-11-20_agentNF126/` (selfcheck 0 problems). 6 entries (NF124's leftovers), all still open, moved to NF128 with progress:
q6454 0x0073AAC0 (11 of 14 binders on `?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ`), q6453 / q6455 0x0073A680 (4 of 13 on the nested
GetWorld claim), q4802 0x0075B8D0 (0x00B20FD0 finished + 5 rows on engine/CCardinalSpline.h; 0x00B21DD0 / 0x00B21E90 blocked), q6408, q4747.

## Rows (19): 19 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 11 self-symbol changes (RENAMES s1): nine leaf-only QAE -> UAE PDB virtual overrides (0x008DC600, 0x008D9480, 0x008D89C0, 0x008DE6A0,
  0x008D9E30, 0x008DBB30 + 0x008DC6C0 as a 2-row family, 0x008DA2E0, 0x008E7390; each in both PDBs, no other referencing row);
  0x008E7290 moves from the global `?OnActivate@CModeDragonBase@@QAEXXZ` onto engine/CModeDragonBase.h (module now
  NCreatureMode::CModeDragonBase, file moved); 0x008D6760 moves from an extern "C" forwarder to the PDB member.
- Advisor decision (owner delegated): 0x008D6760 defines a local `NCreatureMode::CModeExplosiveForcePushed : CTCCreatureModeManager::CModeBase`
  where HEAD had no class. Accepted under a narrow rule: no header for the class exists anywhere under rebuild/include (grep of every class
  the 19 rows newly define: this is the only one), the landed siblings 0x008D6820 / 0x008D6780 use the same shape, and the base comes from
  a shared header. A "base-list local model" (never allowed) is a local redefinition of a class that HAS a shared header.
- 25 caller respellings (section 2); 0x00B22310 / 0x00B22530 / 0x00B22100 also bind the self-leaf STLport spellings of 0x006F0810,
  0x0045BC09 and 0x0075C040.

## Headers (add-only opt-in, default path token-identical; pin_asserts pinned 0; assert_names_check 0 mismatches)
CWorld.h FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK (inline GetMeshBank in the flattened default class, FableWin; no layout change).

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. 748 rows rechecked: 747 good, 1 bad = 0x007E2A60 DIFFER(707v707), pre-existing
(on hold). No-new-split gate 0. Table spelling sets: 0x0073A680 8 -> 6, 0x0073AAC0 4 -> 3, 0x00A567A0 3 -> 2, 0x00B15BD0 2 -> 1; 0x00669580
1 -> 3 is new witness rows only (0x008D9E30 and 0x008DDD70 bind the same GetSubDef spellings before and after: existing split).
Not applied: header changes for 0x00B21DD0 (copy construct that zeroes then assigns) and 0x00B21E90 (inline __copy in erase): "needs header
claim" items for NF128; 0x00B20B60 `CSplineRandomOffsetData::operator=` is one-source (NOTES only).
