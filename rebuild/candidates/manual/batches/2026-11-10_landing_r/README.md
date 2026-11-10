# 2026-11-10 landing r -- chunk 1002, the CTCInventoryWeapons TU (8 of 18 rows)

Source: `pending/2026-11-10_linux_hand1002/`. vland --land: 8 RELOCATION_MATCH, tests PASS. Six of the eight were earlier "register-tie"
or "ltcg/crt/asm-impossible" parks that matched once the shared headers (engine/CTCInventoryWeapons.h from agentSCR) were in place.

- 0x005C4671 GetFreeAugmentationSlotForWeapon, 0x005C66B2 ConstructWeaponItemEntries, 0x005C7452 ConstructWeaponDescriptionDamage,
  0x005C8101 PutWeaponsInSheathes, 0x005C8176 ConstructWeaponItemEntriesPC, 0x005CB128 RemoveItemFromInventory (vtable slot 79),
  0x005CB2B0 SetThingAsActiveWeapon, 0x005CB974 OnInitialActivate (slot 6). Levers per row: the pending folder's NOTES / LESSONS.
- 0x005C7452 calls the shared asm helper engine/GFFloatToLongNear.h (grade `retail_asm`); its `// Retail-asm:` line (retail `fistp`
  at 0x005C75CA / 0x005C75DC) was added at landing, which the purity check requires.
- Headers (opt-in, default paths unchanged): CTCInventoryWeapons.h, NInventory_CTCInventoryBase.h, CTCCarrying.h, CTCInventoryItem.h,
  CTCWeapon.h; new CTCObjectAugmentations.h, CObjectAugmentationsDef.h, CEngineGraphic.h, CTCCarryable.h (hdr/README.md).

Not landed: 0x005C71AA ConstructWeaponDescriptionNoDamageNorWeight parked at DIFFER(680v680) (one argument-push order in the
TEXT_GUI_MENU_NONE block); 9 rows not attempted (they need more headers; NOTES.md). Queued `land1011s`: 12 name fixes
(0x005C552C RemoveWeaponsFromSheathes, CTCHero +0x0C WeaponsUsable, and binding names such as 0x005BE8F3 CreateInventoryObject).
