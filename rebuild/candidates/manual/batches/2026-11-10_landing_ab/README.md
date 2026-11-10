# 2026-11-10 landing ab -- chunk 1009, the CTCInventoryTrade translation unit (5 rows)

Source: `pending/2026-11-10_linux_hand1009/`. vland --land: 5 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x0059BB1B AttachToFrame (optimize("s"), the landed 0x005A5B31 opt-ins, argument named before the virtual call), 0x005A3734
  RefreshBuyPossibilityNumbers / 0x005A8175 ...PC (built from landed BuyItem's sell-side code; one named bool used twice gives retail's
  `sete al; test al,al`), 0x005A9A87 RefreshBuyPossibility (catalog placeholder "Inventory_PopulateBuyList"), 0x005B0AAC BuyItemPC (catalog
  "CTCInventoryTrade::CTCInventoryTrade::BuyItemPC"; body ends at 0x005B109A: 1518 bytes, BOUNDS_FIX split -- the 15 COMDAT stubs after it
  still need manifest starts).
- Coordinator fix: 0x005B0AAC bound the const GetMainPlayer to 0x00449970 (RELOC_CONFLICT with 0x007B0880). Retail calls the non-const
  0x00449970; the row now reaches it through GetPlayerManager() (rechecked RELOCATION_MATCH, logs/vland_recheck_getmainplayer.log).
- Header: CTCInventoryTrade.h opt-ins BUYITEMPC / BUYPOSSIBILITY (declarations only).

Not landed: 0x0059D270 AddWantedItemToListPC, 0x005A3E47 ConstructBuyActionsList, 0x005A79E6 ConstructBuyActionsListPC matched, but as
ports of landed local-model twins (local padded CTCInventoryTrade, invented CComponentMap / operator[] stand-in, CActionParamList):
`rewrite/` for shared headers. Parked (differ/): AlreadyHaveSpecial, AddBuyItemToList, ConstructNormalShopListPC, ApplyTattooCard.
Unworked: ConstructSpecialShopList, OpenInventory 0x005A94BD, ConstructActualInventoryPC 0x005ACEDA, AddBuyItemToListPC.
