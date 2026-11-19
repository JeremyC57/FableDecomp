# 2026-11-19 landing nf103: naming round 10, search tools and NThingMap (78 rows)

Records: `naming/2026-11-19_agentNF103/` (selfcheck 0 problems). 5 entries: 2 done (q11463, q11533), 2 blocked: port (q11280 / q11283),
1 open (q11281).

## Done
- **q11463 NThingMap::LayerInfo 0x0137DAD8:** NF100's 18 leftover `g_MapwhoLevels` rows now bind `?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B`;
  an exe scan of absolute references finds no landed row left on a stand-in. The stand-in declarations in NThingMap_CThingMap.h's
  default branch are retired (header claim posted to Account B; `acctb_hits.txt` lists 13 B workspaces and 40 old `pending/` records that
  still name them).
- **CThingSearchTools statics 0x013B89A8 / AC / B0:** every landed binder the exe scan finds now binds `NoLookups` / `NoThingsScanned` /
  `SearchID` on CThingSearchTools (48 local-model rows declare them inside their existing model; 26 `_Calls` rows take them from the header
  through the new opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_STATICS`; 0x004162B5 likewise); the header's own body copies use the PDB names.
  0x00435530's test defined the retired stand-ins and now defines the PDB names (PASS).
- **q11533:** 0x005DA920 ported onto CTCWeapon.h / CWeaponDef.h as `?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ` (MATCH, file moves);
  0x00789FF0 respelled in place.
- 0x00518900 ported as an explicit instantiation (0x005185F0's define set); its leaf is now what its caller 0x0050A650 binds.

## Left
q11280 / q11283: 56 rows still bind the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins (`q11280_rows_left.txt`:
25 `_Calls` rows, 31 local CThingSearchTools models); not attempted (context), each needs a port. q11281: 0x0078954D / 0x0078971D call
GetMapwhoArea out of line; needs a declared-only opt-in first (NOTES).

## Checks
78 rows MATCH / RELOCATION_MATCH, every test PASS (`logs/recheck.log`); assert_names_check 0 mismatches; bindings_multi_check 1 (NullCoord);
funclet_includes_check 1355 / 0 broken. The NThingMap_CThingMap.h default path changed, so every landed includer of both headers was rechecked
(plus the rows the exe scan finds referencing a respelled target): 275 good / 0 bad; no-new-split gate 0 created (`logs/includer_recheck.txt`, `logs/split_gate.txt`).
