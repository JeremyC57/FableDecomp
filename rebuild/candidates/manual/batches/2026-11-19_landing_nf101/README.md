# 2026-11-19 landing nf101: naming round 9, NF101 (NF98 leftovers, 9 rows)

Records: `naming/2026-11-19_agentNF101/` (selfcheck 0 problems). 8 entries: 4 done (q11260, q11274, q11277, q11286), 1 done-already
(q11271), 3 blocked: port (q11280 / q11283: 0x0049C6F0 still has 57 landed binders on stand-ins; q11281: 0x0078954D DIFFER 640v354,
retail calls five helpers out of line that the shared header expands, plus 0x0078971D).

## Rows (9): RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Self renames (RENAMES_applied.tsv, 2 file moves): 0x0078B070 / 0x00848500 PerformAction, 0x00B9DC60 IsInsideFrustum, 0x00518130
  ForEachInRoughArea<..>, 0x005185F0 GetAllTCDsInRoughArea<..>.
- Ports onto the shared headers: 0x0078B070, 0x00848500, 0x008C64F0 (CThingObject::Create), 0x00B9DC60 (GetMaxDrawDistance), 0x00518130 /
  0x005185F0 / 0x0063FAE0 (GetAllThingMapsInArea, ClipAndConvertWorldMapAreaToLocalArea, the CThingSearchTools statics, NThingMap::LayerInfo),
  0x00670D50 (CValidateDefinitionInit), 0x0055FC90 (GetSurfaceLevel const, in-place respell).
- Headers (additive opt-ins, default path unchanged): CTCWeapon.h `GETMISSILELOADCARRYSLOT`, CCreatureAction_HeroLoadRangedWeapon.h `_CLASS`,
  CThingSearchTools.h `TCDSINROUGHAREA_EGOD`, CThingCreatureDef.h `VALIDATE`. assert_names_check 0 mismatches.

## Checks
bindings_multi_check 1 (NullCoord baseline); funclet_includes_check 1355 / 0 broken; includers 443 good / 0 bad (`logs/includer_recheck.txt`).
**No-new-split gate (new, advisor 2026-11-19; `$S/w22/nftools/gate.py`):** reloc_bindings.tsv keeps one witness row per symbol, so the
gate scans Fable.exe for every landed row that calls or addresses each respelled target (22 targets, 241 other rows; 101 from the
includer recheck objects, 140 compiled, all MATCH / RELOCATION_MATCH) and compares the spellings they bind: 0 created splits. The 12
already-split addresses and the rows that still bind old spellings are in `logs/split_gate.txt` (work for the next naming round).

## Needs a header claim (NAME_FIXES / NOTES)
0x005DA920 is `?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ` (self row still the stand-in GetWeaponDefLongAt94; 0x00789FF0 binds
GetAmmoCarrySlot); the CThingSearchTools.h TCSINROUGHAREA_BODY copy spells 0x013B89A8 / 0x013B89AC / 0x013B89B0 / 0x0137DAD8 with stand-ins.
