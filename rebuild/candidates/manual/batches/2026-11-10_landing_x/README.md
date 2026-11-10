# 2026-11-10 landing x -- chunk 1007, the CTCInventory translation unit (5 rows)

Source: `pending/2026-11-10_linux_hand1007/`. vland --land: 5 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x00583E26 CTCInventory::OnItemSelected (vtable slot 33): visible `IS_THING_ALIVE<CThing*>` under `auto_inline(off)`; case 1 as a nested
  `if` with the return-to-inventory block written in both else arms.
- 0x00587BFF OpenPCSkillsMenu / 0x0058A763 OpenPCStatusMenu: shared headers in the landed OpenPCOptionsMenu 0x0058AF10 spelling.
- 0x0058A0C7 ConstructItemEntry / 0x0058E208 ConstructItemEntryPC: rewritten on shared headers (CMenuEntry, CList, CActionParamSLONG, the
  PDB reference parameter) in the landed FinalConstructWeaponItemEntries 0x005C3E91 spelling; visible STLport `map[k] = ptr.Get()`;
  the PC twin caches the sprite pointers as `CSprite*` locals.
- Headers (opt-in only, default paths unchanged): CTCInventory.h ITEMSELECTED / OPENPC / ITEMENTRY / CONSOLEMENUS, CTCInventoryItem.h
  USEBUTTONACTION, CTCInventoryMap.h PCMAPLIST, NInventory_CTCInventoryBase.h HIGHLIGHTEDITEM / CAPTUREBACKBUFFER (hdr/README.md).

Parked (differ/): AssignThingPC, GetStyleCardsListUIComponent(PC), OpenPCMapMenu, ConstructItemCategoryList, OpenPCInventory,
GetItemsListUIComponent(PC), UpdateContextSensitiveItems; not attempted: OpenPCInventoryMenu, OpenPCPhotojournal. Relaunched as chunk 1007b.
Queued `land1011y`: 0x005D9710 GetUseButtonActionType, 0x0060EEB0 ConstructPCMapList, 0x005BC6ED CaptureBackBufferToTexture,
0x00589402 GetStyleCardsListUIComponentPC, 0x0058A724 ConstructStyleCardsScreen.
