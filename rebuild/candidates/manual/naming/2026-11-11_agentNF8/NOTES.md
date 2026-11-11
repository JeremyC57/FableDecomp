# agentNF8: definition-system binders onto shared headers

## Method
For each target, binders came from the scan (`work/scan.out`, `BIND BAD`) and the NF6/NF7 "binders left" lists. Each row I took
was rewritten as a member of its PDB class: `#include "engine/<Class>.h"` with `FABLE_RETAIL_*` opt-ins, and no local
`CDefinitionManager` / `CDefPointer` / pad structs. The GetDef / GetSubDef calls go through the shared templates, with
`FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY` / `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY`. Those are noinline bodies.
They emit the out-of-line COMDAT call retail has and keep its register allocation. Grading used `work/g.sh`
(`FABLE_HDR_OVERLAY=hdr`, verify_and_land parity + reloc_identity, BIND lines). `work/final.sh` regraded all changed rows
together. Its log is `work/final.log`: 24/24 RELOCATION_MATCH, g++ -std=c++11 PASS 24/24, VC7.1 test PASS 24/24, no
HARD / BIND BAD / RELOC_CONFLICT.

## Rows changed (24)
- CTCShop: 007721a0 IsPermanentlyStocked, 00772610 GetName (QBEKXZ), 007742c0 GetCurrentBuyPrice, 007743b0 GetCurrentSellPrice
- CTCHeroStats: 0057a67e GetMoralityMax, 0057a753 OnHealthModified (renamed), 0057a874 IsDrunk, 0057a89d GetMaxDrunkenness.
  These were free `__fastcall` stand-ins and are now PDB const members.
- CTCInventoryAbilities: 005f3e3e InitialiseExpressions, 005ee207 GetExpressionPDef
- CThingCreatureBase (GetSubDef<CAppearanceDef>): 00662a00, 00662b10, 00662c10, 00662c70, 00662d80, 00662ef0, 00662fa0, 00663260, 006632a0
- CTCLook::OnCreate 00753040; CTCWeapon::OnCreate 005da6e0; CTCVillageMember::OnCreate 007ee370; CTCSmashableBase::OnCreate 007eec90
- CThingSwitch::PeekDefinition 0083c1e0 (renamed from GetSwitchDef). It binds GetDef<CThingSwitchDef> 0x0083CFA0 and PeekDefinitionManager.

Self-symbol changes, where the old row had a wrong cv-qualifier, return type or class: 00772610, 007742c0, 007743b0, 005ee207,
00662a00, 00662b10, 00662ef0, 00662fa0, 006632a0 (its parameter is now `const CCharString&`, which the PDB and the AnimExists
callee require), 005da6e0 (EAE), 007eec90 (MAE), 00753040, 0083c1e0. Each one's grade compares against the retail bytes,
so all of them still RELOCATION_MATCH.

## Tests
- Kept, self-contained, still pass: 00772610, 005f3e3e, 005ee207, 00753040, 005da6e0, 007eec90.
- New `rows/<addr>_test.cpp`: all the others. Several landed tests modelled the old free-function shapes, or GXX_FAILed against
  the member form, so they were replaced. None of the changed rows is PREFAIL, and custom pass strings were kept where present.

## hdr/ overlay
Seven headers, additions only and each behind a new `#ifdef` (see hdr/README.md). They add CTCShop ATTITUDEPRICE,
CTCHeroStats DRUNKENNESS / ONHEALTHMODIFIED, CTCInventoryAbilities INITIALISE, CThingCreatureBase ANIMQUERY,
CAnimationSet QUERIES, CTCWeapon ONCREATE, and CThingSwitch PEEKDEFINITION.

## What is left and why (STATUS.tsv binders_left)
- **CThing-family virtual overrides and getters** (0x00662880/8e0/930, 0x00703xxx, 0x0082dxxx CThingBuilding, 0x0083bxxx,
  0x00832e60, 0x008338d0 ...). These are most of the PeekDefinitionManager / GetDef leftovers. They need CThingBuildingDef.h,
  CThingAICreature, and other headers that don't exist yet, or the row models a whole hierarchy locally.
- **No shared header for the class**: 007ef070 (CTCExpression), 007884f5 (CTCMeleeTrap), 0070b4d0 (CAnimateThing / CAnimInit).
- **Header debt inside an existing header**:
  - 005bf9e3 (SerialisedItems is opaque)
  - 005bdf08 (Categories / find_if)
  - 007498e0 (CTCNoise Definition is opaque)
  - 00744ed0 (setters are inline in the header, so the row body doesn't link)
  - 00772250 (CInventoryItemDef has no StatGetTypeName, so GETSUBDEF_BODY can't be used)
  - 004c04c0 (mesh effect)
  - 005eede2 (stand-in sink bodies)
- **Unclear instantiation**: 0083d900 / 00835b90 call 0x0083E0D0, whose T is not yet proven. 0041e6d3 calls 0x0042B144, a
  second GetDef instantiation. That row's COMDAT order (0x42AF3C, 42AF9E, 42B040, 42B0A2, 42B144, 42B1A6, 42B248) shows its T
  at the line-2168 site (weapon, then GetSubDef<CWeaponDef>) is not CThingBaseDef. It is still unresolved, so the row was left.
- **Preload rows** 006631c0 / 00663140 / 00663610: the CMeshDataBank / CThing::GetWorld spellings are inconsistent across the
  landed rows, so they need their own name pass first.
- **Script-interface rows** (GetDefinitionManager through script wrappers): deep local models, and no shared script headers.
- 0x013833D4 (`g_sellPriceScale_013833d4`) keeps its row spelling. No name for it is proven; other rows use
  g_TradingPriceMult / g_tradingPriceMult, typed differently.

## Descriptive names
None. Every name introduced is a PDB spelling (RENAMES.tsv: OnHealthModified, PeekDefinition).

## NAME_FIXES.tsv
Lists the correct PDB spelling for each callee address that other landed rows still bind under a different name, with the
wrong spellings seen and the rows that use them. The "(old row, now fixed)" entries are the spellings my rewrites replaced.
