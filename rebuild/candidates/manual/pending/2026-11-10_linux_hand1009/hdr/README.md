# Header overlay, chunk 1009

`engine/CTCInventoryTrade.h` is the repo HEAD file with two declaration-only opt-ins added. Both sit just before the existing
`public: std::vector<CEnginePrimitiveHandle> TradeAnimPrimitiveHandles;` line. Neither changes the layout: `sizeof` stays 0x230 and
every static assert is unchanged.

| Opt-in | Declares | Used by |
|---|---|---|
| `FABLE_RETAIL_CTCINVENTORYTRADE_BUYITEMPC` | public `BuyItemPC(CComponent*, long)` 0x005B0AAC (FableWin.pdb `?BuyItemPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z`) and `RefreshBuyDescriptionsPC(CComponent*)` 0x005AE9DE (landed) | src/005b0aac.cpp |
| `FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY` | private `RefreshBuyPossibilityNumbers()` 0x005A3734 (`?RefreshBuyPossibilityNumbers@CTCInventoryTrade@@AAEXXZ`) | src/005a9a87.cpp (the caller), src/005a3734.cpp (the row) |

Every grade in this chunk ran with `FABLE_HDR_OVERLAY=$A/hdr` exported. This applies to `tools/gd.sh`, `gall.sh` and the c2prio
probes. To land the rows, merge both blocks into `rebuild/include/engine/CTCInventoryTrade.h` as they stand.

`src/005a8175.cpp` uses the existing repo opt-in `FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC` (chunk 892) and needs no change.
