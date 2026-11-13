# 2026-11-11_acctB_c2025 -- acctB hand-back, chunk 2025 (2 reserved rows: 2 matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00600B60 CTCInventoryMap::ConstructRegionDescriptionBuyableBuildings | RELOCATION_MATCH 1963v1963, test PASS (g++ and VC7.1) | plain C++, no device; written from the landed twin ConstructRegionDescriptionShops 0x006001E0 (same opt-ins, same same-TU GetDef / map::operator[] bodies under `#pragma auto_inline(off)`). 1 grade |
| 0x00605570 CTCInventoryMap::ConstructRegionDescription | RELOCATION_MATCH 1890v1890, test PASS (g++ and VC7.1) | **`// Flags: /d2inlT107` + `// Inline-budget:` line -- review at landing.** Retail expands GetDef<CUIMiscThingsDef> (c1 106) at its one site but calls CCountedPointer<CComponent>::Reset (c1 95) at all 8 sites; every Reset site is one arm of an `if (pc)` just before GetDef, so with score = c1*(1/f+2)/3 no control flow separates them at the default 74. With `while` search loops the only separating threshold is 107-108. The same TU's Shops / BuyableBuildings rows match at the default, so retail's compiler probably saw different costs here (an unfound header fact); the agent tried the CDefPointer release forms and CCountedPointer opt-ins it could find. 4 grades |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test runs,
read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT; RELOC_NAME label notices on 0x00617A90 and 0x00618420). Purity
and padded-struct greps are clean.

Review items for landing:

- 0x00605570's `/d2inlT107` (above). The row also has the region-info search loop twice (one per if/else arm, as retail has
  two complete copies) and a same-TU STLport 4.6 `__find` body under `auto_inline(off)`.
- NAME_FIXES.tsv (3): 0x005FB7E0 ConstructRegionDescriptionMapKeys is protected `IAE` with `CRegion&` in both PDBs (landed
  as public with `CRegion*`); 0x0050E540 is STLport `??$__find@PAJJ@std@@...` (landed as `Find_0050e540`, "Dinkumware
  _Find"); 0x00618420 is `vector<CUIRegionInfo>::_M_insert_overflow` (landed as `N00618420::vector<Element>`).
- Two wide literals in 0x00605570 (L"" 0x0122D70C, L"\n" 0x01231728) are bound in reloc_bindings to placeholder data names
  from other rows; vland passes.

To land: `hdr/` -- new `engine/CRegionDef.h` (CRegionDef on CParentDefClassBase, members at retail offsets from the landed
CRegionDef::Transfer 0x004715AB); opt-ins `FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL` (declarations) in
CTCInventoryMap.h and `FABLE_RETAIL_CTCBUYABLEHOUSE_ISFORSALE` in CTCBuyableHouse.h; evidence in `hdr/README.md`. Rebuild the
oracle from Fable.exe (no retail bytes are in this folder).
