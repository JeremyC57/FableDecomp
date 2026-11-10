# 2026-11-10 landing ad -- chunk 1009b, retry of the CTCInventoryTrade rows (4 rows)

Source: `pending/2026-11-10_linux_hand1009b/`. vland --land: 4 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x0059D270 AddWantedItemToListPC, 0x005A3E47 ConstructBuyActionsList, 0x005A79E6 ConstructBuyActionsListPC: the chunk 1009 local-model
  ports rewritten on shared headers (no local CTCInventoryTrade / CComponentMap / operator[] stand-in); manifest names corrected (PC suffix).
- 0x005A29F4 ConstructSpecialShopList: FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE, the subtitle as a named CText* before SetName / SetValue,
  "Trade" as a block-scoped named CCharString.
- Headers applied (opt-in only): CTCInventoryTrade.h ACTIONSLISTS / SHOPLISTS, NInventory_CTCInventoryBase.h CONTAINABLE
  (GetAmountOfItemsContainable 0x005BF30D). The other overlay headers (CTattooDef MEMBERS, CTCHero TITLEOBJECT, CTCHeroAttachableAppearance-
  Modifiers WEARING, new CTCHeroMorph.h) serve only parked drafts and stay in the pending hdr/.

Parked (differ/): AlreadyHaveSpecial DIFFER(725v716), AddBuyItemToList DIFFER(1552v1556), AddBuyItemToListPC DIFFER(1669v1662),
ConstructNormalShopListPC DIFFER(1336v1336) (sum operand order), ApplyTattooCard (not reworked). Held: 0x005ACEDA / 0x005A94BD (OpenInventory
pair; CASoundManager SetGroupVolume). Still local-model debt: the landed twins 0x0059CB90 / 0x005A45F6. Queued `land1011ad`.
