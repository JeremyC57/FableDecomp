# Header overlay -- chunk 1009b (export FABLE_HDR_OVERLAY=$A/hdr for every compile / grade)

Copies of `rebuild/include/engine/*.h` with declaration-only retail opt-ins added; every addition sits behind a NEW
`#ifdef FABLE_RETAIL_*` macro, so the default path (and every landed row) compiles exactly as before. One new file.

| file | opt-in | adds | used by |
|---|---|---|---|
| engine/NInventory_CTCInventoryBase.h | `FABLE_RETAIL_CTCINVENTORYBASE_CONTAINABLE` | `long GetAmountOfItemsContainable(long itemDefIndex) const;` 0x005BF30D (`?GetAmountOfItemsContainable@CTCInventoryBase@NInventory@@QBEJJ@Z`, Ego_r + FableWin) | src/005a3e47, src/005a79e6 (MATCH) |
| engine/CTCInventoryTrade.h | `FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS` | public `IsItemAffordable(long,long)` 0x0059DA10; `ConstructBuyActionsList(PC)` 0x005A3E47 / 0x005A79E6 and `ConstructSellActionsList(PC)` 0x005A45F6 / 0x005A731F `(CComponent*, long, long, bool)` | src/005a3e47, src/005a79e6 (MATCH), differ/0059fee3 |
| engine/CTCInventoryTrade.h | `FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS` | `ConstructNormalShopList(PC)` 0x005A6769, `ConstructSpecialShopList(PC)` 0x005A29F4 / 0x005A8A5D, `AddBuyItemToList` 0x0059FEE3 | src/005a29f4 (MATCH), differ/005a6769, differ/0059fee3 |
| engine/CTattooDef.h | `FABLE_RETAIL_CTATTOODEF_MEMBERS` (nested in `FABLE_RETAIL_CTATTOODEF`) | Ego_r members at retail offsets (BankIndex +0x34, RemovesAllTattoos +0x3C, Permanent +0x3D, CustomFileName +0x4C, sizeof 0x60, static-asserted) + `IsConflicting(const CTattooDef&) const` 0x0059B679 | differ/0059fc17 |
| engine/CTCHero.h | `FABLE_RETAIL_CTCHERO_TITLEOBJECT` (nested in `FABLE_RETAIL_CTCHERO_TITLE`) | splits the opaque tail: `long Title` +0x10, `long HeroTitleObjectDef` +0x14, `_tail[0x20]` (sizeof unchanged 0x38) | differ/0059fc17 |
| engine/CTCHeroAttachableAppearanceModifiers.h | `FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_WEARING` (needs `_LIST` for the enum) | `IsWearingAppearanceModifier(long, EHeroAttachableAppearanceModifierType) const` 0x00706000 | differ/0059fc17 |
| engine/CTCHeroMorph.h (NEW) | `FABLE_RETAIL_CTCHEROMORPH` | no promoted engine header exists (the generated one is in engine/_quarantine/); `class CTCHeroMorph : public CTCBase` with `StatGetInterfaceType()` = TCI_APPEARANCE_MORPH (3, CTCAppearanceMorphBase's key), `GetTattoos(vector<CDefPointer<const CTattooDef> >&) const` 0x0071DD50, `HasAPermanentTattooCoveringArea() const` 0x0071C2D0; without the macro only a forward declaration | differ/0059fc17 |

Landing note: the three MATCH rows only need the first three opt-ins (CONTAINABLE, ACTIONSLISTS, SHOPLISTS); the rest serve the
parked AlreadyHaveSpecial draft and can wait until that row lands.
