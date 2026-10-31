# Chunk 618 notes (10 parked rows at retail length)

## Results
- 0x0071BA60 `?GetMorphStrength@CTCHeroMorph@@QBEMW4EHeroMorphType@@@Z` RELOCATION_MATCH 624v624 (vland PASS, no jump-table warnings).
  Lever, case 9: `float difference; return GFLimitReturn(difference = GetMorphStrength((EHeroMorphType)8) - Fatness, 0.0f, 1.0f);`.
  Retail stores the 1.0f/0.0f temporaries before the recursive call and lands the difference in the block-local slot shared across
  cases. GFLimitReturn<float> 0x0057EF86 = `??$GFLimitReturn@M@@YIABMABM00@Z` (both PDBs, reloc binding).
- 0x00849700 `?PerformAction@CCreatureAction_StealThingFromShop@@UAEXXZ` RELOCATION_MATCH 719v719 (vland PASS). Incremental changes
  on the parked source: the landed sibling 0x00849380 CreateThingAt helper (defIndex local, three GetPItem fetches first),
  CThingObject::Create PDB spelling, named `component` / `manager` locals, a ternary GetInterface on the non-const path and an
  if-form one on the const path. Full sibling-model rewrites (7 probes) grew the body to 711-725 bytes.
- Parked, best source in differ/ (line 2 has the residue):
  - 007993f0: 698v698. The CDefPointer hidden return sits in the deed slot. Respelled to (const COpinionDeed&, long).
  - 006668a0: 673v673. Improved from 686v673 / 81 lines to 4 lines.
  - 009644f0: 709v709. Improved from 76 to 56 lines; ebx/ebp swap left.
  - 005c71aa: 680v680. Improved from 12 to 10 lines; the early-return threading is fixed.
  - 00542780: 682v682, load order. Orig is still best after 7 forms.
  - 0047f30f, 00576c4f, 00576ed1: the shared push ecx / fsubp lane gap.

## Names (two sources each; see NAME_FIXES.tsv)
- All 10 own symbols are proven: the full mangled Leaf is in every src/ and differ/ file, and q.py binds each with no WARN.
- 0x00849700 callees respelled to PDB names:
  - IsSpaceInInventoryForItem 0x005BF2ED (was the descriptive CanAddItem).
  - GetMaxNumberOfItems 0x005D9050 (was the descriptive GetMaxNumberItems, as in the landed sibling 0x00849380).
  - Evidence for both is in the src Callees block and NAME_FIXES.
- Catalog labels proven wrong (vland RELOC_NAME):
  - 0x005D9030 and 0x005D9050 are labelled CCategory::GetCategoryIdentifier (byte-identical-body propagation). They are
    CTCInventoryItem::GetInventoryType and CTCInventoryItem::GetMaxNumberOfItems.
  - 0x005BF2ED is labelled GetItemDescription / AcceptsItem. It is CTCInventoryBase::IsSpaceInInventoryForItem.
  - 0x006668A0 is labelled Animation_FilterAndCompile. It is CThingCreatureBase::GetPossibleCombatAnimations.
- 0x00703210 CThingObject::Create: the landed sibling 0x00849380 spells it `?CreateThingObject@@YI...`, which is wrong. Respell
  it at landing.
- Conflict, NOT resolved: 0x0041E5F2.
  - labels_chunk291_corrections says NUISystem::CFrontEndManager::GetInstance.
  - labels_chatgpt_pass8 and the parked 0x0047F30F family say NUISystem::CManager::GetInstance.
  - Both mangled names exist in both PDBs, so the coordinator should resolve it once. The 00542780 differ file uses
    CFrontEndManager.
- 0x004C7A70: the PDB has `?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ` (const, returns a reference). The 00849700 src
  declares it non-const returning a pointer, so its relocation binds by address only. Respell at landing; a reference return
  is equivalent in code.

## Header debt
- 00849700 (vland SHARED_HEADER_AVAILABLE): CTCInventoryItem, CSubtitleRenderer, CPlayerGui and CWorld have engine headers but are
  modelled locally. Switching to the headers moved register allocation in the k-series probes (the sibling-model
  rewrites), so it was left for the coordinator. CTCOwnedEntity is modelled packed (POwner +0x0D), as in the landed
  0x0075D9A0.
- 0071ba60: CTCHeroMorph is modelled locally.
- 005c71aa / 00542780: local CDefPointer / CResource / CGraphicInfo models predate engine/CDefPointer.h.
