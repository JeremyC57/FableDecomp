# Landing 2026-11-09a: chunk 891 (CTCInventoryTrade PC list builders, Opus high), 1 of 3

0x005A5B31 CTCInventoryTrade::ConstructActualWantedInventoryPC (3,128 bytes, plain `ret` at 0x005A6768, next function follows with no
padding): RELOCATION_MATCH + PASS (VC7.1 and g++), no flags. Hand-back: `pending/2026-11-08_linux_hand891/`.

- Name: Ego_r.pdb + FableWin.pdb `?ConstructActualWantedInventoryPC@CTCInventoryTrade@@AAEXXZ`.
- What it does: asks the shop for its wanted items, builds the PC "wanted" goods list under the WANTED menu entry (frame, description
  viewport, title), one sub list per menu branch holding an entry per wanted item, appends branch and category counts to the labels,
  adds descriptions and subtitles, shows the hero's gold and, for an ordinary shop, refreshes the entry amounts.
- Devices: file-level `#pragma optimize("s", on)` (retail's biased frame); same-TU bodies of CBasicString<char>::Compare 0x00411570 /
  operator==(const char*) 0x004115A0 under `#pragma auto_inline(off)` (else VC7.1 reloads the string pointer after each name test).
- Headers (additive opt-ins, hdrcheck default path unchanged): engine/CTCShop.h `FABLE_RETAIL_CTCSHOP_WANTED`,
  engine/CTCInventoryTrade.h `FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY` (the three builders + AttachToFrame, AddWantedItemToListPC,
  AddSellItemToListPC, InitialiseSellableItems, ConstructSubtitlesPC; declarations only).
- Header debt: a local `class CThingPlayerCreature : public CThing { };` as in the landed sibling 0x005A8C66 (the engine header needs
  the retail creature-class chain).
- Names: labels `ghidra_out/labels_landing_2026-11-09a.tsv`; 3 respellings queued as land0809a (AddWantedItemToListPC 0x0059D270,
  CBasicString<char>::Compare 0x00411570 landed as strcmp_signed, the map<CComponent*,long> dtor 0x004DD1C4).
- Parked: Sell 0x005A4CDD DIFFER(3664v3668), one loop-head duplication at 0x005A4E49; Buy 0x005AC10E DIFFER(3538v3532), loop rotation
  at 0x005AC189 + the empty() operand order at 0x005AC373.
