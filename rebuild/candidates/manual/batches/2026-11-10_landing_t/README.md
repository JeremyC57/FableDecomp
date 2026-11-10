# 2026-11-10 landing t -- chunk 1002b, 6 more CTCInventoryWeapons rows

Source: `pending/2026-11-10_linux_hand1002b/`. vland --land: 6 RELOCATION_MATCH, tests PASS (VC7.1 and g++). With batch r the
CTCInventoryWeapons TU now has 14 of its 18 rows landed.

- 0x005C3E91 FinalConstructWeaponItemEntries, 0x005C47D5 FinalConstructWeaponItemEntriesPC, 0x005C4F42 ConstructAugmentationsLeftDescription,
  0x005C574E ConstructAugEntry, 0x005C5F11 ConstructAugEntryPC, 0x005C6B26 ConstructWeaponDescription (retail_asm: GFFloatToLongNear,
  `// Retail-asm:` fistp 0x005C6CBA / 0x005C6CCC). Levers per row in the pending NOTES / LESSONS.
- Headers (opt-in, default paths unchanged): new NUISystem_CActionParamCCountedPointer.h, EObjectAugmentationType.h; CAugmentationDef.h
  _CAUGMENTATIONDEF, NUISystem_CMenuEntry.h _CMENUENTRY_ACTIONLISTS, CTCInventoryItem.h _STATICENGINEGRAPHIC, CEngineGraphic.h
  _CENGINEGRAPHIC_CTOR, CStockItemDef.h _CSTOCKITEMDEF, CTCShop.h _CTCSHOP_INTERFACETYPE.

Parked: 0x005C71AA DIFFER(680v680) (one block's schedule; c2prio: the second "none" copy drops const 0's priority 25 -> 22, so the
count spill never happens). Not authored: 0x005C90FC, 0x005CACB9, 0x005CA68E (NOTES lists the headers they need). Queued `land1011u`:
7 name fixes + the unresolved 0x005BE8D3 GetPerItemDataForItem / GetBoughtPrice question.
