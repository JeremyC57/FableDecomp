- 0x005A04F7 progress (grades ~22): v1 full draft = same size, 464 hunks (frame slots only + wanted sum order). Sharing one CText* subtitle var between the one-off subtitle and the per-entry loop (buy/wanted) -> 5 hunks; same-TU real body of GetMenuBranchFromDefIndex 0x0059B79A (auto_inline off) -> 2 hunks (frame exact).
- Remaining: wanted title sum loads 1F4,1EC,1F0 v retail 1F0,1F4,1EC. Insensitive to: sum operand order (all sections), switch case order, ++/+=1/post++, earlier TU function touching the members.
- grades ~45: wanted sum rotation is a global-count effect: removing the sell UI_TRADING_SELL_TEXT SetValue call, the buy '|| !NotInStock.empty()' or the NotInStock add loop flips it to retail's order; removing AddChild(sellSubtitle) or code after the sum does not. Neutral spellings tried w/o effect: ++/post++, const_iterator, dead local, named total, empty() v begin!=end.

## 0x005A04F7 CTCInventoryTrade::ConstructNormalShopList -- RELOCATION_MATCH (~65 grades incl. probes)
- Matched source: src/CTCInventoryTrade_ConstructNormalShopList_005a04f7.cpp (payload.json / oracle.tsv written by addmatch.py;
  test/005a04f7_test.cpp PASS under vt.py; strict-header regrade RELOCATION_MATCH). Pragma optimize("s",on) only, no flags.
- What made it match: (1) first full draft from OUTLINE was already instruction-identical (same 9469 bytes, 464 frame-slot
  hunks); (2) buy/wanted one-off subtitle CText* reused as the per-entry loop's CText* (one frame node) -> 5 hunks;
  (3) same-TU REAL body of GetMenuBranchFromDefIndex 0x0059B79A (ported onto the shared headers, auto_inline(off)) so c2 knows
  `type` is only written through the reference -> `type` shares `branch`'s slot -> frame exact; (4) the 3-term caption sums
  are reassociated by c2 in an order set by a count of preceding temporaries (not by source operand order; sum permutations,
  switch spellings, case order, ++/post++ had no effect; removing any call-result temporary flips it): BUY's one-off
  `buySubtitle->SetName(BUY_SUBTITLE)` through CCountedPointer::operator-> (one more temp) gives retail's wanted order.
- Headers: hdr/engine/CTCInventoryTrade.h opt-in FABLE_RETAIL_CTCINVENTORYTRADE_SUBTITLES (private ConstructSubtitles decl).
  Macros used from repo headers: the 0x005A5B31 set + DEF_CLASS_BASE, CDEFPOINTER_RELEASE_POINTEE, CTCINVENTORYTRADE_SHOPLISTS,
  CTCINVENTORYTRADE_WANTEDLIST, CTCSHOP_NAME, CDEFSTRINGTABLE, CRESOURCELIST, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER,
  CCOUNTEDPOINTER_CALLED_CTOR, CTCINVENTORYITEM(+_CATEGORY).
- Same-TU real bodies in the row file: GetDef<T> (as 0x005A29F4), CBasicString<T>::Compare / operator== (as 0x005A5B31),
  GetMenuBranchFromDefIndex 0x0059B79A (landed with a local model; re-spelled here over CComponent::FindChild /
  CTCInventoryItem::GetInventoryCategory 0x005D97B0 / GetUIInventoryCategory 0x005D96F0, the landed row calls the latter
  "GetInventoryTabState").
- Name notes: FableWin.pdb spells GetMenuBranchFromDefIndex PRIVATE (`?GetMenuBranchFromDefIndex@CTCInventoryTrade@@AAEPAVCComponent@NUISystem@@JPAV23@AAW4EItem_Menu_Type@1@@Z`);
  the shared header already declares it in its private console-helper block (consistent). Local names subtitleTextBuy / subtitleTextWanted /
  unusedComponent / sellActions are descriptive.
- Lesson: a 3-operand member sum whose load order ignores the source order is c2 reassociation keyed by a running count of
  temporaries: find the section where one call-result temp more/less flips only the residue sum (here BUY's SetName).
