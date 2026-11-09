# Chunk 891 header overlay (additive opt-ins; default paths unchanged)

- engine/CTCShop.h: `FABLE_RETAIL_CTCSHOP_WANTED` declares `std::vector<long> CTCShop::GetWantedItemsList() const` (0x00774100;
  Ego_r.pdb + FableWin.pdb ?GetWantedItemsList@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ). Declaration only.
- engine/CTCInventoryTrade.h: `FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY` declares the three PC inventory builders
  (ConstructActualSell/Wanted/BuyInventoryPC 0x005A4CDD / 0x005A5B31 / 0x005AC10E, private AAEXXZ) and the helpers they call:
  AttachToFrame 0x0059BB1B, AddWantedItemToListPC 0x0059D270, AddSellItemToListPC 0x0059F51E, InitialiseSellableItems 0x0059D820
  (public QAE) and ConstructSubtitlesPC 0x0059E4D7 (private, std::map<CComponent*,pair<CWideString,long> >&). Ego_r.pdb +
  FableWin.pdb spellings; declarations only, no layout change.
Used by src/005a5b31.cpp (matched) and differ/005a4cdd.cpp, differ/005ac10e.cpp.
